//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// DataManager.cpp
// 19/03/2002 - James Smith
//
// $Id: DataManager.cpp,v 1.10 2002/03/26 20:10:48 vap-james Exp $

#include "stdafx.h"
#include "DataManager.h"
#include "Element.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////
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
const float g_pfNodeDisplacements[g_iNumFrames][27] = {
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

const g_iNumGroups = 4;

const TElementType g_ptGroupTypes[g_iNumGroups] = {
   BEAM, SLAB
};
const float g_pfGroupTemps[g_iNumFrames][g_iNumGroups] = {
   {30,30,30,30},{40,40,40,40},{60,60,60,50},{80,80,80,60}
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
const unsigned int g_piBeamGroups[g_iNumBeams] = {
   1,1,2,2,3,3
};
const float g_pfBeamStresses[g_iNumFrames][g_iNumBeams][6] = {
   {
      {
         0,0,0,
         0,0,0
      },
      {
         0,0,0,
         0,0,0
      },
      {
         0,0,0,
         0,0,0
      },
      {
         0,0,0,
         0,0,0
      },
      {
         0,0,0,
         0,0,0
      },
      {
         0,0,0,
         0,0,0
      }
   },
   {
      {
         0,0,0,
         10,0,0
      },
      {
         10,0,0,
         25,0,0
      },
      {
         0,0,0,
         0,0,0,
      },
      {
         0,0,0,
         10,0,0,
      },
      {
         10,0,0,
         25,0,0
      },
      {
         25,0,0,
         25,0,0
      }
   },
   {
      {
         0,0,0,
         20,0,0
      },
      {
         20,0,0,
         50,0,0
      },
      {
         0,0,0,
         10,0,0
      },
      {
         10,0,0,
         25,0,0
      },
      {
         25,0,0,
         50,0,0
      },
      {
         50,0,0,
         50,0,0
      }
   },
   {
      {
         0,0,0,
         50,0,0
      },
      {
         50,0,0,
         100,0,0
      },
      {
         0,0,0,
         25,0,0
      },
      {
         25,0,0,
         50,0,0
      },
      {
         50,0,0,
         100,0,0
      },
      {
         100,0,0,
         100,0,0
      }
   }
};

const unsigned int g_iNumSlabs = 1;
const float g_fSlabThickness = 0.3f;
const unsigned int g_piSlabNodes[1][9] = {
   {9, 6, 3, 2, 1, 4, 7, 8, 5}
};
const unsigned int g_piSlabGroups[g_iNumSlabs] = {
   4
};
const unsigned char g_pcSlabCracks[g_iNumFrames][g_iNumSlabs][9] = {
   {{0,0,0,0,0,0,0,0,0}},
   {{0,0,0,3,0,0,0,0,0}},
   {{1,1,0,3,0,0,0,1,0}},
   {{2,2,1,3,0,0,1,2,0}}
};
const float g_pfSlabStresses[g_iNumFrames][g_iNumSlabs][27] = {
   {
      {
         0,0,0,
         0,0,0,
         0,0,0,
         0,0,0,
         0,0,0,
         0,0,0,
         0,0,0,
         0,0,0,
         0,0,0
      }
   },
   {
      {
         25,0,0,
         25,0,0,
         25,0,0,
         10,0,0,
         0,0,0,
         0,0,0,
         10,0,0,
         25,0,0,
         15,0,0
      }
   },
   {
      {
         50,0,0,
         50,0,0,
         50,0,0,
         20,0,0,
         0,0,0,
         0,0,0,
         25,0,0,
         50,0,0,
         25,0,0
      }
   },
   {
      {
         100,0,0,
         100,0,0,
         100,0,0,
         50,0,0,
         0,0,0,
         0,0,0,
         50,0,0,
         100,0,0,
         75,0,0
      }
   }
};

float g_fMinStress = 0;
float g_fMaxStress = 100;

///////////
// CDataManager

typedef std::vector<CDataManager::CGroup>::iterator grpIter;

CDataManager::CDataManager() :
   m_pcBuffer(NULL),
   m_iNumFrames(0)//,
   //m_iBufferLength(0),
   //m_iDataSize(0),
   //m_iTOCLength(0)
{
}

CDataManager::~CDataManager() {
   if (m_pcBuffer) delete [] m_pcBuffer;
}

bool CDataManager::Setup(const unsigned char* pcData, unsigned int iLength) {
   /*// Is this the first lump of data?
   if (m_pcBuffer == NULL) {
      // Create a minimum buffer space
      m_pcBuffer = (unsigned char*) malloc(iLength);
      if (m_pcBuffer == NULL) return false;
      m_iBufferLength = iLength;
   }
   // Make sure buffer is big enough to take the data
   if (iLength + m_iDataSize > m_iBufferLength) {
      m_pcBuffer = (unsigned char*) realloc(m_pcBuffer,(iLength + m_iDataSize) * 2);
      if (m_pcBuffer == NULL) return false;
      m_iBufferLength = (iLength + m_iDataSize) * 2;
   }
   // Append data to buffer
   memcpy(m_pcBuffer+m_iDataSize,pcData,iLength);
   m_iDataSize += iLength;
   
   // Check the header
   if (m_iDataSize < 4) return false;
   if (m_pcBuffer[0] != 'A' ||
       m_pcBuffer[1] != 'S' ||
       m_pcBuffer[2] != 'V' ||
       m_pcBuffer[3] != 0x01) {
      // Header error
      return false;
   }

   // Make sure we've got a root chunk
   if (m_iDataSize < 5) return false;
   if (m_pcBuffer[4] != 0x00) {
      // Chunk error
      return false;
   }

   // Work out TOC size
   if (m_iDataSize < 10) return false;
   if (m_oTOC.capacity() == 0) {
      // Reserve space in both TOC and buffer
      unsigned char iNumTOCEntries = m_pcBuffer[9];
      m_oTOC.reserve(iNumTOCEntries);
      m_iTOCLength = 10 + (iNumTOCEntries * 5);
      if (m_iBufferLength < m_iTOCLength) {
         m_pcBuffer = (unsigned char*) realloc(m_pcBuffer,m_iTOCLength);
         if (m_pcBuffer == NULL) return false;
         m_iBufferLength = m_iTOCLength;
      }
   }

   // If we haven't got the entire TOC, stop
   if (m_iDataSize < m_iTOCLength) return false;
   // Create TOC
   unsigned int iNumTOCEntries = (m_iTOCLength - 10) / 5;
   for (int i=0; i<iNumTOCEntries; i++) {
      CTOCEntry oEntry;
      oEntry.m_oType = static_cast<TChunkType>(m_pcBuffer[10 + (i*5)]);
      oEntry.m_iOffset = *reinterpret_cast<unsigned int*>(m_pcBuffer + 11 + (i*5));
      m_oTOC.push_back(oEntry);
   }*/

   // Initialise frame counters.
   m_iNumFrames = g_iNumFrames;

   // Create groups
   for (int i=0; i<g_iNumGroups; i++) {
      CGroup oGroup;
      oGroup.m_fTemperature = 0;
      oGroup.m_oType = g_ptGroupTypes[i];
      oGroup.m_pfColour[0] = (float)rand() / (float)RAND_MAX;
      oGroup.m_pfColour[1] = (float)rand() / (float)RAND_MAX;
      oGroup.m_pfColour[2] = (float)rand() / (float)RAND_MAX;
      m_oGroups.push_back(oGroup);
   }

   return true;
}

bool CDataManager::FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength) {
   if (iFrame < m_iNumFrames) {
      // TESTING --------------
      m_iCurrentFrame = iFrame;
      // TESTING --------------
      iOffset = 0;
      iLength = 1;
      //iOffset = m_oTOC[iFrame+1].m_iOffset;
      //iLength = m_oTOC[iFrame+1].m_iLength;
      return true;
   }
   else return false;
}

bool CDataManager::LoadFrame(const unsigned char* pcData, unsigned int iLength) {
   if (m_pcBuffer) {
      delete [] m_pcBuffer;
      m_pcBuffer = NULL;
   }
   
   // Load frame data from passed memory chunk
   //m_pcBuffer = new unsigned char

   // Load temperature data
   for (int i=0; i<g_iNumGroups; i++) {
      m_oGroups[i].m_fTemperature = g_pfGroupTemps[m_iCurrentFrame][i];
   }
   
   // Done
   return true;
}

unsigned int CDataManager::NumFrames(void) {
   return m_iNumFrames;
}

unsigned int CDataManager::NumNodes(void) {
   return g_iNumNodes;
}

const float* CDataManager::NodePositions(void) {
   return g_pfDefaultNodes;
}

const float* CDataManager::NodeDisplacements() {
   return g_pfNodeDisplacements[m_iCurrentFrame];
}

unsigned int CDataManager::NumGroups(void) {
   return g_iNumGroups;
}

float CDataManager::GroupTemp(unsigned int iGroup) {
   return m_oGroups[iGroup-1].m_fTemperature;
}

TElementType CDataManager::GroupType(unsigned int iGroup) {
   return m_oGroups[iGroup-1].m_oType;
}

const float* CDataManager::GroupColour(unsigned int iGroup) {
   return m_oGroups[iGroup-1].m_pfColour;
}

void CDataManager::StressRange(float& fMin, float& fMax) {
   fMin = g_fMinStress;
   fMax = g_fMaxStress;
}

unsigned int CDataManager::NumBeams(void) {
   return g_iNumBeams;
}

const unsigned int* CDataManager::BeamNodes(int iBeam) {
   return g_piBeamNodes[iBeam-1];
}

unsigned int CDataManager::BeamGroup(int iBeam) {
   return g_piBeamGroups[iBeam-1];
}

void CDataManager::BeamSizes(int iGroup, float& fHeight, float& fWidth, float& fFlange, float& fWeb) {
   fHeight = g_fBeamHeight;
   fWidth = g_fBeamWidth;
   fFlange = g_fFlange;
   fWeb = g_fWeb;
}

const float* CDataManager::BeamStresses(unsigned int iBeam) {
   return g_pfBeamStresses[m_iCurrentFrame][iBeam-1];
}

unsigned int CDataManager::NumSlabs(void) {
   return g_iNumSlabs;
}

const unsigned int* CDataManager::SlabNodes(int iSlab) {
   return g_piSlabNodes[iSlab-1];
}

unsigned int CDataManager::SlabGroup(int iSlab) {
   return g_piSlabGroups[iSlab-1];
}

void CDataManager::SlabSizes(int iGroup, float& fThickness) {
   fThickness = g_fSlabThickness;
}

const float* CDataManager::SlabStresses(unsigned int iSlab) {
   return g_pfSlabStresses[m_iCurrentFrame][iSlab-1];
}

const unsigned char* CDataManager::SlabCracks(unsigned int iSlab, unsigned int) {
   return g_pcSlabCracks[m_iCurrentFrame][iSlab-1];
}
