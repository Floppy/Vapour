//==========================================---
// VT Structure Visualisation File Converter
//------------------------------------------
// File converter for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.File Converter.VSV File IO"
//! userlevel =  Normal
//! file      = "Convert/OChunk.cpp"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: OChunk.cpp,v 1.5 2002/04/16 09:45:03 vap-james Exp $"

#include "OChunk.h"

#include <math.h>

#define SCALE_FACTOR 0.01;

COChunk::COChunk(TOChunkType oType) :
   m_iFrame(0),
   m_oType(oType),
   m_pcData(NULL),
   m_iLength(0)
{
}

COChunk::COChunk(const COChunk& oChunk) :
   m_iFrame(oChunk.m_iFrame),
   m_oType(oChunk.m_oType),
   m_pcData(NULL),
   m_iLength(oChunk.m_iLength),
   m_oSubChunks(oChunk.m_oSubChunks)
{
   // Alloocate memory for copied data
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return;
   // Copy data
   memcpy(m_pcData,oChunk.m_pcData,m_iLength);
}

COChunk::~COChunk() {
   if (m_pcData) delete [] m_pcData;
}

bool COChunk::Read(CInputData& oInput) {
   // Check that there isn't already data in this chunk.
   if (m_oType != CHUNK_NONE) 
      return false;

   // Read frame number
   m_iFrame = oInput.LoadIncr();

   // Process chunk
   switch (TranslateType(oInput.SectionType())) {
   case CHUNK_NONE:
      return false;
   case CHUNK_NODES:
      return ReadNodes(oInput);
   case CHUNK_BEAMSIZE:
      return ReadBeamSizes(oInput);
   case CHUNK_SLABSIZE:
      return ReadSlabSizes(oInput);
   case CHUNK_BEAMS:
      if (oInput.Beginning()) return ReadBeams(oInput);
      else return ReadSlabs(oInput);
   case CHUNK_TEMP:
      return ReadTemps(oInput);
   case CHUNK_NODEDISP:
      return ReadNodeDisplacements(oInput);
   case CHUNK_BEAMFORC:
      return ReadBeamForces(oInput);
   case CHUNK_SLABFORC:
      return ReadSlabForces(oInput);
   case CHUNK_CRACKS:
      return ReadCracks(oInput);
   default:
      return false;
   }

}

TOChunkType COChunk::TranslateType(const char* pcType) {
   if (strcmp(pcType,"HEADER")                            == 0) return CHUNK_NONE;
   if (strcmp(pcType,"NODAL GEOMETRY")                    == 0) return CHUNK_NODES;
   if (strcmp(pcType,"SECTION SIZES")                     == 0) return CHUNK_BEAMSIZE;
   if (strcmp(pcType,"MATERIAL PROPERTIES")               == 0) return CHUNK_NONE;
   if (strcmp(pcType,"RESIDUAL STRESSES")                 == 0) return CHUNK_NONE;
   if (strcmp(pcType,"LAYERED SLAB")                      == 0) return CHUNK_SLABSIZE;
   if (strcmp(pcType,"MEMBER DATA")                       == 0) return CHUNK_BEAMS;
   if (strcmp(pcType,"BOUNDARY CONDITIONS")               == 0) return CHUNK_NONE;
   if (strcmp(pcType,"JOINT LOADS")                       == 0) return CHUNK_NONE;
   if (strcmp(pcType,"TEMPERATURES")                      == 0) return CHUNK_TEMP;
   if (strcmp(pcType,"SLAB TEMPERATURES")                 == 0) return CHUNK_TEMP;
   if (strcmp(pcType,"NODAL DISPLACEMENTS")               == 0) return CHUNK_NODEDISP;
   if (strcmp(pcType,"INTERNAL FORCES")                   == 0) return CHUNK_BEAMFORC;
   if (strcmp(pcType,"9_NODED_SLAB_CRACKS")               == 0) return CHUNK_CRACKS;
   if (strcmp(pcType,"9_NODED_PRINCIPAL_MEMBRANE_FORCES") == 0) return CHUNK_SLABFORC;
   return CHUNK_NONE;
}

bool COChunk::ReadNodes(CInputData& oInput) {   
   // Extract data
   std::vector<float*> oList;
   while (oInput.DataReady()) {
      // Read and discard node number
      oInput.GetUInt();
      // Create new node
      float* pEntry = NULL;
      pEntry = new float[3];
      if (pEntry == NULL) return false;
      // Load positions
      pEntry[2] = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[1] = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[0] = oInput.GetFloat() * SCALE_FACTOR;
      // Load onto list
      oList.push_back(pEntry);
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*3*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_NODES);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of nodes
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   float* pfCurrent = reinterpret_cast<float*>(pcCurrent);
   for (std::vector<float*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      *pfCurrent++ = (*pEntry)[0];
      *pfCurrent++ = (*pEntry)[1];
      *pfCurrent++ = (*pEntry)[2];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_NODES;
   return true;
}

bool COChunk::ReadBeamSizes(CInputData& oInput) {
   // Extract data
   std::vector<float*> oList;
   while (oInput.DataReady()) {
      // Read and discard initial number
      oInput.GetUInt();
      // Create new size data block
      float* pEntry = NULL;
      pEntry = new float[4];
      if (pEntry == NULL) return false;
      // Load positions
      pEntry[0] = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[1] = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[2] = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[3] = oInput.GetFloat() * SCALE_FACTOR;
      // Load onto list
      oList.push_back(pEntry);
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*4*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_BEAMSIZE);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of size sets
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   float* pfCurrent = reinterpret_cast<float*>(pcCurrent);
   for (std::vector<float*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      *pfCurrent++ = (*pEntry)[0];
      *pfCurrent++ = (*pEntry)[1];
      *pfCurrent++ = (*pEntry)[2];
      *pfCurrent++ = (*pEntry)[3];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_BEAMSIZE;
   return true;
}

bool COChunk::ReadSlabSizes(CInputData& oInput) {
   // Extract data
   std::vector<float*> oList;
   // Skip initial 18 numbers that we don't need
   for (int i=0; i<18; i++) oInput.GetFloat();
   // Get per-group data
   while (oInput.DataReady()) {
      // Read number of layers
      unsigned int iNumLayers = oInput.GetUInt();
      // Skip 15 numbers
      for (i=0; i<15; i++) oInput.GetFloat();
      // Create new size data block
      float* pEntry = NULL;
      pEntry = new float[1];
      if (pEntry == NULL) return false;
      // Load thicknesses
      float fTop, fBottom;
      fBottom = oInput.GetFloat() * SCALE_FACTOR;
      for (i=0; i<iNumLayers-1; i++) oInput.GetFloat();
      fTop = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[0] = fTop - fBottom;
      // Load onto list
      oList.push_back(pEntry);
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*1*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_SLABSIZE);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of size sets
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   float* pfCurrent = reinterpret_cast<float*>(pcCurrent);
   for (std::vector<float*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      *pfCurrent++ = (*pEntry)[0];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_SLABSIZE;
   return true;
}

bool COChunk::ReadBeams(CInputData& oInput) {
   // Extract data
   std::vector<unsigned int*> oList;
   // Get per-beam data
   while (oInput.DataReady()) {
      // Read beam ID
      unsigned int iBeam = oInput.GetUInt();
      // Read mystery number
      unsigned int iType = oInput.GetUInt();
      // If it's not 1, we've finished with the beams
      if (iType != 1) {
         // Put read data back onto stream
         oInput.UngetUInt(iType);
         oInput.UngetUInt(iBeam);
         break;
      }
      // Skip 3 numbers
      for (int i=0; i<3; i++) oInput.GetFloat();
      // Create new data block
      unsigned int* pEntry = NULL;
      pEntry = new unsigned int[3];
      if (pEntry == NULL) return false;
      // Load node data
      for (i=0; i<2; i++)
         pEntry[i+1] = oInput.GetUInt();
      // Skip 4 numbers
      for (i=0; i<4; i++) oInput.GetFloat();
      // Load group number
      pEntry[0] = oInput.GetUInt();
      if (pEntry[0] > oInput.GetNumBeamGroups()) oInput.SetNumBeamGroups(pEntry[0]);
      // Skip 1 number
      oInput.GetFloat();
      // Load onto list
      oList.push_back(pEntry);
   }

   // Store number of beams
   oInput.SetNumBeams(oList.size());

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*3*sizeof(unsigned int);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_BEAMS);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of beams
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   unsigned int* piCurrent = reinterpret_cast<unsigned int*>(pcCurrent);
   for (std::vector<unsigned int*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      *piCurrent++ = (*pEntry)[0];
      *piCurrent++ = (*pEntry)[1];
      *piCurrent++ = (*pEntry)[2];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_BEAMS;
   return true;
}

bool COChunk::ReadSlabs(CInputData& oInput) {
   // Extract data
   std::vector<unsigned int*> oList;
   // Get per-beam data
   while (oInput.DataReady()) {
      // Read slab ID
      unsigned int iSlab = oInput.GetUInt();
      // Read mystery number
      unsigned int iType = oInput.GetUInt();
      // If it's not 6, we've finished with the slabs as well
      if (iType != 6) {
         // Put read data back onto stream
         oInput.UngetUInt(iType);
         oInput.UngetUInt(iSlab);
         break;
      }
      // Skip 3 numbers
      for (int i=0; i<3; i++) oInput.GetFloat();
      // Create new data block
      unsigned int* pEntry = NULL;
      pEntry = new unsigned int[10];
      if (pEntry == NULL) return false;
      // Load node data
      for (i=1; i<10; i++)
         pEntry[i] = oInput.GetUInt();
      // Load group number
      unsigned int iGroup = oInput.GetUInt();
      if (iGroup > oInput.GetNumSlabGroups()) oInput.SetNumSlabGroups(iGroup);
      pEntry[0] = iGroup + oInput.GetNumBeamGroups();
      // Skip 2 numbers
      oInput.GetFloat(); oInput.GetFloat();
      // Load onto list
      oList.push_back(pEntry);
   }

   // Store number of slabs
   oInput.SetNumSlabs(oList.size());

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*10*sizeof(unsigned int);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_SLABS);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of beams
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   unsigned int* piCurrent = reinterpret_cast<unsigned int*>(pcCurrent);
   for (std::vector<unsigned int*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      for (int i=0; i<10; i++)
         *piCurrent++ = (*pEntry)[i];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_SLABS;
   return true;
}

bool COChunk::ReadTemps(CInputData& oInput) {
   // Extract data
   std::vector<float> oList;
   // Get beam group data
   int iCount = 0;
   while (oInput.DataReady() && iCount++ < oInput.GetNumBeamGroups()) {
      // Read group ID
      oInput.GetUInt();
      // Load temperature data
      oList.push_back(oInput.GetFloat());
      // Skip 12 numbers
      for (int i=0; i<12; i++) oInput.GetFloat();
   }
   oInput.NextSection();
   if (TranslateType(oInput.SectionType()) != CHUNK_TEMP) return false;
   // Get slab group data
   iCount = 0;
   while (oInput.DataReady() && iCount++ < oInput.GetNumSlabGroups()) {
      // Read group ID
      oInput.GetUInt();
      // Load temperature data
      oList.push_back(oInput.GetFloat());
      // Skip 12 numbers
      for (int i=0; i<12; i++) oInput.GetFloat();
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_TEMP);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of temperatures
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   float* pfCurrent = reinterpret_cast<float*>(pcCurrent);
   for (std::vector<float>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      *pfCurrent++ = *pEntry;
   }

   // Done
   m_oType = CHUNK_TEMP;
   return true;
}

bool COChunk::ReadNodeDisplacements(CInputData& oInput) {
   // Extract data
   std::vector<float*> oList;
   while (oInput.DataReady()) {
      // Read and discard node number
      oInput.GetUInt();
      // Create new node
      float* pEntry = NULL;
      pEntry = new float[3];
      if (pEntry == NULL) return false;
      // Load positions
      pEntry[2] = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[1] = oInput.GetFloat() * SCALE_FACTOR;
      pEntry[0] = oInput.GetFloat() * SCALE_FACTOR;
      // Discard next 8 numbers
      for (int i=0; i<8; i++) oInput.GetFloat();
      // Load onto list
      oList.push_back(pEntry);
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*3*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_NODEDISP);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of nodes
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   float* pfCurrent = reinterpret_cast<float*>(pcCurrent);
   for (std::vector<float*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      *pfCurrent++ = (*pEntry)[0];
      *pfCurrent++ = (*pEntry)[1];
      *pfCurrent++ = (*pEntry)[2];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_NODEDISP;
   return true;
}

bool COChunk::ReadBeamForces(CInputData& oInput) {
   // Extract data
   std::vector<float*> oList;
   int iCount = 0;
   while (oInput.DataReady() && iCount < oInput.GetNumBeams()) {
      // Read and discard beam number
      oInput.GetUInt();
      // Read and discard node number
      oInput.GetUInt();
      // Create new force set
      float* pEntry = NULL;
      pEntry = new float[6];
      if (pEntry == NULL) return false;
      // Load forces
      pEntry[2] = fabs(oInput.GetFloat());
      pEntry[1] = fabs(oInput.GetFloat());
      pEntry[0] = fabs(oInput.GetFloat());
      // Skip 9 numbers
      for (int j=0; j<9; j++) oInput.GetFloat();
      pEntry[5] = fabs(oInput.GetFloat());
      pEntry[4] = fabs(oInput.GetFloat());
      pEntry[3] = fabs(oInput.GetFloat());
      // Skip 8 numbers
      for (j=0; j<8; j++) oInput.GetFloat();
      // Set minimum and maximum stress values
      for (j=0; j<6; j++)
         oInput.AddToStressRange(pEntry[j]);
      // Load onto list
      oList.push_back(pEntry);
      iCount++;
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*6*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_BEAMFORC);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of force sets
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   float* pfCurrent = reinterpret_cast<float*>(pcCurrent);
   for (std::vector<float*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      for (int i=0; i<6; i++)
         *pfCurrent++ = (*pEntry)[i];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_BEAMFORC;
   return true;
}

bool COChunk::ReadSlabForces(CInputData& oInput) {
   // Extract data
   std::vector<float*> oList;
   while (oInput.DataReady()) {
      // Read and discard node number
      oInput.GetUInt();
      // Create new force set
      float* pEntry = NULL;
      pEntry = new float[27];
      if (pEntry == NULL) return false;
      // Load forces
      for (int i=0; i<9; i++) {
         pEntry[(i*3) + 2] = fabs(oInput.GetFloat());
         pEntry[(i*3) + 1] = fabs(oInput.GetFloat());
         pEntry[(i*3) + 0] = fabs(oInput.GetFloat());
      }
      // Set minimum and maximum stress values
      for (int j=0; j<27; j++)
         oInput.AddToStressRange(pEntry[j]);
      // Load onto list
      oList.push_back(pEntry);
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*27*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_SLABFORC);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of force sets
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   float* pfCurrent = reinterpret_cast<float*>(pcCurrent);
   for (std::vector<float*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      for (int i=0; i<27; i++)
         *pfCurrent++ = (*pEntry)[i];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_SLABFORC;
   return true;
}

bool COChunk::ReadCracks(CInputData& oInput) {
   // Extract data
   std::vector<unsigned char*> oList;
   while (oInput.DataReady()) {
      // Read and discard node number
      oInput.GetUInt();
      // We're only dealing with cracks on the first layer
      if (oInput.GetUInt() == 1) {
         // Create new crack set
         unsigned char* pEntry = NULL;
         pEntry = new unsigned char[9];
         if (pEntry == NULL) return false;
         // Load positions
         for (int i=0; i<9; i++) {
            pEntry[i] = static_cast<unsigned char>(oInput.GetUInt());
            oInput.GetFloat();
         }
         // Load onto list
         oList.push_back(pEntry);
      }
      else {
         for (int i=0; i<18; i++) oInput.GetFloat();
      }
   }

   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int)) + oList.size()*9*sizeof(unsigned char);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_CRACKS);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of cracks
   *reinterpret_cast<unsigned int*>(pcCurrent) = oList.size();
   pcCurrent += sizeof(unsigned int);
   // Set chunk data
   for (std::vector<unsigned char*>::iterator pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      for (int i=0; i<9; i++) 
         *pcCurrent++ = (*pEntry)[i];
   }

   // Dump temporary data
   for (pEntry = oList.begin(); pEntry != oList.end(); pEntry++) {
      delete *pEntry;      
   }
   oList.empty();
   // Done
   m_oType = CHUNK_CRACKS;
   return true;
}

bool COChunk::CreateFrameInfoChunk(CInputData& oInput) {
   // Create chunk
   m_iLength = 2 + (2*sizeof(unsigned int));
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_FRAMEINF);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of frames
   *reinterpret_cast<unsigned int*>(pcCurrent) = oInput.NumFrames();
   // Done
   m_oType = CHUNK_FRAMEINF;
   return true;
}

bool COChunk::CreateGroupChunk(CInputData& oInput) {
   // Create chunk
   int iNumGroups = oInput.GetNumBeamGroups() + oInput.GetNumSlabGroups();
   m_iLength = 2 + (2*sizeof(unsigned int)) + iNumGroups*sizeof(unsigned char);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_GROUPS);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set number of cracks
   *reinterpret_cast<unsigned int*>(pcCurrent) = iNumGroups;
   pcCurrent += sizeof(unsigned int);
   // Set group data
   for (int i=0; i<oInput.GetNumBeamGroups(); i++)
      *pcCurrent++ = static_cast<unsigned char>(0x00);
   for (i=0; i<oInput.GetNumSlabGroups(); i++)
      *pcCurrent++ = static_cast<unsigned char>(0x01);
   // Done
   m_oType = CHUNK_GROUPS;
   return true;
}

bool COChunk::CreateStressChunk(CInputData& oInput) {
   // Create chunk
   m_iLength = 2 + sizeof(unsigned int) + 2*sizeof(float);
   m_pcData = new unsigned char[m_iLength];
   if (m_pcData == NULL) return false;
   unsigned char* pcCurrent = m_pcData;
   // Set chunk type
   *pcCurrent++ = static_cast<unsigned char>(CHUNK_STRESSR);
   // Set length
   *reinterpret_cast<unsigned int*>(pcCurrent) = m_iLength;
   pcCurrent += sizeof(unsigned int);
   // Set number of subchunks
   *pcCurrent++ = static_cast<unsigned char>(0xFF);
   // Set minimum
   *reinterpret_cast<float*>(pcCurrent) = oInput.GetMinStress();
   pcCurrent += sizeof(float);
   // Set maximum
   *reinterpret_cast<float*>(pcCurrent) = oInput.GetMaxStress();
   // Done
   m_oType = CHUNK_STRESSR;
   return true;
}

bool COChunk::AddSubChunk(const COChunk* pSubChunk) {
   if (m_iLength == 0) {
      m_iLength = 2 + sizeof(unsigned int);
   }
   m_iLength += pSubChunk->Length();
   m_iLength += 1;
   m_iLength += sizeof(unsigned int);
   m_oSubChunks.push_back(pSubChunk);
   return true;
}

bool COChunk::Write(ofstream& oOutput) const {
   const char* pcData = NULL;
   bool bOK = true;
   if (m_oSubChunks.size() > 0) {
      // Calculate sizes and write header
      oOutput << static_cast<unsigned char>(m_oType);
      pcData = reinterpret_cast<const char*>(&m_iLength);
      oOutput.write(pcData,sizeof(unsigned int));
      oOutput << static_cast<unsigned char>(m_oSubChunks.size());
      // Write TOC
      unsigned int iOffset = 2 + sizeof(unsigned int) + (m_oSubChunks.size() * (1 + sizeof(unsigned int)));
      pcData = reinterpret_cast<const char*>(&iOffset);
      for (std::vector<const COChunk*>::const_iterator pSubChunk = m_oSubChunks.begin(); pSubChunk != m_oSubChunks.end(); pSubChunk++) {
         oOutput << static_cast<unsigned char>((*pSubChunk)->Type());
         oOutput.write(pcData,sizeof(unsigned int));
         iOffset += (*pSubChunk)->Length();
      }
      // Write subchunks
      for (pSubChunk = m_oSubChunks.begin(); pSubChunk != m_oSubChunks.end(); pSubChunk++)
         bOK &= (*pSubChunk)->Write(oOutput);
      return bOK;
   }
   else {
      // No subchunks - just write the chunk data to the stream
      oOutput.write(reinterpret_cast<const char *>(m_pcData),m_iLength);
      return true;
   }
}

const COChunk* COChunk::SubChunk(TOChunkType oType) const {
   for (std::vector<const COChunk*>::const_iterator pEntry=m_oSubChunks.begin(); pEntry!=m_oSubChunks.end(); pEntry++) {
      if ((*pEntry)->m_oType == oType) return *pEntry;
   }
   return NULL;
}

void COChunk::AddDisplacements(float* pfDisplacements) {
   // Check chunk type
   if (m_oType != CHUNK_NODEDISP) return;
   // Get pointer to displacement data
   unsigned char* pcData = m_pcData + 2 + sizeof(unsigned int);
   // Get number of displacements
   unsigned int iNumValues = *reinterpret_cast<unsigned int*>(pcData) * 3;
   pcData += sizeof(unsigned int);
   // Get displacement pointer
   float* pfData = reinterpret_cast<float*>(pcData);
   // Add displacements one by one
   for (unsigned int i=0; i<iNumValues; i++) {
      *pfData += *pfDisplacements;
      *pfDisplacements = *pfData;
      pfData++;
      pfDisplacements++;
   }
   // Done
   return;
}

unsigned int COChunk::NumNodes(void) const {
   // Check chunk type
   if (m_oType != CHUNK_NODES) return 0;
   // Get pointer to displacement data
   unsigned char* pcData = m_pcData + 2 + sizeof(unsigned int);
   // Get number of displacements
   return *reinterpret_cast<unsigned int*>(pcData);
}

void COChunk::ZeroDisplacements(void) {
   // Check chunk type
   if (m_oType != CHUNK_NODEDISP) return;
   // Get pointer to displacement data
   unsigned char* pcData = m_pcData + 2 + sizeof(unsigned int);
   // Get number of displacements
   unsigned int iNumValues = *reinterpret_cast<unsigned int*>(pcData) * 3;
   pcData += sizeof(unsigned int);
   // Get displacement pointer
   float* pfData = reinterpret_cast<float*>(pcData);
   // Zero displacements one by one
   for (unsigned int i=0; i<iNumValues; i++) {
      *pfData = 0.0f;
      pfData++;
   }
   // Done
   return;
}