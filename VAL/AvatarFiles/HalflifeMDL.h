//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// HalflifeMDL.h - 16/02/2000 - James Smith
//	Structures and definitions for halflife MDL export
//
// $Id: HalflifeMDL.h,v 1.5 2000/07/21 17:00:08 waz Exp $
//

#ifndef _VAL_HALFLIFEMDL_
#define _VAL_HALFLIFEMDL_

#define HLMDLMAGIC        (('T'<<24)+('S'<<16)+('D'<<8)+'I')
#define HLMDLVERSION      0xA

typedef float HalflifeMDLVector[3];

struct SHalflifeMDLHeader {
   // The magic number for the file type. This is MDLMAGIC
   unsigned long iID;
   // The version number. This is HLMDLVERSION
   unsigned long iVersion;
   // The real name of the MDL file
   char pszName[64];
   // The total length of the file
   unsigned long iLength;
   // Ideal eye position
   HalflifeMDLVector vEyePosition;
   // Movement hull
   HalflifeMDLVector vMin;
   HalflifeMDLVector vMax;
   // Bounding box
   HalflifeMDLVector vBBMin;
   HalflifeMDLVector vBBMax;
     // Flags
   unsigned long iFlags;
   // Number of bones
   unsigned long iNumBones;
   // Bone chunk offset
   unsigned long iBoneChunkOffset;
   // Number of bone controllers
   unsigned long iNumBoneControllers;
   // Bone controller chunk offset
   unsigned long iBoneControllerChunkOffset;
   // Number of hit boxes
   unsigned long iNumHitboxes;
   // Hitbox chunk offset
   unsigned long iHitboxChunkOffset;
   // Number of animation sequences
   unsigned long iNumSeqs;
   // Sequence chunk offset
   unsigned long iSeqChunkOffset;
   // Number of animation sequence groups
   unsigned long iNumSeqGroups;
   // Sequence group chunk offset
   unsigned long iSeqGroupChunkOffset;
   // Number of textures
   unsigned long iNumTextures;
   // Texture header chunk offset
   unsigned long iTextureHeaderChunkOffset;
   // Texture data chunk offset
   unsigned long iTextureDataChunkOffset;
   // Number of replaceable textures
   unsigned long iNumReplaceableTextures;
   // Number of texture families
   unsigned long iNumTextureFamilies;
   // Texture index offset
   unsigned long iTextureIndexOffset;
   // Number of body parts
   unsigned long iNumBodyParts;
   // Body part chunk offset
   unsigned long iBodyPartChunkOffset;
   // Number of attachments
   unsigned long iNumAttachments;
   // Attachment chunk offset
   unsigned long iAttachmentChunkOffset;
   // Sound stuff - all set to 0
   unsigned long iSoundTable;
   unsigned long iSoundOffset;
   unsigned long iSoundGroups;
   unsigned long iSoundGroupOffset;
   // Number of transitions - this is 0;
   unsigned long iNumTransitions;
   // Transition chunk offset
   // As there are no transitions, this is the same as liBodyPartChunkOffset
   unsigned long iTransitionChunkOffset;
};

struct SHalflifeMDLBone {
   // Name
   char pszName[32];
   // Parent
   unsigned long iParent;
   // Bone flags
   unsigned long iFlags;
   // Bonecontroller link
   long iBoneController[6];
   // Joint centre
   HalflifeMDLVector vCentre;
   // Joint Rotation
   HalflifeMDLVector vRotation;
   // Joint centre scale
   HalflifeMDLVector vCentreScale;
   // Joint Rotation scale
   HalflifeMDLVector vRotationScale;
};    

struct SHalflifeMDLBoneController {
   // Which bone this is attached to
   unsigned long iBone;
   // Type
   unsigned long iType;
   // Start
   float fStart;
   // End
   float fEnd;
   // Rest value
   unsigned int iRest;
   // Index - 0-3 user, 4 mouth
   unsigned int iIndex;
};

struct SHalflifeMDLAttachment {
   // Name
   char pszName[32];
   // Type
   unsigned long iType;
   // Which bone this attachment is attached to
   unsigned long iBone;
   // Position
   HalflifeMDLVector vPosition;
   // Vectors??
   HalflifeMDLVector vVector0;
   HalflifeMDLVector vVector1;
   HalflifeMDLVector vVector2;
};

struct SHalflifeMDLHitbox {
   // Bone
   unsigned long iBone;
   // Group
   unsigned long iGroup;
   // Maximum
   HalflifeMDLVector vMaximum;
   // Minimum
   HalflifeMDLVector vMinimum;
};

struct SHalflifeMDLSeqGroup {
   // Name
   char pszName[32];
   // Filename
   char pszFilename[64];
   // Cache index pointer
   unsigned long iCachePtr;
   // Some sort of hack, apparently
   unsigned long iData;
};

struct SHalflifeMDLBodyPart {
   // Name
   char pszName[64];
   // Number of models
   unsigned long iNumModels;
   // Base
   unsigned long iBase;
   // Model index
   unsigned long iModelIndex;
};

struct SHalflifeMDLModel {
   // Name
   char pszName[64];
   // Type
   unsigned long iType;
   // Bounding radius
   float fRadius;
   // Number of meshes
   unsigned long iNumMeshes;
   // Mesh index
   unsigned long iMeshIndex;
   // Number of vertices
   unsigned long iNumVertices;
   // Vertex info index
   unsigned long iVertexInfoIndex;
   // Vertex data index
   unsigned long iVertexDataIndex;
   // Number of normals
   unsigned long iNumNormals;
   // Normal info index
   unsigned long iNormalInfoIndex;
   // Normal data index
   unsigned long iNormalDataIndex;
   // Number of deformation groups
   unsigned long iNumDeformGroups;
   // Deformation group index
   unsigned long iDeformGroupIndex;
};

struct SHalflifeMDLMesh {
   // Number of triangles
   unsigned long iNumTriangles;
   // Triangle data index
   unsigned long iTriangleDataIndex;
   // Skin reference
   unsigned long iSkinRef;
   // Number of per-mesh normals
   unsigned long iNumNormals;
   // Normal index
   unsigned long iNormalIndex;
};

struct SHalflifeMDLTexture {
   // Name
   char pszName[64];
   // Flags
   unsigned long iFlags;
   // Width
   unsigned long iWidth;
   // Height
   unsigned long iHeight;
   // Data index
   unsigned long iIndex;
};

#endif //_VAL_HALFLIFEMDL_