//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// ProgressControl.h - 01/06/2000 - Warren Moore
//	Class for the callback of external progress updates functions
//
// $Id: ProgressControl.h,v 1.1 2000/06/16 21:57:47 waz Exp $
//

#ifndef _PROGRESSCONTROL_
#define _PROGRESSCONTROL_

#pragma once

//#===--- Includes
#include <vector>

//#===--- Typedefs
// Function pointer
typedef void (*TEXT_FUNC)(const char *pText);
typedef void (*HANDLER_FUNC)(double dPercent);

// Progress function name and pointer
typedef struct SProgressControl {
	char *pcName;
	TEXT_FUNC pText;
	HANDLER_FUNC pHandler;
	unsigned int uMax, uCount;

	SProgressControl() {
		pcName = NULL;
		pText = NULL;
		pHandler = NULL;
		uMax = uCount = 0;
	};
} sProgressControl;

// STL vector typedef
typedef std::vector<SProgressControl> VecProgress;

/////////////////////
// CProgressControl

class CProgressControl {
//#===--- External functions
public:

// Constructor/destructor
	CProgressControl();		
	~CProgressControl();

// Client functions
	void SetTextFunction(const char *pcName, TEXT_FUNC pText);
	void SetProgressFunction(const char *pcName, HANDLER_FUNC pHandler);

// Server functions
	// Text output
	void SetText(const char *pcName, const char *pcText);
	// Progress output
	void SetMaxProgress(const char *pcName, unsigned int uMax);
	void SetProgress(const char *pcName, unsigned int uPercent);
	void Step(const char *pcName);

//#===--- Internal Functions
private:

//#===--- Internal Data
private:
	VecProgress m_oProgressTable;				// Progress name and function table
};

#endif // _COMMANDLINE_
