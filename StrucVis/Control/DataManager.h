//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// DataManager.h
// 19/03/2002 - James Smith
//
// $Id: DataManager.h,v 1.1 2002/03/22 14:56:26 vap-james Exp $

#ifndef __DATAMANAGER__
#define __DATAMANAGER__

#include "Element.h"

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

//#===--- Member Functions

   bool Setup(const unsigned char* pcData, unsigned int iLength);
   // Accepts setup data
   // Returns true if setup is complete.
   // NO other data will be available until Setup returns true.

   bool LoadFrame(unsigned int iFrame);
   // Loads data for the specified frame
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

   unsigned char* m_pcBuffer;
   // Data buffer

   unsigned int m_iNumFrames;
   // The number of frames in the sequence

   unsigned int m_iCurrentFrame;
   // The currently loaded frame

};

#endif // __DATAMANAGER__