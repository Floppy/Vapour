//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/SimDataPath.h"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: SimDataPath.h,v 1.11 2002/04/04 21:03:37 vap-warren Exp $"

#ifndef __VTSTUCVIS_SIMDATAPATH__
#define __VTSTUCVIS_SIMDATAPATH__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//: Simulation data path property
// Management class for asynchronous loading of simulation data

class CSimDataPath : public CDataPathProperty {
public:

	CSimDataPath();
   //: Constructor

	virtual ~CSimDataPath();
   //: Destructor

   bool ShowFrame(const unsigned int uiFrame,
                  const unsigned int uiSeek,
                  const unsigned int uiLength);
   //: Show a frame
   // Called by the control to indicate that it wishes to display a frame.
   // The control supplies the file offset and length of the chunk required for the frame.
   // If the data is present, it reads the data into a block of memory and passes to the scene manager.
   // If not, the values are stored and the scene manager is called once the data has downloaded.
   //!param: uiFrame - Frame number
   //!param: uiSeek - Offset from start of simulation data file
   //!param: uiLength - Length of data required
   //!return: true if the scene manager is called immediately, false if the call is deferred

   bool Available(const unsigned int uiSeek, const unsigned int uiLength) const;
   //: Queries if frame data is available
   // Checks if the specified length of data at the specified offset has downloaded
   //!param: uiSeek - Offset from start of simulation data file
   //!param: uiLength - Length of data required
   //!return: true if the data has downloaded

   bool Waiting() const;
   //: Queries if a frame has been deferred
   //!return: true if a frame has been deferred

protected:

   unsigned int m_uiDataRead;                                     //: Data downloaded so far
   bool m_bSetup;                                                 //: Scene setup complete indicator
   unsigned int m_uiFrame, m_uiFrameSeek, m_uiFrameLength;        //: Data to be read for next frame

//:-----
//: Windows Mappings

public:
	//{{AFX_VIRTUAL(CSimDataPath)
   public:
	protected:
	virtual void OnDataAvailable(DWORD dwSize, DWORD bscfFlag);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSimDataPath)
	//}}AFX_MSG

};

inline bool CSimDataPath::Waiting() const {
   return (!m_bSetup || (m_uiFrameLength > 0));
}

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTUCVIS_SIMDATAPATH__
