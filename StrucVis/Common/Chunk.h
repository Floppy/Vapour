//==========================================---
// VT Structure Visualisation File Converter
//------------------------------------------
// File converter for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "File Converter.VSV File IO"
//! userlevel =  Normal
//! file      = "Convert/Chunk.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: Chunk.h,v 1.1 2002/04/03 12:31:23 vap-james Exp $"

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

   bool CreateChunk(const unsigned char* pcData, unsigned int iLength, unsigned int& iUsed, bool bLoadSubChunks = false);
   // Adds data to the chunk
   // Returns true if the chunk is complete
   // Length of unused data is returned in iSpare
   // Subchunks are loaded if bLoadSubChunks = true, otherwise only the TOC is loaded

   TChunkType Type(void) const {return m_oType;}
   // The type of the chunk

   unsigned int Length(void) const {return m_iChunkLength;}
   // Gets the length of the chunk

   bool AddSubChunk(CChunk* pSubChunk);
   // Adds a subchunk to this chunk.

   const unsigned char* Data(void) const;
   // Raw chunk data

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

   void ZeroDisplacements(void);
   // Zeroes displacement values for CHUNK_NODEDISP chunks.

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

   class CTOCEntry {
   public:

      CTOCEntry() : 
         m_pChunk(NULL) ,
         m_iOffset(0),
         m_iLength(0),
         m_oType(CHUNK_NONE) 
      {}

      CTOCEntry(const CTOCEntry& oEntry) : 
         m_pChunk(oEntry.m_pChunk),
         m_iOffset(oEntry.m_iOffset),
         m_iLength(oEntry.m_iLength),
         m_oType(oEntry.m_oType) 
      {}

      ~CTOCEntry() {
         //if (m_pChunk) delete m_pChunk;
      }

      TChunkType m_oType;
      unsigned int m_iOffset;
      unsigned int m_iLength;
      CChunk* m_pChunk;
   };
   // TOC Information

   std::vector<CTOCEntry> m_oTOC;
   // Table of contents
   // Essentally a list of subchunks

   unsigned char* m_pcData;
   // Data buffer

   unsigned int m_iBufferLength;
   // The currently allocated size of the buffer

   unsigned int m_iDataSize;
   // The amount of data in the buffer

   unsigned int m_iDataProcessed;
   // The length of data that has been processed

   unsigned int m_iChunkLength;
   // The length of the chunk

   int m_iFrame;
   // The frame number of the chunk
   // A value of 0 means this is a setup chunk.

   TChunkType m_oType;
   // The type of the chunk

   CChunk* m_pTempSubChunk;
   // Temporary storage for currently-building subchunk

};

#endif //__VTSTRUCVIS_CHUNK__