//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHANIM.h - 04/10/2000 - James Smith
//	H-Anim 1.1 export filter header
//
// $Id: AvatarFileHANIM.h,v 1.0 2000/10/04 16:27:08 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEHANIM_
#define _VAL_AVATARFILEHANIM_

#include "AvatarFileStore.h"

#include <fstream.h>

//////////////////
// CAvatarFileHANIM

class CAvatarFileHANIM : public CAvatarFile {

/////////////////////
// Member Variables
private:

   mutable char* m_pszBasename;
   mutable int* m_pMinima;
   mutable int* m_pMaxima;
   mutable int m_iTotalHeight;

/////////////////////
// Member Functions
public:
	CAvatarFileHANIM();

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
	static const char* GetTitle() {return ("VRML H-Anim 1.1");}
	static const char* GetExtension() {return ("wrl");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return true;}
   static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

private:

	void WriteBodyPart(BodyPart bpCurrentPart, CAvatar* pAvatar, ofstream& osOutputStream) const;
	// Writes a single body part to an output stream

	void WriteUSEStatement(BodyPart bpCurrentPart, CAvatar* pAvatar, ofstream& osOutputStream, bool bSegment = false) const;
	// Writes a USE statement for a body part	

};

#endif // _VAL_AVATARFILEHANIM_
