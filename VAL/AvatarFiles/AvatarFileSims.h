//=======---
// Avanew
//-------
// Avatar editor and exporter
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileSims.h - 5/4/2000 - James Smith
//	Sims export filter header
//
// $Id: AvatarFileSims.h,v 1.1 2000/06/16 21:59:03 waz Exp $
//

#pragma once

#ifndef _VAPOUR_AVATARFILESIMS_
#define _VAPOUR_AVATARFILESIMS_

#include "AvatarFileStore.h"
#include "ImageFileStore.h"
#include "SGAToSims.h"

#include <fstream.h>

extern CImageFileStore g_oImageFileStore;

//////////////////
// CAvatarFileSims

class CAvatarFileSims : public CAvatarFile {

/////////////////////
// Member Variables
private:

   mutable int m_iAge;
   mutable int m_iSex;
   mutable int m_iSkin;
   mutable int m_iBuild;

/////////////////////
// Member Functions
public:
	CAvatarFileSims();

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

   void SetOption(int iOption, int iData) const;

   // AvatarFileProxy Functions
   static float GetVersion() {return 1.0F;}
	static const char* GetTitle() {return ("The Sims");}
	static const char* GetExtension() {return ("sims");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return true;}
	static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

};

#endif // _VAPOUR_AVATARFILESIMS_
