//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderAvatar.h - 29/02/2000 - Warren Moore
//	Avatar render object header
//
// $Id: RenderAvatar.h,v 1.1 2000/07/29 14:04:35 waz Exp $
//

#ifndef _VAL_RENDERAVATAR_
#define _VAL_RENDERAVATAR_

#pragma once

//#===--- Includes
#include "VAL.h"
#include "RenderObject.h"

#include "Avatar.h"
#include "AvatarPose.h"

//#===--- Defines

//////////////////
// CRenderAvatar

class CRenderAvatar : public CRenderObject {
public:
	CRenderAvatar(CRenderContext *pContext);
	virtual ~CRenderAvatar();

	//#===--- Import/Export
	void SetAvatar(CAvatar *pAvatar, bool bGeneric);
	void ImportPose(CAvatarPose &oPose);
	CAvatarPose ExportPose();

//	void MoveBy(int iX, int iY);

	//#===--- Initialisation/Shutdown
	// Create lists and textures
	virtual void Init();
	// Destroy used objects and memory
	virtual void Exit();

	//#===--- RenderMode Control
	virtual void RenderMode(unsigned int uMode);

	//#===--- Execute
	// Render the avatar
	virtual void Execute();

//#===--- Internal Functions
protected:
	// Different render modes
	virtual void RenderFlat();
	virtual void RenderTexture();
	virtual void RenderSelection();

	// Create and destroy the used textures
	void CreateTextures();
	void DeleteTextures();

//#===--- Internal Data
protected:
	CAvatar *m_pAvatar;										// The avatar to be rendered

	bool m_bNewAvatar;										// New avatar indicator
	bool m_bGeneric;											// Generic (i.e. textureless) avatar indicator

	int m_iNumTextures;										// Number of textures loaded
	int m_iTexture[SC_MAXTEXTURES];				// Texture handle table

//#===--- Friend classes

};

//#===--- Inline Functions

#endif // _VAL_RENDERAVATAR_
