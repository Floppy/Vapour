//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// DataManager.cpp
// 19/03/2002 - James Smith
//
// $Id: DataManager.cpp,v 1.3 2002/03/22 17:04:37 vap-james Exp $

#include "stdafx.h"
#include "DataManager.h"
#include "Element.h"

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
         0,0,0,
         10,0,0,
         25,0,0,
         0,0,0,
         15,0,0,
         25,0,0,
         10,0,0,
         25,0,0,
         25,0,0
      }
   },
   {
      {
         0,0,0,
         20,0,0,
         50,0,0,
         0,0,0,
         25,0,0,
         50,0,0,
         25,0,0,
         50,0,0,
         50,0,0
      }
   },
   {
      {
         0,0,0,
         50,0,0,
         100,0,0,
         0,0,0,
         75,0,0,
         100,0,0,
         50,0,0,
         100,0,0,
         100,0,0
      }
   }
};

///////////
// CDataManager

CDataManager::CDataManager() :
   m_pcBuffer(NULL),
   m_iNumFrames(0)
{
}

CDataManager::~CDataManager() {
   if (m_pcBuffer) delete [] m_pcBuffer;
}

bool CDataManager::Setup(const unsigned char* pcData, unsigned int iLength) {
   // Read in setup data
   // Initialise frame counters.
   m_iNumFrames = g_iNumFrames;
   return true;
}

void CDataManager::FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength) {
   iOffset = 0;
   iLength = 0;
   m_iCurrentFrame = iFrame;
}

bool CDataManager::LoadFrame(const unsigned char* pcData, unsigned int iLength) {
   if (true) {
      // Load frame data from passed memory chunk
      // Done
      return true;
   }
   else return false;
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
   return g_pfGroupTemps[m_iCurrentFrame][iGroup];
}

TElementType CDataManager::GroupType(unsigned int iGroup) {
   return g_ptGroupTypes[iGroup];
}

void CDataManager::StressRange(float& fMin, float& fMax) {
   fMin = 0;
   fMax = 100;
}

unsigned int CDataManager::NumBeams(void) {
   return g_iNumBeams;
}

const unsigned int* CDataManager::BeamNodes(int iBeam) {
   return g_piBeamNodes[iBeam];
}

unsigned int CDataManager::BeamGroup(int iBeam) {
   return g_piBeamGroups[iBeam];
}

void CDataManager::BeamSizes(int iGroup, float& fHeight, float& fWidth, float& fFlange, float& fWeb) {
   fHeight = g_fBeamHeight;
   fWidth = g_fBeamWidth;
   fFlange = g_fFlange;
   fWeb = g_fWeb;
}

const float* CDataManager::BeamStresses(unsigned int iBeam) {
   return g_pfBeamStresses[m_iCurrentFrame][iBeam];
}

unsigned int CDataManager::NumSlabs(void) {
   return g_iNumSlabs;
}

const unsigned int* CDataManager::SlabNodes(int iSlab) {
   return g_piSlabNodes[iSlab];
}

unsigned int CDataManager::SlabGroup(int iSlab) {
   return g_piSlabGroups[iSlab];
}

void CDataManager::SlabSizes(int iGroup, float& fThickness) {
   fThickness = g_fSlabThickness;
}

const float* CDataManager::SlabStresses(unsigned int iSlab) {
   return g_pfSlabStresses[m_iCurrentFrame][iSlab];
}

const unsigned char* CDataManager::SlabCracks(unsigned int iSlab, unsigned int) {
   return g_pcSlabCracks[m_iCurrentFrame][iSlab];
}
