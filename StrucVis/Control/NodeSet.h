//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// NodeSet.h
// 19/03/2002 - James Smith
//
// $Id: NodeSet.h,v 1.2 2002/03/21 18:18:12 vap-james Exp $

#ifndef __NODESET__
#define __NODESET__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNodeSet {
//#===--- Construction/Destruction
public:

   CNodeSet(int iNumNodes);
   // Constructor
   
   ~CNodeSet();
   // Destructor

//#===--- Member Functions

   void Set(float* pfNodePositions);
   // The nodes are overwritten with the passed data.
   // (NumNodes * 3) floats are assumed.
   // This also overwrites current positions.

   const float* Node(unsigned int iIndex) const;
   // Gets a pointer to a particular CURRENT node position.
   
//#===--- Member Variables
protected:

   unsigned int m_iNumNodes;
   // How many nodes are there?

   mutable float* m_pfNodePositions;
   // Node positions

};

#endif // __NODESET__
