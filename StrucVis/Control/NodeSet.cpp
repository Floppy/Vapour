//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management"
//! userlevel =  Normal
//! file      = "Control/NodeSet.cpp"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: NodeSet.cpp,v 1.12 2002/04/23 11:32:43 vap-james Exp $"

#include "stdafx.h"
#include "NodeSet.h"

#include <strstrea.h>
#include <memory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////
// CNodeSet

CNodeSet::CNodeSet(CCortonaUtil *pCortona) :
   m_poCortona(pCortona),
   m_poNodePtr(NULL),   
   m_iNumNodes(0),
   m_pfCurrentNodePositions(NULL),
   m_pfDefaultNodePositions(NULL)
{
}

CNodeSet::CNodeSet(int iNumNodes, CCortonaUtil *pCortona) :
   m_poCortona(pCortona),
   m_poNodePtr(NULL),
   m_iNumNodes(0),
   m_pfCurrentNodePositions(NULL),
   m_pfDefaultNodePositions(NULL)
{
   SetSize(iNumNodes);
}

CNodeSet::~CNodeSet() {
   if (m_pfCurrentNodePositions) delete [] m_pfCurrentNodePositions;
   if (m_pfDefaultNodePositions) delete [] m_pfDefaultNodePositions;
   if (m_poNodePtr) {
      m_poNodePtr->Release();
      delete m_poNodePtr;
   }
}

void CNodeSet::SetSize(int iNumNodes) {
   // Store number of nodes
   m_iNumNodes = iNumNodes;
   // Dump old data if present
   if (m_pfCurrentNodePositions) delete [] m_pfCurrentNodePositions;
   if (m_pfDefaultNodePositions) delete [] m_pfDefaultNodePositions;
   // Allocate new memory
   m_pfCurrentNodePositions = new float[iNumNodes * 3];
   m_pfDefaultNodePositions = new float[iNumNodes * 3];
   // Done
   return;
}

// Beginning of NodeSet node
const char pcNodeSet[] = " \
   EXTERNPROTO NodeSet [ \
      exposedField MFVec3f point \
   ] \
   [ \
      \"";

bool CNodeSet::Display(const char* pcURL) {
   // If the node isn't there yet
   if (m_poNodePtr == NULL) {

      // Create a NodeSet node and add it to the scene
      // Create string buffer for VRML text
      int iLength = 2048 + m_iNumNodes*32;
      char* pcBuffer = new char[iLength];
      memset(pcBuffer,0, iLength);
      ostrstream strNSet(pcBuffer, iLength);
      // Add the basic NodeSet syntax
      strNSet << pcNodeSet;
      // Add the EXTERNPROTO URL
      if (pcURL != NULL) strNSet << pcURL;
      // Add the rest of the preamble;
      strNSet << "NodeSet.wrl\" ] NodeSet { ";
      // Setup node positions
      strNSet << " point [";
      for (unsigned int n=0; n<3*m_iNumNodes; n++) strNSet << " " << m_pfDefaultNodePositions[n];
      strNSet << " ]";
      unsigned int iLen = strlen(pcBuffer);
      // Close the NodeSet
      strNSet << "}";
      // Create VRML nodes from the buffer
      if (m_poNodePtr = m_poCortona->CreateVrmlFromString(pcBuffer)) {
         delete [] pcBuffer;
         // Display nodeset
         return Redisplay();
      }
      delete [] pcBuffer;
   }
   return false;
}

void CNodeSet::SetDefault(const float* pfNodePositions) {
   memcpy(m_pfDefaultNodePositions,pfNodePositions,m_iNumNodes*3*sizeof(float));
   memcpy(m_pfCurrentNodePositions,pfNodePositions,m_iNumNodes*3*sizeof(float));
}

bool CNodeSet::Displace(const float* pfDisplacements) const {
   // Check input data
   if (pfDisplacements == NULL) return false;
   // Carry on
   float* pfDefault = m_pfDefaultNodePositions;
   float* pfCurrent = m_pfCurrentNodePositions;
   const float* pfDisplacement = pfDisplacements;
   for (unsigned int n=0; n<m_iNumNodes; n++) {
      for (unsigned int c=0; c<3; c++) {
         *pfCurrent = *pfDefault + (m_pfScale[c] * *pfDisplacement);
         pfCurrent++;
         pfDefault++;
         pfDisplacement++;
      }
   }
   return Update();
}

bool CNodeSet::Update(void) const {
   //#===--- Update node positions
   // Set values
   CCortonaField* poField = m_poCortona->CreateField("MFVec3f");
   if (poField) {
      bool bOK = poField->SetMFVec3f(m_pfCurrentNodePositions, m_iNumNodes);
      // Send event
      if (bOK && !m_poNodePtr->AssignEventIn("set_point", *(poField)))
         bOK = false;
      poField->Release();
      delete poField;
      return bOK;
   }
   return false;
}

void CNodeSet::SetScaleFactor(float fX, float fY, float fZ) const {
   m_pfScale[0] = fX;
   m_pfScale[1] = fY;
   m_pfScale[2] = fZ;
}

const float* CNodeSet::Node(unsigned int iIndex) const {
   if (iIndex == 0) return NULL;
   return m_pfCurrentNodePositions + ((iIndex-1)*3);
}

bool CNodeSet::Connect(const CElement* pElement) {
   if (!m_poCortona || !m_poNodePtr) 
      return false;
   m_poCortona->AddRoute(*m_poNodePtr, "point_changed", *(pElement->m_poNodePtr), "set_nodes");
   return false;
}

bool CNodeSet::Redisplay(void) {
   if (!m_poCortona || !m_poNodePtr) 
      return false;

   // Add nodes to scene
   if (!m_poCortona->AddToScene(*m_poNodePtr)) return false;

   return true;
}


   
