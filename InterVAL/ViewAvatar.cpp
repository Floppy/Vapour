//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// ViewAvatar.cpp - 23/11/2000 - Warren Moore
//	  InterVAL associated Avatar document view
//
// $Id: ViewAvatar.cpp,v 1.2 2000/11/26 17:08:31 waz Exp $
//

#include "StdAfx.h"
#include "InterVAL.h"

#include "DocAvatar.h"
#include "ViewAvatar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////
// CViewAvatar

CViewAvatar::CViewAvatar() :
	m_poDC(NULL),
	m_poScene(NULL) {
} // Constructor

CViewAvatar::~CViewAvatar() {
} // Destructor

BOOL CViewAvatar::PreCreateWindow(CREATESTRUCT& cs) {
	// Set the window properties
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
} // PreCreateWindow

int CViewAvatar::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the device context
	ASSERT(m_poDC == NULL);
	NEWBEGIN
	m_poDC = (CClientDC*) new CClientDC(this);
	NEWEND("CViewAvatar::OnCreate - Client device context\n");
	
	// Create the scene
	NEWBEGIN
	m_poScene = (CSceneAvatarPose*) new CSceneAvatarPose(this);
	NEWEND("CViewAvatar::OnCreate - Scene object\n");

	// Start the scene
	if (m_poScene)
		m_poScene->Create();
	else
		return -1;

	return 0;
} // OnCreate

void CViewAvatar::OnDestroy() {
	CView::OnDestroy();

	// Close the scene
	if (m_poScene) {
		delete m_poScene;
		m_poScene = NULL;
	}

	// Destroy the device context
	if (m_poDC) {
		delete m_poDC;
		m_poDC = NULL;
	}
} // OnDestroy

DCRESULT CViewAvatar::GetDisplayPointer(const int iValue, void *&pData) const {
	// Temp variables
	pData = NULL;
	if (iValue == DC_WIN_DC)
		pData = m_poDC->m_hDC;
	return (pData ? DC_OK : DC_UNSUPPORTED_VALUE);
} // GetDisplayPointer

void CViewAvatar::OnDraw(CDC* pDC) {
	CDocAvatar* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_poScene->Render();
} // OnDraw

void CViewAvatar::OnInitialUpdate() {
	CView::OnInitialUpdate();
	
	// Set the window size
	CRect oRect;
	GetClientRect(oRect);
	m_poScene->SetSize(oRect.right, oRect.bottom);

	// Get the document
	CDocAvatar* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Get the avatar
	bool bGeneric = true;
	CAvatar *poAvatar = pDoc->GetAvatar(bGeneric);
	m_poScene->ImportAvatar(poAvatar, bGeneric);

	// Make sure the display gets updated
	PostMessage(WM_PAINT);
} // OnInitialUpdate

void CViewAvatar::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) {
} // OnUpdate

BOOL CViewAvatar::OnEraseBkgnd(CDC* pDC) {
	// Only erase the background if the window isn't drawing
	return (m_poScene->Active() ? TRUE : CView::OnEraseBkgnd(pDC));
} // OnEraseBackground

void CViewAvatar::OnSize(UINT nType, int cx, int cy) {
	CView::OnSize(nType, cx, cy);

	// Set the new window size
	m_poScene->SetSize(cx, cy);
} // OnSize

void CViewAvatar::OnMouseMove(UINT nFlags, CPoint point) {
	// Get the mouse position
	m_poScene->MoveTo(point.x, point.y);
	// If necessary, redraw the contents
	// TODO: Make this respond to a data changed indicator
	m_poScene->Render();

	CView::OnMouseMove(nFlags, point);
} // OnMouseMove

void CViewAvatar::OnLButtonDown(UINT nFlags, CPoint point) {
	// Capture mouse calls while the button is down, and notify the scene
	SetCapture();
	m_poScene->LeftButton(true);

	CView::OnLButtonDown(nFlags, point);
} // OnLButtonDown

void CViewAvatar::OnLButtonUp(UINT nFlags, CPoint point) {
	// Release any mouse capture, and notify the scene that the button is up
	ReleaseCapture();
	m_poScene->LeftButton(false);

	CView::OnLButtonUp(nFlags, point);
} // OnLButtonUp

void CViewAvatar::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) {
	// Tell the scene that it's stored position is stale
	if (bActivate)
		m_poScene->MoveReset();
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
} // OnActivateView

//#===--- Printing

BOOL CViewAvatar::OnPreparePrinting(CPrintInfo* pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
} // OnPreparePrinting

void CViewAvatar::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// Add extra initialization before printing
} // OnBeginPrinting

void CViewAvatar::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// Add cleanup after printing
} // OnEndPrinting

//#===--- MFC Mapping

IMPLEMENT_DYNCREATE(CViewAvatar, CView)

BEGIN_MESSAGE_MAP(CViewAvatar, CView)
	//{{AFX_MSG_MAP(CViewAvatar)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

//#===--- Diagnostics

#ifdef _DEBUG
void CViewAvatar::AssertValid() const {
	CView::AssertValid();
} // AssertValid

void CViewAvatar::Dump(CDumpContext& dc) const {
	CView::Dump(dc);
} // Dump

CDocAvatar* CViewAvatar::GetDocument() {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocAvatar)));
	return (CDocAvatar*)m_pDocument;
} // GetDocument
#endif //_DEBUG

