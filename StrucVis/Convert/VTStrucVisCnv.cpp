//==========================================---
// VT Structure Visualisation File Converter
//------------------------------------------
// File converter for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "File Converter"
//! userlevel =  Normal
//! file      = "Convert/VTStrucVisCnv.cpp"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: VTStrucVisCnv.cpp,v 1.9 2002/04/04 10:03:07 vap-james Exp $"

#include <iostream>
#include <vector>

#include "InputData.h"
#include "OChunk.h"

using namespace std;

int main(int argc, char* argv[]) {

   // Get command-line parameters
   if (argc != 3) {
      cerr << "Usage: StructVisCnv <input_filename> <output_filename>" << endl;
      return 1;
   }
   cout << "Input: " << argv[1] << endl;
   cout << "Output: " << argv[2] << endl;

   // Open input data file
   CInputData oInput(argv[1]);

   vector<COChunk*> oChunkList;

   // While there is data to read, create chunks and add them to the list
   while (oInput.NextSection()) {
      COChunk* pChunk = new COChunk;
      cout << oInput.SectionType() << ", Frame " << oInput.LoadIncr() << " ";
      if (pChunk->Read(oInput)) {
         oChunkList.push_back(pChunk);
         if (pChunk->Type() == CHUNK_BEAMS) {
            pChunk = new COChunk;
            if (pChunk->Read(oInput)) {
               oChunkList.push_back(pChunk);
            }
            else delete pChunk;
         }
      }
      else {
         cout << "- ignored";
         delete pChunk;
      }
      cout << endl;
   }

   // Create group chunk
   COChunk* pChunk = new COChunk;
   if (pChunk->CreateGroupChunk(oInput)) oChunkList.push_back(pChunk);
   else delete pChunk;

   // Create stress range chunk
   pChunk = new COChunk;
   if (pChunk->CreateStressChunk(oInput)) oChunkList.push_back(pChunk);
   else delete pChunk;

   cout << "Read " << oInput.GetNumBeams() << " Beams and "<< oInput.GetNumSlabs() << " Slabs." << endl;

   // Create frame chunks
   vector<COChunk*> oFrames;
   bool bOK;
   int iFrame = 0;
   int iNumChunks = 0;
   do {
      bOK = false;
      iFrame++;
      COChunk* pChunk = new COChunk(CHUNK_FRAME);
      for (std::vector<COChunk*>::iterator pIter = oChunkList.begin(); pIter != oChunkList.end(); pIter++) {
         if ((*pIter)->Frame() == iFrame) {
            pChunk->AddSubChunk(*pIter);
            iNumChunks++;
            bOK = true;
         }
      }
      if (bOK) {
         // Fill in empty chunks
         const COChunk* pSubChunk = pChunk->SubChunk(CHUNK_NODEDISP);
         if (pSubChunk == NULL) {
            COChunk* pOldChunk = new COChunk(*(oFrames.back()->SubChunk(CHUNK_NODEDISP)));
            pOldChunk->ZeroDisplacements();
            pChunk->AddSubChunk(pOldChunk);
            iNumChunks++;
         }
         pSubChunk = pChunk->SubChunk(CHUNK_SLABFORC);
         if (pSubChunk == NULL) {
            COChunk* pOldChunk = new COChunk(*(oFrames.back()->SubChunk(CHUNK_SLABFORC)));
            pChunk->AddSubChunk(pOldChunk);
            iNumChunks++;
         }
         pSubChunk = pChunk->SubChunk(CHUNK_BEAMFORC);
         if (pSubChunk == NULL) {
            COChunk* pOldChunk = new COChunk(*(oFrames.back()->SubChunk(CHUNK_BEAMFORC)));
            pChunk->AddSubChunk(pOldChunk);
            iNumChunks++;
         }
         pSubChunk = pChunk->SubChunk(CHUNK_CRACKS);
         if (pSubChunk == NULL) {
            COChunk* pOldChunk = new COChunk(*(oFrames.back()->SubChunk(CHUNK_CRACKS)));
            pChunk->AddSubChunk(pOldChunk);
            iNumChunks++;
         }
         // Add frame chunk to frame list
         oFrames.push_back(pChunk);
      }
      else delete pChunk;
   } while (bOK);

   // Create frame info chunk
   pChunk = new COChunk;
   if (pChunk->CreateFrameInfoChunk(oInput)) oChunkList.push_back(pChunk);
   else delete pChunk;

   cout << iNumChunks << " frame chunks created." << endl;

   // Create setup chunk
   iNumChunks = 0;
   COChunk oSetup(CHUNK_SETUP);
   for (std::vector<COChunk*>::iterator pIter = oChunkList.begin(); pIter != oChunkList.end(); pIter++) {
      if ((*pIter)->Type() < CHUNK_FRAME) {
         oSetup.AddSubChunk(*pIter);
         iNumChunks++;
      }
   }

   cout << iNumChunks << " setup chunks created." << endl;

   // Convert node displacement chunks from incremental to absolute displacements.
   // Create displacement data
   int iSize = oSetup.SubChunk(CHUNK_NODES)->NumNodes() * 3;
   float* pfDisplacements = new float[iSize];
   memset(pfDisplacements,0,iSize * sizeof(float));
   // Propogate displacements across frames
   for (pIter = oFrames.begin(); pIter != oFrames.end(); pIter++) {
      const COChunk* pNodeDisp = (*pIter)->SubChunk(CHUNK_NODEDISP);
      if (pNodeDisp != NULL) {
         const_cast<COChunk*>(pNodeDisp)->AddDisplacements(pfDisplacements);
      }
   }
   delete [] pfDisplacements;

   // Create root chunk
   COChunk oRoot(CHUNK_ROOT);
   // Add subchunks to root   
   oRoot.AddSubChunk(&oSetup);
   for (pIter = oFrames.begin(); pIter != oFrames.end(); pIter++)
      oRoot.AddSubChunk(*pIter);

   // Output root chunk to file
   ofstream oOutput(argv[2],ios::binary);
   oOutput << "VSV" << static_cast<char>(1);
   oRoot.Write(oOutput);
   oOutput.close();

   // Dump all chunks
   for (pIter = oChunkList.begin(); pIter != oChunkList.end(); pIter++) {
      delete *pIter;
   }
   for (pIter = oFrames.begin(); pIter != oFrames.end(); pIter++) {
      delete *pIter;
   }

   // Done
   return 0;
}