//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// ViewAvatar.h - 23/11/2000 - Warren Moore
//	  InterVAL associated Avatar document view
//
// $Id: ViewAvatar.h,v 1.1 2000/11/25 22:52:26 cvs Exp $
//

#ifndef _INTERVAL_VIEWAVATAR_
#define _INTERVAL_VIEWAVATAR_

#pragma once

#include "DisplayContext.h"
#include "SceneAvatarPose.h"

class CViewAvatar : public CView, public CDisplayContext {

//#===--- External Functions
public:
	virtual ~CViewAvatar();

	CDocAvatar* GetDocument();

	// CDisplayContext overridden function for obtaining the device context
	virtual DCRESULT GetDisplayPointer(const int iValue, void *&pData) const;

//#===--- Internal Functions

protected: // create from serialization only
	CViewAvatar();
	DECLARE_DYNCREATE(CViewAvatar)

//#===--- Internal Data
protected:
	CClientDC *m_poDC;									// Device context
	CSceneAvatarPose *m_poScene;						// Avatar posing scene

//#===--- Diagnostics
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//#===--- MFC Mapping

	//{{AFX_VIRTUAL(CViewAvatar)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	protected:
	//{{AFX_MSG(CViewAvatar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//#===--- Debug source
#ifndef _DEBUG  
inline CDocAvatar* CViewAvatar::GetDocument()
   { return (CDocAvatar*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}

#endif // _INTERVAL_VIEWAVATAR_
