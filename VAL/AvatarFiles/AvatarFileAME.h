//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileAME.h - 18/05/2000 - James Smith
//	AME import filter header
//
// $Id: AvatarFileAME.h,v 1.1 2000/06/16 21:59:03 waz Exp $
//

#pragma once

#ifndef _VAPOUR_AVATARFILEAME_
#define _VAPOUR_AVATARFILEAME_

#include "AvatarFileStore.h"

#include <fstream.h>

//////////////////
// CAvatarFileAME

class CAvatarFileAME : public CAvatarFile {

/////////////////////
// Member Variables
private:

/////////////////////
// Member Functions
private:

public:
	CAvatarFileAME();

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

   CAvatar* Load(const char* pszFilename) const;
   CAvatar* LoadSections(const char* pszFilename, int bsSections) const;

   // AvatarFileProxy Functions
	static float GetVersion() {return 0.1F;}
	static const char* GetTitle() {return ("AvatarMe model");}
	static const char* GetExtension() {return ("ame");}
	static bool CanLoadFile() {return true;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return false;}
   static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return true;}
	static bool CanLoadBPStream() {return false;}

};

#endif // _VAPOUR_AVATARFILEAME_
