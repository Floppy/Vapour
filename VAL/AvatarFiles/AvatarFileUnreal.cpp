//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileUnreal.cpp - 16/2/2000 - James Smith
//	Unreal export filter implementation
//
// $Id: AvatarFileUnreal.cpp,v 1.4 2000/07/21 17:00:10 waz Exp $
//

#include "stdafx.h"

#include "VAL.h"

#include "AvatarFileUnreal.h"
#include "AvatarFileProxy.h"

#include "Unreal.h"
#include <direct.h>
#include <errno.h>

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

int CAvatarFileUnreal::Save(const char* pszFilename, CAvatar* pAvatar) const {
   if (pAvatar == NULL) return 0;

   int iRetVal = 1;
   
   const int iNumTextures = pAvatar->NumTextures();

   // Setup the export progress
   g_poVAL->SetProgressMax("UTSave", 7 + iNumTextures);

   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Initialising");

   // Work out model name and target directory from passed filename
   char* pszInputName = strdup(pszFilename);
   char* pszTemp = pszInputName;
   // Find target directory
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   int iDirLength = (pszTemp - pszInputName);
   char* pszTgtDir = new char[iDirLength+1];
   for (int i=0; i<iDirLength; i++) {
      pszTgtDir[i] = pszInputName[i];
   }
   pszTgtDir[iDirLength] = 0;
   // Find model name
   char* pszEnd = strchr(pszTemp,'.');
   int iNameLength = (pszEnd != NULL) ? (pszEnd - pszTemp) : strlen(pszTemp);
   char* pszName = new char[iNameLength+1];
   for (i=0; i<iNameLength; i++) {
      pszName[i] = pszTemp[i];
   }
   pszName[iNameLength] = 0;
   iNameLength = strlen(pszName);
   // Tidy up
   free(pszInputName);

   // Create package name
   char* pszTexPackageName = new char[iNameLength + 4];
   strcpy(pszTexPackageName,pszName);
   strcpy(pszTexPackageName+iNameLength,"Tex");

   char pszTempFilename[128]; // Filename storage
   strcpy(pszTempFilename,pszTgtDir);

   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Creating directories");

   // Create output system directory
   strcpy(pszTempFilename+iDirLength,"System\\");
   int iResult = _mkdir(pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create System directory!\n");
      iRetVal = 0;
   }
   // Create output system directory
   strcpy(pszTempFilename+iDirLength,"Textures\\");
   iResult = _mkdir(pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create Textures directory!\n");
      iRetVal = 0;
   }

   // Save geometry file
   // This is documented on the nervedamage.com unreal page
   sprintf(pszTempFilename+iDirLength,"%s_d.3d",pszName);
   ofstream osGeometryStream(pszTempFilename,ios::binary|ios::out);
	if (osGeometryStream.good()) {
      // Write header
      short iNumFaces = pAvatar->NumFaces();
      short iNumVertices = pAvatar->NumVertices();
   	osGeometryStream.write((char*)&iNumFaces,2);
   	osGeometryStream.write((char*)&iNumVertices,2);
      // 32 zeroes
      for (int i=0; i<32; i++) osGeometryStream.write("\0",1);
      // Write empty space after header
      for (i=0; i<12; i++) osGeometryStream.write("\0",1);
      // Write geometry data for body triangles
      const STriFace* pFaces = pAvatar->Faces();
      for (i=0; i<iNumFaces; i++) {
         // Vertex indices
         unsigned short iVertex0 = pFaces[i].m_iVertices[0];
         unsigned short iVertex1 = pFaces[i].m_iVertices[1];
         unsigned short iVertex2 = pFaces[i].m_iVertices[2];
      	osGeometryStream.write((char*)&iVertex0,2);
      	osGeometryStream.write((char*)&iVertex1,2);
      	osGeometryStream.write((char*)&iVertex2,2);
         // Triangle type
         osGeometryStream.write("\0",1);
         // Colour
         char cColour = 0x7F;
         osGeometryStream.write(&cColour,1);
         // Texture coordinates
         char iTextureNumber = (pFaces[i].m_iTextureNumber==pAvatar->TextureIndex(skullbase)) ? 0 : 1;
         CImage* pTexture = pAvatar->Texture(iTextureNumber);
         int iWidth, iHeight;
         pTexture->GetSize(iWidth,iHeight);
         for (int tc=0; tc<3; tc++) {
            char cU = pFaces[i].m_sTexCoords[tc].dU*iWidth;
            char cV = pFaces[i].m_sTexCoords[tc].dV*iHeight;
            osGeometryStream.write(&cU,1);
            osGeometryStream.write(&cV,1);
         }
         // Texture number
      	osGeometryStream.write(&iTextureNumber,1);
         // Flags
         osGeometryStream.write("\0",1);
      }
      // Write geometry data for weapon triangle

      osGeometryStream.close();
	}
   else {
      iRetVal = 0;
   }

   // Save frame file
   // This is documented on the nervedamage.com unreal page
   sprintf(pszTempFilename+iDirLength,"%s_a.3d",pszName);
   ofstream osFrameStream(pszTempFilename,ios::binary|ios::out);
	if (osFrameStream.good()) {
      // Setup sizes & offsets etc
      SPoint3D pntMin, pntMax;
      pAvatar->BoundingBox(unknown,pntMax,pntMin);
      double dHeight = (pntMax.m_dComponents[1] - pntMin.m_dComponents[1]);
      double dScale = 127.5 / dHeight;
      double dYOff = 0;//(pntMax.m_dComponents[1] + pntMin.m_dComponents[1]) / 2 * dScale;
      // Get vertex info
      int iNumVertices = pAvatar->NumVertices();
      const SPoint3D* pVertices = pAvatar->Vertices();
      // Write header
      short iNumFrames = 1; // for the moment, just doing one
      osFrameStream.write((char*)&iNumFrames,2);
      short iFrameLength = iNumVertices * 4;
      osFrameStream.write((char*)&iFrameLength,2);
      // Write frames
      for (int i=0; i<iNumFrames; i++) {
         // This needs to be done for each frame that is output.
         /*CAvatarPose oPose;
         oPose.Load(filename);
         pAvatar->ImportPose(oPose);
         pAvatar->UpdateModel();*/
         // but instead, we do the default
         pAvatar->ResetPose();
         pAvatar->UpdateModel();
         // Pose
         // Write model
         for (int i=0; i<iNumVertices; i++) {
            // Pack vertex into a long int 
            long int lVertex = ( long int( pVertices[i].m_dComponents[2] * dScale *  8.0 ) & 0x7ff );
            lVertex |= ( ( long int( pVertices[i].m_dComponents[0] * dScale * 8.0 ) & 0x7ff ) << 11 );
            lVertex |= ( ( long int( ((pVertices[i].m_dComponents[1] * dScale) - dYOff) * 4.0 ) & 0x3ff ) << 22 );
            osFrameStream.write((char*)&lVertex,4);
         }
         // Write weapon triangle vertices
      }

      osFrameStream.close();
	}
   else {
      iRetVal = 0;
   }

   ofstream osOutputStream; // Create one output stream that is used over and over

   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving model package");

   // Save mesh U file
   // As you can see, this writes a header and not much else.
   sprintf(pszTempFilename+iDirLength,"System\\%s.u",pszName);
   osOutputStream.open(pszTempFilename,ios::binary|ios::out);
	if (osOutputStream.good()) {
      int iOffset = 0;
      // Save avatar pose
      CAvatarPose poOldPose = pAvatar->ExportPose();
      pAvatar->ResetPose();
      pAvatar->UpdateModel();

      // Create file header
      SUnrealPackageHeader sHeader;
      // Write info
      sHeader.iTag = UNREAL_PACKAGE_TAG;
      sHeader.iFileVersion = UNREAL_PACKAGE_VERSION;
      // Set flags
      sHeader.iPackageFlags = UNREAL_PKG_ALLOWDOWNLOAD;
      // Name tables stuff
      sHeader.iNameCount = 0;
      sHeader.iNameOffset = 0; // Address !!!!! FIX THIS !!!!!
      // 0 exported symbols
      sHeader.iExportCount = 0;
      sHeader.iExportOffset = 0; // Address !!!!! FIX THIS !!!!!
      // 3 imported symbols
      sHeader.iImportCount = 0;
      sHeader.iImportOffset = 0; // Address !!!!! FIX THIS !!!!!
      // Create GUID
      iOffset = 0;
      strcpy(sHeader.pcGUID, UNREAL_VAPOUR_GUID_TEX);
      iOffset += strlen(UNREAL_VAPOUR_GUID_TEX);
      sHeader.pcGUID[iOffset++] = pszName[0];
      sHeader.pcGUID[iOffset++] = pszName[1];
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      // Set num of GUIDs
      sHeader.iGUIDCount = 1;
      // Repeated numbers?
      sHeader.iExportCountAgain = sHeader.iExportCount;
      sHeader.iNameCountAgain = sHeader.iNameCount;
      // Set offset
      iOffset = sizeof(SUnrealPackageHeader);

      osOutputStream.write((char*)&sHeader,sizeof(SUnrealPackageHeader));

      // Restore old pose
      pAvatar->ImportPose(poOldPose);
      // Done - close stream
      osOutputStream.close();
   }
   else {
      iRetVal = 0;
   }

   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving model description");

   // Save mesh INT file
   // This is good, it declares the two models which will be required.
   // You can ignore this.
   sprintf(pszTempFilename+iDirLength,"System\\%s.int",pszName);
   osOutputStream.open(pszTempFilename,ios::binary|ios::out);
	if (osOutputStream.good()) {      
      // Write public declaration
      osOutputStream << "[Public]\n";
      // Write player declaration
      osOutputStream << "Object=(Name=";
      osOutputStream << pszName << "." << pszName;
      osOutputStream << ",Class=Class,MetaClass=Botpack.Bot,Description=\"";
      osOutputStream << pszName << "\")\n";
      // Write bot declaration
      osOutputStream << "Object=(Name=";
      osOutputStream << pszName << "." << pszName;
      osOutputStream << ",Class=Class,MetaClass=Botpack.TournamentPlayer,Description=\"";
      osOutputStream << pszName << "\")\n";
      // Done - close stream
      osOutputStream.close();
   }
   else {
      iRetVal = 0;
   }

   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving texture package");

   // Save texture UTX file
   sprintf(pszTempFilename+iDirLength,"Textures\\%s.utx",pszTexPackageName);
   osOutputStream.open(pszTempFilename,ios::binary|ios::out);
	if (osOutputStream.good()) {
      int t; // loop counter
      int iOffset; // file position counter
      // Resize and convert textures
      // All the work went in here and then became useless.      
      std::vector<CImage*> vTextures;
      for (t=0; t<iNumTextures; t++) {
         // Notify of progress
         char pszDisplay[32];
         sprintf(pszDisplay,"Converting texture %d", t);
         g_poVAL->StepProgress("UTSave");
         g_poVAL->SetProgressText("UTSave", pszDisplay);
         // Downsize images
         CImage* pTexture;
         pTexture = new CImage(*(pAvatar->Texture(t)));
         int iWidth = 0;
         int  iHeight = 0;
         pTexture->GetSize(iWidth,iHeight);
         // There should be only two textures - one face, one body, both 256x256
         // See sims code for this.
         if (t == pAvatar->TextureIndex(skullbase)) {
            // Set size of face texture to 256x256
            iWidth  = 256;
            iHeight = 256;
         }
         else if (t == pAvatar->TextureIndex(vl5)) {
            // Set size of torso texture to 128x128
            iWidth  = 128;
            iHeight = 128;
         }
         else {
            // Set size of other textures to 64x64
            iWidth = 64;
            iHeight = 64;
         }
         // Scale
         pTexture->Scale(iWidth,iHeight);
         // Flip
         pTexture->Flip();
         // Reduce colours
         //pTexture->Convert(IT_PALETTE,256);
         // Store
         vTextures.push_back(pTexture);
      }
      
      // Create file header
      SUnrealPackageHeader sHeader;
      // Write info
      sHeader.iTag = UNREAL_PACKAGE_TAG;
      sHeader.iFileVersion = UNREAL_PACKAGE_VERSION;
      // Set flags
      sHeader.iPackageFlags = UNREAL_PKG_ALLOWDOWNLOAD;
      // Name tables stuff
      sHeader.iNameCount = 0 + 2*iNumTextures;
      sHeader.iNameOffset = 0; // Address !!!!! FIX THIS !!!!!
      // 0 exported symbols
      sHeader.iExportCount = 0;
      sHeader.iExportOffset = 0; // Address !!!!! FIX THIS !!!!!
      // 3 imported symbols
      sHeader.iImportCount = 3;
      sHeader.iImportOffset = 0; // Address !!!!! FIX THIS !!!!!
      // Create GUID
      // The GUID is our little starter bit, plus a couple of chars of tha name, plus 5 random nums.
      iOffset = 0;
      strcpy(sHeader.pcGUID, UNREAL_VAPOUR_GUID_TEX);
      iOffset += strlen(UNREAL_VAPOUR_GUID_TEX);
      sHeader.pcGUID[iOffset++] = pszName[0];
      sHeader.pcGUID[iOffset++] = pszName[1];
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      sHeader.pcGUID[iOffset++] = (char)rand();
      // Set num of GUIDs
      sHeader.iGUIDCount = 1;
      // Repeated numbers?
      sHeader.iExportCountAgain = sHeader.iExportCount;
      sHeader.iNameCountAgain = sHeader.iNameCount;
      // Set offset
      iOffset = sizeof(SUnrealPackageHeader);

      // Write name list
      // Some stuff needs to be written into the name list before these. 
      // Look in a package, and you'll see what I mean.
      // I don't kno what it is any better than you.
      // Write palette names
      for (t=0; t<iNumTextures; t++) {
         // Write length
         osOutputStream.write("\x05",1);
         // Write name
         osOutputStream.write("Pal",7);
         char cTexChar = t + 'A';
         osOutputStream.write(&cTexChar,1);
         osOutputStream.write("\0",1);
         // Write flags (don't know meaning of the first 10h)
         osOutputStream.write("\x10\x00\x07\x00",4);
      }
      // Write package name
      // Write tex data names
      for (t=0; t<iNumTextures; t++) {
         // Write length
         osOutputStream.write("\x05",1);
         // Write name
         osOutputStream.write("Tex",7);
         char cTexChar = t + 'A';
         osOutputStream.write(&cTexChar,1);
         osOutputStream.write("\0",1);
         // Write flags (don't know meaning of the first 10h)
         osOutputStream.write("\x10\x00\x07\x00",4);
      }

      // Store palettes for later use
      // Was about to implement ForcoToPalett or MipMapping, but ran out of time
      CImagePalette* pPalettes = new CImagePalette[iNumTextures];
      for (t=0; t<iNumTextures; t++) {
         //pPalettes[i].Copy(vTextures[t]->GetPalette());
      }

      // Write palette data
      for (t=0; t<iNumTextures; t++) {
         // Write palette header (don't know what this means)
         osOutputStream.write("\x02\x40\x04",3);
         // Write palette
         // Currently just writes crap, but actual palette output should go here.
         for (int iEntry=0; iEntry<256; iEntry++) {
            unsigned long uColour = iEntry;
            //pPalettes[i].GetEntry(iEntry,uColour);
            osOutputStream.write((char*)&uColour,3);
            osOutputStream.write("\xFF",1);
         }
      }

      // Write texture data
      for (t=0; t<iNumTextures; t++) {
         CImage* pTexture = vTextures[t];
         int iWidth, iHeight;
         pTexture->GetSize(iWidth,iHeight);
         // Measure no. of bits in width - i.e. num o times to mipmap
         unsigned char iCount = 0;
         while (iWidth > 0) {
            iWidth = iWidth >> 1;
            iCount++;
         }
         // Reset width
         pTexture->GetSize(iWidth,iHeight);
         // Until we reach minimum texture size (0), output image halving size each time
         while (iWidth > 0) {
            // Write data header
            // This should have some value, didn't have time to work it out.
            // Shouldn't be zeroes though.
            // There is also some extra header beore each image set
            // Last few bytes are 'amount of data' stored as a 'compressed value'.
            // I have no idea why, but it's explained in the package document on epicgames.
            // Theres code for it, too.
            osOutputStream.write("\0\0\0\0\0\0",6);
            // Write data
            unsigned long uSize = iWidth * iHeight;
            for (unsigned long uPixel=0; uPixel<uSize; uPixel++) {
               unsigned char uPixelValue = pTexture->GetPixel(uPixel);
               osOutputStream.write((char*)&uPixelValue,1);
            }
            // Write footer
            // Footer contains with, height, followed by size of next image.
            osOutputStream.write((char*)&iWidth,4);
            osOutputStream.write((char*)&iHeight,4);
            iCount--;
            osOutputStream.write(&iCount,1);
            osOutputStream.write(&iCount,1);
            // Halve texture size
            iWidth = iWidth >> 1;
            iHeight = iHeight >> 1;
            pTexture->Scale(iWidth,iHeight);
         }
      }
      // Delete textures
      for (t=0; t<iNumTextures; t++) {
         CImage* pTexture = vTextures.back();
         vTextures.pop_back();
         delete pTexture;
      }

      // Write import table
      // This is actually right! Don't know what it means, but it will work.
      SUnrealImportTableEntry* pImportTable = new SUnrealImportTableEntry[sHeader.iImportCount];
      pImportTable[0].cClassPackage = 0x0C;
      pImportTable[0].cClassName =    0x10;
      pImportTable[0].iPackageIndex = 0;
      pImportTable[0].cObjectName =   0x0F;
      pImportTable[1].cClassPackage = 0x0C;
      pImportTable[1].cClassName =    0x0D;
      pImportTable[1].iPackageIndex = -1;
      pImportTable[1].cObjectName =   0x0E;
      pImportTable[2].cClassPackage = 0x0C;
      pImportTable[2].cClassName =    0x0D;
      pImportTable[2].iPackageIndex = -1;
      pImportTable[2].cObjectName =   0x09;

      // Write export table
      // No idea as yet, spent a long time staring at it.

      // Write chunks to stream
      // This should all be done in memory and then written (HL style). Haven't had time to change the texture 
      // output to work like this yet, it goes straight to stream.
      // Write header
      osOutputStream.write((char*)&sHeader,sizeof(SUnrealPackageHeader));
      // Write name list
      // Write palette data
      // Write texture data
      // Write import table
      osOutputStream.write((char*)pImportTable,sizeof(SUnrealImportTableEntry)*sHeader.iImportCount);
      // Write export table

      // Delete data chunks
      delete [] pImportTable;

      // Done - close stream
      osOutputStream.close();
   }
   else {
      iRetVal = 0;
   }
   
   g_poVAL->StepProgress("UTSave");
   g_poVAL->SetProgressText("UTSave", "Saving texture description");

   // Save texture INT file
   sprintf(pszTempFilename+iDirLength,"System\\%s.int",pszTexPackageName);
   osOutputStream.open(pszTempFilename,ios::out);
	if (osOutputStream.good()) {
      // Write public declaration
      osOutputStream << "[public]\n";
      // Write a line for each texture
      for (int t=0; t<iNumTextures; t++) {
         osOutputStream << "Object=(Name=" << pszTexPackageName << ".Texture" << char(t+'A') << ",Class=Texture";
         if (t==0) osOutputStream << ",Description=\"" << pszName << "\"";
         osOutputStream << ")\n";
      }
      // Done - close stream
      osOutputStream.close();
   }
   else {
      iRetVal = 0;
   }

   // Finish up
   delete [] pszName;
   delete [] pszTgtDir;
   delete [] pszTexPackageName;
   g_poVAL->StepProgress("UTSave");
   return iRetVal;
} // Save(const char* pszFilename, CAvatar* pAvatar)
