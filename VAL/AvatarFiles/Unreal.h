//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Unreal.h - 17/07/2000 - James Smith
//	Structures and definitions for Unreal export
//
// $Id: Unreal.h,v 1.1 2000/07/21 17:00:11 waz Exp $
//

#ifndef _VAL_UNREAL_
#define _VAL_UNREAL_

#define UNREAL_PACKAGE_TAG        0x9E2A83C1
#define UNREAL_PACKAGE_VERSION    0x00000044

// Package flags
#define UNREAL_PKG_ALLOWDOWNLOAD  0x00000001
#define UNREAL_PKG_CLIENTOPTIONAL 0x00000002
#define UNREAL_PKG_SERVERSIDEONLY 0x00000004

// Object flags
#define UNREAL_RF_TRANSACTIONAL   0x00000001
#define UNREAL_RF_PUBLIC          0x00000004
#define UNREAL_RF_SOURCEMODIFIED  0x00000020
#define UNREAL_RF_LOADFORCLIENT   0x00010000
#define UNREAL_RF_LOADFORSERVER   0x00020000
#define UNREAL_RF_LOADFOREDIT     0x00040000
#define UNREAL_RF_STANDALONE      0x00080000
#define UNREAL_RF_HASSTACK        0x02000000
#define UNREAL_RF_INTRINSIC       0x04000000

#define UNREAL_VAPOUR_GUID_TEX "vapour-t-"
#define UNREAL_VAPOUR_GUID_MSH "vapour-m-"

struct SUnrealPackageHeader {
   // Tag - always 0x9E2A83C1 (UNREAL_PACKAGE_TAG)
   unsigned long iTag;
   // Version of the engine which saved the file. (UNREAL_PACKAGE_VERSION)
   unsigned long iFileVersion;
   // Bitflags describing the package - these are the package flags defined above.
   unsigned long iPackageFlags;
   // Number of names stored in the name table. Always >= 0.
   unsigned long iNameCount;
   // Offset into the file of the name table.
   unsigned long iNameOffset;
   // Number of exported objects in the export table. Always >= 0.
   unsigned long iExportCount;
   // Offset into the file of the export table.
   unsigned long iExportOffset;
   // Number of imported objects in the import table. Always >= 0.
   unsigned long iImportCount;
   // Offset into the file of the import table.
   unsigned long iImportOffset;
   // A GUID for the package?
   char pcGUID[16];
   // Number of GUIDs?
   unsigned long iGUIDCount;
   // Repeated number of names?
   unsigned long iNameCountAgain;
   // Repeated number of exports?
   unsigned long iExportCountAgain;
};

struct SUnrealImportTableEntry {
   // Reference to the package to import
   unsigned char cClassPackage;
   // Reference to the class to import
   unsigned char cClassName;
   // Package index
   long iPackageIndex;
   // Object name
   unsigned char cObjectName;
};

#endif //_VAL_UNREAL_