//==========================================---
// VT Structure Visualisation File Converter
//------------------------------------------
// File converter for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.File Converter.VSV File IO"
//! userlevel =  Normal
//! file      = "Convert/OChunk.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: OChunk.h,v 1.5 2002/04/04 11:53:43 vap-warren Exp $"

#ifndef __VTSTRUCVIS_OCHUNK__
#define __VTSTRUCVIS_OCHUNK__

#include "InputData.h"
#include <iostream.h>
#include <fstream.h>
#include <vector>

//: Chunk Types
enum TOChunkType {
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
   CHUNK_CRACKS   = 0x26
};

//: A output-specific data chunk
// This class provides an interface to a VSV file data chunk for output purposes.

class COChunk {

public:

   //:-------------------------
   //: Construction/Destruction

   COChunk(TOChunkType oType = CHUNK_NONE);
   //: Constructor
   //!param: oType - if specified, a chunk of that type is created. Otherwise, the chunk takes it's type from the data loaded into it.

   COChunk(const COChunk& oChunk);
   //: Copy constructor
   // Copies chunk data into a new chunk.
   // Does NOT perform a deep copy of subchunks.
   //!param: oChunk - The chunk to copy.

   ~COChunk();
   //: Destructor

   //:---------
   //: Data I/O

   bool Read(CInputData& oInput);
   //: Read chunk data from the input file
   // The chunk will load the next set of data from the file as appropriate.
   //!param: oInput - The input data class.

   TOChunkType Type(void) const {return m_oType;}
   //: The type of the chunk

   unsigned int Length(void) const {return m_iLength;}
   //: The length of the chunk data

   bool AddSubChunk(const COChunk* pSubChunk);
   //: Add a subchunk.
   //!param: pSubChunk - the subchunk to add
   //!param: return - true if successful

   const COChunk* SubChunk(TOChunkType oType) const;
   //: Get a subchunk with the specified type

   bool Write(ofstream& oOutput) const;
   //: Write to stream
   // Writes the chunk and it's children to the output stream.
   // The output stream should have ios:binary set.

   unsigned int Frame(void) const {return m_iFrame;}
   //: The frame number of the chunk

   bool CreateFrameInfoChunk(CInputData& oInput);
   //: Creates a frame info chunk from the input data
   //!param: oInput - The input data class.

   bool CreateGroupChunk(CInputData& oInput);
   //: Creates a group info chunk using the input data
   //!param: oInput - The input data class.

   bool CreateStressChunk(CInputData& oInput);
   //: Creates a stress range chunk using the input data
   //!param: oInput - The input data class.

   //:----------------
   //: Data Processing

   void AddDisplacements(float* pfDisplacements);
   //: Add node displacments
   // Adds the passed displacements onto the displacements stored in the chunk.
   // Only works on CHUNK_NODEDISP chunks.
   //!param: pfDisplacements - and array of floating-point values to add to the chunk data. After the addition, the resultant data is stored back into pfDisplacements.

   unsigned int NumNodes(void) const;
   //: How many nodes are there in the chunk?
   // Only works on CHUNK_NODES chunks.

   void ZeroDisplacements(void);
   //: Zeroes displacement values 
   // Only works on CHUNK_NODEDISP chunks.

private:

   TOChunkType TranslateType(const char* pcType);
   //: Translates a string into a chunk type

   bool ReadNodes(CInputData& oInput);
   //: Read a CHUNK_NODES chunk from input
   //!param: oInput - The input data class.

   bool ReadBeamSizes(CInputData& oInput);
   //: Read a CHUNK_BEAMSIZE chunk from input
   //!param: oInput - The input data class.

   bool ReadSlabSizes(CInputData& oInput);
   //: Read a CHUNK_SLABSIZE chunk from input
   //!param: oInput - The input data class.

   bool ReadBeams(CInputData& oInput);
   //: Read a CHUNK_BEAMS chunk from input
   //!param: oInput - The input data class.

   bool ReadSlabs(CInputData& oInput);
   //: Read a CHUNK_SLABS chunk from input
   //!param: oInput - The input data class.

   bool ReadTemps(CInputData& oInput);
   //: Read a CHUNK_TEMP chunk from input
   //!param: oInput - The input data class.

   bool ReadNodeDisplacements(CInputData& oInput);
   //: Read a CHUNK_NODEDISP chunk from input
   //!param: oInput - The input data class.

   bool ReadBeamForces(CInputData& oInput);
   //: Read a CHUNK_BEAMFORC chunk from input
   //!param: oInput - The input data class.

   bool ReadSlabForces(CInputData& oInput);
   //: Read a CHUNK_SLABFORC chunk from input
   //!param: oInput - The input data class.

   bool ReadCracks(CInputData& oInput);
   //: Read a CHUNK_CRACKS chunk from input
   //!param: oInput - The input data class.

protected:

   std::vector<const COChunk*> m_oSubChunks;
   //: A list of subchunks

   TOChunkType m_oType;
   //: The type of the chunk.
   
   int m_iFrame;
   //: The frame number of the chunk
   // A value of 0 means this is a setup chunk.

   unsigned char* m_pcData;
   //: Raw chunk data

   unsigned int m_iLength;
   //: Length of chunk data
};

#endif //__VTSTRUCVIS_OCHUNK__