//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// NodeSet.cpp
// 19/03/2002 - James Smith
//
// $Id: NodeSet.cpp,v 1.2 2002/03/20 14:51:30 vap-james Exp $

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
   m_pfCurrentNodePositions(NULL),
   m_pfDefaultNodePositions(NULL)
{
   m_pfCurrentNodePositions = new float[iNumNodes * 3];
   m_pfDefaultNodePositions = new float[iNumNodes * 3];
}

CNodeSet::~CNodeSet() {
   if (m_pfCurrentNodePositions) delete [] m_pfCurrentNodePositions;
   if (m_pfDefaultNodePositions) delete [] m_pfDefaultNodePositions;
}

void CNodeSet::SetDefault(float* pfNodePositions) {
   memcpy(m_pfDefaultNodePositions,pfNodePositions,m_iNumNodes*3*sizeof(float));
   memcpy(m_pfCurrentNodePositions,pfNodePositions,m_iNumNodes*3*sizeof(float));
}

void CNodeSet::Displace(float* pfDisplacements, float* pfScale) const {
   float* pfDefault = m_pfDefaultNodePositions;
   float* pfCurrent = m_pfCurrentNodePositions;
   float* pfDisplacement = pfDisplacements;
   if (pfScale) {
      for (unsigned int n=0; n<m_iNumNodes; n++) {
         for (unsigned int c=0; c<3; c++) {
            *pfCurrent = *pfDefault + (pfScale[c] * *pfDisplacement);
            pfCurrent++;
            pfDefault++;
            pfDisplacement++;
         }
      }
   }
   else {
      for (unsigned int n=0; n<m_iNumNodes; n++) {
         for (unsigned int c=0; c<3; c++) {
            *pfCurrent = *pfDefault + *pfDisplacement;
            pfCurrent++;
            pfDefault++;
            pfDisplacement++;
         }
      }
   }
   return;
}

const float* CNodeSet::Node(unsigned int iIndex) const {
   if (iIndex == 0) return NULL;
   return m_pfCurrentNodePositions + ((iIndex-1)*3);
}
   
