//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// BeamElement.cpp
// 19/03/2002 - James Smith
//
// $Id: BeamElement.cpp,v 1.6 2002/03/20 22:14:18 vap-james Exp $

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

CBeamElement::CBeamElement(CCortonaUtil *pCortona, CNodeSet* pNodeSet) : 
   CElement(pCortona,pNodeSet),
   m_fHeight(1.0f),
   m_fWidth(1.0f),
   m_fFlange(0.1f),
   m_fWeb(0.1f)
{
   m_piNodes[0] = m_piNodes[1] = 0;
   m_pfStresses[0] = m_pfStresses[1] = 0.0f;
   return;
}

// Beginning of BeamElement node
const char pcBeamStart[] = " \
   EXTERNPROTO BeamElement [ \
      eventIn MFColor set_colours \
      eventIn MFVec3f set_nodes \
      eventIn SFBool  set_visible \
      field MFColor colours \
      field SFFloat flange \
      field SFFloat height \
      field SFFloat web \
      field SFFloat width \
      field MFVec3f nodes \
   ] \
   [ \
      \"file://D:\\Vapour\\Dev\\Src\\Research\\CortonaBase\\BeamElement.wrl\" \
   ] \
   BeamElement { \
";

bool CBeamElement::Display(void) const {
   // Calculate colours
   float pfColours[6];
   CalculateColours(pfColours);
   // Calculate node positions
   float pfNodes[6];
   CalculateNodePositions(pfNodes);
   // If the beam isn't there yet
   if (m_pNodePtr == NULL) {
      // Create a beam and add it to the scene
      // Create string buffer for VRML text
      char pcBuffer[2048];
      memset(pcBuffer,0,2048);
      ostrstream strBeam(pcBuffer,2048);
      // Add the basic BeamElement syntax
      strBeam << pcBeamStart;
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
      if (m_pCortona->CreateVrmlFromString(pcBuffer, &m_pNodePtr)) {
         m_pCortona->AddToScene(m_pNodePtr);
         return true;
      }
      else return false;
   }
   else {
      CCortonaField* pField;
      if (m_pCortona->GetField(m_pNodePtr,"nodes",&pField)) {
         pField->GetMFVec3f();
         float fX, fY, fZ;
         pField->GetMFVec3f(0, fX, fY, fZ);
         fY -= 0.1f;
         pField->SetMFVec3f(0, fX, fY, fZ);
         pField->Release();
         return true;
      }      
      return false;
   }
}

void CBeamElement::SetSize(float fHeight, float fWidth, float fFlangeThickness, float fWebThickness) {
   m_fHeight = fHeight;
   m_fWidth = fWidth;
   m_fFlange = fFlangeThickness;
   m_fWeb = fWebThickness;
}

void CBeamElement::SetNodes(int iFirstNode, int iSecondNode) {
   m_piNodes[0] = iFirstNode;
   m_piNodes[1] = iSecondNode;
   return;
}

void CBeamElement::SetStresses(float fFirstNode, float fSecondNode) {
   m_pfStresses[0] = fFirstNode;
   m_pfStresses[1] = fSecondNode;
   return;
}

void CBeamElement::CalculateColours(float* pfColours) const {
   switch (m_oColourScheme) {
   case SOLID:
      {
         for (int i=0; i<2; i++) {
            pfColours[(i*3) + 0] = m_pfColour[0];
            pfColours[(i*3) + 1] = m_pfColour[1];
            pfColours[(i*3) + 2] = m_pfColour[2];
         }
      }
      break;
   case STRESS:
      {
         for (int i=0; i<2; i++) {
            float fStress = (m_pfStresses[i] - m_fMinStress) / (m_fMaxStress - m_fMinStress);
            pfColours[(i*3) + 0] = fStress * 2;
            pfColours[(i*3) + 1] = 2 - (fStress * 2);
            pfColours[(i*3) + 2] = 0;
         }
      }
      break;
   }
   return;
}

void CBeamElement::CalculateNodePositions(float* pfNodes) const {
   for (int i=0; i<2; i++) {
      const float* pNode = m_pNodeSet->Node(m_piNodes[i]);
      pfNodes[(i*3) + 0] = pNode[0];
      pfNodes[(i*3) + 1] = pNode[1];
      pfNodes[(i*3) + 2] = pNode[2];
   }
   return;
}

void CBeamElement::SetVisible(bool bVisible) const {
   // Send eventIn
   return;
}
