//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Stores.cpp - 01/06/2000 - Warren Moore
//	VAL Specialty stores for external linkage
//
// $Id: Stores.cpp,v 1.3 2000/07/19 08:47:36 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"

//#===--- Image file format speciality store
#include "ImageFileStore.h"
CImageFileStore g_oImageFileStore;

//#===--- Avatar file format speciality store
#include "AvatarFileStore.h"
CAvatarFileStore g_oAvatarFileStore;

