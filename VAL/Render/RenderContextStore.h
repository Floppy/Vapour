//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContextStore.h - 05/10/2000 - Warren Moore
//	Render Context selection speciality store header
//
// $Id: RenderContextStore.h,v 1.0 2000/10/05 20:16:39 waz Exp $
//

#pragma once

#ifndef _VAL_RENDERCONTEXTSTORE_
#define _VAL_RENDERCONTEXTSTORE_

#include <stdio.h>
#include <vector>

#include "RenderContext.h"

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

// Predeclare CRenderContextProxyBase - do not include here
class CRenderContextProxyBase;

// Vector and Iterator Typedef
typedef std::vector<const CRenderContextProxyBase*> RenderContextVector;
typedef std::vector<const CRenderContextProxyBase*>::iterator RenderContextIterator;

// Predicate function for comparing filter names. Returns true if pFilterOne < pFilterTwo.
bool CompareFilters(const CRenderContextProxyBase* pFilterOne ,const CRenderContextProxyBase* pFilterTwo);

////////////////////////
// CRenderContextStore

class DLL_IMP_EXP CRenderContextStore {

private:
	void Init();

	RenderContextVector* m_pRenderContexts;

   static const double m_dMaxFilterVersion;

   CRenderContextStore* m_pParentStore;

public:
	CRenderContextStore() {m_pParentStore=NULL;};
	~CRenderContextStore();

	void Register(const CRenderContextProxyBase* pProxy);

   void Override(CRenderContextStore &oRenderContextStore);

/*
	bool CheckForExtension(const char* pszExtension);
	CRenderContext* CreateByExtension(const char* pszExtension);	
*/

	int GetCount() const;
	const CRenderContextProxyBase* GetAt(int i) const;

};

#endif // _VAL_RENDERCONTEXTSTORE_
