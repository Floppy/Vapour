//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SceneManager.cpp
// 19/03/2002 - James Smith
//
// $Id: SceneManager.cpp,v 1.2 2002/03/21 21:55:24 vap-james Exp $

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
   float pfNodes[27] = {
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
   m_oNodeSet.SetSize(9);
   m_oNodeSet.SetDefault(pfNodes);

   // Create some beams
   CBeamElement* pBeam = new CBeamElement(m_poCortona,&m_oNodeSet);
   pBeam->SetID(1);
   pBeam->SetGroup(1);
   pBeam->SetSize(0.5446f, 0.2119f, 0.0213f, 0.0128f);
   unsigned int piBeam1[2] = {1,2};
   pBeam->SetNodes(piBeam1);
   pBeam->SetColourScheme(STRESS);
   pBeam->SetStressRange(0,100);
   m_oElements.push_back(pBeam);

   pBeam = new CBeamElement(m_poCortona,&m_oNodeSet);
   pBeam->SetID(2);
   pBeam->SetGroup(1);
   pBeam->SetSize(0.5446f, 0.2119f, 0.0213f, 0.0128f);
   unsigned int piBeam2[2] = {2,3};
   pBeam->SetNodes(piBeam2);
   pBeam->SetColourScheme(STRESS);
   pBeam->SetStressRange(0,100);
   m_oElements.push_back(pBeam);

   pBeam = new CBeamElement(m_poCortona,&m_oNodeSet);
   pBeam->SetID(3);
   pBeam->SetGroup(1);
   pBeam->SetSize(0.5446f, 0.2119f, 0.0213f, 0.0128f);
   unsigned int piBeam3[2] = {7,8};
   pBeam->SetNodes(piBeam3);
   pBeam->SetColourScheme(STRESS);
   pBeam->SetStressRange(0,100);
   m_oElements.push_back(pBeam);
   
   // Create a slab
   CSlabElement* pSlab = new CSlabElement(m_poCortona,&m_oNodeSet);
   pSlab->SetID(4);
   pSlab->SetGroup(2);
   pSlab->SetSize(0.3f);
   unsigned int piSlab1[9] = {9,6,3,2,1,4,7,8,5};
   pSlab->SetNodes(piSlab1);
   pSlab->SetColourScheme(STRESS);
   pSlab->SetStressRange(0,100);
   m_oElements.push_back(pSlab);

   Update();

}

void CSceneManager::FrameFirst(void) {
   float pfNodes[27] = {
      0, 0, 0,
      0, 0, 0,
      0, -0.5, 0,
      0, 0, 0,
      0, -0.25, 0,
      0, -0.6, 0,
      0, -0.1, 0,
      0, -0.5, 0,      
      0, -1, 0
   };
   m_oNodeSet.Displace(pfNodes);
   // Render
   Update();
}

void CSceneManager::FramePrev(void) {
   float pfNodes[27] = {
      0, 0, 0,
      0, 0, 0,
      0, -0.5, 0,
      0, 0, 0,
      0, -0.25, 0,
      0, -0.6, 0,
      0, -0.1, 0,
      0, -0.5, 0,      
      0, -1, 0
   };
   m_oNodeSet.Displace(pfNodes);
   // Set stresses
   float pfBeam1[2] = {0,50};
   float pfBeam2[2] = {50,100};
   float pfBeam3[2] = {50,100};
   float pfSlab1[9] = {100,100,100,50,0,0,50,100,75};
   (m_oElements[0])->SetStresses(pfBeam1);
   (m_oElements[1])->SetStresses(pfBeam2);
   (m_oElements[2])->SetStresses(pfBeam3);
   (m_oElements[3])->SetStresses(pfSlab1);
   // Render
   Update();
}

void CSceneManager::FrameNext(void) {
   float pfNodes[27] = {
      0, 0, 0,
      0, 0, 0,
      0, -0.5, 0,
      0, 0, 0,
      0, -0.25, 0,
      0, -0.6, 0,
      0, -0.1, 0,
      0, -0.5, 0,      
      0, -1, 0
   };
   m_oNodeSet.Displace(pfNodes);
   // Render
   Update();
}

void CSceneManager::FrameLast(void) {
   float pfNodes[27] = {
      0, 0, 0,
      0, 0, 0,
      0, -0.5, 0,
      0, 0, 0,
      0, -0.25, 0,
      0, -0.6, 0,
      0, -0.1, 0,
      0, -0.5, 0,      
      0, -1, 0
   };
   m_oNodeSet.Displace(pfNodes);
   // Render
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
   //m_oViewpoint.Set(pfPosition,pfRotation);
}

void CSceneManager::Update(void) {   
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      (*pElem)->Display();
   }
}