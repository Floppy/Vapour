//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.h - 07/06/2000 - Warren Moore
//	Time limit validation class 
//
// $Id: TimeLimit.h,v 1.1 2000/06/16 21:57:43 waz Exp $
//

#ifndef _TIMELIMIT_
#define _TIMELIMIT_

#pragma once

//#===--- Defines
#define TL_FILENAME		"sim.dat"

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

#endif // _TIMELIMIT_
