//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// ProgressControl.h - 01/06/2000 - Warren Moore
//	Class for the callback of external progress updates functions
//
// $Id: ProgressControl.h,v 1.3 2000/07/19 08:50:11 waz Exp $
//

#ifndef _VAL_PROGRESSCONTROL_
#define _VAL_PROGRESSCONTROL_

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
	virtual ~CProgressControl();

// Client functions
	virtual void SetTextFunction(const char *pcName, TEXT_FUNC pText);
	virtual void SetProgressFunction(const char *pcName, HANDLER_FUNC pHandler);

// Server functions
	// Text output
	virtual void SetText(const char *pcName, const char *pcText);
	// Progress output
	virtual void SetMaxProgress(const char *pcName, unsigned int uMax);
	virtual void SetProgress(const char *pcName, unsigned int uPercent);
	virtual void Step(const char *pcName);

//#===--- Internal Functions
private:

//#===--- Internal Data
private:
	VecProgress m_oProgressTable;				// Progress name and function table
};

#endif // _VAL_PROGRESSCONTROL_
