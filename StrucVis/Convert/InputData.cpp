//==========================================---
// VT Structure Visualisation File Converter
//------------------------------------------
// File converter for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.File Converter.Arup File IO"
//! userlevel =  Normal
//! file      = "Convert/InputData.cpp"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: InputData.cpp,v 1.4 2002/04/04 11:18:18 vap-warren Exp $"

#include "InputData.h"
#include <fstream>

CInputData::CInputData(const char* pcFilename) :
   m_oInput(pcFilename),
   m_iLoadIncr(0),
   m_iNumBeams(0),
   m_iNumBeamGroups(0),
   m_iNumSlabs(0),
   m_iNumSlabGroups(0),
   m_iNumFrames(0),
   m_fMinStress(0),
   m_fMaxStress(0)
{
}

CInputData::~CInputData() {
   m_oInput.close();
}

using namespace std;

bool CInputData::NextSection(void) {
   // Clear old header data
   memset(m_pcHeader,0,128);
   // Find opening tag
   char ch = 0;
   do {
      m_oInput.get(ch);
   } while (m_oInput.good() && ch != '<');
   if (!m_oInput.good()) {
      return false;
   }
   // Get complete line
   m_oInput.getline(m_pcHeader,128);
   // Chop header line to extract section type
   char* pcEnd = strchr(m_pcHeader,'>');
   if (pcEnd == NULL) return false;
   *pcEnd = 0;
   // Is there any data after the header?
   char* pcExtraData = pcEnd +1;
   if (strlen(pcExtraData) > 0) {
      // Extract the load increment and keep it
      while (*pcExtraData != '#' && *pcExtraData != 0) 
         pcExtraData++;
      m_iLoadIncr = atoi(++pcExtraData);
      if (m_iLoadIncr > m_iNumFrames) m_iNumFrames = m_iLoadIncr;
   }
   m_bBeginning = true;
   return true;
}

const char* CInputData::SectionType(void) {
   return m_pcHeader;
}

unsigned int CInputData::LoadIncr(void) {
   return m_iLoadIncr;
}

float CInputData::GetFloat(void) {
   m_bBeginning = false;
   float fInput;
   m_oInput.clear();
   m_oInput >> fInput;
   return fInput;
}

void CInputData::UngetFloat(float fVal) {
   m_oInput.putback(' ');
   char pcBuffer[32];
   sprintf(pcBuffer,"%.5g",fVal);
   for (int i=strlen(pcBuffer); i-->0; ) {
      m_oInput.putback(pcBuffer[i]);
   }
   m_oInput.putback(' ');
}

unsigned int CInputData::GetUInt(void) {
   m_bBeginning = false;
   unsigned int iInput;
   m_oInput.clear();
   m_oInput >> iInput;
   return iInput;
}

void CInputData::UngetUInt(unsigned int iVal) {
   m_oInput.putback(' ');
   char pcBuffer[32];
   sprintf(pcBuffer,"%d",iVal);
   for (int i=strlen(pcBuffer); i-->0; ) {
      m_oInput.putback(pcBuffer[i]);
   }
   m_oInput.putback(' ');
}

bool CInputData::DataReady(void) {
   // Get next non-whitespace character from stream
   m_oInput >> ws;
   char ch = 0;
   m_oInput.get(ch);
   // Check failure states
   if (!m_oInput.good() || ch == '{') return false;
   // Put the character back on the stream
   m_oInput.putback(ch);
   return true;
}

void CInputData::AddToStressRange(float fStress) {
   if (fStress < m_fMinStress) 
      m_fMinStress = fStress;
   else if (fStress > m_fMaxStress) 
      m_fMaxStress = fStress;
}