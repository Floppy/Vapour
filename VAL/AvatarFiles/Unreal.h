//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Unreal.h - 17/07/2000 - James Smith
//	Structures and definitions for Unreal export
//
// $Id: Unreal.h,v 1.3 2000/08/30 14:08:51 waz Exp $
//

#ifndef _VAL_UNREAL_
#define _VAL_UNREAL_

#define UNREAL_PACKAGE_TAG        0x9E2A83C1
#define UNREAL_PACKAGE_VERSION    0x00000045

// Package flags
#define UNREAL_PKG_ALLOWDOWNLOAD  0x00000001
#define UNREAL_PKG_CLIENTOPTIONAL 0x00000002
#define UNREAL_PKG_SERVERSIDEONLY 0x00000004

// Object flags
#define UNREAL_RF_TRANSACTIONAL   0x00000001
#define UNREAL_RF_PUBLIC          0x00000004
#define UNREAL_RF_UNKNOWNFLAG     0x00000010 // We don't know what this one is... but we need it!
#define UNREAL_RF_SOURCEMODIFIED  0x00000020
#define UNREAL_RF_LOADFORCLIENT   0x00010000
#define UNREAL_RF_LOADFORSERVER   0x00020000
#define UNREAL_RF_LOADFOREDIT     0x00040000
#define UNREAL_RF_STANDALONE      0x00080000
#define UNREAL_RF_SCRIPTFLAG      0x00300000
#define UNREAL_RF_HASSTACK        0x02000000
#define UNREAL_RF_INTRINSIC       0x04000000

// Mutually exclusive triangle flags for .3d file
#define UNREAL_3D_TRI_NORMAL1SIDE   0
#define UNREAL_3D_TRI_NORMAL2SIDE   1
#define UNREAL_3D_TRI_TRANSLUC2SIDE 2
#define UNREAL_3D_TRI_MASKED2SIDE   3
#define UNREAL_3D_TRI_MODBLEND2SIDE 4
#define UNREAL_3D_TRI_WEAPONTRI     8

// Additional triangle flags for .3d file
#define UNREAL_3D_TRI_FULLBRIGHT    16
#define UNREAL_3D_TRI_FLAT          32
#define UNREAL_3D_TRI_ENVMAPPED     64
#define UNREAL_3D_TRI_NOTEXFILTER   128

// Mutually exclusive triangle flags for .u file
#define UNREAL_U_TRI_NORMAL1SIDE   0x00000000
#define UNREAL_U_TRI_WEAPONTRI     0x00000101

// GUID stuff
#define UNREAL_VAPOUR_GUID_TEX "vapour-t-"
#define UNREAL_VAPOUR_GUID_MSH "vapour-m-"

// Class IDs
#define UNREAL_CLASS_TEXTURE          0x82
#define UNREAL_CLASS_PALETTE          0x83
#define UNREAL_CLASS_MESHSCRIPT       0x00010083
#define UNREAL_CLASS_BOTSCRIPT        0x00040083
#define UNREAL_CLASS_SELECTSCRIPT     0x00070083
#define UNREAL_CLASS_MODEL            0x00000087
#define UNREAL_CLASS_MESHPROPERTIES   0x00008800
#define UNREAL_CLASS_BOTPROPERTIES    0x00008900
#define UNREAL_CLASS_SELECTPROPERTIES 0x00008B00
#define UNREAL_CLASS_FORCEMESHTOEXIST 0x0004008C

// Texture sizes
#define UNREAL_TEXTURE_WIDTH  256
#define UNREAL_TEXTURE_HEIGHT 256

struct SUnrealPackageHeader {
   // Tag - always 0x9E2A83C1 (UNREAL_PACKAGE_TAG)
   unsigned long uTag;
   // Version of the engine which saved the file. (UNREAL_PACKAGE_VERSION)
   unsigned long uFileVersion;
   // Bitflags describing the package - these are the package flags defined above.
   unsigned long uPackageFlags;
   // Number of names stored in the name table. Always >= 0.
   unsigned long uNameCount;
   // Offset into the file of the name table.
   unsigned long uNameOffset;
   // Number of exported objects in the export table. Always >= 0.
   unsigned long uExportCount;
   // Offset into the file of the export table.
   unsigned long uExportOffset;
   // Number of imported objects in the import table. Always >= 0.
   unsigned long uImportCount;
   // Offset into the file of the import table.
   unsigned long uImportOffset;
   // A GUID for the package?
   char pcGUID[16];
   // Number of GUIDs?
   unsigned long uGUIDCount;
   // Repeated number of exports?
   unsigned long uExportCountAgain;
   // Repeated number of names?
   unsigned long uNameCountAgain;
};

// Pack next structure tightly
#pragma pack(1)

struct SUnrealImportTableEntry {
   // Reference to the package to import
   unsigned char cClassPackage;
   // Reference to the class to import
   unsigned char cClassName;
   // Package index
   unsigned long uPackageIndex;
   // Object name
   unsigned char cObjectName;
};

// Restore default packing
#pragma pack()

struct SUnrealTriangle {
   // Vertex references
   unsigned short uVertex0;
   unsigned short uVertex1;
   unsigned short uVertex2;
   // Texture coordinates
   unsigned char uU0;
   unsigned char uV0;
   unsigned char uU1;
   unsigned char uV1;
   unsigned char uU2;
   unsigned char uV2;
   // Flags
   unsigned long uFlags;
   // Texture number
   unsigned long uTexture;
};

// Compact Index class
class CUnrealCompactIndex {
public:
   // Sets the compact index to the passed value
   CUnrealCompactIndex(int iIndex);
   // Creates a Compact Index with the passed values
   CUnrealCompactIndex(unsigned char B0=0, unsigned char B1=0, unsigned char B2=0, unsigned char B3=0, unsigned char B4=0);
   // Sets the compact index to the passed value
   void Set(int iIndex);
   // Creates a Compact Index with the passed values
   void Set(unsigned char B0=0, unsigned char B1=0, unsigned char B2=0, unsigned char B3=0, unsigned char B4=0);
   // Returns the value as an integer
   int Index(void) const;
   // How many bytes in the compact index?
   int NumBytes(void) const;
   // Gets the compact index
   const unsigned char* CompactIndex(void) const;

private:

   int m_iNumBytes;
   unsigned char m_pIndexBuffer[5];

};

#endif //_VAL_UNREAL_