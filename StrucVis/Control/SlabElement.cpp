//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SlabElement.cpp
// 19/03/2002 - James Smith
//
// $Id: SlabElement.cpp,v 1.15 2002/03/22 12:48:35 vap-james Exp $

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
   memset(m_pfStresses,0,9*sizeof(float));
   memset(m_piNodes,0,9*sizeof(unsigned int));
   memset(m_pcCracks,0,9*sizeof(unsigned char));
}

// Beginning of SlabElement node
const char pcSlabStart[] = " \
   EXTERNPROTO SlabElement [ \
      eventIn MFColor set_colours \
      eventIn MFInt32 set_cracks \
      eventIn MFVec3f set_nodes \
      eventIn SFBool  set_visible \
      exposedField MFString description \
      field MFColor colours \
      field MFInt32 cracks  \
      field MFVec3f nodes \
      field SFFloat thickness \
   ] \
   [ \
      \"SlabElement.wrl\" \
      \"file://D:\\Vapour\\Dev\\Src\\Research\\CortonaBase\\SlabElement.wrl\" \
      \"file://D:\\James\\vapour\\dev.local\\src\\Research\\CortonaBase\\SlabElement.wrl\" \
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
      // Set the description
      strSlab << " description \"Group: " << m_iGroup << "\" ";
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
      // Setup cracks
      strSlab << " cracks [ ";
      for (int cr=0; cr<9; cr++) strSlab << " " << static_cast<int>(m_pcCracks[cr]);
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
      bool bOK = true;

      // Update node positions
      CCortonaField* pField = m_poCortona->CreateField("MFVec3f");
      if (pField==NULL) return false;
      // Set values
      for (int i=0; i<9 && bOK; i++) {
         if (bOK && !pField->AddMFVec3f(pfNodes[(i*3)], pfNodes[(i*3)+1], pfNodes[(i*3)+2]))
            bOK = false;
      }      
      // Send event
      if (bOK && !m_poNodePtr->AssignEventIn("set_nodes",*pField))
         bOK = false;
      // Delete field
      pField->Release();
      delete pField;

      // Update colours
      pField = m_poCortona->CreateField("MFColor");
      if (pField==NULL) return false;
      // Set values
      for (i=0; i<9 && bOK; i++) {
         if (bOK && !pField->AddMFColor(pfColours[(i*3)], pfColours[(i*3)+1], pfColours[(i*3)+2]))
            bOK = false;
      }      
      // Send event
      if (bOK && !m_poNodePtr->AssignEventIn("set_colours",*pField))
         bOK = false;
      // Delete field
      pField->Release();
      delete pField;

      // Update cracks
      pField = m_poCortona->CreateField("MFInt32");
      if (pField == NULL) return false;
      // Set values
      for (i=0; i<9 && bOK; i++) {
         if (bOK && !pField->AddMFInt32(m_pcCracks[i]))
            bOK = false;
      }      
      // Send event
      if (bOK && !m_poNodePtr->AssignEventIn("set_cracks",*pField))
         bOK = false;
      // Delete field
      pField->Release();
      delete pField;

      // Done
      return bOK;
   }
}

void CSlabElement::SetNodes(const unsigned int* piNodes) {
   memcpy(m_piNodes,piNodes,9*sizeof(int));
   return;
}

void CSlabElement::SetCracks(unsigned int iLayer, const unsigned char* pcCracks) const {
   memcpy(m_pcCracks,pcCracks,9*sizeof(char));
   return;
}

void CSlabElement::SetStresses (const float* pfStresses) const {
   memcpy(m_pfStresses,pfStresses,9*sizeof(float));
   return;
}

void CSlabElement::SetSize(float fThickness) {
   m_fThickness = fThickness;
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

bool CSlabElement::SetVisible(bool bVisible) const {
   if (m_poNodePtr!=NULL) {
      // Create boolean field
      CCortonaField* poSFBool = m_poCortona->CreateField("SFBool");
      if (poSFBool==NULL) return false;
      // Set value
      poSFBool->SetSFBool(bVisible);
      // Send event
      if (!m_poNodePtr->AssignEventIn("set_visible",*poSFBool)) return false;
      // Done!
      poSFBool->Release();
      delete poSFBool;
      return true;
   }
   else return false;
}
