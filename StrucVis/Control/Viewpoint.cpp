//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/Viewpoint.cpp"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: Viewpoint.cpp,v 1.12 2002/04/04 11:01:39 vap-warren Exp $"

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
      eventOut     SFVec3f currentPosition \
      eventOut     SFRotation currentOrientation \
   ] \
   [ \
      \"";

bool CViewpoint::Set(const char* pcURL, float* pfPosition, float* pfRotation) {
   // If the node isn't there yet
   if (m_poNodePtr == NULL) {

      // Create a viewpoint node and add it to the scene
      // Create string buffer for VRML text
      char pcBuffer[2048];
      memset(pcBuffer,0, 2048);
      ostrstream strView(pcBuffer, 2048);
      // Add the basic Viewpoint syntax
      strView << pcViewStart;
      // Add the EXTERNPROTO URL
      if (pcURL != NULL) strView << pcURL;
      // Add the rest of the preamble;
      strView << "HUDView.wrl\" ] HUDView { ";
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
      if (m_poNodePtr = m_poCortona->CreateVrmlFromString(pcBuffer))
         return Redisplay();
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
   return false;
}

void CViewpoint::Animate(bool bAnimate) {
   m_bAnimate = bAnimate;
}

bool CViewpoint::Connect(const CElement* pElement) {
   m_poCortona->AddRoute(*(pElement->m_poNodePtr), "description_changed", *m_poNodePtr, "set_description");
   return false;
}

bool CViewpoint::Redisplay(void) {
   if (!m_poCortona || !m_poNodePtr) 
      return false;

   // Add nodes to scene
   if (!m_poCortona->AddToScene(*m_poNodePtr)) return false;

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

bool CViewpoint::GetCurrentPosition(float& fX, float& fY, float& fZ) {  
   CCortonaField* poField = m_poNodePtr->GetField("currentPosition");
   if (poField == NULL) return false;
   if (!poField->GetSFVec3f(fX,fY,fZ)) return false;
   poField->Release();
   delete poField;
   return true;
}

const char* CViewpoint::GetCurrentPosition(void) {
   float pfData[3];
   // Get position
   if (!GetCurrentPosition(pfData[0],pfData[1],pfData[2]))
      return NULL;
   // Create string
   char* pcString = new char[50];
   sprintf(pcString,"%.3g %.3g %.3g",pfData[0],pfData[1],pfData[2]);
   // Done
   return pcString;
}

bool CViewpoint::GetCurrentOrientation(float& fX, float& fY, float& fZ, float& fA) {
   CCortonaField* poField = m_poNodePtr->GetField("currentOrientation");
   if (poField == NULL) return false;
   if (!poField->GetSFRotation(fX,fY,fZ,fA)) return false;
   poField->Release();
   delete poField;
   return true;
}

const char* CViewpoint::GetCurrentOrientation(void) {
   float pfData[4];
   // Get position
   if (!GetCurrentOrientation(pfData[0],pfData[1],pfData[2],pfData[3]))
      return NULL;
   // Create string
   char* pcString = new char[77];
   sprintf(pcString,"%.3g %.3g %.3g %.3g",pfData[0],pfData[1],pfData[2],pfData[3]);
   // Done
   return pcString;
}

