//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileGeneric.h - 18/05/2000 - James Smith
//	Generic avatar import filter header
//
// $Id: AvatarFileGeneric.h,v 1.1 2000/10/06 13:16:58 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEGENERIC_
#define _VAL_AVATARFILEGENERIC_

#include "AvatarFileStore.h"

#include <fstream.h>

//////////////////
// CAvatarFileGeneric

class CAvatarFileGeneric : public CAvatarFile {

/////////////////////
// Member Variables
private:

/////////////////////
// Member Functions
private:

public:
	CAvatarFileGeneric();

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

   CAvatar* Load(const char* pszFilename = NULL) const;

   // AvatarFileProxy Functions
	static float GetVersion() {return 0.1F;}
	static const char* GetTitle() {return ("Generic avatar");}
	static const char* GetExtension() {return ("generic");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return false;}
	static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}
};

#endif // _VAL_AVATARFILEGENERIC_
