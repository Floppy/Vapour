//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHalflife.cpp - 16/2/2000 - James Smith
//	Halflife export filter implementation
//
// $Id: AvatarFileHalflife.cpp,v 1.10 2000/08/02 18:05:04 waz Exp $
//

#include "stdafx.h"

#include "AvatarFileHalflife.h"
#include "AvatarFileProxy.h"
#include "Image.h"
#include "ImageFileStore.h"
#include "MathConstants.h"

#include <float.h>
#include <direct.h>
#include <errno.h>

#include "RCOpenGLBufferWin32.h"
#include "RenderAvatar.h"
#include "gl\gl.h"

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

CAvatarFileHalflife::CAvatarFileHalflife() {
   m_pszModelname = NULL;
   m_pcTextureDataChunk = NULL;
   m_pcTextureIndexChunk = NULL;
   m_pTextureHeaderChunk = NULL;
   m_pcMeshDataChunk = NULL;
   m_pMeshHeaderChunk = NULL;
   m_pcModelDataChunk = NULL;
   m_pSeqGroupsChunk = NULL;
   m_pHitboxChunk = NULL;
   m_pAttachmentChunk = NULL;
   m_pBoneControllerChunk = NULL;
   m_pBoneChunk = NULL;
   m_pPose = NULL;
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
	g_poVAL->SetProgressMax("HLSave", 17 + pAvatar->NumTextures());
   
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

   // Store old pose
   CAvatarPose oPose = pAvatar->ExportPose();
   m_pPose = &oPose;
   pAvatar->ResetPose();
   pAvatar->UpdateModel();

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
   /*// Line up spine exactly on Y axis
   pAvatar->AlignBodyPart(vl5,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(vt12,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(vt8,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(vt4,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(vc7,vecTarget,false);
   pAvatar->UpdateModel();
   pAvatar->AlignBodyPart(skullbase,vecTarget,false);
   pAvatar->UpdateModel();*/
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
   // Translate model down...
   //pAvatar->SetRootTranslation(CVector3D(0,-(pAvatar->Height()/2.0),0));
   //pAvatar->UpdateModel();

   //pAvatar->ResetPose();
   //pAvatar->UpdateModel();
   
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating header");

   // Create Header & fill in some fields
   SHalflifeMDLHeader sHeader;
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
   sHeader.iLength += sizeof(SHalflifeMDLHeader);
	 	 
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating bones");

   // Prepare Skeleton
   CompressSkeleton(root,pAvatar);

   // Create Bone Chunk
   sHeader.iNumBones = m_vCompressedSkeletonMap.size();
   sHeader.iBoneChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pBoneChunk = new SHalflifeMDLBone[sHeader.iNumBones];
	NEWEND("CAvatarFileHalflife::Save - Bone Chunk Allocation")
   if (m_pBoneChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }

   // Read generic bone data from external file
   int iHandle = m_oWedgie.Open("hlmdlbonedata.bin");
   if (iHandle > -1) {
	   unsigned int uDataSize = sizeof(SHalflifeMDLBone) * sHeader.iNumBones;
	   unsigned int uRead = m_oWedgie.Read(iHandle, (unsigned char*)m_pBoneChunk, uDataSize);
      if (uRead != uDataSize) {
         Cleanup(pAvatar);
         return F_ERROR;
      }
   }
   else {
      Cleanup(pAvatar);
	   return F_ERROR;
   }

   iCurrentPos = 0;
   // Customise bone data
   for (i=0; i<sHeader.iNumBones; i++) {
      // Set name
      BodyPart bpPart = (BodyPart)m_vCompressedSkeletonMap[i];
      strncpy(m_pBoneChunk[i].pszName,m_pszJointNames[bpPart],32);
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
      m_pBoneChunk[i].vCentre[0] =  vecJoint.X();
      m_pBoneChunk[i].vCentre[1] =  vecJoint.Z();
      m_pBoneChunk[i].vCentre[2] = -vecJoint.Y();
      // Set rotation and scales
      /*pBoneChunk[i].vRotation[0]      = 0.0;
      m_pBoneChunk[i].vRotation[1]      = 0.0;
      m_pBoneChunk[i].vRotation[2]      = 0.0;
      m_pBoneChunk[i].vCentreScale[0]   = 1.0;
      m_pBoneChunk[i].vCentreScale[1]   = 1.0;
      m_pBoneChunk[i].vCentreScale[2]   = 1.0;
      m_pBoneChunk[i].vRotationScale[0] = 1.0;
      m_pBoneChunk[i].vRotationScale[1] = 1.0;
      m_pBoneChunk[i].vRotationScale[2] = 1.0;*/
      // Set Flags
      m_pBoneChunk[i].iFlags = 0;
   }
   sHeader.iLength += sizeof(SHalflifeMDLBone) * sHeader.iNumBones;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating bonecontrollers");

   // Create BoneController Chunk
   sHeader.iNumBoneControllers = 4;
   sHeader.iBoneControllerChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pBoneControllerChunk = new SHalflifeMDLBoneController[sHeader.iNumBoneControllers];
	NEWEND("CAvatarFileHalflife::Save - Bone Controller Chunk Allocation")
   if (m_pBoneControllerChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   for (i=0; i<sHeader.iNumBoneControllers; i++) {
      // Set bone link
      m_pBoneControllerChunk[i].iBone  = m_pReverseCompressedSkeletonMap[vl5] + i;
      // Set bone's link to this controller
      m_pBoneChunk[m_pReverseCompressedSkeletonMap[vl5]+i].iBoneController[0] = i;
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
   sHeader.iLength += sizeof(SHalflifeMDLBoneController) * sHeader.iNumBoneControllers;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating attachments");

   // Create Attachments Chunk
   sHeader.iNumAttachments = 3;
   sHeader.iAttachmentChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pAttachmentChunk = new SHalflifeMDLAttachment[sHeader.iNumAttachments];
	NEWEND("CAvatarFileHalflife::Save - Attachment Chunk Allocation")
   if (m_pAttachmentChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   for (i=0; i<sHeader.iNumAttachments; i++) {
      // Set bone
      m_pAttachmentChunk[i].iBone = m_pReverseCompressedSkeletonMap[r_wrist];
      // Set type
      m_pAttachmentChunk[i].iType = 0;
      // Set name amd position
      switch (i) {
      case 0:
         strncpy(m_pAttachmentChunk[i].pszName,"Attachment 0",32);
         m_pAttachmentChunk[i].vPosition[0] = 20.0;
         m_pAttachmentChunk[i].vPosition[1] = 2.0;
         m_pAttachmentChunk[i].vPosition[2] = 5.0;
         break;
      case 1:
         strncpy(m_pAttachmentChunk[i].pszName,"Attachment 1",32);
         m_pAttachmentChunk[i].vPosition[0] = 15.0;
         m_pAttachmentChunk[i].vPosition[1] = 1.5;
         m_pAttachmentChunk[i].vPosition[2] = 3.75;
         break;
      case 2:
         strncpy(m_pAttachmentChunk[i].pszName,"Attachment 2",32);
         m_pAttachmentChunk[i].vPosition[0] = 30.0;
         m_pAttachmentChunk[i].vPosition[1] = 3.0;
         m_pAttachmentChunk[i].vPosition[2] = 7.5;
         break;
      default:
         strncpy(m_pAttachmentChunk[i].pszName,"",32);
         m_pAttachmentChunk[i].vPosition[0] = 0.0;
         m_pAttachmentChunk[i].vPosition[1] = 0.0;
         m_pAttachmentChunk[i].vPosition[2] = 0.0;
         break;
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
   sHeader.iLength += sizeof(SHalflifeMDLAttachment) * sHeader.iNumAttachments;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating hitboxes");

   // Create Hitbox Chunk
   const int piHitGroups[] = {3,3,6,6,6,7,7,7,3,2,2,2,2,1,4,4,4,4,5,5,5,5};
   sHeader.iNumHitboxes = sHeader.iNumBones;
   sHeader.iHitboxChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pHitboxChunk = new SHalflifeMDLHitbox[sHeader.iNumHitboxes];
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
      m_pHitboxChunk[i].vMinimum[0] =  vecMin.X();
      m_pHitboxChunk[i].vMinimum[1] =  vecMin.Z();
      m_pHitboxChunk[i].vMinimum[2] = -vecMin.Y();
      // Write maximum
      m_pHitboxChunk[i].vMaximum[0] =  vecMax.X();
      m_pHitboxChunk[i].vMaximum[1] =  vecMax.Z();
      m_pHitboxChunk[i].vMaximum[2] = -vecMax.Y();
   }
   sHeader.iLength += sizeof(SHalflifeMDLHitbox) * sHeader.iNumHitboxes;

   // Create Animation Sequence Header Chunk
   sHeader.iNumSeqs = 77;
   sHeader.iLength += 0x30304; // Offset into anim file of look_idle seq desc.
   sHeader.iSeqChunkOffset = sHeader.iLength;
   sHeader.iLength += (sHeader.iNumSeqs * 0xB0) + 0x474; // + space for seqs descs and events (0x474 bytes);

   // Create Sequence Groups Chunk
   sHeader.iNumSeqGroups = 1;
   sHeader.iSeqGroupChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pSeqGroupsChunk = new SHalflifeMDLSeqGroup[sHeader.iNumSeqGroups];
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
   sHeader.iLength += sizeof(SHalflifeMDLSeqGroup) * sHeader.iNumSeqGroups;

   // Deal with Transition Chunk (but don't create anything)
   sHeader.iNumTransitions = 0;
   sHeader.iTransitionChunkOffset = sHeader.iLength;
   
   // Process textures, cos we'll need them in a minute
   std::vector<CImage*> vpSmallTextures;
   for (i=0; i<pAvatar->NumTextures(); i++) {
      // Notify of progress
      char pszDisplay[32];
      sprintf(pszDisplay,"Converting texture %d", i);
      g_poVAL->StepProgress("HLSave");
      g_poVAL->SetProgressText("HLSave", pszDisplay);
      // Downsize images
      CImage* pTexture;
      NEWBEGIN
      pTexture = new CImage(*(pAvatar->Texture(i)));
   	NEWEND("CAvatarFileHalflife::Save - Small Image Allocation")
      if (pTexture == NULL) { 
         eResult = F_OUT_OF_MEMORY;
         continue;
      }
      int iWidth = 0;
      int  iHeight = 0;
      pTexture->GetSize(iWidth,iHeight);
      if (i == pAvatar->TextureIndex(skullbase)) {
         // Set size of face texture to 256x256
         iWidth  = 256;
         iHeight = 256;
      }
      else if (i == pAvatar->TextureIndex(vl5)) {
         // Set size of torso texture to 128x128
         iWidth  = 128;
         iHeight = 128;
      }
      else {
         // Halve size of other textures
         iWidth  = iWidth  >> 1;
         iHeight = iHeight >> 1;
         // Max 64x64
         if (iWidth > 64) iWidth = 64;
         if (iHeight > 64) iHeight = 64;
      }
      // Scale
      pTexture->Scale(iWidth,iHeight);
      // Flip
      pTexture->Flip();
      // Reduce colours
      pTexture->Convert(IT_PALETTE,256);
      // Store
      vpSmallTextures.push_back(pTexture);
   }
   
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating body parts");

   // Create Body Part Header
   sHeader.iNumBodyParts = 1;
   sHeader.iBodyPartChunkOffset = sHeader.iLength;
   SHalflifeMDLBodyPart sBodyPartHeader;
   strncpy(sBodyPartHeader.pszName,"body",64);
   sBodyPartHeader.iNumModels  = 1;
   sBodyPartHeader.iBase       = 1;
   sHeader.iLength += sizeof(SHalflifeMDLBodyPart);
   sBodyPartHeader.iModelIndex = sHeader.iLength;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating model");

   // Create Model Header Chunk
   SHalflifeMDLModel sModelHeader;
   // Copy name
   strncpy(sModelHeader.pszName,m_pszModelname,64);
   // Type
   sModelHeader.iType = 0;
   // Bounding radius
   sModelHeader.fRadius = 0.0;
   // Number of submeshes (one for each texture)
   sModelHeader.iNumMeshes   = pAvatar->NumTextures();
   // Vertex info
   sModelHeader.iNumVertices = iNumVertices;
   // Normal info
   sModelHeader.iNumNormals  = iNumVertices;
   // Calculate data segment lengths
   unsigned long iVertexInfoLength = iNumVertices;
   unsigned long iNormalInfoLength = iNumVertices;
   unsigned long iVertexDataLength = iNumVertices * sizeof(HalflifeMDLVector);
   unsigned long iNormalDataLength = iNumVertices * sizeof(HalflifeMDLVector);
   // Word align lengths
   WORDALIGN(iVertexInfoLength);
   WORDALIGN(iNormalInfoLength);
   WORDALIGN(iVertexDataLength);
   WORDALIGN(iNormalDataLength);
   // Fill in vertex and normal indices
   sModelHeader.iVertexInfoIndex = sHeader.iLength + sizeof(SHalflifeMDLModel);
   sModelHeader.iNormalInfoIndex = sModelHeader.iVertexInfoIndex + iVertexInfoLength;
   sModelHeader.iVertexDataIndex = sModelHeader.iNormalInfoIndex + iNormalInfoLength;
   sModelHeader.iNormalDataIndex = sModelHeader.iVertexDataIndex + iVertexDataLength;
   // Groups information
   sModelHeader.iNumDeformGroups = 0;
   sModelHeader.iDeformGroupIndex = 0;
   // We fill more in later on...
   sHeader.iLength += sizeof(SHalflifeMDLModel);

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
      for (i=0; i<iNumVertices; i++) {
         m_pcModelDataChunk[iCurrentPos+i] = m_pReverseCompressedSkeletonMap[pAvatar->GetVertexPart(i)];
      }
      iCurrentPos += iVertexInfoLength;
      // Write normal info
      for (i=0; i<iNumVertices; i++) {
         m_pcModelDataChunk[iCurrentPos+i] = m_pReverseCompressedSkeletonMap[pAvatar->GetVertexPart(i)];
      }
      iCurrentPos += iNormalInfoLength;
      // Write vertex data
      float* pfDataPtr = (float*)(m_pcModelDataChunk+iCurrentPos);
      for (i=0; i<iNumVertices; i++) {
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
         *(pfDataPtr++) =  vecVertex.X();
         *(pfDataPtr++) =  vecVertex.Z();
         *(pfDataPtr++) = -vecVertex.Y();
      }
      iCurrentPos += iVertexDataLength;
      // Write normal data
      pfDataPtr = (float *)(m_pcModelDataChunk+iCurrentPos);
      for (i=0; i<iNumVertices; i++) {
         // Get normal
         CVector3D vecNormal(pNormals[i]);
         // Write
         *(pfDataPtr++) =  vecNormal.X();
         *(pfDataPtr++) =  vecNormal.Z();
         *(pfDataPtr++) = -vecNormal.Y();
      }
   }
   // Update offset
   sHeader.iLength += iModelDataChunkLength;
   
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating meshes");

   // Predeclare a couple of things
   int iMeshDataChunkLength = 0;
   // Split main mesh into submeshes based on texture
   std::vector<STriFace>* vSubMeshes;
   NEWBEGIN
   vSubMeshes = new std::vector<STriFace>[sModelHeader.iNumMeshes];
	NEWEND("CAvatarFileHalflife::Save - Submesh Allocation")
   if (vSubMeshes == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   for (i=0; i<iNumFaces; i++) {
      (vSubMeshes + pFaces[i].m_iTextureNumber)->push_back(pFaces[i]);
   }
   // Compress submeshes into triangle strips
   // ********NOT IMPLEMENTED YET*********

   // Create Mesh Header Chunk
   sModelHeader.iMeshIndex = sHeader.iLength;
   NEWBEGIN
   m_pMeshHeaderChunk = new SHalflifeMDLMesh[sModelHeader.iNumMeshes];
	NEWEND("CAvatarFileHalflife::Save - Mesh Header Chunk Allocation")
   if (m_pMeshHeaderChunk == NULL) {
      Cleanup(pAvatar);
      return F_OUT_OF_MEMORY;
   }
   // Write info into header
   for (i=0; i<sModelHeader.iNumMeshes; i++) {
      // Number of triangles
      m_pMeshHeaderChunk[i].iNumTriangles = vSubMeshes[i].size();
      // We fill in the triangle index in a minute
      // Texture reference
      m_pMeshHeaderChunk[i].iSkinRef = i;
      // Number of normals
      m_pMeshHeaderChunk[i].iNumNormals = vSubMeshes[i].size();
      // Normal index
      m_pMeshHeaderChunk[i].iNormalIndex = 0;
   }
   sHeader.iLength += sizeof(SHalflifeMDLMesh) * sModelHeader.iNumMeshes;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating mesh data");

   // Create Mesh Data Chunk
   int iMeshDataChunkStart = sHeader.iLength;
   for (i=0; i<sModelHeader.iNumMeshes; i++) {
      // Fill in index in header
      m_pMeshHeaderChunk[i].iTriangleDataIndex = sHeader.iLength;
      // Work our mesh length
      int iMeshLength = (vSubMeshes[i].size() * 26) + 2;
      WORDALIGN(iMeshLength);
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
   for (i=0; i<sModelHeader.iNumMeshes; i++) {
      int iCurrentOffset = m_pMeshHeaderChunk[i].iTriangleDataIndex - iMeshDataChunkStart;
      short* pcTempMeshDataPtr = (short*)(m_pcMeshDataChunk + iCurrentOffset);
      for (int j=0; j<vSubMeshes[i].size(); j++) {
         *pcTempMeshDataPtr++ = 3;
         for (int k=2; k>=0; k--) {
            STriFace* iCurrentFace = &((vSubMeshes[i])[j]);
            *pcTempMeshDataPtr++ = iCurrentFace->m_iVertices[k];
            *pcTempMeshDataPtr++ = iCurrentFace->m_iVertices[k];
            int iWidth, iHeight;
            vpSmallTextures[i]->GetSize(iWidth,iHeight);
            *pcTempMeshDataPtr++ = iCurrentFace->m_sTexCoords[k].dU * iWidth;
            *pcTempMeshDataPtr++ = (1-iCurrentFace->m_sTexCoords[k].dV) * iHeight;
         }
      }
      // Write terminating zero
      *pcTempMeshDataPtr = 0;
   }
   // Bin submesh and tristrip information
   delete [] vSubMeshes;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating texture info");

   // Predeclare stuff
   int iTextureIndexChunkLength = 0;
   int iTextureDataChunkLength = 0;
   // Create Texture Header Chunk
   sHeader.iNumTextures = pAvatar->NumTextures();
   sHeader.iTextureHeaderChunkOffset = sHeader.iLength;
   NEWBEGIN
   m_pTextureHeaderChunk = new SHalflifeMDLTexture[sHeader.iNumTextures];
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
      int iWidth, iHeight;
      vpSmallTextures[i]->GetSize(iWidth,iHeight);
      // Store data
      m_pTextureHeaderChunk[i].iWidth = iWidth;
      m_pTextureHeaderChunk[i].iHeight = iHeight;
      // Increase data chunk size to allow space for image @ 8bpp
      iTextureDataChunkLength += (iWidth * iHeight) + 0x300;
   }
   sHeader.iLength += sizeof(SHalflifeMDLTexture) * sHeader.iNumTextures;

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
      const CImage* pTexture = vpSmallTextures[i];
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
         unsigned long uColour;
         pPalette->GetEntry(iEntry,uColour);
         *(m_pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 16) & 0xFF;
         *(m_pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 8) & 0xFF;
         *(m_pcTextureDataChunk+(iCurrentPos++)) = uColour & 0xFF;
      }
   }
   sHeader.iLength += iTextureDataChunkLength;

   // Dispose of resized textures
   for (i=0; i<pAvatar->NumTextures(); i++) {
      CImage* pTexture = vpSmallTextures.back();
      vpSmallTextures.pop_back();
      delete pTexture;
   }

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Writing to file");

   // Write chunks to stream in correct order
   if (eResult == F_OK) {
      osOutputStream.write((char*)&sHeader,sizeof(SHalflifeMDLHeader));
      osOutputStream.write((char*)m_pBoneChunk,sizeof(SHalflifeMDLBone) * sHeader.iNumBones);
      osOutputStream.write((char*)m_pBoneControllerChunk,sizeof(SHalflifeMDLBoneController) * sHeader.iNumBoneControllers);
      osOutputStream.write((char*)m_pAttachmentChunk,sizeof(SHalflifeMDLAttachment) * sHeader.iNumAttachments);
      osOutputStream.write((char*)m_pHitboxChunk,sizeof(SHalflifeMDLHitbox) * sHeader.iNumHitboxes);
      if (m_oWedgie.Extract("hlmdlanimdata.bin", osOutputStream) != WJE_OK) 
         eResult = F_ERROR;
      osOutputStream.write((char*)m_pSeqGroupsChunk,sizeof(SHalflifeMDLSeqGroup) * sHeader.iNumSeqGroups);
      osOutputStream.write((char*)&sBodyPartHeader,sizeof(SHalflifeMDLBodyPart));
      osOutputStream.write((char*)&sModelHeader,sizeof(SHalflifeMDLModel));
      osOutputStream.write(m_pcModelDataChunk,iModelDataChunkLength);
      osOutputStream.write((char*)m_pMeshHeaderChunk,sizeof(SHalflifeMDLMesh) * sModelHeader.iNumMeshes);
      osOutputStream.write(m_pcMeshDataChunk,iMeshDataChunkLength);
      osOutputStream.write((char*)m_pTextureHeaderChunk,sizeof(SHalflifeMDLTexture) * sHeader.iNumTextures);
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
	// The render context
	CRCOpenGLBufferWin32 oRC;
	// Image size
	oRC.SetSize(164, 200);
	// Colour depth
	oRC.SetOption(RCO_DEPTH, 24U);
	// Black background
	oRC.SetOption(RCO_BACKRED, 0.0F);
	oRC.SetOption(RCO_BACKGREEN, 0.0F);
	oRC.SetOption(RCO_BACKBLUE, 0.0F);
	// Set the view planes and angle
	oRC.SetOption(RCO_NEARPLANE, 0.5F);
	oRC.SetOption(RCO_FARPLANE, 20.0F);
	oRC.SetOption(RCO_VIEWANGLE, 10.0F);
	oRC.SetProjectionMode(RCP_PERSPECTIVE);
	// Create the context
	if (oRC.Create() != RC_OK)
		return;
	// Create the avatar render object
	CRenderAvatar oView(&oRC);
	oView.SetAvatar(poAvatar, false);
	// Move the avatar into position
	oView.SetPosition(0.0F, -(poAvatar->Height() / 2.0F), -12.0F);
	// Set the render mode to textured
	oView.RenderMode(ROM_TEXTURE);
	// Enable the context
	oRC.Enable();
	// Initialise the avatar
	oView.Init();
	// Clear the buffer
	oRC.ClearBuffer(RCB_COLOUR | RCB_DEPTH);
	// Reset the matrix
	glLoadIdentity();
	// Place the light
	glEnable(GL_LIGHT0);
	GLfloat fLAmbient[4] = {0.1F, 0.1F, 0.1F, 1.0F};
	GLfloat fLWhite[4] = {1.0F, 1.0F, 1.0F, 1.0F};
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fLWhite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fLWhite);
	GLfloat fLPos[4] = { 0.0F, 0.0F, 0.0F, 1.0F};
	glLightfv(GL_LIGHT0, GL_POSITION, fLPos);
	// Render the avatar
	oView.Execute();
	// Disable the context
	oRC.Disable();
	// Get the snapshot image
	CImage oImage(IT_RGB, 164, 200);
	CImage *poImage = &oImage; // Hack for render context
	oRC.Snapshot(poImage);
	// Destroy the context
	oRC.Destroy();

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
		// Convert the image to 256 colours
		oImage.Convert(IT_PALETTE);
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
   if (m_pHitboxChunk != NULL)         delete [] m_pHitboxChunk;
   if (m_pAttachmentChunk != NULL)     delete [] m_pAttachmentChunk;
   if (m_pBoneControllerChunk != NULL) delete [] m_pBoneControllerChunk;
   if (m_pBoneChunk != NULL)           delete [] m_pBoneChunk;
   // Restore old pose
   if (m_pPose != NULL) {
      pAvatar->ImportPose(*m_pPose);
      pAvatar->UpdateModel();
   }
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
