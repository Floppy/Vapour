//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SceneHLThumbnail.h - 23/11/2000 - Warren Moore
//	Scene rendering class for avatar posing
//
// $Id: SceneAvatarPose.cpp,v 1.2 2000/11/27 01:28:18 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "SceneAvatarPose.h"

#include "RenderAvatar.h"
#include "RenderLight.h"
#include "RenderCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////
// CSceneAvatarPose

CSceneAvatarPose::CSceneAvatarPose(CDisplayContext *poDisplay) : 
	CScene(poDisplay),
	m_poAvatar(NULL),
	m_bGeneric(true),
	m_poRAvatar(NULL),
	m_poRLight(NULL),
	m_poRCamera(NULL),
	m_iXPos(0xFFFFFFFF),
	m_iYPos(0xFFFFFFFF),
	m_bLButton(false),
	m_bRButton(false) {
} // Contructor

CSceneAvatarPose::~CSceneAvatarPose() {
	if (m_poRC)
		Destroy();
} // Destructor

SCRESULT CSceneAvatarPose::Create() {
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
	g_oRenderContextStore.SetContextOption(RCO_COL_DEPTH, 24U);
	g_oRenderContextStore.SetContextOption(RCO_Z_BUF_SIZE, 32U);
	// Check for a suitable context
	m_poRC = g_oRenderContextStore.CreateSuitableContext();
	if (!m_poRC)
		return SC_CONTEXT_UNAVAILABLE;
	// Render context selected and created so set options
	// Black background
	m_poRC->SetOption(RCO_BACKRED, 0.5F);
	m_poRC->SetOption(RCO_BACKGREEN, 0.5F);
	m_poRC->SetOption(RCO_BACKBLUE, 0.5F);
	// Set the view planes and angle
	m_poRC->SetOption(RCO_NEARPLANE, 5.0F);
	m_poRC->SetOption(RCO_FARPLANE, 15.0F);
	m_poRC->SetOption(RCO_VIEWANGLE, 10.0F);
	m_poRC->SetProjectionMode(RCP_PERSPECTIVE);
	// Create the avatar render object
	NEWBEGIN
	m_poRAvatar = (CRenderAvatar*) new CRenderAvatar(m_poRC);
	NEWEND("CSceneAvatarPose::Create - Avatar render object")
	// Create the light object
	NEWBEGIN
	m_poRLight = (CRenderLight*) new CRenderLight(m_poRC);
	NEWEND("CSceneAvatarPose::Create - Light render object")
	// Create the camera object
	NEWBEGIN
	m_poRCamera = (CRenderCamera*) new CRenderCamera(m_poRC);
	NEWEND("CSceneAvatarPose::Create - Camera render object")
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
		m_poRAvatar->SetPosition(0.0F, -(m_poAvatar->Height() / 2.0F), 0.0F);
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
	m_poRCamera->SetPosition(0.0F, 0.0F, -12.0F);
	// Set up the light
	m_poRLight->Init();
	m_poRLight->Enable();
	m_poRC->Disable();

	return SC_OK;
} // Create

SCRESULT CSceneAvatarPose::Destroy() {
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

SCRESULT CSceneAvatarPose::ImportAvatar(CAvatar *poAvatar, bool bGeneric) {
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
		m_poRAvatar->SetPosition(0.0F, -(m_poAvatar->Height() / 2.0F), 0.0F);
		// Initialise the render object
		m_poRC->Enable();
		m_poRAvatar->Init();
		m_poRAvatar->Enable();
		m_poRC->Disable();
	}
	return SC_OK;
} // ImportAvatar

SCRESULT CSceneAvatarPose::Render() {
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
	m_poRCamera->Execute();
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

SCRESULT CSceneAvatarPose::Snapshot(CImage *&poImage) {
	// Check for a render context
	if (!m_poRC)
		return SC_NO_CONTEXT;
	// Get the snapshot
	RCRESULT eResult = m_poRC->Snapshot(poImage);
	if (eResult != RC_OK) {
		TRACE("CSceneAvatarPose::Snapshot - %s\n", m_poRC->GetErrorString(eResult));
		return SC_SNAPSHOT_ERROR;
	}
	return SC_OK;
} // Snapshot

SCRESULT CSceneAvatarPose::SetSize(int iWidth, int iHeight) {
	// Check for a render context
	if (!m_poRC)
		return SC_NO_CONTEXT;
	// Set the size
	m_poRC->SetSize(iWidth, iHeight);
	m_poRC->Resize();
	return SC_OK;
} // SetSize

void CSceneAvatarPose::MoveReset() {
	m_iXPos = 0xFFFFFFFF;
	m_iYPos = 0xFFFFFFFF;
} // ResetMove

void CSceneAvatarPose::MoveTo(int iX, int iY) {
	// Update stale position
	if ((m_iXPos == 0xFFFFFFFF) && (m_iYPos == 0xFFFFFFFF)) {
		m_iXPos = iX;
		m_iYPos = iY;
		return;
	}
	// Calculate position offset
	int iXDiff = iX - m_iXPos;
	int iYDiff = iY - m_iYPos;
	// Act on position update
	if (m_bLButton && (iXDiff || iYDiff)) {
		// TODO: Add state management here
		float fX = ((float)iXDiff) / SC_MOVE_SCALE;
		float fY = ((float)iYDiff) / SC_MOVE_SCALE;
		m_poRCamera->RotateBy(X, fY);
		m_poRCamera->RotateBy(Y, fX);
	}
	// Store the new position
	m_iXPos = iX;
	m_iYPos = iY;
} // MoveTo

void CSceneAvatarPose::LeftButton(bool bDown) {
	m_bLButton = bDown;
} // LeftButton

void CSceneAvatarPose::RightButton(bool bDown) {
	m_bRButton = bDown;
} // RightButton
