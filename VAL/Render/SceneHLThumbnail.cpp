//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SceneHLThumbnail.h - 06/10/2000 - Warren Moore
//	Scene rendering class for HalfLife thumbnail production
//
// $Id: SceneHLThumbnail.cpp,v 1.1 2000/10/10 17:51:46 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "SceneHLThumbnail.h"

#include "RenderAvatar.h"
#include "RenderLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////
// CSceneHLThumbnail

CSceneHLThumbnail::CSceneHLThumbnail() : CScene() {
	m_poAvatar = NULL;
} // Contructor

CSceneHLThumbnail::~CSceneHLThumbnail() {
} // Destructor

SCRESULT CSceneHLThumbnail::Create() {
	// Delete a previous context
	if (m_poRC) {
		delete m_poRC;
		m_poRC = NULL;
	}
	// Set the render context options
	g_oRenderContextStore.ClearContextOptions();
	g_oRenderContextStore.SetContextOption(RCO_MODE, RCV_DONT_CARE);
	g_oRenderContextStore.SetContextOption(RCO_MEDIUM, RCV_BUFFER);
	g_oRenderContextStore.SetContextOption(RCO_VIEW, RCV_NORMAL);
	g_oRenderContextStore.SetContextOption(RCO_WIDTH, 164U);
	g_oRenderContextStore.SetContextOption(RCO_HEIGHT, 200U);
	g_oRenderContextStore.SetContextOption(RCO_COL_DEPTH, 24U);
	g_oRenderContextStore.SetContextOption(RCO_Z_BUF_SIZE, 16U);
	// Check for a suitable context
	m_poRC = g_oRenderContextStore.CreateSuitableContext();
	if (!m_poRC)
		return SC_CONTEXT_UNAVAILABLE;
	// Render context selected and created so return OK
	return SC_OK;
} // Create

SCRESULT CSceneHLThumbnail::Destroy() {
	if (m_poRC)
		delete m_poRC;
	return SC_OK;
} // Destroy

SCRESULT CSceneHLThumbnail::ImportAvatar(CAvatar *poAvatar) {
	// Save the avatar pointer
	m_poAvatar = poAvatar;
	return SC_OK;
} // ImportAvatar

SCRESULT CSceneHLThumbnail::Render() {
	// Check for a render context
	if (!m_poRC)
		return SC_NO_CONTEXT;
	// Check we have an avatar
	if (!m_poAvatar)
		return SC_NO_DATA;
	// Black background
	m_poRC->SetOption(RCO_BACKRED, 0.0F);
	m_poRC->SetOption(RCO_BACKGREEN, 0.0F);
	m_poRC->SetOption(RCO_BACKBLUE, 0.0F);
	// Set the view planes and angle
	m_poRC->SetOption(RCO_NEARPLANE, 0.5F);
	m_poRC->SetOption(RCO_FARPLANE, 20.0F);
	m_poRC->SetOption(RCO_VIEWANGLE, 10.0F);
	m_poRC->SetProjectionMode(RCP_PERSPECTIVE);
	// Create the avatar render object
	CRenderAvatar oAvatar(m_poRC);
	oAvatar.SetAvatar(m_poAvatar, false);
	// Move the avatar into position
	oAvatar.SetPosition(0.0F, -(m_poAvatar->Height() / 2.0F), -12.0F);
	// Set the render mode to textured
	oAvatar.RenderMode(ROM_TEXTURE);
	// Create the light
	CRenderLight oLight(m_poRC);
	// Enable the context
	m_poRC->Enable();
	// Initialise the avatar
	oAvatar.Init();
	// Enable the light
	oLight.Enable();
	// Clear the buffer
	m_poRC->ClearBuffer(RCB_COLOUR | RCB_DEPTH);
	// Set up the light
	oLight.Execute();
	// Render the avatar
	oAvatar.Execute();
	// Clear-up the avatar
	oAvatar.Exit();
	// Disable the context
	m_poRC->Disable();
	// OK to go
	return SC_OK;
} // Render

SCRESULT CSceneHLThumbnail::Snapshot(CImage *&poImage) {
	// Check for a render context
	if (!m_poRC)
		return SC_NO_CONTEXT;
	// Get the snapshot
	RCRESULT eResult = m_poRC->Snapshot(poImage);
	if (eResult != RC_OK)
		return SC_SNAPSHOT_ERROR;
	return SC_OK;
} // Snapshot
