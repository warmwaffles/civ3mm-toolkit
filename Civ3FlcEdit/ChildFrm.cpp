/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FlcChildFrm.cpp
	CREATED:	?.?.2004
	VERSION:	5.0

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CChildFrm class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ChildFrm.h"

#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrm

IMPLEMENT_DYNCREATE(CChildFrm, CNewMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrm, CNewMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrm)
	ON_WM_MDIACTIVATE()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrm construction/destruction

CChildFrm::CChildFrm()
{
}

CChildFrm::~CChildFrm()
{
}

BOOL CChildFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CNewMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style = WS_CHILD | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | FWS_ADDTOTITLE;

	cs.style &= ~WS_SIZEBOX; // to remove child frame resizing

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrm diagnostics

#ifdef _DEBUG
void CChildFrm::AssertValid() const
{
	CNewMDIChildWnd::AssertValid();
}

void CChildFrm::Dump(CDumpContext& dc) const
{
	CNewMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrm message handlers

void CChildFrm::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!
	
	CView* pView = GetActiveView();
	if (pView)
		SetWindowText(uGetFileName(pView->GetDocument()->GetPathName()));
}

void CChildFrm::ActivateFrame(int nCmdShow) 
{
	CSize sz(WND_CX, WND_CY);
	
	CAnimationView *pView = (CAnimationView*)GetActiveView();
	if (pView)
		sz = pView->GetFrameSize();
	
	sz.cx += GetSystemMetrics(SM_CXSIZEFRAME) * 2;
	sz.cy += GetSystemMetrics(SM_CYSMCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME) * 2;
	
	SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	
	CNewMDIChildWnd::ActivateFrame(nCmdShow);
}

void CChildFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd*)
{
	CNewMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, NULL);
	
	CAnimationView* pView = (CAnimationView*)GetActiveView();
	if (pView)
	{
		pView->ActivateDlgBar(bActivate);
	}
}
