//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Stores.cpp - 01/06/2000 - Warren Moore
//	VAL Specialty stores for external linkage
//
// $Id: Stores.cpp,v 1.1 2000/06/16 21:57:48 waz Exp $
//

#include "StdAfx.h"

#include "Defs.h"

//#===--- Image file format speciality store
#include "ImageFileStore.h"
CImageFileStore g_oImageFileStore;

//#===--- Avatar file format speciality store
#include "AvatarFileStore.h"
CAvatarFileStore g_oAvatarFileStore;

//#===--- Progress control store
#include "ProgressControl.h"
CProgressControl g_oProgressControl;