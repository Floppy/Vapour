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
//! rcsid     = "$Id: NodeSet.h,v 1.11 2002/04/23 11:32:43 vap-james Exp $"

#ifndef __VTSTRUCVIS_NODESET__
#define __VTSTRUCVIS_NODESET__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNodeSet;

#include "CortonaUtil.h"
#include "Element.h"

//: A set of node positions

class CNodeSet {

public:

   //:-------------------------
   //: Construction/Destruction

   CNodeSet(CCortonaUtil *pCortona);
   //: Constructor
   // Creates a node set with no allocated node storage.
   //!param: poCortona - A pointer to a valid CortonaUtil object, used to communicate with the browser.
   
   CNodeSet(int iNumNodes, CCortonaUtil *pCortona);
   //: Constructor
   //!param: iNumNodes - the number of nodes to allocate storage for.
   //!param: poCortona - A pointer to a valid CortonaUtil object, used to communicate with the browser.
   
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

   bool Displace(const float* pfDisplacements) const;
   //: Set node displacements
   // Adds the passed array of displacements onto the default nodes to create a new current node list.
   //!param: pfDisplacements - an array of (NumNodes * 3) floats, in order X Y Z.
   //!return: true if VRML update is successful, false otherwise.
   
   void SetScaleFactor(float fX, float fY, float fZ) const;
   //: Sets the displacement scale factor.
   // This is used to multiply the displacements during the Displace() function
   //!param: fX - X scale factor
   //!param: fY - Y scale factor
   //!param: fZ - Z scale factor

   const float* Node(unsigned int iIndex) const;
   //: Gets a pointer to a particular node position.
   //!param: iIndex - an integer between 0 and NumNodes-1.
   
   bool Connect(const CElement* pElement);
   //: Connect the element to the node set
   // This is used to set up connections inside the VRML scene. 
   // Connects nodes_changed eventOut of this to set_nodes on pElement
   //!param: pElement - the element to connect to.
   //!return: true if successful, false otherwise.

   bool Display(const char* pcURL);
   //: Adds the node set to the scene.
   //!param: pcURL - a URL for the directory containing NodeSet.wrl (i.e. excluding the filename).
   //!return: true if successful, false otherwise.

   bool Redisplay(void);
   //: Redisplays the nodeset in the scene.
   //!return: true if successful, false otherwise.

   bool Update(void) const;
   //: Updates VRML data
   //!return: true if successful

protected:

   //:-----------------
   //: Member Variables

   CCortonaUtil* m_poCortona;
   //: Cortona Utility interface

   mutable CCortonaNode *m_poNodePtr;
   //: Pointer to the node in the VRML world;   

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
