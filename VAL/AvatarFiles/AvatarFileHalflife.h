//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHalflife.h - 16/2/2000 - James Smith
//	Halflife MDL export filter header
//
// $Id: AvatarFileHalflife.h,v 1.8 2000/11/21 16:44:32 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEHALFLIFE_
#define _VAL_AVATARFILEHALFLIFE_

#include "AvatarFileStore.h"
#include "AvatarPose.h"
#include "BodyPart.h"
#include "Wedgie.h"
#include "HalflifeMDL.h"

#include <fstream.h>
#include <vector>

////////////////////////
// CAvatarFileHalflife

class CAvatarFileHalflife : public CAvatarFile {

/////////////////////
// Member Variables
private:

   // Data used for export
   mutable char* m_pszModelname;
   mutable std::vector<int> m_vCompressedSkeletonMap;
   mutable int m_pReverseCompressedSkeletonMap[TOTAL_NUMBER_BODYPARTS];
   mutable char* m_pcTextureDataChunk;
   mutable char* m_pcTextureIndexChunk;
   mutable SHalflifeTexture* m_pTextureHeaderChunk;
   mutable char* m_pcMeshDataChunk;
   mutable SHalflifeMesh* m_pMeshHeaderChunk;
   mutable char* m_pcModelDataChunk;
   mutable SHalflifeSeqGroup* m_pSeqGroupsChunk;
   mutable SHalflifeEvent* m_pEventChunk;
   mutable SHalflifeHitbox* m_pHitboxChunk;
   mutable SHalflifeAttachment* m_pAttachmentChunk;
   mutable SHalflifeBoneController* m_pBoneControllerChunk;
   mutable SHalflifeBone* m_pBoneChunk;
   mutable fstream m_fsDataWJE;
   mutable CWedgie m_oWedgie;

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

	FRESULT Save(const char* pszFilename, CAvatar* pAvatar) const;
	FRESULT Save(ofstream& osOutputStream, CAvatar* pAvatar) const;	

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

   void SaveThumbnail(const char *pcBitmap, CAvatar *poAvatar) const;

   void Cleanup(CAvatar* pAvatar) const;
   // Deletes memory used during output, closes streams, and restores avatar pose
};

#endif // _VAL_AVATARFILEHALFLIFE_