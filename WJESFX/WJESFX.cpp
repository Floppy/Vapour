//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// WJESFX.cpp - 11/07/2000 - Warren Moore
//	Application and installation
//
// $Id: WJESFX.cpp,v 1.7 2000/11/29 15:54:25 warren Exp $
//

#include "stdafx.h"
#include "WJESFX.h"

#include "WJESFXDlg.h"
#include "Registry.h"
#include "FolderDialog.h"
#include <direct.h>
#include <errno.h>

#include "VALWin32.h"
#include "SFX.h"
#include "Wedgie.h"
#include "ProgressDlg.h"

//#===--- VALWin32 management object
CVAL *g_poVAL = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////
// CWJESFXApp

BEGIN_MESSAGE_MAP(CWJESFXApp, CWinApp)
	//{{AFX_MSG_MAP(CWJESFXApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CWJESFXApp::CWJESFXApp() {
} // Constructor

CWJESFXApp theApp;

BOOL CWJESFXApp::InitInstance() {

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Create the VALWin32 management object
	NEWBEGIN
	g_poVAL = (CVAL*) new CVALWin32;
	NEWEND("CWJESFXApp::InitInstance - CVALWin32 object")
	
	CWJESFXDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	if (g_poVAL) {
		delete g_poVAL;
		g_poVAL = NULL;
	}

	return FALSE;
} // InitInstance

void CWJESFXApp::InstallSims(CProgressDlg *poDlg) {
// Find the game directory
	CRegistry oReg;
	char pcData[STR_SIZE] = "";
	oReg.ReadString(LOCAL_MACHINE, "Software\\Maxis\\The Sims", "SIMS_DATA", "", pcData);
	CString strPath = pcData;
	if (strPath == "") {
		AfxMessageBox("Unable to find The Sims directory. Please select The Sims directory.", MB_OK);
		CFolderDialog oFolder;
		int iResult = oFolder.DoModal();
		if (iResult == IDOK) {
			strPath = oFolder.GetPathName();
		}
		else
			return;
	}
	// Check for a trailing slash
	if (strPath.Right(1) != "\\") {
		strPath += "\\";
	}
	// Set the game data directory
	strPath += "GameData\\";
	
	CSFX oSFX;
	// Has the end position been set?
	if (!oSFX.EndSet()) {
		// If not, set the end position then exit
		bool bSet = oSFX.SetEnd();
		AfxMessageBox((bSet ? "End position set correctly" : "Unable to set end position"), MB_OK);
	}
	else {
		// End position set to retreive data
		ifstream *poWJE = oSFX.GetWedgie();
		// Check the file stream is valid
		if (!poWJE) {
			AfxMessageBox("Unable to retrieve file handle", MB_OK|MB_ICONERROR);
			return;
		}
		// Create the decompressor and check the data has been supplied
		CWedgie oWedgie;
		WJERESULT eResult = oWedgie.Open((fstream*)poWJE, strPath);
		// Check the wedgie opened correctly
		if (eResult != WJE_OK) {
			if (poDlg)
				poDlg->DestroyWindow();
			AfxMessageBox("Missing model data", MB_OK|MB_ICONERROR);
			delete poWJE;
			return;
		}
		// Set up the dialog
		if (poDlg) {
			poDlg->ShowWindow(SW_SHOWNORMAL);
			poDlg->SetMax((int)oWedgie.Files());
			poDlg->UpdateWindow();
		}
		// Decompress the files
		unsigned int uCount = 0;
		while (uCount < oWedgie.Files()) {
			eResult = oWedgie.Extract(uCount);
			if (eResult != WJE_OK) {
				if (poDlg)
					poDlg->DestroyWindow();
				AfxMessageBox("Error decompressing data", MB_OK|MB_ICONERROR);
				oWedgie.Close();
				delete poWJE;
				return;
			}
			if (poDlg)
				poDlg->Step();
			uCount++;
		}
		// Clean up
		oWedgie.Close();
		poWJE->close();
		delete poWJE;
		// Success!
		AfxMessageBox("Avatar installed successfully", MB_OK);
	}
	if (poDlg)
		poDlg->DestroyWindow();
} // InstallSims

void CWJESFXApp::InstallHL(CProgressDlg *poDlg) {
// Find the game directory
	CRegistry oReg;
	char pcData[STR_SIZE] = "";
	oReg.ReadString(LOCAL_MACHINE, "Software\\Valve\\Half-Life", "InstallPath", "", pcData);
	CString strPath = pcData;
	if (strPath == "") {
		AfxMessageBox("Unable to find the Half Life directory. Please select your Half Life directory.", MB_OK);
		CFolderDialog oFolder;
		int iResult = oFolder.DoModal();
		if (iResult == IDOK) {
			strPath = oFolder.GetPathName();
		}
		else
			return;
	}
	// Check for a trailing slash
	if (strPath.Right(1) != "\\") {
		strPath += "\\";
	}
	// Set the player directory
	strPath += "valve\\models\\player\\";

	CSFX oSFX;
	// Has the end position been set?
	if (!oSFX.EndSet()) {
		// If not, set the end position then exit
		bool bSet = oSFX.SetEnd();
		AfxMessageBox((bSet ? "End position set correctly" : "Unable to set end position"), MB_OK);
	}
	else {
		// End position set to retreive data
		ifstream *poWJE = oSFX.GetWedgie();
		// Check the file stream is valid
		if (!poWJE) {
			AfxMessageBox("Unable to retrieve file handle", MB_OK|MB_ICONERROR);
			return;
		}
		// Create the decompressor and check the data has been supplied
		CWedgie oWedgie;
		WJERESULT eResult = oWedgie.Open((fstream*)poWJE, strPath);
		// Check the wedgie opened correctly
		if (eResult != WJE_OK) {
			if (poDlg)
				poDlg->DestroyWindow();
			AfxMessageBox("Missing model data", MB_OK|MB_ICONERROR);
			delete poWJE;
			return;
		}
		// Set up the dialog
		if (poDlg) {
			poDlg->ShowWindow(SW_SHOWNORMAL);
			poDlg->SetMax((int)oWedgie.Files());
			poDlg->UpdateWindow();
		}
		// Decompress the files
		unsigned int uCount = 0;
		while (uCount < oWedgie.Files()) {
			eResult = oWedgie.Extract(uCount);
			if (eResult != WJE_OK) {
				if (poDlg)
					poDlg->DestroyWindow();
				AfxMessageBox("Error decompressing data", MB_OK|MB_ICONERROR);
				oWedgie.Close();
				delete poWJE;
				return;
			}
			if (poDlg)
				poDlg->Step();
			uCount++;
		}
		// Clean up
		oWedgie.Close();
		poWJE->close();
		delete poWJE;
		// Success!
		AfxMessageBox("Avatar installed successfully", MB_OK);
	}
	if (poDlg)
		poDlg->DestroyWindow();
} // InstallHL

void CWJESFXApp::InstallUT(CProgressDlg *poDlg) {
// Find the game directory
	CRegistry oReg;
	char pcData[STR_SIZE] = "";
	oReg.ReadString(LOCAL_MACHINE, "Software\\Unreal Technology\\Installed Apps\\UnrealTournament", 
						 "Folder", "", pcData);
	CString strPath = pcData;
	if (strPath == "") {
		AfxMessageBox("Unable to find the Unreal Tournament directory. Please select your directory.", MB_OK);
		CFolderDialog oFolder;
		int iResult = oFolder.DoModal();
		if (iResult == IDOK) {
			strPath = oFolder.GetPathName();
		}
		else
			return;
	}
	// Check for a trailing slash
	if (strPath.Right(1) != "\\") {
		strPath += "\\";
	}

	CSFX oSFX;
	// Has the end position been set?
	if (!oSFX.EndSet()) {
		// If not, set the end position then exit
		bool bSet = oSFX.SetEnd();
		AfxMessageBox((bSet ? "End position set correctly" : "Unable to set end position"), MB_OK);
	}
	else {
		// End position set to retreive data
		ifstream *poWJE = oSFX.GetWedgie();
		// Check the file stream is valid
		if (!poWJE) {
			AfxMessageBox("Unable to retrieve file handle", MB_OK|MB_ICONERROR);
			return;
		}
		// Create the decompressor and check the data has been supplied
		CWedgie oWedgie;
		WJERESULT eResult = oWedgie.Open((fstream*)poWJE, strPath);
		// Check the wedgie opened correctly
		if (eResult != WJE_OK) {
			if (poDlg)
				poDlg->DestroyWindow();
			AfxMessageBox("Missing model data", MB_OK|MB_ICONERROR);
			delete poWJE;
			return;
		}
		// Set up the dialog
		if (poDlg) {
			poDlg->ShowWindow(SW_SHOWNORMAL);
			poDlg->SetMax((int)oWedgie.Files());
			poDlg->UpdateWindow();
		}
		// Decompress the files
		unsigned int uCount = 0;
		while (uCount < oWedgie.Files()) {
			eResult = oWedgie.Extract(uCount);
			if (eResult != WJE_OK) {
				if (poDlg)
					poDlg->DestroyWindow();
				AfxMessageBox("Error decompressing data", MB_OK|MB_ICONERROR);
				oWedgie.Close();
				delete poWJE;
				return;
			}
			if (poDlg)
				poDlg->Step();
			uCount++;
		}
		// Add the successfully installed files to UT system\manifest.ini
		bool bExist = false;
		if (!AddFilesToUT(oWedgie, bExist)) 
			AfxMessageBox("Error updating Unreal Tournament installed modules file", MB_OK|MB_ICONERROR);
		else 
			if (!bExist)
				AfxMessageBox("Unreal Tournament file System\\Manifest.ini renamed to System\\Manifest.ini.bak", MB_OK);
		// Clean up
		oWedgie.Close();
		poWJE->close();
		delete poWJE;
		// Success!
		AfxMessageBox("Avatar installed successfully", MB_OK);
	}
	if (poDlg)
		poDlg->DestroyWindow();
} // InstallHL

bool CWJESFXApp::AddFilesToUT(CWedgie &oWJE, bool &bExist) {
	// Overwrite previous value
	bExist = false;

	// Open the ini file
	CString strININame;
	strININame = oWJE.Directory();
	strININame += "system\\manifest.ini";
	ifstream oInFile;
	oInFile.open(strININame, ios::in|ios::nocreate);
	if (oInFile.fail()) {
		oInFile.close();
		return false;
	}

	// Create a temporary ini file
	CString strTempName(strININame);
	strTempName += ".vap";
	ofstream oOutFile;
	oOutFile.open(strTempName, ios::out|ios::trunc);
	if (oOutFile.fail()) {
		oInFile.close();
		oOutFile.close();
		return false;
	}

	// Generate the module name from the executable name
//	const char *pcName = oWJE.Filename(0);
	const char *pcName = g_poVAL->GetAppName();

	if (!pcName) {
		oInFile.close();
		oOutFile.close();
		return false;
	}
	while(strchr(pcName, '\\') != NULL) 
		pcName = strchr(pcName, '\\') + 1;
	int iLength = strlen(pcName);
	bool bFound = false;
	while (!bFound && (iLength > 0))
		if (pcName[iLength] == '.')
			bFound = true;
		else
			iLength--;
	char pcModName[STR_SIZE] = "";
	memcpy(pcModName, pcName, iLength);
	pcModName[iLength] = '\0';

	// Calculate the group name
	char pcGroupName[STR_SIZE] = "Group=";
	strcat(pcGroupName, pcModName);
	// Read through the file
	char pcSection[STR_SIZE] = "";
	char pcLine[STR_SIZE] = "";
	bool bWritten = false;
	while (!oInFile.eof()) {
		// Read a line
		oInFile.getline(pcLine, STR_SIZE);
		// Check if the group has already been written
		if (stricmp(pcGroupName, pcLine) == 0) {
			bExist = true;
		}
		// Check if we need to add our lines
		if ((!bExist) && (!bWritten) && (strlen(pcSection) != 0) && 
			 ((strlen(pcLine) == 0) || (pcLine[0] == '['))) {
			// Check for the setup section
			if (stricmp("Setup", pcSection) == 0)
				oOutFile << "Group=" << pcModName << endl;
			// Check for the ref count section
			if (stricmp("RefCounts", pcSection) == 0) {
				unsigned int uCount = 0;
				while (uCount < oWJE.Files()) {
					oOutFile << "File:" << oWJE.Filename(uCount) << "=1" << endl;
					uCount++;
				}
			}
			bWritten = true;
		}
		// Get the section name, if so
		if (pcLine[0] == '[') {
			strcpy(pcSection, pcLine + 1);
			pcSection[strlen(pcSection) - 1] = '\0';
			bWritten = false;
		}
		// Write out what we read in
		oOutFile.write(pcLine, strlen(pcLine));
		oOutFile << endl;
	}

	// Write the module settings
	if (!bExist) {
		oOutFile << "[" << pcModName << "]" << endl;
		unsigned int uCount = 0;
		while (uCount < oWJE.Files()) {
			oOutFile << "File=" << oWJE.Filename(uCount) << endl;
			uCount++;
		}
		oOutFile << "Caption=" << pcModName << endl;
		oOutFile << "Version=100" << endl << endl;
	}
	// Check everything is Ok
	bool bOk = (!oInFile.bad()) && (!oOutFile.bad());
	// Close the files
	oInFile.close();
	oOutFile.close();
	// If all ok move the files
	if (bOk) {
		// Delete the back up file
		CString strBAKName(strININame);
		strBAKName += ".bak";
		if ((remove(strBAKName) == -1) && (errno != ENOENT))
			return false;
		// Rename the ini file to ini.bak
		if (rename(strININame, strBAKName) == -1)
			return false;
		// Rename the ini.vap to the ini file
		if (rename(strTempName, strININame) == -1)
			return false;
	}
	
	return true;
} // AddFilesToUT
