//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ImageFileStore.h - 21/12/1999 - Warren Moore
//	Header file for image file format self-registering mechanism
//
// $Id: ImageFileStore.h,v 1.3 2000/11/21 16:38:47 waz Exp $
//

#ifndef _VAL_IMAGEFILESTORE_
#define _VAL_IMAGEFILESTORE_

#pragma once

//#===--- Includes
#include <stdio.h>
#include <vector>

#include "ImageFile.h"

// DLL import/export definitions
#ifndef DLL
	#ifdef VAL_DLL_EXPORT
		#define DLL __declspec(dllexport)
	#endif
	#ifdef VAL_DLL_IMPORT			
		#define DLL __declspec(dllimport)
	#endif
	#ifndef DLL
		#define DLL
	#endif
#endif

// Predeclare CImageFileProxyBase - do not include here
class CImageFileProxyBase;

// Vector Typedef
typedef std::vector<const CImageFileProxyBase*> ImageFileVector;

////////////////////
// CImageFileStore

class DLL CImageFileStore {
private:
	void Init();

	ImageFileVector *m_pImageFiles;

   CImageFileStore* m_pParentStore;

public:
	CImageFileStore() {m_pParentStore=NULL;};
	~CImageFileStore();

	void Register(const CImageFileProxyBase *pProxy);

   void Override(CImageFileStore &oImageFileStore);

	bool CheckForExtension(const char *pszExtension);
	CImageFile *CreateByExtension(const char *pszExtension);

	int GetCount() const;
	const CImageFileProxyBase *GetAt(int i) const;

};

#endif // _VAL_IMAGEFILESTORE_
