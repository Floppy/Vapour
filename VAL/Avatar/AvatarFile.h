//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFile.h - 18/05/2000 - James Smith
//	Import/Export filter base class header
//
// $Id: AvatarFile.h,v 1.4 2000/11/21 16:37:42 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILE_
#define _VAL_AVATARFILE_

#include "Avatar.h"
#include <fstream.h>

// DLL import/export definitions
#ifndef DLL
	#ifdef _VAPOUR_EXPORT_DLL_
		#define DLL __declspec(dllexport)
	#endif
	#ifdef _VAPOUR_IMPORT_DLL_
		#define DLL __declspec(dllimport)
	#endif
	#ifndef DLL
		#define DLL
	#endif
#endif

////////////////
// CAvatarFile

class DLL CAvatarFile {
public:
	CAvatarFile() {};

   // Exported Functions
   virtual float GetFilterVersion() const = 0;
   virtual const char* GetFileExtension() const = 0;
   virtual const char* GetFileTitle() const = 0;
   virtual bool CanFilterLoadFile() const = 0;
   virtual bool CanFilterLoadStream() const = 0;
   virtual bool CanFilterSaveFile() const = 0;
   virtual bool CanFilterSaveStream() const = 0;
   virtual bool CanFilterLoadBPFile() const = 0;
   virtual bool CanFilterLoadBPStream() const = 0;

   // Load/Save function have some default behaviour, 
   // so derived classes only have to implement what they actually use.
   // Load functions return NULL if they fail, otherwise they return a new CAvatar
   virtual CAvatar* Load(const char* pszFilename) const {return NULL;}
   virtual CAvatar* Load(ifstream& isInputStream) const {return NULL;}
   // LoadSections functions will not load a complete model - just the specified parts
   virtual CAvatar* LoadSections(const char* pszFilename, int bsSections) const {return NULL;}
   virtual CAvatar* LoadSections(ifstream& isInputStream, int bsSections) const {return NULL;}
   // Save functions return 1 if successful, 0 if not
   virtual FRESULT Save(const char* pszFilename, CAvatar* pAvatar) const {return F_INVALID_OPERATION;}
   virtual FRESULT Save(ofstream& osOutputStream, CAvatar* pAvatar) const {return F_INVALID_OPERATION;}

   // Option functions
   virtual void SetOption(int iOption, int iData) const {return;}
   virtual void SetOption(int iOption, const char* pszData) const {return;}

};

#endif // _VAL_AVATARFILE_
