//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management"
//! userlevel =  Normal
//! file      = "Control/NodeSet.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: NodeSet.h,v 1.10 2002/04/04 11:01:36 vap-warren Exp $"

#ifndef __VTSTRUCVIS_NODESET__
#define __VTSTRUCVIS_NODESET__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//: A set of node positions

class CNodeSet {

public:

   //:-------------------------
   //: Construction/Destruction

   CNodeSet();
   //: Default Constructor
   // Creates a node set with no allocated node storage.
   
   CNodeSet(int iNumNodes);
   //: Constructor
   //!param: iNumNodes - the number of nodes to allocate storage for.
   
   ~CNodeSet();
   //: Destructor

   //:-----------------
   //: Member Functions

   void SetSize(int iNumNodes);
   //: Set the size of the node set
   // All existing data will be dumped.
   //!param: iNumNodes - the number of nodes to allocate storage for.

   void SetDefault(const float* pfNodePositions);
   //: Set default node positions
   // Both default and current nodes are overwritten with the passed data.
   //!param: pfNodePositions - an array of (NumNodes * 3) floats, in order X Y Z.

   void Displace(const float* pfDisplacements) const;
   //: Set node displacements
   // Adds the passed array of displacements onto the default nodes to create a new current node list.
   //!param: pfDisplacements - an array of (NumNodes * 3) floats, in order X Y Z.
   
   void SetScaleFactor(float fX, float fY, float fZ) const;
   //: Sets the displacement scale factor.
   // This is used to multiply the displacements during the Displace() function
   //!param: fX - X scale factor
   //!param: fY - Y scale factor
   //!param: fZ - Z scale factor

   const float* Node(unsigned int iIndex) const;
   //: Gets a pointer to a particular node position.
   //!param: iIndex - an integer between 0 and NumNodes-1.
   
protected:

   //:-----------------
   //: Member Variables

   unsigned int m_iNumNodes;
   //: How many nodes are there?

   mutable float* m_pfCurrentNodePositions;
   //: Current node positions
   
   float* m_pfDefaultNodePositions;
   //: Default node positions

   mutable float m_pfScale[3];
   //: Displacement scale factor

};

#endif // __VTSTRUCVIS_NODESET__
