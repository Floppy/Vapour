//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.h - 07/06/2000 - Warren Moore
//	Time limit validation class 
//
// $Id: TimeLimit.h,v 1.3 2000/07/15 10:40:54 waz Exp $
//

#ifndef _VAL_TIMELIMIT_
#define _VAL_TIMELIMIT_

#pragma once

//#===--- Defines

//#===--- Data types

///////////////
// CTimeLimit

class CTimeLimit {
//#===--- External functions
public:

// Constructor/destructor
	CTimeLimit();		
	~CTimeLimit();

	bool Valid();

//#===--- Internal Functions
private:

//#===--- Internal Data
private:
};

#endif // _VAL_TIMELIMIT_
