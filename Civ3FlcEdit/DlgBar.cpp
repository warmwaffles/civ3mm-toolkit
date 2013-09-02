/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	27.08.2004
	VERSION:	2.0

	AUTHOR:		Dreyk

	DESCRIPTION: DlgBar.cpp : implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "DlgBar.h"

#include "MainFrm.h"
#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RGB_TXT_IN			0xFF0000
#define CY_INDENT			5

#define REG_PANEL_ANIM		"Anim"
#define REG_PANEL_DELAY		"Delay"
#define REG_PANEL_DIRECT	"Direct"
#define REG_PANEL_EXTRA		"Extra"
#define REG_PANEL_SOUND		"Sound"
#define REG_PANEL_VIEW		"View"

/////////////////////////////////////////////////////////////////////////////
// CDlgBar dialog

CDlgBar::CDlgBar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBar)
	//}}AFX_DATA_INIT
	
	m_pPanelAnimation	= NULL;
	m_pPanelDelay		= NULL;
	m_pPanelDirection	= NULL;
	m_pPanelExtra		= NULL;
	m_pPanelSound		= NULL;
	m_pPanelView		= NULL;

	m_pView				= NULL;
	m_hAccelTable		= LoadAccelerators(AfxGetInstanceHandle(), (LPCTSTR)IDR_MAINFRAME);
}

void CDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBar)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBar, CDialog)
	//{{AFX_MSG_MAP(CDlgBar)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBar message handlers

BOOL CDlgBar::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
		if (m_pView && m_hAccelTable)
			TranslateAccelerator(m_pView->m_hWnd, m_hAccelTable, pMsg);

		switch (pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
    }

//	if (AfxGetMainWnd()->PreTranslateMessage(pMsg))
//		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgBar::OnInitDialog() 
{
	CDialog::OnInitDialog();

	BOOL bSuccess = FALSE;
	int nIndex = 0;
	CButtonX* pButton = NULL;

	// Setting up Rollup Control
	CRect rect;
	GetClientRect(rect);
	
/*	rect.top	+= CY_INDENT;
	rect.bottom	-= CY_INDENT;
*/
	bSuccess = m_wndRollup.Create(WS_CHILD | WS_VISIBLE, rect, this, 0);
	ASSERT(bSuccess);
	if (!bSuccess)
	{
		AfxMessageBox(IDS_ERR_EXIT, MB_ICONERROR);
		exit(0);
	}

	m_wndRollup.SetButtonHeight(23);

	// Create Direction panel
	m_pPanelDirection = new CPanelDirection(this);
	bSuccess = m_pPanelDirection->Create(IDD_PANEL_DIRECTION, &m_wndRollup);
	ASSERT(bSuccess);
	if (!bSuccess)
	{
		AfxMessageBox(IDS_ERR_EXIT, MB_ICONERROR);
		exit(0);
	}

	nIndex = m_wndRollup.InsertPage(_T("Direction"), m_pPanelDirection);
	pButton = m_wndRollup.GetPageInfo(nIndex)->pwndButton;
	pButton->Setup(
		IDI_PANEL_DIRECT,
		BX_ALIGN_RIGHT,
		"Direction panel",
		IDC_HAND,
		FALSE,
		RGB_TXT_IN);

	// Create Delay panel
	m_pPanelDelay = new CPanelDelay(this);
	bSuccess = m_pPanelDelay->Create(IDD_PANEL_DELAY, &m_wndRollup);
	ASSERT(bSuccess);
	if (!bSuccess)
	{
		AfxMessageBox(IDS_ERR_EXIT, MB_ICONERROR);
		exit(0);
	}
	nIndex = m_wndRollup.InsertPage(_T("Delay"), m_pPanelDelay);
	pButton = m_wndRollup.GetPageInfo(nIndex)->pwndButton;
	pButton->Setup(
		IDI_PANEL_DELAY,
		BX_ALIGN_RIGHT,
		"Delay panel",
		IDC_HAND,
		FALSE,
		RGB_TXT_IN);

	// Create Animation panel
	m_pPanelAnimation = new CPanelAnimation(this);
	bSuccess = m_pPanelAnimation->Create(IDD_PANEL_ANIMATION, &m_wndRollup);
	ASSERT(bSuccess);
	if (!bSuccess)
	{
		AfxMessageBox(IDS_ERR_EXIT, MB_ICONERROR);
		exit(0);
	}
	nIndex = m_wndRollup.InsertPage(_T("Animation"), m_pPanelAnimation);
	pButton = m_wndRollup.GetPageInfo(nIndex)->pwndButton;
	pButton->Setup(
		IDI_PANEL_ANIM,
		BX_ALIGN_RIGHT,
		"Animation panel",
		IDC_HAND,
		FALSE,
		RGB_TXT_IN);

	// Create View panel
	m_pPanelView = new CPanelView(this);
	bSuccess = m_pPanelView->Create(IDD_PANEL_VIEW, &m_wndRollup);
	ASSERT(bSuccess);
	if (!bSuccess)
	{
		AfxMessageBox(IDS_ERR_EXIT, MB_ICONERROR);
		exit(0);
	}
	nIndex = m_wndRollup.InsertPage(_T("View"), m_pPanelView);
	pButton = m_wndRollup.GetPageInfo(nIndex)->pwndButton;
	pButton->Setup(
		IDI_PANEL_VIEW,
		BX_ALIGN_RIGHT,
		"View panel",
		IDC_HAND,
		FALSE,
		RGB_TXT_IN);

	// Create Sound panel
	m_pPanelSound = new CPanelSound(this);
	bSuccess = m_pPanelSound->Create(IDD_PANEL_SOUND, &m_wndRollup);
	ASSERT(bSuccess);
	if (!bSuccess)
	{
		AfxMessageBox(IDS_ERR_EXIT, MB_ICONERROR);
		exit(0);
	}
	nIndex = m_wndRollup.InsertPage(_T("Sound"), m_pPanelSound);
	pButton = m_wndRollup.GetPageInfo(nIndex)->pwndButton;
	pButton->Setup(
		IDI_PANEL_SOUND,
		BX_ALIGN_RIGHT,
		"Sound panel",
		IDC_HAND,
		FALSE,
		RGB_TXT_IN);

	// Create Extra panel
	m_pPanelExtra = new CPanelExtra(this);
	bSuccess = m_pPanelExtra->Create(IDD_PANEL_EXTRA, &m_wndRollup);
	ASSERT(bSuccess);
	if (!bSuccess)
	{
		AfxMessageBox(IDS_ERR_EXIT, MB_ICONERROR);
		exit(0);
	}
	nIndex = m_wndRollup.InsertPage(_T("Extra"), m_pPanelExtra);
	pButton = m_wndRollup.GetPageInfo(nIndex)->pwndButton;
	pButton->Setup(
		IDI_PANEL_EXTRA,
		BX_ALIGN_RIGHT,
		"Extra panel",
		IDC_HAND,
		FALSE,
		RGB_TXT_IN);

	CWinApp* pApp = AfxGetApp();
	m_wndRollup.ExpandPage(0, pApp->GetProfileInt(REG_DLG_BAR, REG_PANEL_DIRECT, 0));
	m_wndRollup.ExpandPage(1, pApp->GetProfileInt(REG_DLG_BAR, REG_PANEL_DELAY, 0));
	m_wndRollup.ExpandPage(2, pApp->GetProfileInt(REG_DLG_BAR, REG_PANEL_ANIM, 0));
	m_wndRollup.ExpandPage(3, pApp->GetProfileInt(REG_DLG_BAR, REG_PANEL_VIEW, 0));
	m_wndRollup.ExpandPage(4, pApp->GetProfileInt(REG_DLG_BAR, REG_PANEL_SOUND, 0));
	m_wndRollup.ExpandPage(5, pApp->GetProfileInt(REG_DLG_BAR, REG_PANEL_EXTRA, 0));
	
	SetView(NULL);
	return TRUE;
}

CDocument* CDlgBar::GetDoc()
{
	return MAIN_APP->GetDoc();
}

CAnimationView* CDlgBar::GetView()
{
	return m_pView;
/*	CDocument *pDoc = GetDoc();

	if (!pDoc)
		return NULL;
	
	POSITION pos = pDoc->GetFirstViewPosition();
	if (!pos)
		return NULL;
		
	return (CAnimationView*)pDoc->GetNextView(pos);
*/
}

void CDlgBar::SetView(CAnimationView *pView)
{
	m_pView = pView;

	m_pPanelAnimation->SetView(m_pView);
	m_pPanelDelay->SetView(m_pView);
	m_pPanelDirection->SetView(m_pView);
	m_pPanelExtra->SetView(m_pView);
	m_pPanelSound->SetView(m_pView);
	m_pPanelView->SetView(m_pView);
}

BOOL CDlgBar::IsAttached()
{
	return (m_pView != NULL && m_pView->GetSafeHwnd());
}

void CDlgBar::UpdateControlsState()
{
	if (!IsAttached())
		return;

	m_pPanelAnimation->UpdateControlsState();
	m_pPanelDelay->UpdateControlsState();
	m_pPanelDirection->UpdateControlsState();
	m_pPanelExtra->UpdateControlsState();
	m_pPanelSound->UpdateControlsState();
	m_pPanelView->UpdateControlsState();
}

void CDlgBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_wndRollup.GetSafeHwnd())
	{
		::SetWindowPos(m_wndRollup.m_hWnd, NULL, 
			2, 5, cx-5, cy - 5,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CDlgBar::OnDestroy() 
{
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(REG_DLG_BAR, REG_PANEL_DIRECT, m_wndRollup.IsPageExpanded(0));
	pApp->WriteProfileInt(REG_DLG_BAR, REG_PANEL_DELAY, m_wndRollup.IsPageExpanded(1));
	pApp->WriteProfileInt(REG_DLG_BAR, REG_PANEL_ANIM, m_wndRollup.IsPageExpanded(2));
	pApp->WriteProfileInt(REG_DLG_BAR, REG_PANEL_VIEW, m_wndRollup.IsPageExpanded(3));
	pApp->WriteProfileInt(REG_DLG_BAR, REG_PANEL_SOUND, m_wndRollup.IsPageExpanded(4));
	pApp->WriteProfileInt(REG_DLG_BAR, REG_PANEL_EXTRA, m_wndRollup.IsPageExpanded(5));
	
	CDialog::OnDestroy();
}
