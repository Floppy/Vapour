//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHalflife.cpp - 16/2/2000 - James Smith
//	Halflife export filter implementation
//
// $Id: AvatarFileHalflife.cpp,v 1.16 2000/11/21 16:44:31 waz Exp $
//

#include "stdafx.h"

#include "AvatarFileHalflife.h"
#include "AvatarFileProxy.h"
#include "Image.h"
#include "ImageFileStore.h"
#include "MathConstants.h"
#include "DList.h"

#include <float.h>
#include <math.h>
#include <direct.h>
#include <errno.h>

#include "SceneHLThumbnail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORDALIGN(x) if (x & 0x03) x = (x + 0x04) & 0xfffffffc

extern CImageFileStore g_oImageFileStore;

////////////////////////
// CAvatarFileHalflife

CAvatarFileProxy<CAvatarFileHalflife> g_oAvatarProxyHalflife;

/////////////////////
// Member Functions

////////////////////
// Store Functions

CAvatarFileHalflife::CAvatarFileHalflife() :
   m_pszModelname(NULL),
   m_pcTextureDataChunk(NULL),
   m_pcTextureIndexChunk(NULL),
   m_pTextureHeaderChunk(NULL),
   m_pcMeshDataChunk(NULL),
   m_pMeshHeaderChunk(NULL),
   m_pcModelDataChunk(NULL),
   m_pSeqGroupsChunk(NULL),
   m_pEventChunk(NULL),
   m_pHitboxChunk(NULL),
   m_pAttachmentChunk(NULL),
   m_pBoneControllerChunk(NULL),
   m_pBoneChunk(NULL)
{
      return;
} // CAvatarFileHalflife()

float CAvatarFileHalflife::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFileHalflife::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFileHalflife::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFileHalflife::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFileHalflife::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFileHalflife::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFileHalflife::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFileHalflife::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFileHalflife::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

FRESULT CAvatarFileHalflife::Save(const char* pszFilename, CAvatar* pAvatar) const {
   if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;
   
	FRESULT eResult = F_OK;
   
   // Work out filenames
   char pszTgtDir[256];
   strcpy(pszTgtDir,pszFilename);
   char* pszTemp = pszTgtDir;
   // Find model name
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   // Copy name
   m_pszModelname = strdup(pszTemp);
   // Remove extension if there is one
   pszTemp = strchr(m_pszModelname,'.');
   if (pszTemp != NULL) *pszTemp = '\0';
   // Find target directory name
   pszTemp = pszTgtDir;
   // Find last dir separator and terminate string
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   *pszTemp = (char)'\0';
   // Stick model name on the end of target dir
   strcat(pszTgtDir,m_pszModelname);
   strcat(pszTgtDir,"\\");
   // pszTgtDir is now the output directory!
   // Create a filename string (with space for an extension)
   char* pszBaseFilename;
   NEWBEGIN
   pszBaseFilename = new char[strlen(pszTgtDir) + strlen(m_pszModelname) + 5];
	NEWEND("CAvatarFileHalflife::Save - Base Filename Allocation")
   if (pszBaseFilename != NULL) {
      strcpy(pszBaseFilename,pszTgtDir);
      strcat(pszBaseFilename,m_pszModelname);
   }
   else return F_OUT_OF_MEMORY;
   int iBaseFilenameLength = strlen(pszBaseFilename);

   // Create output subdirectory
   int iResult = _mkdir(pszTgtDir);
   if ((iResult == -1) && (errno != EEXIST)) return F_DIR_ERROR;
   
   // Setup the export progress dialog
	g_poVAL->SetProgressMax("HLSave", 18 + pAvatar->NumMaterials());
   
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Saving MDL file");
   
   // Create filename
   strcpy(pszBaseFilename+iBaseFilenameLength,".mdl");
   // Save to output stream
   ofstream osMDLStream(pszBaseFilename,ios::out|ios::binary);
	if (!osMDLStream.fail()) {
   	eResult = Save(osMDLStream,pAvatar);
	}
	osMDLStream.close();

   // Create filename
   strcpy(pszBaseFilename+iBaseFilenameLength,".bmp");
   // Save the thumbnail bitmap
   SaveThumbnail(pszBaseFilename, pAvatar);

   // Finish up
   g_poVAL->StepProgress("HLSave");
   // Delete strings
   delete [] pszBaseFilename;
   free(m_pszModelname);
   m_pszModelname = NULL;
   // Done
   return eResult;
} // Save(const char* pszFilename, CAvatar* pAvatar)

FRESULT CAvatarFileHalflife::Save(ofstream& osOutputStream, CAvatar* pAvatar) const {
	if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;

   FRESULT eResult = F_OK; // Return value
   
   // Store old pose
   pAvatar->PushPose();
   pAvatar->ResetPose();
   pAvatar->UpdateModel();

   // Load required data
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Loading external data");

   // Open the data wedgie
   char pszWJEName[STR_SIZE] = "";
   strcpy(pszWJEName, g_poVAL->GetAppDir());
   strcat(pszWJEName, "hldata.wje");
   m_fsDataWJE.open(pszWJEName, ios::in|ios::binary|ios::nocreate);
   if (m_fsDataWJE.fail()) {
      Cleanup(pAvatar);
      return F_ERROR;
   }
   m_oWedgie.Open(&m_fsDataWJE);

   int iCurrentPos = 0;    // Temporary counter
   int i=0;                // predeclared loop variable
   int t=0;                // predeclared loop variable

   // Set model scale factor - all models will be the same size, 
   // as this is based on height. However, gamers prefer this :)
   const double dScaleFactor = 75.0 / pAvatar->Height();

   // Get avatar data pointers
   const SBodyPart* pBodyParts = pAvatar->BodyParts();
   const SPoint3D*  pVertices  = pAvatar->Vertices();
   const SPoint3D*  pNormals   = pAvatar->VertexNormals();
   const STriFace*  pFaces     = pAvatar->Faces();
   
   // Get avatar data
   const int iNumVertices = pAvatar->NumVertices();
   const int iNumFaces    = pAvatar->NumFaces();

   // Re-pose avatar
   CVector3D vecTarget(0,1,0);
   // Align hips with pelvis joint
   //pAvatar->AlignBodyPart(l_hip,CVector3D(1,0,0),false);
   //pAvatar->AlignBodyPart(r_hip,CVector3D(-1,0,0),false);
   //pAvatar->UpdateModel();
   // Point legs in vaguely the right direction
   pAvatar->SetJointAngle(l_hip,CAxisRotation(0,0,1,V_PI),false);
   pAvatar->SetJointAngle(r_hip,CAxisRotation(0,0,1,V_PI),false);
   pAvatar->UpdateModel();
   // Then make sure they're aligned precisely
   pAvatar->AlignBodyPart(l_knee,vecTarget,false);
   pAvatar->AlignBodyPart(r_knee,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(l_ankle,vecTarget,false);
   pAvatar->AlignBodyPart(r_ankle,vecTarget,false);
   // Line up spine exactly on Y axis
   //pAvatar->AlignBodyPart(vl5,vecTarget,false);
   //pAvatar->UpdateModel();
   //pAvatar->AlignBodyPart(vt12,vecTarget,false);
   //pAvatar->UpdateModel();
   //pAvatar->AlignBodyPart(vt8,vecTarget,false);
   //pAvatar->UpdateModel();
   //pAvatar->AlignBodyPart(vt4,vecTarget,false);
   //pAvatar->UpdateModel();
   //pAvatar->AlignBodyPart(vc7,vecTarget,false);
   //pAvatar->UpdateModel();
   //pAvatar->AlignBodyPart(skullbase,vecTarget,false);
   //pAvatar->UpdateModel();
   // Point arms in vaguely the right directions, right from the clavicles
   pAvatar->SetJointAngle(l_acromioclavicular,CAxisRotation(1,0,0,V_PI),false);
   pAvatar->SetJointAngle(r_acromioclavicular,CAxisRotation(1,0,0,-V_PI),false);
   pAvatar->SetJointAngle(l_wrist,CAxisRotation(0,-1,0,V_PI/2),false);
   pAvatar->SetJointAngle(r_wrist,CAxisRotation(0,-1,0,-V_PI/2),false);
   pAvatar->UpdateModel();   
   // Then align them precisely
   //pAvatar->AlignBodyPart(l_acromioclavicular,CVector3D(1,-1,0),false);
   //pAvatar->AlignBodyPart(r_acromioclavicular,CVector3D(-1,-1,0),false);
   //pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(l_shoulder,vecTarget,false);
   pAvatar->AlignBodyPart(r_shoulder,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(l_elbow,vecTarget,false);
   pAvatar->AlignBodyPart(r_elbow,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(l_wrist,vecTarget,false);
   pAvatar->AlignBodyPart(r_wrist,vecTarget,false);
   pAvatar->UpdateModel();
   // Twist entire model sideways
   pAvatar->SetJointAngle(sacroiliac,CAxisRotation(0,0,1,-V_PI/2),false);
   pAvatar->UpdateModel();

   //pAvatar->ResetPose();
   //pAvatar->UpdateModel();
   
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating header");

   // Create Header & fill in some fields
   SHalflifeHeader sHeader;
   // Set ID and version
   sHeader.iID = HL_MDL_MAGIC;
   sHeader.iVersion = HL_MDL_VERSION;
   // Model name
   char pcModelName[64] = "";
   sprintf(pcModelName, "%s\\%s.mdl", m_pszModelname, m_pszModelname);
   strncpy(sHeader.pszName,pcModelName,64);
   // Initialise length counter
   sHeader.iLength = 0;
   // Set the vectors and flags to 0.
   sHeader.vEyePosition[0]   = 0.0;
   sHeader.vEyePosition[1]   = 0.0;
   sHeader.vEyePosition[2]   = 0.0;
   sHeader.vMin[0]           = 0.0;
   sHeader.vMin[1]           = 0.0;
   sHeader.vMin[2]           = 0.0;
   sHeader.vMax[0]           = 0.0;
   sHeader.vMax[1]           = 0.0;
   sHeader.vMax[2]           = 0.0;
   sHeader.vBBMin[0]         = 0.0;
   sHeader.vBBMin[1]         = 0.0;
   sHeader.vBBMin[2]         = 0.0;
   sHeader.vBBMax[0]         = 0.0;
   sHeader.vBBMax[1]         = 0.0;
   sHeader.vBBMax[2]         = 0.0;
   sHeader.iFlags            = 0;   
   // Set sound stuff to 0
   sHeader.iSoundTable       = 0;
   sHeader.iSoundOffset      = 0;
   sHeader.iSoundGroups      = 0;
   sHeader.iSoundGroupOffset = 0;

   // Add header to file offset counter
   sHeader.iLength += sizeof(SHalflifeHeader);
	 	 
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating bones");

   // Prepare Skeleton
   CompressSkeleton(root,pAvatar);

   // Create Bone Chunk
   sHeader.iNumBones = m_vCompressedSkeletonMap.size();
   sHeader.iBoneChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pBoneChunk = new SHalflifeBone[sHeader.iNumBones];
	NEWEND("CAvatarFileHalflife::Save - Bone Chunk Allocation")
   if (m_pBoneChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }

   // Read generic bone data from external file
   int iHandle = m_oWedgie.Open("hlmdlbonedata.bin");
   if (iHandle > -1) {
	   unsigned int uDataSize = sizeof(SHalflifeBone) * sHeader.iNumBones;
	   unsigned int uRead = m_oWedgie.Read(iHandle, (unsigned char*)m_pBoneChunk, uDataSize);
      if (uRead != uDataSize) {
         Cleanup(pAvatar);
         return F_ERROR;
      }
   }
   else {
      Cleanup(pAvatar);
	   return F_ERROR;
   }/**/

   iCurrentPos = 0;
   // Enter bone data
   for (i=0; i<sHeader.iNumBones; i++) {
      // Set name
      BodyPart bpPart = (BodyPart)m_vCompressedSkeletonMap[i];
      strncpy(m_pBoneChunk[i].pszName,m_pszJointNames[bpPart],32);
      // Set Flags
      m_pBoneChunk[i].iFlags = 0;
      // Set parent reference
      BodyPart bpParent = (bpPart==l_acromioclavicular)||(bpPart==r_acromioclavicular) ? vc7 : pBodyParts[bpPart].m_bpParent;
      m_pBoneChunk[i].iParent = bpParent==unknown ? -1 : m_pReverseCompressedSkeletonMap[bpParent];
      // Get this joint centre
      CVector3D vecJoint(pBodyParts[bpPart].m_pntCurrentCentre);
      // Get dist from parent
      if (bpParent != unknown) {
         vecJoint -= CVector3D(pBodyParts[bpParent].m_pntCurrentCentre);
      }
      // Scale vector
      vecJoint *= dScaleFactor;
      // Write centre
      //m_pBoneChunk[i].vCentre[0] = vecJoint.Y();
      //m_pBoneChunk[i].vCentre[1] = vecJoint.Z();
      //m_pBoneChunk[i].vCentre[2] = vecJoint.X();
      m_pBoneChunk[i].vCentre[0] =  vecJoint.X();
      m_pBoneChunk[i].vCentre[1] =  vecJoint.Z();
      m_pBoneChunk[i].vCentre[2] = -vecJoint.Y();
      // Set small numbers to 0
      if (fabs(m_pBoneChunk[i].vCentre[0]) < 1e-8) m_pBoneChunk[i].vCentre[0] = 0;
      if (fabs(m_pBoneChunk[i].vCentre[1]) < 1e-8) m_pBoneChunk[i].vCentre[1] = 0;
      if (fabs(m_pBoneChunk[i].vCentre[2]) < 1e-8) m_pBoneChunk[i].vCentre[2] = 0;
      // Set bone rotation
      /*CEulerRotation oRot(pBodyParts[bpParent].m_rotCurrentRotation,XYZs);
      m_pBoneChunk[i].vRotation[0] = oRot.Angle(0);
      m_pBoneChunk[i].vRotation[1] = oRot.Angle(1);
      m_pBoneChunk[i].vRotation[2] = oRot.Angle(2);
      // Set bone scales
      m_pBoneChunk[i].vCentreScale[0]   = 0.01f;
      m_pBoneChunk[i].vCentreScale[1]   = 0.01f;
      m_pBoneChunk[i].vCentreScale[2]   = 0.01f;
      m_pBoneChunk[i].vRotationScale[0] = 0.01f;
      m_pBoneChunk[i].vRotationScale[1] = 0.01f;
      m_pBoneChunk[i].vRotationScale[2] = 0.01f;*/
      // Set bone controller links
      m_pBoneChunk[i].piBoneController[3] = 0xFFFFFFFF;
   }
   sHeader.iLength += sizeof(SHalflifeBone) * sHeader.iNumBones;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating bonecontrollers");

   // Create BoneController Chunk
   sHeader.iNumBoneControllers = 4;
   sHeader.iBoneControllerChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pBoneControllerChunk = new SHalflifeBoneController[sHeader.iNumBoneControllers];
	NEWEND("CAvatarFileHalflife::Save - Bone Controller Chunk Allocation")
   if (m_pBoneControllerChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   for (i=0; i<sHeader.iNumBoneControllers; i++) {
      // Set bone link
      m_pBoneControllerChunk[i].iBone  = m_pReverseCompressedSkeletonMap[vl5] + i;
      // Set bone's link to this controller
      m_pBoneChunk[m_pReverseCompressedSkeletonMap[vl5]+i].piBoneController[3] = i;
      // Set type = XR
      m_pBoneControllerChunk[i].iType  = 0x08;
      // Set range
      m_pBoneControllerChunk[i].fStart = -30;
      m_pBoneControllerChunk[i].fEnd   = 30;
      // Set rest value
      m_pBoneControllerChunk[i].iRest  = 0;
      // Set index
      m_pBoneControllerChunk[i].iIndex = i;
   }
   sHeader.iLength += sizeof(SHalflifeBoneController) * sHeader.iNumBoneControllers;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating attachments");

   // Create Attachments Chunk
   sHeader.iNumAttachments = 3;
   sHeader.iAttachmentChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pAttachmentChunk = new SHalflifeAttachment[sHeader.iNumAttachments];
	NEWEND("CAvatarFileHalflife::Save - Attachment Chunk Allocation")
   if (m_pAttachmentChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   const char* pszAttachmentNames[3]       = {"", "", ""};
   const float pszAttachmentPositions[3][3] = { {20.0, 2.0, 5.0 },
                                                {15.0, 1.5, 3.75},
                                                {30.0, 3.0, 7.5 } };
   for (i=0; i<sHeader.iNumAttachments; i++) {
      // Set bone
      m_pAttachmentChunk[i].iBone = m_pReverseCompressedSkeletonMap[r_wrist];
      // Set type
      m_pAttachmentChunk[i].iType = 0;
      // Set name
      strncpy(m_pAttachmentChunk[i].pszName,pszAttachmentNames[i],32);
      // Set position
      for (int j=0; j<3; j++) {
         m_pAttachmentChunk[i].vPosition[j] = pszAttachmentPositions[i][j];
      }
      // Zero other vectors
      m_pAttachmentChunk[i].vVector0[0] = 0.0;
      m_pAttachmentChunk[i].vVector0[1] = 0.0;
      m_pAttachmentChunk[i].vVector0[2] = 0.0;
      m_pAttachmentChunk[i].vVector1[0] = 0.0;
      m_pAttachmentChunk[i].vVector1[1] = 0.0;
      m_pAttachmentChunk[i].vVector1[2] = 0.0;
      m_pAttachmentChunk[i].vVector2[0] = 0.0;
      m_pAttachmentChunk[i].vVector2[1] = 0.0;
      m_pAttachmentChunk[i].vVector2[2] = 0.0;
   }
   sHeader.iLength += sizeof(SHalflifeAttachment) * sHeader.iNumAttachments;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating hitboxes");

   // Create Hitbox Chunk
   const int piHitGroups[] = {3,3,6,6,6,7,7,7,3,2,2,2,2,1,4,4,4,4,5,5,5,5};
   sHeader.iNumHitboxes = sHeader.iNumBones;
   sHeader.iHitboxChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pHitboxChunk = new SHalflifeHitbox[sHeader.iNumHitboxes];
	NEWEND("CAvatarFileHalflife::Save - Hitbox Chunk Allocation")
   if (m_pHitboxChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   for (i=0; i<sHeader.iNumHitboxes; i++) {
      // Write hitbox info
      m_pHitboxChunk[i].iBone  = i;
      m_pHitboxChunk[i].iGroup = piHitGroups[i];
      // Calc bounding box
      SPoint3D min, max;
      pAvatar->BoundingBox((BodyPart)m_vCompressedSkeletonMap[i],max,min);
      CVector3D vecMin(min);
      CVector3D vecMax(max);
      // Work out offsets from bone centre
      CVector3D vecCentre(pBodyParts[m_vCompressedSkeletonMap[i]].m_pntCurrentCentre);
      vecMin -= vecCentre;
      vecMax -= vecCentre;
      // Scale
      vecMin *= dScaleFactor;
      vecMax *= dScaleFactor;
      // Write minimum
      //m_pHitboxChunk[i].vMinimum[0] = vecMin.Y();
      //m_pHitboxChunk[i].vMinimum[1] = vecMin.Z();
      //m_pHitboxChunk[i].vMinimum[2] = vecMin.X();
      m_pHitboxChunk[i].vMinimum[0] =  vecMax.X();
      m_pHitboxChunk[i].vMinimum[1] =  vecMax.Z();
      m_pHitboxChunk[i].vMinimum[2] = -vecMin.Y();
      // Write maximum
      //m_pHitboxChunk[i].vMaximum[0] = vecMax.Y();
      //m_pHitboxChunk[i].vMaximum[1] = vecMax.Z();
      //m_pHitboxChunk[i].vMaximum[2] = vecMax.X();
      m_pHitboxChunk[i].vMaximum[0] =  vecMin.X();
      m_pHitboxChunk[i].vMaximum[1] =  vecMin.Z();
      m_pHitboxChunk[i].vMaximum[2] = -vecMax.Y();
   }
   sHeader.iLength += sizeof(SHalflifeHitbox) * sHeader.iNumHitboxes;

   // Create Animation Sequence Header Chunk
   sHeader.iNumSeqs = 77;
   sHeader.iLength += 0x2FE80; // offset in anim file of first seq desc
   sHeader.iSeqChunkOffset = sHeader.iLength;
   sHeader.iLength += (sHeader.iNumSeqs * sizeof(SHalflifeSequence));

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating events");

   // Create Event Chunk
   const int iNumEvents = 15; // 15 events defined
   // Declare event info - this comes from player_shared.qc
   const int piEventFrames[iNumEvents] = {
      9,    //die_simple
      13,   //die_backwards1
      15,   //die_backwards
      8,    //die_forwards
      22,   //headshot
      16,   //die_spin
      22,   //gutshot
      0,    //ref_shoot_onehanded
      0,    //crouch_shoot_onehanded
      0,    //ref_shoot_python
      0,    //crouch_shoot_python
      0,    //ref_shoot_shotgun
      0,    //crouch_shoot_shotgun
      0,    //ref_shoot_mp5
      0,    //crouch_shoot_mp5
   };
   const int piEvents[iNumEvents] = {
      2001, //die_simple
      2001, //die_backwards1
      2001, //die_backwards
      2001, //die_forwards
      2001, //headshot
      2001, //die_spin
      2001, //gutshot
      5011, //ref_shoot_onehanded
      5011, //crouch_shoot_onehanded
      5011, //ref_shoot_python
      5011, //crouch_shoot_python
      5021, //ref_shoot_shotgun
      5021, //crouch_shoot_shotgun
      5001, //ref_shoot_mp5
      5001, //crouch_shoot_mp5
   };
   const char* piEventOpts[iNumEvents] = {
      "",   //die_simple
      "",   //die_backwards1
      "",   //die_backwards
      "",   //die_forwards
      "",   //headshot
      "",   //die_spin
      "",   //gutshot
      "21", //ref_shoot_onehanded
      "21", //crouch_shoot_onehanded
      "31", //ref_shoot_python
      "31", //crouch_shoot_python
      "51", //ref_shoot_shotgun
      "51", //crouch_shoot_shotgun
      "40", //ref_shoot_mp5
      "40", //crouch_shoot_mp5
   };
   // Allocate memory
   NEWBEGIN
   m_pEventChunk = new SHalflifeEvent[iNumEvents];
	NEWEND("CAvatarFileHalflife::Save - Event Chunk Allocation")
   if (m_pEventChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   // Create event data chunk
   for (i=0; i<iNumEvents; i++) {
      // Copy frame number
      m_pEventChunk[i].iFrame = piEventFrames[i];
      // Copy event number
      m_pEventChunk[i].iEvent = piEvents[i];
      // Type is always 0
      m_pEventChunk[i].iType = 0;
      // Copy options - pad with 0 to make sure no junk gets through
      strncpy(m_pEventChunk[i].pcOptions,piEventOpts[i],64);
   }
   sHeader.iLength += (iNumEvents * sizeof(SHalflifeEvent));

   // Create Sequence Groups Chunk
   sHeader.iNumSeqGroups = 1;
   sHeader.iSeqGroupChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pSeqGroupsChunk = new SHalflifeSeqGroup[sHeader.iNumSeqGroups];
	NEWEND("CAvatarFileHalflife::Save - Sequence Groups Chunk Allocation")
   if (m_pSeqGroupsChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   else {
      for (i=0; i<sHeader.iNumSeqGroups; i++) {
         // Name
         strncpy(m_pSeqGroupsChunk[i].pszName,"default",32);
         // Filename
         strncpy(m_pSeqGroupsChunk[i].pszFilename,"",64);
         // Cache Pointer
         m_pSeqGroupsChunk[i].iCachePtr = 0;
         // Data
         m_pSeqGroupsChunk[i].iData = 0;
      }
   }
   sHeader.iLength += sizeof(SHalflifeSeqGroup) * sHeader.iNumSeqGroups;

   // Deal with Transition Chunk (but don't create anything)
   sHeader.iNumTransitions = 0;
   sHeader.iTransitionChunkOffset = sHeader.iLength;
   
   const int uFaceTexIndex = pAvatar->MaterialIndex(skullbase);

   // We create two textures - head and body.
   // Deal with head texture first: this is the easy bit...
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Converting head texture");
   // Copy texture out
   CImage imgFace(*(pAvatar->Material(uFaceTexIndex)->Texture()));
   // Scale...
   imgFace.Scale(HL_TEXTURE_WIDTH,HL_TEXTURE_HEIGHT);
   // flip...
   imgFace.Flip();
   // and convert to palette!
   imgFace.Convert(IT_PALETTE);

   unsigned int uNumTextures = pAvatar->NumMaterials();

   unsigned int* uFirstPixel = new unsigned int[uNumTextures];
   unsigned int* uImageHeight = new unsigned int[uNumTextures];

   // Now sort out body texture:
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating body texture");
   // Store torso tex index for future reference
   unsigned int uTorsoTexIndex = pAvatar->MaterialIndex(vl5);

   // Resize textures
   CImage** pImages = NULL;
   NEWBEGIN
   pImages = new CImage*[uNumTextures];
   NEWEND("CAvatarFileHalflife::Save - image pointer allocation");
   if (pImages == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   for (t=0; t<uNumTextures; t++) {
      pImages[t] = NULL;
   }
   unsigned int uTotalHeight = 0;
   for (t=0; t<uNumTextures; t++) {
      // If this is not an exempt texture
      if (t != uFaceTexIndex) {
         // Variables
         int iWidth = 0;
         int iHeight = 0;
         CImage* pSmallerImage = NULL;
         // Create temporary image by copying current image
         NEWBEGIN
         pSmallerImage = new CImage(*(pAvatar->Material(t)->Texture()));
         NEWEND("CAvatarFileHalflife::Save - small image allocation");
         if (pSmallerImage == NULL) {
            // Dump data
            for (int i=0; i<uNumTextures; i++) {
               if (pImages[i] != NULL) delete pImages[i];
            }
            delete [] pImages;
            return F_OUT_OF_MEMORY;
         }
         // Get size
         pSmallerImage->GetSize(iWidth,iHeight);
         // Modify width to be correct width
         iWidth = HL_TEXTURE_WIDTH;
         // Modify height to be a maximum of a quarter of the width.
         // Unless it is the torso, where we want more detail
         if (t == uTorsoTexIndex) iHeight = iWidth;
         else if (iHeight > iWidth/4) iHeight = iWidth/4;
         // Scale image to new size
         pSmallerImage->Scale(iWidth,iHeight);
         // Flip
         pSmallerImage->Flip();
         // Store image data
         uFirstPixel[t] = uTotalHeight;
         uImageHeight[t] = iHeight;
         uTotalHeight += iHeight;
         // Store image pointer
         pImages[t] = pSmallerImage;
      }
      // If it is, we don't bother with it.
      else {
         uFirstPixel[t] = uTotalHeight;
         uImageHeight[t] = 0;
         pImages[t] = NULL;
      }
   }
   // Create body image
   CImage imgBody(IT_RGB,HL_TEXTURE_WIDTH,uTotalHeight);
   int iWidth = 0; 
   int iHeight = 0;
   imgBody.GetSize(iWidth,iHeight);
   if (iWidth==0 || iHeight==0) {
      Cleanup(pAvatar);
      for (int i=0; i<uNumTextures; i++) {
         if (pImages[i] != NULL) delete pImages[i];
      }
      delete [] pImages;
      return F_OUT_OF_MEMORY;
   }
   else {
      for (t=0; t<uNumTextures; t++) {
         if (pImages[t] != NULL) {
            imgBody.Paste(*(pImages[t]),0,uFirstPixel[t]);
            delete pImages[t];
         }
      }
   }
   delete [] pImages;
   imgBody.Scale(HL_TEXTURE_WIDTH,HL_TEXTURE_HEIGHT);
   // Convert
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Converting body texture");
   imgBody.Convert(IT_PALETTE);

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating body parts");

   // Create Body Part Header
   sHeader.iNumBodyParts = 1;
   sHeader.iBodyPartChunkOffset = sHeader.iLength;
   SHalflifeBodyPart sBodyPartHeader;
   strncpy(sBodyPartHeader.pszName,"body",64);
   sBodyPartHeader.iNumModels  = 1;
   sBodyPartHeader.iBase       = 1;
   sHeader.iLength += sizeof(SHalflifeBodyPart);
   sBodyPartHeader.iModelIndex = sHeader.iLength;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating model");

   // Precalc a couple of texture vals
   const int iMaxU = HL_TEXTURE_WIDTH-1;
   const int iMaxV = HL_TEXTURE_HEIGHT-1;      
   // Split main mesh into two submeshes based on texture
   CDList<STriFace> pSubMeshes[2];
   for (i=0; i<iNumFaces; i++) {
      int iTextureNumber = pFaces[i].m_iMaterialNumber;
      CDList<STriFace>* pSubMesh = (iTextureNumber==uFaceTexIndex) ? pSubMeshes : pSubMeshes+1;
      pSubMesh->AddBack(pFaces[i]);
      // Rescale texture coordinates
      for (int v=3; v--!=0; ) {
         // Get tex coords
         double dU = pFaces[i].m_sVertices[v].m_sTexCoord.dU;
         double dV = 1-pFaces[i].m_sVertices[v].m_sTexCoord.dV;
         // Recalculate texture V coordinates if this is not the face texture
         if (iTextureNumber!=uFaceTexIndex) {
            // Clamp V to 0.1..0.9 to remove texture glitches
            if (dV > 0.9) dV = 0.9;
            else if (dV < 0.1) dV = 0.1;
            // Multiply by height of this particular image
            dV *= uImageHeight[iTextureNumber];
            // Add where this image starts
            dV += uFirstPixel[iTextureNumber];
            // Divide by the total height of the image
            dV /= uTotalHeight;
         }
         // Rescale and store
         STexCoord* pTexCoord = &((*pSubMesh)[pSubMesh->Length()-1].m_sVertices[v].m_sTexCoord);
         pTexCoord->dU = dU*iMaxU;
         pTexCoord->dV = dV*iMaxV;
      }
   }
   // Dump texture coordinate info
   delete [] uFirstPixel;
   delete [] uImageHeight;

   // Create Model Header Chunk
   SHalflifeModel sModelHeader;
   // Copy name
   strncpy(sModelHeader.pszName,m_pszModelname,64);
   // Type
   sModelHeader.iType = 0;
   // Bounding radius
   sModelHeader.fRadius = 0.0;
   // Number of submeshes (one head, one body)
   sModelHeader.iNumMeshes   = 2;
   // Vertex info
   sModelHeader.iNumVertices = iNumVertices;
   // Normal info
   sModelHeader.iNumNormals  = iNumVertices;//0x4B0;//iNumFaces;
   // Calculate data segment lengths
   unsigned long iVertexInfoLength = sModelHeader.iNumVertices;
   unsigned long iNormalInfoLength = sModelHeader.iNumNormals;
   unsigned long iVertexDataLength = sModelHeader.iNumVertices * sizeof(HalflifeVector);
   unsigned long iNormalDataLength = sModelHeader.iNumNormals * sizeof(HalflifeVector);
   // Word align lengths
   WORDALIGN(iVertexInfoLength);
   WORDALIGN(iNormalInfoLength);
   WORDALIGN(iVertexDataLength);
   WORDALIGN(iNormalDataLength);
   // Fill in vertex and normal indices
   sModelHeader.iVertexInfoIndex = sHeader.iLength + sizeof(SHalflifeModel);
   sModelHeader.iNormalInfoIndex = sModelHeader.iVertexInfoIndex + iVertexInfoLength;
   sModelHeader.iVertexDataIndex = sModelHeader.iNormalInfoIndex + iNormalInfoLength;
   sModelHeader.iNormalDataIndex = sModelHeader.iVertexDataIndex + iVertexDataLength;
   // Groups information
   sModelHeader.iNumDeformGroups = 0;
   sModelHeader.iDeformGroupIndex = 0;
   // We fill more in later on...
   sHeader.iLength += sizeof(SHalflifeModel);

   // Create Model Data Chunk
   int iModelDataChunkLength = iVertexInfoLength + iNormalInfoLength + iVertexDataLength + iNormalDataLength;
   NEWBEGIN
   m_pcModelDataChunk = new char[iModelDataChunkLength];
	NEWEND("CAvatarFileHalflife::Save - Model Data Chunk Allocation")
   if (m_pcModelDataChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   else {
      memset(m_pcModelDataChunk,0,iModelDataChunkLength);
      iCurrentPos = 0;
      // Write vertex info
      for (i=0; i<sModelHeader.iNumVertices; i++) {
         m_pcModelDataChunk[iCurrentPos+i] = m_pReverseCompressedSkeletonMap[pAvatar->GetVertexPart(i)];
      }
      iCurrentPos += iVertexInfoLength;
      // Write normal info
      /*for (int m=0; m<2; m++) {
         int iNumNormals = (m==0) ? 0x113 : iNumVertices-0x113;//0x39D;//pSubMeshes[m].Length();
         for (i=0; i<iNumNormals; i++) {
            if (m==0) {
               m_pcModelDataChunk[iCurrentPos+i] = 13;//m_pReverseCompressedSkeletonMap[skullbase];
            }
            else {
               // Assign normal to correct bone
               m_pcModelDataChunk[iCurrentPos+i] = 8;//m_pReverseCompressedSkeletonMap[vl5];
            }
         }
      }*/
      for (i=0; i<sModelHeader.iNumNormals; i++) {
         m_pcModelDataChunk[iCurrentPos+i] = m_pReverseCompressedSkeletonMap[pAvatar->GetVertexPart(i)];
      }
      iCurrentPos += iNormalInfoLength;
      // Write vertex data
      float* pfDataPtr = (float*)(m_pcModelDataChunk+iCurrentPos);
      for (i=0; i<sModelHeader.iNumVertices; i++) {
         BodyPart bpPart = (BodyPart)pAvatar->GetVertexPart(i);
         // Get vertex position
         CVector3D vecVertex(pVertices[i]);
         // Get bone centre
         CVector3D vecOffset = CVector3D(pBodyParts[bpPart].m_pntCurrentCentre);
         // Fix neck centre
         if (bpPart == vc7) {
            const double* pdCentre = pBodyParts[skullbase].m_pntCurrentCentre.m_dComponents;
            vecOffset += CVector3D(pdCentre[0],pdCentre[1],vecOffset.Z()-((vecOffset.Z()-pdCentre[2])/2));
            vecOffset /= 2;
         }
         // Fix lower torso offset
         else if (bpPart == vl5) {
            vecOffset += CVector3D(pBodyParts[sacroiliac].m_pntCurrentCentre);
            vecOffset /= 2;
         }
         // Fix lower mid torso offset
         else if (bpPart == vt12) {
            vecOffset += CVector3D(pBodyParts[vl5].m_pntCurrentCentre);
            vecOffset /= 2;
         }
         // Work out offset from local centre
         vecVertex -= vecOffset;
         // Scale
         vecVertex *= dScaleFactor;
         // Write
         //*(pfDataPtr++) = vecVertex.Y();
         //*(pfDataPtr++) = vecVertex.Z();
         //*(pfDataPtr++) = vecVertex.X();
         *(pfDataPtr++) =  vecVertex.X();
         *(pfDataPtr++) =  vecVertex.Z();
         *(pfDataPtr++) = -vecVertex.Y();
      }
      iCurrentPos += iVertexDataLength;
      // Write normal data
      pfDataPtr = (float *)(m_pcModelDataChunk+iCurrentPos);
      /*for (m=0; m<2; m++) {
         int iNumNormals = (m==0) ? 0x113 : iNumVertices-0x113;//0x39D;//pSubMeshes[m].Length();
         for (i=0; i<iNumNormals; i++) {
            // Get normal
            CVector3D vecNormal((pSubMeshes[m])[i].m_dNormal[0],(pSubMeshes[m])[i].m_dNormal[1],(pSubMeshes[m])[i].m_dNormal[2]);
            // Write
            //*(pfDataPtr++) = vecNormal.Y();
            //*(pfDataPtr++) = vecNormal.Z();
            //*(pfDataPtr++) = vecNormal.X();
            *(pfDataPtr++) =  vecNormal.X();
            *(pfDataPtr++) =  vecNormal.Z();
            *(pfDataPtr++) = -vecNormal.Y();
         }
      }*/
      for (i=0; i<sModelHeader.iNumNormals; i++) {
         // Get normal
         CVector3D vecNormal(pNormals[i]);
         // Write
         //*(pfDataPtr++) = vecNormal.Y();
         //*(pfDataPtr++) = vecNormal.Z();
         //*(pfDataPtr++) = vecNormal.X();
         *(pfDataPtr++) =  vecNormal.X();
         *(pfDataPtr++) =  vecNormal.Z();
         *(pfDataPtr++) = -vecNormal.Y();
      }
   }
   // Update offset
   sHeader.iLength += iModelDataChunkLength;
   
   // Create mesh data
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating meshes");

   // Create Mesh Header Chunk
   sModelHeader.iMeshIndex = sHeader.iLength;
   NEWBEGIN
   m_pMeshHeaderChunk = new SHalflifeMesh[sModelHeader.iNumMeshes];
	NEWEND("CAvatarFileHalflife::Save - Mesh Header Chunk Allocation")
   if (m_pMeshHeaderChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   // Write info into header
   for (i=sModelHeader.iNumMeshes; i--!=0; ) {
      // Number of triangles
      m_pMeshHeaderChunk[i].iNumTriangles = pSubMeshes[i].Length();
      // We fill in the triangle index in a minute
      // Texture reference
      m_pMeshHeaderChunk[i].iSkinRef = i;
      // Number of normals
      m_pMeshHeaderChunk[i].iNumNormals = (i==0) ? 0x113 : iNumVertices-0x113;//0x39D;//pSubMeshes[i].Length();
      // Normal index
      m_pMeshHeaderChunk[i].iNormalIndex = 0;
   }
   sHeader.iLength += sizeof(SHalflifeMesh) * sModelHeader.iNumMeshes;

   // Strip submeshes
   CDList<CTriStrip> pStrippedSubMeshes[2];
   // Work on one submesh at a time
   for (int m=2; m--!=0; ) {
      // Store data pointers
      CDList<STriFace>* pSubMesh = pSubMeshes + m;
      CDList<CTriStrip>* pStrippedMesh = pStrippedSubMeshes + m;
      // How may triangles and vertices in mesh
      const int iNumVertices = pAvatar->NumVertices();
      const int iNumTriangles = pSubMesh->Length();
      // How many triangles left to do?
      int iRemainingTriangles = iNumTriangles;

      // Create various mesh information lists
      // Create vertex connectivity list
      CDList<int>* plVertexConnectivity = new CDList<int>[iNumVertices];
      for (int f=iNumTriangles; f--!=0; ) {
         for (int v=3; v--!=0; ) {
            plVertexConnectivity[(*pSubMesh)[f].m_sVertices[v].m_iVertex].AddBack(f);
         }
      }
      // Create vertex valence list
      int* piVertexValences = new int[iNumVertices];
      for (int v=iNumVertices; v--!=0; ) {
         piVertexValences[v] = plVertexConnectivity[v].Length();
      }
      // Create face connectivity list
      CDList<int>* plFaceConnectivity = new CDList<int>[iNumTriangles];
      for (f=iNumTriangles; f--!=0; ) {
         for (int v=3; v--!=0; ) {
            int iFirstVertex = (*pSubMesh)[f].m_sVertices[v].m_iVertex;
            int iSecondVertex = (*pSubMesh)[f].m_sVertices[(v+1)%3].m_iVertex;
            for (int t1=piVertexValences[iFirstVertex]; t1--!=0; ) {
               int iFirstVertexFace = plVertexConnectivity[iFirstVertex][t1];
               if (iFirstVertexFace != f) {
                  for (int t2=piVertexValences[iSecondVertex]; t2--!=0; ) {
                     int iSecondVertexFace = plVertexConnectivity[iSecondVertex][t2];
                     if (iFirstVertexFace == iSecondVertexFace) {
                        plFaceConnectivity[f].AddBack(iFirstVertexFace);
                     }
                  }
               }
            }
         }         
      }
      // Create face valence list
      int* piFaceValences = new int[iNumTriangles];
      for (f=iNumTriangles; f--!=0; ) {
         piFaceValences[f] = plFaceConnectivity[f].Length();
      }
      // Create processed face list
      bool* pbProcessedFaces = new bool[iNumTriangles];
      for (f=iNumTriangles; f--!=0; ) pbProcessedFaces[f] = false;
      
      // Create triangle strips
      while (iRemainingTriangles > 0) {
         // Create new tristrip
         CTriStrip oTriStrip;
         // Find starting triangle - the one with the lowest valence
         int iStart = -1;
         for (int t=iNumTriangles; t--!=0; ) {
            if (pbProcessedFaces[t] == false) {
               if (iStart < 0) iStart = t;
               else if (piFaceValences[t] < piFaceValences[iStart]) iStart = t;
            }
         }
         // Add starting triangle to strip
         for (int v=3; v--!=0; ) {
            // Stick vertex on strip
            oTriStrip.push_back((*pSubMesh)[iStart].m_sVertices[v]);
            // Reduce vertex valence
            piVertexValences[(*pSubMesh)[iStart].m_sVertices[v].m_iVertex]--;
         }
         // Mark starting triangle as done
         pbProcessedFaces[iStart] = true;
         // Reduce valences of surrounding triangles
         for (t=plFaceConnectivity[iStart].Length(); t--!=0; ) {
            piFaceValences[plFaceConnectivity[iStart][t]]--;
         }
         // Reduce number of remaining triangles
         iRemainingTriangles--;
         // Add triangles to strip until we can't do any more or we reach the max length
         bool bDone = false;
         int iLastTriangle = iStart;
         int iTriangleScores[3];
         while (!bDone && (oTriStrip.size() < HL_MAX_STRIP_LENGTH)) {
            // Select next triangle...
            bool bCandidateExists = false;
            // Get neighbouring triangle numbers
            int iNextTriangles[3] = {-1,-1,-1};
            for (int t=plFaceConnectivity[iLastTriangle].Length(); t--!=0; ) {
               iNextTriangles[t] = plFaceConnectivity[iLastTriangle][t];
            }
            // Score neighbouring triangles which have not yet been done.
            for (t=3; t--!=0; ) {
               if ((iNextTriangles[t]!=-1) && (pbProcessedFaces[iNextTriangles[t]]==false)) {
                  // Triangle is a candidate
                  bCandidateExists = true;
                  iTriangleScores[t] = 0;
                  // Add valence to score
                  iTriangleScores[t] += piFaceValences[iNextTriangles[t]];
                  // Add 1 for highest vertex valence, subtract 1 otherwise
                  // ...not used yet...
                  // Add 1 for if swap required, subtract 1 otherwise
                  // ...not used yet...
               }
               else {
                  // Triangle does not exist or has already been 
                  // processed, so give it a very high score
                  iTriangleScores[t] = 100;
               }
            }
            // If there are any candidate triangles, add one to the strip
            if (bCandidateExists) {
               // Select the lowest scoring triangle
               int iNextTriangle = 2;
               for (t=2; t--!=0; ) {
                  if (iTriangleScores[t] < iTriangleScores[iNextTriangle])
                     iNextTriangle = t;
               }
               iNextTriangle = iNextTriangles[iNextTriangle];
               // Find it's unique vertex - this is the one that doesn't connect to the last triangle
               int iNextVertex = 0;
               bool bFound = false;
               for (int v=3; !bFound && v--!=0; ) {
                  // Get vertex number
                  int iVertex = (*pSubMesh)[iNextTriangle].m_sVertices[v].m_iVertex;
                  // If the vertex does not belong to the last triangle
                  if (iVertex != (*pSubMesh)[iLastTriangle].m_sVertices[0].m_iVertex &&
                      iVertex != (*pSubMesh)[iLastTriangle].m_sVertices[1].m_iVertex &&
                      iVertex != (*pSubMesh)[iLastTriangle].m_sVertices[2].m_iVertex) {
                     // Store it
                     iNextVertex = v;
                     // Found it!
                     bFound = true;
                  }
               }
               // Stick vertex on strip
               oTriStrip.push_back((*pSubMesh)[iNextTriangle].m_sVertices[iNextVertex]);
               // Reduce vertex valences
               for (v=3; v--!=0; ) {
                  piVertexValences[(*pSubMesh)[iNextTriangle].m_sVertices[v].m_iVertex]--;
               }
               // Mark triangle as done
               pbProcessedFaces[iNextTriangle] = true;
               // Reduce valences of surrounding triangles
               for (t=plFaceConnectivity[iNextTriangle].Length(); t--!=0; ) {
                  piFaceValences[plFaceConnectivity[iNextTriangle][t]]--;
               }
               // Reduce number of remaining triangles
               iRemainingTriangles--;
               // Store triangle index
               iLastTriangle = iNextTriangle;
            }
            // Otherwise, we're done
            else bDone = true;
         }
         // Store new tristrip
         pStrippedMesh->AddBack(oTriStrip);
      }
      // Check data
      for (f=iNumTriangles; f--!=0; ) {
         if (pbProcessedFaces[f] == false) cout << "unprocessed triangle!" << endl;
         if (piFaceValences[f] < 0) cout << "overzealous triangle - " << piFaceValences[f] << "!" << endl;
         else if (piFaceValences[f] > 0) cout << "uncompleted triangle - " << piFaceValences[f] << "!" << endl;
      }
      for (v=iNumVertices; v--!=0; ) {
         if (piVertexValences[v] > 0) cout << "uncompleted vertex!" << endl;
      }
      // Dump temporary data
      delete [] pbProcessedFaces;
      delete [] piFaceValences;
      delete [] piVertexValences;
      delete [] plFaceConnectivity;
      delete [] plVertexConnectivity;
   }  
   // Dump submesh information
   pSubMeshes[0].Clear();
   pSubMeshes[1].Clear();

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Storing meshes");

   // Create Mesh Data Chunk
   int iMeshDataChunkLength = 0;
   int iMeshDataChunkStart = sHeader.iLength;
   for (i=0; i<sModelHeader.iNumMeshes; i++) {
      // Fill in index in header
      m_pMeshHeaderChunk[i].iTriangleDataIndex = sHeader.iLength;
      // Work our mesh length
      int iMeshLength = 2; // Space for terminating 0
      for (int s=pStrippedSubMeshes[i].Length(); s--!=0; ) {
         iMeshLength += 2; // Space for number of points in strip
         iMeshLength += (pStrippedSubMeshes[i])[s].size() * 8; // Space for points
      }
      // Word align the mesh
      WORDALIGN(iMeshLength);
      // Update chunk & file size variables
      iMeshDataChunkLength += iMeshLength;
      sHeader.iLength += iMeshLength;
   }
   NEWBEGIN
   m_pcMeshDataChunk = new char[iMeshDataChunkLength];
	NEWEND("CAvatarFileHalflife::Save - Mesh Data Chunk Allocation")
   if (m_pcMeshDataChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   memset(m_pcMeshDataChunk,0,iMeshDataChunkLength);
   // Write data
   int iBaseNormal = 0;
   for (i=0; i<sModelHeader.iNumMeshes; i++) {
      int iCurrentOffset = m_pMeshHeaderChunk[i].iTriangleDataIndex - iMeshDataChunkStart;
      short* pcTempMeshDataPtr = (short*)(m_pcMeshDataChunk + iCurrentOffset);
      for (int j=0; j<pStrippedSubMeshes[i].Length(); j++) {
         CTriStrip* pStrip = &((pStrippedSubMeshes[i])[j]);
         int iNumPoints = pStrip->size();
         *pcTempMeshDataPtr++ = iNumPoints;
         for (int k=0; k<iNumPoints; k++) {
            // Vertex
            *pcTempMeshDataPtr++ = (*pStrip)[k].m_iVertex;
            // Normal
            *pcTempMeshDataPtr++ = (*pStrip)[k].m_iVertex;
            // Texture Coordinates
            *pcTempMeshDataPtr++ = short((*pStrip)[k].m_sTexCoord.dU + 0.5); // Add 0.5 to ensure correct rounding
            *pcTempMeshDataPtr++ = short((*pStrip)[k].m_sTexCoord.dV + 0.5); // Add 0.5 to ensure correct rounding
         }
      }
      // Write terminating zero
      *pcTempMeshDataPtr = 0;
      iBaseNormal = 700;
   }

   // Dump triangle strips
   pStrippedSubMeshes[0].Clear();
   pStrippedSubMeshes[1].Clear();

   // Write Textures
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating texture info");

   // Predeclare stuff
   int iTextureIndexChunkLength = 0;
   int iTextureDataChunkLength = 0;
   // Create Texture Header Chunk
   sHeader.iNumTextures = 2;
   sHeader.iTextureHeaderChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pTextureHeaderChunk = new SHalflifeTexture[sHeader.iNumTextures];
	NEWEND("CAvatarFileHalflife::Save - Texture Header Chunk Allocation")
   if (m_pTextureHeaderChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   for (i=0; i<sHeader.iNumTextures; i++) {
      // Set flags
      m_pTextureHeaderChunk[i].iFlags = 0;
      // Write name
      memset(m_pTextureHeaderChunk[i].pszName,0,64);
      strcpy(m_pTextureHeaderChunk[i].pszName,m_pszModelname);
      int iLength = strlen(m_pTextureHeaderChunk[i].pszName);
      strcpy(m_pTextureHeaderChunk[i].pszName + iLength, "_a.bmp");
      m_pTextureHeaderChunk[i].pszName[iLength + 1] += i;
      // Get image size
      // Store data
      m_pTextureHeaderChunk[i].iWidth = HL_TEXTURE_WIDTH;
      m_pTextureHeaderChunk[i].iHeight = HL_TEXTURE_HEIGHT;
      // Increase data chunk size to allow space for image @ 8bpp
      iTextureDataChunkLength += (HL_TEXTURE_WIDTH * HL_TEXTURE_HEIGHT) + 0x300;
   }
   sHeader.iLength += sizeof(SHalflifeTexture) * sHeader.iNumTextures;

   // Create Texture Index Chunk
   sHeader.iNumReplaceableTextures = sHeader.iNumTextures;
   sHeader.iNumTextureFamilies = 1;
   sHeader.iTextureIndexOffset = sHeader.iLength;
   iTextureIndexChunkLength = sHeader.iNumReplaceableTextures * 2;
   WORDALIGN(iTextureIndexChunkLength);
   NEWBEGIN
   m_pcTextureIndexChunk = new char[iTextureIndexChunkLength];
	NEWEND("CAvatarFileHalflife::Save - Texture Index Chunk Allocation")
   if (m_pcTextureIndexChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   memset(m_pcTextureIndexChunk,0,iTextureIndexChunkLength);
   for (i=0; i<sHeader.iNumReplaceableTextures; i++) {
      ((short*)m_pcTextureIndexChunk)[i] = i;
   }
   sHeader.iLength += iTextureIndexChunkLength;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Storing textures");

   // Create Texture Data Chunk
   sHeader.iTextureDataChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pcTextureDataChunk = new char[iTextureDataChunkLength];
	NEWEND("CAvatarFileHalflife::Save - Texture Data Chunk Allocation")
   if (m_pcTextureDataChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   iCurrentPos = 0;
   for (i=0; i<sHeader.iNumTextures; i++) {
      const CImage* pTexture = i==0 ? &imgFace : &imgBody;
      // Store index in header
      m_pTextureHeaderChunk[i].iIndex = sHeader.iLength + iCurrentPos;
      // Write texture data
      int iTextureSize = m_pTextureHeaderChunk[i].iWidth * m_pTextureHeaderChunk[i].iHeight;
      for (int iPixel=0; iPixel<iTextureSize; iPixel++) {
         *(m_pcTextureDataChunk+(iCurrentPos++)) = (char)pTexture->GetPixel(iPixel);
      }            
      // Write palette
      const CImagePalette* pPalette = pTexture->GetPalette();
      for (int iEntry=0; iEntry<256; iEntry++) {
         //memset(m_pcTextureDataChunk+iCurrentPos,iEntry,0x03);
         //iCurrentPos += 3;
         unsigned int uColour;
         pPalette->GetEntry(iEntry,uColour);
         *(m_pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 16) & 0xFF;
         *(m_pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 8) & 0xFF;
         *(m_pcTextureDataChunk+(iCurrentPos++)) = uColour & 0xFF;
      }
   }
   sHeader.iLength += iTextureDataChunkLength;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Writing to file");

   // Write chunks to stream in correct order
   if (eResult == F_OK) {
      osOutputStream.write((char*)&sHeader,sizeof(SHalflifeHeader));
      osOutputStream.write((char*)m_pBoneChunk,sizeof(SHalflifeBone) * sHeader.iNumBones);
      osOutputStream.write((char*)m_pBoneControllerChunk,sizeof(SHalflifeBoneController) * sHeader.iNumBoneControllers);
      osOutputStream.write((char*)m_pAttachmentChunk,sizeof(SHalflifeAttachment) * sHeader.iNumAttachments);
      osOutputStream.write((char*)m_pHitboxChunk,sizeof(SHalflifeHitbox) * sHeader.iNumHitboxes);
      if (m_oWedgie.Extract("hlmdlanimdata.bin", osOutputStream) != WJE_OK) 
         eResult = F_ERROR;
      osOutputStream.write((char*)m_pEventChunk,sizeof(SHalflifeEvent) * iNumEvents);
      osOutputStream.write((char*)m_pSeqGroupsChunk,sizeof(SHalflifeSeqGroup) * sHeader.iNumSeqGroups);
      osOutputStream.write((char*)&sBodyPartHeader,sizeof(SHalflifeBodyPart));
      osOutputStream.write((char*)&sModelHeader,sizeof(SHalflifeModel));
      osOutputStream.write(m_pcModelDataChunk,iModelDataChunkLength);
      osOutputStream.write((char*)m_pMeshHeaderChunk,sizeof(SHalflifeMesh) * sModelHeader.iNumMeshes);
      osOutputStream.write(m_pcMeshDataChunk,iMeshDataChunkLength);
      osOutputStream.write((char*)m_pTextureHeaderChunk,sizeof(SHalflifeTexture) * sHeader.iNumTextures);
      osOutputStream.write(m_pcTextureIndexChunk,iTextureIndexChunkLength);
      osOutputStream.write(m_pcTextureDataChunk,iTextureDataChunkLength);
   }

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Cleaning up");

   // Clean up
   Cleanup(pAvatar);

   // Done
   return eResult;
} // Save(ofstream& osOutputStream, CAvatar* pAvatar)

void CAvatarFileHalflife::CompressSkeleton(const BodyPart bpBodyPart, CAvatar* pAvatar) const {
   if (bpBodyPart < l_foot_tip) {
      m_pReverseCompressedSkeletonMap[bpBodyPart] = m_vCompressedSkeletonMap.size();
      m_vCompressedSkeletonMap.push_back((int)bpBodyPart);
      const SBodyPart* pBodyParts = pAvatar->BodyParts();
      if (pBodyParts[bpBodyPart].m_bpChildren[0] != -1) CompressSkeleton(pBodyParts[bpBodyPart].m_bpChildren[0],pAvatar);
      if (pBodyParts[bpBodyPart].m_bpChildren[1] != -1) CompressSkeleton(pBodyParts[bpBodyPart].m_bpChildren[1],pAvatar);
      if (pBodyParts[bpBodyPart].m_bpChildren[2] != -1) CompressSkeleton(pBodyParts[bpBodyPart].m_bpChildren[2],pAvatar);
   }
   return;
} //CompressedSkeleton(BodyPart bpBodyPart, CAvatar* pAvatar) const

void CAvatarFileHalflife::SaveThumbnail(const char *pcBitmap, CAvatar *poAvatar) const {
	ASSERT(pcBitmap && poAvatar);
	// Create thumbnail
	g_poVAL->StepProgress("HLSave");
	g_poVAL->SetProgressText("HLSave", "Rendering thumbnail");
	// Get the snapshot image
	CImage oImage(IT_RGB, 164, 200);
	// Create the scene
	CSceneHLThumbnail oScene;
	// Create context
	if (oScene.Create() == SC_OK) {
		// Import the avatar
		oScene.ImportAvatar(poAvatar);
		// Render the scene
		oScene.Render();
		// Get a pointer reference for the snapshot
		CImage *poImage = &oImage;
		// Get the snapshot
		oScene.Snapshot(poImage);
	}

	// Write thumbnail
	g_poVAL->StepProgress("HLSave");
	g_poVAL->SetProgressText("HLSave", "Writing thumbnail");
	// Open the data wedgie file
	char pcWJEName[STR_SIZE] = "";
	strcpy(pcWJEName, g_poVAL->GetAppDir());
	strcat(pcWJEName, "hldata.wje");
	fstream oDataWJE;
	oDataWJE.open(pcWJEName, ios::in|ios::binary|ios::nocreate);
	if (oDataWJE.fail())
		return;
	// Open the wedgie data
	CWedgie oWedgie;
	if (oWedgie.Open(&oDataWJE) != WJE_OK)
		return;
	// Create the memory into which to decompress the raw logo image
	unsigned int uLogoSize = HL_LOGO_WIDTH * HL_LOGO_HEIGHT * 3;
	unsigned char *pcRawLogo = NULL;
	NEWBEGIN
	pcRawLogo = (unsigned char*) new unsigned char[uLogoSize];
	NEWEND("CAvatarFile::SaveThumbnail - Raw logo image data")
	if (pcRawLogo) {
		// Decompress the logo
		int iHandle = oWedgie.Open("logo.raw");
		if (iHandle != -1) {
			oWedgie.Read(iHandle, pcRawLogo, uLogoSize);
			oWedgie.Close(iHandle);
		}
		// Import the logo into an image
		CImage oLogo(IT_RGB, HL_LOGO_WIDTH, HL_LOGO_HEIGHT);
		oLogo.ImportRawImage(pcRawLogo);
		// Paste the logo into the image
		oImage.Paste(oLogo, 0.5F, true, 0, 164 - HL_LOGO_WIDTH, 200 - HL_LOGO_HEIGHT);
		// Convert the image to 192 colours
		oImage.Convert(IT_PALETTE,192);
		// Create the BMP save filter
		CImageFile *poBMPFile = g_oImageFileStore.CreateByExtension("bmp");
		if (poBMPFile) {
			// Save the image
			oImage.Save(pcBitmap, poBMPFile);
			delete [] pcRawLogo;
			delete poBMPFile;
		}
	}
	// Close the wedgie file
	oWedgie.Close();
	oDataWJE.close();
} // SaveThumbnail

void CAvatarFileHalflife::Cleanup(CAvatar* pAvatar) const {
   // Dispose of old memory
   m_vCompressedSkeletonMap.clear();
   if (m_pcTextureDataChunk != NULL)   delete [] m_pcTextureDataChunk;
   if (m_pcTextureIndexChunk != NULL)  delete [] m_pcTextureIndexChunk;
   if (m_pTextureHeaderChunk != NULL)  delete [] m_pTextureHeaderChunk;
   if (m_pcMeshDataChunk != NULL)      delete [] m_pcMeshDataChunk;
   if (m_pMeshHeaderChunk != NULL)     delete [] m_pMeshHeaderChunk;
   if (m_pcModelDataChunk != NULL)     delete [] m_pcModelDataChunk;
   if (m_pSeqGroupsChunk != NULL)      delete [] m_pSeqGroupsChunk;
   if (m_pEventChunk != NULL)          delete [] m_pEventChunk;
   if (m_pHitboxChunk != NULL)         delete [] m_pHitboxChunk;
   if (m_pAttachmentChunk != NULL)     delete [] m_pAttachmentChunk;
   if (m_pBoneControllerChunk != NULL) delete [] m_pBoneControllerChunk;
   if (m_pBoneChunk != NULL)           delete [] m_pBoneChunk;
   // Restore old pose
   pAvatar->PopPose();
   pAvatar->UpdateModel();
   // Close the wedgie and file stream
   m_oWedgie.Close();
   m_fsDataWJE.close();
   // Done
   return;
} //Cleanup(CAvatar* pAvatar) const

const char* CAvatarFileHalflife::m_pszJointNames[] =
{
	"Bip01",
	"Bip01 Pelvis",
	"Bip01 L Leg",
	"Bip01 L Leg1",
	"Bip01 L Foot",
   "Bip01 L Toe0",
   "Bip01 L Toe01",
   "Bip01 L Toe02",
	"Bip01 R Leg",
	"Bip01 R Leg1",
	"Bip01 R Foot",
   "Bip01 R Toe0",
   "Bip01 R Toe01",
   "Bip01 R Toe02",
	"Bip01 Spine",
	"vl4",
	"vl3",
	"vl2",
	"vl1",
	"Bip01 Spine1",
	"vt11",
	"vt10",
	"vt9",
	"Bip01 Spine2",
	"vt7",
	"vt6",
	"vt5",
	"Bip01 Spine3",
	"vt3",
	"vt2",
	"vt1",
	"Bip01 Neck",
	"vc6",
	"vc5",
	"vc4",
	"vc3",
	"vc2",
	"vc1",
	"Bip01 Head",
	"l_eyeball_joint",
	"r_eyeball_joint",
	"l_sternoclavicular",
   "Bip01 L Arm",
   "Bip01 L Arm1",
   "Bip01 L Arm2",
   "Bip01 L Hand",
	"l_thumb1",
	"l_thumb2",
	"l_thumb3",
	"l_index0",
	"l_index1",
	"l_index2",
	"l_index3",
	"l_middle0",
	"l_middle1",
	"l_middle2",
	"l_middle3",
	"l_ring0",
	"l_ring1",
	"l_ring2",
	"l_ring3",
	"l_pinky0",
	"l_pinky1",
	"l_pinky2",
	"l_pinky3",
	"r_sternoclavicular",
   "Bip01 R Arm",
   "Bip01 R Arm1",
   "Bip01 R Arm2",
   "Bip01 R Hand",
	"r_thumb1",
	"r_thumb2",
	"r_thumb3",
	"r_index0",
	"r_index1",
	"r_index2",
	"r_index3",
	"r_middle0",
	"r_middle1",
	"r_middle2",
	"r_middle3",
	"r_ring0",
	"r_ring1",
	"r_ring2",
	"r_ring3",
	"r_pinky0",
	"r_pinky1",
	"r_pinky2",
	"r_pinky3",
   "l_foot_tip",
   "r_foot_tip",
   "skull_tip",
   "l_hand_tip",
   "r_hand_tip",   
};
