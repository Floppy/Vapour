//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileUnreal.cpp - 16/2/2000 - James Smith
//	Unreal export filter implementation
//
// $Id: AvatarFileUnreal.cpp,v 1.10 2000/08/30 14:08:50 waz Exp $
//

#include "stdafx.h"

#include "VAL.h"

#include "AvatarFileUnreal.h"
#include "AvatarFileProxy.h"

#include "Unreal.h"

//#include "ImageFileStore.h"
//extern CImageFileStore g_oImageFileStore;

#include <direct.h>
#include <errno.h>
#include <time.h>
#include <strstrea.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////
// CAvatarFileUnreal

CAvatarFileProxy<CAvatarFileUnreal> g_oAvatarProxyUnreal;
/////////////////////
// Member Functions

////////////////////
// Store Functions

CAvatarFileUnreal::CAvatarFileUnreal() {
   m_pszName = NULL;
   m_pszTgtDir = NULL;
   m_pszTexPackageName = NULL;
   m_pszTexBaseName = NULL;
   m_puFirstPixel = NULL;
   m_puImageHeight = NULL;
   m_uTotalHeight = 0;
   m_iSex = UNREAL_MALE;
   m_dYOffset = 0;
   m_dScale = 1;
   return;
} // CAvatarFileUnreal()

float CAvatarFileUnreal::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFileUnreal::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFileUnreal::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFileUnreal::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFileUnreal::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFileUnreal::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFileUnreal::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFileUnreal::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFileUnreal::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

FRESULT CAvatarFileUnreal::Save(const char* pszFilename, CAvatar* pAvatar) const {

   // Check parameters
   if (pszFilename == NULL) return F_NO_FILE_NAME;
   if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;

   // Return value
   FRESULT eResult = F_OK;

   // Setup the export progress system
   g_poVAL->SetProgressMax("UTSave", 7 + pAvatar->NumTextures());

   // Initialise
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Initialising");
   // Perform initialisation
   eResult = Init(pszFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   unsigned short uDirLength = strlen(m_pszTgtDir);
   unsigned short uNameLength = strlen(m_pszName);

   // Save texture INT file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving texture description");
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"System\\%s.int",m_pszTexPackageName);
   // Save
   eResult = SaveTextureINT(m_pszTempFilename);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save mesh INT file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving model description");
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"System\\%s.int",m_pszName);
   // Save
   eResult = SaveMeshINT(m_pszTempFilename);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }
  
   // Save texture UTX file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving texture package");
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"Textures\\%s.utx",m_pszTexPackageName);
   // Save
   eResult = SaveTextureUTX(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save mesh U file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving model package");
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"System\\%s.u",m_pszName);
   // Save
   eResult = SaveMeshU(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   /*//////////////////////////////////////////////////////////////////////

   // Save mesh description .3d file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving mesh description .3d file");
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"%s\\Models\\%s_d.3d",m_pszName,m_pszName);
   // Save
   eResult = SaveMeshD3D(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save mesh frame data .3d file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving mesh frame data .3d file");   
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"%s\\Models\\%s_a.3d",m_pszName,m_pszName);
   // Save
   eResult = SaveMeshA3D(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save selection mesh description .3d file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving selection mesh description .3d file");   
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"%s\\Models\\Select%s_d.3d",m_pszName,m_pszName);
   // Save
   eResult = SaveSelectionMeshD3D(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save selection mesh frame data .3d file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving selection mesh frame data .3d file");   
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"%s\\Models\\Select%s_a.3d",m_pszName,m_pszName);
   // Save
   eResult = SaveSelectionMeshA3D(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save mesh script file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving mesh script file");   
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"%s\\Classes\\%s.uc",m_pszName,m_pszName);
   // Save
   eResult = SaveMeshScript(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save selection mesh script file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving selection mesh script file");   
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"%s\\Classes\\Select%s.uc",m_pszName,m_pszName);
   // Save
   eResult = SaveSelectionMeshScript(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   // Save bot script file
   // Notify of progress
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving bot script file");   
   // Create filename
   sprintf(m_pszTempFilename+uDirLength,"%s\\Classes\\%sBot.uc",m_pszName,m_pszName);
   // Save
   eResult = SaveBotScript(m_pszTempFilename,pAvatar);
   // Check result
   if (eResult != F_OK) {
      Cleanup();
      return eResult;
   }

   //////////////////////////////////////////////////////////////////////*/

   // Tidy up allocated memory
   eResult = Cleanup();
   // Step progress
   g_poVAL->StepProgress("UTSave");
   // Done!
   return eResult;
} // Save(const char* pszFilename, CAvatar* pAvatar)

FRESULT CAvatarFileUnreal::Init(const char* pszFilename, CAvatar* pAvatar) const {

   // Declare a loop variable
   int i=0;

   // Seed random number generator
   srand(time(NULL)*clock());

   // Work out model name and target directory from passed filename
   char* pszInputName = strdup(pszFilename);
   char* pszTemp = pszInputName;
   // Find target directory
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   int uDirLength = (pszTemp - pszInputName);
   NEWBEGIN
   m_pszTgtDir = new char[uDirLength+1];
   NEWEND("CAvatarFileUnreal::Save - target directory string allocation")
   if (m_pszTgtDir == NULL) {
      free(pszInputName);
      return F_OUT_OF_MEMORY;
   }
   for (i=0; i<uDirLength; i++) {
      m_pszTgtDir[i] = pszInputName[i];
   }
   m_pszTgtDir[uDirLength] = 0;
   // Find model name
   char* pszEnd = strchr(pszTemp,'.');
   int uNameLength = (pszEnd != NULL) ? (pszEnd - pszTemp) : strlen(pszTemp);
   NEWBEGIN
   m_pszName = new char[uNameLength+1];
   NEWEND("CAvatarFileUnreal::Save - name string allocation")
   if (m_pszName == NULL) {
      free(pszInputName);
      return F_OUT_OF_MEMORY;
   }

   for (i=0; i<uNameLength; i++) {
      m_pszName[i] = pszTemp[i];
   }
   m_pszName[uNameLength] = 0;
   uNameLength = strlen(m_pszName);
   // Tidy up
   free(pszInputName);

   // Create base texture name
   NEWBEGIN
   m_pszTexBaseName = new char[5];
   NEWEND("CAvatarFileUnreal::Save - texture base name string allocation")
   for (i=0; i<4 && i<uNameLength; i++) {
      m_pszTexBaseName[i] = m_pszName[i];
   }
   for ( ; i<4; i++) {
      m_pszTexBaseName[i] = 'x';
   }
   // Terminate string!
   m_pszTexBaseName[4] = 0;

   // Create package name
   NEWBEGIN
   m_pszTexPackageName = new char[uNameLength + 6];
   NEWEND("CAvatarFileUnreal::Save - texture package name string allocation")
   if (m_pszTexPackageName == NULL) return F_OUT_OF_MEMORY;

   strcpy(m_pszTexPackageName,m_pszName);
   strcpy(m_pszTexPackageName+uNameLength,"Skins");

   strcpy(m_pszTempFilename,m_pszTgtDir);

   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Creating directories");

   // Create output system directory
   sprintf(m_pszTempFilename+uDirLength,"System\\",m_pszName);
   int iResult = _mkdir(m_pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create System directory!\n");
      return F_DIR_ERROR;
   }
   // Create output textures directory
   sprintf(m_pszTempFilename+uDirLength,"Textures\\",m_pszName);
   iResult = _mkdir(m_pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create Textures directory!\n");
      return F_DIR_ERROR;
   }
   // Create package directory
   sprintf(m_pszTempFilename+uDirLength,"%s\\",m_pszName);
   iResult = _mkdir(m_pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create Package directory!\n");
      return F_DIR_ERROR;
   }
   // Create Models directory
   sprintf(m_pszTempFilename+uDirLength,"%s\\Models\\",m_pszName);
   iResult = _mkdir(m_pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create Models directory!\n");
      return F_DIR_ERROR;
   }
   // Create Classes directory
   sprintf(m_pszTempFilename+uDirLength,"%s\\Classes\\",m_pszName);
   iResult = _mkdir(m_pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create Classes directory!\n");
      return F_DIR_ERROR;
   }

   // Allocate texture coordinate minima and maxima storage
   NEWBEGIN
   m_puImageHeight = new unsigned int[pAvatar->NumTextures()];
   NEWEND("CAvatarFileUnreal::Init - maxima storage allocation")
   if (m_puImageHeight == NULL) return F_OUT_OF_MEMORY;

   NEWBEGIN
   m_puFirstPixel = new unsigned int[pAvatar->NumTextures()];
   NEWEND("CAvatarFileUnreal::Init - minima storage allocation")
   if (m_puFirstPixel == NULL) return F_OUT_OF_MEMORY;

   // Calculate model scales and offsets
   // Reset Pose
   pAvatar->ResetPose();
   pAvatar->UpdateModel();
   // Setup sizes & offsets etc
   SPoint3D pntMin, pntMax;
   pAvatar->BoundingBox(unknown,pntMax,pntMin);
   m_dScale = 110 / (pntMax.m_dComponents[1] - pntMin.m_dComponents[1]);
   m_dYOffset = -((pntMax.m_dComponents[1] + pntMin.m_dComponents[1]) / 2 * m_dScale);
   
   // Done!
   return F_OK;
} //Init(const char* pszFilename, const CAvatar* pAvatar) const

FRESULT CAvatarFileUnreal::SaveMeshINT(const char* pszFilename) const {
   FRESULT eResult = F_OK;
   // Open output stream
   ofstream osOutputStream;
   osOutputStream.open(pszFilename,ios::binary|ios::out);
   // Write file if stream is OK.
	if (!osOutputStream.fail()) {
      // Write public declaration
      osOutputStream << "[Public]\n";
      // Write player declaration
      osOutputStream << "Object=(Name=";
      osOutputStream << m_pszName << "." << m_pszName;
      osOutputStream << ",Class=Class,MetaClass=Botpack.TournamentPlayer,Description=\"";
      osOutputStream << m_pszName << "\")\n";
      // Write bot declaration
      osOutputStream << "Object=(Name=";
      osOutputStream << m_pszName << "." << m_pszName;
      osOutputStream << "Bot,Class=Class,MetaClass=Botpack.Bot,Description=\"";
      osOutputStream << m_pszName << "\")\n";
      // Done - close stream
      osOutputStream.close();
   }
   else eResult = F_FILE_ERROR;
   // Done!
   return eResult;
} //SaveMeshINT(const char* pszFilename) const

FRESULT CAvatarFileUnreal::SaveTextureINT(const char* pszFilename) const {
   FRESULT eResult = F_OK;
   // Open output stream
   ofstream osOutputStream;
   osOutputStream.open(pszFilename,ios::binary|ios::out);
   // Write file if stream is OK.
	if (!osOutputStream.fail()) {
      // Write public declaration
      osOutputStream << "[public]\n";
      // Write face texture name
      osOutputStream << "Object=(Name=" << m_pszTexPackageName << '.' << m_pszTexBaseName << "1,Class=Texture,Description=\"" << m_pszName << "\")\n";
      // Write body texture name
      osOutputStream << "Object=(Name=" << m_pszTexPackageName << '.' << m_pszTexBaseName << "2,Class=Texture)\n";
      // Done - close stream
      osOutputStream.close();
   }
   else eResult = F_FILE_ERROR;
   // Done!
   return eResult;
} //SaveTextureINT(const char* pszFilename) const

FRESULT CAvatarFileUnreal::SaveTextureUTX(const char* pszFilename, const CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Open output stream
   ofstream osOutputStream;
   osOutputStream.open(pszFilename,ios::binary|ios::out);
   // Write file if stream is OK.   
   if (!osOutputStream.fail()) {

      CImagePalette pPalettes[2];

      int t,i; // loop counters
      const unsigned int uFaceTexIndex = pAvatar->TextureIndex(skullbase);
      int iNumTextures = pAvatar->NumTextures();
      const unsigned int uNameLength = strlen(m_pszName);

      // We create two textures - head and body.
      // Deal with head texture first:
      g_poVAL->StepProgress("UTSave");
      g_poVAL->SetProgressText("UTSave", "Converting head texture");
      // Copy texture out
      CImage imgFace(*(pAvatar->Texture(uFaceTexIndex)));
      // Scale
      imgFace.Scale(UNREAL_TEXTURE_WIDTH,UNREAL_TEXTURE_HEIGHT);
      // and convert to palette!
      CImage* imgNewFace;
      NEWBEGIN
      imgNewFace = new CImage(imgFace,IT_PALETTE);
      NEWEND("CAvatarFileUnreal::Save - body image conversion");
      if (imgNewFace != NULL) {
         pPalettes[1].Copy(imgNewFace->GetPalette());
         delete imgNewFace;
      }
      else {
         return F_OUT_OF_MEMORY;
      }

      // Now sort out body texture:
      g_poVAL->StepProgress("UTSave");
      g_poVAL->SetProgressText("UTSave", "Creating body texture");
      // Store torso tex index for future reference
      unsigned int uTorsoTexIndex = pAvatar->TextureIndex(vl5);
      // Resize textures
      CImage** pImages = NULL;
      NEWBEGIN
      pImages = new CImage*[iNumTextures];
      NEWEND("CAvatarFileUnreal::Save - image pointer allocation");
      if (pImages == NULL) {
         Cleanup();
         return F_OUT_OF_MEMORY;
      }
      for (t=0; t<iNumTextures; t++) {
         pImages[t] = NULL;
      }
      m_uTotalHeight = 0;
      const SBodyPart* pBodyParts = pAvatar->BodyParts();
      for (t=0; t<iNumTextures; t++) {
         // If this is not an exempt texture
         if (t != uFaceTexIndex) {
            // Variables
            int iWidth = 0;
            int iHeight = 0;
            CImage* pSmallerImage = NULL;
            // Create temporary image by copying current image
            NEWBEGIN
            pSmallerImage = new CImage(*(pAvatar->Texture(t)));
            NEWEND("CAvatarFileUnreal::Save - small image allocation");
            if (pSmallerImage == NULL) {
               // Dump data
               for (int i=0; i<iNumTextures; i++) {
                  if (pImages[i] != NULL) delete pImages[i];
               }
               delete [] pImages;
               return F_OUT_OF_MEMORY;
            }
            // Get size
            pSmallerImage->GetSize(iWidth,iHeight);
            // Modify width to be correct width
            iWidth = UNREAL_TEXTURE_WIDTH;
            // Modify height to be a maximum of a quarter of the width.
            // Unless it is the torso, where we want more detail
            if (t == uTorsoTexIndex) iHeight = iWidth;
            else if (iHeight > iWidth/4) iHeight = iWidth/4;
            // Scale image to new sie
            pSmallerImage->Scale(iWidth,iHeight);
            // Store image data
            m_puFirstPixel[t] = m_uTotalHeight;
            m_puImageHeight[t] = iHeight;
            m_uTotalHeight += iHeight;
            // Store image pointer
            pImages[t] = pSmallerImage;
         }
         // If it is, we don't bother with it.
         else {
            m_puFirstPixel[t] = m_uTotalHeight;
            m_puImageHeight[t] = 0;
            pImages[t] = NULL;
         }
      }
      // Create body image
      CImage imgBody(IT_RGB,UNREAL_TEXTURE_WIDTH,m_uTotalHeight);
      int iWidth = 0; 
      int iHeight = 0;
      imgBody.GetSize(iWidth,iHeight);
      if (iWidth==0 || iHeight==0) {
         Cleanup();
         for (int i=0; i<iNumTextures; i++) {
            if (pImages[i] != NULL) delete pImages[i];
         }
         delete [] pImages;
         return F_OUT_OF_MEMORY;
      }
      else {
         for (t=0; t<iNumTextures; t++) {
            if (pImages[t] != NULL) {
               imgBody.Paste(*(pImages[t]),0,m_puFirstPixel[t]);
               delete pImages[t];
            }
         }
      }
      delete [] pImages;
      imgBody.Scale(UNREAL_TEXTURE_WIDTH,UNREAL_TEXTURE_HEIGHT);
      // Convert
      g_poVAL->StepProgress("UTSave");
      g_poVAL->SetProgressText("UTSave", "Converting body texture");
      CImage* imgNewBody;
      NEWBEGIN
      imgNewBody = new CImage(imgBody,IT_PALETTE);
      NEWEND("CAvatarFileUnreal::Save - body image conversion");
      if (imgNewBody != NULL) {
         pPalettes[0].Copy(imgNewBody->GetPalette());
         delete imgNewBody;
      }
      else {
         return F_OUT_OF_MEMORY;
      }

      unsigned int uCurrentOffset = 0; // file position counter
           
      // Create file header
      SUnrealPackageHeader sHeader;
      // Write info
      sHeader.uTag = UNREAL_PACKAGE_TAG;
      sHeader.uFileVersion = UNREAL_PACKAGE_VERSION;
      // Set flags
      sHeader.uPackageFlags = UNREAL_PKG_ALLOWDOWNLOAD;
      // Name tables stuff
      sHeader.uNameCount = 25; // 21 basic names, plus two for each of the two textures
      // 0 exported symbols
      sHeader.uExportCount = 4;
      // 3 imported symbols
      sHeader.uImportCount = 3;
      // Create GUID
      // The GUID is our little starter bit, plus a couple of chars of tha name, plus 5 random nums.
      uCurrentOffset = 0;
      strcpy(sHeader.pcGUID, UNREAL_VAPOUR_GUID_TEX);
      uCurrentOffset += strlen(UNREAL_VAPOUR_GUID_TEX);
      // Add maximum 3 chars of name
      for (i=0; i<uNameLength && i<3; i++) {
         sHeader.pcGUID[uCurrentOffset++] = m_pszName[i];
      }
      // Fill up to seven characters with random data.
      for ( ; t<7; t++) {
         sHeader.pcGUID[uCurrentOffset++] = (char)rand();
      }      
      // Set num of GUIDs?
      sHeader.uGUIDCount = 1;
      // Repeated numbers?
      sHeader.uExportCountAgain = sHeader.uExportCount;
      sHeader.uNameCountAgain = sHeader.uNameCount;
      // Update file offset
      uCurrentOffset = sizeof(SUnrealPackageHeader);

      // Write name table
      sHeader.uNameOffset = uCurrentOffset;
      unsigned long uNameTableLength = 0x147 + uNameLength; // 0x147 for basic names and texture names, plus name length for part of package name entry
      char* pcNameTable;
      NEWBEGIN
      pcNameTable = new char[uNameTableLength];
      NEWEND("CAvatarFileUnreal::Save - name table allocation");
      if (pcNameTable != NULL) {
         unsigned long uNameTableOffset = 0;
         // Write standard names
         const int iNumNames = 25;
         const char* m_pszNames[iNumNames] = {
            "None","InternalTime","Color","MipZero","VBits","UClamp",
            "VClamp","USize","VSize","Palette","MaxColor","UBits",
            "bHighColorQuality","bHighTextureQuality","LODSet","Package","Core","Engine",
            "","","Texture","Class","","",""
         };
         const int piFlags[iNumNames] = {
            0x04070410,0x00070010,0x04070410,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x04070410,0x04070010,0x04070010,
            0x00000001,0x00000002,0x04070010,0x04070410,0x00000003,0x00000004,0x00000005
         };
         for (int i=0; i<iNumNames; i++) {
            unsigned long uFlags;
            switch (piFlags[i]) {
            case 1:
               // Write palette names
               {
                  const char* pszBasePalName = "Palette";
                  // Write length
                  pcNameTable[uNameTableOffset++] = strlen(pszBasePalName)+2;
                  // Write name
                  strcpy(pcNameTable+uNameTableOffset,pszBasePalName);
                  uNameTableOffset += strlen(pszBasePalName);
                  char cPalNum = '3';
                  pcNameTable[uNameTableOffset++] = cPalNum;
                  pcNameTable[uNameTableOffset++] = 0;
                  // Write flags
                  uFlags = UNREAL_RF_UNKNOWNFLAG | UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT;
                  *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
                  uNameTableOffset += 4;
               }
               break;
            case 2:
               // Write palette names
               {
                  const char* pszBasePalName = "Palette";
                  // Write length
                  pcNameTable[uNameTableOffset++] = strlen(pszBasePalName)+2;
                  // Write name
                  strcpy(pcNameTable+uNameTableOffset,pszBasePalName);
                  uNameTableOffset += strlen(pszBasePalName);
                  char cPalNum = '1';
                  pcNameTable[uNameTableOffset++] = cPalNum;
                  pcNameTable[uNameTableOffset++] = 0;
                  // Write flags
                  uFlags = UNREAL_RF_UNKNOWNFLAG | UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT;
                  *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
                  uNameTableOffset += 4;
               }
               break;
            case 3:
               // Write package name
               pcNameTable[uNameTableOffset++] = strlen(m_pszTexPackageName)+1;
               strcpy(pcNameTable+uNameTableOffset,m_pszTexPackageName);
               uNameTableOffset += strlen(m_pszTexPackageName)+1;
               uFlags = UNREAL_RF_UNKNOWNFLAG | UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT;
               *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
               uNameTableOffset += 4;
               break;
            case 4:
               // Write tex data names
               {
                  // Write length
                  pcNameTable[uNameTableOffset++] = 6; // 4 for name, 1 for ID, 1 for terminator
                  // Write and terminate name
                  int c=0;
                  for ( ; c<4 && c<uNameLength; c++) {
                     pcNameTable[uNameTableOffset++] = m_pszName[c];
                  }
                  for ( ; c<4; c++) {
                     pcNameTable[uNameTableOffset++] = 'x';
                  }
                  char cTexNum = '1';
                  pcNameTable[uNameTableOffset++] = cTexNum;
                  pcNameTable[uNameTableOffset++] = 0;
                  // Write flags
                  uFlags = UNREAL_RF_UNKNOWNFLAG | UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT;
                  *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
                  uNameTableOffset += 4;
               }
               break;
            case 5:
               // Write tex data names
               {
                  // Write length
                  pcNameTable[uNameTableOffset++] = 6; // 4 for name, 1 for ID, 1 for terminator
                  // Write and terminate name
                  int c=0;
                  for ( ; c<4 && c<uNameLength; c++) {
                     pcNameTable[uNameTableOffset++] = m_pszName[c];
                  }
                  for ( ; c<4; c++) {
                     pcNameTable[uNameTableOffset++] = 'x';
                  }
                  char cTexNum = '2';
                  pcNameTable[uNameTableOffset++] = cTexNum;
                  pcNameTable[uNameTableOffset++] = 0;
                  // Write flags
                  uFlags = UNREAL_RF_UNKNOWNFLAG | UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT;
                  *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
                  uNameTableOffset += 4;
               }
               break;
            default:
               pcNameTable[uNameTableOffset++] = strlen(m_pszNames[i])+1;
               strcpy(pcNameTable+uNameTableOffset,m_pszNames[i]);
               uNameTableOffset += strlen(m_pszNames[i])+1;
               *((long*)(pcNameTable+uNameTableOffset)) = piFlags[i];
               uNameTableOffset += 4;
               break;
            }
         }
      }
      else {
         osOutputStream.close();
         return F_OUT_OF_MEMORY;
      }
      // Update file offset
      uCurrentOffset += uNameTableLength;

      // Allocate palette data space
      const unsigned int uPaletteDataLength = 0x806; // 3 byte header & 2 palettes
      char* pcPaletteData;
      NEWBEGIN
      pcPaletteData = new char[uPaletteDataLength];
      NEWEND("CAvatarFileUnreal::Save - palette data allocation")
      if (pcPaletteData != NULL) {
         // Write palette data
         int iPaletteDataOffset = 0;
         for (t=2; t--!=0; ) {
            // Write palette header
            pcPaletteData[iPaletteDataOffset++] = 0x00;
            pcPaletteData[iPaletteDataOffset++] = 0x40;
            pcPaletteData[iPaletteDataOffset++] = 0x04;
            // Write palette
            for (int iEntry=0; iEntry<256; iEntry++) {
               // Get colour value
               unsigned int uColour;
               pPalettes[t].GetEntry(iEntry,uColour);
               unsigned char cRed, cGreen, cBlue;
               cBlue = (char)uColour;
               uColour = uColour >> 8;
               cGreen = (char)uColour;
               uColour = uColour >> 8;
               cRed = (char)uColour;
               // Write colour values
               pcPaletteData[iPaletteDataOffset++] = cRed;
               pcPaletteData[iPaletteDataOffset++] = cGreen;
               pcPaletteData[iPaletteDataOffset++] = cBlue;
               // Write alpha value
               pcPaletteData[iPaletteDataOffset++] = (char)0xFF;
            }
         }
      }
      else {
         delete [] pcNameTable;
         osOutputStream.close();
         return F_OUT_OF_MEMORY;
      }
      // Store palette data offsets
      int pPaletteOffset[2];
      pPaletteOffset[0] = uCurrentOffset;
      pPaletteOffset[1] = uCurrentOffset + 0x403;
      // Update file offset
      uCurrentOffset += uPaletteDataLength;

      g_poVAL->StepProgress("UTSave");
      g_poVAL->SetProgressText("UTSave", "Writing texture data");
   
      // Write texture data
      unsigned long uTextureDataLength = 0x2AC56; // Sufficient space for 2 256x256 images
      char* pcTextureData;
      NEWBEGIN
      pcTextureData = new char[uTextureDataLength];
      NEWEND("CAvatarFileUnreal::Save - texture data chunk allocation");
      // Texture data offsets
      int pTextureOffset[2];
      int pTextureSize[2];
      if (pcTextureData != NULL) {
         unsigned long uTextureDataOffset = 0;
         // Write texture data
         for (t=0; t<2; t++) {
            pTextureOffset[t] = uCurrentOffset + uTextureDataOffset;
            CImage* pTexture = t ?  &imgFace : &imgBody;
            int iWidth, iHeight;
            pTexture->GetSize(iWidth,iHeight);
            // Measure no. of bits in width - i.e. num of times to mipmap
            unsigned char cCount = 0;
            while (iWidth > 0) {
               iWidth = iWidth >> 1;
               cCount++;
            }
            // Reset width
            pTexture->GetSize(iWidth,iHeight);
            // Write texture header information
            pcTextureData[uTextureDataOffset++] = (char)0x0C;
            pcTextureData[uTextureDataOffset++] = (char)0xD3;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x0D;
            pcTextureData[uTextureDataOffset++] = (char)0xD3;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x0E;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x02;
            pcTextureData[uTextureDataOffset++] = (char)0x09;
            pcTextureData[uTextureDataOffset++] = (char)0x05;
            // Don't know what the rest does :)
            // This is all texture flags and so on.
            // Haven't had time to decode it properly.
            if (t==0) {
               pcTextureData[uTextureDataOffset++] = (char)0x02;
            }
            else {
               pcTextureData[uTextureDataOffset++] = (char)0x01;
            }
            pcTextureData[uTextureDataOffset++] = (char)0x0B;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x08;
            pcTextureData[uTextureDataOffset++] = (char)0x04;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x08;
            pcTextureData[uTextureDataOffset++] = (char)0x07;
            pcTextureData[uTextureDataOffset++] = (char)0x22;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x08;
            pcTextureData[uTextureDataOffset++] = (char)0x22;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x05;
            pcTextureData[uTextureDataOffset++] = (char)0x22;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x06;
            pcTextureData[uTextureDataOffset++] = (char)0x22;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x03;
            pcTextureData[uTextureDataOffset++] = (char)0x2A;
            pcTextureData[uTextureDataOffset++] = (char)0x02;
            if (t==0) {
               pcTextureData[uTextureDataOffset++] = (char)0x6F;
               pcTextureData[uTextureDataOffset++] = (char)0x73;
               pcTextureData[uTextureDataOffset++] = (char)0x70;
            }
            else {
               pcTextureData[uTextureDataOffset++] = (char)0x7B;
               pcTextureData[uTextureDataOffset++] = (char)0x6C;
               pcTextureData[uTextureDataOffset++] = (char)0x5F;
            }
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x0A;
            pcTextureData[uTextureDataOffset++] = (char)0x2A;
            pcTextureData[uTextureDataOffset++] = (char)0x02;
            if (t==0) {
               pcTextureData[uTextureDataOffset++] = (char)0xD1;
               pcTextureData[uTextureDataOffset++] = (char)0xD8;
               pcTextureData[uTextureDataOffset++] = (char)0xD8;
            }
            else {
               pcTextureData[uTextureDataOffset++] = (char)0xE0;
               pcTextureData[uTextureDataOffset++] = (char)0xC8;
               pcTextureData[uTextureDataOffset++] = (char)0xB7;
            }
            pcTextureData[uTextureDataOffset++] = (char)0xFF;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0x22;
            if (t==0) {
               pcTextureData[uTextureDataOffset++] = (char)0xF2;
               pcTextureData[uTextureDataOffset++] = (char)0xD9;
               pcTextureData[uTextureDataOffset++] = (char)0x84;
               pcTextureData[uTextureDataOffset++] = (char)0x5C;
            }
            else {
               pcTextureData[uTextureDataOffset++] = (char)0xD1;
               pcTextureData[uTextureDataOffset++] = (char)0x43;
               pcTextureData[uTextureDataOffset++] = (char)0x3D;
               pcTextureData[uTextureDataOffset++] = (char)0xED;
            }
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            pcTextureData[uTextureDataOffset++] = (char)0xA2;
            pcTextureData[uTextureDataOffset++] = (char)0x01;
            if (t==0) {
               pcTextureData[uTextureDataOffset++] = (char)0xCC;
            }
            else {
               pcTextureData[uTextureDataOffset++] = (char)0xCE;
            }
            pcTextureData[uTextureDataOffset++] = (char)0x0A;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            pcTextureData[uTextureDataOffset++] = (char)0x00;
            // Number of images to output
            char cNumImages = 1;
            pcTextureData[uTextureDataOffset++] = (char)cNumImages;
            // Until we reach the minimum texture size (0), output image halving size each time
            for (i=0; i<cNumImages; i++) {
               // Reduce image by the correct amount and convert it.
               CImage imgMipmap(*pTexture);
               imgMipmap.Scale(iWidth,iHeight);
               imgMipmap.ForceToPalette(pPalettes[t]);
               // Write mip image header...
               // Store address of foot offset
               unsigned long uFooterOffset = uTextureDataOffset;
               uTextureDataOffset += 4;
               // Write compact index of data length
               unsigned long uSize = iWidth * iHeight;
               CUnrealCompactIndex oCIndex((int)uSize);
               for (int b=0; b<oCIndex.NumBytes(); b++) {
                  pcTextureData[uTextureDataOffset++] = oCIndex.CompactIndex()[b];
               }
               // Write data
               for (unsigned long uPixel=0; uPixel<uSize; uPixel++) {
                  unsigned char cPixelValue = imgMipmap.GetPixel(uPixel);
                  pcTextureData[uTextureDataOffset++] = cPixelValue;
               }
               // Write foot
               // Write offset of footer into header
               *((long*)(pcTextureData+uFooterOffset)) = uCurrentOffset + uTextureDataOffset;
               // Store width & height
               *((long*)(pcTextureData+uTextureDataOffset)) = iWidth;
               uTextureDataOffset += 4;
               *((long*)(pcTextureData+uTextureDataOffset)) = iHeight;
               uTextureDataOffset += 4;
               // Store bit size (width and height) of next image
               cCount--;
               pcTextureData[uTextureDataOffset++] = cCount;
               pcTextureData[uTextureDataOffset++] = cCount;
               // Halve sizes for next image
               iWidth = iWidth >> 1;
               iHeight = iHeight >> 1;
            }
            pTextureSize[t] = (t==0) ? uTextureDataOffset : uTextureDataOffset-pTextureSize[0];
         }
         uTextureDataLength = uTextureDataOffset;
      }
      else {
         delete [] pcNameTable;
         delete [] pcPaletteData;
         osOutputStream.close();
         return F_OUT_OF_MEMORY;
      }
      // Update file offset
      uCurrentOffset += uTextureDataLength;
      
      // Write import table
      // This imports various classes into this package
      sHeader.uImportOffset = uCurrentOffset;
      SUnrealImportTableEntry* pImportTable;
      NEWBEGIN
      pImportTable = new SUnrealImportTableEntry[sHeader.uImportCount];
      NEWEND("CAvatarFileUnreal::Save - import table allocation");
      if (pImportTable != NULL) {

         pImportTable[0].cClassPackage = 0x10;
         pImportTable[0].cClassName    = 0x0F;
         pImportTable[0].uPackageIndex = 0;
         pImportTable[0].cObjectName   = 0x11;

         pImportTable[1].cClassPackage = 0x10;
         pImportTable[1].cClassName    = 0x15;
         pImportTable[1].uPackageIndex = -1;
         pImportTable[1].cObjectName   = 0x14;

         pImportTable[2].cClassPackage = 0x10;
         pImportTable[2].cClassName    = 0x15;
         pImportTable[2].uPackageIndex = -1;
         pImportTable[2].cObjectName   = 0x09;

      }
      else {
         delete [] pcPaletteData;
         delete [] pcTextureData;
         osOutputStream.close();
         return F_OUT_OF_MEMORY;
      }
      uCurrentOffset += sizeof(SUnrealImportTableEntry)*sHeader.uImportCount;

      // Write export table
      sHeader.uExportOffset = uCurrentOffset;
      unsigned int uExportTableLength = 21 * 4; // max length of export table entry is 21 and we export 4 symbols
      char* pcExportTable;
      NEWBEGIN
      pcExportTable = new char[uExportTableLength];
      NEWEND("CAvatarFileUnreal::Save - export table allocation")
      if (pcExportTable != NULL) {
         int uExportTableOffset = 0;
         // Create compact index variable
         CUnrealCompactIndex oCIndex;
         // Write export declaration for palette 1
         pcExportTable[uExportTableOffset++] = (char)UNREAL_CLASS_PALETTE; // Class ID
         memset(pcExportTable+uExportTableOffset,0,5);
         uExportTableOffset+=5;
         pcExportTable[uExportTableOffset++] = 0x13; // Name number
         long uFlags = UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT | UNREAL_RF_PUBLIC;
         *((long*)(pcExportTable+uExportTableOffset)) = uFlags; // Object flags
         uExportTableOffset+=4;
         oCIndex.Set((int)0x403); // CI of data size (0x403 for palette)
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         oCIndex.Set((int)pPaletteOffset[0]); // CI of data address
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         // Write export declaration for palette 3
         pcExportTable[uExportTableOffset++] = (char)UNREAL_CLASS_PALETTE; // Class ID
         memset(pcExportTable+uExportTableOffset,0,5);
         uExportTableOffset+=5;
         pcExportTable[uExportTableOffset++] = 0x12; // Name number
         *((long*)(pcExportTable+uExportTableOffset)) = uFlags; // Object flags
         uExportTableOffset+=4;
         oCIndex.Set((int)0x403); // CI of data size (0x403 for palette)
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         oCIndex.Set((int)pPaletteOffset[1]); // CI of data address
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         // Write export declaration for texture 1
         pcExportTable[uExportTableOffset++] = (char)UNREAL_CLASS_TEXTURE; // Class ID
         memset(pcExportTable+uExportTableOffset,0,5);
         uExportTableOffset+=5;
         pcExportTable[uExportTableOffset++] = 0x18; // Name number
         uFlags = UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT | UNREAL_RF_PUBLIC | UNREAL_RF_STANDALONE;
         *((long*)(pcExportTable+uExportTableOffset)) = uFlags; // Object flags
         uExportTableOffset+=4;
         oCIndex.Set((int)pTextureSize[0]); // CI of data size
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         oCIndex.Set((int)pTextureOffset[0]); // CI of data address
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         // Write export declaration for texture 2
         pcExportTable[uExportTableOffset++] = (char)UNREAL_CLASS_TEXTURE; // Class ID
         memset(pcExportTable+uExportTableOffset,0,5);
         uExportTableOffset+=5;
         pcExportTable[uExportTableOffset++] = 0x17; // Name number
         *((long*)(pcExportTable+uExportTableOffset)) = uFlags; // Object flags
         uExportTableOffset+=4;
         oCIndex.Set((int)pTextureSize[0]); // CI of data size
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         oCIndex.Set((int)pTextureOffset[1]); // CI of data address
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[i];
         }
         // Update length to take CIs into account
         uExportTableLength = uExportTableOffset;
      }
      else {
         delete [] pcPaletteData;
         delete [] pcTextureData;
         delete [] pImportTable;
         osOutputStream.close();
         return F_OUT_OF_MEMORY;
      }

      // Write chunks to stream
      // Write header
      osOutputStream.write((char*)&sHeader,sizeof(SUnrealPackageHeader));
      // Write name list
      osOutputStream.write(pcNameTable,uNameTableLength);
      // Write palette data
      osOutputStream.write(pcPaletteData,uPaletteDataLength);
      // Write texture data
      osOutputStream.write(pcTextureData,uTextureDataLength);
      // Write import table
      osOutputStream.write((char*)pImportTable,sizeof(SUnrealImportTableEntry)*sHeader.uImportCount);
      // Write export table
      osOutputStream.write(pcExportTable,uExportTableLength);

      // Done - close stream
      osOutputStream.close();

      // Delete data chunks
      delete [] pcExportTable;
      delete [] pImportTable;
      delete [] pcTextureData;
      delete [] pcPaletteData;
      delete [] pcNameTable;
   }
   else eResult = F_FILE_ERROR;
   // Done!
   return eResult;
} //SaveTextureUTX(const char* pszFilename, const CAvatar* pAvatar) const

FRESULT CAvatarFileUnreal::SaveMeshU(const char* pszFilename, CAvatar* pAvatar) const {

   FRESULT eResult = F_OK;

   const unsigned int uNumFaces = pAvatar->NumFaces();
   const unsigned int uNumVertices = pAvatar->NumVertices();

   // Open output stream
   ofstream osOutputStream;
   osOutputStream.open(pszFilename,ios::binary|ios::out);
   // Write file if stream is OK.   
   if (!osOutputStream.fail()) {

      int i; // Generic loop counter
      const unsigned int uNumFrames = 700;
      const unsigned int uNumSeqs = 69;
      const unsigned int uNameLength = strlen(m_pszName);
      unsigned int uCurrentOffset = 0;
      const SBodyPart* pBodyParts = pAvatar->BodyParts();

      unsigned int* pTempAddressPtr = NULL;

      // Store old avatar pose
      CAvatarPose poOldPose = pAvatar->ExportPose();
      pAvatar->ResetPose();
      pAvatar->UpdateModel();

      // File offsets and size for export table
      unsigned long uExportOffset[10] = {0,0,0,0,0,0,0,0,0,0};

      // Create file header
      SUnrealPackageHeader sHeader;
      // Write info
      sHeader.uTag = UNREAL_PACKAGE_TAG;
      sHeader.uFileVersion = UNREAL_PACKAGE_VERSION;
      // Set flags
      sHeader.uPackageFlags = UNREAL_PKG_ALLOWDOWNLOAD;
      // Name tables stuff
      sHeader.uNameCount = 0x6C;
      // 9 exported symbols
      sHeader.uExportCount = 0x09; // 0x09 exported names
      // 12 imported symbols
      sHeader.uImportCount = 0x0C; // 0x0C imported names
      // Create GUID
      // The GUID is our little starter bit, plus a couple of chars of tha name, plus 5 random nums.
      uCurrentOffset = 0;
      strcpy(sHeader.pcGUID, UNREAL_VAPOUR_GUID_MSH);
      uCurrentOffset += strlen(UNREAL_VAPOUR_GUID_MSH);
      // Add maximum 3 chars of name
      for (i=0; i<uNameLength && i<3; i++) {
         sHeader.pcGUID[uCurrentOffset++] = m_pszName[i];
      }
      // Fill up to seven characters with random data.
      for ( ; i<7; i++) {
         sHeader.pcGUID[uCurrentOffset++] = (char)rand();
      }      
      // Set num of GUIDs?
      sHeader.uGUIDCount = 1;
      // Repeated numbers?
      sHeader.uExportCountAgain = sHeader.uExportCount;
      sHeader.uNameCountAgain = sHeader.uNameCount;
      // Update file offset
      uCurrentOffset = sizeof(SUnrealPackageHeader);

      // Need to store a couple of things for later...
      unsigned int uMeshName, uSelectName, uBotName;
      // Create name table
      sHeader.uNameOffset = uCurrentOffset;
      unsigned long uNameTableLength = ( m_iSex==UNREAL_FEMALE ? 0x599 : 0x593 )+ 3*uNameLength; // 0x593 for basic names and constant parts of model names, plus 2 * name length for names
      char* pcNameTable;
      NEWBEGIN
      pcNameTable = new char[uNameTableLength];
      NEWEND("CAvatarFileUnreal::Save - name table allocation");
      if (pcNameTable != NULL) {
         unsigned long uNameTableOffset = 0;
         // Write standard names
         const int iNumNames = 0x6C;
         const char* pszNames[iNumNames] = {
            "None","Waiting","PlayFootStep","TakeHit","LandThump","Jumping","MovingFire","Gesture",
            "*name*","Botpack","Core","Engine","All","System","DefaultPackage","Mesh","TeamSkin2",
            "TeamSkin1","Ducking","MenuName","Landing","DefaultSkinName","CarcassType","SelectionMesh",
            "SpecialMesh","Dead1","Fighter","User","VoiceType","Walk","GutHit","Dead2","LeftHit","RightHit",
            "Breath2","Dead4","HeadHit","RunSm","StillSmFr","StillFrRp","WalkSm","ForceMeshToExist",
            "DuckWlkS","JumpLgFr","RunLg","Dead7","JumpSmFr","WalkLg","WalkLgFr","WalkSmFr","DuckWlkL",
            "RunLgFr","RunSmFr","TreadSm","Dead3","TreadLg","AimUpLg","AimDnLg","AimDnSm","CockGun",
            "SwimLg","SwimSm","AimUpSm","LandLgFr","LandSmFr","TurnLg","DeathEnd3","StillLgFr","TurnSm",
            "DeathEnd2","CockGunL","Breath2L","Breath1L","DeathEnd","Breath1","Taunt1","Look","Victory1",
            "WaveL","LookL","Wave","Thrust","Flip","Chat1","*select*","Dead9","StrafeL","StrafeR",
            "BackRun","Dead9B","DodgeR","DodgeL","Dead8","DodgeB","*bot*","Dead11","DodgeF","Chat2",
            "TextBuffer","Object","Class",m_iSex==UNREAL_FEMALE?"TFemale2Carcass":"TMale2Carcass",
            m_iSex==UNREAL_FEMALE?"TournamentFemale":"TournamentMale","SelectionDude",
            m_iSex==UNREAL_FEMALE?"FemaleBotPlus":"MaleBotPlus","Function","ScriptText","Package",
         };
         const int piFlags[iNumNames] = {
            0x04070410,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00000001,0x00070010,0x04070010,0x04070010,0x04070010,0x04070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x04070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00000002,0x00070010,0x00070010,0x00070010,
            0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00070010,0x00000003,0x00070010,0x00070010,0x00070010,
            0x04070010,0x04070010,0x04070410,0x00070010,
            0x00070010,0x00070010,
            0x00070010,0x04070410,0x00070010,0x04070410,
         };
         unsigned long uFlags = UNREAL_RF_UNKNOWNFLAG | UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER |UNREAL_RF_LOADFOREDIT;
         for (int i=0; i<iNumNames; i++) {
            switch (piFlags[i]) {
            case 1:
               // Write "Name" entry
               pcNameTable[uNameTableOffset++] = uNameLength+1;
               strcpy(pcNameTable+uNameTableOffset,m_pszName);
               uNameTableOffset += uNameLength+1;
               *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
               uNameTableOffset += 4;
               uMeshName = i;
               break;
            case 2:
               // Write "SelectName" entry
               pcNameTable[uNameTableOffset++] = uNameLength+7;
               strcpy(pcNameTable+uNameTableOffset,"Select");
               uNameTableOffset += 6;
               strcpy(pcNameTable+uNameTableOffset,m_pszName);
               uNameTableOffset += uNameLength+1;
               *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
               uNameTableOffset += 4;
               uSelectName = i;
               break;
            case 3:
               // Write "NameBot" entry
               pcNameTable[uNameTableOffset++] = uNameLength+4;
               strcpy(pcNameTable+uNameTableOffset,m_pszName);
               uNameTableOffset += uNameLength;
               strcpy(pcNameTable+uNameTableOffset,"Bot");
               uNameTableOffset += 4;
               *((long*)(pcNameTable+uNameTableOffset)) = uFlags;
               uNameTableOffset += 4;
               uBotName = i;
               break;
            default:
               // Write other entry
               pcNameTable[uNameTableOffset++] = strlen(pszNames[i])+1;
               strcpy(pcNameTable+uNameTableOffset,pszNames[i]);
               uNameTableOffset += strlen(pszNames[i])+1;
               *((long*)(pcNameTable+uNameTableOffset)) = piFlags[i];
               uNameTableOffset += 4;
               break;
            }
         }
      }
      else {
         return F_OUT_OF_MEMORY;
      }
      // Update file offset
      uCurrentOffset += uNameTableLength;

      // Update export offset
      uExportOffset[0] = uCurrentOffset;
      // Write Mesh Properties
      unsigned int uMeshPropertiesLength = (m_iSex==UNREAL_FEMALE ? 0xC8 : 0xC4) + 5*uNameLength;
      char* pcMeshProperties = NULL;
      NEWBEGIN
      pcMeshProperties = new char[uMeshPropertiesLength];
      NEWEND("CAvatarFileUnreal::Save - Standard mesh properties data chunk allocation")
      if (pcMeshProperties != NULL) {
         unsigned int uPropertiesOffset = 0;
         char pszPropertyName[32];
         // Write ID
         pcMeshProperties[uPropertiesOffset++] = (char)0x88;
         // Write standard data
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         // Write specific data
         pcMeshProperties[uPropertiesOffset++] = (char)0x06;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         // Write compact index of name table entry
         pcMeshProperties[uPropertiesOffset++] = (char)0x08;
         // Write standard data
         memset(pcMeshProperties+uPropertiesOffset,(char)-1,8);
         uPropertiesOffset+=8;
         memset(pcMeshProperties+uPropertiesOffset,(char)0,4);
         uPropertiesOffset+=4;
         // Write specific data
         pcMeshProperties[uPropertiesOffset++] = (char)0x02;
         pcMeshProperties[uPropertiesOffset++] = (char)0x0C;
         pcMeshProperties[uPropertiesOffset++] = (char)0x04;
         pcMeshProperties[uPropertiesOffset++] = (char)0xF8;
         // Write standard data
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         // Write specific data
         pcMeshProperties[uPropertiesOffset++] = (char)0x08;
         // Write standard data
         memset(pcMeshProperties+uPropertiesOffset,(char)0,2);
         uPropertiesOffset+=2;
         memset(pcMeshProperties+uPropertiesOffset,(char)-1,10);
         uPropertiesOffset+=10;
         memset(pcMeshProperties+uPropertiesOffset,(char)0,4);
         uPropertiesOffset+=4;
         // Write specific data
         pcMeshProperties[uPropertiesOffset++] = (char)0x36;
         // Write standard data
         memset(pcMeshProperties+uPropertiesOffset,(char)0,19);
         uPropertiesOffset+=19;
         pcMeshProperties[uPropertiesOffset++] = (char)0x02;
         // Write specific data
         pcMeshProperties[uPropertiesOffset++] = (char)0x01;
         // Write standard data
         pcMeshProperties[uPropertiesOffset++] = (char)0x01;
         memset(pcMeshProperties+uPropertiesOffset,(char)0,7);
         uPropertiesOffset+=7;
         // Write ID again
         pcMeshProperties[uPropertiesOffset++] = (char)0x88;
         // Write standard data
         pcMeshProperties[uPropertiesOffset++] = (char)0x01;
         memset(pcMeshProperties+uPropertiesOffset,(char)0,3);
         uPropertiesOffset+=3;
         // Write specific data
         pcMeshProperties[uPropertiesOffset++] = (char)0x54;
         pcMeshProperties[uPropertiesOffset++] = (char)0xC9;
         pcMeshProperties[uPropertiesOffset++] = (char)0x16;
         pcMeshProperties[uPropertiesOffset++] = (char)0xB8;
         // Write standard data
         pcMeshProperties[uPropertiesOffset++] = (char)0x04;
         pcMeshProperties[uPropertiesOffset++] = (char)0x08;
         pcMeshProperties[uPropertiesOffset++] = (char)0x09;
         pcMeshProperties[uPropertiesOffset++] = (char)0x0B;
         pcMeshProperties[uPropertiesOffset++] = (char)0x0A;
         pcMeshProperties[uPropertiesOffset++] = (char)0x85;
         // Write property data
         pcMeshProperties[uPropertiesOffset++] = (char)0x1B;
         pcMeshProperties[uPropertiesOffset++] = (char)0x11;
         pcMeshProperties[uPropertiesOffset++] = (char)0x22;
         pcMeshProperties[uPropertiesOffset++] = (char)0x01;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         pcMeshProperties[uPropertiesOffset++] = (char)0x10;
         pcMeshProperties[uPropertiesOffset++] = (char)0x22;
         pcMeshProperties[uPropertiesOffset++] = (char)0x02;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
         // Write skin name
         sprintf(pszPropertyName,"%sSkins.%s",m_pszName,m_pszTexBaseName);
         pcMeshProperties[uPropertiesOffset++] = (char)0x15;
         pcMeshProperties[uPropertiesOffset++] = (char)0x5D;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcMeshProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write texture package name
         sprintf(pszPropertyName,"%sSkins.",m_pszName);
         pcMeshProperties[uPropertiesOffset++] = (char)0x0E;
         pcMeshProperties[uPropertiesOffset++] = (char)0x5D;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcMeshProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write selection mesh name
         sprintf(pszPropertyName,"%s.Select%s",m_pszName,m_pszName);
         pcMeshProperties[uPropertiesOffset++] = (char)0x16;
         pcMeshProperties[uPropertiesOffset++] = (char)0x05;
         pcMeshProperties[uPropertiesOffset++] = (char)0x8A;
         pcMeshProperties[uPropertiesOffset++] = (char)0x17;
         pcMeshProperties[uPropertiesOffset++] = (char)0x5D;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcMeshProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write special mesh name
         sprintf(pszPropertyName,(m_iSex==UNREAL_FEMALE ? "Botpack.TrophyFemale2" : "Botpack.TrophyMale2"),m_pszName);
         pcMeshProperties[uPropertiesOffset++] = (char)0x18;
         pcMeshProperties[uPropertiesOffset++] = (char)0x5D;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcMeshProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write mesh name
         sprintf(pszPropertyName,"%s",m_pszName);
         pcMeshProperties[uPropertiesOffset++] = (char)0x13;
         pcMeshProperties[uPropertiesOffset++] = (char)0x5D;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcMeshProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write voice name
         sprintf(pszPropertyName,(m_iSex==UNREAL_FEMALE ? "Botpack.VoiceFemaleTwo" : "Botpack.VoiceMaleTwo"),m_pszName);
         pcMeshProperties[uPropertiesOffset++] = (char)0x1C;
         pcMeshProperties[uPropertiesOffset++] = (char)0x5D;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcMeshProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcMeshProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Finish
         pcMeshProperties[uPropertiesOffset++] = (char)0x0F;
         pcMeshProperties[uPropertiesOffset++] = (char)0x05;
         pcMeshProperties[uPropertiesOffset++] = (char)0x02;
         pcMeshProperties[uPropertiesOffset++] = (char)0x00;
      }
      else {
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      // Update file offset
      uCurrentOffset += uMeshPropertiesLength;

      // Update export offset
      uExportOffset[1] = uCurrentOffset;
      // Write standard mesh header
      unsigned int uMeshHeaderLength = 0x50;
      char* pcMeshHeader = NULL;
      NEWBEGIN
      pcMeshHeader = new char[uMeshHeaderLength];
      NEWEND("CAvatarFileUnreal::Save - Mesh top section allocation")
      if (pcMeshHeader != NULL) {
         // Write header
         unsigned int uHeaderOffset = 0;
         // Write frame 0 description part 1
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x40;
         pcMeshHeader[uHeaderOffset++] = (char)0x01;
         pcMeshHeader[uHeaderOffset++] = (char)0xC4;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x80;
         pcMeshHeader[uHeaderOffset++] = (char)0xFC;
         pcMeshHeader[uHeaderOffset++] = (char)0xC3;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x7F;
         pcMeshHeader[uHeaderOffset++] = (char)0xC3;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x40;
         pcMeshHeader[uHeaderOffset++] = (char)0x29;
         pcMeshHeader[uHeaderOffset++] = (char)0x44;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x17;
         pcMeshHeader[uHeaderOffset++] = (char)0x44;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x80;
         pcMeshHeader[uHeaderOffset++] = (char)0xE6;
         pcMeshHeader[uHeaderOffset++] = (char)0x43;
         pcMeshHeader[uHeaderOffset++] = (char)0x01;
         // Write frame 0 description part 2
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0xA0;
         pcMeshHeader[uHeaderOffset++] = (char)0x42;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x46;
         pcMeshHeader[uHeaderOffset++] = (char)0x42;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0x00;
         pcMeshHeader[uHeaderOffset++] = (char)0xCE;
         pcMeshHeader[uHeaderOffset++] = (char)0x42;
         pcMeshHeader[uHeaderOffset++] = (char)0xAB;
         pcMeshHeader[uHeaderOffset++] = (char)0x4C;
         pcMeshHeader[uHeaderOffset++] = (char)0x39;
         pcMeshHeader[uHeaderOffset++] = (char)0x44;
         // Store address of end of frame chunk address
         *((unsigned int*)(pcMeshHeader+uHeaderOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcMeshHeader+uHeaderOffset);
         uHeaderOffset+=4;
         // Write CI of amount of data
         CUnrealCompactIndex oCIndex((int)((uNumVertices+3)*uNumFrames));
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcMeshHeader[uHeaderOffset++] = oCIndex.CompactIndex()[i];
         }
         uMeshHeaderLength = uHeaderOffset;
      }
      else {
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      // Update file offset
      uCurrentOffset += uMeshHeaderLength;

      // Actual frame data is just too huge to store in-core,
      // unless to want to allocate nearly 4 meg of memory.
      // Therefore, we write it to a temp file in the right 
      // format and read it back in later.
      unsigned int uFrameDataLength = 0;
      ofstream osTempStream;
      sprintf(m_pszTempFilename+strlen(m_pszTgtDir),"tempframedata.bin");
      osTempStream.open(m_pszTempFilename,ios::out|ios::binary);
	   if (!osTempStream.fail()) {
         // Information variables
         const unsigned int uNumVertices = pAvatar->NumVertices();
         const SPoint3D* pVertices = pAvatar->Vertices();
         // Store weapon triangle vertices
         SPoint3D pWeaponVertices[3]; // top/back, top/front, bottom/front in that order
         // In default pose, we use bounding box information for right hand to calculate these.
         pAvatar->ResetPose();
         pAvatar->UpdateModel();
         SPoint3D pntMax, pntMin;
         pAvatar->BoundingBox(r_wrist,pntMax,pntMin);
         // Calculate rough Y value of palm (i.e. for grip);
         //double dYMiddle = (pntMin.m_dComponents[1] + pntMax.m_dComponents[1]*3) / 4;
         // Set vertex at top/back of hand
         pWeaponVertices[0].m_dComponents[0] = pntMax.m_dComponents[0];
         pWeaponVertices[0].m_dComponents[1] = pntMax.m_dComponents[1];
         pWeaponVertices[0].m_dComponents[2] = pntMax.m_dComponents[2];
         // Set vertex at top/front of hand
         pWeaponVertices[1].m_dComponents[0] = pntMax.m_dComponents[0];
         pWeaponVertices[1].m_dComponents[1] = pntMin.m_dComponents[1];
         pWeaponVertices[1].m_dComponents[2] = pntMax.m_dComponents[2];
         // Set vertex at bottom/front of hand
         pWeaponVertices[2].m_dComponents[0] = pntMax.m_dComponents[0];
         pWeaponVertices[2].m_dComponents[1] = pntMin.m_dComponents[1];
         pWeaponVertices[2].m_dComponents[2] = pntMin.m_dComponents[2];
         // Open data wedgie for frame files
         char pszWJEName[STR_SIZE] = "";
         strcpy(pszWJEName, g_poVAL->GetAppDir());
         strcat(pszWJEName, "utdata.wje");
         fstream fsDataWJE(pszWJEName, ios::in|ios::binary|ios::nocreate);
         if (fsDataWJE.fail()) {
            delete [] pcMeshHeader;
            delete [] pcMeshProperties;
            delete [] pcNameTable;
            osTempStream.close();
            TRACE("Could not open wedgie file!\n");
            return F_FILE_ERROR;
         }
         CWedgie oWedgie;
         oWedgie.Open(&fsDataWJE);
         // Storage for filename
         char pszPoseFilename[STR_SIZE];
         // Write frames
         for (i=0; i<uNumFrames; i++) {
            // This needs to be done for each frame that is output.
            // Create filename to load: utp000.vpo to utp699.vpo
            sprintf(pszPoseFilename,"poses\\utp%03d.vpo",i);
            // Load file from wedgie7
            CAvatarPose oPose;
            if (oPose.Load(pszPoseFilename, oWedgie) == F_OK) {
               // Apply to avatar
               pAvatar->ImportPose(oPose);
					// Update Pose
					pAvatar->UpdateModel();
					// Remove body parts as necessary for particular frames
					// Head shot
					if ((i >= 567) && (i <= 582))
                  pAvatar->Chop(vc7);
            }
            else {
               // Fail export if we can't load a pose
               delete [] pcMeshHeader;
               delete [] pcMeshProperties;
               delete [] pcNameTable;
			      oWedgie.Close();
               fsDataWJE.close();
               osTempStream.close();
               TRACE("Could not open pose %d!\n",i);
               return F_FILE_ERROR;
            }

            // Write vertices
            for (int v=0; v<uNumVertices; v++) {
               // Pack vertex into a long int 
               long lVertex = PackVertex(pVertices[v],m_dScale,m_dYOffset);
               // Write long to stream
               osTempStream.write((char*)&lVertex,4);
               uFrameDataLength += 4;
            }

            // Get transform for weapon triangle vertices
            CHomTransform htTransform(pBodyParts[r_wrist].m_htLowerTransform);
            // Write weapon triangle vertices
            for (int wv=0; wv<3; wv++) {
               // Update vertex pos
               SPoint3D pntNewPosition = htTransform * pWeaponVertices[wv];
               // Pack vertex into a long int 
               long lVertex = PackVertex(pntNewPosition,m_dScale,m_dYOffset);
               // Write long to stream
               osTempStream.write((char*)&lVertex,4);
               uFrameDataLength += 4;
            }
         }
			oWedgie.Close();
         fsDataWJE.close();
         osTempStream.close();
      }
      else {
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_FILE_ERROR;
      }
      // Update file offset
      uCurrentOffset += uFrameDataLength;
      // Store current offset in temp pointer
      *pTempAddressPtr = uCurrentOffset;

      // Write the rest of the mesh data
      unsigned int uMeshDataLength = 
         9  + sizeof(SUnrealTriangle)*(uNumFaces+1) + // mesh header + data
         5  + (19*uNumSeqs) + 110 + // sequence data
         9  + (8 * (uNumVertices+3)) + //vertex info
         50 + (12 * (uNumFaces+1)) + //face info
         8  + (25 * uNumFrames) + // frame data part 1
         5  + (16 * uNumFrames) + // frame data part 2
         69; // footer
      char* pcMeshData = NULL;
      NEWBEGIN
      pcMeshData = new char[uMeshDataLength];
      NEWEND("CAvatarFileUnreal::Save - mesh data allocation");
      if (pcMeshData != NULL) {

         unsigned long uMeshOffset = 0; // temp offset counter
         int f,v; // loop counters

         ////////////////////////////////////////////////////////////
         // Write topology data header
         ////////////////////////////////////////////////////////////

         // Store address of end of topology chunk
         *((unsigned int*)(pcMeshData+uMeshOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcMeshData+uMeshOffset);
         uMeshOffset+=4;
         // CI of number of faces
         CUnrealCompactIndex oCIndex((int)(uNumFaces+1));
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcMeshData[uMeshOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write topology data
         ////////////////////////////////////////////////////////////
         // Store texture info
         const int iMaxU = UNREAL_TEXTURE_WIDTH-1;
         const int iMaxV = UNREAL_TEXTURE_HEIGHT-1;
         // Some other stuff we need
         const STriFace* pFaces = pAvatar->Faces();
         const unsigned int uFaceTexIndex = pAvatar->TextureIndex(skullbase);
         // Cast pointer
         SUnrealTriangle* pFaceData = (SUnrealTriangle*)(pcMeshData + uMeshOffset);
         // Write faces
         for (f=0; f<uNumFaces; f++) {
            // Vertex indices
            pFaceData[f].uVertex0 = pFaces[f].m_iVertices[2];
            pFaceData[f].uVertex1 = pFaces[f].m_iVertices[1];
            pFaceData[f].uVertex2 = pFaces[f].m_iVertices[0];
            // Write Flags
            pFaceData[f].uFlags = UNREAL_U_TRI_NORMAL1SIDE;
            // Texture coordinates
            int iTextureNumber = pFaces[f].m_iTextureNumber;
            for (int tc=3; tc--!=0; ) {
               // Get tex coords
               double dU = pFaces[f].m_sTexCoords[tc].dU;
               double dV = pFaces[f].m_sTexCoords[tc].dV;
               // Clamp to 0..1
               if (dU > 1) dU = 1;
               else if (dU < 0) dU = 0;
               if (dV > 0.9) dV = 0.9;
               else if (dV < 0.1) dV = 0.1;
               // Recalculate texture V coordinates if this is not the face texture
               if (iTextureNumber!=uFaceTexIndex) {
                  // Multiply by height of this particular image
                  dV *= m_puImageHeight[iTextureNumber];
                  // Add where this image starts
                  dV += m_puFirstPixel[iTextureNumber];
                  // Divide by the total height of the image
                  dV /= m_uTotalHeight;
               }
               // Scale and store
               char cU = (dU*iMaxU)+0.5; // Add 0.5 to ensure correct rounding
               char cV = (dV*iMaxV)+0.5; // Add 0.5 to ensure correct rounding
               switch (tc) {
               case 2:
                  pFaceData[f].uU0 = cU;
                  pFaceData[f].uV0 = cV;
                  break;
               case 1:
                  pFaceData[f].uU1 = cU;
                  pFaceData[f].uV1 = cV;
                  break;
               case 0:
                  pFaceData[f].uU2 = cU;
                  pFaceData[f].uV2 = cV;
                  break;
               }
            }
            // Write Texture number
            pFaceData[f].uTexture = iTextureNumber==uFaceTexIndex ? 0 : 1;
         }
         // Write data for weapon triangle
         // Write vertex indices: three vertices after main vertex list
         unsigned short uVertexNumber = pAvatar->NumVertices();
         pFaceData[f].uVertex0 = uVertexNumber++;
         pFaceData[f].uVertex1 = uVertexNumber++;
         pFaceData[f].uVertex2 = uVertexNumber;
         // Write Flags
         pFaceData[f].uFlags = UNREAL_U_TRI_WEAPONTRI;
         // Texture coordinates
         pFaceData[f].uU0 = 0;
         pFaceData[f].uV0 = 0;
         pFaceData[f].uU1 = 0;
         pFaceData[f].uV1 = 0;
         pFaceData[f].uU2 = 0;
         pFaceData[f].uV2 = 0;
         // Texture number
         pFaceData[f].uTexture = 0;
         // Update offset
         uMeshOffset += (uNumFaces+1) * sizeof(SUnrealTriangle);
         // Store current offset in temp pointer
         *pTempAddressPtr = uCurrentOffset + uMeshOffset;

         ////////////////////////////////////////////////////////////
         // Write sequence info header
         ////////////////////////////////////////////////////////////

         // Write CI of number of sequences
         oCIndex.Set((int)uNumSeqs);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcMeshData[uMeshOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write sequence info
         ////////////////////////////////////////////////////////////

         // Sequence information
         const int piNameRef[uNumSeqs] = {
            0x0C, 0x39, 0x3A, 0x38, 0x3E, 0x24, 0x20, 0x1E, 0x21, 0x27, 0x43, 0x26, 
            0x37, 0x35, 0x48, 0x47, 0x46, 0x4C, 0x4F, 0x4A, 0x22, 0x3B, 0x53, 0x61,
            0x4D, 0x4E, 0x50, 0x51, 0x4B, 0x5D, 0x60, 0x5A, 0x5B, 0x52, 0x2B, 0x2E,
            0x3F, 0x40, 0x32, 0x2A, 0x3C, 0x3D, 0x2F, 0x30, 0x28, 0x31, 0x1D, 0x2C,
            0x33, 0x25, 0x34, 0x58, 0x56, 0x57, 0x41, 0x44, 0x1A, 0x19, 0x1F, 0x36,
            0x23, 0x2D, 0x5C, 0x55, 0x59, 0x5F, 0x49, 0x45, 0x42
         };
         const char piGroup[uNumSeqs] = {
            0, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 20, 20, 18, 18, 0, 0, 0, 6, 0,
            6, 0, 0, 6, 0, 6, 6, 6, 6, 0, 0, 0, 3, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0
         };
         const int piStartFrame[uNumSeqs] = {
            0, 1, 2, 3, 4, 90, 95, 0, 136, 177, 187, 197, 235, 250, 373, 380, 400,
            96, 408, 25, 32, 52, 463, 476, 265, 448, 343, 486, 5, 501, 502, 503, 504,
            505, 91, 92, 93, 94, 60, 75, 205, 220, 283, 298, 313, 328, 358, 137, 147,
            157, 167, 670, 680, 690, 298, 328, 187, 525, 538, 554, 567, 583, 604, 622,
            642, 652, 537, 553, 566
         };
         const int piNumFrames[uNumSeqs] = {
            700, 1, 1, 1, 1, 1, 1, 1, 1, 10, 10, 8, 15, 15, 7, 20, 8, 40, 40, 7, 20, 8,
            13, 10, 18, 15, 15, 15, 20, 1, 1, 1, 1, 20, 1, 1, 1, 1, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 10, 10, 10, 10, 10, 10, 10, 2, 2, 1, 13, 16, 13, 16, 21, 18,
            20, 10, 18, 1, 1, 1
         };
         const float pfRate[uNumSeqs] = {
            30, 30, 30, 30, 30, 30, 30, 30, 30, 15, 15, 15, 15, 15, 6, 7, 6, 15, 15, 6, 7, 6, 6,
            6, 11, 15, 15, 20, 15, 30, 30, 30, 30, 30, 30, 30, 30, 30, 15, 15, 15, 15, 18, 18, 18,
            18, 18, 17, 17, 17, 17, 17, 17, 17, 15, 15, 30, 10, 10, 10, 10, 12, 10, 30, 15,
            10, 30, 30, 30
         };
         const char piNumNotifies[uNumSeqs] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
            2, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0,
         };
         const float pfNotifyData[22] = {
            0.25F, 0.75F, 0.25F, 0.75F, 0.25F, 0.75F, 0.25F, 0.75F, 0.25F, 0.75F, 0.25F,
            0.75F, 0.25F, 0.75F, 0.7F, 0.9F, 0.45F, 0.6F, 0.7F, 0.7F, 0.8F, 0.57F
         };
         const char piNotifyType[22] = {
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            4, 4, 4, 4, 4, 4, 4, 4
         };
         int iNotifyCounter = 0;
         // Write information to memory
         for (int s=0; s<uNumSeqs; s++) {
            // Write CI of sequence name
            oCIndex.Set(piNameRef[s]);
            for (i=0; i<oCIndex.NumBytes(); i++) {
               pcMeshData[uMeshOffset++] = oCIndex.CompactIndex()[i];
            }
            // Write group number
            pcMeshData[uMeshOffset++] = piGroup[s];
            // Write start frame for sequence
            *((long*)(pcMeshData+uMeshOffset)) = piStartFrame[s];
            uMeshOffset+=4;
            // Write number of frames
            *((long*)(pcMeshData+uMeshOffset)) = piNumFrames[s];
            uMeshOffset+=4;
            // Write number of notifies
            pcMeshData[uMeshOffset++] = piNumNotifies[s];
            if (piNumNotifies[s] != 0) {
               // Write notify data
               for (int n=0; n<piNumNotifies[s]; n++) {
                  *((float*)(pcMeshData+uMeshOffset)) = pfNotifyData[iNotifyCounter];
                  uMeshOffset+=4;
                  pcMeshData[uMeshOffset++] = piNotifyType[iNotifyCounter++];
               }
            }
            // Write rate
            *((float*)(pcMeshData+uMeshOffset)) = pfRate[s];
            uMeshOffset+=4;
         }

         ////////////////////////////////////////////////////////////
         // Write vertex info header
         ////////////////////////////////////////////////////////////

         // Store address of end of vertex info chunk
         *((unsigned int*)(pcMeshData+uMeshOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcMeshData+uMeshOffset);
         uMeshOffset+=4;
         // Write CI of number of vertices
         oCIndex.Set((int)(uNumVertices+3));
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcMeshData[uMeshOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write vertex info
         ////////////////////////////////////////////////////////////

         // Cast pointer
         unsigned long* puTempPtr = (unsigned long*)(pcMeshData + uMeshOffset);
         unsigned int uTempOffset = 0;
         unsigned int uTotalValence = 0;
         for (v=0; v<uNumVertices; v++) {
            // Write vertex data
            puTempPtr[uTempOffset++] = pAvatar->FacesPerVertex(v)->Length(); // Valence
            puTempPtr[uTempOffset++] = uTotalValence; // Total
            uTotalValence += pAvatar->FacesPerVertex(v)->Length();
         }
         // Write weapon triangle data
         // Vertex 0
         puTempPtr[uTempOffset++] = 1; // Valence
         puTempPtr[uTempOffset++] = uTotalValence; // Total
         uTotalValence++;
         // Vertex 1
         puTempPtr[uTempOffset++] = 1; // Valence
         puTempPtr[uTempOffset++] = uTotalValence; // Total
         uTotalValence++;
         // Vertex 2
         puTempPtr[uTempOffset++] = 1; // Valence
         puTempPtr[uTempOffset++] = uTotalValence; // Total
         // Update offset
         uMeshOffset += 8 * (uNumVertices+3);
         // Store current offset in temp pointer
         *pTempAddressPtr = uCurrentOffset + uMeshOffset;

         ////////////////////////////////////////////////////////////
         // Write face info header
         ////////////////////////////////////////////////////////////

         // Write face info header
         // Write frame 0 description part 1
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x40;
         pcMeshData[uMeshOffset++] = (char)0x01;
         pcMeshData[uMeshOffset++] = (char)0xC4;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x80;
         pcMeshData[uMeshOffset++] = (char)0xFC;
         pcMeshData[uMeshOffset++] = (char)0xC3;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x7F;
         pcMeshData[uMeshOffset++] = (char)0xC3;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x40;
         pcMeshData[uMeshOffset++] = (char)0x29;
         pcMeshData[uMeshOffset++] = (char)0x44;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x17;
         pcMeshData[uMeshOffset++] = (char)0x44;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x80;
         pcMeshData[uMeshOffset++] = (char)0xE6;
         pcMeshData[uMeshOffset++] = (char)0x43;
         pcMeshData[uMeshOffset++] = (char)0x01;
         // Write frame 0 description part 2
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0xA0;
         pcMeshData[uMeshOffset++] = (char)0x42;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x46;
         pcMeshData[uMeshOffset++] = (char)0x42;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0xCE;
         pcMeshData[uMeshOffset++] = (char)0x42;
         pcMeshData[uMeshOffset++] = (char)0xAB;
         pcMeshData[uMeshOffset++] = (char)0x4C;
         pcMeshData[uMeshOffset++] = (char)0x39;
         pcMeshData[uMeshOffset++] = (char)0x44;

         // Store address of end of face info chunk
         *((unsigned int*)(pcMeshData+uMeshOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcMeshData+uMeshOffset);
         uMeshOffset+=4;
         // Write CI of data length
         oCIndex.Set((int)(uNumFaces+1)*3);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcMeshData[uMeshOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write face info
         ////////////////////////////////////////////////////////////

         puTempPtr = (unsigned long*)(pcMeshData + uMeshOffset);
         uTempOffset = 0;
         for (f=0; f<uNumFaces; f++) {
            // Work out adjacent faces
            int iAdjacentFaces[3];
            for (int a=0; a<3; a++) {
               iAdjacentFaces[a] = -1;
               int b = (a+1) % 3;
               const CDList<int>* pFaceList = pAvatar->FacesPerVertex(pFaces[f].m_iVertices[a]);
               for (int i=0; (i<pFaceList->Length()) && (iAdjacentFaces[a]<0); i++) {
                  int iFace = (*pFaceList)[i];
                  if (iFace != f) { 
                     if ((pFaces[iFace].m_iVertices[0] == pFaces[f].m_iVertices[b]) ||
                         (pFaces[iFace].m_iVertices[1] == pFaces[f].m_iVertices[b]) ||
                         (pFaces[iFace].m_iVertices[2] == pFaces[f].m_iVertices[b])) 
                         iAdjacentFaces[a] = iFace;
                  }
               }
               // Make sure we're been successful
               ASSERT(iAdjacentFaces[a] != -1);
            }
            // Write adjacent face numbers
            puTempPtr[uTempOffset++] = iAdjacentFaces[2];
            puTempPtr[uTempOffset++] = iAdjacentFaces[0];
            puTempPtr[uTempOffset++] = iAdjacentFaces[1];
            // Make sure all three are different
            ASSERT((iAdjacentFaces[1] != iAdjacentFaces[2]) && (iAdjacentFaces[2] != iAdjacentFaces[0]) && (iAdjacentFaces[0] != iAdjacentFaces[1]));
         }
         // Write weapon triangle data
         puTempPtr[uTempOffset++] = f;
         puTempPtr[uTempOffset++] = f;
         puTempPtr[uTempOffset++] = f;
         // Update offset
         uMeshOffset += 12 * (uNumFaces+1);
         // Store current offset in temp pointer
         *pTempAddressPtr = uCurrentOffset + uMeshOffset;

         ////////////////////////////////////////////////////////////
         // Write frame data part 1 header
         ////////////////////////////////////////////////////////////

         // Write header data - don't know what this means
         pcMeshData[uMeshOffset++] = (char)0x02;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         // Write CI of number of frames
         oCIndex.Set((int)uNumFrames);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcMeshData[uMeshOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write frame data part 1
         ////////////////////////////////////////////////////////////
   
         // Write frame data part 1 - 25 bytes per frame
         for (f=0; f<uNumFrames; f++) {
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x0C;
            pcMeshData[uMeshOffset++] = (char)0xC2;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x5B;
            pcMeshData[uMeshOffset++] = (char)0xC3;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x5B;
            pcMeshData[uMeshOffset++] = (char)0xC3;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x80;
            pcMeshData[uMeshOffset++] = (char)0x9C;
            pcMeshData[uMeshOffset++] = (char)0x43;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x52;
            pcMeshData[uMeshOffset++] = (char)0x43;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x3C;
            pcMeshData[uMeshOffset++] = (char)0x43;
            pcMeshData[uMeshOffset++] = (char)0x01;
         }

         ////////////////////////////////////////////////////////////
         // Write frame data part 2 header
         ////////////////////////////////////////////////////////////

         // Write CI of number of frames
         oCIndex.Set((int)uNumFrames);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcMeshData[uMeshOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write frame data part 2
         ////////////////////////////////////////////////////////////

         // Write frame data part 2 - 16 bytes per frame
         for (f=0; f<uNumFrames; f++) {
	         pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x8E;
            pcMeshData[uMeshOffset++] = (char)0x42;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x00;
            pcMeshData[uMeshOffset++] = (char)0x3F;
            pcMeshData[uMeshOffset++] = (char)0x15;
            pcMeshData[uMeshOffset++] = (char)0x1F;
            pcMeshData[uMeshOffset++] = (char)0x74;
            pcMeshData[uMeshOffset++] = (char)0x43;
         }

         ////////////////////////////////////////////////////////////
         // Write footer
         ////////////////////////////////////////////////////////////

         // Number of vertices
         *((long*)(pcMeshData+uMeshOffset)) = uNumVertices+3;
         uMeshOffset+=4;
         // Number of sequences
         *((long*)(pcMeshData+uMeshOffset)) = uNumFrames;
         uMeshOffset+=4;
         // Standard data
         memset(pcMeshData+uMeshOffset,0,8);
         uMeshOffset+=8;
         pcMeshData[uMeshOffset++] = (char)0xCD;
         pcMeshData[uMeshOffset++] = (char)0xCC;
         pcMeshData[uMeshOffset++] = (char)0xCC;
         pcMeshData[uMeshOffset++] = (char)0x3D;
         pcMeshData[uMeshOffset++] = (char)0xCD;
         pcMeshData[uMeshOffset++] = (char)0xCC;
         pcMeshData[uMeshOffset++] = (char)0xCC;
         pcMeshData[uMeshOffset++] = (char)0x3D;
         pcMeshData[uMeshOffset++] = (char)0xCD;
         pcMeshData[uMeshOffset++] = (char)0xCC;
         pcMeshData[uMeshOffset++] = (char)0x4C;
         pcMeshData[uMeshOffset++] = (char)0x3E;
         memset(pcMeshData+uMeshOffset,0,32);
         uMeshOffset+=32;
         pcMeshData[uMeshOffset++] = (char)0x02;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x80;
         pcMeshData[uMeshOffset++] = (char)0x3F;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x00;
         pcMeshData[uMeshOffset++] = (char)0x80;
         pcMeshData[uMeshOffset++] = (char)0x3F;

         // Done - update size
         uMeshDataLength = uMeshOffset;
      }
      uCurrentOffset += uMeshDataLength;

      // Update export offset
      uExportOffset[2] = uCurrentOffset;
      // Write Bot Script
      const unsigned int uBotScriptLength = 0x70 + (uNameLength * 3);
      char* pcBotScript = NULL;
      NEWBEGIN
      pcBotScript = new char[uBotScriptLength];
      NEWEND("CAvatarFileUnreal::Save - bot script allocation");
      if (pcBotScript != NULL) {
         // Create output string stream
         ostrstream strScript(pcBotScript,uBotScriptLength);
         // Zero first nine values
         strScript << (char)0 << (char)0 << (char)0;
         strScript << (char)0 << (char)0 << (char)0;
         strScript << (char)0 << (char)0 << (char)0;
         // Write script length
         CUnrealCompactIndex oCIndex((int)(0x65+(uNameLength*3)));
         // This should only ever write two bytes
         ASSERT(oCIndex.NumBytes()==2);
         for (int b=0; b<oCIndex.NumBytes(); b++) {
            strScript << oCIndex.CompactIndex()[b];
         }
         // Write script itself.         
         strScript << "// " << m_pszName << "Bot.\x0D\x0A";
         strScript << "class " << m_pszName << "Bot extends MaleBotPlus;\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "function ForceMeshToExist()\x0D\x0A";
         strScript << "{\x0D\x0A";
	      strScript << "   Spawn(class'" << m_pszName << "');\x0D\x0A";
         strScript << "}\x0D\x0A";
         strScript << "\x0D\x0A";
         // Terminate
         strScript << (char)0;
      }
      else {
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      uCurrentOffset += uBotScriptLength;

      // Update export offset
      uExportOffset[3] = uCurrentOffset;
      // Write Bot Properties
      unsigned int uBotPropertiesLength = 0x94 + 5*uNameLength;
      char* pcBotProperties = NULL;
      NEWBEGIN
      pcBotProperties = new char[uBotPropertiesLength];
      NEWEND("CAvatarFileUnreal::Save - Bot properties data chunk allocation")
      if (pcBotProperties != NULL) {
         unsigned int uPropertiesOffset = 0;
         char pszPropertyName[32];
         // Write ID
         pcBotProperties[uPropertiesOffset++] = (char)0x89;
         // Write standard data
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         // Write specific data
         pcBotProperties[uPropertiesOffset++] = (char)0x03;
         pcBotProperties[uPropertiesOffset++] = (char)0x08;
         // Write compact index of name table entry
         pcBotProperties[uPropertiesOffset++] = (char)0x5E;
         pcBotProperties[uPropertiesOffset++] = (char)0x01;
         // Write standard data
         memset(pcBotProperties+uPropertiesOffset,(char)-1,8);
         uPropertiesOffset+=8;
         memset(pcBotProperties+uPropertiesOffset,(char)0,4);
         uPropertiesOffset+=4;
         // Write specific data
         pcBotProperties[uPropertiesOffset++] = (char)0x42;
         pcBotProperties[uPropertiesOffset++] = (char)0x9C;
         pcBotProperties[uPropertiesOffset++] = (char)0x04;
         pcBotProperties[uPropertiesOffset++] = (char)0xF8;
         // Write standard data
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         // Write specific data
         pcBotProperties[uPropertiesOffset++] = (char)0x0D;
         // Write standard data
         memset(pcBotProperties+uPropertiesOffset,(char)0,2);
         uPropertiesOffset+=2;
         memset(pcBotProperties+uPropertiesOffset,(char)-1,10);
         uPropertiesOffset+=10;
         memset(pcBotProperties+uPropertiesOffset,(char)0,4);
         uPropertiesOffset+=4;
         // Write specific data
         pcBotProperties[uPropertiesOffset++] = (char)0x36;
         // Write standard data
         memset(pcBotProperties+uPropertiesOffset,(char)0,19);
         uPropertiesOffset+=19;
         pcBotProperties[uPropertiesOffset++] = (char)0x02;
         // Write specific data
         pcBotProperties[uPropertiesOffset++] = (char)0x04;
         // Write standard data
         pcBotProperties[uPropertiesOffset++] = (char)0x01;
         memset(pcBotProperties+uPropertiesOffset,(char)0,7);
         uPropertiesOffset+=7;
         // Write ID again
         pcBotProperties[uPropertiesOffset++] = (char)0x89;
         // Write standard data
         pcBotProperties[uPropertiesOffset++] = (char)0x01;
         memset(pcBotProperties+uPropertiesOffset,(char)0,3);
         uPropertiesOffset+=3;
         // Write specific data
         pcBotProperties[uPropertiesOffset++] = (char)0x14;
         pcBotProperties[uPropertiesOffset++] = (char)0xF3;
         pcBotProperties[uPropertiesOffset++] = (char)0x11;
         pcBotProperties[uPropertiesOffset++] = (char)0x26;
         // Write standard data
         pcBotProperties[uPropertiesOffset++] = (char)0x04;
         pcBotProperties[uPropertiesOffset++] = (char)0x08;
         pcBotProperties[uPropertiesOffset++] = (char)0x09;
         pcBotProperties[uPropertiesOffset++] = (char)0x0B;
         pcBotProperties[uPropertiesOffset++] = (char)0x0A;
         pcBotProperties[uPropertiesOffset++] = (char)0x85;
         // Write property data
         pcBotProperties[uPropertiesOffset++] = (char)0x0D;
         pcBotProperties[uPropertiesOffset++] = (char)0x16;
         pcBotProperties[uPropertiesOffset++] = (char)0x05;
         pcBotProperties[uPropertiesOffset++] = (char)0x8A;
         pcBotProperties[uPropertiesOffset++] = (char)0x11;
         pcBotProperties[uPropertiesOffset++] = (char)0x22;
         pcBotProperties[uPropertiesOffset++] = (char)0x01;
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         pcBotProperties[uPropertiesOffset++] = (char)0x10;
         pcBotProperties[uPropertiesOffset++] = (char)0x22;
         pcBotProperties[uPropertiesOffset++] = (char)0x02;
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
         // Write skin name
         sprintf(pszPropertyName,"%sSkins.%s",m_pszName,m_pszTexBaseName);
         pcBotProperties[uPropertiesOffset++] = (char)0x15;
         pcBotProperties[uPropertiesOffset++] = (char)0x5D;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcBotProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write texture package name
         sprintf(pszPropertyName,"%sSkins.",m_pszName);
         pcBotProperties[uPropertiesOffset++] = (char)0x0E;
         pcBotProperties[uPropertiesOffset++] = (char)0x5D;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcBotProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write selection mesh name
         sprintf(pszPropertyName,"%s.Select%s",m_pszName,m_pszName);
         pcBotProperties[uPropertiesOffset++] = (char)0x17;
         pcBotProperties[uPropertiesOffset++] = (char)0x5D;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcBotProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Write mesh name
         sprintf(pszPropertyName,"%s",m_pszName);
         pcBotProperties[uPropertiesOffset++] = (char)0x13;
         pcBotProperties[uPropertiesOffset++] = (char)0x5D;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 2;
         pcBotProperties[uPropertiesOffset++] = strlen(pszPropertyName) + 1;
         strcpy(pcBotProperties+uPropertiesOffset,pszPropertyName);
         uPropertiesOffset += strlen(pszPropertyName) + 1;
         // Finish
         pcBotProperties[uPropertiesOffset++] = (char)0x0F;
         pcBotProperties[uPropertiesOffset++] = (char)0x05;
         pcBotProperties[uPropertiesOffset++] = (char)0x02;
         pcBotProperties[uPropertiesOffset++] = (char)0x00;
      }
      else {
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      // Update file offset
      uCurrentOffset += uBotPropertiesLength;

      // Update export offset
      uExportOffset[4] = uCurrentOffset;
      // Write Selection Mesh Script
      const unsigned int uSelectScriptLength = 0x152 + (uNameLength * 8);
      char* pcSelectScript = NULL;
      NEWBEGIN
      pcSelectScript = new char[uSelectScriptLength];
      NEWEND("CAvatarFileUnreal::Save - selection mesh script allocation");
      if (pcSelectScript != NULL) {
         // Create output string stream
         ostrstream strScript(pcSelectScript,uSelectScriptLength);
         // Zero first nine values
         strScript << (char)0 << (char)0 << (char)0;
         strScript << (char)0 << (char)0 << (char)0;
         strScript << (char)0 << (char)0 << (char)0;
         // Write script length
         CUnrealCompactIndex oCIndex((int)(0x147+(uNameLength*8)));
         // This should only ever write two bytes
         ASSERT(oCIndex.NumBytes()==2);
         for (int b=0; b<oCIndex.NumBytes(); b++) {
            strScript << oCIndex.CompactIndex()[b];
         }
         // Write script itself.         
         strScript << "// Select" << m_pszName << ".\x0D\x0A";
         strScript << "class Select" << m_pszName << " extends SelectionDude;\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH IMPORT MESH=Select" << m_pszName << " ANIVFILE=MODELS\\Select" << m_pszName << "_a.3d DATAFILE=MODELS\\Select" << m_pszName << "_d.3d X=0 Y=0 Z=0 MLOD=0\x0D\x0A";
         strScript << "#exec MESH ORIGIN MESH=Select" << m_pszName << " X=0 Y=0 Z=0\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=Select" << m_pszName << " SEQ=All STARTFRAME=0 NUMFRAMES=1\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESHMAP SCALE MESHMAP=Select" << m_pszName << " X=0.1 Y=0.1 Z=0.2\x0D\x0A";
         strScript << "\x0D\x0A";
         // Terminate
         strScript << (char)0;
      }
      else {
         delete [] pcBotProperties;
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      uCurrentOffset += uSelectScriptLength;

      // Update export offset
      uExportOffset[5] = uCurrentOffset;
      // Write Mesh Script
      const unsigned int uMeshScriptLength = (m_iSex==UNREAL_FEMALE ? 0x1DA8 : 0x1DA6) + (uNameLength * 98);
      char* pcMeshScript = NULL;
      NEWBEGIN
      pcMeshScript = new char[uMeshScriptLength];
      NEWEND("CAvatarFileUnreal::Save - mesh script allocation");
      if (pcSelectScript != NULL) {
         // Create output string stream
         ostrstream strScript(pcMeshScript,uMeshScriptLength);
         // Zero first nine values
         strScript << (char)0 << (char)0 << (char)0;
         strScript << (char)0 << (char)0 << (char)0;
         strScript << (char)0 << (char)0 << (char)0;
         // Write script length
         CUnrealCompactIndex oCIndex((int)((m_iSex==UNREAL_FEMALE?0x1D9E:0x1D9C)+(uNameLength*98)));
         // This should only ever write two bytes
         ASSERT(oCIndex.NumBytes()==2);
         for (int b=0; b<oCIndex.NumBytes(); b++) {
            strScript << oCIndex.CompactIndex()[b];
         }
         // Write script itself.         
         strScript << "// " << m_pszName << ".\x0D\x0A";
         if (m_iSex == UNREAL_FEMALE) 
            strScript << "class " << m_pszName << " extends TournamentFemale;\x0D\x0A";
         else 
            strScript << "class " << m_pszName << " extends TournamentMale;\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH IMPORT MESH=" << m_pszName << " ANIVFILE=MODELS\\" << m_pszName << "_a.3d DATAFILE=MODELS\\" << m_pszName << "_d.3d X=0 Y=0 Z=0 MLOD=0\x0D\x0A";
         strScript << "#exec MESH ORIGIN MESH=" << m_pszName << " X=0 Y=0 Z=0\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=All       STARTFRAME=0   NUMFRAMES=700\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimDnLg   STARTFRAME=1   NUMFRAMES=1 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimDnSm   STARTFRAME=2   NUMFRAMES=1 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimUpLg   STARTFRAME=3   NUMFRAMES=1 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimUpSm   STARTFRAME=4   NUMFRAMES=1 Group=Waiting\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=HeadHit   STARTFRAME=90  NUMFRAMES=1 Group=TakeHit\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LeftHit   STARTFRAME=95  NUMFRAMES=1 Group=TakeHit\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=GutHit    STARTFRAME=0   NUMFRAMES=1 Group=TakeHit\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RightHit  STARTFRAME=136 NUMFRAMES=1 Group=TakeHit\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StillFrRp STARTFRAME=177 NUMFRAMES=10 RATE=15 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StillLgFr STARTFRAME=187 NUMFRAMES=10 RATE=15 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StillSmFr STARTFRAME=197 NUMFRAMES=8  RATE=15 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TreadLg   STARTFRAME=235 NUMFRAMES=15 RATE=15 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TreadSm   STARTFRAME=250 NUMFRAMES=15 RATE=15 Group=Waiting\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath1L  STARTFRAME=373 NUMFRAMES=7  RATE=6	 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath2L  STARTFRAME=380 NUMFRAMES=20 RATE=7	 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=CockGunL  STARTFRAME=400 NUMFRAMES=8  RATE=6	 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Look      STARTFRAME=96  NUMFRAMES=40 RATE=15 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LookL     STARTFRAME=408 NUMFRAMES=40 RATE=15 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath1   STARTFRAME=25  NUMFRAMES=7  RATE=6	 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath2   STARTFRAME=32  NUMFRAMES=20 RATE=7	 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=CockGun   STARTFRAME=52  NUMFRAMES=8  RATE=6	 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Chat1     STARTFRAME=463 NUMFRAMES=13 RATE=6	 Group=Waiting\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Chat2     STARTFRAME=476 NUMFRAMES=10 RATE=6	 Group=Waiting\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Victory1  STARTFRAME=265 NUMFRAMES=18 RATE=11 Group=Gesture\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WaveL     STARTFRAME=448 NUMFRAMES=15 RATE=15 Group=Gesture\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Wave      STARTFRAME=343 NUMFRAMES=15 RATE=15 Group=Gesture\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Thrust    STARTFRAME=486 NUMFRAMES=15 RATE=20 Group=Gesture\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Taunt1    STARTFRAME=5   NUMFRAMES=20 RATE=15 Group=Gesture\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeB    STARTFRAME=501 NUMFRAMES=1  Group=Jumping\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeF    STARTFRAME=502 NUMFRAMES=1  Group=Jumping\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeR    STARTFRAME=503 NUMFRAMES=1  Group=Jumping\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeL    STARTFRAME=504 NUMFRAMES=1  Group=Jumping\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Flip      STARTFRAME=505 NUMFRAMES=20 Group=Jumping\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=JumpLgFr  STARTFRAME=91  NUMFRAMES=1  Group=Jumping\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=JumpSmFr  STARTFRAME=92  NUMFRAMES=1  Group=Jumping\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LandLgFr  STARTFRAME=93  NUMFRAMES=1  Group=Landing\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LandSmFr  STARTFRAME=94  NUMFRAMES=1  Group=Landing\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DuckWlkL  STARTFRAME=60  NUMFRAMES=15 RATE=15 Group=Ducking\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DuckWlkS  STARTFRAME=75  NUMFRAMES=15 RATE=15 Group=Ducking\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=SwimLg    STARTFRAME=205 NUMFRAMES=15 RATE=15\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=SwimSm    STARTFRAME=220 NUMFRAMES=15 RATE=15\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkLg    STARTFRAME=283 NUMFRAMES=15 RATE=18\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkLgFr  STARTFRAME=298 NUMFRAMES=15 RATE=18 Group=MovingFire\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkSm    STARTFRAME=313 NUMFRAMES=15 RATE=18\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkSmFr  STARTFRAME=328 NUMFRAMES=15 RATE=18 Group=MovingFire\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Walk      STARTFRAME=358 NUMFRAMES=15 RATE=18\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunLg     STARTFRAME=137 NUMFRAMES=10 RATE=17\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunLgFr   STARTFRAME=147 NUMFRAMES=10 RATE=17 Group=MovingFire\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunSm     STARTFRAME=157 NUMFRAMES=10 RATE=17\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunSmFr   STARTFRAME=167 NUMFRAMES=10 RATE=17 Group=MovingFire\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=BackRun   STARTFRAME=670 NUMFRAMES=10 RATE=17 Group=MovingFire\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StrafeL   STARTFRAME=680 NUMFRAMES=10 RATE=17 Group=MovingFire\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StrafeR   STARTFRAME=690 NUMFRAMES=10 RATE=17 Group=MovingFire\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TurnLg    STARTFRAME=298 NUMFRAMES=2  RATE=15\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TurnSm    STARTFRAME=328 NUMFRAMES=2  RATE=15\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Fighter   STARTFRAME=187 NUMFRAMES=1\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead1     STARTFRAME=525 NUMFRAMES=13 RATE=10 Group=TakeHit\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead2     STARTFRAME=538 NUMFRAMES=16 RATE=10\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead3     STARTFRAME=554 NUMFRAMES=13 RATE=10\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead4     STARTFRAME=567 NUMFRAMES=16 RATE=10\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead7     STARTFRAME=583 NUMFRAMES=21 RATE=12 Group=TakeHit\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead8     STARTFRAME=604 NUMFRAMES=18 RATE=10 Group=TakeHit\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead9     STARTFRAME=622 NUMFRAMES=20 RATE=30 Group=TakeHit\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead9B    STARTFRAME=642 NUMFRAMES=10 RATE=15\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead11    STARTFRAME=652 NUMFRAMES=18 RATE=10\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DeathEnd  STARTFRAME=537 NUMFRAMES=1\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DeathEnd2 STARTFRAME=553 NUMFRAMES=1\x0D\x0A";
         strScript << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DeathEnd3 STARTFRAME=566 NUMFRAMES=1\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESHMAP SCALE MESHMAP=" << m_pszName << " X=0.1 Y=0.1 Z=0.2\x0D\x0A";
         strScript << "\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLG   TIME=0.25 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLG   TIME=0.75 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLGFR TIME=0.25 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLGFR TIME=0.75 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSM   TIME=0.25 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSM   TIME=0.75 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSMFR TIME=0.25 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSMFR TIME=0.75 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeL TIME=0.25 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeL TIME=0.75 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeR TIME=0.25 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeR TIME=0.75 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=BackRun TIME=0.25 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=BackRun TIME=0.75 FUNCTION=PlayFootStep\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead1   TIME=0.7  FUNCTION=LandThump\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead2   TIME=0.9  FUNCTION=LandThump\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead3   TIME=0.45 FUNCTION=LandThump\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead4   TIME=0.6  FUNCTION=LandThump\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead7   TIME=0.7  FUNCTION=LandThump\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead8   TIME=0.7  FUNCTION=LandThump\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead9B  TIME=0.8  FUNCTION=LandThump\x0D\x0A";
         strScript << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead11  TIME=0.57 FUNCTION=LandThump\x0D\x0A";
         strScript << "\x0D\x0A";
         // Terminate
         strScript << (char)0;
      }
      else {
         delete [] pcSelectScript;
         delete [] pcBotProperties;
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      uCurrentOffset += uMeshScriptLength;

      // Update export offset
      uExportOffset[6] = uCurrentOffset;
      // Write Selection Mesh Properties
      unsigned int uSelectPropertiesLength = 0x57;
      char* pcSelectProperties = NULL;
      NEWBEGIN
      pcSelectProperties = new char[uSelectPropertiesLength];
      NEWEND("CAvatarFileUnreal::Save - Selection mesh properties data chunk allocation")
      if (pcSelectProperties != NULL) {
         unsigned int uPropertiesOffset = 0;
         // Write ID
         pcSelectProperties[uPropertiesOffset++] = (char)0x8B;
         // Write standard data
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         // Write specific data
         pcSelectProperties[uPropertiesOffset++] = (char)0x05;
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         // Write compact index of name table entry
         pcSelectProperties[uPropertiesOffset++] = (char)0x54;
         pcSelectProperties[uPropertiesOffset++] = (char)0x01;
         // Write standard data
         memset(pcSelectProperties+uPropertiesOffset,(char)-1,8);
         uPropertiesOffset+=8;
         memset(pcSelectProperties+uPropertiesOffset,(char)0,4);
         uPropertiesOffset+=4;
         // Write specific data
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         // Write standard data
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         // Write specific data
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
         // Write standard data
         memset(pcSelectProperties+uPropertiesOffset,(char)0,2);
         uPropertiesOffset+=2;
         memset(pcSelectProperties+uPropertiesOffset,(char)-1,10);
         uPropertiesOffset+=10;
         memset(pcSelectProperties+uPropertiesOffset,(char)0,4);
         uPropertiesOffset+=4;
         // Write specific data
         pcSelectProperties[uPropertiesOffset++] = (char)0x12;
         // Write standard data
         memset(pcSelectProperties+uPropertiesOffset,(char)0,19);
         uPropertiesOffset+=19;
         pcSelectProperties[uPropertiesOffset++] = (char)0x02;
         // Write specific data
         pcSelectProperties[uPropertiesOffset++] = (char)0x07;
         // Write standard data
         pcSelectProperties[uPropertiesOffset++] = (char)0x01;
         memset(pcSelectProperties+uPropertiesOffset,(char)0,7);
         uPropertiesOffset+=7;
         // Write ID again
         pcSelectProperties[uPropertiesOffset++] = (char)0x8B;
         // Write standard data
         pcSelectProperties[uPropertiesOffset++] = (char)0x01;
         memset(pcSelectProperties+uPropertiesOffset,(char)0,3);
         uPropertiesOffset+=3;
         // Write specific data
         pcSelectProperties[uPropertiesOffset++] = (char)0xF9;
         pcSelectProperties[uPropertiesOffset++] = (char)0xCE;
         pcSelectProperties[uPropertiesOffset++] = (char)0x3A;
         pcSelectProperties[uPropertiesOffset++] = (char)0x9E;
         // Write standard data
         pcSelectProperties[uPropertiesOffset++] = (char)0x04;
         pcSelectProperties[uPropertiesOffset++] = (char)0x08;
         pcSelectProperties[uPropertiesOffset++] = (char)0x09;
         pcSelectProperties[uPropertiesOffset++] = (char)0x0B;
         pcSelectProperties[uPropertiesOffset++] = (char)0x0A;
         pcSelectProperties[uPropertiesOffset++] = (char)0x85;
         // Finish
         pcSelectProperties[uPropertiesOffset++] = (char)0x0D;
         pcSelectProperties[uPropertiesOffset++] = (char)0x00;
      }
      else {
         delete [] pcMeshScript;
         delete [] pcSelectScript;
         delete [] pcBotProperties;
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      // Update file offset
      uCurrentOffset += uSelectPropertiesLength;

      // Update export offset
      uExportOffset[7] = uCurrentOffset;
      // Write ForceMeshToExist Data
      const unsigned int uForceMeshToExistDataLength = 0x20;
      char* pcForceMeshToExistData = NULL;
      NEWBEGIN
      pcForceMeshToExistData = new char[uForceMeshToExistDataLength];
      NEWEND("CAvatarFileUnreal::Save - ForceMeshToExist data allocation");
      if (pcForceMeshToExistData != NULL) {
         pcForceMeshToExistData[0x00] = 0x00;
         pcForceMeshToExistData[0x01] = 0x00;
         pcForceMeshToExistData[0x02] = 0x00;
         pcForceMeshToExistData[0x03] = 0x00;
         pcForceMeshToExistData[0x04] = 0x00;
         pcForceMeshToExistData[0x05] = 0x29;
         pcForceMeshToExistData[0x06] = 0x06;
         pcForceMeshToExistData[0x07] = 0x00;
         pcForceMeshToExistData[0x08] = 0x00;
         pcForceMeshToExistData[0x09] = 0x00;
         pcForceMeshToExistData[0x0A] = 0x58; // This one may well mean something - it has been known to change
         pcForceMeshToExistData[0x0B] = 0x00;
         pcForceMeshToExistData[0x0C] = 0x00;
         pcForceMeshToExistData[0x0D] = 0x00;
         pcForceMeshToExistData[0x0E] = 0x0A;
         pcForceMeshToExistData[0x0F] = 0x00;
         pcForceMeshToExistData[0x10] = 0x00;
         pcForceMeshToExistData[0x11] = 0x00;
         pcForceMeshToExistData[0x12] = 0x61;
         pcForceMeshToExistData[0x13] = 0x16;
         pcForceMeshToExistData[0x14] = 0x20;
         pcForceMeshToExistData[0x15] = 0x01;
         pcForceMeshToExistData[0x16] = 0x16;
         pcForceMeshToExistData[0x17] = 0x04;
         pcForceMeshToExistData[0x18] = 0x0B;
         pcForceMeshToExistData[0x19] = 0x00;
         pcForceMeshToExistData[0x1A] = 0x00;
         pcForceMeshToExistData[0x1B] = 0x00;
         pcForceMeshToExistData[0x1C] = 0x02;
         pcForceMeshToExistData[0x1D] = 0x00;
         pcForceMeshToExistData[0x1E] = 0x00;
         pcForceMeshToExistData[0x1F] = 0x00;
      }
      else {
         delete [] pcSelectProperties;
         delete [] pcMeshScript;
         delete [] pcSelectScript;
         delete [] pcBotProperties;
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      uCurrentOffset += uForceMeshToExistDataLength;

      // Update export offset
      uExportOffset[8] = uCurrentOffset;
      // Write Selection Model
      unsigned int uSelectMeshLength = 
         50 + (4*uNumVertices) + // vertex frame data
         9  + (sizeof(SUnrealTriangle)*uNumFaces) + // Face header + data
         5  + 19 + // sequence data
         9  + (8 * (uNumVertices+3)) + //vertex info
         50 + (12*uNumFaces) + // face info
         8  + 25 +// frame data part 1
         5  + 16 +// frame data part 2
         69; // Footer data

      char* pcSelectMesh = NULL;
      NEWBEGIN
      pcSelectMesh = new char[uSelectMeshLength];
      NEWEND("CAvatarFileUnreal::Save - selection mesh allocation failure");
      if (pcSelectMesh != NULL) {
         unsigned int uSelectOffset = 0; // counter variable
         int f,v; // loop variables

         ////////////////////////////////////////////////////////////
         // Write header
         ////////////////////////////////////////////////////////////

         // Write frame 0 description part 1
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x84;
         pcSelectMesh[uSelectOffset++] = (char)0xC2;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x1B;
         pcSelectMesh[uSelectOffset++] = (char)0xC3;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x5B;
         pcSelectMesh[uSelectOffset++] = (char)0xC3;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0xDE;
         pcSelectMesh[uSelectOffset++] = (char)0x42;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x1B;
         pcSelectMesh[uSelectOffset++] = (char)0x43;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x5C;
         pcSelectMesh[uSelectOffset++] = (char)0x43;
         pcSelectMesh[uSelectOffset++] = (char)0x01;
         // Write frame 0 description part 2
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0xB4;
         pcSelectMesh[uSelectOffset++] = (char)0x41;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x3F;
         pcSelectMesh[uSelectOffset++] = (char)0x28;
         pcSelectMesh[uSelectOffset++] = (char)0xE8;
         pcSelectMesh[uSelectOffset++] = (char)0x6E;
         pcSelectMesh[uSelectOffset++] = (char)0x43;

         // Store address of end of frame data chunk
         *((unsigned int*)(pcSelectMesh+uSelectOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcSelectMesh+uSelectOffset);
         uSelectOffset+=4;
         // Write CI of amount of data
         CUnrealCompactIndex oCIndex((int)uNumVertices);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write vertex data
         ////////////////////////////////////////////////////////////

         unsigned long* puTempPtr = (unsigned long*)(pcSelectMesh + uSelectOffset);
         // Reset Pose
         pAvatar->ResetPose();
         pAvatar->UpdateModel();
         // Get vertex info
         const SPoint3D* pVertices = pAvatar->Vertices();
         // Write vertices
         for (v=0; v<uNumVertices; v++) {
            // Store packed vertex
            puTempPtr[v] = PackVertex(pVertices[v],m_dScale,m_dYOffset);
            // Increment offset
            uSelectOffset += 4;
         }         
         // Store current offset in temp pointer
         *pTempAddressPtr = uCurrentOffset + uSelectOffset;

         ////////////////////////////////////////////////////////////
         // Write topology data header
         ////////////////////////////////////////////////////////////

         // Store address of end of face chunk
         *((unsigned int*)(pcSelectMesh+uSelectOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcSelectMesh+uSelectOffset);
         uSelectOffset+=4;
         // CI of number of faces
         oCIndex.Set((int)uNumFaces);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write topology data
         ////////////////////////////////////////////////////////////

         // Store texture info
         const int iMaxU = UNREAL_TEXTURE_WIDTH-1;
         const int iMaxV = UNREAL_TEXTURE_HEIGHT-1;
         // Some other stuff we need
         const STriFace* pFaces = pAvatar->Faces();
         const unsigned int uFaceTexIndex = pAvatar->TextureIndex(skullbase);
         // Cast pointer
         SUnrealTriangle* pFaceData = (SUnrealTriangle*)(pcSelectMesh + uSelectOffset);
         // Write faces
         for (f=0; f<uNumFaces; f++) {
            // Vertex indices
            pFaceData[f].uVertex0 = pFaces[f].m_iVertices[2];
            pFaceData[f].uVertex1 = pFaces[f].m_iVertices[1];
            pFaceData[f].uVertex2 = pFaces[f].m_iVertices[0];
            // Write Flags
            pFaceData[f].uFlags = UNREAL_U_TRI_NORMAL1SIDE;
            // Texture coordinates
            int iTextureNumber = pFaces[f].m_iTextureNumber;
            for (int tc=3; tc--!=0; ) {
               // Get tex coords
               double dU = pFaces[f].m_sTexCoords[tc].dU;
               double dV = pFaces[f].m_sTexCoords[tc].dV;
               // Clamp to 0..1
               if (dU > 1) dU = 1;
               else if (dU < 0) dU = 0;
               if (dV > 0.9) dV = 0.9;
               else if (dV < 0.1) dV = 0.1;
               // Recalculate texture V coordinates if this is not the face texture
               if (iTextureNumber!=uFaceTexIndex) {
                  // Multiply by height of this particular image
                  dV *= m_puImageHeight[iTextureNumber];
                  // Add where this image starts
                  dV += m_puFirstPixel[iTextureNumber];
                  // Divide by the total height of the image
                  dV /= m_uTotalHeight;
               }
               // Scale and store
               char cU = (dU*iMaxU)+0.5; // Add 0.5 to ensure correct rounding
               char cV = (dV*iMaxV)+0.5; // Add 0.5 to ensure correct rounding
               switch (tc) {
               case 2:
                  pFaceData[f].uU0 = cU;
                  pFaceData[f].uV0 = cV;
                  break;
               case 1:
                  pFaceData[f].uU1 = cU;
                  pFaceData[f].uV1 = cV;
                  break;
               case 0:
                  pFaceData[f].uU2 = cU;
                  pFaceData[f].uV2 = cV;
                  break;
               }
            }
            // Write Texture number
            pFaceData[f].uTexture = iTextureNumber==uFaceTexIndex ? 0 : 1;
         }
         uSelectOffset += uNumFaces * sizeof(SUnrealTriangle);
         // Store current offset in temp pointer
         *pTempAddressPtr = uCurrentOffset + uSelectOffset;

         ////////////////////////////////////////////////////////////
         // Write sequence info header
         ////////////////////////////////////////////////////////////

         // Write CI of number of sequences
         oCIndex.Set((int)1);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write sequence info
         ////////////////////////////////////////////////////////////

         // Write CI of sequence ID
         oCIndex.Set(0x0C);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }
         // Write group number
         pcSelectMesh[uSelectOffset++] = 0;
         // Write start frame for sequence
         *((long*)(pcSelectMesh+uSelectOffset)) = 0;
         uSelectOffset+=4;
         // Write number of frames
         *((long*)(pcSelectMesh+uSelectOffset)) = 1;
         uSelectOffset+=4;
         // Write number of notifies
         pcSelectMesh[uSelectOffset++] = 0;
         // Write rate
         *((float*)(pcSelectMesh+uSelectOffset)) = 30;
         uSelectOffset+=4;
         
         ////////////////////////////////////////////////////////////
         // Write vertex info header
         ////////////////////////////////////////////////////////////

         // Store address of end of vertex info chunk
         *((unsigned int*)(pcSelectMesh+uSelectOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcSelectMesh+uSelectOffset);
         uSelectOffset+=4;
         // Write CI of number of vertices
         oCIndex.Set((int)(uNumVertices));
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write vertex info
         ////////////////////////////////////////////////////////////

         // Cast pointer
         puTempPtr = (unsigned long*)(pcSelectMesh + uSelectOffset);
         unsigned int uTempOffset = 0;
         unsigned int uTotalValence = 0;
         for (v=0; v<uNumVertices; v++) {
            // Write vertex data
            puTempPtr[uTempOffset++] = pAvatar->FacesPerVertex(v)->Length(); // Valence
            puTempPtr[uTempOffset++] = uTotalValence; // Total
            uTotalValence += pAvatar->FacesPerVertex(v)->Length();
         }
         uSelectOffset += 8 * uNumVertices;
         // Store current offset in temp pointer
         *pTempAddressPtr = uCurrentOffset + uSelectOffset;

         ////////////////////////////////////////////////////////////
         // Write face info header
         ////////////////////////////////////////////////////////////

         // Write face info header
         // Write frame 0 description part 1
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x84;
         pcSelectMesh[uSelectOffset++] = (char)0xC2;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x1B;
         pcSelectMesh[uSelectOffset++] = (char)0xC3;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x5B;
         pcSelectMesh[uSelectOffset++] = (char)0xC3;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0xDE;
         pcSelectMesh[uSelectOffset++] = (char)0x42;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x1B;
         pcSelectMesh[uSelectOffset++] = (char)0x43;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x5C;
         pcSelectMesh[uSelectOffset++] = (char)0x43;
         pcSelectMesh[uSelectOffset++] = (char)0x01;
         // Write frame 0 description part 2
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0xB4;
         pcSelectMesh[uSelectOffset++] = (char)0x41;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x3F;
         pcSelectMesh[uSelectOffset++] = (char)0x28;
         pcSelectMesh[uSelectOffset++] = (char)0xE8;
         pcSelectMesh[uSelectOffset++] = (char)0x6E;
         pcSelectMesh[uSelectOffset++] = (char)0x43;

         // Store address of end of face info chunk
         *((unsigned int*)(pcSelectMesh+uSelectOffset)) = 0;
         pTempAddressPtr = (unsigned int*)(pcSelectMesh+uSelectOffset);
         uSelectOffset+=4;
         // Write CI of data length
         oCIndex.Set((int)uNumFaces*3);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write face info
         ////////////////////////////////////////////////////////////

         puTempPtr = (unsigned long*)(pcSelectMesh + uSelectOffset);
         uTempOffset = 0;
         for (f=0; f<uNumFaces; f++) {
            // Work out adjacent faces
            int iAdjacentFaces[3];
            for (int a=0; a<3; a++) {
               iAdjacentFaces[a] = -1;
               int b = (a+1) % 3;
               const CDList<int>* pFaceList = pAvatar->FacesPerVertex(pFaces[f].m_iVertices[a]);
               for (int i=0; (i<pFaceList->Length()) && (iAdjacentFaces[a]<0); i++) {
                  int iFace = (*pFaceList)[i];
                  if (iFace != f) { 
                     if ((pFaces[iFace].m_iVertices[0] == pFaces[f].m_iVertices[b]) ||
                         (pFaces[iFace].m_iVertices[1] == pFaces[f].m_iVertices[b]) ||
                         (pFaces[iFace].m_iVertices[2] == pFaces[f].m_iVertices[b])) 
                         iAdjacentFaces[a] = iFace;
                  }
               }
               // Make sure we're been successful
               ASSERT(iAdjacentFaces[a] != -1);
            }
            // Write adjacent face numbers
            puTempPtr[uTempOffset++] = iAdjacentFaces[2];
            puTempPtr[uTempOffset++] = iAdjacentFaces[0];
            puTempPtr[uTempOffset++] = iAdjacentFaces[1];
            // Make sure all three are different
            ASSERT((iAdjacentFaces[1] != iAdjacentFaces[2]) && (iAdjacentFaces[2] != iAdjacentFaces[0]) && (iAdjacentFaces[0] != iAdjacentFaces[1]));
         }
         uSelectOffset += 12 * uNumFaces;
         // Store current offset in temp pointer
         *pTempAddressPtr = uCurrentOffset + uSelectOffset;

         ////////////////////////////////////////////////////////////
         // Write frame data part 1 header
         ////////////////////////////////////////////////////////////

         // Write header data - don't know what this means
         pcSelectMesh[uSelectOffset++] = (char)0x02;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         // Write CI of number of frames
         oCIndex.Set((int)1);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write frame data part 1
         ////////////////////////////////////////////////////////////
   
         // Write frame data part 1 - 25 bytes per frame
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x84;
         pcSelectMesh[uSelectOffset++] = (char)0xC2;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x1B;
         pcSelectMesh[uSelectOffset++] = (char)0xC3;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x5B;
         pcSelectMesh[uSelectOffset++] = (char)0xC3;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0xDE;
         pcSelectMesh[uSelectOffset++] = (char)0x42;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x1B;
         pcSelectMesh[uSelectOffset++] = (char)0x43;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x5C;
         pcSelectMesh[uSelectOffset++] = (char)0x43;
         pcSelectMesh[uSelectOffset++] = (char)0x01;

         ////////////////////////////////////////////////////////////
         // Write frame data part 2 header
         ////////////////////////////////////////////////////////////

         // Write CI of number of frames
         oCIndex.Set((int)1);
         for (i=0; i<oCIndex.NumBytes(); i++) {
            pcSelectMesh[uSelectOffset++] = oCIndex.CompactIndex()[i];
         }

         ////////////////////////////////////////////////////////////
         // Write frame data part 2
         ////////////////////////////////////////////////////////////

         // Write frame data part 2 - 16 bytes per frame
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0xB4;
         pcSelectMesh[uSelectOffset++] = (char)0x41;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x3F;
         pcSelectMesh[uSelectOffset++] = (char)0x28;
         pcSelectMesh[uSelectOffset++] = (char)0xE8;
         pcSelectMesh[uSelectOffset++] = (char)0x6E;
         pcSelectMesh[uSelectOffset++] = (char)0x43;

         ////////////////////////////////////////////////////////////
         // Write footer
         ////////////////////////////////////////////////////////////

         // Number of vertices
         *((long*)(pcSelectMesh+uSelectOffset)) = uNumVertices;
         uSelectOffset+=4;
         // Number of sequences
         *((long*)(pcSelectMesh+uSelectOffset)) = 1;
         uSelectOffset+=4;
         // Standard data
         memset(pcSelectMesh+uSelectOffset,0,8);
         uSelectOffset+=8;
         pcSelectMesh[uSelectOffset++] = (char)0xCD;
         pcSelectMesh[uSelectOffset++] = (char)0xCC;
         pcSelectMesh[uSelectOffset++] = (char)0xCC;
         pcSelectMesh[uSelectOffset++] = (char)0x3D;
         pcSelectMesh[uSelectOffset++] = (char)0xCD;
         pcSelectMesh[uSelectOffset++] = (char)0xCC;
         pcSelectMesh[uSelectOffset++] = (char)0xCC;
         pcSelectMesh[uSelectOffset++] = (char)0x3D;
         pcSelectMesh[uSelectOffset++] = (char)0xCD;
         pcSelectMesh[uSelectOffset++] = (char)0xCC;
         pcSelectMesh[uSelectOffset++] = (char)0x4C;
         pcSelectMesh[uSelectOffset++] = (char)0x3E;
         memset(pcSelectMesh+uSelectOffset,0,32);
         uSelectOffset+=32;
         pcSelectMesh[uSelectOffset++] = (char)0x02;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x80;
         pcSelectMesh[uSelectOffset++] = (char)0x3F;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x00;
         pcSelectMesh[uSelectOffset++] = (char)0x80;
         pcSelectMesh[uSelectOffset++] = (char)0x3F;

         // Done - update size
         uSelectMeshLength = uSelectOffset;
      }
      else {
         delete [] pcForceMeshToExistData;
         delete [] pcSelectProperties;
         delete [] pcMeshScript;
         delete [] pcSelectScript;
         delete [] pcBotProperties;
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      uCurrentOffset += uSelectMeshLength;

      // Update header info
      sHeader.uImportOffset = uCurrentOffset;
      // Update export offset
      uExportOffset[9] = uCurrentOffset;
      // Write Import Table
      const unsigned int uImportTableLength = 104;
      unsigned char pcImportTable[uImportTableLength] = {
      	0x0A, 0x6B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x0A, 0x6B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x09, 
	      0x0A, 0x64, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x62, 0x01, 0x0A, 0x64, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 
	      0x64, 0x01, 0x0A, 0x64, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x63, 0x01, 0x0A, 0x6B, 0x01, 0x00, 0x00, 
	      0x00, 0x00, 0x0B, 0x0A, 0x64, 0x01, 0xFA, 0xFF, 0xFF, 0xFF, 0x0F, 0x0A, 0x64, 0x01, 0xFE, 0xFF, 
	      0xFF, 0xFF, 0x66, 0x01, 0x0A, 0x64, 0x01, 0xFE, 0xFF, 0xFF, 0xFF, 0x68, 0x01, 0x0A, 0x64, 0x01, 
	      0xFE, 0xFF, 0xFF, 0xFF, 0x65, 0x01, 0x0A, 0x64, 0x01, 0xFE, 0xFF, 0xFF, 0xFF, 0x67, 0x01, 0x0A, 
	      0x64, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x69, 0x01,
      };
      uCurrentOffset += uImportTableLength;

      // Update header info
      sHeader.uExportOffset = uCurrentOffset;
      // Write Export Table
      unsigned int uExportTableLength = 9 * 24;
      char* pcExportTable = NULL;
      NEWBEGIN
      pcExportTable = new char[uExportTableLength];
      NEWEND("CAvatarFileUnreal::Save - export table allocation");
      if (pcExportTable != NULL) {
         unsigned int uExportTableOffset = 0;
         // Write export table
         const int piClass[9] = {
            UNREAL_CLASS_MESHPROPERTIES, UNREAL_CLASS_MODEL, UNREAL_CLASS_BOTSCRIPT,
            UNREAL_CLASS_BOTPROPERTIES, UNREAL_CLASS_SELECTSCRIPT, UNREAL_CLASS_MESHSCRIPT,
            UNREAL_CLASS_SELECTPROPERTIES, UNREAL_CLASS_FORCEMESHTOEXIST, UNREAL_CLASS_MODEL
         };
         const char piName[9] = {
            (char)0x08, (char)0x08, (char)0x6A,
            (char)0x5E, (char)0x6A, (char)0x6A,
            (char)0x54, (char)0x29, (char)0x54,
         };
         for (i=0; i<9; i++) {
            int iSize = uExportOffset[i+1] - uExportOffset[i];
            int iOffset = uExportOffset[i];
            int b; // loop counter
            // Write export declaration
            *((long*)(pcExportTable+uExportTableOffset)) = piClass[i];
            uExportTableOffset += 4;
            // Couple of zeroes
            pcExportTable[uExportTableOffset++] = 0x0;
            pcExportTable[uExportTableOffset++] = 0x0;
            // CI of name
            CUnrealCompactIndex oCIndex((int)piName[i]);
            for (b=0; b<oCIndex.NumBytes(); b++) {
               pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[b];
            }
            // Flags
            long uFlags;
            if (piName[i] == 0x6A) uFlags = UNREAL_RF_SCRIPTFLAG | UNREAL_RF_LOADFOREDIT;
            else if (piName[i] == 0x29) uFlags = UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER | UNREAL_RF_LOADFOREDIT | UNREAL_RF_PUBLIC;
            else uFlags = UNREAL_RF_LOADFORCLIENT | UNREAL_RF_LOADFORSERVER | UNREAL_RF_LOADFOREDIT | UNREAL_RF_PUBLIC | UNREAL_RF_STANDALONE;
            *((long*)(pcExportTable+uExportTableOffset)) = uFlags; // Object flags
            uExportTableOffset+=4;
            // CI of size
            oCIndex.Set(iSize); // CI of data size
            for (b=0; b<oCIndex.NumBytes(); b++) {
               pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[b];
            }
            // CI of offset
            oCIndex.Set(iOffset); // CI of data address
            for (b=0; b<oCIndex.NumBytes(); b++) {
               pcExportTable[uExportTableOffset++] = oCIndex.CompactIndex()[b];
            }
         }
         uExportTableLength = uExportTableOffset;
      }
      else {
         delete [] pcSelectMesh;
         delete [] pcForceMeshToExistData;
         delete [] pcSelectProperties;
         delete [] pcMeshScript;
         delete [] pcSelectScript;
         delete [] pcBotProperties;
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_OUT_OF_MEMORY;
      }
      uCurrentOffset += uExportTableLength;

      // Write data chunks to stream in order
      // Write header
      osOutputStream.write((char*)&sHeader,sizeof(SUnrealPackageHeader));
      // Write Name Table
      osOutputStream.write(pcNameTable,uNameTableLength);
      // Write mesh properties
      osOutputStream.write(pcMeshProperties,uMeshPropertiesLength);
      // Write mesh top data
      osOutputStream.write(pcMeshHeader,uMeshHeaderLength);
      // Write mesh frame data
      ifstream isTempStream(m_pszTempFilename,ios::in|ios::binary);
      if (!isTempStream.fail()) {
         osOutputStream << isTempStream.rdbuf();
         isTempStream.close();
      }
      else {
         delete [] pcExportTable;
         delete [] pcSelectMesh;
         delete [] pcForceMeshToExistData;
         delete [] pcSelectProperties;
         delete [] pcMeshScript;
         delete [] pcSelectScript;
         delete [] pcBotProperties;
         delete [] pcBotScript;
         delete [] pcMeshData;
         delete [] pcMeshHeader;
         delete [] pcMeshProperties;
         delete [] pcNameTable;
         return F_FILE_ERROR;
      }
      // Write mesh bottom data
      osOutputStream.write(pcMeshData,uMeshDataLength);
      // Write Bot Script
      osOutputStream.write(pcBotScript,uBotScriptLength);
      // Write Bot properties
      osOutputStream.write(pcBotProperties,uBotPropertiesLength);
      // Write SelectionMesh script
      osOutputStream.write(pcSelectScript,uSelectScriptLength);
      // Write Mesh script
      osOutputStream.write(pcMeshScript,uMeshScriptLength);
      // Write SelectionMesh properties
      osOutputStream.write(pcSelectProperties,uSelectPropertiesLength);
      // Write ForceMeshToExist data
      osOutputStream.write(pcForceMeshToExistData,uForceMeshToExistDataLength);
      // Write Selection Mesh
      osOutputStream.write(pcSelectMesh,uSelectMeshLength);
      // Write import table
      osOutputStream.write(pcImportTable,uImportTableLength);
      // Write export table
      osOutputStream.write(pcExportTable,uExportTableLength);
      
      // Done - close stream
      osOutputStream.close();

      // Delete allocated memory
      delete [] pcExportTable;
      delete [] pcSelectMesh;
      delete [] pcForceMeshToExistData;
      delete [] pcSelectProperties;
      delete [] pcMeshScript;
      delete [] pcSelectScript;
      delete [] pcBotProperties;
      delete [] pcBotScript;
      delete [] pcMeshData;
      delete [] pcMeshHeader;
      delete [] pcMeshProperties;
      delete [] pcNameTable;

      // Restore old pose
      pAvatar->ImportPose(poOldPose);      

   }
   else eResult = F_FILE_ERROR;
   // Done!
   return eResult;
} //SaveMeshU(const char* pszFilename, CAvatar* pAvatar) const

FRESULT CAvatarFileUnreal::SaveMeshD3D(const char* pszFilename, CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Save mesh geometry file
   // This is documented on the nervedamage.com unreal page
   int i=0; // generic loop counter
   const unsigned int uFaceTexIndex = pAvatar->TextureIndex(skullbase);
   ofstream osOutputStream(pszFilename,ios::binary|ios::out);   
	if (!osOutputStream.fail()) {
      // Store texture info
      const int iMaxU = UNREAL_TEXTURE_WIDTH-1;
      const int iMaxV = UNREAL_TEXTURE_HEIGHT-1;
      // Write header
      short iNumFaces = pAvatar->NumFaces();
      short uNumVertices = pAvatar->NumVertices();
      short iTotalNumFaces = iNumFaces+1; // add one for weapon tri
      short iTotalNumVertices = uNumVertices+3; // add three for weapon tri
   	osOutputStream.write((char*)&iTotalNumFaces,2);
   	osOutputStream.write((char*)&iTotalNumVertices,2);
      // 32 zeroes
      for (i=0; i<32; i++) osOutputStream.write("\0",1);
      // Write empty space after header
      for (i=0; i<12; i++) osOutputStream.write("\0",1);
      // Write data for body triangles
      const STriFace* pFaces = pAvatar->Faces();
      for (i=0; i<iNumFaces; i++) {
         // Vertex indices
         unsigned short iVertex0 = pFaces[i].m_iVertices[0];
         unsigned short iVertex1 = pFaces[i].m_iVertices[1];
         unsigned short iVertex2 = pFaces[i].m_iVertices[2];
      	osOutputStream.write((char*)&iVertex2,2);
      	osOutputStream.write((char*)&iVertex1,2);
      	osOutputStream.write((char*)&iVertex0,2);
         // Triangle type
         char cTriangleType = UNREAL_3D_TRI_NORMAL1SIDE;
         osOutputStream.write(&cTriangleType,1);
         // Colour
         char cColour = 0x7F;
         osOutputStream.write(&cColour,1);
         // Texture coordinates
         int iTextureNumber = pFaces[i].m_iTextureNumber;
         for (int tc=3; tc--!=0; ) {
            // Get tex coords
            double dU = pFaces[i].m_sTexCoords[tc].dU;
            double dV = pFaces[i].m_sTexCoords[tc].dV;
            // Clamp to 0..1
            if (dU > 1) dU = 1;
            else if (dU < 0) dU = 0;
            if (dV > 0.9) dV = 0.9;
            else if (dV < 0.1) dV = 0.1;
            // Recalculate texture V coordinates if appropriate
            if (iTextureNumber!=uFaceTexIndex) {
               // Multiply by height of this particular image
               dV *= m_puImageHeight[iTextureNumber];
               // Add where this image starts
               dV += m_puFirstPixel[iTextureNumber];
               // Divide by the total height of the image
               dV /= m_uTotalHeight;
            }
            // Scale and store
            char cU = (dU*iMaxU)+0.5; // Add 0.5 to ensure correct rounding
            char cV = (dV*iMaxV)+0.5; // Add 0.5 to ensure correct rounding
            osOutputStream.write(&cU,1);
            osOutputStream.write(&cV,1);
         }
         // Write Texture number
         osOutputStream.write((iTextureNumber==uFaceTexIndex) ? "\0x00" : "\x01", 1);
         // Write Flags
         osOutputStream.write("\0",1);
      }
      // Write data for weapon triangle
      // Write vertex indices: three vertices after main vertex list
      unsigned short sVertexNumber = uNumVertices;
      osOutputStream.write((char*)&sVertexNumber,2);
      sVertexNumber++;
      osOutputStream.write((char*)&sVertexNumber,2);
      sVertexNumber++;
      osOutputStream.write((char*)&sVertexNumber,2);
      // Triangle type
      char cTriangleType = UNREAL_3D_TRI_WEAPONTRI;
      osOutputStream.write(&cTriangleType,1);
      // Colour
      osOutputStream.write("\0",1);
      // Texture coordinates
      osOutputStream.write("\0\0\0\0\0\0",6);
      // Texture number
      osOutputStream.write("\0",1);
      // Write Flags
      osOutputStream.write("\0",1);
      // All done!
      osOutputStream.close();
	}
   else eResult = F_FILE_ERROR;
   // Done
   return eResult;   
}

FRESULT CAvatarFileUnreal::SaveMeshA3D(const char* pszFilename, CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Save mesh frame file
   // This is documented on the nervedamage.com unreal page
   ofstream osOutputStream(pszFilename,ios::binary|ios::out);
	if (!osOutputStream.fail()) {
      const SBodyPart* pBodyParts = pAvatar->BodyParts();
      // Get vertex info
      int uNumVertices = pAvatar->NumVertices();
      const SPoint3D* pVertices = pAvatar->Vertices();
      // Write header
      short iNumFrames = 700;
      osOutputStream.write((char*)&iNumFrames,2);
      short iFrameLength = (uNumVertices + 3) * 4; // 4 bytes per vertex, plus three weapon triangle vertices
      osOutputStream.write((char*)&iFrameLength,2);
      // Read rest of data from the temp file created in the SaveMeshU function.
      // (this file should be deleted by this point in the real thing)
      ifstream isTempStream;
      sprintf(m_pszTempFilename+strlen(m_pszTgtDir),"tempframedata.bin");
      isTempStream.open(m_pszTempFilename,ios::in|ios::binary);
      if (!isTempStream.fail()) {
         osOutputStream << isTempStream.rdbuf();
         isTempStream.close();
      }
      else eResult = F_FILE_ERROR;
      // Done
      osOutputStream.close();
	}
   else eResult = F_FILE_ERROR;
   // Done
   return eResult;   
}

FRESULT CAvatarFileUnreal::SaveSelectionMeshD3D(const char* pszFilename, CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Save selection mesh geometry file
   // This is documented on the nervedamage.com unreal page
   int i=0; // generic loop counter
   const unsigned int uFaceTexIndex = pAvatar->TextureIndex(skullbase);
   ofstream osOutputStream(pszFilename,ios::binary|ios::out);
	if (!osOutputStream.fail()) {
      // Store texture info
      const int iMaxU = UNREAL_TEXTURE_WIDTH-1;
      const int iMaxV = UNREAL_TEXTURE_HEIGHT-1;
      // Write header
      const short uNumFaces = pAvatar->NumFaces();
      const short uNumVertices = pAvatar->NumVertices();
   	osOutputStream.write((char*)&uNumFaces,2);
   	osOutputStream.write((char*)&uNumVertices,2);
      // 32 zeroes
      for (i=0; i<32; i++) osOutputStream.write("\0",1);
      // Write empty space after header
      for (i=0; i<12; i++) osOutputStream.write("\0",1);
      // Write data for body triangles
      const STriFace* pFaces = pAvatar->Faces();
      for (i=0; i<uNumFaces; i++) {
         // Vertex indices
         unsigned short iVertex0 = pFaces[i].m_iVertices[0];
         unsigned short iVertex1 = pFaces[i].m_iVertices[1];
         unsigned short iVertex2 = pFaces[i].m_iVertices[2];
      	osOutputStream.write((char*)&iVertex2,2);
      	osOutputStream.write((char*)&iVertex1,2);
      	osOutputStream.write((char*)&iVertex0,2);
         // Triangle type
         char cTriangleType = UNREAL_3D_TRI_NORMAL1SIDE;
         osOutputStream.write(&cTriangleType,1);
         // Colour
         char cColour = 0x7F;
         osOutputStream.write(&cColour,1);
         // Texture coordinates
         int iTextureNumber = pFaces[i].m_iTextureNumber;
         for (int tc=3; tc--!=0; ) {
            // Get tex coords
            double dU = pFaces[i].m_sTexCoords[tc].dU;
            double dV = pFaces[i].m_sTexCoords[tc].dV;
            // Clamp to 0..1
            if (dU > 1) dU = 1;
            else if (dU < 0) dU = 0;
            if (dV > 0.9) dV = 0.9;
            else if (dV < 0.1) dV = 0.1;
            // Recalculate texture V coordinates if appropriate
            if (iTextureNumber!=uFaceTexIndex) {
               // Multiply by height of this particular image
               dV *= m_puImageHeight[iTextureNumber];
               // Add where this image starts
               dV += m_puFirstPixel[iTextureNumber];
               // Divide by the total height of the image
               dV /= m_uTotalHeight;
            }
            // Scale and store
            char cU = (dU*iMaxU)+0.5; // Add 0.5 to ensure correct rounding
            char cV = (dV*iMaxV)+0.5; // Add 0.5 to ensure correct rounding
            osOutputStream.write(&cU,1);
            osOutputStream.write(&cV,1);
         }
         // Write Texture number
         osOutputStream.write((iTextureNumber==uFaceTexIndex) ? "\0x00" : "\x01", 1);
         // Write Flags
         osOutputStream.write("\0",1);
      }
      // All done!
      osOutputStream.close();
	}
   else eResult = F_FILE_ERROR;
   // Done
   return eResult;
}

FRESULT CAvatarFileUnreal::SaveSelectionMeshA3D(const char* pszFilename, CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Save selection mesh frame file
   // This is documented on the nervedamage.com unreal page
   ofstream osOutputStream(pszFilename,ios::binary|ios::out);
	if (!osOutputStream.fail()) {
      // Reset avatar pose
      pAvatar->ResetPose();
      pAvatar->UpdateModel();
      // Get vertex info
      const int uNumVertices = pAvatar->NumVertices();
      const SPoint3D* pVertices = pAvatar->Vertices();
      // Write header
      short iNumFrames = 1;
      osOutputStream.write((char*)&iNumFrames,2);
      short iFrameLength = uNumVertices * 4; // 4 bytes per vertex
      osOutputStream.write((char*)&iFrameLength,2);
      // Write frame
      int v = 0; // loop counter
      // Write vertices
      for (v=0; v<uNumVertices; v++) {
         // Pack vertex into a long int 
         long lVertex = PackVertex(pVertices[v],m_dScale,m_dYOffset);
         // Write long to stream
         osOutputStream.write((char*)&lVertex,4);
      }
      // Done
      osOutputStream.close();
	}
   else eResult = F_FILE_ERROR;
   // Done
   return eResult;
}

FRESULT CAvatarFileUnreal::SaveMeshScript(const char* pszFilename, CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Save selection mesh script
   ofstream osOutputStream(pszFilename,ios::out);
	if (!osOutputStream.fail()) {
      osOutputStream << "// " << m_pszName << ".\n";
      if (m_iSex == UNREAL_FEMALE) 
         osOutputStream << "class " << m_pszName << " extends TournamentFemale;\n";
      else 
         osOutputStream << "class " << m_pszName << " extends TournamentMale;\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH IMPORT MESH=" << m_pszName << " ANIVFILE=MODELS\\" << m_pszName << "_a.3d DATAFILE=MODELS\\" << m_pszName << "_d.3d X=0 Y=0 Z=0 MLOD=0\n";
      osOutputStream << "#exec MESH ORIGIN MESH=" << m_pszName << " X=0 Y=0 Z=0\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=All       STARTFRAME=0   NUMFRAMES=700\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimDnLg   STARTFRAME=1   NUMFRAMES=1 Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimDnSm   STARTFRAME=2   NUMFRAMES=1 Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimUpLg   STARTFRAME=3   NUMFRAMES=1 Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=AimUpSm   STARTFRAME=4   NUMFRAMES=1 Group=Waiting\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=HeadHit   STARTFRAME=90  NUMFRAMES=1	Group=TakeHit\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LeftHit   STARTFRAME=95  NUMFRAMES=1	Group=TakeHit\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=GutHit    STARTFRAME=0   NUMFRAMES=1	Group=TakeHit\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RightHit  STARTFRAME=136 NUMFRAMES=1	Group=TakeHit\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StillFrRp STARTFRAME=177 NUMFRAMES=10 RATE=15	Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StillLgFr STARTFRAME=187 NUMFRAMES=10 RATE=15	Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StillSmFr STARTFRAME=197 NUMFRAMES=8  RATE=15	Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TreadLg   STARTFRAME=235 NUMFRAMES=15 RATE=15	Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TreadSm   STARTFRAME=250 NUMFRAMES=15 RATE=15	Group=Waiting\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath1L  STARTFRAME=373 NUMFRAMES=7  RATE=6  Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath2L  STARTFRAME=380 NUMFRAMES=20 RATE=7  Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=CockGunL  STARTFRAME=400 NUMFRAMES=8  RATE=6  Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Look      STARTFRAME=96  NUMFRAMES=40 RATE=15 Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LookL     STARTFRAME=408 NUMFRAMES=40 RATE=15 Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath1   STARTFRAME=25  NUMFRAMES=7  RATE=6  Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Breath2   STARTFRAME=32  NUMFRAMES=20 RATE=7  Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=CockGun   STARTFRAME=52  NUMFRAMES=8  RATE=6  Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Chat1     STARTFRAME=463 NUMFRAMES=13 RATE=6  Group=Waiting\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Chat2     STARTFRAME=476 NUMFRAMES=10 RATE=6  Group=Waiting\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Victory1  STARTFRAME=265 NUMFRAMES=18 RATE=11 Group=Gesture\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WaveL     STARTFRAME=448 NUMFRAMES=15 RATE=15	Group=Gesture\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Wave      STARTFRAME=343 NUMFRAMES=15 RATE=15	Group=Gesture\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Thrust    STARTFRAME=486 NUMFRAMES=15 RATE=20	Group=Gesture\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Taunt1    STARTFRAME=5   NUMFRAMES=20 RATE=15	Group=Gesture\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeB    STARTFRAME=501 NUMFRAMES=1  Group=Jumping\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeF    STARTFRAME=502 NUMFRAMES=1  Group=Jumping\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeR    STARTFRAME=503 NUMFRAMES=1  Group=Jumping\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DodgeL    STARTFRAME=504 NUMFRAMES=1  Group=Jumping\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Flip      STARTFRAME=505 NUMFRAMES=20 Group=Jumping\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=JumpLgFr  STARTFRAME=91  NUMFRAMES=1  Group=Jumping\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=JumpSmFr  STARTFRAME=92  NUMFRAMES=1  Group=Jumping\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LandLgFr  STARTFRAME=93  NUMFRAMES=1  Group=Landing\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=LandSmFr  STARTFRAME=94  NUMFRAMES=1  Group=Landing\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DuckWlkL  STARTFRAME=60  NUMFRAMES=15 RATE=15	Group=Ducking\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DuckWlkS  STARTFRAME=75  NUMFRAMES=15 RATE=15	Group=Ducking\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=SwimLg    STARTFRAME=205 NUMFRAMES=15 RATE=15\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=SwimSm    STARTFRAME=220 NUMFRAMES=15 RATE=15\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkLg    STARTFRAME=283 NUMFRAMES=15 RATE=18\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkLgFr  STARTFRAME=298 NUMFRAMES=15 RATE=18	Group=MovingFire\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkSm    STARTFRAME=313 NUMFRAMES=15 RATE=18\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=WalkSmFr  STARTFRAME=328 NUMFRAMES=15 RATE=18	Group=MovingFire\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Walk      STARTFRAME=358 NUMFRAMES=15 RATE=18\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunLg     STARTFRAME=137 NUMFRAMES=10 RATE=17\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunLgFr   STARTFRAME=147 NUMFRAMES=10 RATE=17 Group=MovingFire\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunSm     STARTFRAME=157 NUMFRAMES=10 RATE=17\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=RunSmFr   STARTFRAME=167 NUMFRAMES=10 RATE=17	Group=MovingFire\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=BackRun   STARTFRAME=670 NUMFRAMES=10 RATE=17	Group=MovingFire\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StrafeL   STARTFRAME=680 NUMFRAMES=10 RATE=17	Group=MovingFire\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=StrafeR   STARTFRAME=690 NUMFRAMES=10 RATE=17	Group=MovingFire\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TurnLg    STARTFRAME=298 NUMFRAMES=2  RATE=15\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=TurnSm    STARTFRAME=328 NUMFRAMES=2  RATE=15\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Fighter   STARTFRAME=187 NUMFRAMES=1\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead1     STARTFRAME=525 NUMFRAMES=13 RATE=10	Group=TakeHit\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead2     STARTFRAME=538 NUMFRAMES=16 RATE=10\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead3     STARTFRAME=554 NUMFRAMES=13 RATE=10\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead4     STARTFRAME=567 NUMFRAMES=16 RATE=10\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead7     STARTFRAME=583 NUMFRAMES=21 RATE=12	Group=TakeHit\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead8     STARTFRAME=604 NUMFRAMES=18 RATE=10	Group=TakeHit\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead9     STARTFRAME=622 NUMFRAMES=20 RATE=30	Group=TakeHit\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead9B    STARTFRAME=642 NUMFRAMES=10 RATE=15\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=Dead11    STARTFRAME=652 NUMFRAMES=18 RATE=10\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DeathEnd  STARTFRAME=537 NUMFRAMES=1\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DeathEnd2 STARTFRAME=553 NUMFRAMES=1\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=" << m_pszName << " SEQ=DeathEnd3 STARTFRAME=566 NUMFRAMES=1\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESHMAP SCALE MESHMAP=" << m_pszName << " X=0.1 Y=0.1 Z=0.2\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLG   TIME=0.25 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLG   TIME=0.75 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLGFR TIME=0.25 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunLGFR TIME=0.75 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSM   TIME=0.25 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSM   TIME=0.75 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSMFR TIME=0.25 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=RunSMFR TIME=0.75 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeL TIME=0.25 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeL TIME=0.75 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeR TIME=0.25 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=StrafeR TIME=0.75 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=BackRun TIME=0.25 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=BackRun TIME=0.75 FUNCTION=PlayFootStep\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead1   TIME=0.7  FUNCTION=LandThump\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead2   TIME=0.9  FUNCTION=LandThump\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead3   TIME=0.45 FUNCTION=LandThump\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead4   TIME=0.6  FUNCTION=LandThump\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead7   TIME=0.7  FUNCTION=LandThump\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead8   TIME=0.7  FUNCTION=LandThump\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead9B  TIME=0.8  FUNCTION=LandThump\n";
      osOutputStream << "#exec MESH NOTIFY MESH=" << m_pszName << " SEQ=Dead11  TIME=0.57 FUNCTION=LandThump\n";
      osOutputStream << "\n";
      osOutputStream << "defaultproperties\n";
      osOutputStream << "{\n";
      osOutputStream << "   FaceSkin=0\n";
      osOutputStream << "   TeamSkin1=1\n";
      osOutputStream << "   TeamSkin2=2\n";
      osOutputStream << "   DefaultSkinName=\"" << m_pszName << "Skins." << m_pszTexBaseName << "\"\n";
      osOutputStream << "   DefaultPackage=\"" << m_pszName << "Skins.\"\n";
      osOutputStream << "   SelectionMesh=\"" << m_pszName << ".Select" << m_pszName << "\"\n";
      if (m_iSex == UNREAL_FEMALE) {
         osOutputStream << "   LandGrunt=Sound'FemaleSounds.(All).land10'\n";
         osOutputStream << "   CarcassType=Class'Botpack.TFemale2Carcass'\n";
         osOutputStream << "   SpecialMesh=\"Botpack.TrophyFemale2\"\n";
         osOutputStream << "   VoiceType=\"Botpack.VoiceFemaleTwo\"\n";
      }
      else {
         osOutputStream << "   LandGrunt=Sound'MaleSounds.(All).land10'\n";
         osOutputStream << "   CarcassType=Class'Botpack.TMale2Carcass'\n";
         osOutputStream << "   SpecialMesh=\"Botpack.TrophyMale2\"\n";
         osOutputStream << "   VoiceType=\"Botpack.VoiceMaleTwo\"\n";
      }
      osOutputStream << "   MenuName=\"" << m_pszName << "\"\n";
      osOutputStream << "   Mesh=" << m_pszName << "\n";
      osOutputStream << "}\n";
      osOutputStream.close();
   }  
   else eResult = F_FILE_ERROR;
   // Done
   return eResult;
} //SaveMeshScript(const char* pszFilename, CAvatar* pAvatar) const 

FRESULT CAvatarFileUnreal::SaveSelectionMeshScript(const char* pszFilename, CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Save selection mesh script
   ofstream osOutputStream(pszFilename,ios::out);
	if (!osOutputStream.fail()) {
      osOutputStream << "// Select" << m_pszName << ".\n";
      osOutputStream << "class Select" << m_pszName << " extends SelectionDude;\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH IMPORT MESH=Select" << m_pszName << " ANIVFILE=MODELS\\Select" << m_pszName << "_a.3d DATAFILE=MODELS\\Select" << m_pszName << "_d.3d X=0 Y=0 Z=0 MLOD=0\n";
      osOutputStream << "#exec MESH ORIGIN MESH=Select" << m_pszName << " X=0 Y=0 Z=0\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESH SEQUENCE MESH=Select" << m_pszName << " SEQ=All STARTFRAME=0 NUMFRAMES=1\n";
      osOutputStream << "\n";
      osOutputStream << "#exec MESHMAP SCALE MESHMAP=Select" << m_pszName << " X=0.1 Y=0.1 Z=0.2\n";
      osOutputStream << "\n";
      osOutputStream << "defaultproperties\n";
      osOutputStream << "{\n";
      osOutputStream << "}\n";
      osOutputStream.close();
   }
   else eResult = F_FILE_ERROR;
   // Done
   return eResult;
} //SaveSelectionMeshScript(const char* pszFilename, CAvatar* pAvatar) const

FRESULT CAvatarFileUnreal::SaveBotScript(const char* pszFilename, CAvatar* pAvatar) const {
   FRESULT eResult = F_OK;
   // Save bot script
   ofstream osOutputStream(pszFilename,ios::out);
	if (!osOutputStream.fail()) {
      osOutputStream << "// " << m_pszName << "Bot.\n";
      if (m_iSex == UNREAL_FEMALE) 
         osOutputStream << "class " << m_pszName << "Bot extends FemaleBotPlus;\n";
      else 
         osOutputStream << "class " << m_pszName << "Bot extends MaleBotPlus;\n";
      osOutputStream << "\n";
      osOutputStream << "function ForceMeshToExist()\n";
      osOutputStream << "{\n";
	   osOutputStream << "   Spawn(class'" << m_pszName << "');\n";
      osOutputStream << "}\n";
      osOutputStream << "\n";
      osOutputStream << "defaultproperties\n";
      osOutputStream << "{\n";
      if (m_iSex == UNREAL_FEMALE) {
         osOutputStream << "   LandGrunt=Sound'FemaleSounds.(All).land10'\n";
         osOutputStream << "   CarcassType=Class'Botpack.TFemale2Carcass'\n";
      }
      else {
         osOutputStream << "   LandGrunt=Sound'MaleSounds.(All).land10'\n";
         osOutputStream << "   CarcassType=Class'Botpack.TMale2Carcass'\n";
      }
      osOutputStream << "   FaceSkin=0\n";
      osOutputStream << "   TeamSkin1=1\n";
      osOutputStream << "   TeamSkin2=2\n";
      osOutputStream << "   DefaultSkinName=\"" << m_pszName << "Skins." << m_pszTexBaseName << "\"\n";
      osOutputStream << "   DefaultPackage=\"" << m_pszName << "Skins.\"\n";
      osOutputStream << "   SelectionMesh=\"" << m_pszName << ".Select" << m_pszName << "\"\n";
      osOutputStream << "   MenuName=\"" << m_pszName << "\"\n";
      osOutputStream << "   Mesh=" << m_pszName << "\n";
      osOutputStream << "}\n";
      osOutputStream.close();
   }  
   else eResult = F_FILE_ERROR;
   // Done
   return eResult;
} //SaveBotScript(const char* pszFilename, CAvatar* pAvatar) const 

long CAvatarFileUnreal::PackVertex(const SPoint3D& pntVertex, double dScale, double dYOff) const {
   long int lVertex = ( long int( pntVertex.m_dComponents[2] * dScale *  8.0 ) & 0x7ff );
   lVertex |= ( ( long int( -pntVertex.m_dComponents[0] * dScale * 8.0 ) & 0x7ff ) << 11 );
   lVertex |= ( ( long int( ((pntVertex.m_dComponents[1] * dScale) + dYOff) * 4.0 ) & 0x3ff ) << 22 );
   return lVertex;
} //PackVertex(const SPoint3D& pntVertex, double dScale, double dYOff) const

FRESULT CAvatarFileUnreal::Cleanup(void) const {
   FRESULT eResult = F_OK;
   // Remove temp file created by SaveMeshU
   sprintf(m_pszTempFilename+strlen(m_pszTgtDir),"tempframedata.bin");
   if ((remove(m_pszTempFilename) == -1) && (errno != ENOENT)) {
      TRACE("Could not remove temp frame data file\n");
      eResult = F_FILE_ERROR;
   }
   // Delete memory
   if (m_pszName != NULL) {
      delete [] m_pszName; 
      m_pszName = NULL;
   }
   if (m_pszTgtDir != NULL) {
      delete [] m_pszTgtDir; 
      m_pszTgtDir = NULL;
   }
   if (m_pszTexPackageName != NULL) {
      delete [] m_pszTexPackageName; 
      m_pszTexPackageName = NULL;
   }
   if (m_pszTexBaseName != NULL) {
      delete [] m_pszTexBaseName; 
      m_pszTexBaseName = NULL;
   }
   if (m_puFirstPixel != NULL) {
      delete [] m_puFirstPixel; 
      m_puFirstPixel = NULL;
   }
   if (m_puImageHeight != NULL) {
      delete [] m_puImageHeight; 
      m_puImageHeight = NULL;
   }
   return eResult;
} //Cleanup(void) const

void CAvatarFileUnreal::SetOption(int iOption, int iData) const {
   switch (iOption) {
   case UNREAL_SEX:
      m_iSex = iData;
      break;
   default:
      break;
   }
   return;
} //SetOption(int iOption, int iData;)
