//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// DataManager.h
// 19/03/2002 - James Smith
//
// $Id: DataManager.h,v 1.9 2002/03/27 15:11:18 vap-james Exp $

#ifndef __VTSTRUCVIS_DATAMANAGER__
#define __VTSTRUCVIS_DATAMANAGER__

#include "Element.h"
#include "Chunk.h"
#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManager {
//#===--- Construction/Destruction
public:

   CDataManager();
   // Constructor
   
   ~CDataManager();
   // Destructor

//#===--- Data I/O

   bool Setup(const unsigned char* pcData, unsigned int iLength);
   // Accepts setup data
   // Returns true if setup is complete.
   // NO other data will be available until Setup returns true.

   bool FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength);
   // Gets frame location information
   // Frame offset and length for iFrame are returned in the parameters.
   // Returns false if an invalid frame is requested

   bool LoadFrame(const unsigned char* pcData, unsigned int iLength);
   // Loads frame data from the passed memory chunk
   // Returns true if load was successful

//#===--- Data Access

   unsigned int NumFrames(void);
   // Returns the number of frames in the animation

   unsigned int NumNodes(void);
   // Returns the number of nodes in the simulation

   const float* NodePositions(void);
   // Returns a pointer to an array of the default node positions
   // This is a flat array of 3 floats per node, in order X Y Z.
   
   const float* NodeDisplacements(void);
   // Returns a pointer to an array of node displacements for the current frame
   // This is a flat array of 3 floats per node, in order X Y Z.

   unsigned int NumGroups(void);
   // Returns the number of groups (temperature profiles).

   float GroupTemp(unsigned int iGroup);
   // Returns the temperature of the group at the current frame

   TElementType GroupType(unsigned int iGroup);
   // Returns the type of element that the group contains.

   const float* GroupColour(unsigned int iGroup);
   // Returns the colour assigned to the group.

   void StressRange(float& fMin, float& fMax);
   // Return the minimum and maximum stress values.

   unsigned int NumBeams(void);
   // Returns the number of beam elements in the dataset.

   const unsigned int* BeamNodes(int iBeam);
   // Returns the controlling nodes for a particular beam

   unsigned int BeamGroup(int iBeam);
   // Returns the group ID for a particular beam

   void BeamSizes(int iGroup, float& fHeight, float& fWidth, float& fFlange, float& fWeb);
   // Returns the beam sizes for a partcular beam group

   const float* BeamStresses(unsigned int iBeam);
   // Returns a pointer to an array of node stresses for the current frame and specified beam
   // This is a flat array of 3 floats per node, in order X Y Z.

   unsigned int NumSlabs(void);
   // Returns the number of slab elements in the dataset.

   const unsigned int* SlabNodes(int iSlab);
   // Returns the controlling nodes for a particular slab

   unsigned int SlabGroup(int iSlab);
   // Returns the group ID for a particular slab

   void SlabSizes(int iGroup, float& fThickness);
   // Returns the slab size for a partcular slab group

   const float* SlabStresses(unsigned int iSlab);
   // Returns a pointer to an array of node stresses for the current frame and specified slab
   // This is a flat array of 3 floats per node, in order X Y Z.   

   const unsigned char* SlabCracks(unsigned int iSlab, unsigned int iLayer);
   // Returns a pointer to an array of node cracks for the specified slab, layer and current frame
   // This is a flat array of 1 unsigned char per node


//#===--- Member Variables
protected:

   class CGroup {
   public:
      TElementType m_oType;
      float m_fTemperature;
      float m_pfColour[3];
   };
   // Group information type
   
   std::vector<CGroup> m_oGroups;
   // Group information

   CRootChunk m_oRoot;
   // The root chunk of the file. 
   // Contains the complete TOC.

   unsigned int m_iNumBeams;
   unsigned int m_iNumSlabs;
   // beam & slab counters

   CChunk* m_pChunk;
   // Temporary chunk storage

   enum TIOState {
      INIT,
      VALID,
      LOADING,
      READY
   } m_oState;

};

#endif // __VTSTRUCVIS_DATAMANAGER__