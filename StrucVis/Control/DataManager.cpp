//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management.VSV File IO"
//! userlevel =  Normal
//! file      = "Control/DataManager.cpp"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: DataManager.cpp,v 1.21 2002/04/04 11:01:34 vap-warren Exp $"

#include "stdafx.h"
#include "DataManager.h"
#include "Element.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CDataManager

typedef std::vector<CDataManager::CGroup>::iterator grpIter;

CDataManager::CDataManager() :
   m_oState(INIT),
   m_pChunk(NULL),
   m_iNumBeams(0),
   m_iNumSlabs(0)
{
}

CDataManager::~CDataManager() {
   if (m_pChunk) delete m_pChunk;
   m_pChunk = NULL;   
}

bool CDataManager::Setup(const unsigned char* pcData, unsigned int iLength) {

   // If we already have complete data, dump it and start again
   if (m_oState == READY) {
      m_oRoot.Reset();
      // Reset data manager state
      m_oState = INIT;
   }

   // Validate header
   if (m_oState == INIT) {
      // We need 4 bytes to validate the header
      if (iLength < 4) return false;
      // Validate header
      if (pcData[0] != 'V' || 
          pcData[1] != 'S' ||
          pcData[2] != 'V' ||
          pcData[3] != 0x01) return false;
      else {
         pcData += 4;
         iLength -= 4;
         m_oState = VALID;
      }
   }

   // Read TOC into root chunk
   unsigned int iUsed = 0;
   if (m_oState == VALID) {
      if (!m_oRoot.CreateChunk(pcData,iLength,iUsed)) return false;
      else {
         pcData += iUsed;
         iLength -= iUsed;
         m_oState = LOADING;
      }
   }


   // Read setup data
   if (m_oState == LOADING) {
      if (m_pChunk == NULL) m_pChunk = new CIChunk();
      unsigned int iUsed = 0;
      if (!m_pChunk->CreateChunk(pcData,iLength,iUsed,true)) return false;
      else m_oState = READY;
   }

   // Load data if we're ready to
   if (m_oState == READY) {

      if (m_pChunk->Type() != CHUNK_SETUP) {
         delete m_pChunk;
         m_pChunk = NULL;
         return false;
      }

      // Read number of beams and slabs
      const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_BEAMS);
      if (pChunk) {
         m_iNumBeams = *reinterpret_cast<const unsigned int*>(pChunk->Data());
      }
      pChunk = m_pChunk->SubChunk(CHUNK_SLABS);
      if (pChunk) {
         m_iNumSlabs = *reinterpret_cast<const unsigned int*>(pChunk->Data());
      }

      // Create groups
      pChunk = m_pChunk->SubChunk(CHUNK_GROUPS);
      if (pChunk) {
         const unsigned char* pcData = pChunk->Data();
         int iNumGroups = *reinterpret_cast<const unsigned int*>(pcData);
         pcData += sizeof(unsigned int);
         for (int i=0; i<iNumGroups; i++) {
            CGroup oGroup;
            oGroup.m_fTemperature = 0;
            oGroup.m_oType = *pcData++ == 0x00 ? ELEMENT_BEAM : ELEMENT_SLAB;
            oGroup.m_pfColour[0] = (float)rand() / (float)RAND_MAX;
            oGroup.m_pfColour[1] = (float)rand() / (float)RAND_MAX;
            oGroup.m_pfColour[2] = (float)rand() / (float)RAND_MAX;
            m_oGroups.push_back(oGroup);
         }
      }

   }

   return true;
}

bool CDataManager::FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength) {
   bool bOK = m_oRoot.FrameInfo(iFrame,iOffset,iLength);
   // Add header length to offset;
   iOffset += 4;
   return bOK;
}

bool CDataManager::LoadFrame(const unsigned char* pcData, unsigned int iLength) {
   // Release existing data
   if (m_pChunk) delete m_pChunk;
   m_pChunk = NULL;
   // Load frame data from passed memory chunk
   m_pChunk = new CIChunk();
   unsigned int iUsed = 0;
   if (!m_pChunk->CreateChunk(pcData,iLength,iUsed,true) || m_pChunk->Type() != CHUNK_FRAME) {
      delete m_pChunk;
      m_pChunk = NULL;
      return false;
   }

   // Load temperature data
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_TEMP);
   if (pChunk) {
      const unsigned char* pcData = pChunk->Data();
      pcData += sizeof(unsigned int);
      for (int i=0; i<NumGroups(); i++) {
         m_oGroups[i].m_fTemperature = *reinterpret_cast<const float*>(pcData);
         pcData += sizeof(float);
      }
   }

   // Done
   return true;
}

unsigned int CDataManager::NumFrames(void) {
   return m_oRoot.NumFrames();
}

unsigned int CDataManager::NumNodes(void) {
   if (m_pChunk == NULL) return NULL;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_NODES);
   if (pChunk) {
      return *reinterpret_cast<const unsigned int*>(pChunk->Data());
   }
   return 0;
}

const float* CDataManager::NodePositions(void) {
   if (m_pChunk == NULL) return NULL;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_NODES);
   if (pChunk) {
      const unsigned char* pcData = pChunk->Data();
      pcData += sizeof(unsigned int);
      return reinterpret_cast<const float*>(pcData);
   }
   return NULL;
}

const float* CDataManager::NodeDisplacements() {
   if (m_pChunk == NULL) return NULL;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_NODEDISP);
   if (pChunk) {
      const unsigned char* pcData = pChunk->Data();
      pcData += sizeof(unsigned int);
      return reinterpret_cast<const float*>(pcData);
   }
   return NULL;
}

unsigned int CDataManager::NumGroups(void) {
   return m_oGroups.size();
}

float CDataManager::GroupTemp(unsigned int iGroup) {
   if (iGroup == 0) return 0;
   iGroup--;
   return m_oGroups[iGroup].m_fTemperature;
}

TElementType CDataManager::GroupType(unsigned int iGroup) {
   if (iGroup == 0) return ELEMENT_NONE;
   iGroup--;
   return m_oGroups[iGroup].m_oType;
}

const float* CDataManager::GroupColour(unsigned int iGroup) {
   if (iGroup == 0) return NULL;
   iGroup--;
   return m_oGroups[iGroup].m_pfColour;
}

void CDataManager::StressRange(float& fMin, float& fMax) {
   if (m_pChunk == NULL) return;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_STRESSR);
   if (pChunk) {
      const float *pfData = reinterpret_cast<const float*>(pChunk->Data());
      fMin = *pfData++;
      fMax = *pfData++;
   }
   return;
}

unsigned int CDataManager::NumBeams(void) {
   return m_iNumBeams;
}

const unsigned int* CDataManager::BeamNodes(int iBeam) {
   if (m_pChunk == NULL) return NULL;
   iBeam--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_BEAMS);
   if (pChunk) {
      const unsigned int* piData = reinterpret_cast<const unsigned int*>(pChunk->Data());
      int iNumBeams = *piData++;
      if (iBeam < iNumBeams) {
         return (piData + (iBeam*3) + 1);
      }
   }
   return NULL;
}

unsigned int CDataManager::BeamGroup(int iBeam) {
   if (m_pChunk == NULL) return 0;
   iBeam--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_BEAMS);
   if (pChunk) {
      const unsigned int* piData = reinterpret_cast<const unsigned int*>(pChunk->Data());
      int iNumBeams = *piData++;
      if (iBeam < iNumBeams) {
         return *(piData + (iBeam*3));
      }
   }
   return 0;
}

void CDataManager::BeamSizes(int iGroup, float& fHeight, float& fWidth, float& fFlange, float& fWeb) {
   if (m_pChunk == NULL) return;
   iGroup--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_BEAMSIZE);
   if (pChunk) {
      const unsigned char* pcData = pChunk->Data();
      int iNumGroups = *reinterpret_cast<const unsigned int*>(pcData);
      pcData += sizeof(unsigned int);
      pcData += iGroup * 4*sizeof(float);
      if (iGroup < iNumGroups) {
         fHeight = *reinterpret_cast<const float*>(pcData);
         pcData += sizeof(float);
         fWidth = *reinterpret_cast<const float*>(pcData);
         pcData += sizeof(float);
         fFlange = *reinterpret_cast<const float*>(pcData);
         pcData += sizeof(float);
         fWeb = *reinterpret_cast<const float*>(pcData);
         pcData += sizeof(float);
      }
   }
}

const float* CDataManager::BeamStresses(unsigned int iBeam) {
   if (m_pChunk == NULL) return NULL;
   iBeam--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_BEAMFORC);
   if (pChunk) {
      const float* pfData = reinterpret_cast<const float*>(pChunk->Data() + sizeof(unsigned int));
      pfData += 6 * iBeam;
      return pfData;
   }
   return NULL;
}

unsigned int CDataManager::NumSlabs(void) {
   return m_iNumSlabs;
}

const unsigned int* CDataManager::SlabNodes(int iSlab) {
   if (m_pChunk == NULL) return NULL;
   iSlab--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_SLABS);
   if (pChunk) {
      const unsigned int* piData = reinterpret_cast<const unsigned int*>(pChunk->Data());
      int iNumSlabs = *piData++;
      if (iSlab < iNumSlabs) {
         return (piData + (iSlab*10) + 1);
      }
   }
   return NULL;
}

unsigned int CDataManager::SlabGroup(int iSlab) {
   if (m_pChunk == NULL) return 0;
   iSlab--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_SLABS);
   if (pChunk) {
      const unsigned int* piData = reinterpret_cast<const unsigned int*>(pChunk->Data());
      int iNumSlabs = *piData++;
      if (iSlab < iNumSlabs) {
         return *(piData + (iSlab*10));
      }
   }
   return 0;
}

void CDataManager::SlabSizes(int iGroup, float& fThickness) {
   if (m_pChunk == NULL) return;
   iGroup--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_SLABSIZE);
   if (pChunk) {
      const unsigned char* pcData = pChunk->Data();
      int iNumGroups = *reinterpret_cast<const unsigned int*>(pcData);
      pcData += sizeof(unsigned int);
      pcData += iGroup * sizeof(float);
      if (iGroup < iNumGroups) {
         fThickness = *reinterpret_cast<const float*>(pcData);
         pcData += sizeof(float);
      }
   }
}

const float* CDataManager::SlabStresses(unsigned int iSlab) {
   if (m_pChunk == NULL) return NULL;
   iSlab--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_SLABFORC);
   if (pChunk) {
      const float* pfData = reinterpret_cast<const float*>(pChunk->Data() + sizeof(unsigned int));
      pfData += 27 * iSlab;
      return pfData;
   }
   return NULL;
}

const unsigned char* CDataManager::SlabCracks(unsigned int iSlab, unsigned int) {
   if (m_pChunk == NULL) return NULL;
   iSlab--;
   const CIChunk* pChunk = m_pChunk->SubChunk(CHUNK_CRACKS);
   if (pChunk) {
      return pChunk->Data() + sizeof(unsigned int) + (9*iSlab*sizeof(unsigned char));
   }
   return NULL;
}
