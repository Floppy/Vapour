//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContextStore.cpp - 05/10/2000 - Warren Moore
//	Render context selection speciality store implementation
//
// $Id: RenderContextStore.cpp,v 1.1 2000/10/06 13:04:43 waz Exp $
//

#include "stdafx.h"

#include "RenderContextStore.h"
#include "RenderContextProxy.h"
#include "RenderContext.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////
// CRenderContextStore

const double CRenderContextStore::m_dMaxVersion = 2.0F;

/////////////////////
// Member Functions

CRenderContextStore::~CRenderContextStore() {
	if (m_pRenderContexts!=NULL) delete m_pRenderContexts;
} // Destructor

void CRenderContextStore::Init() {
	if (!m_pRenderContexts)
		m_pRenderContexts = new RenderContextVector;
} // Init

void CRenderContextStore::Register(const CRenderContextProxyBase* pProxy) {
   if (m_pParentStore) m_pParentStore->Register(pProxy);
   else {
      Init();
      m_pRenderContexts->push_back(pProxy);
   }
   return;
} // Register

void CRenderContextStore::Override(CRenderContextStore &oRenderContextStore) {
   int iNumFilters = GetCount();
   for (int i=0; i<iNumFilters; i++) {
      oRenderContextStore.Register(GetAt(i));
   }
   delete m_pRenderContexts;
   m_pRenderContexts = NULL;
   m_pParentStore = &oRenderContextStore;
   return;
} // Override

int CRenderContextStore::GetCount() const {
   if (m_pParentStore)
		return m_pParentStore->GetCount();
   else
		return (m_pRenderContexts) ? m_pRenderContexts->size() : 0;
} // GetCount

const CRenderContextProxyBase* CRenderContextStore::GetAt(int i) const {
   if (m_pParentStore)
		return m_pParentStore->GetAt(i);
	else
		return m_pRenderContexts->at(i);
} // GetAt

//#===--- Context selection
void CRenderContextStore::ClearContextOptions() {
	m_oOptionList.clear();
} // ClearContextOptions

void CRenderContextStore::SetContextOption(int iOption, unsigned int uValue) {
	// Store the tuple
	SRCOptionTuple sTuple;
	sTuple.m_iOption = iOption;
	sTuple.m_uValue = uValue;
	// Push it on the list
	m_oOptionList.push_back(sTuple);
} // SetContextOption

CRenderContext *CRenderContextStore::CreateSuitableContext() {
	// Give up if empty option list
	if (m_oOptionList.size() == 0)
		return NULL;
	int iCount = 0;
	CRenderContext *pContext = NULL;
	// Loop through all registered contexts until one found or all tried
	while ((pContext == NULL) && (iCount < GetCount())) {
		// Create the context under creation
		pContext = GetAt(iCount)->CreateObject();
		// Pass in the selection options
		RCRESULT eResult = RC_OK;
		eResult = pContext->CheckSelection(m_oOptionList);
		// If succeeded, try to create the context
		if (eResult == RC_OK)
			eResult = pContext->Create();
		// If not ok, delete the context
		if (eResult != RC_OK) {
			delete pContext;
			pContext = NULL;
		}
		// Step to the next context
		iCount++;
	}
	return pContext;
} // CreateSuitableContext

