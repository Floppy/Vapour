//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileDXF.cpp - 04/10/2000 - James Smith
//	AutoCAD DXF export filter implementation
//
// $Id: AvatarFileDXF.cpp,v 1.0 2000/10/04 16:27:07 waz Exp $
//

#include "stdafx.h"

#include "AvatarFileDXF.h"
#include "AvatarFileProxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////
// CAvatarFileDXF

CAvatarFileProxy<CAvatarFileDXF> g_oAvatarProxyDXF;

/////////////////////
// Member Functions

////////////////////
// Store Functions

CAvatarFileDXF::CAvatarFileDXF() {
   return;
} // CAvatarFileDXF()

float CAvatarFileDXF::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFileDXF::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFileDXF::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFileDXF::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFileDXF::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFileDXF::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFileDXF::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFileDXF::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFileDXF::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

FRESULT CAvatarFileDXF::Save(const char* pszFilename, CAvatar* pAvatar) const {

   if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;
   FRESULT eResult = F_OK;
      
   // Setup the export progress control
   g_poVAL->SetProgressMax("DXFSave",5);
   g_poVAL->StepProgress("DXFSave");
   g_poVAL->SetProgressText("DXFSave","Saving DXF File");

   ofstream osOutputStream(pszFilename,ios::out|ios::binary);
	if (osOutputStream.good()) {
		eResult = Save(osOutputStream, pAvatar);
	}
   
   g_poVAL->StepProgress("DXFSave");
	
   osOutputStream.close();

   return eResult;
} // Save(const char* pszFilename, CAvatar* pAvatar)

FRESULT CAvatarFileDXF::Save(ofstream& osOutputStream, CAvatar* pAvatar) const {

   if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;
   FRESULT eResult = F_OK;

   g_poVAL->StepProgress("DXFSave");
   g_poVAL->SetProgressText("DXFSave","Writing Header");

   // Write header
	osOutputStream << "0\n";
	osOutputStream << "SECTION\n";
	osOutputStream << "2\n";
	osOutputStream << "HEADER\n";
	osOutputStream << "9\n";
	osOutputStream << "$ACADVER\n";
	osOutputStream << "1\n";
	osOutputStream << "AC1006\n";
	osOutputStream << "0\n";
	osOutputStream << "ENDSEC\n";

   g_poVAL->StepProgress("DXFSave");
   g_poVAL->SetProgressText("DXFSave","Writing Blocks");

   // Write blocks
	osOutputStream << "0\n";
	osOutputStream << "SECTION\n";
	osOutputStream << "2\n";
	osOutputStream << "BLOCKS\n";
	osOutputStream << "0\n";
	osOutputStream << "ENDSEC\n";

   g_poVAL->StepProgress("DXFSave");
   g_poVAL->SetProgressText("DXFSave","Writing Entities");

   // Write entities
	osOutputStream << "0\n";
	osOutputStream << "SECTION\n";
	osOutputStream << "2\n";
	osOutputStream << "ENTITIES\n";
	const SPoint3D* pVertices = pAvatar->Vertices();
	const STriFace* pFaces = pAvatar->Faces();
	for (int f=0; f<pAvatar->NumFaces(); f++) {
		osOutputStream << "0\n";
		osOutputStream << "3DFACE\n";
		osOutputStream << "8\n";
		osOutputStream << "Avatar\n";
		osOutputStream << "10\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[0].m_iVertex].m_dComponents[0] << "\n";
		osOutputStream << "20\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[0].m_iVertex].m_dComponents[1] << "\n";
		osOutputStream << "30\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[0].m_iVertex].m_dComponents[2] << "\n";
		osOutputStream << "11\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[1].m_iVertex].m_dComponents[0] << "\n";
		osOutputStream << "21\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[1].m_iVertex].m_dComponents[1] << "\n";
		osOutputStream << "31\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[1].m_iVertex].m_dComponents[2] << "\n";
		osOutputStream << "12\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[2].m_iVertex].m_dComponents[0] << "\n";
		osOutputStream << "22\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[2].m_iVertex].m_dComponents[1] << "\n";
		osOutputStream << "32\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[2].m_iVertex].m_dComponents[2] << "\n";
		osOutputStream << "13\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[2].m_iVertex].m_dComponents[0] << "\n";
		osOutputStream << "23\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[2].m_iVertex].m_dComponents[1] << "\n";
		osOutputStream << "33\n";
		osOutputStream << pVertices[pFaces[f].m_sVertices[2].m_iVertex].m_dComponents[2] << "\n";
	}
	osOutputStream << "0\n";
	osOutputStream << "ENDSEC\n";

	// End file
	osOutputStream << "0\n";
	osOutputStream << "EOF\n";
   if (osOutputStream.bad()) eResult = F_FILE_ERROR;

	return eResult;
} // Save(ofstream& osOutputStream, CAvatar* pAvatar)
