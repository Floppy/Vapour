//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.VRML.VSV File Loading"
//! userlevel =  Normal
//! file      = "Control/DataManager.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: DataManager.h,v 1.17 2002/04/25 15:44:04 vap-warren Exp $"

#ifndef __VTSTRUCVIS_DATAMANAGER__
#define __VTSTRUCVIS_DATAMANAGER__

#include "Element.h"
#include "IChunk.h"
#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//: The Data Manager
// This class controls loading of data segments from streams, and conversion into an appropriate format.
// To load data, give data to Setup() until it returns true. Then request frame information from FrameInfo(). This gives
// a file offset and a length. The data in this section of the file should be passed to LoadFrame() to display it.

class CDataManager {

public:

   //:-------------------------
   //: Construction/Destruction

   CDataManager();
   //: Constructor
   
   ~CDataManager();
   //: Destructor

   //:---------
   //: Data I/O

   bool Setup(const unsigned char* pcData, unsigned int iLength);
   //: Accepts setup data
   // NO other data will be available until Setup returns true.
   //!param: pcData - raw input data
   //!param: iLength - length of pcData
   //!return: true if setup is complete, false if more data is required.

   bool FrameInfo(unsigned int iFrame, unsigned int& iOffset, unsigned int& iLength);
   //: Frame location information
   //!param: iFrame - the requested frame - between 0 and NumFrames-1.
   //!param: iOffset - the offset of the frame data is placed here.
   //!param: iLength - the length of the frame data is placed here.
   //!return: false if an invalid frame is requested, true otherwise.

   bool LoadFrame(const unsigned char* pcData, unsigned int iLength);
   //: Loads frame data from the passed memory chunk
   //!param: pcData - raw input data
   //!param: iLength - length of pcData
   // Returns true if load was successful, false otherwise

   //:------------
   //: Data Access

   unsigned int NumFrames(void);
   //: The number of frames in the sequence

   unsigned int NumNodes(void);
   //: The number of nodes in the sequence

   const float* NodePositions(void);
   //: Default node positions
   // This is only usable after Setup() is complete, and before LoadFrame() is used.
   //!return: A flat array of 3 floats per node, in order X Y Z.
   
   const float* NodeDisplacements(void);
   //: Current node displacements
   // This is only usable after LoadFrame() is used successfully.
   //!return: A flat array of 3 floats per node, in order X Y Z.

   unsigned int NumGroups(void);
   //: The number of groups (temperature profiles).

   float GroupTemp(unsigned int iGroup);
   //: The current temperature of the specified group
   //!param: iGroup - a value between 1 and NumGroups

   TElementType GroupType(unsigned int iGroup);
   //: The type of elements that the group contains
   //!param: iGroup - a value between 1 and NumGroups

   const float* GroupColour(unsigned int iGroup);
   //: The colour assigned to the group
   //!param: iGroup - a value between 1 and NumGroups

   void StressRange(float& fMin, float& fMax);
   //: The range of stresses in the dataset.
   //!param: fMin - the minimum stress value
   //!param: fMax - the maximum stress value

   unsigned int NumBeams(void);
   //: How many beam elements in the dataset?

   const unsigned int* BeamNodes(int iBeam);
   //: Returns the controlling nodes for a particular beam
   //!param: iBeam - a value between 1 and NumBeams
   //!return: an array of 2 unsigned integers.

   unsigned int BeamGroup(int iBeam);
   // Returns the group ID for a particular beam
   //!param: iBeam - a value between 1 and NumBeams
   //!return: the group number of the beam

   void BeamSizes(int iGroup, float& fHeight, float& fWidth, float& fFlange, float& fWeb);
   // Returns the beam sizes for a partcular beam group
   //!param: iGroup - a value between 1 and NumGroups
   //!param: fHeight - the height of the beam
   //!param: fWidth - the width of the beam
   //!param: fFlange - the thickness of the flange
   //!param: fWeb - the thickness of the web

   const float* BeamStresses(unsigned int iBeam);
   // Returns a pointer to an array of node stresses for the current frame and specified beam
   //!param: iBeam - a value between 1 and NumBeams
   //!return: A flat array of 3 floats per node, in order X Y Z.

   unsigned int NumSlabs(void);
   //: How many slab elements in the dataset?

   const unsigned int* SlabNodes(int iSlab);
   //: Returns the controlling nodes for a particular slab
   //!param: iSlab - a value between 1 and NumSlabs
   //!return: an array of 9 unsigned integers.

   unsigned int SlabGroup(int iSlab);
   // Returns the group ID for a particular slab
   //!param: iSlab - a value between 1 and NumSlabs
   //!return: the group number of the slab

   void SlabSizes(int iGroup, float& fThickness);
   // Returns the slab size for a partcular slab group
   //!param: iGroup - a value between 1 and NumGroups
   //!param: fThickness - the thickness of the slab

   const float* SlabStresses(unsigned int iSlab);
   // Returns a pointer to an array of node stresses for the current frame and specified slab
   //!param: iSlab - a value between 1 and NumSlabs
   //!return: A flat array of 3 floats per node, in order X Y Z.

   const unsigned char* SlabCracks(unsigned int iSlab, unsigned int iLayer);
   // Returns a pointer to an array of node cracks for the specified slab, layer and current frame
   //!param: iSlab - a value between 1 and NumSlabs
   //!return: A flat array of 1 unsigned char per node


//#===--- Member Variables
protected:

   //: Group information type
   class CGroup {
   public:
      TElementType m_oType;
      float m_fTemperature;
      float m_pfColour[3];
   };
   
   std::vector<CGroup> m_oGroups;
   //: Group information

   CRootChunk m_oRoot;
   //: The root chunk of the file. 
   // Contains the complete TOC.

   unsigned int m_iNumBeams;
   //: The number of beams in the data
      
   unsigned int m_iNumSlabs;
   //: The number of slabs in the data

   CIChunk* m_pChunk;
   //: Temporary chunk storage

   enum TIOState {
      INIT,
      VALID,
      LOADING,
      READY
   } m_oState;
   //: Stream state

};

#endif // __VTSTRUCVIS_DATAMANAGER__