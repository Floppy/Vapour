//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SceneAvatarPose.h - 23/11/2000 - Warren Moore
//	Scene rendering class for avatar posing
//
// $Id: SceneAvatarPose.h,v 1.1 2000/12/02 07:39:54 warren Exp $
//

#ifndef _VAL_SCENEAVATARPOSE_
#define _VAL_SCENEAVATARPOSE_

#pragma once

#include "VAL.h"

#include "Scene.h"
#include "Image.h"
#include "Avatar.h"

//#===--- Predeclared classes
class CRenderAvatar;
class CRenderLight;
class CRenderCamera;

//#===--- Defines

#define SC_MOVE_SCALE			3.0F

//#===--- Data types

/////////////////////
// CSceneAvatarPose

class CSceneAvatarPose : public CScene {
public:
	CSceneAvatarPose(CDisplayContext *poDisplay);
	virtual ~CSceneAvatarPose();

//#===--- External Functions
	//#===--- Display Management
	// Creates the scene
	SCRESULT Create();
	// Destroys the scene
	SCRESULT Destroy();
	// Creates the image from the supplied avatar
	SCRESULT Render();
	// Returns an image of the previously rendered avatar
	SCRESULT Snapshot(CImage *&poImage);
	// Sets the window size
	SCRESULT SetSize(int iWidth, int iHeight);

	//#===--- Data Import/Export
	// Imports an avatar (reference used so calling function must delete avatar)
	SCRESULT ImportAvatar(CAvatar *poAvatar, bool bGeneric = true);

	//#===--- Control
	// Indicates the the current stored mouse pos is stale
	void MoveReset();
	// Report that the mouse has been moved to a specified position
	void MoveTo(int iX, int iY);
	// Set to show current state of left mouse button
	void LeftButton(bool bDown);
	// Set to show current state of right mouse button
	void RightButton(bool bDown);

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	CAvatar *m_poAvatar;								// Avatar pointer
	bool m_bGeneric;									// Generic avatar indicator

	//#===--- Render Objects
	CRenderAvatar *m_poRAvatar;					// Avatar renderer
	CRenderLight *m_poRLight;						// Light renderer
	CRenderCamera *m_poRCamera;					// Renderer camera

	//#===--- State variables
	int m_iXPos, m_iYPos;							// Pointer position
	bool m_bLButton, m_bRButton;					// Button indicators
};

//#===--- Inline Functions

#endif // _VAL_SCENEAVATARPOSE_
