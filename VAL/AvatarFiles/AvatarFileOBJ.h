//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileOBJ.h - 04/10/2000 - James Smith
//	Wavefront OBJ export filter header
//
// $Id: AvatarFileOBJ.h,v 1.0 2000/10/04 16:27:09 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEOBJ_
#define _VAL_AVATARFILEOBJ_

#include "AvatarFileStore.h"

#include <fstream.h>

//////////////////
// CAvatarFileOBJ

class CAvatarFileOBJ : public CAvatarFile {

/////////////////////
// Member Variables
private:

   mutable char* m_pszBasename;

/////////////////////
// Member Functions
private:

public:
	CAvatarFileOBJ();

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
	static float GetVersion() {return 0.1F;}
	static const char* GetTitle() {return ("Wavefront");}
	static const char* GetExtension() {return ("obj");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return true;}
	static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

};

#endif // _VAL_AVATARFILEOBJ_
