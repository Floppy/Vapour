//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SceneManager.h
// 19/03/2002 - James Smith
//
// $Id: SceneManager.h,v 1.2 2002/03/21 23:36:45 vap-james Exp $

#ifndef __SCENEMANAGER__
#define __SCENEMANAGER__

#include <vector>
#include "Element.h"
#include "Viewpoint.h"

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

   void Load(void);
   // Load a dataset
   // The old dataset is destroyed
   
   unsigned int NumFrames(void) {return m_iNumFrames;}
   // The number of frames

   void FrameFirst(void);
   // Skips to the first frame

   void FramePrev(void);
   // Displays the previous frame

   void FrameNext(void);
   // Displays the next frame

   void FrameLast(void);
   // Skips to the last frame

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

private:

   class CGroup {
   public:
      TElementType m_oType;
      float m_fTemperature;
   };
   // Group information type

   void ShowFrame(void);
   // Show current frame

   void Update(void);
   // Updates the display of all elements

//#===--- Member Variables
protected:

   CCortonaUtil *m_poCortona;
   // Cortona Utility class

   CNodeSet m_oNodeSet;
   // The set of nodes
   
   std::vector<CElement*> m_oElements;
   // Element list

   std::vector<CGroup> m_oGroups;
   // Group information

   CViewpoint m_oViewpoint;
   // The virtual camera

   unsigned int m_iNumFrames;
   // The number of frames in the sequence

   unsigned int m_iCurrentFrame;
   // The frame we're currently on

};

#endif // __SCENEMANAGER__
