//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// Defs.h - 21/12/1999 - Warren Moore
//	Global Definitions for VAL
//
// $Id: Defs.h,v 1.3 2000/07/10 09:08:02 waz Exp $
//

#ifndef _VAL_DEFS_
#define _VAL_DEFS_

#pragma once

//#===--- Macros

#define NEWBEGIN	try { 
#define NEWEND(x)	} catch (CMemoryException *pException) { TRACE("Out of memory : %s\n", x); pException->Delete(); } 


#define STR_SIZE				256

#define SPLASH_TIME			3

//#===--- CImage

// Defines
#define IMG_MAXWIDTH		1600
#define IMG_MAXHEIGHT		1200

#define IMG_MAXZOOM  5
#define IMG_MINZOOM -5

// Data Types
enum IMAGETYPE {
	IT_UNKNOWN,
	IT_MONO,
	IT_GREY,
	IT_PALETTE,
	IT_RGB,
};

enum IMAGEFILTERTYPE {
	IF_BOX,
	IF_BILINEAR,
};

enum IRESULT {
	I_OK,
	I_OUTOFRANGE,
	I_INVALIDPARAM,
	I_INCORRECTTYPE,
	I_UNSUPPORTEDTYPE,
	I_NOIMAGE,
	I_OUTOFMEMORY,
	I_ERROR,
};

//#===--- Standard Enum
enum AXIS {
	X,
	Y,
	Z,
};

//#===--- Generic File Results
enum FRESULT {
	F_OK,
	F_DOESNOTEXIST,
	F_FILEERROR,
	F_WRONGFILETYPE,
	F_WRONGIMAGETYPE,
	F_OUTOFMEMORY,
	F_WRONGCONTEXT,
	F_HASNTCHANGED,
	F_NOFILENAME,
	F_NOFILEEXTENSION,
	F_NODATATOSAVE,
	F_ERROR,
};

#endif // _VAL_DEFS_