//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Unreal.cpp - 21/08/2000 - James Smith
//	Unreal support classes implementation
//
// $Id: Unreal.cpp,v 1.0 2000/08/21 23:06:51 waz Exp $
//

// Windows includes/defines
#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Unreal.h"

// Sets the compact index to the passed value
CUnrealCompactIndex::CUnrealCompactIndex(int iIndex) {
   Set(iIndex);
   return;
}

// Creates a Compact Index with the passed values
CUnrealCompactIndex::CUnrealCompactIndex(unsigned char B0, unsigned char B1, unsigned char B2, unsigned char B3, unsigned char B4) {
   Set(B0,B1,B2,B3,B4);
   return;
}

// Sets the compact index to the passed value
void CUnrealCompactIndex::Set(int iIndex) {
   m_iNumBytes = 0;
   int iOriginal = iIndex;
   unsigned int V = abs(iIndex);
   m_pIndexBuffer[0] = ((iIndex>=0) ? 0 : 0x80) + ((V < 0x40) ? V : ((V & 0x3f)+0x40));
   m_iNumBytes++;
   if( m_pIndexBuffer[0] & 0x40 ) {
      V >>= 6;
      m_pIndexBuffer[1] = (V < 0x80) ? V : ((V & 0x7f)+0x80);
      m_iNumBytes++;
      if( m_pIndexBuffer[1] & 0x80 ) {
         V >>= 7;
         m_pIndexBuffer[2] = (V < 0x80) ? V : ((V & 0x7f)+0x80);
         m_iNumBytes++;
         if( m_pIndexBuffer[2] & 0x80 ) {
            V >>= 7;
            m_pIndexBuffer[3] = (V < 0x80) ? V : ((V & 0x7f)+0x80);
            m_iNumBytes++;
            if( m_pIndexBuffer[3] & 0x80 ) {
               V >>= 7;
               m_pIndexBuffer[4] = V;
               m_iNumBytes++;
            }
         }
      }
   }
   return;
}

// Creates a Compact Index with the passed values
void CUnrealCompactIndex::Set(unsigned char B0, unsigned char B1, unsigned char B2, unsigned char B3, unsigned char B4) {
   m_pIndexBuffer[0] = B0;
   m_pIndexBuffer[1] = B1;
   m_pIndexBuffer[2] = B2;
   m_pIndexBuffer[3] = B3;
   m_pIndexBuffer[4] = B4;
}
// Returns the value as an integer
int CUnrealCompactIndex::Index(void) const {
   // Result var
   int iResult = 0;
   // Uncompact index
   if( m_pIndexBuffer[0] & 0x40 ) {
      if( m_pIndexBuffer[1] & 0x80 ) {
         if( m_pIndexBuffer[2] & 0x80 ) {
            if( m_pIndexBuffer[3] & 0x80 ) {
               iResult = m_pIndexBuffer[4];
            }
            iResult = (iResult << 7) + (m_pIndexBuffer[3] & 0x7f);
         }
         iResult = (iResult << 7) + (m_pIndexBuffer[2] & 0x7f);
      }
      iResult = (iResult << 7) + (m_pIndexBuffer[1] & 0x7f);
   }
   iResult = (iResult << 6) + (m_pIndexBuffer[0] & 0x3f);
   // Check sign
   if( m_pIndexBuffer[0] & 0x80 ) iResult = -iResult;
   // Return
   return iResult;
}
// How many bytes in the compact index?
int CUnrealCompactIndex::NumBytes(void) const {
   return m_iNumBytes;
}
// Gets the compact index
const unsigned char* CUnrealCompactIndex::CompactIndex(void) const {
   return m_pIndexBuffer;
}