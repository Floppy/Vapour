//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_IMAGE_FILTER_
#define _VALET_IMAGE_FILTER_

////////////////
//! file 		= "VALET/image/filter.h"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid 		= "$Id: filter.h,v 1.3 2001/10/18 11:22:30 vap-warren Exp $"
//! userlevel 	= Develop
//! docentry 	= "VALET.Image.Scaling"

namespace NValet {

   //#===--- CBaseFilter
   //: Base Image Filter Class
   // Base class for image scaling filters
   //!classtodo: Seperate out into inline files when profiling

   class CBaseFilter {
   public:

   	CBaseFilter(double dWidth = 0) : m_dWidth(dWidth) {};
      //: Constructor
      // Creates the base filter with a specified width
      //!param: dWidth = Filter width

   	virtual ~CBaseFilter() {};
      //: Destructor

      void Width(double dWidth);
      // Set filter width

      double Width() const;
      // Get filter width

      virtual double Filter(double dVal) = 0;
      // Returns the filter weight at the specified pos
      // Must be implemented in derived class

   protected:

      double m_dWidth;     // Filter width

   };

   //#===--- CBoxFilter
   //: Box Image Filter
   // Box image filter for pixel resize 'blocky' image scaling
   //!classtodo: Seperate out into inline files when profiling

   class CBoxFilter : public CBaseFilter {
   public:

      CBoxFilter(double dWidth = 0.5) : CBaseFilter(dWidth) {};
      //: Constructor

      ~CBoxFilter() {};
      //: Destructor

      double Filter(double dVal);
      // Returns the filter weight at the specified pos

   };

   //#===--- CBilinearFilter
   //: Bilinear Image Filter
   // Bilinear image filter for smooth image scaling
   //!classtodo: Seperate out into inline files when profiling

   class CBilinearFilter : public CBaseFilter {
   public:

      CBilinearFilter(double dWidth = 1.0) : CBaseFilter(dWidth) {};
      //: Constructor
      ~CBilinearFilter() {};
      //: Destructor

      double Filter(double dVal);
      // Returns the filter weight at the specified pos

   };

}

#endif // _VALET_IMAGE_FILTER_
