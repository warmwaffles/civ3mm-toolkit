/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	16.09.2004
	VERSION:	5.0

	AUTHOR:		Dreyk

	DESCRIPTION: PanelExtra.cpp : implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "PanelExtra.h"

#include "DlgBar.h"
#include "MainFrm.h"
#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelExtra dialog

CPanelExtra::CPanelExtra(CWnd* pParent /*=NULL*/)
  : CDialog(CPanelExtra::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPanelExtra)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_pView   = NULL;
    m_pParent = (CDlgBar*)pParent;
}

void
CPanelExtra::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPanelExtra)
    DDX_Control(pDX, IDC_INFO, m_btnInfo);
    DDX_Control(pDX, IDC_COLOR_TABLE, m_btnColorTable);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPanelExtra, CDialog)
//{{AFX_MSG_MAP(CPanelExtra)
ON_BN_CLICKED(IDC_INFO, OnInfo)
ON_BN_CLICKED(IDC_COLOR_TABLE, OnColorTable)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelExtra message handlers

BOOL
CPanelExtra::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_btnInfo.SetIcon(IDI_INFO);
    m_btnColorTable.SetIcon(IDI_PALETTE);

    SetView(NULL);
    return TRUE;
}

void
CPanelExtra::SetView(CAnimationView* pView)
{
    m_pView = pView;

    if (!m_pView) {
        m_btnInfo.EnableWindow(FALSE);
        m_btnColorTable.EnableWindow(FALSE);

        UpdateData(FALSE);
    }
}

void
CPanelExtra::OnInfo()
{
    if (IsAttached())
        m_pView->OnInfo();
}

void
CPanelExtra::OnColorTable()
{
    if (IsAttached())
        m_pView->OnColorTable();
}

BOOL
CPanelExtra::IsAttached()
{
    return (m_pView != NULL && m_pView->GetSafeHwnd());
}

void
CPanelExtra::UpdateControlsState()
{
    if (m_pView->IsLoaded()) {
        m_btnInfo.EnableWindow(TRUE);
        m_btnColorTable.EnableWindow(TRUE);
    } else {
        m_btnInfo.EnableWindow(FALSE);
        m_btnColorTable.EnableWindow(FALSE);
    }
}

BOOL
CPanelExtra::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
            case VK_RETURN:
            case VK_ESCAPE:
                ::TranslateMessage(pMsg);
                ::DispatchMessage(pMsg);
                return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}
