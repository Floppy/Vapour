//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// Chunk.cpp
// 19/03/2002 - James Smith
//
// $Id: Chunk.cpp,v 1.3 2002/03/27 09:42:51 vap-james Exp $

#include "stdafx.h"
#include "Chunk.h"

#include <memory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////
// CChunk

CChunk::CChunk() :
   m_iBufferLength(0),
   m_iDataSize(0),
   m_pcBuffer(NULL),
   m_oType(NONE)
{
}

CChunk::~CChunk() {
   if (m_pcBuffer) delete [] m_pcBuffer;
}

bool CChunk::CreateChunk(const unsigned char* pcData, unsigned int iLength, unsigned int& iUsed, bool bLoadSubChunks) {

   // Is this the first lump of data?
   if (m_pcBuffer == NULL) {
      // Create a minimum buffer space
      m_pcBuffer = static_cast<unsigned char*>(malloc(iLength));
      if (m_pcBuffer == NULL) return false;
      m_iBufferLength = iLength;
   }
   // Make sure buffer is big enough to take the data
   if (iLength + m_iDataSize > m_iBufferLength) {
      m_pcBuffer = static_cast<unsigned char*>(realloc(m_pcBuffer,(iLength + m_iDataSize) * 2));
      if (m_pcBuffer == NULL) return false;
      m_iBufferLength = (iLength + m_iDataSize) * 2;
   }
   // Append data to buffer
   memcpy(m_pcBuffer+m_iDataSize,pcData,iLength);
   m_iDataSize += iLength;
   
   // Read type from data chunk
   if (m_iDataSize < 1) return false;
   m_oType = static_cast<TChunkType>(m_pcBuffer[0]);
   
   // Read chunk size
   if (m_iDataSize < 5) return false;
   unsigned int iSize = *reinterpret_cast<unsigned int*>(m_pcBuffer+1);

   // Read number of TOC entries?
   if (m_iDataSize < 6) return false;
   int iTOCLength = 6 + m_pcBuffer[5] * (1 + sizeof(unsigned int));
   // Check if we have all TOC data
   if (m_iDataSize < iTOCLength) {
      iUsed = iLength;
      return false;   
   }
   iUsed = iLength - (m_iDataSize - iTOCLength);

   // Create TOC
   unsigned char* pcCurrent = m_pcBuffer + 6;
   for (int i=0; i<m_pcBuffer[5]; i++) {
      CTOCEntry oEntry;
      oEntry.m_oType = static_cast<TChunkType>(*pcCurrent++);
      oEntry.m_iOffset = *reinterpret_cast<unsigned int*>(pcCurrent);
      pcCurrent += sizeof(unsigned int);
      if (i > 0) {
         m_oTOC.back().m_iLength = oEntry.m_iOffset - m_oTOC.back().m_iOffset;
         if (i < m_pcBuffer[5]-1) {
            oEntry.m_iLength = *reinterpret_cast<unsigned int*>(m_pcBuffer+1) - oEntry.m_iOffset;
         }
      }
      m_oTOC.push_back(oEntry);
   }

   // Load subchunks if necessary
   if (bLoadSubChunks) {
   }

   // Done
   return true;
}

/////////////
// CRootChunk

CRootChunk::CRootChunk() {
}

CRootChunk::~CRootChunk() {
}

bool CRootChunk::FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength) {
   if (iFrame < m_oTOC.size()-1) {
      iOffset = m_oTOC[iFrame+1].m_iOffset;
      iLength = m_oTOC[iFrame+1].m_iLength;
      return true;
   }
   else return false;
}

unsigned int CRootChunk::NumFrames(void) {
   return m_oTOC.size() - 1;
}

void CRootChunk::Reset(void) {
   // Dump raw data
   if (m_pcBuffer) delete [] m_pcBuffer;
   m_pcBuffer = NULL;
   m_iDataSize = 0;
   m_iBufferLength = 0;
   // Empty TOC
   m_oTOC.empty();
   return;
}
