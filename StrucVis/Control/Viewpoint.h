//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/Viewpoint.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: Viewpoint.h,v 1.12 2002/04/04 00:03:55 vap-james Exp $"

#ifndef __VTSTRUCVIS_VIEWPOINT__
#define __VTSTRUCVIS_VIEWPOINT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaUtil.h"
#include "Element.h"

//: An interface to the VRML scene's Viewpoint
// This class provides an interface to the PROTO object contained HUDView.wrl.

class CViewpoint {

public:

   //:-------------------------
   //: Construction/Destruction

   CViewpoint(CCortonaUtil *pCortona) :
      m_poCortona(pCortona),
      m_poNodePtr(NULL),
      m_bAnimate(false)
   {}
   //: Constructor
   //!param: poCortona - A pointer to a valid CortonaUtil object, used to communicate with the browser.

   
   ~CViewpoint() {
      if (m_poNodePtr) {
         m_poNodePtr->Release();
         delete m_poNodePtr;
      }
   }
   //: Destructor

   //:-----------------
   //: Member Functions

   bool Set(const char* pcURL, float* pfPosition = NULL, float* pfRotation = NULL);
   //: Set viewpoint position.
   // Method for generating parameters for this function are outlined in the description of the HUDView node in the "VRML Nodes" section of this documentation.
   //!param: pcURL - a URL for the directory containing HUDView.wrl (i.e. excluding the filename).
   //!param: pfPosition - an array of 3 floats defining the viewpoint position. If this is NULL, 0 0 10 is used.
   //!param: pfRotation - an array of 4 floats defining the viewpoint orientation, in axis-angle notation. If this is NULL, 0 1 0 0 is used.
   //!param: return - true if successful, false otherwise.

   void Animate(bool bAnimate);
   //: Animate smoothly betwen viewpoints?

   bool Connect(const CElement* pElement);
   //: Connect the element to the viewpoint
   // This is used to set up connections inside the VRML scene. 
   // Connects description_changed eventOut of pElement to set_description on the HUDView node.
   //!param: pElement - the element to connect from.
   //!param: return - true if successful, false otherwise.

   bool Redisplay(void);
   //: Redisplays the viewpoint in the scene.
   // Moves the camera back to the viewpoint position.
   //!param: return - true if successful, false otherwise.

   bool GetCurrentPosition(float& fX, float& fY, float& fZ);
   //: Gets the current viewpoint position
   //!param: fX - the X value is placed in here
   //!param: fY - the Y value is placed in here
   //!param: fZ - the Z value is placed in here
   //!param: return - true if successful, false otherwise.
   
   const char* GetCurrentPosition(void);
   //: Gets the current viewpoint position as a string
   // It is the client's responsibility to delete the memory returned from this function, using delete [].
   //!param: return - NULL on failure, otherwise a pointer to a string.
   
   bool GetCurrentOrientation(float& fX, float& fY, float& fZ, float& fA);
   //: Gets the current viewpoint orientation
   //!param: fX - the X value is placed in here
   //!param: fY - the Y value is placed in here
   //!param: fZ - the Z value is placed in here
   //!param: fA - the angle value is placed in here
   //!param: return - true if successful, false otherwise.

   const char* GetCurrentOrientation(void);
   //: Gets the current viewpoint orientation as a string
   // It is the client's responsibility to delete the memory returned from this function, using delete [].
   //!param: return - NULL on failure, otherwise a pointer to a string.

protected:

   //:-----------------
   //: Member Variables

   CCortonaUtil* m_poCortona;
   //: Cortona Utility interface

   mutable CCortonaNode *m_poNodePtr;
   //: Pointer to the node in the VRML world;   

   bool m_bAnimate;
   //: Smooth animation?

};

#endif // __VTSTRUCVIS_VIEWPOINT__
