//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management.VSV File IO"
//! userlevel =  Normal
//! file      = "Control/IChunk.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: Chunk.h,v 1.19 2002/04/04 21:37:43 vap-warren Exp $"

#ifndef __VTSTRUCVIS_ICHUNK__
#define __VTSTRUCVIS_ICHUNK__

#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum TIChunkType {
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
//: Chunk Types
//

//: An input data chunk
// This class provides an interface to a VSV file data chunk, specifically for file loading.

class CIChunk {

public:

   //:-------------------------
   //: Construction/Destruction

   CIChunk();
   //: Constructor
   
   virtual ~CIChunk();
   //: Destructor

   //:---------
   //: Data I/O

   bool CreateChunk(const unsigned char* pcData, unsigned int iLength, unsigned int& iUsed, bool bLoadSubChunks = false);
   //: Adds data to the chunk
   //!param: pcData - raw input data
   //!param: iLength - length of pcData
   //!param: iUsed - amount of data used is returned in this parameter
   //!param: bLoadSubChunks - if true, subchunks are loaded
   //!return: true if the chunk is complete, false if more data is required

   TIChunkType Type(void) {return m_oType;}
   //: The type of the chunk

   const unsigned char* Data(void) const;
   //: Access to raw chunk data

   const CIChunk* SubChunk(TIChunkType oType) const;
   //: Get a subchunk with the specified type

protected:

   //:-----------------
   //: Member Variables

   class CTOCEntry {
   public:

      CTOCEntry() : m_pChunk(NULL) {}

      ~CTOCEntry() {
         if (m_pChunk) delete m_pChunk;
      }

      TIChunkType m_oType;
      unsigned int m_iOffset;
      unsigned int m_iLength;
      CIChunk* m_pChunk;
   };
   //: TOC Information

   std::vector<CTOCEntry> m_oTOC;
   //: Table of contents

   unsigned char* m_pcBuffer;
   //: Data buffer

   unsigned int m_iBufferLength;
   //: The currently allocated size of the buffer

   unsigned int m_iDataSize;
   //: The amount of data in the buffer

   unsigned int m_iDataProcessed;
   //: The length of data that has been processed

   unsigned int m_iChunkLength;
   //: The length of the chunk

   TIChunkType m_oType;
   //: The type of the chunk

   CIChunk* m_pTempSubChunk;
   //: Temporary storage for currently-building subchunk

};

//: A specialised data chunk
// This class provides an interface to a VSV file root data chunk.

class CRootChunk : public CIChunk {

public:

   //:-------------------------
   //: Construction/Destruction

   CRootChunk();
   //: Constructor
   
   virtual ~CRootChunk();
   //: Destructor

   //:---------
   //: Data I/O

   bool FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength);
   //: Frame location information
   //!param: iFrame - the requested frame - between 0 and NumFrames-1.
   //!param: iOffset - the offset of the frame data is placed here.
   //!param: iLength - the length of the frame data is placed here.
   //!return: false if an invalid frame is requested, true otherwise.

   unsigned int NumFrames(void);
   //: The number of frame chunks in the file

   void Reset(void);
   //: Resets all data

};


#endif // __VTSTRUCVIS_ICHUNK__