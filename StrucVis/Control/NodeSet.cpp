//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// NodeSet.cpp
// 19/03/2002 - James Smith
//
// $Id: NodeSet.cpp,v 1.8 2002/03/27 15:08:40 vap-james Exp $

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

CNodeSet::CNodeSet() :
   m_iNumNodes(0),
   m_pfCurrentNodePositions(NULL),
   m_pfDefaultNodePositions(NULL)
{
}

CNodeSet::CNodeSet(int iNumNodes) :
   m_iNumNodes(0),
   m_pfCurrentNodePositions(NULL),
   m_pfDefaultNodePositions(NULL)
{
   SetSize(iNumNodes);
}

CNodeSet::~CNodeSet() {
   if (m_pfCurrentNodePositions) delete [] m_pfCurrentNodePositions;
   if (m_pfDefaultNodePositions) delete [] m_pfDefaultNodePositions;
}

void CNodeSet::SetSize(int iNumNodes) {
   // Store number of nodes
   m_iNumNodes = iNumNodes;
   // Dump old data if present
   if (m_pfCurrentNodePositions) delete [] m_pfCurrentNodePositions;
   if (m_pfDefaultNodePositions) delete [] m_pfDefaultNodePositions;
   // Allocate new memory
   m_pfCurrentNodePositions = new float[iNumNodes * 3];
   m_pfDefaultNodePositions = new float[iNumNodes * 3];
   // Done
   return;
}

void CNodeSet::SetDefault(const float* pfNodePositions) {
   memcpy(m_pfDefaultNodePositions,pfNodePositions,m_iNumNodes*3*sizeof(float));
   memcpy(m_pfCurrentNodePositions,pfNodePositions,m_iNumNodes*3*sizeof(float));
}

void CNodeSet::Displace(const float* pfDisplacements) const {
   // Check input data
   if (pfDisplacements == NULL) return;
   // Carry on
   float* pfDefault = m_pfDefaultNodePositions;
   float* pfCurrent = m_pfCurrentNodePositions;
   const float* pfDisplacement = pfDisplacements;
   for (unsigned int n=0; n<m_iNumNodes; n++) {
      for (unsigned int c=0; c<3; c++) {
         *pfCurrent = *pfDefault + (m_pfScale[c] * *pfDisplacement);
         pfCurrent++;
         pfDefault++;
         pfDisplacement++;
      }
   }
   return;
}

void CNodeSet::SetScaleFactor(float fX, float fY, float fZ) const {
   m_pfScale[0] = fX;
   m_pfScale[1] = fY;
   m_pfScale[2] = fZ;
}

const float* CNodeSet::Node(unsigned int iIndex) const {
   if (iIndex == 0) return NULL;
   return m_pfCurrentNodePositions + ((iIndex-1)*3);
}
   
