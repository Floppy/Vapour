//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// HalflifeMDL.h - 16/02/2000 - James Smith
//	Structures and definitions for halflife MDL export
//
// $Id: HalflifeMDL.h,v 1.1 2000/06/16 21:58:14 waz Exp $
//

#ifndef _VAPOUR_HALFLIFEMDL_
#define _VAPOUR_HALFLIFEMDL_

#include "MathConstants.h"

#define HLMDLMAGIC        (('T'<<24)+('S'<<16)+('D'<<8)+'I')
#define HLMDLVERSION      0xA0
#define HLMDLHEADERLENGTH sizeof(HlMDLHeader)

#define SIZEOF_VECTOR   0xC

struct HlMDLHeader {
   
   // The magic number for the file type. This is MDLMAGIC
   long int liID;
   // The version number. This is MDLVERSION
   long int liVersion;

   // The real name of the MDL file
   char psName[64];

   // The total length of the file
   long int liLength;

   // Various vectors and flags, all of which are set to 0
   char cVectorsAndFlags[64];

   // Number of bones
   long int liNumBones;
   // Bone chunk offset
   long int liBoneChunkOffset;

   // Number of bone controllers
   long int liNumBoneControllers;
   // Bone controller chunk offset
   long int liBoneControllerChunkOffset;

   // Number of hit boxes
   long int liNumHitboxes;
   // Hitbox chunk offset
   long int liHitboxChunkOffset;

   // Number of animation sequences
   long int liNumSeqs;
   // Sequence chunk offset
   long int liSeqChunkOffset;

   // Number of animation sequence groups
   long int liNumSeqGroups;
   // Sequence group chunk offset
   long int liSeqGroupChunkOffset;

   // Number of textures
   long int liNumTextures;
   // Texture header chunk offset
   long int liTextureHeaderChunkOffset;
   // Texture data chunk offset
   long int liTextureDataChunkOffset;

   // Number of replaceable textures
   long int liNumReplaceableTextures;
   // Number of texture families
   long int liNumTextureFamilies;
   // Texture index offset
   long int liTextureIndexOffset;

   // Number of body parts
   long int liNumBodyParts;
   // Body part chunk offset
   long int liBodyPartChunkOffset;

   // Number of attachments
   long int liNumAttachments;
   // Attachment chunk offset
   long int liAttachmentChunkOffset;

   // Sound stuff - all set to 0
   long int liSoundTable;
   long int liSoundOffset;
   long int liSoundGroups;
   long int liSoundGroupOffset;

   // Number of transitions - this is 0;
   long int liNumTransitions;
   // Transition chunk offset
   // As there are no transitions, this is the same as liBodyPartChunkOffset
   long int liTransitionChunkOffset;

};

#endif //_VAPOUR_HALFLIFEMDL_