//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SceneAvatarPose.h - 23/11/2000 - Warren Moore
//	Scene rendering class for avatar posing
//
// $Id: SceneAvatarPose.h,v 1.1 2000/11/25 11:30:37 waz Exp $
//

#ifndef _VAL_SCENEAVATARPOSE_
#define _VAL_SCENEAVATARPOSE_

#pragma once

#include "VAL.h"

#include "Scene.h"
#include "Image.h"
#include "Avatar.h"

//#===--- Defines

//#===--- Data types

/////////////////////
// CSceneAvatarPose

class CSceneAvatarPose : public CScene {
public:
	CSceneAvatarPose(CDisplayContext *poDisplay);
	virtual ~CSceneAvatarPose();

//#===--- External Functions
	// Creates the scene
	SCRESULT Create();
	// Destroys the scene
	SCRESULT Destroy();
	// Imports an avatar (reference used so calling function must delete avatar)
	SCRESULT ImportAvatar(CAvatar *poAvatar);
	// Creates the image from the supplied avatar
	SCRESULT Render();
	// Returns an image of the previously rendered avatar
	SCRESULT Snapshot(CImage *&poImage);

	// Sets the window size
	SCRESULT SetSize(int iWidth, int iHeight);
	// Checks whether the render context is active
	bool Active() const;

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	CAvatar *m_poAvatar;
};

//#===--- Inline Functions

inline bool CSceneAvatarPose::Active() const {
	return (m_poRC != NULL);
} // Active

#endif // _VAL_SCENEAVATARPOSE_
