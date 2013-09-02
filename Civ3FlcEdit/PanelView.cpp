/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	16.09.2004
	VERSION:	5.0

	AUTHOR:		Dreyk

	DESCRIPTION: PanelView.cpp : implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "PanelView.h"

#include "DlgBar.h"
#include "MainFrm.h"
#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelView dialog


CPanelView::CPanelView(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelView)
	m_nPalette = 0;
	//}}AFX_DATA_INIT
	
	m_pView		= NULL;
	m_pParent	= (CDlgBar*)pParent;
}


void CPanelView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelView)
	DDX_Control(pDX, IDC_USE_ALPHA_BLENDING, m_btnAlphaBlend);
	DDX_Control(pDX, IDC_STATIC_CIV_COLOR, m_wndStaticCivColor);
	DDX_CBIndex(pDX, IDC_COMBO_PALETTES, m_nPalette);
	DDX_Control(pDX, IDC_COMBO_PALETTES, m_wndPalettes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelView, CDialog)
	//{{AFX_MSG_MAP(CPanelView)
	ON_CBN_SELENDOK(IDC_COMBO_PALETTES, OnSelendokComboPalettes)
	ON_BN_CLICKED(IDC_USE_ALPHA_BLENDING, OnUseAlphaBlending)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelView message handlers

BOOL CPanelView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Fill the Palettes colors combo box
	m_wndPalettes.EnableWindow(FALSE);
	m_wndPalettes.Initialize();
	
	SetView(NULL);
	return TRUE;
}

void CPanelView::SetView(CAnimationView *pView)
{
	m_pView = pView;
	
	if (!m_pView)
	{
		m_btnAlphaBlend.EnableWindow(FALSE);
		m_wndPalettes.EnableWindow(FALSE);
		m_wndStaticCivColor.EnableWindow(FALSE);
		
		UpdateData(FALSE);
	}
}

void CPanelView::OnSelendokComboPalettes() 
{
	if (!IsAttached())
		return;
	
	// Here should be changed FLC file's palette
	int nOldPalette = m_nPalette;
	
	UpdateData();
	
	if (nOldPalette == m_nPalette)
		return;
	
	m_pView->SelendokComboPalettes(nOldPalette);
}

void CPanelView::OnUseAlphaBlending() 
{
	if (IsAttached())
		m_pView->OnUseAlphaBlending();
}

BOOL CPanelView::IsAttached()
{
	return (m_pView != NULL && m_pView->GetSafeHwnd());
}

void CPanelView::UpdateControlsState()
{
	if (m_pView->IsLoaded())
	{
		if (m_pView->IsLeaderHead())
		{
			m_wndPalettes.EnableWindow(FALSE);
			m_btnAlphaBlend.EnableWindow(FALSE);
		}
		else
		{
			BOOL bEnable = FALSE;
			switch (MAIN_APP->GetPaletteType())
			{
			case PalGame:
				bEnable = MAIN_APP->IsCivRegistered();
				break;
			case PalRes:
				bEnable = TRUE;
				break;
			case PalDir:
				bEnable = uIsDirExist(MAIN_APP->GetPaletteDirectory());
				break;
			}
			
			m_wndPalettes.EnableWindow(bEnable);
			m_btnAlphaBlend.EnableWindow(TRUE);
		}
		
		m_btnAlphaBlend.SetCheck(m_pView->IsUseAlphaBlending());
		m_wndStaticCivColor.EnableWindow(TRUE);
	}
	else
	{
		m_btnAlphaBlend.EnableWindow(FALSE);
		m_wndPalettes.EnableWindow(FALSE);
		m_wndStaticCivColor.EnableWindow(FALSE);
	}
}


BOOL CPanelView::PreTranslateMessage(MSG* pMsg) 
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
