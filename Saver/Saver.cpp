//======---
// Saver
//------
// Vapour Technology Screen Saver
// Copyright 2000 Vapour Technology Ltd.
//
// Saver.cpp - 26/11/2000 - Warren Moore
//	  Main screen saver Windows source
//
// $Id: Saver.cpp,v 1.3 2000/12/02 07:38:38 warren Exp $
//

#include "Saver.h"

#include <stdlib.h>
#include <stdio.h>

#include "VALWin32.h"

#include "SaverSettings.h"
#include "SceneTestSaver.h"
#include "SFX.h"
#include <fstream.h>
#include "Wedgie.h"

//#===--- Avatar includes
#include "Avatar.h"
#include "AvatarFileStore.h"
extern CAvatarFileStore g_oAvatarFileStore;

//#===--- Globals

// The VAL management object
CVAL *g_poVAL = NULL;
// The screen saver settings object
CSaverSettings g_oSettings;
// The screen saver display manager
SAVER_SCENE *g_poScene = NULL;
// The selected avatar
CAvatar *g_poAvatar = NULL;

//#===--- Functions

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	// Store the application instance
	g_oSettings.SetInstance(hInstance);
	// Parse command line arguments, retrieving HWND if set
	g_oSettings.ParseCommandLine(GetCommandLine());
	// Create the VAL object
	g_poVAL = (CVALWin32*) new CVALWin32;
	if (!g_poVAL)
		return -1;
	// Read in the settings
	g_oSettings.GetSettings();
	// Run the screensaver in the correct mode
	switch (g_oSettings.GetMode()) {
		case SA_PASSWORD:
			ChangePassword();
			break;
		case SA_CONFIG:
			DialogBox(g_oSettings.GetInstance(),
						 MAKEINTRESOURCE(DLG_CONFIG),
						 g_oSettings.GetHWND(),
						 ConfigDialogProc);
			break;
		case SA_SAVER:
		case SA_PREVIEW:
			StartSaver();
			break;
		default:
			break;
	}
	// Store the settings
	g_oSettings.StoreSettings();
	// Delet the VAL object
	delete g_poVAL;

	return 0;
} // WinMain

void StartSaver() {
	// Set the screen saver window properties
	// NOTE: Must be called before CreateScene, as HWND required to create render context
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = SaverWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_oSettings.GetInstance();
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "SaverClass";
	// Register the screen saver class
	RegisterClass(&wc);
	// Create the window
	HWND hNewWND = NULL;
	HWND hParentWND = g_oSettings.GetHWND();
	if (g_oSettings.GetMode() == SA_PREVIEW) {
		// Check we have a parent window
		if (hParentWND == NULL)
			return;
		// Create the preview window wthin the parent
		RECT sRect;
		GetWindowRect(hParentWND, &sRect);
		int iWidth = sRect.right - sRect.left, iHeight = sRect.bottom - sRect.top;
		hNewWND = CreateWindowEx(0,
										 "SaverClass",
										 "Vapour Technology Screen Saver Preview",
										 WS_CHILD | WS_VISIBLE,
										 0,
										 0,
										 iWidth,
										 iHeight,
										 hParentWND,
										 NULL,
										 g_oSettings.GetInstance(),
										 NULL);
	}
	else {
		// Create a full screen window
		int iWidth = GetSystemMetrics(SM_CXSCREEN), iHeight = GetSystemMetrics(SM_CYSCREEN);
		DWORD exstyle, style;
		/* Debug style
		iWidth /= 3;
		iHeight /= 3;
		exstyle = 0;
		style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		*/
		exstyle = WS_EX_TOPMOST;
		style = WS_POPUP | WS_VISIBLE;
		hNewWND = CreateWindowEx(exstyle,
										 "SaverClass",
										 "Vapour Technology Screen Saver",
										 style,
										 0,
										 0,
										 iWidth,
										 iHeight,
										 NULL,
										 NULL,
										 g_oSettings.GetInstance(),
										 NULL);
	}
	// Make sure we have a window
	if (hNewWND == NULL)
		return;

	// Set up the scene
	CreateScene();

	// Get the old SPI_SCREENSAVERRUNNING value
	if (g_poScene) {
		UINT uParam;
		if (g_oSettings.GetMode() == SA_SAVER)
			SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &uParam, 0);
		// Run the screen saver message loop
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Set the old SPI_SCREENSAVERRUNNING value
		if (g_oSettings.GetMode() == SA_SAVER)
			SystemParametersInfo(SPI_SCREENSAVERRUNNING, 0, &uParam, 0);
	}
	// Clean up the windows
	UnregisterClass("SaverClass", g_oSettings.GetInstance());

	// Clean up the scene
	DestroyScene();
} // StartSaver

void CreateScene() {
	// Look for wedgie at end of the saver
	CSFX oSFX;
	ifstream *poWJEFile = NULL;
#ifdef _DEBUG
	// Try to load the wedgie from a seperate file
	poWJEFile = (ifstream*) new fstream;
	if (!poWJEFile)
		return;
	poWJEFile->open("sadata.wje", ios::in|ios::binary|ios::nocreate);
	if (poWJEFile->fail()) {
		poWJEFile->close();
		delete poWJEFile;
		return;
	}
#else
	if (!oSFX.EndSet()) {
		// Try to set the end pointer
		oSFX.SetEnd();
		return;
	}
	else
		poWJEFile = oSFX.GetWedgie();
#endif
	CWedgie oWJE;
	// Open the wedgie
	if (oWJE.Open((fstream*)poWJEFile) != WJE_OK) {
		oWJE.Close();
		delete poWJEFile;
		return;
	}
	// Extract the avatar saver.ame (can't load avatar from wedgie)
	oWJE.Extract("saver.ame");
	// Load the avatar
	CAvatarFile *poFilter = g_oAvatarFileStore.CreateByExtension("ame");
	CAvatar *poAvatar = NULL;
	if (poFilter) {
		g_poAvatar = poFilter->Load("saver.ame");
		// Delete the avatar
		remove("saver.ame");
		delete poFilter;
	}
	// Create the scene
	if (g_poAvatar) {
		g_poScene = (CSceneTestSaver*) new CSceneTestSaver(&g_oSettings);
		if (g_poScene) {
			g_poScene->Create();
			g_poScene->ImportAvatar(g_poAvatar, false);
		}
	}
	// Load any poses
	if (g_poScene) {
		char pcPoseName[STR_SIZE] = "";
		int iCount = 0;
		CAvatarPose oPose;
		bool bDone = false;
		do {
			sprintf(pcPoseName, "sap%03d.vpo", iCount);
			bDone = (oPose.Load(pcPoseName, oWJE) != F_OK);
			// If loaded, import into scene
			if (!bDone)
				g_poScene->ImportPose(oPose);
			iCount++;
		} while (!bDone);
	}
	// Close the wedgie
	oWJE.Close();
	poWJEFile->close();
	delete poWJEFile;
} // CreateScene

void DestroyScene() {
	if (g_poScene) {
		delete g_poScene;
		g_poScene = NULL;
	}
	if (g_poAvatar) {
		delete g_poAvatar;
		g_poAvatar = NULL;
	}
} // DestroyScene

bool VerifyPassword() {
	// Check the OS type
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(osv);
	GetVersionEx(&osv);
	// Return true immediately if NT - handles it's own passwords
	if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return true;
	// Get the password panel
	HINSTANCE hpwdcpl = ::LoadLibrary("PASSWORD.CPL");
	if (hpwdcpl == NULL)
		return true;
	// Get the verify function
	bool bVerify = true;
	typedef BOOL (WINAPI *VERIFYSCREENSAVEPWD)(HWND hwnd);
	VERIFYSCREENSAVEPWD VerifyScreenSavePwd;
	VerifyScreenSavePwd = (VERIFYSCREENSAVEPWD)GetProcAddress(hpwdcpl, "VerifyScreenSavePwd");
	// If found, verify the password
	if (VerifyScreenSavePwd)
		bVerify = (VerifyScreenSavePwd(g_oSettings.GetHWND()) == TRUE);
	// Free the panel
	FreeLibrary(hpwdcpl);

	return bVerify;
} // VerifyPassword

void ChangePassword() {
	// Get the DLL
	HINSTANCE hmpr = ::LoadLibrary("MPR.DLL");
	if (hmpr == NULL)
		return;
	// Get the change function
	typedef VOID (WINAPI *PWDCHANGEPASSWORD)(LPCSTR lpcRegkeyname, HWND hwnd, UINT uiReserved1, UINT uiReserved2);
	PWDCHANGEPASSWORD PwdChangePassword = (PWDCHANGEPASSWORD)::GetProcAddress(hmpr, "PwdChangePasswordA");
	// If found, change the password
	if (PwdChangePassword)
		PwdChangePassword("SCRSAVE", g_oSettings.GetHWND(), 0, 0);
	// Free the DLL
	FreeLibrary(hmpr);
} // ChangePassword

LRESULT CALLBACK SaverWindowProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
	switch (msg) {
		case WM_CREATE:
			// Save the cursor position
			g_oSettings.SaveCursor();
			// Set the draw window
			g_oSettings.SetHWND(hwnd);
			// Create the timer
			g_oSettings.StartTimer();
			break;
		case WM_TIMER:
			// Set the screen size
			RECT sRect;
			GetClientRect(hwnd, &sRect); 
			g_poScene->SetSize(sRect.right, sRect.bottom);
			// Render the scene
			g_poScene->Render();
			break;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_NCACTIVATE:
			// Deactivate for debug
			if ((g_oSettings.GetMode() == SA_SAVER) &&
				 (!g_oSettings.DialogActive()) &&
				 (LOWORD(wParam) == WA_INACTIVE)) {
				g_oSettings.Close();
			}
			break;
		case WM_SETCURSOR:
			if ((g_oSettings.GetMode() == SA_SAVER) &&
				 (!g_oSettings.DialogActive()))
				SetCursor(NULL);
			else
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
			if ((g_oSettings.GetMode() == SA_SAVER) &&
				 (!g_oSettings.DialogActive())) {
				g_oSettings.Close();
			}
			break;
		case WM_MOUSEMOVE:
			// Deactivate for debug
			if ((g_oSettings.GetMode() == SA_SAVER) &&
				 (!g_oSettings.DialogActive())) {
				if (g_oSettings.MovedEnough()) {
					g_oSettings.Close();
				}
			}
			break;
		case (WM_SYSCOMMAND):
			// Only process if saver running
			if (g_oSettings.GetMode() == SA_SAVER) {
				// Eat message to stop a new saver starting
				if (wParam == SC_SCREENSAVE)
					return FALSE;
				// Eat message - Deactivate for debug
				if (wParam == SC_CLOSE)
					return FALSE;
			}
			break;
		case WM_CLOSE:
			// Process if saver running, close called by us and no dialog up
			if ((g_oSettings.GetMode() == SA_SAVER) &&
				 (g_oSettings.Closed()) && 
				 (!g_oSettings.DialogActive())) {
				bool bClose = true;
				// If password required
				if (g_oSettings.CheckPasswordDelay()) {
					// Run the dialog
					g_oSettings.DialogActive(true);
					bClose = VerifyPassword();
					g_oSettings.DialogActive(false);
					// Save the cursor position
					g_oSettings.SaveCursor();
				}
				// Close if set
				if (bClose)
					DestroyWindow(hwnd);
			}
			// Return false so DefProcWindow doesn't get called
			if (g_oSettings.GetMode() == SA_SAVER)
				return FALSE;
			break;
		case WM_DESTROY:
			// Close it all down
			g_oSettings.StopTimer();
			g_poScene->Destroy();
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
} // SaverWindowProc

BOOL CALLBACK ConfigDialogProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {
			return TRUE;
		}
		case WM_COMMAND: {
			int id = LOWORD(wParam);
			if (id == IDOK || id == IDCANCEL)
				EndDialog(hwnd,id);
		}
		break;
	}
	return FALSE;
} // ConfigDialogProc

#ifdef _DEBUG
HWND CheckForScrprev() {
	// Looks for the window
	HWND hwnd = FindWindow("Scrprev", NULL); 
	// If not found, try to run it
	if (hwnd == NULL) {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);
		si.lpReserved = NULL;
		si.lpTitle = NULL;
		si.dwFlags = 0;
		si.cbReserved2 = 0;
		si.lpReserved2 = 0;
		si.lpDesktop = 0;
		BOOL cres = CreateProcess(NULL,
										  "Scrprev",
										  0,
										  0,
										  FALSE,
										  CREATE_NEW_PROCESS_GROUP | CREATE_DEFAULT_ERROR_MODE,
										  0,
										  0,
										  &si,
										  &pi);
		// If not run, leave now
		if (!cres) {
			return NULL;
		}
		// Will never time out, so something wrong
		DWORD wres = WaitForInputIdle(pi.hProcess, 2000);
		if (wres == WAIT_TIMEOUT)
			return NULL;
		if (wres == 0xFFFFFFFF)
			return NULL;
		// Get the window
		hwnd = FindWindow("Scrprev", NULL);
	}
	// If still not found, give up
	if (hwnd == NULL)
		return NULL;
	// Get the child window
	::SetForegroundWindow(hwnd);
	hwnd = GetWindow(hwnd, GW_CHILD);
	if (hwnd == NULL)
		return NULL;

	return hwnd;
} // CheckForScrprev
#endif // _DEBUG
