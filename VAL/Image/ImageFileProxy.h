//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ImageFileProxy.h - 21/12/1999 - Warren Moore
//	Header file for image file format self-registering mechanism
//
// $Id: ImageFileProxy.h,v 1.2 2000/06/17 10:42:19 waz Exp $
//

#ifndef _VAL_IMAGEFILEPROXY_
#define _VAL_IMAGEFILEPROXY_

#pragma once

//#===--- Includes
#include "ImageFile.h"
#include "ImageFileStore.h"
#include "Image.h"

#include <stdio.h>

//#===--- External Dependencies
extern CImageFileStore g_oImageFileStore;

////////////////////////
// CImageFileProxyBase

class CImageFileProxyBase {
public:
	CImageFileProxyBase();
	virtual CImageFile *CreateObject() const = 0;

	virtual const char *GetTitle() const = 0;
	virtual const char *GetExtension() const = 0;
};

////////////////////////////
// CImageFileProxy Template

template <class T>
class CImageFileProxy : public CImageFileProxyBase {
public:
	CImageFile *CreateObject() const
		{ return new T; }

	virtual const char *GetTitle() const {
		return T::GetTitle();
	}

	virtual const char *GetExtension() const {
		return T::GetExtension();
	}
};

//#===--- Inline Functions

inline CImageFileProxyBase::CImageFileProxyBase() {
	g_oImageFileStore.Register(this);
} // Constructor

#endif // _VAL_IMAGEFILEPROXY_