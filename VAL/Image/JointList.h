//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// JointList.h - 28/12/1999 - Warren Moore
//	Octree colour cube joint manager declaration
//
// $Id: JointList.h,v 1.3 2000/07/22 23:23:07 waz Exp $
//

#ifndef _VAL_JOINTLIST_
#define _VAL_JOINTLIST_

#pragma once

#include "ColourOctree.h"

///////////////
// CJointList

class CJointList {

//#===--- Internal Structures
private:
	typedef struct SJointNode {
		CColourOctree *m_pOctant;
		SJointNode *m_pNext;

		SJointNode() {
			m_pOctant = NULL;
			m_pNext = NULL;
		}
	} SJoint;

//#===--- Functions
public:
	CJointList();
	~CJointList();

	void AddOctant(CColourOctree &oOctree);
	CColourOctree &FindBest(int &iSum);
	void Clear();
	bool Empty() const;

//#===--- Internal Functions

//#===--- Internal Data
protected:
	SJoint *m_pList;
	int m_iEntries;
};

//#===--- Inline Functions

inline bool CJointList::Empty() const {
	return (m_pList == NULL);
} // Empty

#endif // _VAL_JOINTLIST_
