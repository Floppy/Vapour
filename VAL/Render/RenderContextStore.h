//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContextStore.h - 05/10/2000 - Warren Moore
//	Render Context selection speciality store header
//
// $Id: RenderContextStore.h,v 1.3 2000/11/25 11:30:37 waz Exp $
//

#pragma once

#ifndef _VAL_RENDERCONTEXTSTORE_
#define _VAL_RENDERCONTEXTSTORE_

#include <stdio.h>
#include <vector>
#include "DisplayContext.h"

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

// Predeclare CRenderContextProxyBase - do not include here
class CRenderContextProxyBase;

// Predeclare CRenderContext as only for return type
class CRenderContext;

// Vector and Iterator Typedef
typedef std::vector<const CRenderContextProxyBase*> RenderContextVector;
typedef std::vector<const CRenderContextProxyBase*>::iterator RenderContextIterator;

// Context selection option structure and vector
struct SRCOptionTuple {
	int m_iOption;
	unsigned int m_uValue;
};
typedef std::vector<SRCOptionTuple> RCOptionListVector;

////////////////////////
// CRenderContextStore

class DLL CRenderContextStore {
public:
//#===--- External Functions
	CRenderContextStore() : m_poDC(NULL), m_pParentStore(NULL) {};
	~CRenderContextStore();

	// Proxy register function
	void Register(const CRenderContextProxyBase* pProxy);

	// Register store to master store
   void Override(CRenderContextStore &oRenderContextStore);

	// Registered proxy direct access
	int GetCount() const;
	const CRenderContextProxyBase* GetAt(int i) const;

	// Context selection
	void SetDisplayContext(const CDisplayContext *poDC);
	void ClearContextOptions();
	void SetContextOption(int iOption, unsigned int uValue);
	CRenderContext *CreateSuitableContext();

private:
//#===--- Internal Functions
	void Init();

//#===--- Internal Data
	CDisplayContext *m_poDC;								// Display context
	RenderContextVector* m_pRenderContexts;			// List of registered contexts
   static const double m_dMaxVersion;					// Maximum usable context version
   CRenderContextStore* m_pParentStore;				// Pointer to master store
	RCOptionListVector m_oOptionList;					// Context selection option list

};

#endif // _VAL_RENDERCONTEXTSTORE_
