//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/image/filter.cpp"
//! author		= "Warren Moore"
//! date			= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid		= "$Id: filter.cpp,v 1.1 2001/10/17 22:05:33 vap-warren Exp $"

//#===--- Includes
#include "filter.h"

#include <math.h>

namespace NValet {

   //#===--- CBaseFilter

   void CBaseFilter::Width(double dWidth) {
	   m_dWidth = dWidth;
   } // CBaseFilter::Width (Set)

   double CBaseFilter::Width() const {
	   return m_dWidth;
   } // CBaseFilter::Width (Get)

   //#===--- CBoxFilter

   double CBoxFilter::Filter(double dVal) {
      return (fabs(dVal) <= m_dWidth ? 1.0 : 0.0);
   } // CBoxFilter::Filter
	
   //#===--- CBilinearFilter

   double CBilinearFilter::Filter(double dVal) {
	   dVal = fabs(dVal);
	   return (dVal < m_dWidth ? m_dWidth - dVal : 0.0);
   } // CBilinearFilter::Filter

}
