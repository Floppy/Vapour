//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SlabElement.cpp
// 19/03/2002 - James Smith
//
// $Id: SlabElement.cpp,v 1.7 2002/03/21 14:32:08 vap-warren Exp $

#include "stdafx.h"
#include "SlabElement.h"

#include <strstrea.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CBeamElement

CSlabElement::CSlabElement(CCortonaUtil *poCortona, CNodeSet* poNodeSet) : 
   CElement(poCortona, poNodeSet),
   m_fThickness(1)
{
}

// Beginning of SlabElement node
const char pcSlabStart[] = " \
   EXTERNPROTO SlabElement [ \
      eventIn MFColor set_colours \
      eventIn MFVec3f set_nodes \
      eventIn SFBool  set_visible \
      field MFColor colours \
      field MFVec3f nodes \
      field SFFloat thickness \
   ] \
   [ \
      \"file://D:\\Vapour\\Dev\\Src\\Research\\CortonaBase\\SlabElement.wrl\" \
   ] \
   SlabElement { \
";

bool CSlabElement::Display(void) const {
   // Calculate colours
   float pfColours[27];
   CalculateColours(pfColours);
   // Calculate node positions
   float pfNodes[27];
   CalculateNodePositions(pfNodes);
   // If the slab isn't there yet
   if (m_poNodePtr == NULL) {
      // Create a slab and add it to the scene
      // Create string buffer for VRML text
      char pcBuffer[2048];
      memset(pcBuffer,0, 2048);
      ostrstream strSlab(pcBuffer, 2048);
      // Add the basic SlabElement syntax
      strSlab << pcSlabStart;
      // Set the size
      strSlab << " thickness " << m_fThickness;
      // Set colours
      strSlab << " colours [ ";
      for (int c=0; c<27; c++) strSlab << " " << pfColours[c];
      strSlab << " ] ";
      // Setup node positions
      strSlab << " nodes [ ";
      for (int n=0; n<27; n++) strSlab << " " << pfNodes[n];
      strSlab << " ] ";
      // Close the SlabElement
      strSlab << "}";
      // Create VRML nodes from the buffer
      if (m_poNodePtr = m_poCortona->CreateVrmlFromString(pcBuffer)) {
         m_poCortona->AddToScene(*m_poNodePtr);
         return true;
      }
      else return false;
   }
   else {
      CCortonaField* poTranslation;
      if (poTranslation = m_poCortona->GetField(*m_poNodePtr,"translation")) {
         float fX, fY, fZ;
         poTranslation->GetSFVec3f(fX, fY, fZ);
         fX += 0.1f;
         poTranslation->SetSFVec3f(fX, fY, fZ);
         poTranslation->Release();
         delete poTranslation;
         return true;
      }      
      return false;
   }
}

void CSlabElement::SetNodes(int iFirstNode, int iSecondNode, int iThirdNode, 
                            int iFourthNode, int iFifthNode, int iSixthNode, 
                            int iSeventhNode, int iEighthNode, int iNinthNode) {
   m_piNodes[0] = iFirstNode;
   m_piNodes[1] = iSecondNode;
   m_piNodes[2] = iThirdNode;
   m_piNodes[3] = iFourthNode;
   m_piNodes[4] = iFifthNode;
   m_piNodes[5] = iSixthNode;
   m_piNodes[6] = iSeventhNode;
   m_piNodes[7] = iEighthNode;
   m_piNodes[8] = iNinthNode;
   return;
}

void CSlabElement::SetSize(float fThickness) {
   m_fThickness = fThickness;
}

void CSlabElement::SetStresses(float fFirstNode, float fSecondNode, float fThirdNode, 
                               float fFourthNode, float fFifthNode, float fSixthNode, 
                               float fSeventhNode, float fEighthNode, float fNinthNode) {
   m_pfStresses[0] = fFirstNode;
   m_pfStresses[1] = fSecondNode;
   m_pfStresses[2] = fThirdNode;
   m_pfStresses[3] = fFourthNode;
   m_pfStresses[4] = fFifthNode;
   m_pfStresses[5] = fSixthNode;
   m_pfStresses[6] = fSeventhNode;
   m_pfStresses[7] = fEighthNode;
   m_pfStresses[8] = fNinthNode;
   return;
}

void CSlabElement::CalculateColours(float* pfColours) const {
   switch (m_oColourScheme) {
   case SOLID:
      {
         for (int i=0; i<9; i++) {
            pfColours[(i*3) + 0] = m_pfColour[0];
            pfColours[(i*3) + 1] = m_pfColour[1];
            pfColours[(i*3) + 2] = m_pfColour[2];
         }
      }
      break;
   case STRESS:
      {
         for (int i=0; i<9; i++) {
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

void CSlabElement::CalculateNodePositions(float* pfNodes) const {
   for (int i=0; i<9; i++) {
      const float* pNode = m_poNodeSet->Node(m_piNodes[i]);
      pfNodes[(i*3) + 0] = pNode[0];
      pfNodes[(i*3) + 1] = pNode[1];
      pfNodes[(i*3) + 2] = pNode[2];
   }
   return;
}

void CSlabElement::SetVisible(bool bVisible) const {
   // Send eventIn
   return;
}
