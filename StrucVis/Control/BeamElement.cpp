//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// BeamElement.cpp
// 19/03/2002 - James Smith
//
// $Id: BeamElement.cpp,v 1.1 2002/03/20 02:06:23 vap-james Exp $

#include "stdafx.h"
#include "BeamElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CBeamElement

CBeamElement::CBeamElement(CCortonaUtil* pCortona) : CElement(pCortona)
{
   return;
}

// Simple vrml scene
const char pcCubeSyntax[] = " \
   EXTERNPROTO BeamElement [ \
      eventIn MFColor set_colours \
      eventIn MFVec3f set_nodes \
      field MFColor colours \
      field SFFloat flange \
      field SFFloat height \
      field SFFloat web \
      field SFFloat width \
      field MFVec3f nodes \
   ] \
   [ \
      \"file://D:\\James\\vapour\\dev.local\\src\\Research\\CortonaBase\\BeamElement.wrl\" \
   ] \
   Transform { \
      children [ \
         BeamElement { \
            nodes [ \
               0 0 0 \
               0 0 -.125 \
            ] \
            height .5446 \
            width  .2119 \
            flange .0213 \
            web    .0128 \
         } \
      ] \
   }\
";

bool CBeamElement::Display(void) const {
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
      CCortonaField* pField;
      if (m_pCortona->GetField(m_pNodePtr,"nodes",&pField)) {
         float fY;
         pField->GetMFVec3f_Y(0,fY);
         fY -= 0.1f;
         pField->GetMFVec3f_Y(0,fY);
         pField->Release();
         return true;
      }      
      return false;
   }
}

void CBeamElement::SetNodes(int iFirstNode, int iSecondNode) {
   m_piNodes[0] = iFirstNode;
   m_piNodes[1] = iSecondNode;
   return;
}