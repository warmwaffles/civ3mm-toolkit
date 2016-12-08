/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 OptionsDlgView.cpp
	CREATED:	 14.3.2003

	AUTHOR:		 Dreyk
	MODIFIED:	

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "OptionsDlgView.h"

#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgView dialog

COptionsDlgView::COptionsDlgView(CWnd* pParent /*=NULL*/)
  : CDialogX(COptionsDlgView::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptionsDlgView)
    m_bStatusBar       = FALSE;
    m_bToolBar         = FALSE;
    m_bBtnShowIcon     = FALSE;
    m_bBtnVisualEff    = FALSE;
    m_bMenuSameBorders = FALSE;
    m_bMenuBorder      = FALSE;
    m_bMenuGloom       = FALSE;
    m_bMenuSelection   = FALSE;
    m_nMenuStyle       = -1;
    //}}AFX_DATA_INIT

    m_bDrawGripper = FALSE;

    SetControlInfo(IDC_STATIC_BARS, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_2, CF_RESIZE_BOTH);

    SetControlInfo(IDC_TOOLBAR, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATUSBAR, CF_RESIZE_HOR);

    SetControlInfo(IDC_MENU_BORDER, CF_RESIZE_HOR);
    SetControlInfo(IDC_MENU_ALL_SAMEBORDER, CF_RESIZE_HOR);
    SetControlInfo(IDC_MENU_SHOWSELECTION, CF_RESIZE_HOR);
    SetControlInfo(IDC_MENU_ENABLEGLOOM, CF_RESIZE_HOR);
    SetControlInfo(IDC_MENU_XP, CF_RESIZE_HOR);
    SetControlInfo(IDC_MENU_OLD, CF_RESIZE_HOR);

    SetControlInfo(IDC_BUTTONX_VISUAL_EFFECTS, CF_RESIZE_HOR);
    SetControlInfo(IDC_BUTTONX_SHOW_ICON, CF_RESIZE_HOR);
}

void
COptionsDlgView::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptionsDlgView)
    DDX_Control(pDX, IDC_BUTTONX_SHOW_ICON, m_wndShowIcon);
    DDX_Control(pDX, IDC_MENU_ENABLEGLOOM, m_wndGloom);
    DDX_Check(pDX, IDC_STATUSBAR, m_bStatusBar);
    DDX_Check(pDX, IDC_TOOLBAR, m_bToolBar);
    DDX_Check(pDX, IDC_BUTTONX_SHOW_ICON, m_bBtnShowIcon);
    DDX_Check(pDX, IDC_BUTTONX_VISUAL_EFFECTS, m_bBtnVisualEff);
    DDX_Check(pDX, IDC_MENU_ALL_SAMEBORDER, m_bMenuSameBorders);
    DDX_Check(pDX, IDC_MENU_BORDER, m_bMenuBorder);
    DDX_Check(pDX, IDC_MENU_ENABLEGLOOM, m_bMenuGloom);
    DDX_Check(pDX, IDC_MENU_SHOWSELECTION, m_bMenuSelection);
    DDX_Radio(pDX, IDC_MENU_XP, m_nMenuStyle);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionsDlgView, CDialogX)
//{{AFX_MSG_MAP(COptionsDlgView)
ON_BN_CLICKED(IDC_TOOLBAR, OnChanged)
ON_BN_CLICKED(IDC_STATUSBAR, OnChanged)
ON_BN_CLICKED(IDC_BUTTONX_SHOW_ICON, OnChanged)
ON_BN_CLICKED(IDC_BUTTONX_VISUAL_EFFECTS, OnChangedVisualEffects)
ON_BN_CLICKED(IDC_MENU_ALL_SAMEBORDER, OnChanged)
ON_BN_CLICKED(IDC_MENU_BORDER, OnChanged)
ON_BN_CLICKED(IDC_MENU_ENABLEGLOOM, OnChanged)
ON_BN_CLICKED(IDC_MENU_OLD, OnChangedStyle)
ON_BN_CLICKED(IDC_MENU_SHOWSELECTION, OnChanged)
ON_BN_CLICKED(IDC_MENU_XP, OnChangedStyle)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgView message handlers

BOOL
COptionsDlgView::OnInitDialog()
{
    CDialogX::OnInitDialog();

    CWinApp* pApp = AfxGetApp();

    // Get control bars visual aspect
    m_bToolBar   = pApp->GetProfileInt(REG_OPTIONS, REG_SHOW_TOOLBAR, 1);
    m_bStatusBar = pApp->GetProfileInt(REG_OPTIONS, REG_SHOW_STATUSBAR, 1);

    m_bMenuSelection   = pApp->GetProfileInt(REG_MENU, REG_MENU_SELECTION, 1);
    m_bMenuGloom       = CNewMenu::GetXpBlendig();
    m_bMenuSameBorders = CNewMenu::GetNewMenuBorderAllMenu();

    int nStyle = pApp->GetProfileInt(REG_MENU, REG_MENU_STYLE, 2);

    switch (nStyle) {
        case CNewMenu::STYLE_ORIGINAL:
            m_nMenuStyle  = 1;
            m_bMenuBorder = TRUE;
            break;

        case CNewMenu::STYLE_ORIGINAL_NOBORDER:
            m_nMenuStyle  = 1;
            m_bMenuBorder = FALSE;
            break;

        case CNewMenu::STYLE_XP:
            m_nMenuStyle  = 0;
            m_bMenuBorder = TRUE;
            break;

        case CNewMenu::STYLE_XP_NOBORDER:
            m_nMenuStyle  = 0;
            m_bMenuBorder = FALSE;
            break;
    }

    m_wndGloom.EnableWindow(m_nMenuStyle == 0);

#ifdef BX_USE_SETTINGS
    UINT nFlags = pApp->GetProfileInt(BX_REG_KEY, BX_REG_SETTINGS, 0);

    m_bBtnVisualEff = !(nFlags & BX_OFF_OWNERDRAW);
    m_bBtnShowIcon  = !(nFlags & BX_OFF_ICON);
    m_wndShowIcon.EnableWindow(m_bBtnVisualEff);
#endif // BX_USE_SETTINGS

    UpdateData(FALSE);
    return TRUE;
}

void
COptionsDlgView::OnChangeData()
{
    COptionsDlg* pDlg = (COptionsDlg*)GetParent();
    pDlg->m_btnApply.EnableWindow();
}

void
COptionsDlgView::Apply()
{
    UpdateData();

    CWinApp* pApp = AfxGetApp();

    // Save control bars visual aspect
    pApp->WriteProfileInt(REG_OPTIONS, REG_SHOW_TOOLBAR, m_bToolBar);
    pApp->WriteProfileInt(REG_OPTIONS, REG_SHOW_STATUSBAR, m_bStatusBar);

    // Menues
    int nStyle = CNewMenu::STYLE_XP;

    if (m_nMenuStyle == 1 && m_bMenuBorder)
        nStyle = CNewMenu::STYLE_ORIGINAL;
    else if (m_nMenuStyle == 1 && !m_bMenuBorder)
        nStyle = CNewMenu::STYLE_ORIGINAL_NOBORDER;
    else if (m_nMenuStyle == 0 && m_bMenuBorder)
        nStyle = CNewMenu::STYLE_XP;
    else if (m_nMenuStyle == 0 && !m_bMenuBorder)
        nStyle = CNewMenu::STYLE_XP_NOBORDER;

    CNewMenu::SetMenuDrawMode(nStyle);
    pApp->WriteProfileInt(REG_MENU, REG_MENU_STYLE, nStyle);

    CNewMenu::SetXpBlendig(m_bMenuGloom);
    pApp->WriteProfileInt(REG_MENU, REG_MENU_GLOOM, m_bMenuGloom);

    CNewMenu::SetSelectDisableMode(m_bMenuSelection);
    pApp->WriteProfileInt(REG_MENU, REG_MENU_SELECTION, m_bMenuSelection);

    CNewMenu::SetNewMenuBorderAllMenu(m_bMenuSameBorders);

// Buttons
#ifdef BX_USE_SETTINGS
    UINT nFlags = 0;

    if (!m_bBtnVisualEff)
        nFlags |= BX_OFF_OWNERDRAW; // disable all visual effects

    if (!m_bBtnShowIcon)
        nFlags |= BX_OFF_ICON; // not show icon

    // SendMessageToDescendants( UINT message, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE );
    // Sends the specified message to all immediate children of the CWindow object.
    // If bDeep is TRUE, the message is additionally sent to all other descendant windows.
    MAIN_FRAME->SendMessageToDescendants(WM_BX_SETTINGCHANGE, NULL, (LPARAM)nFlags, TRUE);

    COptionsDlg* pParent = (COptionsDlg*)GetParent();

    if (pParent->GetSafeHwnd()) {
        pParent->SendMessageToDescendants(WM_BX_SETTINGCHANGE, NULL, (LPARAM)nFlags);
    }

    pApp->WriteProfileInt(BX_REG_KEY, BX_REG_SETTINGS, nFlags);

    // Setup style for all ButtonX in our app
    CButtonX::SetButtonsSettings(nFlags);
#endif // BX_USE_SETTINGS

    UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Reset all values to the default state.
//
void
COptionsDlgView::SetDefault()
{
    m_bToolBar   = TRUE;
    m_bStatusBar = TRUE;

    // Menues
    m_bMenuSameBorders = TRUE;
    m_bMenuBorder      = TRUE;
    m_bMenuGloom       = TRUE;
    m_bMenuSelection   = TRUE;
    m_nMenuStyle       = 0;

    // Buttons
    m_bBtnVisualEff = TRUE;
    m_bBtnShowIcon  = TRUE;

    m_wndGloom.EnableWindow(TRUE);
    m_wndShowIcon.EnableWindow(TRUE);

    UpdateData(FALSE);
}

void
COptionsDlgView::OnChanged()
{
    OnChangeData();
}

void
COptionsDlgView::OnChangedStyle()
{
    UpdateData();
    m_wndGloom.EnableWindow(m_nMenuStyle == 0);

    OnChangeData();
}

void
COptionsDlgView::OnChangedVisualEffects()
{
    UpdateData();
    m_wndShowIcon.EnableWindow(m_bBtnVisualEff);

    OnChangeData();
}
