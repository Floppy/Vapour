//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// JointList.cpp - 28/12/1999 - Warren Moore
//	Octree colour cube joint manager
//
// $Id: JointList.cpp,v 1.1 2000/06/16 21:59:47 waz Exp $
//

#include "stdafx.h"

#include "JointList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////
// CJointList

CJointList::CJointList() {
	m_pList = NULL;
	m_iEntries = 0;
} // Contructor

CJointList::~CJointList() {
	if (m_pList)
		Clear();
} // Destructor

void CJointList::AddOctant(CColourOctree &oOctree) {
/*
	sJoint *pTest = m_pList;
	if (pTest) {
		while (pTest) {
			if (&oOctree == pTest->m_pOctant) {
//				TRACE("Multiple entry\n");
				return;
			}
			pTest = pTest->m_pNext;
		}
	}
*/

// Add entry
	sJoint *pJoint = new sJoint;
	if (!pJoint)
		return;

	pJoint->m_pOctant = &oOctree;
	pJoint->m_pNext = m_pList;
	m_pList = pJoint;
	m_iEntries++;
} // AddOctant

CColourOctree &CJointList::FindBest(int &iSum) {
	ASSERT(m_pList);

	sJoint *pJoint = m_pList;
	int iHits = 0;
// Set start point
	int iLowest = pJoint->m_pOctant->SumNodes();
	CColourOctree *pOctant = pJoint->m_pOctant;
	sJoint *pBest = NULL, *pLast = NULL;
// Go through entries
	while (pJoint->m_pNext != NULL) {
		pLast = pJoint;
		pJoint = pJoint->m_pNext;
		iHits = pJoint->m_pOctant->SumNodes();
	// If lower hit count, store octant
		if (iHits < iLowest) {
			iLowest = iHits;
			pOctant = pJoint->m_pOctant;
			pBest = pLast;
		}
	} 
// Remove from list
	if (pBest) {
		pJoint = pBest->m_pNext;
		pBest->m_pNext = pJoint->m_pNext;
		delete pJoint;
	}
	else {
		pJoint = m_pList;
		m_pList = pJoint->m_pNext;
		delete pJoint;
	}

	m_iEntries--;
	iSum = iLowest;
	return *pOctant;
} // FindBest

void CJointList::Clear() {
	if (!m_pList)
		return;

	sJoint *pJoint = m_pList;
	sJoint *pNext = pJoint->m_pNext;
	while (pJoint) {
		pNext = pJoint->m_pNext;
		delete pJoint;
		pJoint = pNext;
	}

	m_pList = NULL;
	m_iEntries = 0;
} // Clear
