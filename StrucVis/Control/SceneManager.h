//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SceneManager.h
// 19/03/2002 - James Smith
//
// $Id: SceneManager.h,v 1.6 2002/03/22 17:04:37 vap-james Exp $

#ifndef __SCENEMANAGER__
#define __SCENEMANAGER__

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
   
   unsigned int NumFrames(void) {return m_oDataMgr.NumFrames();}
   // The number of frames

   void FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength);
   // Gets frame location information
   // Frame offset and length for iFrame are returned in the parameters.

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

   class CGroup {
   public:
      TElementType m_oType;
      float m_fTemperature;
      float m_pfColour[3];
   };
   // Group information type

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

   std::vector<CGroup> m_oGroups;
   // Group information

   CViewpoint m_oViewpoint;
   // The virtual camera

   TColourScheme m_tColourScheme;
   // The current colouring scheme

};

#endif // __SCENEMANAGER__
