//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SlabElement.cpp
// 19/03/2002 - James Smith
//
// $Id: SlabElement.cpp,v 1.4 2002/03/20 21:57:19 vap-warren Exp $

#include "stdafx.h"
#include "SlabElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CBeamElement

CSlabElement::CSlabElement(CCortonaUtil *pCortona, CNodeSet* pNodeSet) : CElement(pCortona,pNodeSet)
{
   return;
}

// Simple vrml scene
const char pcCubeSyntax[] = "                   \
    DEF CUBE Transform {                        \
        translation 0 3 0                       \
        children Shape {                        \
            geometry Box {                      \
                size 2 2 2                      \
            }                                   \
            appearance Appearance {             \
                material DEF RED Material {     \
                    diffuseColor 1 0 0          \
                }                               \
            }                                   \
        }                                       \
    }";

bool CSlabElement::Display(void) const {
   // If the cube isn't there yet
   if (m_pNodePtr == NULL) {
      // Create a cube and add it to the scene
      if (m_pCortona->CreateVrmlFromString(pcCubeSyntax, &m_pNodePtr)) {
         m_pCortona->AddToScene(m_pNodePtr);
         return true;
      }
      else return false;
   }
   else {
      CCortonaField* pTranslation;
      if (m_pCortona->GetField(m_pNodePtr,"translation",&pTranslation)) {
         float fX, fY, fZ;
         pTranslation->GetSFVec3f(fX, fY, fZ);
         fX += 0.1f;
         pTranslation->SetSFVec3f(fX, fY, fZ);
         pTranslation->Release();
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
      const float* pNode = m_pNodeSet->Node(m_piNodes[i]);
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
