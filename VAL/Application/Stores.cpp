//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Stores.cpp - 01/06/2000 - Warren Moore
//	VAL Specialty stores for external linkage
//
// $Id: Stores.cpp,v 1.4 2000/10/06 13:02:48 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"

//#===--- Image file format speciality store
#include "ImageFileStore.h"
CImageFileStore g_oImageFileStore;

//#===--- Avatar file format speciality store
#include "AvatarFileStore.h"
CAvatarFileStore g_oAvatarFileStore;

//#===--- Render context speciality store
#include "RenderContextStore.h"
CRenderContextStore g_oRenderContextStore;

