//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContextProxy.h - 05/10/2000 - Warren Moore
//	Render context selection speciality store proxy header
//
// $Id: RenderContextProxy.h,v 1.1 2000/10/06 13:04:43 waz Exp $
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
#ifndef DLL_IMP_EXP
	#ifdef _EXP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllexport)
	#endif
	#ifdef _IMP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllimport)
	#endif
	#ifndef DLL_IMP_EXP
		#define DLL_IMP_EXP
	#endif
#endif

////////////////////////////
// CRenderContextProxyBase

class CRenderContextProxyBase {
public:
	CRenderContextProxyBase();
	virtual CRenderContext* CreateObject() const = 0;

	virtual const char* GetID() const = 0;
	virtual float GetVersion() const = 0;

};

/////////////////////////////////
// CRenderContextProxy Template

template <class T>
class DLL_IMP_EXP CRenderContextProxy : public CRenderContextProxyBase {
public:
	CRenderContext* CreateObject() const { 
		T *pT = NULL;
		NEWBEGIN
		pT = new T;
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
