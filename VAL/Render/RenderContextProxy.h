//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContextProxy.h - 05/10/2000 - Warren Moore
//	Render context selection speciality store proxy header
//
// $Id: RenderContextProxy.h,v 1.3 2000/11/25 11:30:37 waz Exp $
//

#pragma once

#ifndef _VAL_RENDERCONTEXTPROXY_
#define _VAL_RENDERCONTEXTPROXY_

#include <stdio.h>

#include "RenderContext.h"
#include "RenderContextStore.h"

//////////////////////////
// External Dependencies

extern CRenderContextStore g_oRenderContextStore;

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

////////////////////////////
// CRenderContextProxyBase

class CRenderContextProxyBase {
public:
	CRenderContextProxyBase();
	virtual CRenderContext* CreateObject(CDisplayContext *poDisplay = NULL) const = 0;

	virtual const char* GetID() const = 0;
	virtual float GetVersion() const = 0;

};

/////////////////////////////////
// CRenderContextProxy Template

template <class T>
class DLL CRenderContextProxy : public CRenderContextProxyBase {
public:
	CRenderContext* CreateObject(CDisplayContext *poDisplay = NULL) const { 
		T *pT = NULL;
		NEWBEGIN
		pT = new T(poDisplay);
		NEWEND("CRenderContextProxy::CreateObject - Render context")
		return pT;
	}
	virtual const char* GetID() const {
		return T::GetID();
	}
	virtual float GetVersion() const {
		return T::GetVersion();
	}
};

/////////////////////
// Inline Functions

inline CRenderContextProxyBase::CRenderContextProxyBase() {
	g_oRenderContextStore.Register(this);
} // Constructor

#endif // _VAL_RENDERCONTEXTPROXY_
