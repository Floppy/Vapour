//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileAME.cpp - 17/06/2000 - James Smith
//	AME import filter implementation
//
// $Id: AvatarFileAME.cpp,v 1.7 2000/08/21 23:07:49 waz Exp $
//

#include "stdafx.h"

#include "VAL.h"
#include "AvatarFileAME.h"
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


///////////////////
// CAvatarFileAME

CAvatarFileProxy<CAvatarFileAME> g_oAvatarProxyAME;

/////////////////////
// Member Functions

////////////////////
// Store Functions

CAvatarFileAME::CAvatarFileAME() {
} // CAvatarFileAME()

float CAvatarFileAME::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFileAME::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFileAME::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFileAME::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFileAME::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFileAME::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFileAME::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFileAME::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFileAME::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

CAvatar* CAvatarFileAME::Load(const char* pszFilename) const {
   // Setup the import progress dialog
	g_poVAL->SetProgressMax("AMELoad", 12);
   g_poVAL->StepProgress("AMELoad") ;
   // Start loading
   g_poVAL->SetProgressText("AMELoad", "Loading AvatarMe file");
   cAvatar AMeModel;
   CAvatar* pNewAvatar = NULL;
   if (AMeModel.Load(pszFilename) == 0) {
      g_poVAL->StepProgress("AMELoad") ;
      g_poVAL->SetProgressText("AMELoad", "Creating structures");
      pNewAvatar = new CAvatar(AMeModel.iTotalNumVertices,AMeModel.iTotalNumTriangles);
      bool bAllocError = false;
      if (pNewAvatar->Status() == CAvatar::AV_NOALLOC) {
         bAllocError = true;
      }
      else {         
			//Open up loading functions
			pNewAvatar->StartLoad();

         //Load info
         switch (AMeModel.cSex) {
         case 'm':
            pNewAvatar->SetSex(CAvatar::SEX_MALE);
            break;
         case 'f':
            pNewAvatar->SetSex(CAvatar::SEX_FEMALE);
            break;
         default:
            pNewAvatar->SetSex(CAvatar::SEX_UNKNOWN);
            break;
         }         
         switch (AMeModel.cAgeGroup) {
         case 'a':
            pNewAvatar->SetAge(CAvatar::AGE_ADULT);
            break;
         case 't':
            pNewAvatar->SetAge(CAvatar::AGE_TEEN);
            break;
         case 'c':
            pNewAvatar->SetAge(CAvatar::AGE_CHILD);
            break;
         default:
            pNewAvatar->SetAge(CAvatar::AGE_UNKNOWN);
            break;
         }

         // Calculate height
         double dScale = AMeModel.fHeight;
         if (dScale==0.0) dScale = AMeModel.fSGAHeight;
         dScale /= AMeModel.fSGAHeight;
         // Correct for fuji cameras, just to make prototype booth work. If it's not a child, we assume minimum height of 0.5
         if ((pNewAvatar->Age() != CAvatar::AGE_CHILD) && (dScale < 0.5)) dScale *= 3000.0/1280.0;
         // If it is, assume minimum of 0.25
         else if (dScale < 0.25) dScale *= 3000.0/1280.0;

         //Load textures into CAvatar
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Loading textures");
         int iNumTextures = AMeModel.iTotalNumTextures;
         for (int t=0; t<iNumTextures; t++) {
            CImage* pNewImage = new CImage(IT_RGB);
            if (pNewImage) {
               pNewImage->ImportAMETexture(AMeModel.TextureArr[t]);
               int iWidth = 0;
               int iHeight = 0;
               pNewImage->GetSize(iWidth, iHeight);
               // Find new size - lower power of 2 up to iMaxSize
               const int iMaxSize = 512;
               if (iWidth > iMaxSize) iWidth = iMaxSize;
               else {
                  int iMax = iMaxSize;
                  while ((iWidth & iMax) == 0) {
                     iMax = iMax>>1;
                  }
                  iWidth = iMax;
               }
               if (iHeight > iMaxSize) iHeight = iMaxSize;
               else {
                  int iMax = iMaxSize;
                  while ((iHeight & iMax) == 0) {
                     iMax = iMax>>1;
                  }
                  iHeight = iMax;
               }
               // Rescale texture
               pNewImage->Scale(iWidth,iHeight);
               // And add to avatar
               pNewAvatar->AddTexture(pNewImage);
            }
         }

         //Load all data into "current" fields, ready for unposing

         //Load vertices
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Loading vertices");
			int iNumVertices = pNewAvatar->NumVertices();
			for (int i=0; i<iNumVertices; i++) pNewAvatar->SetVertex(i,AMeModel.VerticesArr[i].fX*dScale,AMeModel.VerticesArr[i].fY*dScale,AMeModel.VerticesArr[i].fZ*dScale);

         //Load faces
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Loading faces");
		   int iNumFaces = pNewAvatar->NumFaces();
		   for (i=0; i<iNumFaces; i++) {
            pNewAvatar->SetFace(
               i,
               AMeModel.TrianglesArr[i].iVIndex[0],
               AMeModel.TrianglesArr[i].iVIndex[1],
               AMeModel.TrianglesArr[i].iVIndex[2],
               AMeModel.TrianglesArr[i].iTArrIndex,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[0]].fU,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[0]].fV,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[1]].fU,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[1]].fV,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[2]].fU,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[2]].fV
            );
		   }

         //Load body parts
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Loading body parts");
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
            pNewAvatar->SetCentre(bpJoint,AMeModel.JointsArr[ameJoint].fX*dScale,AMeModel.JointsArr[ameJoint].fY*dScale,AMeModel.JointsArr[ameJoint].fZ*dScale);
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
            // Associate faces
  			   int iNumFaces = AMeModel.NumTrianglesInBodyPart[amePart];
			   int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[amePart];
            pNewAvatar->AssociateTexture(bpPart,pFaces[iCurrentFace].m_iTextureNumber);
			   for (int j=0; j<iNumFaces; j++) {
				   pNewAvatar->AssociateFace(bpPart, iCurrentFace++);
			   }
         }
         // Associate skirt faces
  			int iNumBPFaces = AMeModel.NumTrianglesInBodyPart[bp_Skirt];
         if (iNumBPFaces != 0) {
			   int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[bp_Skirt];
            pNewAvatar->AssociateTexture(skirt,pFaces[iCurrentFace].m_iTextureNumber);
			   for (int j=0; j<iNumBPFaces; j++) {
				   pNewAvatar->AssociateFace(skirt, iCurrentFace++);
			   }
         }
         // Fix face & vertex associations
         pNewAvatar->FixAssociations();
         // Associate torso texture with extra body parts
			int iTorsoTexture = pNewAvatar->TextureIndex(vl5);
			pNewAvatar->AssociateTexture(vt12,iTorsoTexture);
			pNewAvatar->AssociateTexture(vt8,iTorsoTexture);
			pNewAvatar->AssociateTexture(vt4,iTorsoTexture);
			pNewAvatar->AssociateTexture(l_acromioclavicular,iTorsoTexture);
			pNewAvatar->AssociateTexture(r_acromioclavicular,iTorsoTexture);

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
            pNewAvatar->SetCentre(bpSite,AMeModel.SitesArr[ameSite].fX*dScale,AMeModel.SitesArr[ameSite].fY*dScale,AMeModel.SitesArr[ameSite].fZ*dScale);
         }

         //Create skeleton structure
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Creating skeleton");
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
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Setting skeleton parameters");
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
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Unposing model");
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
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Repositioning model");
         pNewAvatar->UnTranslateDefaultModel();
			// Update the model
         g_poVAL->StepProgress("AMELoad") ;
         g_poVAL->SetProgressText("AMELoad", "Updating model");
			pNewAvatar->UpdateModel();
			// Tell the avatar that loading is complete
         g_poVAL->StepProgress("AMELoad") ;
			pNewAvatar->FinishLoad();
		}
		if (bAllocError) {
			delete pNewAvatar;
			pNewAvatar = NULL;
		}
	}
	return pNewAvatar;
} // Load(const char* pszFilename)

CAvatar* CAvatarFileAME::LoadSections(const char* pszFilename, int bsSections) const {
   // Start loading
   cAvatar AMeModel;
   CAvatar* pNewAvatar = NULL;
   if (AMeModel.Load(pszFilename) == 0) {      pNewAvatar = new CAvatar(AMeModel.iTotalNumVertices,AMeModel.iTotalNumTriangles);
      bool bAllocError = false;
      if (pNewAvatar->Status() == CAvatar::AV_NOALLOC) {
         bAllocError = true;
      }
      else {         
			//Open up loading functions
			pNewAvatar->StartLoad();

         //Load info
         switch (AMeModel.cSex) {
         case 'm':
            pNewAvatar->SetSex(CAvatar::SEX_MALE);
            break;
         case 'f':
            pNewAvatar->SetSex(CAvatar::SEX_FEMALE);
            break;
         default:
            pNewAvatar->SetSex(CAvatar::SEX_UNKNOWN);
            break;
         }         
         switch (AMeModel.cAgeGroup) {
         case 'a':
            pNewAvatar->SetAge(CAvatar::AGE_ADULT);
            break;
         case 't':
            pNewAvatar->SetAge(CAvatar::AGE_TEEN);
            break;
         case 'c':
            pNewAvatar->SetAge(CAvatar::AGE_CHILD);
            break;
         default:
            pNewAvatar->SetAge(CAvatar::AGE_UNKNOWN);
            break;
         }

         // Calculate height
         double dScale = AMeModel.fHeight;
         if (dScale==0.0) dScale = AMeModel.fSGAHeight;
         dScale /= AMeModel.fSGAHeight;
         // Correct for fuji cameras, just to make prototype booth work. If it's not a child, we assume minimum height of 0.5
         if ((pNewAvatar->Age() != CAvatar::AGE_CHILD) && (dScale < 0.5)) dScale *= 3000.0/1280.0;
         // If it is, assume minimum of 0.25
         else if (dScale < 0.25) dScale *= 3000.0/1280.0;

         //Load all data into "current" fields, ready for unposing

         //Load vertices
			int iNumVertices = pNewAvatar->NumVertices();
			for (int i=0; i<iNumVertices; i++) pNewAvatar->SetVertex(i,AMeModel.VerticesArr[i].fX*dScale,AMeModel.VerticesArr[i].fY*dScale,AMeModel.VerticesArr[i].fZ*dScale);

         //Load faces
		   int iNumFaces = pNewAvatar->NumFaces();
		   for (i=0; i<iNumFaces; i++) {
            pNewAvatar->SetFace(
               i,
               AMeModel.TrianglesArr[i].iVIndex[0],
               AMeModel.TrianglesArr[i].iVIndex[1],
               AMeModel.TrianglesArr[i].iVIndex[2],
               AMeModel.TrianglesArr[i].iTArrIndex,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[0]].fU,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[0]].fV,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[1]].fU,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[1]].fV,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[2]].fU,
               AMeModel.TexelsArr[AMeModel.TrianglesArr[i].iTIndex[2]].fV
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
            pNewAvatar->SetCentre(bpJoint,AMeModel.JointsArr[ameJoint].fX*dScale,AMeModel.JointsArr[ameJoint].fY*dScale,AMeModel.JointsArr[ameJoint].fZ*dScale);
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
            // Associate faces
  			   int iNumFaces = AMeModel.NumTrianglesInBodyPart[amePart];
			   int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[amePart];
            pNewAvatar->AssociateTexture(bpPart,pFaces[iCurrentFace].m_iTextureNumber);
			   for (int j=0; j<iNumFaces; j++) {
				   pNewAvatar->AssociateFace(bpPart, iCurrentFace++);
			   }
         }
         // Associate skirt faces
  			int iNumBPFaces = AMeModel.NumTrianglesInBodyPart[bp_Skirt];
         if (iNumBPFaces != 0) {
			   int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[bp_Skirt];
            pNewAvatar->AssociateTexture(skirt,pFaces[iCurrentFace].m_iTextureNumber);
			   for (int j=0; j<iNumBPFaces; j++) {
				   pNewAvatar->AssociateFace(skirt, iCurrentFace++);
			   }
         }
         // Fix face & vertex associations
         pNewAvatar->FixAssociations();
         // Associate torso texture with extra body parts
         int iTorsoTexture = pNewAvatar->TextureIndex(vl5);
         pNewAvatar->AssociateTexture(vt12,iTorsoTexture);
         pNewAvatar->AssociateTexture(vt8,iTorsoTexture);
         pNewAvatar->AssociateTexture(vt4,iTorsoTexture);
         pNewAvatar->AssociateTexture(l_acromioclavicular,iTorsoTexture);
         pNewAvatar->AssociateTexture(r_acromioclavicular,iTorsoTexture);

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
            pNewAvatar->SetCentre(bpSite,AMeModel.SitesArr[ameSite].fX*dScale,AMeModel.SitesArr[ameSite].fY*dScale,AMeModel.SitesArr[ameSite].fZ*dScale);
         }         

         //Load textures into CAvatar
         int iNumTextures = AMeModel.iTotalNumTextures;
         bool* bLoadTexture = new bool[iNumTextures];
         for (i=0; i<iNumTextures; i++) bLoadTexture[i] = false;
         if (bsSections & BS_HEAD) {
            bLoadTexture[pNewAvatar->TextureIndex(skullbase)] = true;
         }
         if (bsSections & BS_TORSO) {
            bLoadTexture[pNewAvatar->TextureIndex(vl5)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(vc7)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(l_shoulder)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(l_elbow)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(l_wrist)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(r_shoulder)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(r_elbow)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(r_wrist)] = true;
         }
         if (bsSections & BS_LEGS) {
            bLoadTexture[pNewAvatar->TextureIndex(sacroiliac)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(l_hip)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(l_knee)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(l_ankle)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(r_hip)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(r_knee)] = true;
            bLoadTexture[pNewAvatar->TextureIndex(r_ankle)] = true;
         }

         for (int t=0; t<iNumTextures; t++) {
            if (bLoadTexture[t] == true) {
               CImage* pNewImage = new CImage(IT_RGB);
               if (pNewImage) {
                  pNewImage->ImportAMETexture(AMeModel.TextureArr[t]);
                  int iWidth = 0;
                  int iHeight = 0;
                  pNewImage->GetSize(iWidth, iHeight);
                  // Find new size - lower power of 2 up to iMaxSize
                  const int iMaxSize = 512;
                  if (iWidth > iMaxSize) iWidth = iMaxSize;
                  else {
                     int iMax = iMaxSize;
                     while ((iWidth & iMax) == 0) {
                        iMax = iMax>>1;
                     }
                     iWidth = iMax;
                  }
                  if (iHeight > iMaxSize) iHeight = iMaxSize;
                  else {
                     int iMax = iMaxSize;
                     while ((iHeight & iMax) == 0) {
                        iMax = iMax>>1;
                     }
                     iHeight = iMax;
                  }
                  // Rescale texture
                  pNewImage->Scale(iWidth,iHeight);
                  // And add to avatar
                  pNewAvatar->AddTexture(pNewImage);
               }
            }
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
         pNewAvatar->SetJointLimit(l_hip,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(l_knee,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(l_ankle,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(r_hip,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(r_knee,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(r_ankle,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(vl5,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(vt12,0,0,0,0,0,0);
         pNewAvatar->SetJointLimit(vt8,0,0,0,0,0,0);
         pNewAvatar->SetJointLimit(vt4,0,0,0,0,0,0);
         pNewAvatar->SetJointLimit(vc7,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(skullbase,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(l_acromioclavicular,0,0,0,0,0,0);
         pNewAvatar->SetJointLimit(l_shoulder,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(l_elbow,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(l_wrist,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(r_acromioclavicular,0,0,0,0,0,0);
         pNewAvatar->SetJointLimit(r_shoulder,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(r_elbow,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
         pNewAvatar->SetJointLimit(r_wrist,V_PI,V_PI,V_PI,V_MINUS_PI,V_MINUS_PI,V_MINUS_PI);
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
			// Tell the avatar that loading is complete
			pNewAvatar->FinishLoad((~bsSections) & BS_ALL);
		}
		if (bAllocError) {
			delete pNewAvatar;
			pNewAvatar = NULL;
		}
	}
	return pNewAvatar;
}

/*int CAvatarFileAME::LoadBP(int bsSections, const char* pszFilename, CAvatar* pAvatar) const {
   int iRetVal = 1;
   // Setup the import progress dialog
   CProgressDialog dlgProgress;
   dlgProgress.Setup(IDB_AVATARME,12);
   g_poVAL->StepProgress("AMELoad") ;
   // Start loading
   g_poVAL->SetProgressText("AMELoad", "Loading AvatarMe file");
   cAvatar AMeModel;
   if (AMeModel.Load(pszFilename) == 0) {
      CAvatarPose poOldPose = pAvatar->ExportPose();
      pAvatar->ResetPose();
      pAvatar->UpdateModel();
      pAvatar->EnableAccess();
      SBodyPart* pBodyParts = pAvatar->BodyParts();
      SPoint3D* pVertices = pAvatar->Vertices();
      STriFace* pFaces = pAvatar->Faces();
      if ((bsSections & BSHEAD) && iRetVal==1) {
         // Load head
         const int iNumParts = 1;
         BodyPart bpAvatarParts[] = {skullbase};
         enumJointID bpFileParts[] = {j_Head};
         for (int i=0; i<iNumParts; i++) {
            BodyPart bpAvatar = bpAvatarParts[i];
            enumJointID bpFile = bpFileParts[i];
            if ((AMeModel.NumVerticesInBodyPart[bpFile] != pBodyParts[bpAvatar].m_iNumVertices) ||
               (AMeModel.NumTrianglesInBodyPart[bpFile] != pBodyParts[bpAvatar].m_iNumFaces))
               iRetVal = 0;
         }
         if (AMeModel.iTotalNumTextures != pAvatar->NumTextures()) iRetVal = 0;
         //CVector3D vecSkullbase(pBodyParts[skullbase].m_pntCurrentCentre);
         //CVector3D vecNewSkullbase(AMeModel.JointsArr[j_Head].fX,AMeModel.JointsArr[j_Head].fY,AMeModel.JointsArr[j_Head].fZ);
         //CVector3D vecOffset = vecSkullbase - vecNewSkullbase;
         //double dX, dY, dZ;
         //vecOffset.ToDouble(dX,dY,dZ);
         for (i=0; i<iNumParts && iRetVal==1; i++) {
            BodyPart bpAvatar = bpAvatarParts[i];
            enumJointID bpFile = bpFileParts[i];
			   int iNumFacesInBodyPart = AMeModel.NumTrianglesInBodyPart[bpFile];
            CImage* pNewImage = new CImage(IT_RGB);
            if (pNewImage && (iNumFacesInBodyPart!=0)) {
			      int iFace = AMeModel.FirstTriangleOfBodyPart[bpFile];
               pNewImage->ImportAMETexture(AMeModel.TextureArr[AMeModel.TrianglesArr[iFace].iTArrIndex]);
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
               // And add to avatar
               pAvatar->ReplaceTexture(pAvatar->TextureIndex(bpAvatar),pNewImage);
            }
            else delete pNewImage;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[0] = AMeModel.JointsArr[bpFile].fX;//+dX;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[1] = AMeModel.JointsArr[bpFile].fY;//+dY;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[2] = AMeModel.JointsArr[bpFile].fZ;//+dZ;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[0] = AMeModel.JointsArr[bpFile].fX;//+dX;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[1] = AMeModel.JointsArr[bpFile].fY;//+dY;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[2] = AMeModel.JointsArr[bpFile].fZ;//+dZ;
			   int iNumVerticesInBodyPart = AMeModel.NumVerticesInBodyPart[bpFile];
			   int iCurrentVertex = AMeModel.FirstVertexOfBodyPart[bpFile];
            int iLastVertex = iCurrentVertex + iNumVerticesInBodyPart;
			   while (iCurrentVertex<iLastVertex) {
				   (pVertices[iCurrentVertex]).m_dComponents[0] = AMeModel.VerticesArr[iCurrentVertex].fX;//+dX;
				   (pVertices[iCurrentVertex]).m_dComponents[1] = AMeModel.VerticesArr[iCurrentVertex].fY;//+dY;
				   (pVertices[iCurrentVertex]).m_dComponents[2] = AMeModel.VerticesArr[iCurrentVertex].fZ;//+dZ;
				   iCurrentVertex++;
			   }
			   int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[bpFile];
            int iLastFace = iCurrentFace + iNumFacesInBodyPart;
			   while (iCurrentFace<iLastFace) {
               (pFaces[iCurrentFace]).m_sTexCoords[0].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[0]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[0].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[0]].fV;
               (pFaces[iCurrentFace]).m_sTexCoords[1].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[1]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[1].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[1]].fV;
               (pFaces[iCurrentFace]).m_sTexCoords[2].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[2]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[2].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[2]].fV;
				   iCurrentFace++;
            }
         }
         const int iNumSites = 1;
         BodyPart bpAvatarSites[] = {skull_tip};
         enumSiteID bpFileSites[] = {s_skull_tip};
         for (int s=0; s<iNumSites && iRetVal==1; s++) {
            BodyPart bpAvatar = bpAvatarSites[s];
            enumSiteID bpFile = bpFileSites[s];
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[0] = AMeModel.SitesArr[bpFile].fX;//+dX;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[1] = AMeModel.SitesArr[bpFile].fY;//+dY;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[2] = AMeModel.SitesArr[bpFile].fZ;//+dZ;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[0] = AMeModel.SitesArr[bpFile].fX;//+dX;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[1] = AMeModel.SitesArr[bpFile].fY;//+dY;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[2] = AMeModel.SitesArr[bpFile].fZ;//+dZ;
         }
      }
      if ((bsSections & BSTORSO) && iRetVal==1) {
         // Load torso and arms
         const int iNumParts = 8;
         BodyPart bpAvatarParts[] = {vl5, vc7, l_shoulder, l_elbow, l_wrist, r_shoulder, r_elbow, r_wrist};
         enumJointID bpFileParts[] = {j_Torso, j_Neck, j_LeftUpperArm, j_LeftForeArm, j_LeftHand, j_RightUpperArm, j_RightForeArm, j_RightHand};
         for (int i=0; i<iNumParts; i++) {
            BodyPart bpAvatar = bpAvatarParts[i];
            enumJointID bpFile = bpFileParts[i];
            if ((AMeModel.NumVerticesInBodyPart[bpFile] != pBodyParts[bpAvatar].m_iNumVertices) ||
               (AMeModel.NumTrianglesInBodyPart[bpFile] != pBodyParts[bpAvatar].m_iNumFaces))
               iRetVal = 0;
         }
         if (AMeModel.iTotalNumTextures != pAvatar->NumTextures()) iRetVal = 0;
			pBodyParts[root].m_pntDefaultCentre.m_dComponents[0] = AMeModel.JointsArr[j_Start].fX;
			pBodyParts[root].m_pntDefaultCentre.m_dComponents[1] = AMeModel.JointsArr[j_Start].fY;
			pBodyParts[root].m_pntDefaultCentre.m_dComponents[2] = AMeModel.JointsArr[j_Start].fZ;
			pBodyParts[root].m_pntCurrentCentre.m_dComponents[0] = AMeModel.JointsArr[j_Start].fX;
			pBodyParts[root].m_pntCurrentCentre.m_dComponents[1] = AMeModel.JointsArr[j_Start].fY;
			pBodyParts[root].m_pntCurrentCentre.m_dComponents[2] = AMeModel.JointsArr[j_Start].fZ;
         for (i=0; i<iNumParts && iRetVal==1; i++) {
            BodyPart bpAvatar = bpAvatarParts[i];
            enumJointID bpFile = bpFileParts[i];
			   int iNumFacesInBodyPart = AMeModel.NumTrianglesInBodyPart[bpFile];
            CImage* pNewImage = new CImage(IT_RGB);
            if (pNewImage && (iNumFacesInBodyPart!=0)) {
			      int iFace = AMeModel.FirstTriangleOfBodyPart[bpFile];
               pNewImage->ImportAMETexture(AMeModel.TextureArr[AMeModel.TrianglesArr[iFace].iTArrIndex]);
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
               // And add to avatar
               pAvatar->ReplaceTexture(pAvatar->TextureIndex(bpAvatar),pNewImage);
            }
            else delete pNewImage;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[0] = AMeModel.JointsArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[1] = AMeModel.JointsArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[2] = AMeModel.JointsArr[bpFile].fZ;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[0] = AMeModel.JointsArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[1] = AMeModel.JointsArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[2] = AMeModel.JointsArr[bpFile].fZ;
			   int iNumVerticesInBodyPart = AMeModel.NumVerticesInBodyPart[bpFile];
			   int iCurrentVertex = AMeModel.FirstVertexOfBodyPart[bpFile];
            int iLastVertex = iCurrentVertex + iNumVerticesInBodyPart;
			   while (iCurrentVertex<iLastVertex) {
				   (pVertices[iCurrentVertex]).m_dComponents[0] = AMeModel.VerticesArr[iCurrentVertex].fX;
				   (pVertices[iCurrentVertex]).m_dComponents[1] = AMeModel.VerticesArr[iCurrentVertex].fY;
				   (pVertices[iCurrentVertex]).m_dComponents[2] = AMeModel.VerticesArr[iCurrentVertex].fZ;
				   iCurrentVertex++;
			   }
			   int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[bpFile];
            int iLastFace = iCurrentFace + iNumFacesInBodyPart;
			   while (iCurrentFace<iLastFace) {
               (pFaces[iCurrentFace]).m_sTexCoords[0].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[0]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[0].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[0]].fV;
               (pFaces[iCurrentFace]).m_sTexCoords[1].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[1]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[1].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[1]].fV;
               (pFaces[iCurrentFace]).m_sTexCoords[2].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[2]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[2].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[2]].fV;
				   iCurrentFace++;
            }
         }
         const int iNumSites = 2;
         BodyPart bpAvatarSites[] = {l_hand_tip, r_hand_tip};
         enumSiteID bpFileSites[] = {s_l_hand_tip, s_r_hand_tip};
         for (int s=0; s<iNumSites && iRetVal==1; s++) {
            BodyPart bpAvatar = bpAvatarSites[s];
            enumSiteID bpFile = bpFileSites[s];
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[0] = AMeModel.SitesArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[1] = AMeModel.SitesArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[2] = AMeModel.SitesArr[bpFile].fZ;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[0] = AMeModel.SitesArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[1] = AMeModel.SitesArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[2] = AMeModel.SitesArr[bpFile].fZ;
         }
         // Load spine centres
         CVector3D vecTop(pBodyParts[vc7].m_pntDefaultCentre);
         CVector3D vecBottom(pBodyParts[vl5].m_pntDefaultCentre);
         CVector3D vecSpineSegment = (vecTop - vecBottom) / 4;
			double dX, dY, dZ;
         // vt12
         (vecBottom+vecSpineSegment).ToDouble(dX,dY,dZ);
         pBodyParts[vt12].m_pntDefaultCentre.m_dComponents[0] = dX;
			pBodyParts[vt12].m_pntDefaultCentre.m_dComponents[1] = dY;
			pBodyParts[vt12].m_pntDefaultCentre.m_dComponents[2] = dZ;
			pBodyParts[vt12].m_pntCurrentCentre.m_dComponents[0] = dX;
			pBodyParts[vt12].m_pntCurrentCentre.m_dComponents[1] = dY;
			pBodyParts[vt12].m_pntCurrentCentre.m_dComponents[2] = dZ;
         // vt8
         (vecBottom+(vecSpineSegment*2)).ToDouble(dX,dY,dZ);
         pBodyParts[vt8].m_pntDefaultCentre.m_dComponents[0] = dX;
			pBodyParts[vt8].m_pntDefaultCentre.m_dComponents[1] = dY;
			pBodyParts[vt8].m_pntDefaultCentre.m_dComponents[2] = dZ;
			pBodyParts[vt8].m_pntCurrentCentre.m_dComponents[0] = dX;
			pBodyParts[vt8].m_pntCurrentCentre.m_dComponents[1] = dY;
			pBodyParts[vt8].m_pntCurrentCentre.m_dComponents[2] = dZ;
         //vt4
         (vecBottom+(vecSpineSegment*3)).ToDouble(dX,dY,dZ);
         pBodyParts[vt4].m_pntDefaultCentre.m_dComponents[0] = dX;
			pBodyParts[vt4].m_pntDefaultCentre.m_dComponents[1] = dY;
			pBodyParts[vt4].m_pntDefaultCentre.m_dComponents[2] = dZ;
			pBodyParts[vt4].m_pntCurrentCentre.m_dComponents[0] = dX;
			pBodyParts[vt4].m_pntCurrentCentre.m_dComponents[1] = dY;
			pBodyParts[vt4].m_pntCurrentCentre.m_dComponents[2] = dZ;
			// - the l_clavicle body part
			pBodyParts[l_acromioclavicular].m_pntDefaultCentre.m_dComponents[0] = AMeModel.JointsArr[j_l_clavicle].fX;
			pBodyParts[l_acromioclavicular].m_pntDefaultCentre.m_dComponents[1] = AMeModel.JointsArr[j_l_clavicle].fY;
			pBodyParts[l_acromioclavicular].m_pntDefaultCentre.m_dComponents[2] = AMeModel.JointsArr[j_l_clavicle].fZ;
			pBodyParts[l_acromioclavicular].m_pntCurrentCentre.m_dComponents[0] = AMeModel.JointsArr[j_l_clavicle].fX;
			pBodyParts[l_acromioclavicular].m_pntCurrentCentre.m_dComponents[1] = AMeModel.JointsArr[j_l_clavicle].fY;
			pBodyParts[l_acromioclavicular].m_pntCurrentCentre.m_dComponents[2] = AMeModel.JointsArr[j_l_clavicle].fZ;
			// - the r_clavicle body part
			pBodyParts[r_acromioclavicular].m_pntDefaultCentre.m_dComponents[0] = AMeModel.JointsArr[j_r_clavicle].fX;
			pBodyParts[r_acromioclavicular].m_pntDefaultCentre.m_dComponents[1] = AMeModel.JointsArr[j_r_clavicle].fY;
			pBodyParts[r_acromioclavicular].m_pntDefaultCentre.m_dComponents[2] = AMeModel.JointsArr[j_r_clavicle].fZ;
			pBodyParts[r_acromioclavicular].m_pntCurrentCentre.m_dComponents[0] = AMeModel.JointsArr[j_r_clavicle].fX;
			pBodyParts[r_acromioclavicular].m_pntCurrentCentre.m_dComponents[1] = AMeModel.JointsArr[j_r_clavicle].fY;
			pBodyParts[r_acromioclavicular].m_pntCurrentCentre.m_dComponents[2] = AMeModel.JointsArr[j_r_clavicle].fZ;
         // Unpose model
         pAvatar->AlignDefaultCentres(l_elbow,CVector3D(0.2,-1,0));
         pAvatar->AlignDefaultCentres(l_wrist,CVector3D(0,-1,0));
         pAvatar->AlignDefaultCentres(l_hand_tip,CVector3D(0,-1,0));
         pAvatar->AlignDefaultCentres(r_elbow,CVector3D(-0.2,-1,0));
         pAvatar->AlignDefaultCentres(r_wrist,CVector3D(0,-1,0));
         pAvatar->AlignDefaultCentres(r_hand_tip,CVector3D(0,-1,0));
         CAxisRotation rotLeftWrist(0,1,0,-1.57);
         CAxisRotation rotRightWrist(0,1,0,1.57);
         pBodyParts[l_wrist].m_rotCurrentRotation = pBodyParts[l_wrist].m_rotCurrentRotation.MergeOutside(rotLeftWrist);
         pBodyParts[r_wrist].m_rotCurrentRotation = pBodyParts[r_wrist].m_rotCurrentRotation.MergeOutside(rotRightWrist);
         pAvatar->CalculateDefaultVertices();
			// Remove any x-axis translation on the model
         pAvatar->UnTranslateDefaultModel();
			// Calculate all normals for current pose
			pAvatar->CalculateNormals(true);
			// Update the model
			pAvatar->UpdateModel();
      }
      if ((bsSections & BSLEGS) && iRetVal==1) {
         // Load pelvis and legs
         const int iNumParts = 7;
         BodyPart bpAvatarParts[] = {sacroiliac, l_hip, l_knee, l_ankle, r_hip, r_knee, r_ankle};
         enumJointID bpFileParts[] = {j_Pelvis, j_LeftThigh, j_LeftCalf, j_LeftFoot, j_RightThigh, j_RightCalf, j_RightFoot};         
         for (int i=0; i<iNumParts; i++) {
            BodyPart bpAvatar = bpAvatarParts[i];
            enumJointID bpFile = bpFileParts[i];
            if ((AMeModel.NumVerticesInBodyPart[bpFile] != pBodyParts[bpAvatar].m_iNumVertices) ||
               (AMeModel.NumTrianglesInBodyPart[bpFile] != pBodyParts[bpAvatar].m_iNumFaces))
               iRetVal = 0;
         }
         if (AMeModel.iTotalNumTextures != pAvatar->NumTextures()) iRetVal = 0;
			pBodyParts[root].m_pntDefaultCentre.m_dComponents[0] = AMeModel.JointsArr[j_Start].fX;
			pBodyParts[root].m_pntDefaultCentre.m_dComponents[1] = AMeModel.JointsArr[j_Start].fY;
			pBodyParts[root].m_pntDefaultCentre.m_dComponents[2] = AMeModel.JointsArr[j_Start].fZ;
			pBodyParts[root].m_pntCurrentCentre.m_dComponents[0] = AMeModel.JointsArr[j_Start].fX;
			pBodyParts[root].m_pntCurrentCentre.m_dComponents[1] = AMeModel.JointsArr[j_Start].fY;
			pBodyParts[root].m_pntCurrentCentre.m_dComponents[2] = AMeModel.JointsArr[j_Start].fZ;
         for (i=0; i<iNumParts && iRetVal==1; i++) {
            BodyPart bpAvatar = bpAvatarParts[i];
            enumJointID bpFile = bpFileParts[i];
			   int iNumFacesInBodyPart = AMeModel.NumTrianglesInBodyPart[bpFile];
            CImage* pNewImage = new CImage(IT_RGB);
            if (pNewImage && (iNumFacesInBodyPart!=0)) {
			      int iFace = AMeModel.FirstTriangleOfBodyPart[bpFile];
               pNewImage->ImportAMETexture(AMeModel.TextureArr[AMeModel.TrianglesArr[iFace].iTArrIndex]);
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
               // And add to avatar
               pAvatar->ReplaceTexture(pAvatar->TextureIndex(bpAvatar),pNewImage);
            }
            else delete pNewImage;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[0] = AMeModel.JointsArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[1] = AMeModel.JointsArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[2] = AMeModel.JointsArr[bpFile].fZ;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[0] = AMeModel.JointsArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[1] = AMeModel.JointsArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[2] = AMeModel.JointsArr[bpFile].fZ;
			   int iNumVerticesInBodyPart = AMeModel.NumVerticesInBodyPart[bpFile];
			   int iCurrentVertex = AMeModel.FirstVertexOfBodyPart[bpFile];
            int iLastVertex = iCurrentVertex + iNumVerticesInBodyPart;
			   while (iCurrentVertex<iLastVertex) {
				   (pVertices[iCurrentVertex]).m_dComponents[0] = AMeModel.VerticesArr[iCurrentVertex].fX;
				   (pVertices[iCurrentVertex]).m_dComponents[1] = AMeModel.VerticesArr[iCurrentVertex].fY;
				   (pVertices[iCurrentVertex]).m_dComponents[2] = AMeModel.VerticesArr[iCurrentVertex].fZ;
				   iCurrentVertex++;
			   }
			   int iCurrentFace = AMeModel.FirstTriangleOfBodyPart[bpFile];
            int iLastFace = iCurrentFace + iNumFacesInBodyPart;
			   while (iCurrentFace<iLastFace) {
               (pFaces[iCurrentFace]).m_sTexCoords[0].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[0]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[0].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[0]].fV;
               (pFaces[iCurrentFace]).m_sTexCoords[1].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[1]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[1].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[1]].fV;
               (pFaces[iCurrentFace]).m_sTexCoords[2].dU = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[2]].fU;
               (pFaces[iCurrentFace]).m_sTexCoords[2].dV = AMeModel.TexelsArr[AMeModel.TrianglesArr[iCurrentFace].iTIndex[2]].fV;
				   iCurrentFace++;
            }
         }
         const int iNumSites = 2;
         BodyPart bpAvatarSites[] = {l_foot_tip, r_foot_tip};
         enumSiteID bpFileSites[] = {s_l_middistal_tip, s_r_middistal_tip};
         for (int s=0; s<iNumSites && iRetVal==1; s++) {
            BodyPart bpAvatar = bpAvatarSites[s];
            enumSiteID bpFile = bpFileSites[s];
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[0] = AMeModel.SitesArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[1] = AMeModel.SitesArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntDefaultCentre.m_dComponents[2] = AMeModel.SitesArr[bpFile].fZ;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[0] = AMeModel.SitesArr[bpFile].fX;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[1] = AMeModel.SitesArr[bpFile].fY;
			   pBodyParts[bpAvatar].m_pntCurrentCentre.m_dComponents[2] = AMeModel.SitesArr[bpFile].fZ;
         }
         pAvatar->AlignDefaultCentres(l_knee,CVector3D(0,-1,0));
         pAvatar->AlignDefaultCentres(l_ankle,CVector3D(0,-1,0));
         pAvatar->AlignDefaultCentres(r_knee,CVector3D(0,-1,0));
         pAvatar->AlignDefaultCentres(r_ankle,CVector3D(0,-1,0));
         pAvatar->CalculateDefaultVertices();
			// Remove any x-axis translation on the model
         pAvatar->UnTranslateDefaultModel();
			// Calculate all normals for current pose
			pAvatar->CalculateNormals(true);
			// Update the model
			pAvatar->UpdateModel();
      }
      pAvatar->DisableAccess();
      pAvatar->ImportPose(poOldPose);
      pAvatar->UpdateModel();
   }
   else iRetVal = 0;
   return iRetVal;
} // LoadBP(BodySection bsSection, const char* pszFilename, CAvatar* pAvatar)*/

