//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// Viewpoint.cpp
// 19/03/2002 - James Smith
//
// $Id: Viewpoint.cpp,v 1.5 2002/03/24 13:52:01 vap-warren Exp $

#include "stdafx.h"
#include "Viewpoint.h"

#include <strstrea.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CViewpoint

// Beginning of HUDView node
const char pcViewStart[] = " \
   EXTERNPROTO HUDView [ \
      eventIn      SFBool set_bind \
      eventIn      MFString set_description \
      exposedField SFVec3f position \
      exposedField SFRotation orientation \
      exposedField SFVec3f textPosition \
      exposedField SFBool jump \
   ] \
   [ \
      \"HUDView.wrl\" \
      \"file://D:\\Vapour\\Dev\\Src\\Research\\CortonaBase\\HUDView.wrl\" \
      \"file://D:\\James\\vapour\\dev.local\\src\\Research\\CortonaBase\\HUDView.wrl\" \
   ] \
   HUDView { \
";

bool CViewpoint::Set(float* pfPosition, float* pfRotation) {
   // If the node isn't there yet
   if (m_poNodePtr == NULL) {

      // Create a viewpoint node and add it to the scene
      // Create string buffer for VRML text
      char pcBuffer[2048];
      memset(pcBuffer,0, 2048);
      ostrstream strView(pcBuffer, 2048);
      // Add the basic Viewpoint syntax
      strView << pcViewStart;
      // Set animation mode      
      if (m_bAnimate) strView << " jump FALSE ";
      else strView << " jump TRUE ";
      // Set position
      if (pfPosition != NULL) {
         strView << " position ";
         for (int i=0; i<3; i++) strView << pfPosition[i] << " ";
      }
      // Set rotation
      if (pfRotation != NULL) {
         strView << " orientation ";
         for (int i=0; i<4; i++) strView << pfRotation[i] << " ";
      }
      // Close the Viewpoint
      strView << "}";
      // Create VRML nodes from the buffer
      if (m_poNodePtr = m_poCortona->CreateVrmlFromString(pcBuffer)) {
         m_poCortona->AddToScene(*m_poNodePtr);
      }
      // Bind node to move the camera
      // Create boolean field
      CCortonaField* poSFBool = m_poCortona->CreateField("SFBool");
      if (poSFBool==NULL) return false;
      // Set value
      poSFBool->SetSFBool(true);
      // Send event
      if (!m_poNodePtr->AssignEventIn("set_bind",*poSFBool)) return false;
      // Done!
      poSFBool->Release();
      delete poSFBool;
      return true;
   }
   else {
      // Set animation mode
      CCortonaField* poAnimation = m_poNodePtr->GetField("jump");
      poAnimation->SetSFBool(m_bAnimate);
      poAnimation->Release();
      delete poAnimation;
      // Set position
      if (pfPosition) {
         CCortonaField* poPosition = m_poNodePtr->GetField("position");
         poPosition->SetSFVec3f(pfPosition[0],pfPosition[1],pfPosition[2]);
         poPosition->Release();
         delete poPosition;
      }
      // Set orientation
      if (pfRotation) {
         CCortonaField* poRotation = m_poNodePtr->GetField("orientation");
         poRotation->SetSFRotation(pfRotation[0],pfRotation[1],pfRotation[2],pfRotation[3]);
         poRotation->Release();
         delete poRotation;
      }
      // Rebind the viewpoint to move the camera
      CCortonaField* poSFBool = m_poCortona->CreateField("SFBool");
      if (poSFBool==NULL) return false;
      // Unbind
      poSFBool->SetSFBool(false);
      if (!m_poNodePtr->AssignEventIn("set_bind",*poSFBool)) return false;
      // Rebind
      poSFBool->SetSFBool(true);
      if (!m_poNodePtr->AssignEventIn("set_bind",*poSFBool)) return false;
      // Done!
      poSFBool->Release();
      delete poSFBool;
      return true;
   }
}

void CViewpoint::Animate(bool bAnimate) {
   m_bAnimate = bAnimate;
}

bool CViewpoint::Connect(const CElement* pElement) {
   m_poCortona->AddRoute(*(pElement->m_poNodePtr), "description_changed", *m_poNodePtr, "set_description");
   return false;
}
