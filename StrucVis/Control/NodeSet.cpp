//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// NodeSet.cpp
// 19/03/2002 - James Smith
//
// $Id: NodeSet.cpp,v 1.3 2002/03/21 18:18:12 vap-james Exp $

#include "stdafx.h"
#include "NodeSet.h"

#include <memory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////
// CNodeSet

CNodeSet::CNodeSet(int iNumNodes) :
   m_iNumNodes(iNumNodes),
   m_pfNodePositions(NULL)
{
   m_pfNodePositions = new float[iNumNodes * 3];
}

CNodeSet::~CNodeSet() {
   if (m_pfNodePositions) delete [] m_pfNodePositions;
}

void CNodeSet::Set(float* pfNodePositions) {
   memcpy(m_pfNodePositions,pfNodePositions,m_iNumNodes*3*sizeof(float));
}

const float* CNodeSet::Node(unsigned int iIndex) const {
   if (iIndex == 0) return NULL;
   return m_pfNodePositions + ((iIndex-1)*3);
}
   
