//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SceneManager.cpp
// 19/03/2002 - James Smith
//
// $Id: SceneManager.cpp,v 1.23 2002/03/26 17:45:45 vap-james Exp $

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
   m_oViewpoint(poCortona),
   m_bLoading(false),
   m_pcURL(NULL)
{
}
   
CSceneManager::~CSceneManager() {
   Empty();
   if (m_pcURL != NULL) free(m_pcURL);
}

bool CSceneManager::Setup(const unsigned char* pcData, unsigned int iLength) {
   if (!m_bLoading) {
      Empty();
      m_bLoading = true;
   }
   if (m_oDataMgr.Setup(pcData,iLength)) {
      Load();
      m_bLoading = false;
      return true;
   }
   else return false;
}

void CSceneManager::Empty(void) {
   // Empty VRML scene
   m_poCortona->Clear();
   // Delete elements
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      delete *pElem;
   }
   // Clear element list
   m_oElements.clear();
   // Redisplay viewpoint geometry, in case we're not closing down completely
   m_oViewpoint.Redisplay();
}

void CSceneManager::Load(void) {
   
   // Initialise the NodeSet
   m_oNodeSet.SetSize(m_oDataMgr.NumNodes());
   m_oNodeSet.SetDefault(m_oDataMgr.NodePositions());

   // Create some beams
   for (int i=1; i<=m_oDataMgr.NumBeams(); i++) {
      // Create beam
      CBeamElement* pBeam = new CBeamElement(m_poCortona,&m_oNodeSet);
      // Set info
      pBeam->SetID(i);
      pBeam->SetGroup(m_oDataMgr.BeamGroup(pBeam->ID()));
      pBeam->SetTemp(m_oDataMgr.GroupTemp(pBeam->Group()));
      // Set sizes
      float fHeight, fWidth, fFlange, fWeb;
      m_oDataMgr.BeamSizes(pBeam->Group(),fHeight, fWidth, fFlange, fWeb);
      pBeam->SetSize(fHeight, fWidth, fFlange, fWeb);
      // Set nodes
      pBeam->SetNodes(m_oDataMgr.BeamNodes(pBeam->ID()));
      // Set other info
      pBeam->SetColourScheme(m_tColourScheme);
      pBeam->SetColour(m_oDataMgr.GroupColour(pBeam->Group()));
      float fMin,fMax;
      m_oDataMgr.StressRange(fMin,fMax);
      pBeam->SetStressRange(fMin,fMax);
      // Add to list      
      m_oElements.push_back(pBeam);   
   }

   // Create some slabs
   for (i=1; i<=m_oDataMgr.NumSlabs(); i++) {
      // Create slab
      CSlabElement* pSlab = new CSlabElement(m_poCortona,&m_oNodeSet);
      // Set info
      pSlab->SetID(i);
      pSlab->SetGroup(m_oDataMgr.SlabGroup(pSlab->ID()));
      pSlab->SetTemp(m_oDataMgr.GroupTemp(pSlab->Group()));
      // Set sizes
      float fThickness;
      m_oDataMgr.SlabSizes(pSlab->Group(),fThickness);
      pSlab->SetSize(fThickness);
      // Set nodes
      pSlab->SetNodes(m_oDataMgr.SlabNodes(pSlab->ID()));
      // Set other info
      pSlab->SetColourScheme(m_tColourScheme);
      pSlab->SetColour(m_oDataMgr.GroupColour(pSlab->Group()));
      float fMin,fMax;
      m_oDataMgr.StressRange(fMin,fMax);
      pSlab->SetStressRange(fMin,fMax);
      // Add to list      
      m_oElements.push_back(pSlab);
   }
   
   Update();

   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      m_oViewpoint.Connect(*pElem);
   }      
}

bool CSceneManager::FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength) {
   return m_oDataMgr.FrameInfo(iFrame,iOffset,iLength);
}

unsigned int CSceneManager::NumGroups(void) {
   return m_oDataMgr.NumGroups();
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
   return m_oDataMgr.GroupType(iGroup);
}

float CSceneManager::Temperature(unsigned int iGroup) {
   return m_oDataMgr.GroupTemp(iGroup);
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
   m_oViewpoint.Set(m_pcURL,pfPosition,pfRotation);
}

void CSceneManager::SetColourScheme(TColourScheme oColour) {
   m_tColourScheme = oColour;
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      (*pElem)->SetColourScheme(m_tColourScheme);
   }
   Update();
}

bool CSceneManager::ShowFrame(const unsigned char* pcData, unsigned int iLength) {
   if (!m_oDataMgr.LoadFrame(pcData,iLength)) {
      return false;
   }
   // Load node displacements
   m_oNodeSet.Displace(m_oDataMgr.NodeDisplacements());
   // Load per-element data
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      // Set new temperature
      (*pElem)->SetTemp(m_oDataMgr.GroupTemp((*pElem)->Group()));
      // Load node stresses
      if ((*pElem)->Type() == BEAM) {
         // Enter beam node stresses
         float pfStresses[2];
         for (int i=0; i<2; i++) {
            // Display X values of stresses
            pfStresses[i] = m_oDataMgr.BeamStresses((*pElem)->ID())[i*3];
         }
         (*pElem)->SetStresses(pfStresses);
      }
      else if ((*pElem)->Type() == SLAB) {
         // Enter slab node stresses and cracks
         float pfStresses[9];
         for (int i=0; i<9; i++) {
            // Display X values of stresses
            pfStresses[i] = m_oDataMgr.SlabStresses((*pElem)->ID())[i*3];
         }
         (*pElem)->SetStresses(pfStresses);
         unsigned int iLayer = 0;
         static_cast<CSlabElement*>(*pElem)->SetCracks(iLayer,m_oDataMgr.SlabCracks((*pElem)->ID(),iLayer));
      }
   }
   // Render
   Update();
   // Done
   return true;
}

void CSceneManager::Update(void) {   
   // Freeze rendering if appropriate
   // Update element display
   for (elemIter pElem = m_oElements.begin(); pElem != m_oElements.end(); pElem++) {
      (*pElem)->Display(m_pcURL);
   }
   // Start rendering again
   // Done
   return;
}

void CSceneManager::SetBaseURL(const char* pcURL) {
   m_pcURL = strdup(pcURL);
   return;
}