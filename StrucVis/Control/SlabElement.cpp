//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SlabElement.cpp
// 19/03/2002 - James Smith
//
// $Id: SlabElement.cpp,v 1.2 2002/03/20 13:29:50 vap-james Exp $

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
         float fX;
         pTranslation->GetSFVec3f_X(fX);
         fX += 0.1f;
         pTranslation->SetSFVec3f_X(fX);
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