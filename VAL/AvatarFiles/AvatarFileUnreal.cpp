//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileUnreal.cpp - 16/2/2000 - James Smith
//	Unreal export filter implementation
//
// $Id: AvatarFileUnreal.cpp,v 1.2 2000/07/16 12:28:05 waz Exp $
//

#include "stdafx.h"

#include "VAL.h"

#include "AvatarFileUnreal.h"
#include "AvatarFileProxy.h"

#include "ProgressControl.h"
extern CProgressControl g_oProgressControl;

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
	int iRetVal = 1;
   // Setup the export progress
   g_oProgressControl.SetMaxProgress("UTSave", 2);
   g_oProgressControl.Step("UTSave");
   g_oProgressControl.SetText("UTSave", "Saving Geometry file");
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
   // Save geometry file   
   char* pszGeometryFilename = new char[iBaseNameLength+6];
   memset(pszGeometryFilename,0,iBaseNameLength+6);
   sprintf(pszGeometryFilename,"%s_d.3d",pszBasename);
   ofstream osGeometryStream(pszGeometryFilename,ios::binary|ios::out);
	if (osGeometryStream.good()) {
      // Write header
      short iNumFaces = pAvatar->NumFaces();
      short iNumVertices = pAvatar->NumVertices();
   	osGeometryStream.write((char*)&iNumFaces,2);
   	osGeometryStream.write((char*)&iNumVertices,2);
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
         char iTextureNumber = pFaces[i].m_iTextureNumber;
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
   delete [] pszGeometryFilename;
   // Save frame file
   char* pszFrameFilename = new char[iBaseNameLength+6];
   memset(pszFrameFilename,0,iBaseNameLength+6);
   sprintf(pszFrameFilename,"%s_a.3d",pszBasename);
   ofstream osFrameStream(pszFrameFilename,ios::binary|ios::out);
   CAvatarPose poOldPose = pAvatar->ExportPose();
	if (osFrameStream.good()) {
      // Setup sizes & offsets etc
      SPoint3D pntMin, pntMax;
      pAvatar->BoundingBox(unknown,pntMax,pntMin);
      double dHeight = (pntMax.m_dComponents[1] - pntMin.m_dComponents[1]);
      double dScale = 255.0 / dHeight;
      double dYOff = (pntMax.m_dComponents[1] + pntMin.m_dComponents[1]) / 2 * dScale;
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
         // Pose
         pAvatar->ResetPose();
         pAvatar->UpdateModel();
         // Write model
         for (int i=0; i<iNumVertices; i++) {
            // Pack vertex into a long int 
            long int lVertex = ( long int( pVertices[i].m_dComponents[0] * dScale *  8.0 ) & 0x7ff );
            lVertex |= ( ( long int( pVertices[i].m_dComponents[2] * dScale * 8.0 ) & 0x7ff ) << 11 );
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
   delete [] pszFrameFilename;
   // Finish up
   pAvatar->ImportPose(poOldPose);
   delete [] pszBasename;
   g_oProgressControl.Step("UTSave");
   return iRetVal;
} // Save(const char* pszFilename, CAvatar* pAvatar)
