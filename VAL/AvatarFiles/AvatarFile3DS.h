//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFile3DS.h - 04/10/2000 - James Smith
//	3D Studio export filter header
//
// $Id: AvatarFile3DS.h,v 1.0 2000/10/04 16:27:07 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILE3DS_
#define _VAL_AVATARFILE3DS_

#include "Val.h"

#include "AvatarFileStore.h"
#include "ImageFileStore.h"

#include <fstream.h>

extern CImageFileStore g_oImageFileStore;

//////////////////
// CAvatarFile3DS

class CAvatarFile3DS : public CAvatarFile {

/////////////////////
// Member Variables
private:

/////////////////////
// Member Functions
public:
	CAvatarFile3DS();

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

   FRESULT Save(const char* pszFilename, CAvatar* pAvatar) const;

   // AvatarFileProxy Functions
   static float GetVersion() {return 1.0F;}
	static const char* GetTitle() {return ("3D Studio");}
	static const char* GetExtension() {return ("3ds");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return true;}
	static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

};

#endif // _VAL_AVATARFILE3DS_
