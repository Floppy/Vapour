//=======---
// Avanew
//-------
// Avatar editor and exporter
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHalflife.cpp - 16/2/2000 - James Smith
//	Halflife export filter implementation
//
// $Id: AvatarFileHalflife.cpp,v 1.1 2000/07/11 16:42:24 waz Exp $
//


#include "stdafx.h"

#include "AvatarFileHalflife.h"
#include "AvatarFileProxy.h"
#include "HalflifeMdl.h"
#include "Image.h"
#include "ImageFileStore.h"

#include <float.h>
#include <direct.h>
#include <errno.h>

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

CAvatarFileHalflife::CAvatarFileHalflife() : m_iScaleFactor(43) {
   m_pProgressDlg = NULL;
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
   char* pszLocalFilename = strdup(pszFilename);
   char* pszTemp = pszLocalFilename;
   // Find model name
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   int iLength = (strchr(pszTemp,'.') - pszTemp);
   m_pszModelname = new char[iLength+1];
   memset(m_pszModelname,0,iLength+1);
   for (int i=0; i<iLength; i++) m_pszModelname[i] = pszTemp[i];
   // Chop off file extension to create direcory name
   pszTemp = pszLocalFilename;
   while (strchr(pszTemp,'.') != NULL) {
      pszTemp = strchr(pszTemp,'.') + 1;
   }
   *(pszTemp-1) = (char)'\\';
   *pszTemp = (char)'\0';
   // pszLocalFilename is now the output directory!
   // Create a filename string (with space for an extension)
   char* pszBaseFilename = new char[strlen(pszLocalFilename) + strlen(m_pszModelname) + 5];
   strcpy(pszBaseFilename,pszLocalFilename);
   strcpy(pszBaseFilename + strlen(pszLocalFilename), m_pszModelname);
   int iBaseFilenameLength = strlen(pszBaseFilename);

   // Create output subdirectory
   int iResult = _mkdir(pszLocalFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create output directory!\n");
      return 0;
   }
   
   // Setup the export progress dialog
   m_pProgressDlg = new CProgressDialog;
   m_pProgressDlg->Setup(IDB_HALFLIFE,17 + 2*pAvatar->NumTextures());
   m_pProgressDlg->Step();
   m_pProgressDlg->SetText("Saving MDL file");
   
   // Create filename
   strcpy(pszBaseFilename+iBaseFilenameLength,".mdl");
   // Save to output stream
   ofstream osOutputStream(pszBaseFilename);
	if (osOutputStream.good()) {
   	iRetVal = Save(osOutputStream,pAvatar);
	}

   // Create thumbnail
   m_pProgressDlg->Step();
   m_pProgressDlg->SetText("Rendering thumbnail");
   CImage imThumbnail(IT_PALETTE,164,200,256);

   // Write thumbnail
   m_pProgressDlg->Step();
   m_pProgressDlg->SetText("Writing thumbnail");
   // Create filename
   strcpy(pszBaseFilename+iBaseFilenameLength,".bmp");
   // Create filter and save
   CImageFile* pThumbFilter = g_oImageFileStore.CreateByExtension("bmp");
   imThumbnail.Save(pszBaseFilename,pThumbFilter);
   // Tidy up
   delete pThumbFilter;

   // Finish up
   m_pProgressDlg->Step();
   delete m_pProgressDlg;
   m_pProgressDlg = NULL;
   // Delete strings
   free(pszLocalFilename);
   delete [] pszBaseFilename;
   delete [] m_pszModelname;
   m_pszModelname = NULL;
   // DOne
   return iRetVal;
} // Save(const char* pszFilename, CAvatar* pAvatar)

int CAvatarFileHalflife::Save(ofstream& osOutputStream, CAvatar* pAvatar) const {
   int iRetVal = 1;
   int iOffset = 0;     // File offset counter
   int i=0;             // loop variable
   int iCurrentPos = 0; // Temporary counter

   // Store old pose
   CAvatarPose oldPose = pAvatar->ExportPose();
   pAvatar->ResetPose();
   pAvatar->UpdateModel();

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
   pAvatar->SetRootTranslation(CVector3D(0,-(pAvatar->Height()/2.0),0));
   pAvatar->UpdateModel();
   
   pAvatar->CalculateNormals(true);


   // Force binary stream mode
   int iOldMode = osOutputStream.setmode(filebuf::binary);

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating header");
   }

   // Create Header & fill in a few fields
   HlMDLHeader sHeader;
   sHeader.liID = HLMDLMAGIC;
   sHeader.liVersion = HLMDLVERSION;
   memset(sHeader.psName,0,64);
   strcpy(sHeader.psName,m_pszModelname);
   memset(sHeader.cVectorsAndFlags,0,64);

   // Deal with sound chunk stuff
   sHeader.liSoundTable = 0;
   sHeader.liSoundOffset = 0;
   sHeader.liSoundGroups = 0;
   sHeader.liSoundGroupOffset = 0;

   // Add header to file offset counter
   iOffset += sizeof(HlMDLHeader);

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating bones");
   }

   // Create Bone Chunk
   m_vCompressedSkeletonMap.clear();
   CompressSkeleton(root,pAvatar);
   sHeader.liNumBones = m_vCompressedSkeletonMap.size();
   sHeader.liBoneChunkOffset = iOffset;
   int iBoneChunkLength = 0x70 * sHeader.liNumBones;
   char* pcBoneChunk = new char[iBoneChunkLength];
   ifstream ifBoneStream("hlmdlbonedata.bin",ios::in|ios::binary);
   ifBoneStream.read(pcBoneChunk,iBoneChunkLength);
   ifBoneStream.close();
   iCurrentPos = 0;
   const SBodyPart* pBodyParts = pAvatar->BodyParts();
   // Pose model
   UEulerType etType;
   etType.m_eIdentifier = XYZs;
   //for (i=0; i<sHeader.liNumBones; i++) {
   //   float* pfBonePtr = (float*)(pcBoneChunk + i*0x70);
   //   CEulerRotation erRotation(pfBonePtr[19],pfBonePtr[21],pfBonePtr[20],etType);
   //   CAxisRotation arRotation(erRotation);
   //   pAvatar->SetJointAngle((BodyPart)m_vCompressedSkeletonMap[i],arRotation,false);
   //   pAvatar->UpdateModel();
   //}
   for (i=0; i<sHeader.liNumBones; i++) {
      BodyPart bpCurrentBodyPart = (BodyPart)m_vCompressedSkeletonMap[i];
      char* pcTempBoneChunkPtr = pcBoneChunk + i*0x70;
      // Parent
      BodyPart bpParent = pBodyParts[bpCurrentBodyPart].m_bpParent;
      // Position
      float diffx, diffy, diffz;
      if (bpParent == unknown) {
         diffx = pBodyParts[bpCurrentBodyPart].m_pntCurrentCentre.m_dComponents[0];
         diffy = pBodyParts[bpCurrentBodyPart].m_pntCurrentCentre.m_dComponents[1];
         diffz = pBodyParts[bpCurrentBodyPart].m_pntCurrentCentre.m_dComponents[2];
      }
      else {
         diffx = pBodyParts[bpCurrentBodyPart].m_pntCurrentCentre.m_dComponents[0] - pBodyParts[bpParent].m_pntCurrentCentre.m_dComponents[0];
         diffy = pBodyParts[bpCurrentBodyPart].m_pntCurrentCentre.m_dComponents[1] - pBodyParts[bpParent].m_pntCurrentCentre.m_dComponents[1];
         diffz = pBodyParts[bpCurrentBodyPart].m_pntCurrentCentre.m_dComponents[2] - pBodyParts[bpParent].m_pntCurrentCentre.m_dComponents[2];
      }
      ((float*)pcTempBoneChunkPtr)[16] =  (diffx * m_iScaleFactor);
      ((float*)pcTempBoneChunkPtr)[17] = -(diffz * m_iScaleFactor);
      ((float*)pcTempBoneChunkPtr)[18] =  (diffy * m_iScaleFactor);
   }
   iOffset += iBoneChunkLength;

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating bonecontrollers");
   }

   // Create BoneController Chunk
   sHeader.liNumBoneControllers = 4;
   sHeader.liBoneControllerChunkOffset = iOffset;
   int iBoneControllerChunkLength = 0x18 * sHeader.liNumBoneControllers;
   char* pcBoneControllerChunk = new char[iBoneControllerChunkLength];
   memset(pcBoneControllerChunk,0,iBoneControllerChunkLength);
   for (i=0; i<sHeader.liNumBoneControllers; i++) {
      char* pcTempBoneControllerPtr = pcBoneControllerChunk + i*0x18;
      ((int*)pcTempBoneControllerPtr)[0] = m_pReverseCompressedSkeletonMap[vl5] + i;
      *((int*)(pcBoneChunk + (m_pReverseCompressedSkeletonMap[vl5]+i)*0x70 + 0x34)) = i;
      ((int*)pcTempBoneControllerPtr)[1] = 0x08;
      ((float*)pcTempBoneControllerPtr)[2] = -30;
      ((float*)pcTempBoneControllerPtr)[3] = 30;
      ((int*)pcTempBoneControllerPtr)[5] = i;
   }
   iOffset += iBoneControllerChunkLength;

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating attachments");
   }

   // Create Attachments Chunk
   sHeader.liNumAttachments = 3;
   sHeader.liAttachmentChunkOffset = iOffset;
   int iAttachmentChunkLength = 0x58 * sHeader.liNumAttachments;
   char* pcAttachmentChunk = new char[iAttachmentChunkLength];
   memset(pcAttachmentChunk,0,iAttachmentChunkLength);
   for (i=0; i<sHeader.liNumAttachments; i++) {
      char* pcTempAttachmentChunkPtr = pcAttachmentChunk + i*0x58;
      ((int*)pcTempAttachmentChunkPtr)[9] = m_pReverseCompressedSkeletonMap[r_wrist];
      switch (i) {
      case 0:
         strcpy(pcTempAttachmentChunkPtr,"Attachment 0");
         ((float*)pcTempAttachmentChunkPtr)[10] = 20;
         ((float*)pcTempAttachmentChunkPtr)[11] = 2;
         ((float*)pcTempAttachmentChunkPtr)[12] = 5;
         break;
      case 1:
         strcpy(pcTempAttachmentChunkPtr,"Attachment 1");
         ((float*)pcTempAttachmentChunkPtr)[10] = 15;
         ((float*)pcTempAttachmentChunkPtr)[11] = 1.5;
         ((float*)pcTempAttachmentChunkPtr)[12] = 3.75;
         break;
      case 2:
         strcpy(pcTempAttachmentChunkPtr,"Attachment 2");
         ((float*)pcTempAttachmentChunkPtr)[10] = 30;
         ((float*)pcTempAttachmentChunkPtr)[11] = 3;
         ((float*)pcTempAttachmentChunkPtr)[12] = 7.5;
         break;
      default:
         break;
      }
   }
   iOffset += iAttachmentChunkLength;

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating hitboxes");
   }

   // Create Hitbox Chunk
   sHeader.liNumHitboxes = sHeader.liNumBones;
   sHeader.liHitboxChunkOffset = iOffset;
   const int piHitGroups[] = {3,3,6,6,6,7,7,7,3,2,2,2,2,1,4,4,4,4,5,5,5,5};
   int iHitboxChunkLength = 0x20 * sHeader.liNumHitboxes;
   char* pcHitboxChunk = new char[iHitboxChunkLength];
   memset(pcHitboxChunk,0,iHitboxChunkLength);
   for (i=0; i<sHeader.liNumHitboxes; i++) {
      // Write hitbox info
      char* pcTempHitboxChunkPtr = pcHitboxChunk + i*0x20;
      ((int*)pcTempHitboxChunkPtr)[0] = i;
      ((int*)pcTempHitboxChunkPtr)[1] = piHitGroups[i];
      // Calc bounding box
      SPoint3D min, max;
      pAvatar->BoundingBox((BodyPart)m_vCompressedSkeletonMap[i],max,min);
      CVector3D vecMin(min);
      CVector3D vecMax(max);      
      // Work out offsets from bone centre
      CVector3D vecCentre(pBodyParts[m_vCompressedSkeletonMap[i]].m_pntCurrentCentre);
      vecMin -= vecCentre;
      vecMax -= vecCentre;
      // Rotate vectors
      CAxisRotation arRotation(1,0,0,V_PI);
      vecMin = vecMin.Rotate(arRotation);
      vecMax = vecMax.Rotate(arRotation);
      // Convert to doubles for output
      double dMinX,dMinY,dMinZ,dMaxX,dMaxY,dMaxZ;
      vecMin.ToDouble(dMinX, dMinY, dMinZ);
      vecMax.ToDouble(dMaxX, dMaxY, dMaxZ);
      // Write minimum
      ((float*)pcTempHitboxChunkPtr)[2] =  dMinX * m_iScaleFactor;
      ((float*)pcTempHitboxChunkPtr)[3] = -dMinZ * m_iScaleFactor;
      ((float*)pcTempHitboxChunkPtr)[4] =  dMinY * m_iScaleFactor;
      // Write maximum
      ((float*)pcTempHitboxChunkPtr)[5] =  dMaxX * m_iScaleFactor;
      ((float*)pcTempHitboxChunkPtr)[6] = -dMaxZ * m_iScaleFactor;
      ((float*)pcTempHitboxChunkPtr)[7] =  dMaxY * m_iScaleFactor;
   }
   iOffset += iHitboxChunkLength;

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Loading animations");
   }
   // Create Animation Sequence Header Chunk
   sHeader.liNumSeqs = 77;
   ifstream ifAnimStream("hlmdlanimdata.bin",ios::in|ios::binary);
   iOffset += 0x2DB84;//0x2DFB8;
   sHeader.liSeqChunkOffset = iOffset;
   iOffset += (sHeader.liNumSeqs * 0xB0) + 0x474;

   // Create Sequence Groups Chunk
   sHeader.liNumSeqGroups = 1;
   sHeader.liSeqGroupChunkOffset = iOffset;
   int iSequenceGroupsChunkLength = 0x68 * sHeader.liNumSeqGroups;
   char* pcSequenceGroupsChunk = new char[iSequenceGroupsChunkLength];
   memset(pcSequenceGroupsChunk,0,0x68);
   strcpy(pcSequenceGroupsChunk,"default");
   iOffset += iSequenceGroupsChunkLength;

   // Deal with Transition Chunk (but don't create anything)
   sHeader.liNumTransitions = 0;
   sHeader.liTransitionChunkOffset = iOffset;
   
   // Process textures, cos we'll need them in a minute
   std::vector<CImage*> vpSmallTextures;
   for (i=0; i<pAvatar->NumTextures(); i++) {
      // Notify of progress
      if (m_pProgressDlg != NULL) {
         char pszDisplay[32];
         sprintf(pszDisplay,"Converting texture %d", i);
         m_pProgressDlg->Step();
         m_pProgressDlg->SetText(pszDisplay);
      }
      // Downsize images
      CImage* pTexture;
      pTexture = new CImage(*(pAvatar->Texture(i)));
      int iWidth = 0;
      int  iHeight = 0;
      pTexture->GetSize(iWidth,iHeight);
      if (i == pAvatar->TextureIndex(skullbase) || i == pAvatar->TextureIndex(vl5)) {
         // Set size of face and torso textures to 128x128
         iWidth  = 128;
         iHeight = 128;
      }
      else {
         // Quarter size of other textures
         iWidth  = iWidth  >> 2;
         iHeight = iHeight >> 2;
         // Max 64x64
         if (iWidth > 64) iWidth = 64;
         if (iHeight > 64) iHeight = 64;
      }
      // Scale
      pTexture->Scale(iWidth,iHeight);
      // Flip
      pTexture->Flip();
      // Reduce colours
      //pTexture->Convert(IT_PALETTE,256);
      // Store
      vpSmallTextures.push_back(pTexture);
   }
   
   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating body parts");
   }

   // Create Body Parts Header Chunk
   sHeader.liNumBodyParts = 1;
   sHeader.liBodyPartChunkOffset = iOffset;
   int iBodyPartsHeaderChunkLength = 0x4C;
   char* pcBodyPartsHeaderChunk = new char[iBodyPartsHeaderChunkLength];
   memset(pcBodyPartsHeaderChunk,0,iBodyPartsHeaderChunkLength);
   strcpy(pcBodyPartsHeaderChunk,"body");
   ((int*)pcBodyPartsHeaderChunk)[16] = 1;
   ((int*)pcBodyPartsHeaderChunk)[17] = 1;
   iOffset += iBodyPartsHeaderChunkLength;
   ((int*)pcBodyPartsHeaderChunk)[18] = iOffset;

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating model");
   }

   // Create Model Header Chunk
   int iModelHeaderChunkLength = 0x70;
   char* pcModelHeaderChunk = new char[iModelHeaderChunkLength];
   memset(pcModelHeaderChunk,0,iModelHeaderChunkLength);
   iOffset += iModelHeaderChunkLength;
   strcpy(pcModelHeaderChunk,m_pszModelname);
   int iNumMeshes = pAvatar->NumTextures();
   ((int*)pcModelHeaderChunk)[18] = iNumMeshes;
   int iNumVertices = pAvatar->NumVertices();
   ((int*)pcModelHeaderChunk)[20] = iNumVertices;
   ((int*)pcModelHeaderChunk)[23] = iNumVertices;
   // We fill more in later

   // Create Model Data Chunk
   int iVertexInfoLength = iNumVertices;
   WORDALIGN(iVertexInfoLength);
   int iNormalInfoLength = iNumVertices;
   WORDALIGN(iNormalInfoLength);
   int iVertexDataLength = iNumVertices * HLSIZEOFVECTOR;
   WORDALIGN(iVertexDataLength);
   int iNormalDataLength = iNumVertices * HLSIZEOFVECTOR;
   WORDALIGN(iNormalDataLength);
   ((int*)pcModelHeaderChunk)[21] = iOffset;
   ((int*)pcModelHeaderChunk)[24] = iOffset + iVertexInfoLength;
   ((int*)pcModelHeaderChunk)[22] = iOffset + iVertexInfoLength + iNormalInfoLength;
   ((int*)pcModelHeaderChunk)[25] = iOffset + iVertexInfoLength + iNormalInfoLength + iVertexDataLength;
   int iModelDataChunkLength = iVertexInfoLength + iNormalInfoLength + iVertexDataLength + iNormalDataLength;
   char* pcModelDataChunk = new char[iModelDataChunkLength];
   memset(pcModelDataChunk,0,iModelDataChunkLength);
   iCurrentPos = 0;
   // Build vertex->segment map
   int* piVertexToSegmentMap = new int[iNumVertices];
   for (i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
      int iNumBodyPartVertices = pBodyParts[i].m_iNumVertices;
      if (iNumBodyPartVertices != 0) {
         for (int j=0; j<iNumBodyPartVertices; j++) {
            piVertexToSegmentMap[pBodyParts[i].m_piVertices[j]] = m_pReverseCompressedSkeletonMap[i];
         }
      }
   }
   // Write vertex info
   for (i=0; i<iNumVertices; i++) {
      pcModelDataChunk[iCurrentPos + i] = piVertexToSegmentMap[i];
   }
   iCurrentPos += iVertexInfoLength;
   // Write normal info
   for (i=0; i<iNumVertices; i++) {
      pcModelDataChunk[iCurrentPos + i] = piVertexToSegmentMap[i];
   }
   iCurrentPos += iNormalInfoLength;
   char* pcTempModelDataPtr = pcModelDataChunk + iCurrentPos;
   // Write vertex data
   const SPoint3D* pVertices = pAvatar->Vertices();
   for (i=0; i<iNumVertices; i++) {
      CAxisRotation arRotation(1,0,0,V_PI);
      BodyPart bpPart = (BodyPart)m_vCompressedSkeletonMap[piVertexToSegmentMap[i]];      
      double dX = pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[0];
      double dY = pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[1];
      double dZ = pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[2];
      CVector3D vecVertex(pVertices[i].m_dComponents[0]-dX,pVertices[i].m_dComponents[1]-dY,pVertices[i].m_dComponents[2]-dZ);
      vecVertex = vecVertex.Rotate(arRotation);
      vecVertex.ToDouble(dX,dY,dZ);
      ((float*)pcTempModelDataPtr)[i*3]     =  dX * m_iScaleFactor;
      ((float*)pcTempModelDataPtr)[i*3 + 1] = -dZ * m_iScaleFactor;
      ((float*)pcTempModelDataPtr)[i*3 + 2] =  dY * m_iScaleFactor;
   }
   iCurrentPos += iVertexDataLength;
   // Delete vertex->segment map
   delete [] piVertexToSegmentMap;
   // Write normal data
   pcTempModelDataPtr = pcModelDataChunk + iCurrentPos;
   const SPoint3D* pNormals = pAvatar->VertexNormals();
   for (i=0; i<iNumVertices; i++) {
      CAxisRotation arRotation(1,0,0,V_PI);
      CVector3D vecVertex(pNormals[i]);
      //vecVertex = vecVertex.Rotate(arRotation);
      double dX, dY, dZ;
      vecVertex.ToDouble(dX,dY,dZ);
      ((float*)pcTempModelDataPtr)[i*3]     = -dX;
      ((float*)pcTempModelDataPtr)[i*3 + 1] =  dZ;
      ((float*)pcTempModelDataPtr)[i*3 + 2] = -dY;
   }
   // Update offset
   iOffset += iModelDataChunkLength;
   
   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating meshes");
   }
   // Create Mesh Header Chunk
   ((int*)pcModelHeaderChunk)[19] = iOffset;
   int iMeshHeaderChunkLength = 0x14 * iNumMeshes;
   char* pcMeshHeaderChunk = new char[iMeshHeaderChunkLength];
   memset(pcMeshHeaderChunk,0,iMeshHeaderChunkLength);
   // Split main mesh into submeshes based on texture
   std::vector<STriFace>* vSubMeshes = new std::vector<STriFace>[iNumMeshes];
   int iNumFaces = pAvatar->NumFaces();
   const STriFace* pFaces = pAvatar->Faces();
   for (i=0; i<iNumFaces; i++) {
      (vSubMeshes + pFaces[i].m_iTextureNumber)->push_back(pFaces[i]);
   }
   // Compress submeshes into triangle strips
   // ********IMPLEMENT THIS*********
   // Write info into header
   for (i=0; i<iNumMeshes; i++) {
      char* pcTempPtr = pcMeshHeaderChunk + i*0x14;
      // Number of triangles
      ((int*)pcTempPtr)[0] = vSubMeshes[i].size();
      // Texture reference
      ((int*)pcTempPtr)[2] = i;
      // Number of normals
      ((int*)pcTempPtr)[3] = vSubMeshes[i].size();
   }
   iOffset += iMeshHeaderChunkLength;
   
   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating mesh data");
   }
   // Create Mesh Data Chunk
   int iMeshStartOffset = iOffset;
   int iMeshDataChunkLength = 0;
   for (i=0; i<iNumMeshes; i++) {
      *((int*)(pcMeshHeaderChunk + i*0x14 + 4)) = iOffset;
      int iMeshLength = (vSubMeshes[i].size() * 26) + 1;
      WORDALIGN(iMeshLength);
      iMeshDataChunkLength += iMeshLength;
      iOffset += iMeshLength;
   }
   char* pcMeshDataChunk = new char[iMeshDataChunkLength];
   memset(pcMeshDataChunk,0,iMeshDataChunkLength);
   for (i=0; i<iNumMeshes; i++) {
      int iCurrentOffset = *((int*)(pcMeshHeaderChunk + i*0x14 + 4)) - iMeshStartOffset;
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
      *pcTempMeshDataPtr = 0;
   }
   // Bin submesh and tristrip information
   delete [] vSubMeshes;

   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Creating texture info");
   }
   // Create Texture Header Chunk
   sHeader.liNumTextures = pAvatar->NumTextures();
   sHeader.liTextureHeaderChunkOffset = iOffset;
   int iTextureHeaderChunkLength = 0x50 * sHeader.liNumTextures;
   char* pcTextureHeaderChunk = new char[iTextureHeaderChunkLength];
   memset(pcTextureHeaderChunk,0,iTextureHeaderChunkLength);
   for (i=0; i<sHeader.liNumTextures; i++) {
      // Write filename
      iCurrentPos = i * 0x50;
      strcpy(pcTextureHeaderChunk + iCurrentPos,m_pszModelname);
      iCurrentPos += strlen(m_pszModelname);
      pcTextureHeaderChunk[iCurrentPos++]  = '_';
      pcTextureHeaderChunk[iCurrentPos++]  = 'a' + i;
      strcpy(pcTextureHeaderChunk + iCurrentPos,".bmp");
   }
   iOffset += iTextureHeaderChunkLength;

   // Create Texture Index Chunk
   sHeader.liNumReplaceableTextures = sHeader.liNumTextures;
   sHeader.liNumTextureFamilies = 1;
   sHeader.liTextureIndexOffset = iOffset;
   int iTextureIndexChunkLength = sHeader.liNumReplaceableTextures * 2;
   WORDALIGN(iTextureIndexChunkLength);
   char* pcTextureIndexChunk = new char[iTextureIndexChunkLength];
   memset(pcTextureIndexChunk,0,iTextureIndexChunkLength);
   for (i=0; i<sHeader.liNumReplaceableTextures; i++) {
      ((short*)pcTextureIndexChunk)[i] = i;
   }
   iOffset += iTextureIndexChunkLength;

   // Create Texture Data Chunk
   sHeader.liTextureDataChunkOffset = iOffset;
   int iTextureDataChunkLength = 0;
   // Calculate data chunk size
   for (i=0; i<sHeader.liNumTextures; i++) {
      int iWidth, iHeight;
      vpSmallTextures[i]->GetSize(iWidth,iHeight);
      // Add space for image @ 8bpp
      iTextureDataChunkLength += iWidth * iHeight;
      // Add space for palette
      iTextureDataChunkLength += 0x300;
   }
   char* pcTextureDataChunk = new char[iTextureDataChunkLength];
   memset(pcTextureDataChunk,0,iTextureDataChunkLength);
   iCurrentPos = 0;
   for (i=0; i<sHeader.liNumTextures; i++) {
      if (m_pProgressDlg != NULL) {
         char pszDisplay[32];
         sprintf(pszDisplay,"Storing texture %d", i);
         m_pProgressDlg->Step();
         m_pProgressDlg->SetText(pszDisplay);
      }
      // Write header information
      CImage* pTexture = vpSmallTextures[i];
      int iWidth, iHeight;
      pTexture->GetSize(iWidth,iHeight);
      int iTextureSize = iWidth * iHeight;
      // Set offset and write width
      char* pcTempTexHdrPtr = pcTextureHeaderChunk + i*0x50 + 0x44;
      *((int*)pcTempTexHdrPtr) = iWidth;
      // Write height
      pcTempTexHdrPtr += 0x04;
      *((int*)pcTempTexHdrPtr) = iHeight;
      // Write data pointer
      pcTempTexHdrPtr += 0x04;
      *((int*)pcTempTexHdrPtr) = iCurrentPos + iOffset;
      // Write texture data
      for (int iPixel=0; iPixel<iTextureSize; iPixel++) {
         //memset(pcTextureDataChunk+(iCurrentPos++),(char)i,0x01);
         *(pcTextureDataChunk+(iCurrentPos++)) = (char)pTexture->GetPixel(iPixel);
      }            
      // Write palette
      const CImagePalette* pPalette = pTexture->GetPalette();
      for (int iEntry=0; iEntry<256; iEntry++) {
         memset(pcTextureDataChunk+iCurrentPos,iEntry,0x03);
         iCurrentPos += 3;
         /*unsigned long uColour;
         pPalette->GetEntry(iEntry,uColour);
         *(pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 16) & 0xFF;
         *(pcTextureDataChunk+(iCurrentPos++)) = (uColour >> 8) & 0xFF;
         *(pcTextureDataChunk+(iCurrentPos++)) = uColour & 0xFF;*/
      }
   }
   iOffset += iTextureDataChunkLength;

   // Bin resized textures
   for (i=0; i<pAvatar->NumTextures(); i++) {
      CImage* pTexture = vpSmallTextures.back();
      vpSmallTextures.pop_back();
      delete pTexture;
   }

   // Calculate and fill in file size information
   sHeader.liLength = iOffset;

   
   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Writing to file");
   }
   // Write chunks to stream in correct order
   osOutputStream.write((char*)&sHeader,sizeof(HlMDLHeader));
   osOutputStream.write(pcBoneChunk,iBoneChunkLength);
   osOutputStream.write(pcBoneControllerChunk,iBoneControllerChunkLength);
   osOutputStream.write(pcAttachmentChunk,iAttachmentChunkLength);
   osOutputStream.write(pcHitboxChunk,iHitboxChunkLength);
   osOutputStream << ifAnimStream.rdbuf();
   osOutputStream.write(pcSequenceGroupsChunk,iSequenceGroupsChunkLength);
   osOutputStream.write(pcBodyPartsHeaderChunk,iBodyPartsHeaderChunkLength);
   osOutputStream.write(pcModelHeaderChunk,iModelHeaderChunkLength);
   osOutputStream.write(pcModelDataChunk,iModelDataChunkLength);
   osOutputStream.write(pcMeshHeaderChunk,iMeshHeaderChunkLength);
   osOutputStream.write(pcMeshDataChunk,iMeshDataChunkLength);
   osOutputStream.write(pcTextureHeaderChunk,iTextureHeaderChunkLength);
   osOutputStream.write(pcTextureIndexChunk,iTextureIndexChunkLength);
   osOutputStream.write(pcTextureDataChunk,iTextureDataChunkLength);

   // Close files
   ifAnimStream.close();

   // Clean up memory
   if (m_pProgressDlg != NULL) {
      m_pProgressDlg->Step();
      m_pProgressDlg->SetText("Cleaning up");
   }
   if (pcTextureDataChunk != NULL) delete [] pcTextureDataChunk;
   if (pcTextureIndexChunk != NULL) delete [] pcTextureIndexChunk;
   if (pcTextureHeaderChunk != NULL) delete [] pcTextureHeaderChunk;
   if (pcMeshDataChunk != NULL) delete [] pcMeshDataChunk;
   if (pcMeshHeaderChunk != NULL) delete [] pcMeshHeaderChunk;
   if (pcModelDataChunk != NULL) delete [] pcModelDataChunk;
   if (pcModelHeaderChunk != NULL) delete [] pcModelHeaderChunk;
   if (pcBodyPartsHeaderChunk != NULL) delete [] pcBodyPartsHeaderChunk;
   if (pcSequenceGroupsChunk != NULL) delete [] pcSequenceGroupsChunk;
   if (pcHitboxChunk != NULL) delete [] pcHitboxChunk;
   if (pcAttachmentChunk != NULL) delete [] pcAttachmentChunk;
   if (pcBoneControllerChunk != NULL) delete [] pcBoneControllerChunk;
   if (pcBoneChunk != NULL) delete [] pcBoneChunk;

   // Restore old stream mode
   if (iOldMode != -1) osOutputStream.setmode(iOldMode);

   //pAvatar->ImportPose(oldPose);
   
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
