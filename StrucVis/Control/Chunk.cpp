//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// Chunk.cpp
// 19/03/2002 - James Smith
//
// $Id: Chunk.cpp,v 1.2 2002/03/27 02:19:31 vap-james Exp $

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

bool CChunk::CreateChunk(const unsigned char* pcData, unsigned int iLength) {

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

   // Is the chunk complete?
   if (m_iDataSize < iSize) return false;

   // Done
   return true;
}

/////////////
// CRootChunk

CRootChunk::CRootChunk() :
   m_iNumFrames(0)
{
}

CRootChunk::~CRootChunk() {
}

bool CRootChunk::FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength) {
   if (iFrame < m_iNumFrames) {
      iOffset = m_oTOC[iFrame+1].m_iOffset;
      iLength = m_oTOC[iFrame+1].m_iLength;
      return true;
   }
   else return false;
}

bool CRootChunk::CreateTOC(const unsigned char* pcData, unsigned int iLength, unsigned int& iUsed) {
   // Store data
   CreateChunk(pcData,iLength);
   // Check if we have any data
   if (m_iDataSize == 0) return false;
   // Check we have a root chunk
   if (m_oType != ROOT) return false;
   // Can we read the number of TOC entries?
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
   // Number of frames is one less than the number of TOC entries
   m_iNumFrames = m_oTOC.size() - 1;
   // Done
   return true;
}

unsigned int CRootChunk::NumFrames(void) {
   return m_iNumFrames;
}

void CRootChunk::Reset(void) {
   // Dump raw data
   if (m_pcBuffer) delete [] m_pcBuffer;
   m_pcBuffer = NULL;
   m_iDataSize = 0;
   m_iBufferLength = 0;
   // Empty TOC
   m_oTOC.empty();
   // Dump extra data
   m_iNumFrames = 0;
   return;
}
