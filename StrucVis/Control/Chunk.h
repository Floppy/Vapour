//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// Chunk.h
// 19/03/2002 - James Smith
//
// $Id: Chunk.h,v 1.8 2002/03/27 15:07:49 vap-james Exp $

#ifndef __VTSTRUCVIS_CHUNK__
#define __VTSTRUCVIS_CHUNK__

#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum TChunkType {
   NONE     = 0xFF,
   ROOT     = 0x00,
   //-------------
   // Setup Data
   SETUP    = 0x10,
   NODES    = 0x11,
   GROUPS   = 0x12,
   BEAMSIZE = 0x13,
   SLABSIZE = 0x14,
   BEAMS    = 0x15,
   SLABS    = 0x16,
   STRESSR  = 0x17,
   FRAMEINF = 0x18,
   //-------------
   // Frame Data
   FRAME    = 0x20,
   TEMP     = 0x21,
   TIME     = 0x22,
   NODEDISP = 0x23,
   BEAMFORC = 0x24,
   SLABFORC = 0x25,
   CRACKS   = 0x26,
};

class CChunk {
//#===--- Construction/Destruction
public:

   CChunk();
   // Constructor
   
   virtual ~CChunk();
   // Destructor

//#===--- Data I/O

   bool CreateChunk(const unsigned char* pcData, unsigned int iLength, unsigned int& iUsed, bool bLoadSubChunks = false);
   // Adds data to the chunk
   // Returns true if the chunk is complete
   // Length of unused data is returned in iSpare
   // Subchunks are loaded if bLoadSubChunks = true, otherwise only the TOC is loaded

   TChunkType Type(void) {return m_oType;}
   // The type of the chunk

   const unsigned char* Data(void) const;
   // Raw chunk data

   const CChunk* SubChunk(TChunkType oType) const;
   // Get a subchunk with the specified type

//#===--- Member Variables
protected:

   class CTOCEntry {
   public:

      CTOCEntry() : m_pChunk(NULL) {}

      ~CTOCEntry() {
         if (m_pChunk) delete m_pChunk;
      }

      TChunkType m_oType;
      unsigned int m_iOffset;
      unsigned int m_iLength;
      CChunk* m_pChunk;
   };
   // TOC Information

   std::vector<CTOCEntry> m_oTOC;
   // Table of contents

   unsigned char* m_pcBuffer;
   // Data buffer

   unsigned int m_iBufferLength;
   // The currently allocated size of the buffer

   unsigned int m_iDataSize;
   // The amount of data in the buffer

   unsigned int m_iDataProcessed;
   // The length of data that has been processed

   unsigned int m_iChunkLength;
   // The length of the chunk

   TChunkType m_oType;
   // The type of the chunk

   CChunk* m_pTempSubChunk;
   // Temporary storage for currently-building subchunk

};

class CRootChunk : public CChunk {

public:

   CRootChunk();
   // Constructor
   
   virtual ~CRootChunk();
   // Destructor

//#===--- Data I/O
   bool FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength);
   // Gets information for a particular frame

   unsigned int NumFrames(void);
   // The number of frame chunks in the file

   void Reset(void);
   // Resets all data

};


#endif // __VTSTRUCVIS_CHUNK__