//==========================================---
// VT Structure Visualisation File Converter
//------------------------------------------
// File converter for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
// Chunk.cpp
// 19/03/2002 - James Smith
//
// $Id: Chunk.h,v 1.4 2002/03/31 17:26:02 vap-james Exp $

#ifndef __VTSTRUCVIS_CHUNK__
#define __VTSTRUCVIS_CHUNK__

#include "InputData.h"
#include <fstream>
#include <vector>

using namespace std;

enum TChunkType {
   CHUNK_NONE     = 0xFF,
   CHUNK_ROOT     = 0x00,
   //-------------
   // Setup Data
   CHUNK_SETUP    = 0x10,
   CHUNK_NODES    = 0x11,
   CHUNK_GROUPS   = 0x12,
   CHUNK_BEAMSIZE = 0x13,
   CHUNK_SLABSIZE = 0x14,
   CHUNK_BEAMS    = 0x15,
   CHUNK_SLABS    = 0x16,
   CHUNK_STRESSR  = 0x17,
   CHUNK_FRAMEINF = 0x18,
   //-------------
   // Frame Data
   CHUNK_FRAME    = 0x20,
   CHUNK_TEMP     = 0x21,
   CHUNK_TIME     = 0x22,
   CHUNK_NODEDISP = 0x23,
   CHUNK_BEAMFORC = 0x24,
   CHUNK_SLABFORC = 0x25,
   CHUNK_CRACKS   = 0x26,
};

class CChunk {

public:

   CChunk(TChunkType oType = CHUNK_NONE);
   // Constructor
   // If type is specified, a chunk of that type is created.
   // Otherwise, the chunk will take on the type of the 
   // data loaded into it.

   CChunk(const CChunk& oChunk);
   // Copy constructor
   // Copies chunk data into a new chunk.
   // Does NOT perform a deep copy of subchunks.

   ~CChunk();
   // Destructor

   bool Read(CInputData& oInput);
   // Reads chunk data from the input file

   TChunkType Type(void) const {return m_oType;}
   // The type of the chunk

   unsigned int Length(void) const {return m_iLength;}
   // Gets the length of the chunk

   bool AddSubChunk(const CChunk* pSubChunk);
   // Adds a subchunk to this chunk.

   const CChunk* SubChunk(TChunkType oType) const;
   // Get a subchunk with the specified type

   bool Write(ofstream& oOutput) const;
   // Writes the chunk and it's children to the output stream.
   // The output stream should have ios:binary set.

   unsigned int Frame(void) const {return m_iFrame;}
   // Gets the frame number

   bool CreateFrameInfoChunk(CInputData& oInput);
   // Creates a frame info chunk using the input data

   bool CreateGroupChunk(CInputData& oInput);
   // Creates a group info chunk using the input data

   bool CreateStressChunk(CInputData& oInput);
   // Creates a stress range chunk using the input data

   void AddDisplacements(float* pfDisplacements);
   // Adds the passed displacements onto the displacements stored in the chunk.
   // Only works on CHUNK_NODEDISP chunks.
   // After the addition, the resultant data is stored in pfDisplacements.

   unsigned int NumNodes(void) const;
   // How many nodes are there in the chunk?
   // Only works on CHUNK_NODES chunks

private:

   TChunkType TranslateType(const char* pcType);
   // Translates a string into a chunk type

   bool ReadNodes(CInputData& oInput);
   bool ReadBeamSizes(CInputData& oInput);
   bool ReadSlabSizes(CInputData& oInput);
   bool ReadBeams(CInputData& oInput);
   bool ReadSlabs(CInputData& oInput);
   bool ReadTemps(CInputData& oInput);
   bool ReadNodeDisplacements(CInputData& oInput);
   bool ReadBeamForces(CInputData& oInput);
   bool ReadSlabForces(CInputData& oInput);
   bool ReadCracks(CInputData& oInput);
   // Read a particular type of chunk data from the current input data section

protected:

   vector<const CChunk*> m_oSubChunks;
   // A list of subchunks

   TChunkType m_oType;
   // The type of the chunk.
   
   int m_iFrame;
   // The frame number of the chunk
   // A value of 0 means this is a setup chunk.

   unsigned char* m_pcData;
   // Raw chunk data

   unsigned int m_iLength;
   // Length of chunk data
};

#endif //__VTSTRUCVIS_CHUNK__