//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHalflife.h - 16/2/2000 - James Smith
//	Halflife MDL export filter header
//
// $Id: AvatarFileHalflife.h,v 1.3 2000/07/15 10:39:23 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEHALFLIFE_
#define _VAL_AVATARFILEHALFLIFE_

#include "AvatarFileStore.h"
#include "BodyPart.h"

#include <fstream.h>
#include <vector>

typedef std::vector<int> vTriStrip;

////////////////////////
// CAvatarFileHalflife

class CAvatarFileHalflife : public CAvatarFile {

/////////////////////
// Member Variables
private:

   mutable char* m_pszModelname;
   mutable std::vector<int> m_vCompressedSkeletonMap;
   mutable int m_pReverseCompressedSkeletonMap[TOTAL_NUMBER_BODYPARTS];

   static const char* m_pszJointNames[];

/////////////////////
// Member Functions
public:
	CAvatarFileHalflife();

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
	int Save(ofstream& osOutputStream, CAvatar* pAvatar) const;	

   // AvatarFileProxy Functions
	static float GetVersion() {return 0.2F;}
	static const char* GetTitle() {return ("Half-Life");}
   static const char* GetExtension() {return ("mdl");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
   static bool CanSaveFile() {return true;}
	static bool CanSaveStream() {return true;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

private:

   void CompressSkeleton(const BodyPart bpBodyPart, CAvatar* pAvatar) const;
   std::vector<vTriStrip> CompressSubMesh(std::vector<STriFace>& vSubMesh) const;

};

#endif // _VAL_AVATARFILEHALFLIFE_