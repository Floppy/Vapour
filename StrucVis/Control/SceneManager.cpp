//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SceneManager.cpp
// 19/03/2002 - James Smith
//
// $Id: SceneManager.cpp,v 1.13 2002/03/22 14:56:26 vap-james Exp $

#include "stdafx.h"
#include "SceneManager.h"

#include "BeamElement.h"
#include "SlabElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////
// CSceneManager

typedef std::vector<CElement*>::iterator elemIter;
typedef std::vector<CSceneManager::CGroup>::iterator grpIter;

CSceneManager::CSceneManager(CCortonaUtil *poCortona) :
   m_poCortona(poCortona),
   m_oViewpoint(poCortona)
{
}
   
CSceneManager::~CSceneManager() {
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      delete *pElem;
   }
}

void CSceneManager::Load(void) {
   
   // Initialise the NodeSet
   m_oNodeSet.SetSize(m_oDataMgr.NumNodes());
   m_oNodeSet.SetDefault(m_oDataMgr.NodePositions());

   // Create groups
   for (int i=0; i<m_oDataMgr.NumGroups(); i++) {
      CGroup oGroup;
      oGroup.m_fTemperature = 0;
      oGroup.m_oType = m_oDataMgr.GroupType(i);
      m_oGroups.push_back(oGroup);
   }

   // Create some beams
   for (i=0; i<m_oDataMgr.NumBeams(); i++) {
      // Create beam
      CBeamElement* pBeam = new CBeamElement(m_poCortona,&m_oNodeSet);
      // Set info
      pBeam->SetID(i);
      pBeam->SetGroup(m_oDataMgr.BeamGroup(pBeam->ID()));
      // Set sizes
      float fHeight, fWidth, fFlange, fWeb;
      m_oDataMgr.BeamSizes(pBeam->Group(),fHeight, fWidth, fFlange, fWeb);
      pBeam->SetSize(fHeight, fWidth, fFlange, fWeb);
      // Set nodes
      pBeam->SetNodes(m_oDataMgr.BeamNodes(pBeam->ID()));
      // Set other info
      pBeam->SetColourScheme(STRESS);
      pBeam->SetStressRange(0,100);
      // Add to list      
      m_oElements.push_back(pBeam);   
   }

   // Create some slabs
   for (i=0; i<m_oDataMgr.NumSlabs(); i++) {
      // Create slab
      CSlabElement* pSlab = new CSlabElement(m_poCortona,&m_oNodeSet);
      // Set info
      pSlab->SetID(i);
      pSlab->SetGroup(m_oDataMgr.SlabGroup(pSlab->ID()));
      // Set sizes
      float fThickness;
      m_oDataMgr.SlabSizes(pSlab->Group(),fThickness);
      pSlab->SetSize(fThickness);
      // Set nodes
      pSlab->SetNodes(m_oDataMgr.SlabNodes(pSlab->ID()));
      // Set other info
      pSlab->SetColourScheme(STRESS);
      pSlab->SetStressRange(0,100);
      // Add to list      
      m_oElements.push_back(pSlab);
   }

   // Done
   ShowFrame(0);

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
   // Load frame into data manager
   m_oDataMgr.LoadFrame(iFrame);
   // Load temperatures into groups
   for (int g=0; g<m_oGroups.size(); g++) {
      m_oGroups[g].m_fTemperature = m_oDataMgr.GroupTemp(g);
   }
   // Load node displacements
   m_oNodeSet.Displace(m_oDataMgr.NodeDisplacements());
   // Load node stresses
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      if ((*pElem)->Type() == BEAM) {
         // Enter beam node stresses
         float pfStresses[2];
         for (int i=0; i<2; i++) {
            pfStresses[i] = m_oDataMgr.BeamStresses((*pElem)->ID())[(*pElem)->Node(i)];
         }
         (*pElem)->SetStresses(pfStresses);
      }
      else if ((*pElem)->Type() == SLAB) {
         // Enter slab node stresses and cracks
         float pfStresses[9];
         for (int i=0; i<9; i++) {
            pfStresses[i] = m_oDataMgr.SlabStresses((*pElem)->ID())[(*pElem)->Node(i)];
         }
         (*pElem)->SetStresses(pfStresses);
         unsigned int iLayer = 0;
         static_cast<CSlabElement*>(*pElem)->SetCracks(iLayer,m_oDataMgr.SlabCracks((*pElem)->ID(),iLayer));
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