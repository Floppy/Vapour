//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// NodeSet.h
// 19/03/2002 - James Smith
//
// $Id: NodeSet.h,v 1.1 2002/03/20 13:29:50 vap-james Exp $

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

   void SetDefault(float* pfNodePositions);
   // The default nodes are overwritten with the passed data.
   // (NumNodes * 3) floats are assumed.
   // This also overwrites current positions.

   void Displace(float* pfDisplacements, float* pfScale = NULL) const;
   // Adds the passed array of displacements onto the default nodes
   // to create a new current node list.
   // Displacements are multiplied by the passed scale factor (an array of 3 floats).
   // (NumNodes * 3) floats are assumed in the displacement list.
   
   const float* Node(unsigned int iIndex) const;
   // Gets a pointer to a particular CURRENT node position.
   
//#===--- Member Variables
protected:

   unsigned int m_iNumNodes;
   // How many nodes are there?

   mutable float* m_pfCurrentNodePositions;
   // Current node positions
   
   float* m_pfDefaultNodePositions;
   // Default node positions

};

#endif // __NODESET__
