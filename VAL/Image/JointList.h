//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// JointList.h - 28/12/1999 - Warren Moore
//	Octree colour cube joint manager declaration
//
// $Id: JointList.h,v 1.2 2000/06/17 10:42:18 waz Exp $
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
	typedef struct sJointNode {
		CColourOctree *m_pOctant;
		sJointNode *m_pNext;

		sJointNode() {
			m_pOctant = NULL;
			m_pNext = NULL;
		}
	} sJoint;

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
	sJoint *m_pList;
	int m_iEntries;
};

//#===--- Inline Functions

inline bool CJointList::Empty() const {
	return (m_pList == NULL);
} // Empty

#endif // _VAL_JOINTLIST_
