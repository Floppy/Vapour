//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFile3DS.cpp - 04/10/2000 - James Smith
//	3D Studio export filter implementation
//
// $Id: AvatarFile3DS.cpp,v 1.0 2000/10/04 16:27:06 waz Exp $
//

#include "stdafx.h"

#include "AvatarFile3DS.h"
#include "AvatarFileProxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// CAvatarFile3DS

CAvatarFileProxy<CAvatarFile3DS> g_oAvatarProxy3DS;

/////////////////////
// Member Functions

////////////////////
// Store Functions

CAvatarFile3DS::CAvatarFile3DS() {
} // CAvatarFile3DS()

float CAvatarFile3DS::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFile3DS::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFile3DS::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFile3DS::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFile3DS::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFile3DS::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFile3DS::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFile3DS::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFile3DS::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

FRESULT CAvatarFile3DS::Save(const char* pszFilename, CAvatar* pAvatar) const {

   if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;
   FRESULT eResult = F_OK;

   // Work out base filename and store it
   char* pszLocalFilename = strdup(pszFilename);
   char* pszTemp = pszLocalFilename;
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   int iLength = (strchr(pszTemp,'.') - pszTemp);
   char* pszBasename = new char[iLength+1];
   memset(pszBasename,0,iLength+1);
   for (int i=0; i<iLength; i++) pszBasename[i] = pszTemp[i];
   free(pszLocalFilename);
   int iBaseNameLength = strlen(pszBasename);
   // Setup the export progress dialog
   g_poVAL->SetProgressMax("3DSSave",5 + pAvatar->NumTextures());
   g_poVAL->StepProgress("3DSSave");
   g_poVAL->SetProgressText("3DSSave","Saving 3DS file");
   // Save 3DS to stream
   ofstream osOutputStream(pszFilename,ios::binary);
	if (osOutputStream.good()) {
      float* pfTempPtr = NULL;
      short* psTempPtr = NULL;

      // Create Vertex Chunk
      g_poVAL->StepProgress("3DSSave");
      g_poVAL->SetProgressText("3DSSave","Saving Vertices");
      const SPoint3D* pVertices = pAvatar->Vertices();
      int iNumVertices = pAvatar->NumFaces() * 3;
      const STriFace* pFaces = pAvatar->Faces();
      int iNumFaces = pAvatar->NumFaces();
      int iVertexChunkLength = (iNumVertices * 12) + 8;
      char* pcVertexChunk = new char[iVertexChunkLength];
      memset(pcVertexChunk,0,iVertexChunkLength);
      *((short*)pcVertexChunk) = 0x4110;
      *((int*)(pcVertexChunk+2)) = iVertexChunkLength;
      *((short*)(pcVertexChunk+6)) = iNumVertices;
      for (int i=0; i<iNumVertices; i++) {
         pfTempPtr = (float*)((pcVertexChunk+8) + (i*12));
         int iFace = i/3;
         const SPoint3D* pCurrentVertex = pVertices + pFaces[iFace].m_sVertices[i%3].m_iVertex;
         pfTempPtr[0] = pCurrentVertex->m_dComponents[0];
         pfTempPtr[1] = -pCurrentVertex->m_dComponents[2];
         pfTempPtr[2] = pCurrentVertex->m_dComponents[1];
      }

      // Create Face Material Chunk
      int iNumMaterials = pAvatar->NumTextures();
      int iFaceMaterialChunkLength = (iNumMaterials*(strlen(pszBasename) + 11)) + (iNumFaces*2);
      char* pcFaceMaterialChunk = new char[iFaceMaterialChunkLength];
      memset(pcFaceMaterialChunk,0,iFaceMaterialChunkLength);
      int iOffset = 0;
      for (i=0; i<iNumMaterials; i++) {
         *((short*)(pcFaceMaterialChunk+iOffset)) = 0x4130;
         int iStartOffset = iOffset;
         iOffset += 6;
         strcpy(pcFaceMaterialChunk+iOffset,pszBasename);
         iOffset += strlen(pszBasename);
         pcFaceMaterialChunk[iOffset++] = '_';
         pcFaceMaterialChunk[iOffset++] = i+0x61;
         iOffset++;
         int iNumFacesOffset = iOffset;
         iOffset += 2;
         int iFaceCount = 0;
         for (int j=0; j<iNumFaces; j++) {
            if (pFaces[j].m_iTextureNumber == i) {
               *((short*)(pcFaceMaterialChunk+iOffset)) = j;
               iOffset += 2;
               iFaceCount++;
            }
         }
         *((short*)(pcFaceMaterialChunk+iNumFacesOffset)) = iFaceCount;
         *((long*)(pcFaceMaterialChunk+iStartOffset+2)) = iOffset - iStartOffset;
      }
   
      // Create Smoothing Group Chunk
      int iSmoothChunkLength = (iNumFaces * 4) + 6;
      char* pcSmoothChunk = new char[iSmoothChunkLength];
      memset(pcSmoothChunk,0,iSmoothChunkLength);
      *((short*)pcSmoothChunk) = 0x4150;
      *((long*)(pcSmoothChunk+2)) = iSmoothChunkLength;
      for (i=0; i<iNumFaces; i++) {
         (pcSmoothChunk+6)[i*4] = 1;
      }   

      // Create Face Chunk
      g_poVAL->StepProgress("3DSSave");
      g_poVAL->SetProgressText("3DSSave","Saving Faces");
      int iFaceChunkLength = (iNumFaces * 8) + 8;
      char* pcFaceChunk = new char[iFaceChunkLength];
      memset(pcFaceChunk,0,iFaceChunkLength);
      *((short*)pcFaceChunk) = 0x4120;
      *((long*)(pcFaceChunk+2)) = iFaceChunkLength + iFaceMaterialChunkLength + iSmoothChunkLength;
      *((short*)(pcFaceChunk+6)) = iNumFaces;
      for (i=0; i<iNumFaces; i++) {
         psTempPtr = (short*)((pcFaceChunk+8) + (i*8));
         psTempPtr[0] = i*3;
         psTempPtr[1] = i*3 + 1;
         psTempPtr[2] = i*3 + 2;
         psTempPtr[3] = 0;
      }

      // Create Mapping Coordinates Chunk
      int iUVChunkLength = (iNumVertices * 8) + 8;
      char* pcUVChunk = new char[iUVChunkLength];
      memset(pcUVChunk,0,iUVChunkLength);
      *((short*)pcUVChunk) = 0x4140;
      *((long*)(pcUVChunk+2)) = iUVChunkLength;
      *((short*)(pcUVChunk+6)) = iNumVertices;
      for (i=0; i<iNumVertices; i++) {
         pfTempPtr = (float*)((pcUVChunk+8) + (i*8));
         int iFace = i/3;
         const STexCoord* pCurrentTexCoord = &(pFaces[iFace].m_sVertices[i%3].m_sTexCoord);
         pfTempPtr[0] = pCurrentTexCoord->dU;
         pfTempPtr[1] = 1-pCurrentTexCoord->dV;
      }

      // Create TriPolygon Chunk
      int iTriPolygonChunkLength = 6;
      char* pcTriPolygonChunk = new char[iTriPolygonChunkLength];
      memset(pcTriPolygonChunk,0,iTriPolygonChunkLength);
      *((short*)pcTriPolygonChunk) = 0x4100;
      *((long*)(pcTriPolygonChunk+2)) = iTriPolygonChunkLength + iVertexChunkLength + *((long*)(pcFaceChunk+2)) + *((long*)(pcUVChunk+2));

      // Create Object Chunk
      int iObjectChunkLength = 6 + strlen(pszBasename) + 1;
      char* pcObjectChunk = new char[iObjectChunkLength];
      memset(pcObjectChunk,0,iObjectChunkLength);
      *((short*)pcObjectChunk) = 0x4000;
      *((long*)(pcObjectChunk+2)) = iObjectChunkLength + *((long*)(pcTriPolygonChunk+2));
      strcpy(pcObjectChunk+6,pszBasename);

      // Create Materials Chunk
      int iSingleMaterialLength = 0xE6 + 2*(strlen(pszBasename)+2);
      int iMaterialChunkLength = iNumMaterials * iSingleMaterialLength;
      char* pcMaterialChunk = new char[iMaterialChunkLength];
      memset(pcMaterialChunk,0,iMaterialChunkLength);
      for (int m=0; m<iNumMaterials; m++) {
         int iOffset = 0;
         char* pcCurrentMaterial = pcMaterialChunk + m*iSingleMaterialLength;

         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xAFFF;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = iSingleMaterialLength;
         iOffset += 4;

         // Material name
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA000;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = strlen(pszBasename)+9;
         iOffset += 4;
         strcpy(pcCurrentMaterial+iOffset,pszBasename);
         iOffset += strlen(pszBasename);
         pcCurrentMaterial[iOffset++] = '_';
         pcCurrentMaterial[iOffset++] = m + 'a';
         iOffset++;

         // ambient colour
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA010;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x18;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0011;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x9;
         iOffset += 7;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0012;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x9;
         iOffset += 7;

         // diffuse colour
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA020;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x18;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0011;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x9;
         iOffset += 4;
         pcCurrentMaterial[iOffset++] = 0;
         pcCurrentMaterial[iOffset++] = 0;
         pcCurrentMaterial[iOffset++] = 0;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0012;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x9;
         iOffset += 4;
         pcCurrentMaterial[iOffset++] = 0;
         pcCurrentMaterial[iOffset++] = 0;
         pcCurrentMaterial[iOffset++] = 0;
         // specular colour
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA030;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x18;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0011;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x9;
         iOffset += 7;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0012;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x9;
         iOffset += 7;
         // shininess
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA040;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xE;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0030;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x8;
         iOffset += 6;
         // shininess strength
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA041;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xE;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0030;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x8;
         iOffset += 6;
         // transparency
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA050;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xE;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0030;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x8;
         iOffset += 6;
         // transparency falloff
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA052;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xE;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x30;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x8;
         iOffset += 6;
         // reflect blur
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA053;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xE;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0030;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x8;
         iOffset += 6;
         // material type
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA100;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x8;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x3; //Phong
         iOffset += 2;   
         // self illumination
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA084;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xE;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0030;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x8;
         iOffset += 6;
         // wire thickness
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA087;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xA;
         iOffset += 4;
         *((float*)(pcCurrentMaterial+iOffset)) = 0.07F;
         iOffset += 4;
         // and finally, the actual texture map!
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA200;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0x2D + strlen(pszBasename);
         iOffset += 4;
         // Amount of...
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x0030;
         iOffset += 2;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x8;
         iOffset += 4;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x64; // Amount of texture??
         iOffset += 2;
         // Filename
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA300;
         iOffset += 2;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)strlen(pszBasename) + 13;
         iOffset += 4;
         strcpy(pcCurrentMaterial+iOffset,pszBasename);
         iOffset += strlen(pszBasename);
         pcCurrentMaterial[iOffset++] = '_';
         pcCurrentMaterial[iOffset++] = m + 'a';
         strcpy(pcCurrentMaterial+iOffset,".jpg");
         iOffset+=5;
         // map options
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA351;
         iOffset += 2;
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0x8;
         iOffset += 6;
         // filtering blur
         *((short*)(pcCurrentMaterial+iOffset)) = (short)0xA353;
         iOffset += 2;
         *((long*)(pcCurrentMaterial+iOffset)) = 0xA;
         iOffset += 4;
         *((float*)(pcCurrentMaterial+iOffset)) = 1.0;
      }

      // Create Editor Chunk
      int iEditorChunkLength = 6;
      char* pcEditorChunk = new char[iEditorChunkLength];
      memset(pcEditorChunk,0,iEditorChunkLength);
      *((short*)pcEditorChunk) = 0x3D3D;
      *((long*)(pcEditorChunk+2)) = iEditorChunkLength + iMaterialChunkLength + *((int*)(pcObjectChunk+2));

      // Create Keyframe Chunk

      // Create Main Chunk
      int iMainChunkLength = 16;
      char* pcMainChunk = new char[iMainChunkLength];
      memset(pcMainChunk,0,iMainChunkLength);
      *((short*)pcMainChunk) = 0x4D4D;
      *((long*)(pcMainChunk+2)) = iMainChunkLength + *((long*)(pcEditorChunk+2));
      pcMainChunk[6] = 0x2;
      pcMainChunk[8] = 0xA;
      pcMainChunk[12] = 0x3;

      // Write to stream
      osOutputStream.write(pcMainChunk,iMainChunkLength);
      osOutputStream.write(pcEditorChunk,iEditorChunkLength);
      osOutputStream.write(pcMaterialChunk,iMaterialChunkLength);
      osOutputStream.write(pcObjectChunk,iObjectChunkLength);
      osOutputStream.write(pcTriPolygonChunk,iTriPolygonChunkLength);
      osOutputStream.write(pcVertexChunk,iVertexChunkLength);
      osOutputStream.write(pcFaceChunk,iFaceChunkLength);
      osOutputStream.write(pcFaceMaterialChunk,iFaceMaterialChunkLength);
      osOutputStream.write(pcSmoothChunk,iSmoothChunkLength);
      osOutputStream.write(pcUVChunk,iUVChunkLength);

      // Clean up memory
      g_poVAL->StepProgress("3DSSave");
      g_poVAL->SetProgressText("3DSSave","Cleaning up");
      if (pcMainChunk != NULL) delete [] pcMainChunk;
      if (pcEditorChunk != NULL) delete [] pcEditorChunk;
      if (pcMaterialChunk != NULL) delete [] pcMaterialChunk;
      if (pcObjectChunk != NULL) delete [] pcObjectChunk;
      if (pcTriPolygonChunk != NULL) delete [] pcTriPolygonChunk;
      if (pcUVChunk != NULL) delete [] pcUVChunk;
      if (pcFaceMaterialChunk != NULL) delete [] pcFaceMaterialChunk;
      if (pcSmoothChunk != NULL) delete [] pcSmoothChunk;
      if (pcFaceChunk != NULL) delete [] pcFaceChunk;
      if (pcVertexChunk != NULL) delete [] pcVertexChunk;
   
      if (osOutputStream.bad()) eResult = F_FILE_ERROR;
	}
	osOutputStream.close();
   // Save images
   CImageFile* pImageFile = g_oImageFileStore.CreateByExtension("jpg");
   for (int t=0; t<pAvatar->NumTextures(); t++) {
      g_poVAL->StepProgress("3DSSave");
      char pszDisplay[256];
      sprintf(pszDisplay,"Saving JPEG texture %d", t);
      g_poVAL->SetProgressText("3DSSave",pszDisplay);
      char* pszTextureFilename = new char[iBaseNameLength+7];
      strcpy(pszTextureFilename,pszBasename);
      pszTextureFilename[iBaseNameLength] = '_';
      pszTextureFilename[iBaseNameLength+1] = t+'a';
      strcpy(pszTextureFilename+iBaseNameLength+2,".jpg");
      pAvatar->Texture(t)->Save(pszTextureFilename,pImageFile);
      delete [] pszTextureFilename;
   }
   // Finish
   g_poVAL->StepProgress("3DSSave");
   delete [] pszBasename;
   pszBasename = NULL;
   return eResult;
} // Save(const char* pszFilename, CAvatar* pAvatar)