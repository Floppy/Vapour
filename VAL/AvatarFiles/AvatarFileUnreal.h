//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileUnreal.h - 5/4/2000 - James Smith
//	Unreal export filter header
//
// $Id: AvatarFileUnreal.h,v 1.5 2000/08/21 23:06:42 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEUNREAL_
#define _VAL_AVATARFILEUNREAL_

#include "AvatarFileStore.h"
#include "Palette.h"

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

   FRESULT Save(const char* pszFilename, CAvatar* pAvatar) const;

   // AvatarFileProxy Functions
   static float GetVersion() {return 1.0F;}
	static const char* GetTitle() {return ("Unreal");}
	static const char* GetExtension() {return ("u");}
	static bool CanLoadFile() {return false;}
	static bool CanLoadStream() {return false;}
	static bool CanSaveFile() {return true;}
	static bool CanSaveStream() {return false;}
	static bool CanLoadBPFile() {return false;}
	static bool CanLoadBPStream() {return false;}

private:

   // Packs a vertex into 4 bytes
   long PackVertex(const SPoint3D& pntVertex, double dScale, double dYOff) const;
   // Cleans up all allocated memory
   FRESULT Cleanup(void) const;

   FRESULT SaveMeshINT(const char* pszFilename) const;
   FRESULT SaveTextureINT(const char* pszFilename) const;
   FRESULT SaveTextureUTX(const char* pszFilename, const CAvatar* pAvatar) const;
   FRESULT SaveMeshU(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT SaveMeshD3D(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT SaveMeshA3D(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT SaveSelectionMeshD3D(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT SaveSelectionMeshA3D(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT SaveMeshScript(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT SaveSelectionMeshScript(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT SaveBotScript(const char* pszFilename, CAvatar* pAvatar) const;
   FRESULT Init(const char* pszFilename, const CAvatar* pAvatar) const;

   // Various name strings
   mutable char* m_pszName;
   mutable char* m_pszTgtDir;
   mutable char* m_pszTexPackageName;
   mutable char* m_pszTexBaseName;
   mutable char m_pszTempFilename[128];
   // Texture coordinate translation data
   mutable unsigned int* m_puImageMinima;
   mutable unsigned int* m_puImageMaxima;
   mutable unsigned int m_uTotalHeight;

};

#endif // _VAL_AVATARFILEUNREAL_
