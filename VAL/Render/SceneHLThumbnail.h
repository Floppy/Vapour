//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SceneHLThumbnail.h - 06/10/2000 - Warren Moore
//	Scene rendering class for HalfLife thumbnail production
//
// $Id: SceneHLThumbnail.h,v 1.1 2000/10/10 17:51:46 waz Exp $
//

#ifndef _VAL_SCENEHLTHUMBNAIL_
#define _VAL_SCENEHLTHUMBNAIL_

#pragma once

#include "VAL.h"

#include "Scene.h"
#include "Image.h"
#include "Avatar.h"

//#===--- Defines

//#===--- Data types

//////////////////////
// CSceneHLThumbnail

class CSceneHLThumbnail : public CScene {
public:
	CSceneHLThumbnail();
	virtual ~CSceneHLThumbnail();

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

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	CAvatar *m_poAvatar;
};

//#===--- Inline Functions

#endif // _VAL_SCENEHLTHUMBNAIL_
