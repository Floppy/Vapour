//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SceneManager.h
// 19/03/2002 - James Smith
//
// $Id: SceneManager.h,v 1.10 2002/03/24 23:00:27 vap-james Exp $

#ifndef __VTSTRUCVIS_SCENEMANAGER__
#define __VTSTRUCVIS_SCENEMANAGER__

#include <vector>
#include "Element.h"
#include "Viewpoint.h"
#include "DataManager.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSceneManager {
//#===--- Construction/Destruction
public:

   CSceneManager(CCortonaUtil *poCortona);
   // Constructor
   
   ~CSceneManager();
   // Destructor

//#===--- Member Functions

   bool Setup(const unsigned char* pcData, unsigned int iLength);
   // Load initial parts of a dataset
   // Any old dataset is destroyed
   // When setup is complete (i.e. enough data has been sent in, the data is loaded 
   // and Setup() will return true.
   
   void Empty(void);
   // Deletes all information from the scene

   unsigned int NumFrames(void) {return m_oDataMgr.NumFrames();}
   // The number of frames

   bool FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength);
   // Gets frame location information
   // Frame offset and length for iFrame are returned in the parameters.
   // Returns false if an invalid frame is requested

   unsigned int NumGroups(void);
   // How many groups are there?
   
   unsigned int NumElements(unsigned int iGroup = 0);
   // How many elements are there?
   // If a group index is supplied, the answer is the number of elements in that group.

   TElementType GroupType(unsigned int iGroup);
   // What type is the group?

   float Temperature(unsigned int iGroup);
   // Returns the current temperature of the requested group

   void SetVisibility(unsigned int iGroup, bool bVisible);
   // Sets visibility of a group

   void SetScaleFactor(float fX, float fY, float fZ);
   // Sets displacement scale factor

   void SetViewpoint(float pfPosition[3], float pfRotation[4]);
   // Set the camera position

   void SetColourScheme(TColourScheme oColour);
   // Set the colouring scheme

   bool ShowFrame(const unsigned char* pcData, unsigned int iLength);
   // Show the frame represented by the passed data
   // Returns true if display is successful

private:

   void Load(void);
   // Load initial data from data manager

   void Update(void);
   // Updates the display of all elements

//#===--- Member Variables
protected:

   CCortonaUtil *m_poCortona;
   // Cortona Utility class

   CNodeSet m_oNodeSet;
   // The set of nodes
   
   CDataManager m_oDataMgr;
   // The data manager

   std::vector<CElement*> m_oElements;
   // Element list

   CViewpoint m_oViewpoint;
   // The virtual camera

   TColourScheme m_tColourScheme;
   // The current colouring scheme

   bool m_bLoading;
   // Is a load in progress?

};

#endif // __VTSTRUCVIS_SCENEMANAGER__
