//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileDXF.h - 04/10/2000 - James Smith
//	AutoCAD DXF export filter header
//
// $Id: AvatarFileDXF.h,v 1.0 2000/10/04 16:27:07 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEDXF_
#define _VAL_AVATARFILEDXF_

#include "Val.h"
#include "AvatarFileStore.h"

#include <fstream.h>

//////////////////
// CAvatarFileDXF

class CAvatarFileDXF : public CAvatarFile {

/////////////////////
// Member Variables
private:

/////////////////////
// Member Functions
private:

public:
	CAvatarFileDXF();

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
	FRESULT Save(ofstream& osOutputStream, CAvatar* pAvatar) const;
	
   // AvatarFileProxy Functions
   static float GetVersion() {return 0.1F;}
	static const char* GetTitle() {return ("AutoCAD");}
	static const char* GetExtension() {return ("dxf");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
   static bool CanSaveFile() {return true;}
	static bool CanSaveStream() {return true;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

};

#endif // _VAL_AVATARFILEDXF_
