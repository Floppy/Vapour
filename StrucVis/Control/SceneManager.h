//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.VRML"
//! userlevel =  Normal
//! file      = "Control/SceneManager.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: SceneManager.h,v 1.19 2002/04/25 15:44:05 vap-warren Exp $"

#ifndef __VTSTRUCVIS_SCENEMANAGER__
#define __VTSTRUCVIS_SCENEMANAGER__

#include <vector>
#include "Element.h"
#include "Viewpoint.h"
#include "DataManager.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//: The Scene Manager
// This class provides overall scene management capabilities.
// To load data, give data to Setup() until it returns true. Then request frame information from FrameInfo(). This gives
// a file offset and a length. The data in this section of the file should be passed to ShowFrame() to display it.

class CSceneManager {

public:

   //:-------------------------
   //: Construction/Destruction

   CSceneManager(CCortonaUtil *poCortona);
   //: Constructor
   
   ~CSceneManager();
   //: Destructor

   //:---------
   //: Data I/O

   bool Setup(const unsigned char* pcData, unsigned int iLength);
   //: Accepts setup data
   // NO other data will be available until Setup returns true.
   //!param: pcData - raw input data
   //!param: iLength - length of pcData
   //!return: true if setup is complete, false if more data is required.
   
   bool FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength);
   //: Frame location information
   //!param: iFrame - the requested frame - between 0 and NumFrames-1.
   //!param: iOffset - the offset of the frame data is placed here.
   //!param: iLength - the length of the frame data is placed here.
   //!return: false if an invalid frame is requested, true otherwise.

   bool ShowFrame(const unsigned char* pcData, unsigned int iLength);
   //: Loads frame data from the passed memory chunk
   //!param: pcData - raw input data
   //!param: iLength - length of pcData
   // Returns true if load was successful, false otherwise

   void Empty(void);
   //: Deletes all information from the scene

   //:------------
   //: Data Access

   unsigned int NumFrames(void) {return m_oDataMgr.NumFrames();}
   //: The number of frames in the sequence

   unsigned int NumGroups(void);
   //: The number of groups (temperature profiles).
   
   unsigned int NumElements(unsigned int iGroup = 0);
   //: The number of elements.
   //!param: iGroup - If this is not 0, the returned value is the number of elements in that group.

   TElementType GroupType(unsigned int iGroup);
   //: The type of elements that the group contains
   //!param: iGroup - a value between 1 and NumGroups

   float Temperature(unsigned int iGroup);
   //: The current temperature of the specified group
   //!param: iGroup - a value between 1 and NumGroups

   void SetVisibility(unsigned int iGroup, bool bVisible);
   //: Sets visibility of a group
   //!param: iGroup - a value between 1 and NumGroups
   //!param: bVisible - should the group be visible?

   void SetScaleFactor(float fX, float fY, float fZ);
   //: Sets the displacement scale factor.
   // This is used to multiply the displacements during the Displace() function
   //!param: fX - X scale factor
   //!param: fY - Y scale factor
   //!param: fZ - Z scale factor

   void SetViewpoint(float pfPosition[3], float pfRotation[4]);
   //: Set viewpoint position.
   // Method for generating parameters for this function are outlined in the description of the HUDView node in the "VRML Nodes" section of this documentation.
   //!param: pfPosition - an array of 3 floats defining the viewpoint position.
   //!param: pfRotation - an array of 4 floats defining the viewpoint orientation, in axis-angle notation.

   void SetColourScheme(TColourScheme oColour);
   //: Set which colour scheme to use.

   void SetBaseURL(const char* pcURL);
   //: Sets the base directory for the auxiliary VRML files
   //!param: pcURL - a string containing the URL of the directory.

   const char* GetCurrentPosition(void) {return m_oViewpoint.GetCurrentPosition();}
   //: Gets the current viewpoint position as a string
   // It is the client's responsibility to delete the memory returned from this function, using delete [].
   //!return: NULL on failure, otherwise a pointer to a string.
   
   const char* GetCurrentOrientation(void)  {return m_oViewpoint.GetCurrentOrientation();}
   //: Gets the current viewpoint orientation as a string
   // It is the client's responsibility to delete the memory returned from this function, using delete [].
   //!return: NULL on failure, otherwise a pointer to a string.

private:

   void Load(void);
   //: Load initial data from data manager

   void Update(void);
   //: Updates the display of all elements

protected:

   //:-----------------
   //: Member Variables

   CCortonaUtil *m_poCortona;
   //: Cortona Utility class

   CNodeSet m_oNodeSet;
   //: The set of nodes
   
   CDataManager m_oDataMgr;
   //: The data manager

   std::vector<CElement*> m_oElements;
   //: Element list

   CViewpoint m_oViewpoint;
   //: The virtual camera

   TColourScheme m_tColourScheme;
   //: The current colouring scheme

   bool m_bLoading;
   //: Is a load in progress?

   char* m_pcURL;
   //: Base URL

};

#endif // __VTSTRUCVIS_SCENEMANAGER__
