//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SceneManager.cpp
// 19/03/2002 - James Smith
//
// $Id: SceneManager.cpp,v 1.10 2002/03/22 12:14:48 vap-james Exp $

#include "stdafx.h"
#include "SceneManager.h"

#include "BeamElement.h"
#include "SlabElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////
// Test Data

const int g_iNumNodes = 9;
const float g_pfDefaultNodes[27] = {
   -1, 0, -1,
   -1, 0, 0,
   -1, 0, 1,
   0, 0, -1,
   0, 0, 0,
   0, 0, 1,
   1, 0, -1,
   1, 0, 0,
   1, 0, 1
};

const g_iNumFrames = 4;
const float g_pfNodeDisplacments[g_iNumFrames][27] = {
   {
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0
   },
   {
      0, 0, 0,
      0, 0, 0,
      0, -0.1f, 0,
      0, 0, 0,
      0, -0.05f, 0,
      0, -0.2f, 0,
      0, 0, 0,
      0, -0.1f, 0,
      0, -0.4f, 0
   },
   {
      0, 0, 0,
      0, 0, 0,
      0, -0.2f, 0,
      0, 0, 0,
      0, -0.1f, 0,
      0, -0.4f, 0,
      0, 0, 0,
      0, -0.2f, 0,      
      0, -0.7f, 0
   },
   {
      0, 0, 0,
      0, 0, 0,
      0, -0.5, 0,
      0, 0, 0,
      0, -0.25, 0,
      0, -0.6f, 0,
      0, -0.1f, 0,
      0, -0.5, 0,      
      0, -1, 0
   }
};
const float g_pfNodeStresses[g_iNumFrames][9] = {
   {0,0,0,0,0,0,0,0,0},
   {0,10,25,0,15,25,10,25,25},
   {0,20,50,0,25,50,25,50,50},
   {0,50,100,0,75,100,50,100,100}
};

const float g_fStartTemp = 20.0f;

const g_iNumGroups = 2;

const TElementType g_ptGroupTypes[g_iNumGroups] = {
   BEAM, SLAB
};
const float g_pfGroupTemps[g_iNumFrames][g_iNumGroups] = {
   {30,30},{40,40},{60,50},{80,60}
};

const unsigned int g_iNumBeams = 6;
const float g_fBeamHeight = 0.5446f;
const float g_fBeamWidth = 0.2119f;
const float g_fFlange = 0.0213f;
const float g_fWeb = 0.0128f;
const unsigned int g_piBeamNodes[g_iNumBeams][2] = {
   {1, 2},
   {2, 3},
   {1, 4},
   {4, 7},
   {7, 8},
   {8, 9}
};
const unsigned int g_piNodeGroups[g_iNumBeams] = {
   1,1,1,1,1,1
};

const unsigned int g_iNumSlabs = 1;
const float g_fSlabThickness = 0.3f;
const unsigned int g_piSlabNodes[1][9] = {
   {9, 6, 3, 2, 1, 4, 7, 8, 5}
};
const unsigned int g_piSlabGroups[g_iNumSlabs] = {
   2
};
const unsigned char g_pcSlabCracks[g_iNumFrames][9] = {
   {0,0,0,0,0,0,0,0,0},
   {0,0,1,0,0,0,0,1,0},
   {0,1,1,0,1,1,0,1,1},
   {3,1,2,3,2,2,1,2,2}
};

///////////
// CSceneManager

typedef std::vector<CElement*>::iterator elemIter;
typedef std::vector<CSceneManager::CGroup>::iterator grpIter;

CSceneManager::CSceneManager(CCortonaUtil *poCortona) :
   m_poCortona(poCortona),
   m_oViewpoint(poCortona),
   m_iNumFrames(0)
{
}
   
CSceneManager::~CSceneManager() {
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      delete *pElem;
   }
}

void CSceneManager::Load(void) {
   
   // Initialise the NodeSet
   m_oNodeSet.SetSize(9);
   m_oNodeSet.SetDefault(g_pfDefaultNodes);

   // Create groups
   for (int i=0; i<g_iNumGroups; i++) {
      CGroup oGroup;
      oGroup.m_fTemperature = g_fStartTemp;
      oGroup.m_oType = g_ptGroupTypes[i];
      m_oGroups.push_back(oGroup);      
   }

   // Load frame info
   m_iNumFrames = g_iNumFrames;
      
   // Create some beams
   for (i=0; i<g_iNumBeams; i++) {
      CBeamElement* pBeam = new CBeamElement(m_poCortona,&m_oNodeSet);
      pBeam->SetID(i);
      pBeam->SetGroup(g_piNodeGroups[i]);
      pBeam->SetSize(g_fBeamHeight,g_fBeamWidth,g_fFlange,g_fWeb);
      pBeam->SetNodes(g_piBeamNodes[i]);
      pBeam->SetColourScheme(STRESS);
      pBeam->SetStressRange(0,100);
      m_oElements.push_back(pBeam);   
   }

   // Create some slabs
   for (i=0; i<g_iNumSlabs; i++) {
      CSlabElement* pSlab = new CSlabElement(m_poCortona,&m_oNodeSet);
      pSlab->SetID(i+g_iNumBeams);
      pSlab->SetGroup(g_piSlabGroups[i]);
      pSlab->SetSize(g_fSlabThickness);
      pSlab->SetNodes(g_piSlabNodes[i]);
      pSlab->SetColourScheme(STRESS);
      pSlab->SetStressRange(0,100);
      m_oElements.push_back(pSlab);
   }

   // Done
   Update();

}

unsigned int CSceneManager::NumGroups(void) {
   return m_oGroups.size();
}

unsigned int CSceneManager::NumElements(unsigned int iGroup) {
   // If a group hasn't been set:
   if (iGroup == 0) 
      return m_oElements.size();
   // Otherwise, count the elements in the group
   int iCount = 0;
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      if ((*pElem)->Group() == iGroup) iCount++;
   }   
   return iCount;
}

TElementType CSceneManager::GroupType(unsigned int iGroup) {
   return m_oGroups[iGroup].m_oType;
}

float CSceneManager::Temperature(unsigned int iGroup) {
   return m_oGroups[iGroup].m_fTemperature;
}

void CSceneManager::SetVisibility(unsigned int iGroup, bool bVisible) {
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      if ((*pElem)->Group() == iGroup) (*pElem)->SetVisible(bVisible);
   }   
}

void CSceneManager::SetScaleFactor(float fX, float fY, float fZ) {
   m_oNodeSet.SetScaleFactor(fX,fY,fZ);
}

void CSceneManager::SetViewpoint(float pfPosition[3], float pfRotation[4]) {
   m_oViewpoint.Set(pfPosition,pfRotation);
}

void CSceneManager::ShowFrame(unsigned int iFrame) {
   // Load temperatures into groups
   for (int g=0; g<m_oGroups.size(); g++) {
      m_oGroups[g].m_fTemperature = g_pfGroupTemps[iFrame][g];
   }
   // Load node displacements
   m_oNodeSet.Displace(g_pfNodeDisplacments[iFrame]);
   // Load node stresses
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      if ((*pElem)->Type() == BEAM) {
         // Enter beam node stresses
         float pfStresses[2];
         for (int i=0; i<2; i++) {
            pfStresses[i] = g_pfNodeStresses[iFrame][(*pElem)->Node(i)];
         }
         (*pElem)->SetStresses(pfStresses);
      }
      else if ((*pElem)->Type() == SLAB) {
         // Enter slab node stresses and cracks
         float pfStresses[9];
         for (int i=0; i<9; i++) {
            pfStresses[i] = g_pfNodeStresses[iFrame][(*pElem)->Node(i)];
         }
         (*pElem)->SetStresses(pfStresses);
         static_cast<CSlabElement*>(*pElem)->SetCracks(0,g_pcSlabCracks[iFrame]);
      }
   }
   // Render
   Update();
}

void CSceneManager::Update(void) {   
   // Freeze rendering if appropriate
   // Update element display
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      (*pElem)->Display();
   }
   // Start rendering again
   // Done
   return;
}