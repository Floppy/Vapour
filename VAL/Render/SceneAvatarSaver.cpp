//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SceneAvatarSaver.cpp - 23/11/2000 - Warren Moore
//	  Scene rendering class for avatar screen saver
//
// $Id: SceneAvatarSaver.cpp,v 1.1 2000/11/27 20:30:51 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "SceneAvatarSaver.h"

#include "RenderAvatar.h"
#include "RenderLight.h"
#include "RenderCamera.h"
#include "AvatarPose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////
// CSceneAvatarSaver

CSceneAvatarSaver::CSceneAvatarSaver(CDisplayContext *poDisplay) : 
	CScene(poDisplay),
	m_poAvatar(NULL),
	m_bGeneric(true),
	m_poRAvatar(NULL),
	m_poRLight(NULL),
	m_poRCamera(NULL),
	m_iPoses(0),
	m_iCurPose(0) {
} // Contructor

CSceneAvatarSaver::~CSceneAvatarSaver() {
	if (m_poRC)
		Destroy();
} // Destructor

SCRESULT CSceneAvatarSaver::Create() {
	// Delete a previous context
	if (m_poRC)
		Destroy();

	// Set the render context options
	g_oRenderContextStore.ClearContextOptions();
	g_oRenderContextStore.SetDisplayContext(m_poDisplay);
	g_oRenderContextStore.SetContextOption(RCO_MODE, RCV_DONT_CARE);
	g_oRenderContextStore.SetContextOption(RCO_MEDIUM, RCV_WINDOW);
	g_oRenderContextStore.SetContextOption(RCO_VIEW, RCV_NORMAL);
	g_oRenderContextStore.SetContextOption(RCO_WIDTH, 0U);
	g_oRenderContextStore.SetContextOption(RCO_HEIGHT, 0U);
	g_oRenderContextStore.SetContextOption(RCO_COL_DEPTH, 16U);
	g_oRenderContextStore.SetContextOption(RCO_Z_BUF_SIZE, 16U);
	// Check for a suitable context
	m_poRC = g_oRenderContextStore.CreateSuitableContext();
	if (!m_poRC)
		return SC_CONTEXT_UNAVAILABLE;
	// Render context selected and created so set options
	// Black background
	m_poRC->SetOption(RCO_BACKRED, 0.0F);
	m_poRC->SetOption(RCO_BACKGREEN, 0.0F);
	m_poRC->SetOption(RCO_BACKBLUE, 0.0F);
	// Set the view planes and angle
	m_poRC->SetOption(RCO_NEARPLANE, 5.0F);
	m_poRC->SetOption(RCO_FARPLANE, 25.0F);
	m_poRC->SetOption(RCO_VIEWANGLE, 10.0F);
	m_poRC->SetProjectionMode(RCP_PERSPECTIVE);
	// Create the avatar render object
	NEWBEGIN
	m_poRAvatar = (CRenderAvatar*) new CRenderAvatar(m_poRC);
	NEWEND("CSceneAvatarSaver::Create - Avatar render object")
	// Create the light object
	NEWBEGIN
	m_poRLight = (CRenderLight*) new CRenderLight(m_poRC);
	NEWEND("CSceneAvatarSaver::Create - Light render object")
	// Create the camera object
	NEWBEGIN
	m_poRCamera = (CRenderCamera*) new CRenderCamera(m_poRC);
	NEWEND("CSceneAvatarSaver::Create - Camera render object")
	// Check we're ok
	if ((!m_poRAvatar) || (!m_poRLight) || (!m_poRCamera)) {
		Destroy();
		return SC_OUT_OF_MEMORY;
	}
	// Set up the avatar
	if (m_poAvatar) {
		// Import the avatar
		m_poRAvatar->SetAvatar(m_poAvatar, m_bGeneric);
		// Move the avatar into position
		m_poRAvatar->SetPosition(0.0F, -1.2F, 0.0F);
		// Set the render mode to textured (will set flat-shaded if generic set)
		m_poRAvatar->RenderMode(ROM_TEXTURE);
		// Initialise the avatar
		m_poRC->Enable();
		m_poRAvatar->Init();
		m_poRAvatar->Enable();
		m_poRC->Disable();
	}
	// Set the default camera position
	m_poRC->Enable();
	m_poRCamera->Init();
	m_poRCamera->Enable();
	m_poRCamera->SetPosition(0.0F, 0.0F, -18.0F);
	// Set up the light
	m_poRLight->Init();
	m_poRLight->Enable();
	m_poRC->Disable();

	return SC_OK;
} // Create

SCRESULT CSceneAvatarSaver::Destroy() {
	// If the context is active, clear up objects and delete
	if (m_poRC) {
		// Clear up objects
		m_poRC->Enable();
		if (m_poRAvatar) {
			m_poRAvatar->Disable();
			m_poRAvatar->Exit();
		}
		if (m_poRLight) {
			m_poRLight->Disable();
			m_poRLight->Exit();
		}
		if (m_poRCamera) {
			m_poRCamera->Disable();
			m_poRCamera->Exit();
		}
		m_poRC->Disable();
		// Delete the render context
		delete m_poRC;
		m_poRC = NULL;
	}
	// Delete the objects (done individually in case called due to create error)
	if (m_iPoses > 0) {
		for (int i = 0; i < m_iPoses; i++) 
			delete m_ppPose[i];
		m_iPoses = 0;
	}
	if (m_poRAvatar) {
		delete m_poRAvatar;
		m_poRAvatar = NULL;
	}
	if (m_poRLight) {
		delete m_poRLight;
		m_poRLight = NULL;
	}
	if (m_poRCamera) {
		delete m_poRCamera;
		m_poRCamera = NULL;
	}
	return SC_OK;
} // Destroy

SCRESULT CSceneAvatarSaver::ImportAvatar(CAvatar *poAvatar, bool bGeneric) {
	// Close down the avatar if we have one
	if (m_poAvatar) {
		m_poRC->Enable();
		m_poRAvatar->Disable();
		m_poRAvatar->Exit();
		m_poRC->Disable();
		// Make sure no-one gets crossed wires
		m_poAvatar = NULL;
		m_bGeneric = true;
		m_poRAvatar->SetAvatar(NULL, true);
	}
	// Set up the new avatar
	if (poAvatar) {
		// Set up the vars
		m_poAvatar = poAvatar;
		m_bGeneric = bGeneric;
		// Set the avatar
		m_poRAvatar->SetAvatar(m_poAvatar, m_bGeneric);
		// Set the render mode to textured (will set flat shaded if generic set)
		m_poRAvatar->RenderMode(ROM_TEXTURE);
		// Move the avatar into position
		m_poRAvatar->SetPosition(0.0F, -1.2F, 0.0F);
		// Initialise the render object
		m_poRC->Enable();
		m_poRAvatar->Init();
		m_poRAvatar->Enable();
		m_poRC->Disable();
	}
	return SC_OK;
} // ImportAvatar

SCRESULT CSceneAvatarSaver::ImportPose(CAvatarPose &oPose) {
	// Create a copy of the pose
	CAvatarPose *poPose = NULL;
	NEWBEGIN
	poPose = (CAvatarPose*) new CAvatarPose(oPose);
	NEWEND("CSceneAvatarSaver::ImportPose - New pose object");
	if (!poPose)
		return SC_OUT_OF_MEMORY;
	// Add the pose to the list
	m_ppPose[m_iPoses] = poPose;
	m_iPoses++;
	return SC_OK;
} // ImportPose

SCRESULT CSceneAvatarSaver::Render() {
	// Check for a render context
	if (!m_poRC)
		return SC_NO_CONTEXT;
	// Enable the context
	m_poRC->Enable();
	// Begint the render process
	m_poRC->BeginRender();
	// Clear the buffer
	m_poRC->ClearBuffer(RCB_COLOUR | RCB_DEPTH);
	// Set up the light
	m_poRLight->Execute();
	// Move the camera
	m_poRCamera->RotateBy(Y, -2.0F);
	m_poRCamera->Execute();
	// Update the pose (if any)
	if (m_iPoses > 0) {
		m_poAvatar->ImportPose(*(m_ppPose[m_iCurPose]));
		m_iCurPose++;
		if (m_iCurPose >= m_iPoses)
			m_iCurPose = 0;
	}
	// Render if we have an avatar
	if (m_poAvatar) {
		m_poRAvatar->Execute();
	}
	// Flush the back buffer
	m_poRC->EndRender();
	// Disable the context
	m_poRC->Disable();
	// OK to go
	return SC_OK;
} // Render

SCRESULT CSceneAvatarSaver::Snapshot(CImage *&poImage) {
	// Check for a render context
	if (!m_poRC)
		return SC_NO_CONTEXT;
	// Get the snapshot
	RCRESULT eResult = m_poRC->Snapshot(poImage);
	if (eResult != RC_OK) {
		TRACE("CSceneAvatarSaver::Snapshot - %s\n", m_poRC->GetErrorString(eResult));
		return SC_SNAPSHOT_ERROR;
	}
	return SC_OK;
} // Snapshot

SCRESULT CSceneAvatarSaver::SetSize(int iWidth, int iHeight) {
	// Check for a render context
	if (!m_poRC)
		return SC_NO_CONTEXT;
	// Set the size
	m_poRC->SetSize(iWidth, iHeight);
	return SC_OK;
} // SetSize

