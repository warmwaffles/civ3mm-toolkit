/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	16.09.2004
	VERSION:	5.0

	AUTHOR:		Dreyk

	DESCRIPTION: PanelDirection.cpp : implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "PanelDirection.h"

#include "DlgBar.h"
#include "MainFrm.h"
#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelDirection dialog


CPanelDirection::CPanelDirection(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelDirection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelDirection)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pView		= NULL;
	m_pParent	= (CDlgBar*)pParent;
}


void CPanelDirection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelDirection)
	DDX_Control(pDX, IDC_DIRECTION_W, m_btnW);
	DDX_Control(pDX, IDC_DIRECTION_SW, m_btnSW);
	DDX_Control(pDX, IDC_DIRECTION_SE, m_btnSE);
	DDX_Control(pDX, IDC_DIRECTION_S, m_btnS);
	DDX_Control(pDX, IDC_DIRECTION_NW, m_btnNW);
	DDX_Control(pDX, IDC_DIRECTION_NE, m_btnNE);
	DDX_Control(pDX, IDC_DIRECTION_N, m_btnN);
	DDX_Control(pDX, IDC_DIRECTION_E, m_btnE);
	DDX_Control(pDX, IDC_DIRECTION_ALL, m_btnAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelDirection, CDialog)
	//{{AFX_MSG_MAP(CPanelDirection)
	ON_BN_CLICKED(IDC_DIRECTION_SW, OnDirectionSW)
	ON_BN_CLICKED(IDC_DIRECTION_W, OnDirectionW)
	ON_BN_CLICKED(IDC_DIRECTION_NW, OnDirectionNW)
	ON_BN_CLICKED(IDC_DIRECTION_N, OnDirectionN)
	ON_BN_CLICKED(IDC_DIRECTION_NE, OnDirectionNE)
	ON_BN_CLICKED(IDC_DIRECTION_E, OnDirectionE)
	ON_BN_CLICKED(IDC_DIRECTION_SE, OnDirectionSE)
	ON_BN_CLICKED(IDC_DIRECTION_S, OnDirectionS)
	ON_BN_CLICKED(IDC_DIRECTION_ALL, OnDirectionAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelDirection message handlers

BOOL CPanelDirection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btnW.SetIcon(IDI_DIR_W);
	m_btnSW.SetIcon(IDI_DIR_SW);
	m_btnSE.SetIcon(IDI_DIR_SE);
	m_btnS.SetIcon(IDI_DIR_S);
	m_btnNW.SetIcon(IDI_DIR_NW);
	m_btnNE.SetIcon(IDI_DIR_NE);
	m_btnN.SetIcon(IDI_DIR_N);
	m_btnE.SetIcon(IDI_DIR_E);
	m_btnAll.SetIcon(IDI_DIR_ALL);
	
	m_btnW.SetTooltipText(IDS_DIRECT_W);
	m_btnSW.SetTooltipText(IDS_DIRECT_SW);
	m_btnSE.SetTooltipText(IDS_DIRECT_SE);
	m_btnS.SetTooltipText(IDS_DIRECT_S);
	m_btnNW.SetTooltipText(IDS_DIRECT_NW);
	m_btnNE.SetTooltipText(IDS_DIRECT_NE);
	m_btnN.SetTooltipText(IDS_DIRECT_N);
	m_btnE.SetTooltipText(IDS_DIRECT_E);
	m_btnAll.SetTooltipText(IDS_DIRECT_ALL);
	
	m_btnW.DrawFlatFocus(TRUE);
	m_btnSW.DrawFlatFocus(TRUE);
	m_btnSE.DrawFlatFocus(TRUE);
	m_btnS.DrawFlatFocus(TRUE);
	m_btnNW.DrawFlatFocus(TRUE);
	m_btnNE.DrawFlatFocus(TRUE);
	m_btnN.DrawFlatFocus(TRUE);
	m_btnE.DrawFlatFocus(TRUE);
	m_btnAll.DrawFlatFocus(TRUE);
	
	SetView(NULL);
	return TRUE;
}
void CPanelDirection::SetView(CAnimationView *pView)
{
	m_pView = pView;
	
	if (!m_pView)
	{
		m_btnW.EnableWindow(FALSE);
		m_btnSW.EnableWindow(FALSE);
		m_btnSE.EnableWindow(FALSE);
		m_btnS.EnableWindow(FALSE);
		m_btnNW.EnableWindow(FALSE);
		m_btnNE.EnableWindow(FALSE);
		m_btnN.EnableWindow(FALSE);
		m_btnE.EnableWindow(FALSE);
		m_btnAll.EnableWindow(FALSE);\
		
		UpdateData(FALSE);
	}
}


void CPanelDirection::OnDirectionSW() 
{
	if (IsAttached())
		m_pView->SetDirection(DirSW);
}

void CPanelDirection::OnDirectionW() 
{
	if (IsAttached())
		m_pView->SetDirection(DirW);
}

void CPanelDirection::OnDirectionNW() 
{
	if (IsAttached())
		m_pView->SetDirection(DirNW);
}

void CPanelDirection::OnDirectionN() 
{
	if (IsAttached())
		m_pView->SetDirection(DirN);
}

void CPanelDirection::OnDirectionNE() 
{
	if (IsAttached())
		m_pView->SetDirection(DirNE);
}

void CPanelDirection::OnDirectionE() 
{
	if (IsAttached())
		m_pView->SetDirection(DirE);
}

void CPanelDirection::OnDirectionSE() 
{
	if (IsAttached())
		m_pView->SetDirection(DirSE);
}

void CPanelDirection::OnDirectionS() 
{
	if (IsAttached())
		m_pView->SetDirection(DirS);
}

void CPanelDirection::OnDirectionAll() 
{
	if (IsAttached())
		m_pView->SetDirection(DirAll);
}

BOOL CPanelDirection::IsAttached()
{
	return (m_pView != NULL && m_pView->GetSafeHwnd());
}

void CPanelDirection::UpdateControlsState()
{
	if (m_pView->IsLoaded())
	{
		BOOL bEnable = (m_pView->m_nDirCount == DIRECTIONS_MAX);
		
		m_btnW.EnableWindow(bEnable);
		m_btnSW.EnableWindow(bEnable);
		m_btnSE.EnableWindow(bEnable);
		m_btnS.EnableWindow(bEnable);
		m_btnNW.EnableWindow(bEnable);
		m_btnNE.EnableWindow(bEnable);
		m_btnN.EnableWindow(bEnable);
		m_btnE.EnableWindow(bEnable);
		m_btnAll.EnableWindow(TRUE);

		EDirection eDirection = m_pView->GetDirection();
		m_btnW.SetCheck(eDirection == DirW);
		m_btnSW.SetCheck(eDirection == DirSW);
		m_btnSE.SetCheck(eDirection == DirSE);
		m_btnS.SetCheck(eDirection == DirS);
		m_btnNW.SetCheck(eDirection == DirNW);
		m_btnNE.SetCheck(eDirection == DirNE);
		m_btnN.SetCheck(eDirection == DirN);
		m_btnE.SetCheck(eDirection == DirE);
		m_btnAll.SetCheck(eDirection == DirAll);
	}
	else
	{
		m_btnW.EnableWindow(FALSE);
		m_btnSW.EnableWindow(FALSE);
		m_btnSE.EnableWindow(FALSE);
		m_btnS.EnableWindow(FALSE);
		m_btnNW.EnableWindow(FALSE);
		m_btnNE.EnableWindow(FALSE);
		m_btnN.EnableWindow(FALSE);
		m_btnE.EnableWindow(FALSE);
		m_btnAll.EnableWindow(FALSE);
	}
}

BOOL CPanelDirection::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
    {
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}
