//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/BeamElement.cpp
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: BeamElement.cpp,v 1.31 2002/04/04 11:01:31 vap-warren Exp $"

#include "stdafx.h"
#include "BeamElement.h"

#include <strstrea.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CBeamElement

CBeamElement::CBeamElement(CCortonaUtil *poCortona, CNodeSet* poNodeSet) : 
   CElement(poCortona, poNodeSet),
   m_fHeight(1.0f),
   m_fWidth(1.0f),
   m_fFlange(0.1f),
   m_fWeb(0.1f)
{
   m_piNodes[0] = m_piNodes[1] = 0;
   m_pfStresses[0] = m_pfStresses[1] = 0.0f;
   memset(m_ppoField, 0, 3 * sizeof(CCortonaField*));
}

CBeamElement::~CBeamElement() {
   for (int i = 0; i < 3; i++)
      if (m_ppoField[i]) {
         m_ppoField[i]->Release();
         delete m_ppoField[i];
      }
}

// Beginning of BeamElement node
const char pcBeamStart[] = " \
   EXTERNPROTO BeamElement [ \
      eventIn MFColor  set_colours \
      eventIn MFVec3f  set_nodes \
      eventIn SFBool   set_visible \
      field   MFColor  colours \
      field   MFString description \
      field   SFFloat  flange \
      field   SFFloat  height \
      field   SFFloat  web \
      field   SFFloat  width \
      field   MFVec3f  nodes \
      eventOut MFString description_changed \
   ] \
   [ \
      \"";

bool CBeamElement::Display(const char* pcURL) const {
   // Calculate colours
   float pfColours[6];
   CalculateColours(pfColours);
   // Calculate node positions
   float pfNodes[6];
   CalculateNodePositions(pfNodes);
   // If the beam isn't there yet
   if (m_poNodePtr == NULL) {
      // Create a beam and add it to the scene
      // Create string buffer for VRML text
      char pcBuffer[2048];
      memset(pcBuffer,0,2048);
      ostrstream strBeam(pcBuffer,2048);
      // Add the basic BeamElement syntax
      strBeam << pcBeamStart;
      // Add the EXTERNPROTO URL
      if (pcURL != NULL) strBeam << pcURL;
      // Add the rest of the preamble;
      strBeam << "BeamElement.wrl\" ] BeamElement { ";
      // Set the description
      strBeam << " description [\"Beam: " << m_iElement << "\" ";
      strBeam << " \"Group: " << m_iGroup << "\" ";
      strBeam << " \"Temp: " << m_fTemp << "\"] ";
      // Set the size
      strBeam << " height " << m_fHeight;
      strBeam << " width "  << m_fWidth;
      strBeam << " flange " << m_fFlange;
      strBeam << " web "    << m_fWeb;
      // Set colours
      strBeam << " colours [ ";
      for (int c=0; c<6; c++) strBeam << " " << pfColours[c];
      strBeam << " ] ";
      // Setup node positions
      strBeam << " nodes [ ";
      for (int n=0; n<6; n++) strBeam << " " << pfNodes[n];
      strBeam << " ] ";
      // Close the BeamElement
      strBeam << "}";
      // Create VRML nodes from the buffer
      if (m_poNodePtr = m_poCortona->CreateVrmlFromString(pcBuffer)) {
         m_poCortona->AddToScene(*m_poNodePtr);
         // Create the field caches
         m_ppoField[0] = m_poCortona->CreateField("MFVec3f");
         if (m_ppoField[0])
            m_ppoField[0]->SetMFCount(2);
         else
            return false;
         m_ppoField[1] = m_poCortona->CreateField("MFColor");
         if (m_ppoField[1])
            m_ppoField[1]->SetMFCount(2);
         else
            return false;
         m_ppoField[2] = m_poCortona->CreateField("SFBool");
         return ((m_ppoField[0]->GetMFCount() == 2) &&
                 (m_ppoField[1]->GetMFCount() == 2) && 
                  m_ppoField[2]);
         return true;
      }
      else 
         return false;
   }
   else {
      bool bOK = true;
      //#===--- Update node positions
      // Set values
      if (m_ppoField[0]) {
         for (int i=0; i<2 && bOK; i++) {
            bOK  = m_ppoField[0]->SetMFVec3f(i, pfNodes[(i*3)], pfNodes[(i*3)+1], pfNodes[(i*3)+2]);
         }      
         // Send event
         if (bOK && !m_poNodePtr->AssignEventIn("set_nodes",*(m_ppoField[0])))
            bOK = false;
      }

      //#===--- Update colours
      // Set values
      if (m_ppoField[1]) {
         for (int i=0; i<2 && bOK; i++) {
            bOK = m_ppoField[1]->SetMFColor(i, pfColours[(i*3)], pfColours[(i*3)+1], pfColours[(i*3)+2]);
         }      
         // Send event
         if (bOK && !m_poNodePtr->AssignEventIn("set_colours", *(m_ppoField[1])))
            bOK = false;
      }

      // Done
      return bOK;
   }
}

void CBeamElement::SetTemp(float fTemp) const {
   m_fTemp = fTemp;
   // Update description
   if (m_poNodePtr) {
      CCortonaField* poString = m_poNodePtr->GetField("description");
      char pcBuffer[32];
      sprintf(pcBuffer,"Temp: %.2f",m_fTemp);
      poString->SetMFString(2,pcBuffer);
      poString->Release();
      delete poString;
   }
}

void CBeamElement::SetSize(float fHeight, float fWidth, float fFlangeThickness, float fWebThickness) {
   m_fHeight = fHeight;
   m_fWidth = fWidth;
   m_fFlange = fFlangeThickness;
   m_fWeb = fWebThickness;
}

void CBeamElement::SetNodes(const unsigned int* piNodes) {
   memcpy(m_piNodes,piNodes,2*sizeof(int));
   return;
}

void CBeamElement::SetStresses (const float* pfStresses) const {
   memcpy(m_pfStresses,pfStresses,2*sizeof(float));
   return;
}

void CBeamElement::CalculateColours(float* pfColours) const {
   switch (m_oColourScheme) {
   case COLOUR_GROUP:
      {
         for (int i=0; i<2; i++) {
            pfColours[(i*3) + 0] = m_pfColour[0];
            pfColours[(i*3) + 1] = m_pfColour[1];
            pfColours[(i*3) + 2] = m_pfColour[2];
         }
      }
      break;
   case COLOUR_STRESS:
      {
         for (int i=0; i<2; i++) {
            float fStress = (m_pfStresses[i] - m_fMinStress) / (m_fMaxStress - m_fMinStress);
            pfColours[(i*3) + 0] = fStress * 2;
            pfColours[(i*3) + 1] = 2 - (fStress * 2);
            pfColours[(i*3) + 2] = 0;
            // Clip to 0..1
            if (pfColours[(i*3) + 0] > 1) pfColours[(i*3) + 0] = 1;
            if (pfColours[(i*3) + 1] > 1) pfColours[(i*3) + 1] = 1;
         }
      }
      break;
   }
   return;
}

void CBeamElement::CalculateNodePositions(float* pfNodes) const {
   for (int i=0; i<2; i++) {
      const float* pNode = m_poNodeSet->Node(m_piNodes[i]);
      pfNodes[(i*3) + 0] = pNode[0];
      pfNodes[(i*3) + 1] = pNode[1];
      pfNodes[(i*3) + 2] = pNode[2];
   }
   return;
}

bool CBeamElement::SetVisible(bool bVisible) const {
   if (m_poNodePtr && m_ppoField[2]) {
      // Set value
      m_ppoField[2]->SetSFBool(bVisible);
      // Send event
      if (m_poNodePtr->AssignEventIn("set_visible", *(m_ppoField[2])))
         return true;
   }
   return false;
}
