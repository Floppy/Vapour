//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.Scene Management"
//! userlevel =  Normal
//! file      = "Control/NodeSet.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: NodeSet.h,v 1.8 2002/04/03 15:57:10 vap-warren Exp $"

#ifndef __VTSTRUCVIS_NODESET__
#define __VTSTRUCVIS_NODESET__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNodeSet {
//#===--- Construction/Destruction
public:

   CNodeSet();
   // Constructor
   
   CNodeSet(int iNumNodes);
   // Constructor
   
   ~CNodeSet();
   // Destructor

//#===--- Member Functions

   void SetSize(int iNumNodes);
   // Set the size of the node set
   // All data will be dumped.

   void SetDefault(const float* pfNodePositions);
   // The default nodes are overwritten with the passed data.
   // (NumNodes * 3) floats are assumed.
   // This also overwrites current positions.

   void Displace(const float* pfDisplacements) const;
   // Adds the passed array of displacements onto the default nodes
   // to create a new current node list.
   // Displacements are multiplied by the scale factor.
   // (NumNodes * 3) floats are assumed in the displacement list.
   
   void SetScaleFactor(float fX, float fY, float fZ) const;
   // Sets the displacement scale factor.

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

   mutable float m_pfScale[3];
   // Displacement scale factor

};

#endif // __VTSTRUCVIS_NODESET__
