/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	MainFrm.cpp
	CREATED:	26.08.2004
	VERSION:	5.4

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the CMainFrame class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "MainFrm.h"

#include "FlcView.h"
#include "OptionsDlg.h"
#include "FlcDoc.h"
#include "C3fDoc.h"
#include "DocManagerEx.h"
#include "ImportToC3F.h"
#include "ColorReplaceDlg.h"

#include "..\MFCaids\ColourPickerXP.h"
//#include "..\MFCaids\FadeWnd.h"

#include <../src/afximpl.h> // to use AfxDeferRegisterClass

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CNewMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CNewMDIFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_WM_CLOSE()
ON_COMMAND(ID_OPTIONS, OnOptions)
ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
ON_COMMAND(ID_VIEW_DIALOGBAR, OnViewDialogBar)
ON_UPDATE_COMMAND_UI(ID_VIEW_DIALOGBAR, OnUpdateViewDialogBar)
ON_COMMAND(ID_ALWAYSONTOP, OnAlwaysontop)
ON_UPDATE_COMMAND_UI(ID_ALWAYSONTOP, OnUpdateAlwaysontop)
ON_COMMAND(ID_C3F_NEW, OnNewStoryboard)
ON_UPDATE_COMMAND_UI(ID_PLAY, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdateFALSEandUNCHECK)
ON_COMMAND(ID_OPEN_INI, OnOpenINI)
ON_COMMAND(ID_OPEN_MULTI, OnOpenMulti)
ON_WM_COPYDATA()
ON_COMMAND(ID_IMPORT_C3F, OnImporC3f)
ON_COMMAND(ID_COLOR_REPLACE, OnColorReplace)
ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_INFO, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_BGD_DEFAULT, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_BGD_LAND, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_BGD_SEA, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_BGD_SELECT, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_CUSTOM_COLOR, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_FIRST, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_PREVIOUS, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_NEXT, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_LAST, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_COLOR_TABLE, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_BACK, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_FORWARD, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_IMPORT_PALETTE, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_EXPORT_PALETTE, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_REFRESH, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_USE_ALPHA_BLENDING, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_COPY_TO_FILE, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_COPY_TO_CLIPBOARD, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_FLC_EXPORT, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_C3F_EXPORT, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_ZOOM, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSEALL, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_OFFSET, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_SAVE_FRAME, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_ALL, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_E, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_N, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_NE, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_NW, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_S, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_SE, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_SW, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_DIRECTION_W, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_BGD_GRID, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_BGD_IMG, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_SELECT_IMG, OnUpdateFALSE)
ON_UPDATE_COMMAND_UI(ID_SHRINK_FRM_COUNT, OnUpdateFALSE)
//}}AFX_MSG_MAP
//	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
ON_NOTIFY(NM_CUSTOMDRAW, AFX_IDW_TOOLBAR, OnDrawColorButton)
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR, // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

static TBBUTTONSETUP BtnsMain[] = {
    { { 0, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 0, 0 }, TRUE, IDR_MRU },
    { { 1, ID_FILE_CLOSE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 2, ID_OPEN_INI, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 3, ID_OPEN_MULTI, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 4, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 5, ID_FILE_SAVE_AS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 6, ID_FLC_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN | BTNS_WHOLEDROPDOWN, 0, 0 }, FALSE, IDR_MRU_FLC },
    { { 7, ID_C3F_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN | BTNS_WHOLEDROPDOWN, 0, 0 }, FALSE, IDR_MRU_C3F },
    { { 8, ID_C3F_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 9, ID_FLC_EXPORT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 10, ID_C3F_EXPORT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 11, ID_REFRESH, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 12, ID_APP_EXIT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 13, ID_PLAY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 14, ID_PAUSE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 15, ID_STOP, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 16, ID_FIRST, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 17, ID_BACK, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 18, ID_PREVIOUS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 19, ID_NEXT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 20, ID_FORWARD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 21, ID_LAST, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 22, ID_BGD_SELECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 23, ID_COLOR_TABLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 24, ID_IMPORT_PALETTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 25, ID_EXPORT_PALETTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 26, ID_INFO, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 27, ID_COPY_TO_CLIPBOARD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 28, ID_COPY_TO_FILE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 29, ID_ZOOM, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 30, ID_WINDOW_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 31, ID_WINDOW_CASCADE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 32, ID_WINDOW_TILE_HORZ, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 33, ID_WINDOW_ARRANGE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 34, ID_WINDOW_CLOSEALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 35, ID_OPTIONS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, TRUE },
    { { 36, ID_APP_ABOUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 37, ID_OFFSET, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 38, ID_IMPORT_C3F, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 39, ID_SAVE_FRAME, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 40, ID_DIRECTION_SW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 41, ID_DIRECTION_W, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 42, ID_DIRECTION_NW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 43, ID_DIRECTION_N, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 44, ID_DIRECTION_NE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 45, ID_DIRECTION_E, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 46, ID_DIRECTION_SE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 47, ID_DIRECTION_S, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 48, ID_DIRECTION_ALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 49, ID_COLOR_REPLACE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 50, ID_SELECT_IMG, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE },
    { { 51, ID_SHRINK_FRM_COUNT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, FALSE }
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    m_crBgd       = ::GetSysColor(COLOR_BTNFACE);
    m_hWndTopMost = HWND_NOTOPMOST;
}

CMainFrame::~CMainFrame()
{
}

int
CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CNewMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    LoadFromRegistry(); // get some information from registry

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // m_wndStatusBar
    if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar\n");
        return -1; // fail to create
    }

    m_wndStatusBar.ShowWindow(m_bShowStatusBar ? SW_SHOW : SW_HIDE);

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // m_wndToolBar
    if (!m_wndToolBar.CreateEx(this)) {
        TRACE0("Failed to create ToolBar\n");
        return -1; // fail to create
    }

    m_wndToolBar.SetButtons(sizeof(BtnsMain) / sizeof(BtnsMain[0]), BtnsMain, toNoTextLabels);
    m_wndToolBar.SetBitmap(IDB_MAINFRAME, ioSmallIcons, RGB_BACKGROUND);
    m_wndToolBar.SetWindowText(uLoadString(IDS_TOOLBAR_CAPTION));
    //	m_wndToolBar.AddDropDownButton(ID_FILE_OPEN, IDR_MRU, TRUE);
    //	m_wndToolBar.AddDropDownButton(ID_FLC_OPEN, IDR_MRU_FLC, FALSE);
    //	m_wndToolBar.AddDropDownButton(ID_C3F_OPEN, IDR_MRU_C3F, FALSE);
    m_wndToolBar.RestoreState();

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // m_wndReBar
    if (!m_wndReBar.Create(this) || !m_wndReBar.AddBar(&m_wndToolBar)) {
        TRACE0("Failed to create rebar\n");
        return -1; // fail to create
    }

    m_wndReBar.RestoreState();
    /*	
	// Setup buttons, which haven't text in the "Text on right" mode
	CDWordArray arID;
	arID.Add(ID_APP_EXIT);
	arID.Add(ID_APP_ABOUT);
	
	m_wndToolBar.SetNoTextOnRight(arID);
*/

    EnableDocking(CBRS_ALIGN_ANY);

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // m_wndDialogBar
    // Try to create the DialogBar
    if (!m_wndDialogBar.Create(this, &m_wndDlgBar, uLoadString(IDS_DIALOGBAR_TITLE), IDD_DIALOG_BAR)) {
        TRACE0("Failed to create scheme bar\n");
        return -1; // fail to create
    }

    m_wndDialogBar.SetBarStyle(m_wndDialogBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED /*CBRS_SIZE_DYNAMIC*/);
    m_wndDialogBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
    DockControlBar(&m_wndDialogBar);

    if (AfxGetApp()->GetProfileInt(m_wndDialogBar.RegKey(), REG_FLOATING, FALSE))
        m_wndDialogBar.ToggleDocking();

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // Load and set dock state
    CDockState DockState;
    DockState.LoadState(REG_CONTROL_BAR);
    for (int i = 0; i < DockState.m_arrBarInfo.GetSize(); i++) {
        CControlBarInfo* pInfo = (CControlBarInfo*)DockState.m_arrBarInfo[i];
        if (GetControlBar(pInfo->m_nBarID) == NULL)
            DockState.m_arrBarInfo.RemoveAt(i--);
    }
    SetDockState(DockState);

    UpdateViewOptions();

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // NewMenu

    // Load our own toolbar
    m_DefaultNewMenu.LoadToolBar(TbMain, RGB_BACKGROUND);

    // Set the color in the bitmaps that is the background transparent color
    m_DefaultNewMenu.SetBitmapBackground(RGB_BACKGROUND);

    // Use ModifyODMenu to add a bitmap to a menu options.
    m_DefaultNewMenu.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_FLC), IDB_FLCTYPE);
    m_DefaultNewMenu.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_C3F), IDB_C3FTYPE);

    // Set title to the system menu
    m_SystemNewMenu.SetMenuTitle(uLoadString(AFX_IDS_APP_TITLE), MFT_GRADIENT | MFT_SIDE_TITLE);

    //	CWnd::RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, CWnd::reposDefault, NULL);
    //	OnUpdateFrameMenu(NULL);
    //	DrawMenuBar();

    return 0;
}

BOOL
CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CNewMDIFrameWnd::PreCreateWindow(cs))
        return FALSE;

    //	cs.style |= WS_MAXIMIZEBOX;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void
CMainFrame::AssertValid() const
{
    CNewMDIFrameWnd::AssertValid();
}

void
CMainFrame::Dump(CDumpContext& dc) const
{
    CNewMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/////////////////////////////////////////////////////////////////////////
// Get some information from the registry.
//
void
CMainFrame::LoadFromRegistry()
{
    CWinApp* pApp = AfxGetApp();

    // Get control bars visual aspect
    m_bShowToolBar   = pApp->GetProfileInt(REG_OPTIONS, REG_SHOW_TOOLBAR, 1);
    m_bShowStatusBar = pApp->GetProfileInt(REG_OPTIONS, REG_SHOW_STATUSBAR, 1);
    m_bShowDialogBar = pApp->GetProfileInt(REG_OPTIONS, REG_SHOW_DIALOGBAR, 1);

    // Is app window always on top or not
    BOOL bRes     = pApp->GetProfileInt(REG_OPTIONS, REG_ALWAYS_ON_TOP, 0);
    m_hWndTopMost = bRes ? HWND_TOPMOST : HWND_NOTOPMOST;
}

/////////////////////////////////////////////////////////////////////////
// Set some information to the registry.
//
void
CMainFrame::SaveToRegistry()
{
    CWinApp* pApp = AfxGetApp();

    // Save control bars visual aspect
    pApp->WriteProfileInt(REG_OPTIONS, REG_SHOW_TOOLBAR, m_wndToolBar.IsWindowVisible());
    pApp->WriteProfileInt(REG_OPTIONS, REG_SHOW_STATUSBAR, m_wndStatusBar.IsWindowVisible());
    pApp->WriteProfileInt(REG_OPTIONS, REG_SHOW_DIALOGBAR, m_wndDlgBar.IsWindowVisible());

    SavePosition(this);     // save frame position
    m_wndReBar.SaveState(); // save ReBar state

    CDockState DockState;
    GetDockState(DockState);
    DockState.SaveState(REG_CONTROL_BAR); // save dock state

    // Is app window always on top or not
    pApp->WriteProfileInt(REG_OPTIONS, REG_ALWAYS_ON_TOP, m_hWndTopMost == HWND_TOPMOST ? TRUE : FALSE);
}

void
CMainFrame::OnClose()
{
    /*	// Request before app exit
	if (!MAIN_APP->GetDoc()->IsModified())
	{
		if (AfxMessageBox(IDS_APP_EXIT, MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;
	}
*/
    SaveToRegistry();
    CNewMDIFrameWnd::OnClose();
}

/////////////////////////////////////////////////////////////////////////
// Update view state of the control bars and menu manager.
//
void
CMainFrame::UpdateViewOptions()
{
    // set view states for control bars
    ShowDialogBar(m_bShowDialogBar);
    ShowControlBar(&m_wndStatusBar, m_bShowStatusBar, FALSE);
    ShowToolBars();

    // Set background color for CColourPopupXP the same as CNewMenu
    int nStyle = CNewMenu::GetMenuDrawMode();

    if (nStyle == CNewMenu::STYLE_XP || nStyle == CNewMenu::STYLE_XP_NOBORDER)
        CColourPopupXP::m_bMenuXpStyle = TRUE;
    else
        CColourPopupXP::m_bMenuXpStyle = FALSE;
}

/////////////////////////////////////////////////////////////////////////
// Update view status for toolbars.
//
void
CMainFrame::ShowToolBars()
{
    m_wndToolBar.ShowWindow(m_bShowToolBar ? SW_SHOW : SW_HIDE);

    CWnd::RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, CWnd::reposDefault, NULL);
}

void
CMainFrame::OnViewToolbar()
{
    m_bShowToolBar = !m_wndToolBar.IsWindowVisible();
    ShowToolBars();
}

void
CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndToolBar.IsWindowVisible());
}

void
CMainFrame::OnViewStatusBar()
{
    m_bShowStatusBar = !m_bShowStatusBar;
    ShowControlBar(&m_wndStatusBar, m_bShowStatusBar, FALSE);
}

void
CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bShowStatusBar);
}

void
CMainFrame::ApplyOptions()
{
    LoadFromRegistry();
    UpdateViewOptions();

    CAnimationView* pView = GetAnimationView(); //!!!
    if (pView)
        pView->UpdateControlsState();
}

void
CMainFrame::OnOptions()
{
    SaveToRegistry();

    COptionsDlg dlg;
    dlg.DoModal();

    CDocManagerEx* pDocMan = MAIN_APP->GetDocManager();

    POSITION posDoc            = NULL;
    POSITION pos               = NULL;
    CDocTemplate* pDocTemplate = pDocMan->GetDocTemplateFlc();

    if (pDocTemplate) {
        posDoc = pDocTemplate->GetFirstDocPosition();

        CFlcDoc* pFlcDoc = NULL;
        while (posDoc) {
            pFlcDoc = (CFlcDoc*)pDocTemplate->GetNextDoc(posDoc);

            if (pFlcDoc) {
                CAnimationView* pView = NULL;
                pos                   = pFlcDoc->GetFirstViewPosition();

                while (pos) {
                    pView = (CAnimationView*)pFlcDoc->GetNextView(pos);

                    if (pView)
                        pView->UpdateParams();
                }
            }
        }
    }

    posDoc       = NULL;
    pos          = NULL;
    pDocTemplate = pDocMan->GetDocTemplateC3f();

    if (pDocTemplate) {
        posDoc = pDocTemplate->GetFirstDocPosition();

        CC3fDoc* pC3fDoc = NULL;
        while (posDoc) {
            pC3fDoc = (CC3fDoc*)pDocTemplate->GetNextDoc(posDoc);

            if (pC3fDoc) {
                CAnimationView* pView = NULL;
                pos                   = pC3fDoc->GetFirstViewPosition();

                while (pos) {
                    pView = (CAnimationView*)pC3fDoc->GetNextView(pos);

                    if (pView)
                        pView->UpdateParams();
                }
            }
        }
    }
}

BOOL
CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
    /*	if (!CNewMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
		return FALSE;
	else
		return TRUE;
*/
    ASSERT_VALID_IDR(nIDResource);
    ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

    m_nIDHelp = nIDResource; // ID for help context (HID_BASE_RESOURCE)

    CString strFullString;
    if (strFullString.LoadString(nIDResource))
        AfxExtractSubString(m_strTitle, strFullString, 0); // first sub-string

    VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));

    // Remove maximize button and frame resizing
    //	dwDefaultStyle &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX/* | WS_VISIBLE*/);

    LPCTSTR lpszClass = CIV3FLC_CLASS_NAME;

    /////////////////////////////////////////////////////////////////
    ASSERT_VALID_IDR(nIDResource);
    HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_GROUP_ICON);
    HICON hIcon     = ::LoadIcon(hInst, MAKEINTRESOURCE(nIDResource));
    if (hIcon != NULL) {
        CREATESTRUCT cs;
        memset(&cs, 0, sizeof(CREATESTRUCT));
        cs.style = dwDefaultStyle;
        PreCreateWindow(cs);

        // ignore instance handle from PreCreateWindow.
        HINSTANCE hInst = AfxGetInstanceHandle();

        // see if the class already exists
        WNDCLASS wndcls;

        if (cs.lpszClass != NULL && GetClassInfo(hInst, cs.lpszClass, &wndcls) && wndcls.hIcon != hIcon) {
            // we need to register a new class
            //			wndcls.style			= dwDefaultStyle;
            //			wndcls.lpfnWndProc		= DefWindowProc;
            //			wndcls.cbClsExtra		= 0;
            //			wndcls.cbWndExtra		= 0;
            //			wndcls.hInstance		= hInst;
            wndcls.hIcon = hIcon;
            //			wndcls.hCursor			= hCursor;
            //			wndcls.hbrBackground	= hbrBackground;
            //			wndcls.lpszMenuName		= NULL;
            wndcls.lpszClassName = lpszClass;

            if (!::RegisterClass(&wndcls)) {
                TRACE("Can't register window class\n");
                return FALSE;
            }
        }
    }

    /////////////////////////////////////////////////////////////////
    // attempt to create the window
    LPCTSTR lpszTitle = m_strTitle;

    if (!Create(lpszClass, lpszTitle, dwDefaultStyle, rectDefault, pParentWnd, MAKEINTRESOURCE(nIDResource), 0L, pContext)) {
        return FALSE; // will self destruct on failure normally
    }

    // save the default menu handle
    ASSERT(m_hWnd != NULL);
    m_hMenuDefault = ::GetMenu(m_hWnd);

    // load accelerator resource
    LoadAccelTable(MAKEINTRESOURCE(nIDResource));

    if (pContext == NULL) // send initial update
        SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

    // save menu to use when no active MDI child window is present
    ASSERT(m_hWnd != NULL);
    m_hMenuDefault = ::GetMenu(m_hWnd);
    if (m_hMenuDefault == NULL)
        TRACE0("Warning: CNewMDIFrameWnd without a default menu.\n");

    LoadPosition(this);
    return TRUE;
}
/*
void CMainFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{
    NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
ASSERT(FALSE);	
#ifdef NEW_MENU_INCLUDED
	CNewMenu menu;
#else
	CMenu menu;
#endif
	
    CMenu* pPopup = NULL;
	
    switch (pNMToolBar->iItem)
    {
	case ID_FILE_OPEN:
		VERIFY(menu.LoadMenu(IDR_MRU));
		menu.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_FLC), IDB_FLCTYPE);
		menu.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_C3F), IDB_C3FTYPE);		
		pPopup = menu.GetSubMenu(0);
		break;

	case ID_FLC_OPEN:
		VERIFY(menu.LoadMenu(IDR_MRU_FLC));
		pPopup = menu.GetSubMenu(0);
		break;
	
	case ID_C3F_OPEN:
		VERIFY(menu.LoadMenu(IDR_MRU_C3F));
		pPopup = menu.GetSubMenu(0);
		break;

	default:
		ASSERT(FALSE);
		break;
    }
	
    if (pPopup != 0)
    {
        CRect rc;
        ::SendMessage(pNMToolBar->hdr.hwndFrom, 
            TB_GETRECT, pNMToolBar->iItem, (LPARAM)&rc);
        rc.top = rc.bottom;
        ::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rc.TopLeft());
        pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rc.left, rc.top, this);
    }
	
    *pResult = TBDDRET_DEFAULT;
}
*/
CToolBarX*
CMainFrame::GetToolBar()
{
    return &m_wndToolBar;
}

/////////////////////////////////////////////////////////////////////////////
// This function draws color indicators on the toolbar buttons.
//
void
CMainFrame::OnDrawColorButton(NMHDR* pNotifyStruct, LRESULT* result)
{
    ASSERT(pNotifyStruct->idFrom == AFX_IDW_TOOLBAR);

    NMTBCUSTOMDRAW* cdr = (NMTBCUSTOMDRAW*)pNotifyStruct;

    // Draw color bar indicators
    if (cdr->nmcd.dwDrawStage == CDDS_POSTPAINT) {
        int n    = m_wndToolBar.GetToolBarCtrl().GetButtonCount();
        CDC* pDC = CDC::FromHandle(cdr->nmcd.hdc);
        CRect r;
        UINT id;
        TBBUTTONINFO bi;
        bi.cbSize = sizeof(TBBUTTONINFO);

        // enumerate all buttons
        for (int i = 0; i < n; ++i) {
            id        = m_wndToolBar.GetItemID(i);
            bi.dwMask = TBIF_COMMAND;
            m_wndToolBar.GetToolBarCtrl().GetButtonInfo(id, &bi);

            // Draw color bar if there's color button
            if (bi.idCommand == ID_BGD_SELECT) {
                m_wndToolBar.GetItemRect(i, r);

                // Draw enabled button
                if (m_wndToolBar.GetToolBarCtrl().IsButtonEnabled(id)) {
                    // Offset to right-down by 1 pixel if button is pressed
                    if (m_wndToolBar.GetToolBarCtrl().IsButtonPressed(id) || m_wndToolBar.GetToolBarCtrl().IsButtonChecked(id)) {
                        if (m_wndToolBar.GetTextOptions() == toNoTextLabels)
                            r.OffsetRect(1, 0);
                        else
                            r.OffsetRect(1, 1);
                    }

                    pDC->FillSolidRect(r.left + 3, r.bottom - 8, 16, 5, GetSysColor(COLOR_3DSHADOW));
                    pDC->FillSolidRect(r.left + 4, r.bottom - 7, 14, 3, m_crBgd);
                } else {
                    // Draw disabled button
                    pDC->FillSolidRect(r.left + 3, r.bottom - 8, 16, 5, GetSysColor(COLOR_3DSHADOW));
                    pDC->FillSolidRect(r.left + 4, r.bottom - 7, 14, 3, GetSysColor(COLOR_BTNFACE));
                }
            }
        }
    }

    if (cdr->nmcd.dwDrawStage == CDDS_PREPAINT)
        *result = CDRF_NOTIFYPOSTPAINT;
    else
        *result = CDRF_DODEFAULT;
}

void
CMainFrame::OnAlwaysontop()
{
    if (m_hWndTopMost == HWND_NOTOPMOST)
        m_hWndTopMost = HWND_TOPMOST;
    else
        m_hWndTopMost = HWND_NOTOPMOST;

    ::SetWindowPos(this->m_hWnd, m_hWndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void
CMainFrame::OnUpdateAlwaysontop(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_hWndTopMost == HWND_TOPMOST);
}

LRESULT
CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        // Set app window placement: always on top or not
        case WM_WINDOWPOSCHANGED:
            ::SetWindowPos(this->m_hWnd, m_hWndTopMost, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
            break;
    }

    return CNewMDIFrameWnd::WindowProc(message, wParam, lParam);
}

CAnimationView*
CMainFrame::GetAnimationView()
{
    return (CAnimationView*)GetActiveView(); //!!!
}

void
CMainFrame::ShowDialogBar(BOOL bShow)
{
    // we should show the dialog bar only when m_bShowDialogBar flag is true
    if (bShow && !m_bShowDialogBar)
        return;

    ShowControlBar(&m_wndDialogBar, bShow, FALSE);
}

CDlgBar*
CMainFrame::GetDlgBar()
{
    return &m_wndDlgBar;
}

void
CMainFrame::OnViewDialogBar()
{
    m_bShowDialogBar = !m_bShowDialogBar;
    ShowDialogBar(m_bShowDialogBar);
    //	CFlcView *pView = (CFlcView*)GetActiveView();
    //	if (pView)
    //		pView->m_wndTitle.Invalidate();
}

void
CMainFrame::OnUpdateViewDialogBar(CCmdUI* pCmdUI)
{
    if (m_bShowDialogBar != m_wndDialogBar.IsWindowVisible())
        m_bShowDialogBar = !m_bShowDialogBar;

    pCmdUI->SetCheck(m_bShowDialogBar);
}

void
CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    if ((GetStyle() & FWS_ADDTOTITLE) == 0)
        return; // leave child window alone!

    CMDIChildWnd* pActiveChild = NULL;
    CDocument* pDocument       = GetActiveDocument();
    if (bAddToTitle && (pActiveChild = MDIGetActive()) != NULL && (pActiveChild->GetStyle() & WS_MAXIMIZE) == 0 &&
        (pDocument != NULL || (pDocument = pActiveChild->GetActiveDocument()) != NULL)) {
        CString sTitle;
        sTitle.Format("%s - %s [%s]",
                      m_strTitle,
                      pDocument->GetPathName(), // GetTitle(),
                      pDocument->IsKindOf(RUNTIME_CLASS(CFlcDoc)) ? "FLC" : "Storyboard");

        AfxSetWindowText(this->m_hWnd, sTitle);
        //		UpdateFrameTitleForDocument(pDocument->GetTitle());
    } else {
        LPCTSTR lpstrTitle = NULL;
        CString strTitle;

        if (pActiveChild != NULL) {
            strTitle = pActiveChild->GetTitle();
            if (!strTitle.IsEmpty())
                lpstrTitle = strTitle;
        }

        UpdateFrameTitleForDocument(lpstrTitle);
    }
}

void
CMainFrame::OnNewStoryboard()
{
    //	CFadeWnd wndFade(this, FALSE);

    CFlcImage img;
    img.NewStoryboard();
}

void
CMainFrame::OnUpdateFALSE(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(FALSE);
}

void
CMainFrame::OnUpdateFALSEandUNCHECK(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(FALSE);
    pCmdUI->SetCheck(0);
}

void
CMainFrame::OnOpenINI()
{
    MAIN_APP->GetDocManager()->OpenFromINI();
}

void
CMainFrame::OnOpenMulti()
{
    MAIN_APP->GetDocManager()->OpenFromFolder();
}

//////////////////////////////////////////////////////////////////////////
// This message is sent by another instance.
// It notifies that it's needed to open document.
//
BOOL
CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
    // openin document
    if (pCopyDataStruct->dwData == MSG_DOCUMENT_OPEN && pCopyDataStruct->cbData > 0) {
        // get file name
        LPCTSTR szFileName = (LPCTSTR)pCopyDataStruct->lpData;

        // openning...
        MAIN_APP->OpenDocumentFile(szFileName);

        SetForegroundWindow();

        return TRUE;
    }

    return CNewMDIFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

//////////////////////////////////////////////////////////////////////////
// Import from an existent image file to C3F Storyboard.
//
void
CMainFrame::OnImporC3f()
{
    CImportToC3F dlg;
    dlg.DoModal();
}

void
CMainFrame::OnColorReplace()
{
    //	RGBQUADPAL m_rgbPal;
    //	CColorReplaceDlg dlg(&m_rgbPal);

    CColorReplaceDlg dlg;
    dlg.DoModal();
}
