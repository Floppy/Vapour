//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileGeneric.cpp - 04/10/2000 - James Smith
//	Generic avatar import filter implementation
//
// $Id: AvatarFileGeneric.cpp,v 1.2 2000/11/21 16:42:23 waz Exp $
//

#include "stdafx.h"

#include "VAL.h"
#include "AvatarFileGeneric.h"
#include "AvatarFileProxy.h"
#include "MathConstants.h"
#include "Image.h"

// AvatarMe SDK include
#include "cAvatar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAvatarFileProxy<CAvatarFileGeneric> g_oAvatarProxyGeneric;

//////////////////
// CAvatarFileGeneric

/////////////////////
// Member Functions

////////////////////
// Store Functions

CAvatarFileGeneric::CAvatarFileGeneric() {
} // CAvatarFileGeneric()

float CAvatarFileGeneric::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFileGeneric::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFileGeneric::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFileGeneric::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFileGeneric::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFileGeneric::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFileGeneric::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFileGeneric::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFileGeneric::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

CAvatar* CAvatarFileGeneric::Load(const char* pszFilename) const {
   // Start loading
   cAvatar AMeModel;
   CAvatar* pNewAvatar = NULL;
   AMeModel.LoadSGA();
   pNewAvatar = new CAvatar(AMeModel.iTotalNumVertices,AMeModel.iTotalNumTriangles);
   bool bAllocError = false;
   if (pNewAvatar->Status() == CAvatar::AV_NOALLOC) {
      bAllocError = true;
   }
   else {         
		//Open up loading functions
		pNewAvatar->StartLoad();
      //Load info
      pNewAvatar->SetSex(CAvatar::SEX_UNKNOWN);
      pNewAvatar->SetAge(CAvatar::AGE_UNKNOWN);
      //Load textures into CAvatar
      int iNumTextures = AMeModel.iTotalNumTextures;
      for (int t=0; t<iNumTextures; t++) {
         CImage* pNewImage = new CImage(IT_RGB);
         if (pNewImage != NULL) {
            pNewImage->ImportAMETexture(AMeModel.TextureArr[t]);
            int iWidth = 0;
            int iHeight = 0;
            pNewImage->GetSize(iWidth, iHeight);
            // Find new size - lower power of 2 up to 512
            if (iWidth > 512) iWidth = 512;
            else {
               int iMax = 512;
               while ((iWidth & iMax) == 0) {
                  iMax = iMax>>1;
               }
               iWidth = iMax;
            }
            if (iHeight > 512) iHeight = 512;
            else {
               int iMax = 512;
               while ((iHeight & iMax) == 0) {
                  iMax = iMax>>1;
               }
               iHeight = iMax;
            }
            // Rescale texture
            pNewImage->Scale(iWidth,iHeight);
            // Create new material and set texture
            int iNewMaterial = pNewAvatar->AddMaterial();
            if (iNewMaterial > -1) pNewAvatar->Material(iNewMaterial)->SetTexture(pNewImage);
            else delete pNewImage;
         }
      }
      
      //Load all data into "current" fields, ready for unposing
		
      //Load vertices
		int iNumVertices = pNewAvatar->NumVertices();
      SPoint3D pntVertex;
      for (int i=0; i<iNumVertices; i++) {
         pntVertex.m_dComponents[0] = AMeModel.VerticesArr[i].fX;
         pntVertex.m_dComponents[1] = AMeModel.VerticesArr[i].fY;
         pntVertex.m_dComponents[2] = AMeModel.VerticesArr[i].fZ;
         pNewAvatar->SetVertex(i,pntVertex);
      }

      //Load faces
		int iNumFaces = pNewAvatar->NumFaces();
		for (i=0; i<iNumFaces; i++) {
         pNewAvatar->SetFace(
            i,
            AMeModel.TrianglesArr[i].iVIndex[0],
            AMeModel.TrianglesArr[i].iVIndex[1],
            AMeModel.TrianglesArr[i].iVIndex[2],
            AMeModel.TrianglesArr[i].iTArrIndex,
            0,
            0,
            0,
            0,
            0,
            0
         );
		}

      //Load body parts
      const STriFace* pFaces = pNewAvatar->Faces();
		const SBodyPart* pBodyParts = pNewAvatar->BodyParts();

      // Load joint centres
      BodyPart bpJoints[] = {
         root,
         sacroiliac,
         l_hip,
         l_knee,
         l_ankle,
         r_hip,
         r_knee,
         r_ankle,
         vl5,
         vc7,
         skullbase,
         l_acromioclavicular,
         l_shoulder,
         l_elbow,
         l_wrist,
         r_acromioclavicular,
         r_shoulder,
         r_elbow,
         r_wrist
      };
      enumJointID ameJoints[] = {
         j_Start,
         j_Pelvis,
         j_LeftThigh,
         j_LeftCalf,
         j_LeftFoot,
         j_RightThigh,
         j_RightCalf,
         j_RightFoot,
         j_Torso,
         j_Neck,
         j_Head,
         j_l_clavicle,
         j_LeftUpperArm,
         j_LeftForeArm,
         j_LeftHand,
         j_r_clavicle,
         j_RightUpperArm,
         j_RightForeArm,
         j_RightHand,
      };
      for (i=0; i<19; i++) {
         BodyPart bpJoint = bpJoints[i];
         enumJointID ameJoint = ameJoints[i];
         // Load centre
         pNewAvatar->SetCentre(bpJoint,AMeModel.JointsArr[ameJoint].fX,AMeModel.JointsArr[ameJoint].fY,AMeModel.JointsArr[ameJoint].fZ);
      }
      // Setup extra joint positions - vt12, vt8, vt4
      CVector3D vecTop(pBodyParts[vc7].m_pntCurrentCentre);
      CVector3D vecBottom(pBodyParts[vl5].m_pntCurrentCentre);
      CVector3D vecSpineSegment = (vecTop - vecBottom) / 4;
		double dX, dY, dZ;
      // vt12
      (vecBottom+vecSpineSegment).ToDouble(dX,dY,dZ);
      pNewAvatar->SetCentre(vt12,dX,dY,dZ);
      // vt8
      (vecBottom+(vecSpineSegment*2)).ToDouble(dX,dY,dZ);
      pNewAvatar->SetCentre(vt8,dX,dY,dZ);
      //vt4
      (vecBottom+(vecSpineSegment*3)).ToDouble(dX,dY,dZ);
      pNewAvatar->SetCentre(vt4,dX,dY,dZ);

      // Load body part info
      BodyPart bpParts[] = {
         sacroiliac,
         l_hip,
         l_knee,
         l_ankle,
         r_hip,
         r_knee,
         r_ankle,
         vl5,
         vc7,
         skullbase,
         l_shoulder,
         l_elbow,
         l_wrist,
         r_shoulder,
         r_elbow,
         r_wrist
      };
      enumBodyPartID ameParts[] = {
         bp_Pelvis,
         bp_LeftThigh,
         bp_LeftCalf,
         bp_LeftFoot,
         bp_RightThigh,
         bp_RightCalf,
         bp_RightFoot,
         bp_Torso,
         bp_Neck,
         bp_Head,
         bp_LeftUpperArm,
         bp_LeftForeArm,
         bp_LeftHand,
         bp_RightUpperArm,
         bp_RightForeArm,
         bp_RightHand
      };
      // Associate vertices
      for (i=0; i<16; i++) {
         BodyPart bpPart = bpParts[i];
         enumBodyPartID amePart= ameParts[i];
         if (bpPart == vl5) {
            // Deal specially with the torso - we need to split it into 4
            double dYvl5  = pBodyParts[vl5].m_pntCurrentCentre.m_dComponents[1];
            double dYvc7  = pBodyParts[vc7].m_pntCurrentCentre.m_dComponents[1];
            double dYvt12 = pBodyParts[vt12].m_pntCurrentCentre.m_dComponents[1];
            double dYvt8  = pBodyParts[vt8].m_pntCurrentCentre.m_dComponents[1];
            double dYvt4  = pBodyParts[vt4].m_pntCurrentCentre.m_dComponents[1];
            // Associate vertices
			   int iNumVertices = AMeModel.NumVerticesInBodyPart[amePart];
			   int iCurrentVertex = AMeModel.FirstVertexOfBodyPart[amePart];
			   for (int j=0; j<iNumVertices; j++) {
               BodyPart bpNewPart = unknown;
               double dY = AMeModel.VerticesArr[iCurrentVertex].fY;
               if (dY > dYvt12) {
                  if (dY > dYvt8) {
                     if (dY > dYvt4) bpNewPart = vt4;
                     else bpNewPart = vt8;
                  }
                  else bpNewPart = vt12;
               }
               else bpNewPart = vl5;
               pNewAvatar->AssociateVertex(bpNewPart, iCurrentVertex++);
			   }
         }
         else {
            // Associate vertices
			   int iNumVertices = AMeModel.NumVerticesInBodyPart[amePart];
			   int iCurrentVertex = AMeModel.FirstVertexOfBodyPart[amePart];
			   for (int j=0; j<iNumVertices; j++) {
				   pNewAvatar->AssociateVertex(bpPart, iCurrentVertex++);
			   }
         }
      }
      // Fix vertex associations
      pNewAvatar->FixVertexAssociations();
      // Associate faces
      for (i=0; i<16; i++) {
         BodyPart bpPart = bpParts[i];
         enumBodyPartID amePart= ameParts[i];
  			int iNumFaces = AMeModel.NumTrianglesInBodyPart[amePart];
			int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[amePart];
         // Associate material
         pNewAvatar->AssociateMaterial(bpPart,pFaces[iCurrentFace].m_iMaterialNumber);
         // Associate faces
         if (bpPart == vl5) {
            // Deal specially with the torso - we need to split it into 4
            for (int j=0; j<iNumFaces; j++) {
               bpPart = unknown;
               for (int v=0; v<3; v++) {
                  BodyPart bpVertex = (BodyPart)pNewAvatar->GetVertexPart(pFaces[iCurrentFace].m_sVertices[v].m_iVertex);
                  if (bpVertex > bpPart) bpPart = bpVertex;
               }
      			pNewAvatar->AssociateFace(bpPart, iCurrentFace++);
   	   	}
         }
         else {
            // Associate faces & textures
            for (int j=0; j<iNumFaces; j++) {
   				pNewAvatar->AssociateFace(bpPart, iCurrentFace++);
	   		}
         }
      }
      // Associate skirt faces if skirt exists
  		int iNumBPFaces = AMeModel.NumTrianglesInBodyPart[bp_Skirt];
      if (iNumBPFaces != 0) {
			int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[bp_Skirt];
         pNewAvatar->AssociateMaterial(skirt,pFaces[iCurrentFace].m_iMaterialNumber);
			for (int j=0; j<iNumBPFaces; j++) {
				pNewAvatar->AssociateFace(skirt, iCurrentFace++);
			}
      }
      // Fix face associations
      pNewAvatar->FixFaceAssociations();
      // Associate torso material with extra body parts
		int iTorsoMaterial = pNewAvatar->MaterialIndex(vl5);
		pNewAvatar->AssociateMaterial(vt12,iTorsoMaterial);
		pNewAvatar->AssociateMaterial(vt8,iTorsoMaterial);
		pNewAvatar->AssociateMaterial(vt4,iTorsoMaterial);
		pNewAvatar->AssociateMaterial(l_acromioclavicular,iTorsoMaterial);
		pNewAvatar->AssociateMaterial(r_acromioclavicular,iTorsoMaterial);

      // Load site positions
      BodyPart bpSites[] = {
         l_foot_tip,
         r_foot_tip,
         skull_tip,
         l_hand_tip,
         r_hand_tip
      };
      enumSiteID ameSites[] = {
         s_l_middistal_tip,
         s_r_middistal_tip,
         s_skull_tip,
         s_l_hand_tip,
         s_r_hand_tip
      };
      for (i=0; i<5; i++) {
         BodyPart bpSite = bpSites[i];
         enumSiteID ameSite = ameSites[i];
         pNewAvatar->SetCentre(bpSite,AMeModel.SitesArr[ameSite].fX,AMeModel.SitesArr[ameSite].fY,AMeModel.SitesArr[ameSite].fZ);
      }

      //Create skeleton structure
      pNewAvatar->SetupSkeleton(root,unknown,sacroiliac);
      pNewAvatar->SetupSkeleton(sacroiliac,root,l_hip,r_hip,vl5);
		pNewAvatar->SetupSkeleton(l_hip,sacroiliac,l_knee);
		pNewAvatar->SetupSkeleton(l_knee,l_hip,l_ankle);
		pNewAvatar->SetupSkeleton(l_ankle,l_knee,l_foot_tip);
		pNewAvatar->SetupSkeleton(l_foot_tip,l_ankle);
		pNewAvatar->SetupSkeleton(r_hip,sacroiliac,r_knee);
		pNewAvatar->SetupSkeleton(r_knee,r_hip,r_ankle);
		pNewAvatar->SetupSkeleton(r_ankle,r_knee,r_foot_tip);
		pNewAvatar->SetupSkeleton(r_foot_tip,r_ankle);
		pNewAvatar->SetupSkeleton(vl5,sacroiliac,vt12);
		pNewAvatar->SetupSkeleton(vt12,vl5,vt8);
		pNewAvatar->SetupSkeleton(vt8,vt12,vt4);
		pNewAvatar->SetupSkeleton(vt4,vt8,vc7,l_acromioclavicular,r_acromioclavicular);
		pNewAvatar->SetupSkeleton(vc7,vt4,skullbase);
      pNewAvatar->SetupSkeleton(skullbase,vc7,skull_tip);
      pNewAvatar->SetupSkeleton(skull_tip,skullbase);
      pNewAvatar->SetupSkeleton(l_acromioclavicular,vt4,l_shoulder);
      pNewAvatar->SetupSkeleton(l_shoulder,l_acromioclavicular,l_elbow);
      pNewAvatar->SetupSkeleton(l_elbow,l_shoulder,l_wrist);
      pNewAvatar->SetupSkeleton(l_wrist,l_elbow,l_hand_tip);
      pNewAvatar->SetupSkeleton(l_hand_tip,l_wrist);
      pNewAvatar->SetupSkeleton(r_acromioclavicular,vt4,r_shoulder);
      pNewAvatar->SetupSkeleton(r_shoulder,r_acromioclavicular,r_elbow);
      pNewAvatar->SetupSkeleton(r_elbow,r_shoulder,r_wrist);
      pNewAvatar->SetupSkeleton(r_wrist,r_elbow,r_hand_tip);
      pNewAvatar->SetupSkeleton(r_hand_tip,r_wrist);
      // Set joint limits
      pNewAvatar->SetJointLimit(root,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
      pNewAvatar->SetJointLimit(sacroiliac,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
      pNewAvatar->SetJointLimit(l_hip,0.5,0.78,0.78,-2,-0.78,-0.34);
      pNewAvatar->SetJointLimit(l_knee,2.34,0,0,0,0,0);
      pNewAvatar->SetJointLimit(l_ankle,1.56,0.2,0.2,-0.2,-0.2,-0.2);
      pNewAvatar->SetJointLimit(r_hip,0.5,0.78,0.34,-2,-0.78,-0.78);
      pNewAvatar->SetJointLimit(r_knee,2.34,0,0,0,0,0);
      pNewAvatar->SetJointLimit(r_ankle,1.56,0.2,0.2,-0.2,-0.2,-0.2);
      pNewAvatar->SetJointLimit(vl5,0.2,0.2,0.2,-0.2,-0.2,-0.2);
      pNewAvatar->SetJointLimit(vt12,0.2,0.2,0.2,-0.2,-0.2,-0.2);
      pNewAvatar->SetJointLimit(vt8,0.2,0.2,0.2,-0.2,-0.2,-0.2);
      pNewAvatar->SetJointLimit(vt4,0.2,0.2,0.2,-0.2,-0.2,-0.2);
      pNewAvatar->SetJointLimit(vc7,0.1,0.1,0.1,0,-0.1,-0.1);
      pNewAvatar->SetJointLimit(skullbase,1.56,1.56,0.78,-0.78,-1.56,-0.78);
      pNewAvatar->SetJointLimit(l_acromioclavicular,0,0,0,0,0,0);
      pNewAvatar->SetJointLimit(l_shoulder,0.6,1.56,3,-3.14,-0.2,-0.78);
      pNewAvatar->SetJointLimit(l_elbow,0,0,0,0,-3.14,-2.35);
      pNewAvatar->SetJointLimit(l_wrist,0.6,0,1.56,-0.2,0,-1.3);
      pNewAvatar->SetJointLimit(r_acromioclavicular,0,0,0,0,0,0);
      pNewAvatar->SetJointLimit(r_shoulder,0.6,0.2,0.78,-3.14,-1.56,-3);
      pNewAvatar->SetJointLimit(r_elbow,0,3.14,2.35,0,0,0);
      pNewAvatar->SetJointLimit(r_wrist,0.6,0,1.3,-0.2,0,-1.56);
      // Set joint damping
      pNewAvatar->SetJointDamping(l_hip,1.0,0.5,1.0);
      pNewAvatar->SetJointDamping(r_hip,1.0,0.5,1.0);
      pNewAvatar->SetJointDamping(l_ankle,1.0,0.2,1.0);
      pNewAvatar->SetJointDamping(r_ankle,1.0,0.2,1.0);
      pNewAvatar->SetJointDamping(vl5,1.0,1.0,0.1);
      pNewAvatar->SetJointDamping(vt12,1.0,1.0,0.1);
      pNewAvatar->SetJointDamping(vt8,1.0,1.0,0.1);
      pNewAvatar->SetJointDamping(vt4,1.0,1.0,0.1);
      pNewAvatar->SetJointDamping(vc7,1.0,1.0,0.1);
      pNewAvatar->SetJointDamping(skullbase,1.0,1.0,0.1);
      // Unpose model
      pNewAvatar->AlignDefaultCentres(l_knee,CVector3D(0,-1,0));
      pNewAvatar->AlignDefaultCentres(l_ankle,CVector3D(0,-1,0));
      pNewAvatar->AlignDefaultCentres(r_knee,CVector3D(0,-1,0));
      pNewAvatar->AlignDefaultCentres(r_ankle,CVector3D(0,-1,0));
      pNewAvatar->AlignDefaultCentres(l_elbow,CVector3D(0.2,-1,0));
      pNewAvatar->AlignDefaultCentres(l_wrist,CVector3D(0,-1,0));
      pNewAvatar->AlignDefaultCentres(l_hand_tip,CVector3D(0,-1,0));
      pNewAvatar->AlignDefaultCentres(r_elbow,CVector3D(-0.2,-1,0));
      pNewAvatar->AlignDefaultCentres(r_wrist,CVector3D(0,-1,0));
      pNewAvatar->AlignDefaultCentres(r_hand_tip,CVector3D(0,-1,0));
      CAxisRotation rotLeftWrist(0,1,0,-1.57);
      CAxisRotation rotRightWrist(0,1,0,1.57);
      pNewAvatar->SetJointAngle(l_wrist,pBodyParts[l_wrist].m_rotCurrentRotation.MergeOutside(rotLeftWrist),false);
      pNewAvatar->SetJointAngle(r_wrist,pBodyParts[r_wrist].m_rotCurrentRotation.MergeOutside(rotRightWrist),false);
      pNewAvatar->CalculateDefaultVertices();
		// Remove any x-axis translation on the model
      pNewAvatar->UnTranslateDefaultModel();
		// Update the model
		pNewAvatar->UpdateModel();
      // Reset pose
      pNewAvatar->ResetPose();
      pNewAvatar->UpdateModel();
		// Tell the avatar that loading is complete
		pNewAvatar->FinishLoad();
 	}
	if (bAllocError) {
		delete pNewAvatar;
		pNewAvatar = NULL;
	}
	return pNewAvatar;
} // Load(const char* pszFilename)