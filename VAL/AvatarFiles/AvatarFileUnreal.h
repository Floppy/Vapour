//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileUnreal.h - 5/4/2000 - James Smith
//	Unreal export filter header
//
// $Id: AvatarFileUnreal.h,v 1.3 2000/07/21 17:00:09 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEUNREAL_
#define _VAL_AVATARFILEUNREAL_

#include "AvatarFileStore.h"

#include <fstream.h>

//////////////////////
// CAvatarFileUnreal

class CAvatarFileUnreal : public CAvatarFile {

/////////////////////
// Member Functions
public:
	CAvatarFileUnreal();

// Exported Functions
	float GetFilterVersion() const;
	const char* GetFileExtension() const;
	const char* GetFileTitle() const;
   bool CanFilterLoadFile() const;
   bool CanFilterLoadStream() const;
   bool CanFilterSaveFile() const;
   bool CanFilterSaveStream() const;
   bool CanFilterLoadBPFile() const;
   bool CanFilterLoadBPStream() const;

   int Save(const char* pszFilename, CAvatar* pAvatar) const;

   // AvatarFileProxy Functions
   static float GetVersion() {return 1.0F;}
	static const char* GetTitle() {return ("Unreal");}
	static const char* GetExtension() {return ("3d");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return true;}
	static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

};

#endif // _VAL_AVATARFILEUNREAL_
