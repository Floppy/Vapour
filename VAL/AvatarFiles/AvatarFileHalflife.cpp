//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHalflife.cpp - 16/2/2000 - James Smith
//	Halflife export filter implementation
//
// $Id: AvatarFileHalflife.cpp,v 1.6 2000/07/22 23:24:58 waz Exp $
//


#include "stdafx.h"

#include "AvatarFileHalflife.h"
#include "AvatarFileProxy.h"
#include "HalflifeMDL.h"
#include "Image.h"
#include "ImageFileStore.h"
#include "MathConstants.h"

#include <float.h>
#include <direct.h>
#include <errno.h>

#include "Wedgie.h"

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

int CAvatarFileHalflife::Save(const char* pszFilename, CAvatar* pAvatar) const {
	if (pAvatar == NULL) return 0;
   
	int iRetVal = 1;

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
   if (pszBaseFilename == NULL) iRetVal = 0;
   else {
      strcpy(pszBaseFilename,pszTgtDir);
      strcat(pszBaseFilename,m_pszModelname);
   }
   int iBaseFilenameLength = strlen(pszBaseFilename);

   // Create output subdirectory
   int iResult = _mkdir(pszTgtDir);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create output directory!\n");
      return 0;
   }
   
   // Setup the export progress dialog
	g_poVAL->SetProgressMax("HLSave", 17 + 2*pAvatar->NumTextures());
   
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Saving MDL file");
   
   // Create filename
   strcpy(pszBaseFilename+iBaseFilenameLength,".mdl");
   // Save to output stream
   ofstream osOutputStream(pszBaseFilename,ios::out|ios::binary);
	if (!osOutputStream.fail()) {
   	iRetVal = Save(osOutputStream,pAvatar);
	}

   // Create thumbnail
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Rendering thumbnail");
   CImage imThumbnail(IT_PALETTE,164,200,256);

   // Write thumbnail
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Writing thumbnail");
   // Create filename
   strcpy(pszBaseFilename+iBaseFilenameLength,".bmp");
   // Create filter and save
   CImageFile* pThumbFilter = g_oImageFileStore.CreateByExtension("bmp");
   imThumbnail.Save(pszBaseFilename,pThumbFilter);
   // Tidy up
   delete pThumbFilter;

   // Finish up
   g_poVAL->StepProgress("HLSave");
   // Delete strings
   delete [] pszBaseFilename;
   free(m_pszModelname);
   m_pszModelname = NULL;
   // DOne
   return iRetVal;
} // Save(const char* pszFilename, CAvatar* pAvatar)

int CAvatarFileHalflife::Save(ofstream& osOutputStream, CAvatar* pAvatar) const {

   // Force binary stream mode
   int iOldMode = osOutputStream.setmode(filebuf::binary);
   
   int iRetVal = 1;        // Return value
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
   CAvatarPose oldPose = pAvatar->ExportPose();
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
   sHeader.iID = HLMDLMAGIC;
   sHeader.iVersion = HLMDLVERSION;
   // Model name
   strncpy(sHeader.pszName,m_pszModelname,64);
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
   g_poVAL->SetProgressText("HLSave", "Loading external data");

	 // Open the data wedgie
	 char pcWJEName[STR_SIZE] = "";
	 strcpy(pcWJEName, g_poVAL->GetAppDir());
	 strcat(pcWJEName, "hldata.wje");
	 fstream oDataWJE;
	 oDataWJE.open(pcWJEName, ios::in|ios::binary|ios::nocreate);
	 if (oDataWJE.fail())
		 iRetVal = 0;
	 CWedgie oWedgie;
	 oWedgie.Open(&oDataWJE);
	 	 
   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating bones");

   // Prepare Skeleton
   CompressSkeleton(root,pAvatar);

   // Create Bone Chunk
   sHeader.iNumBones = m_vCompressedSkeletonMap.size();
   sHeader.iBoneChunkOffset = sHeader.iLength;
   SHalflifeMDLBone* pBoneChunk;
   NEWBEGIN
   pBoneChunk = new SHalflifeMDLBone[sHeader.iNumBones];
	NEWEND("CAvatarFileHalflife::Save - Bone Chunk Allocation")
   if (pBoneChunk == NULL) iRetVal = 0;
   else {
      // Read generic bone data from external file
			int iHandle = oWedgie.Open("hlmdlbonedata.bin");
			if (iHandle > -1) {
				unsigned int uDataSize = sizeof(SHalflifeMDLBone) * sHeader.iNumBones;
				unsigned int uRead = oWedgie.Read(iHandle, (unsigned char*)pBoneChunk, uDataSize);
				if (uRead != uDataSize)
					iRetVal = 0;
			}
			else
				iRetVal = 0;

      iCurrentPos = 0;
      // Customise bone data
      for (i=0; i<sHeader.iNumBones; i++) {
         // Set name
         BodyPart bpPart = (BodyPart)m_vCompressedSkeletonMap[i];
         strncpy(pBoneChunk[i].pszName,m_pszJointNames[bpPart],32);
         // Set parent reference
         BodyPart bpParent = (bpPart==l_acromioclavicular)||(bpPart==r_acromioclavicular) ? vc7 : pBodyParts[bpPart].m_bpParent;
         pBoneChunk[i].iParent = bpParent==unknown ? -1 : m_pReverseCompressedSkeletonMap[bpParent];
         // Get this joint centre
         CVector3D vecJoint(pBodyParts[bpPart].m_pntCurrentCentre);
         // Get dist from parent
         if (bpParent != unknown) {
            vecJoint -= CVector3D(pBodyParts[bpParent].m_pntCurrentCentre);
         }
         // Scale vector
         vecJoint *= dScaleFactor;
         // Write centre
         pBoneChunk[i].vCentre[0] =  vecJoint.X();
         pBoneChunk[i].vCentre[1] =  vecJoint.Z();
         pBoneChunk[i].vCentre[2] = -vecJoint.Y();
         // Set rotation and scales
         /*pBoneChunk[i].vRotation[0]      = 0.0;
         pBoneChunk[i].vRotation[1]      = 0.0;
         pBoneChunk[i].vRotation[2]      = 0.0;
         pBoneChunk[i].vCentreScale[0]   = 1.0;
         pBoneChunk[i].vCentreScale[1]   = 1.0;
         pBoneChunk[i].vCentreScale[2]   = 1.0;
         pBoneChunk[i].vRotationScale[0] = 1.0;
         pBoneChunk[i].vRotationScale[1] = 1.0;
         pBoneChunk[i].vRotationScale[2] = 1.0;*/
         // Set Flags
         pBoneChunk[i].iFlags = 0;
      }
   }
   sHeader.iLength += sizeof(SHalflifeMDLBone) * sHeader.iNumBones;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating bonecontrollers");

   // Create BoneController Chunk
   sHeader.iNumBoneControllers = 4;
   sHeader.iBoneControllerChunkOffset = sHeader.iLength;
   SHalflifeMDLBoneController* pBoneControllerChunk;
   NEWBEGIN
   pBoneControllerChunk = new SHalflifeMDLBoneController[sHeader.iNumBoneControllers];
	NEWEND("CAvatarFileHalflife::Save - Bone Controller Chunk Allocation")
   if (pBoneControllerChunk == NULL) iRetVal = 0;
   else {   
      for (i=0; i<sHeader.iNumBoneControllers; i++) {
         // Set bone link
         pBoneControllerChunk[i].iBone  = m_pReverseCompressedSkeletonMap[vl5] + i;
         // Set bone's link to this controller
         pBoneChunk[m_pReverseCompressedSkeletonMap[vl5]+i].iBoneController[0] = i;
         // Set type = XR
         pBoneControllerChunk[i].iType  = 0x08;
         // Set range
         pBoneControllerChunk[i].fStart = -30;
         pBoneControllerChunk[i].fEnd   = 30;
         // Set rest value
         pBoneControllerChunk[i].iRest  = 0;
         // Set index
         pBoneControllerChunk[i].iIndex = i;
      }
   }
   sHeader.iLength += sizeof(SHalflifeMDLBoneController) * sHeader.iNumBoneControllers;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating attachments");

   // Create Attachments Chunk
   sHeader.iNumAttachments = 3;
   sHeader.iAttachmentChunkOffset = sHeader.iLength;
   SHalflifeMDLAttachment* pAttachmentChunk;
   NEWBEGIN
   pAttachmentChunk = new SHalflifeMDLAttachment[sHeader.iNumAttachments];
	NEWEND("CAvatarFileHalflife::Save - Attachment Chunk Allocation")
   if (pAttachmentChunk == NULL) iRetVal = 0;
   else {
      for (i=0; i<sHeader.iNumAttachments; i++) {
         // Set bone
         pAttachmentChunk[i].iBone = m_pReverseCompressedSkeletonMap[r_wrist];
         // Set type
         pAttachmentChunk[i].iType = 0;
         // Set name amd position
         switch (i) {
         case 0:
            strncpy(pAttachmentChunk[i].pszName,"Attachment 0",32);
            pAttachmentChunk[i].vPosition[0] = 20.0;
            pAttachmentChunk[i].vPosition[1] = 2.0;
            pAttachmentChunk[i].vPosition[2] = 5.0;
            break;
         case 1:
            strncpy(pAttachmentChunk[i].pszName,"Attachment 1",32);
            pAttachmentChunk[i].vPosition[0] = 15.0;
            pAttachmentChunk[i].vPosition[1] = 1.5;
            pAttachmentChunk[i].vPosition[2] = 3.75;
            break;
         case 2:
            strncpy(pAttachmentChunk[i].pszName,"Attachment 2",32);
            pAttachmentChunk[i].vPosition[0] = 30.0;
            pAttachmentChunk[i].vPosition[1] = 3.0;
            pAttachmentChunk[i].vPosition[2] = 7.5;
            break;
         default:
            strncpy(pAttachmentChunk[i].pszName,"",32);
            pAttachmentChunk[i].vPosition[0] = 0.0;
            pAttachmentChunk[i].vPosition[1] = 0.0;
            pAttachmentChunk[i].vPosition[2] = 0.0;
            break;
         }
         // Zero other vectors
         pAttachmentChunk[i].vVector0[0] = 0.0;
         pAttachmentChunk[i].vVector0[1] = 0.0;
         pAttachmentChunk[i].vVector0[2] = 0.0;
         pAttachmentChunk[i].vVector1[0] = 0.0;
         pAttachmentChunk[i].vVector1[1] = 0.0;
         pAttachmentChunk[i].vVector1[2] = 0.0;
         pAttachmentChunk[i].vVector2[0] = 0.0;
         pAttachmentChunk[i].vVector2[1] = 0.0;
         pAttachmentChunk[i].vVector2[2] = 0.0;
      }
   }
   sHeader.iLength += sizeof(SHalflifeMDLAttachment) * sHeader.iNumAttachments;

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating hitboxes");

   // Create Hitbox Chunk
   const int piHitGroups[] = {3,3,6,6,6,7,7,7,3,2,2,2,2,1,4,4,4,4,5,5,5,5};
   sHeader.iNumHitboxes = sHeader.iNumBones;
   sHeader.iHitboxChunkOffset = sHeader.iLength;
   SHalflifeMDLHitbox* pHitboxChunk;
   NEWBEGIN
   pHitboxChunk = new SHalflifeMDLHitbox[sHeader.iNumHitboxes];
	NEWEND("CAvatarFileHalflife::Save - Hitbox Chunk Allocation")
   if (pHitboxChunk == NULL) iRetVal = 0;
   else {
      for (i=0; i<sHeader.iNumHitboxes; i++) {
         // Write hitbox info
         pHitboxChunk[i].iBone  = i;
         pHitboxChunk[i].iGroup = piHitGroups[i];
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
         pHitboxChunk[i].vMinimum[0] =  vecMin.X();
         pHitboxChunk[i].vMinimum[1] =  vecMin.Z();
         pHitboxChunk[i].vMinimum[2] = -vecMin.Y();
         // Write maximum
         pHitboxChunk[i].vMaximum[0] =  vecMax.X();
         pHitboxChunk[i].vMaximum[1] =  vecMax.Z();
         pHitboxChunk[i].vMaximum[2] = -vecMax.Y();
      }
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
   SHalflifeMDLSeqGroup* pSeqGroupsChunk;
   NEWBEGIN
   pSeqGroupsChunk = new SHalflifeMDLSeqGroup[sHeader.iNumSeqGroups];
	NEWEND("CAvatarFileHalflife::Save - Sequence Groups Chunk Allocation")
   if (pSeqGroupsChunk == NULL) iRetVal = 0;
   else {
      for (i=0; i<sHeader.iNumSeqGroups; i++) {
         // Name
         strncpy(pSeqGroupsChunk[i].pszName,"default",32);
         // Filename
         strncpy(pSeqGroupsChunk[i].pszFilename,"",64);
         // Cache Pointer
         pSeqGroupsChunk[i].iCachePtr = 0;
         // Data
         pSeqGroupsChunk[i].iData = 0;
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
         iRetVal = 0; 
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
   char* pcModelDataChunk;
   NEWBEGIN
   pcModelDataChunk = new char[iModelDataChunkLength];
	NEWEND("CAvatarFileHalflife::Save - Model Data Chunk Allocation")
   if (pcModelDataChunk == NULL) iRetVal = 0;
   else {
      memset(pcModelDataChunk,0,iModelDataChunkLength);
      iCurrentPos = 0;
      // Write vertex info
      for (i=0; i<iNumVertices; i++) {
         pcModelDataChunk[iCurrentPos+i] = m_pReverseCompressedSkeletonMap[pAvatar->GetVertexPart(i)];
      }
      iCurrentPos += iVertexInfoLength;
      // Write normal info
      for (i=0; i<iNumVertices; i++) {
         pcModelDataChunk[iCurrentPos+i] = m_pReverseCompressedSkeletonMap[pAvatar->GetVertexPart(i)];
      }
      iCurrentPos += iNormalInfoLength;
      // Write vertex data
      float* pfDataPtr = (float*)(pcModelDataChunk+iCurrentPos);
      for (i=0; i<iNumVertices; i++) {
         BodyPart bpPart = (BodyPart)pAvatar->GetVertexPart(i);
         // Get vertex position
         CVector3D vecVertex(pVertices[i]);
         // Work out offset from bone centre
         vecVertex -= CVector3D(pBodyParts[bpPart].m_pntCurrentCentre);
         // Scale
         vecVertex *= dScaleFactor;
         // Write
         *(pfDataPtr++) =  vecVertex.X();
         *(pfDataPtr++) =  vecVertex.Z();
         *(pfDataPtr++) = -vecVertex.Y();
      }
      iCurrentPos += iVertexDataLength;
      // Write normal data
      pfDataPtr = (float *)(pcModelDataChunk+iCurrentPos);
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
   SHalflifeMDLMesh* pMeshHeaderChunk = NULL;
   char* pcMeshDataChunk = NULL;
   int iMeshDataChunkLength = 0;
   // Split main mesh into submeshes based on texture
   std::vector<STriFace>* vSubMeshes;
   NEWBEGIN
   vSubMeshes = new std::vector<STriFace>[sModelHeader.iNumMeshes];
	NEWEND("CAvatarFileHalflife::Save - Submesh Allocation")
   if (vSubMeshes == NULL) iRetVal = 0;
   else {
      for (i=0; i<iNumFaces; i++) {
         (vSubMeshes + pFaces[i].m_iTextureNumber)->push_back(pFaces[i]);
      }
      // Compress submeshes into triangle strips
      // ********NOT IMPLEMENTED YET*********

      // Create Mesh Header Chunk
      sModelHeader.iMeshIndex = sHeader.iLength;
      NEWBEGIN
      pMeshHeaderChunk = new SHalflifeMDLMesh[sModelHeader.iNumMeshes];
	   NEWEND("CAvatarFileHalflife::Save - Mesh Header Chunk Allocation")
      if (pMeshHeaderChunk == NULL) iRetVal = 0;
      else {
         // Write info into header
         for (i=0; i<sModelHeader.iNumMeshes; i++) {
            // Number of triangles
            pMeshHeaderChunk[i].iNumTriangles = vSubMeshes[i].size();
            // We fill in the triangle index in a minute
            // Texture reference
            pMeshHeaderChunk[i].iSkinRef = i;
            // Number of normals
            pMeshHeaderChunk[i].iNumNormals = vSubMeshes[i].size();
            // Normal index
            pMeshHeaderChunk[i].iNormalIndex = 0;
         }
      }
      sHeader.iLength += sizeof(SHalflifeMDLMesh) * sModelHeader.iNumMeshes;
   
      g_poVAL->StepProgress("HLSave");
      g_poVAL->SetProgressText("HLSave", "Creating mesh data");

      // Create Mesh Data Chunk
      int iMeshDataChunkStart = sHeader.iLength;
      for (i=0; i<sModelHeader.iNumMeshes; i++) {
         // Fill in index in header
         pMeshHeaderChunk[i].iTriangleDataIndex = sHeader.iLength;
         // Work our mesh length
         int iMeshLength = (vSubMeshes[i].size() * 26) + 2;
         WORDALIGN(iMeshLength);
         iMeshDataChunkLength += iMeshLength;
         sHeader.iLength += iMeshLength;
      }
      NEWBEGIN
      pcMeshDataChunk = new char[iMeshDataChunkLength];
	   NEWEND("CAvatarFileHalflife::Save - Mesh Data Chunk Allocation")
      if (pcMeshDataChunk == NULL) iRetVal = 0;
      else {
         memset(pcMeshDataChunk,0,iMeshDataChunkLength);
         for (i=0; i<sModelHeader.iNumMeshes; i++) {
            int iCurrentOffset = pMeshHeaderChunk[i].iTriangleDataIndex - iMeshDataChunkStart;
            short* pcTempMeshDataPtr = (short*)(pcMeshDataChunk + iCurrentOffset);
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
      }
      // Bin submesh and tristrip information
      delete [] vSubMeshes;
   }

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Creating texture info");

   // Predeclare stuff
   char* pcTextureIndexChunk = NULL;
   char* pcTextureDataChunk = NULL;
   int iTextureIndexChunkLength = 0;
   int iTextureDataChunkLength = 0;
   // Create Texture Header Chunk
   sHeader.iNumTextures = pAvatar->NumTextures();
   sHeader.iTextureHeaderChunkOffset = sHeader.iLength;
   SHalflifeMDLTexture* pTextureHeaderChunk;
   NEWBEGIN
   pTextureHeaderChunk = new SHalflifeMDLTexture[sHeader.iNumTextures];
	NEWEND("CAvatarFileHalflife::Save - Texture Header Chunk Allocation")
   if (pTextureHeaderChunk == NULL) iRetVal = 0;
   else {      
      for (i=0; i<sHeader.iNumTextures; i++) {
         // Set flags
         pTextureHeaderChunk[i].iFlags = 0;
         // Write name
         memset(pTextureHeaderChunk[i].pszName,0,64);
         strcpy(pTextureHeaderChunk[i].pszName,m_pszModelname);
         int iLength = strlen(pTextureHeaderChunk[i].pszName);
         strcpy(pTextureHeaderChunk[i].pszName + iLength, "_a.bmp");
         pTextureHeaderChunk[i].pszName[iLength + 1] += i;
         // Get image size
         int iWidth, iHeight;
         vpSmallTextures[i]->GetSize(iWidth,iHeight);
         // Store data
         pTextureHeaderChunk[i].iWidth = iWidth;
         pTextureHeaderChunk[i].iHeight = iHeight;
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
      pcTextureIndexChunk = new char[iTextureIndexChunkLength];
	   NEWEND("CAvatarFileHalflife::Save - Texture Index Chunk Allocation")
      if (pcTextureIndexChunk == NULL) iRetVal = 0;
      else {
         memset(pcTextureIndexChunk,0,iTextureIndexChunkLength);
         for (i=0; i<sHeader.iNumReplaceableTextures; i++) {
            ((short*)pcTextureIndexChunk)[i] = i;
         }
      }
      sHeader.iLength += iTextureIndexChunkLength;

      g_poVAL->StepProgress("HLSave");
      g_poVAL->SetProgressText("HLSave", "Storing textures");

      // Create Texture Data Chunk
      sHeader.iTextureDataChunkOffset = sHeader.iLength;
      NEWBEGIN
      pcTextureDataChunk = new char[iTextureDataChunkLength];
	   NEWEND("CAvatarFileHalflife::Save - Texture Data Chunk Allocation")
      if (pcTextureDataChunk == NULL) iRetVal = 0;
      else {
         iCurrentPos = 0;
         for (i=0; i<sHeader.iNumTextures; i++) {
            const CImage* pTexture = vpSmallTextures[i];
            // Store index in header
            pTextureHeaderChunk[i].iIndex = sHeader.iLength + iCurrentPos;
            // Write texture data
            int iTextureSize = pTextureHeaderChunk[i].iWidth * pTextureHeaderChunk[i].iHeight;
            for (int iPixel=0; iPixel<iTextureSize; iPixel++) {
               *(pcTextureDataChunk+(iCurrentPos++)) = (char)pTexture->GetPixel(iPixel);
            }            
            // Write palette
            const CImagePalette* pPalette = pTexture->GetPalette();
            for (int iEntry=0; iEntry<256; iEntry++) {
               /*memset(pcTextureDataChunk+iCurrentPos,iEntry,0x03);
               iCurrentPos += 3;*/
               unsigned long uColour;
               pPalette->GetEntry(iEntry,uColour);
               *(pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 16) & 0xFF;
               *(pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 8) & 0xFF;
               *(pcTextureDataChunk+(iCurrentPos++)) = uColour & 0xFF;
            }
         }
      }
      sHeader.iLength += iTextureDataChunkLength;
   }

   // Dispose of resized textures
   for (i=0; i<pAvatar->NumTextures(); i++) {
      CImage* pTexture = vpSmallTextures.back();
      vpSmallTextures.pop_back();
      delete pTexture;
   }

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Writing to file");

   // Write chunks to stream in correct order
   if (iRetVal != 0) {
      osOutputStream.write((char*)&sHeader,sizeof(SHalflifeMDLHeader));
      osOutputStream.write((char*)pBoneChunk,sizeof(SHalflifeMDLBone) * sHeader.iNumBones);
      osOutputStream.write((char*)pBoneControllerChunk,sizeof(SHalflifeMDLBoneController) * sHeader.iNumBoneControllers);
      osOutputStream.write((char*)pAttachmentChunk,sizeof(SHalflifeMDLAttachment) * sHeader.iNumAttachments);
      osOutputStream.write((char*)pHitboxChunk,sizeof(SHalflifeMDLHitbox) * sHeader.iNumHitboxes);
			if (oWedgie.Extract("hlmdlanimdata.bin", osOutputStream) != WJE_OK)
				iRetVal = 0;
      osOutputStream.write((char*)pSeqGroupsChunk,sizeof(SHalflifeMDLSeqGroup) * sHeader.iNumSeqGroups);
      osOutputStream.write((char*)&sBodyPartHeader,sizeof(SHalflifeMDLBodyPart));
      osOutputStream.write((char*)&sModelHeader,sizeof(SHalflifeMDLModel));
      osOutputStream.write(pcModelDataChunk,iModelDataChunkLength);
      osOutputStream.write((char*)pMeshHeaderChunk,sizeof(SHalflifeMDLMesh) * sModelHeader.iNumMeshes);
      osOutputStream.write(pcMeshDataChunk,iMeshDataChunkLength);
      osOutputStream.write((char*)pTextureHeaderChunk,sizeof(SHalflifeMDLTexture) * sHeader.iNumTextures);
      osOutputStream.write(pcTextureIndexChunk,iTextureIndexChunkLength);
      osOutputStream.write(pcTextureDataChunk,iTextureDataChunkLength);
   }

   g_poVAL->StepProgress("HLSave");
   g_poVAL->SetProgressText("HLSave", "Cleaning up");

   // Clean up memory
   m_vCompressedSkeletonMap.clear();
   if (pcTextureDataChunk != NULL)   delete [] pcTextureDataChunk;
   if (pcTextureIndexChunk != NULL)  delete [] pcTextureIndexChunk;
   if (pTextureHeaderChunk != NULL)  delete [] pTextureHeaderChunk;
   if (pcMeshDataChunk != NULL)      delete [] pcMeshDataChunk;
   if (pMeshHeaderChunk != NULL)     delete [] pMeshHeaderChunk;
   if (pcModelDataChunk != NULL)     delete [] pcModelDataChunk;
   if (pSeqGroupsChunk != NULL)      delete [] pSeqGroupsChunk;
   if (pHitboxChunk != NULL)         delete [] pHitboxChunk;
   if (pAttachmentChunk != NULL)     delete [] pAttachmentChunk;
   if (pBoneControllerChunk != NULL) delete [] pBoneControllerChunk;
   if (pBoneChunk != NULL)           delete [] pBoneChunk;

   // Restore old stream mode
   if (iOldMode != -1) osOutputStream.setmode(iOldMode);

   // Restore old pose
   pAvatar->ImportPose(oldPose);
   pAvatar->UpdateModel();

	 // Close the wedgie and file
	 oWedgie.Close();
	 oDataWJE.close();

   // Done
   return iRetVal;
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

std::vector<vTriStrip> CAvatarFileHalflife::CompressSubMesh(std::vector<STriFace>& vSubMesh) const {  
   std::vector<vTriStrip> vvTriStrips;
   bool* pbUsedFaces = new bool[vSubMesh.size()];
   memset(pbUsedFaces,0,vSubMesh.size());
   int* piConnectivity = new int[vSubMesh.size()];
   
   delete [] piConnectivity;
   delete [] pbUsedFaces;
   return vvTriStrips;
} //CompressSubMesh(std::vector<STriFace> vSubMesh)

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
