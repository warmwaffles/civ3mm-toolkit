/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 ToolBarX.cpp
	CREATED:	 25.04.2003
	AUTHOR:		 Dreyk
	VERSION:     1.2

	DESCRIPTION: This is the ToolBar class with customization and Controls.
	The CToolbarX class supports basic customization (as in IE) with controls on it. 
	Additionally it can hide the controls when the toolbar is docked vertically. 
	This class uses the framework provided by ToolBarCtrl to do the customization 
	of the Toolbar. 
	It also supports Large Icons and Text on Buttons.

	Heavily based on the CToolBarEx class of Nikolay Denisov (nick@actor.ru).

  	-------------------------------------------------------------------------
	    Instructions on how to use CToolBarX in your application:
	-------------------------------------------------------------------------
	
	1. Add '#include "ToolBarX.h"' in the header file and change the 
	definition of the toolbars from 'CToolBar ...' to 'CToolBarX ...'.

    2. In your OnCreate override in your CMainFrame class, you should create
	the toolbar and make all necessary changes with it (add controls, drop down 
	buttons etc.).
	
	3. You can use the following functions for make additional settings:
	
	- This function loads the toolbar specified by nIDResource.
	 BOOL LoadToolBar(UINT nIDResource, COLORREF clrMask = RGB_BACKGROUND);
    
	- This function sets the bitmap image for the toolbar by the given ID.
    The bitmap should contain one image for each toolbar button.
	 BOOL LoadBitmap(UINT nIDResource);

    - This function sets the bitmap image for the toolbar by the given ID. 
    Also you can set default icon options and background (transparent) color.
	 BOOL SetBitmap(UINT nIDResource, EIconOptions eIconOptionsDef = ioSmallIcons, COLORREF clrMask = RGB_BACKGROUND);

    - This function sets each toolbar button's command ID to the value 
    specified by the corresponding element of the TBBUTTONEX array:
	 void SetButtons(int nNumButtons, TBBUTTONSETUP* lpButtons, ETextOptions eTextOptionsDef = toNone);

    - This function creates and inserts the control into the toolbar and returns the window inserted:
	 CWnd* InsertControl(CRuntimeClass* pClass, LPCTSTR lpszWindowName, CRect& rect, UINT nID, DWORD dwStyle = NULL);
	
	- This function inserts the already created control into the toolbar:
	 CWnd* InsertControl(CWnd* pCtrl, CRect& rect, UINT nID);
	
	- Call to add drop-down buttons:
	 BOOL AddDropDownButton(UINT nButtonID, UINT nMenuID, BOOL bArrow = TRUE);
	
	- This function saves the state of the toolbar in the registry:
	 void SaveState();

    - This function restores the state of the toolbar from the registry:
	 void RestoreState();

    - This function call Customize Dialog:
	 void Customize();

    - This function enable or disable customization:
     BOOL EnableCustomize(BOOL bEnable = TRUE);

    - This function sets flag - whether the Controls are visible in
	the vertical docking mode:
	 void HideCtrlsOnVertical(BOOL bHide = TRUE);

    - This function sets the default state of the toolbar. 
	The default state is set when Reset button of Customize Dialog Box is pressed:
	 BOOL MarkDefaultState();

    - This function sets text options of the toolbar:
	 void SetTextOptions(ETextOptions eTextOptions, BOOL bUpdate = TRUE);
	
	- This function sets icon options of the toolbar:
	 void SetIconOptions(EIconOptions eIconOptions, BOOL bUpdate = TRUE);
	
	- This function is used to determine which buttons has not text in "Selective Text on Right":
	 void SetNoTextOnRight(CDWordArray &arID);

    - This function is sets icon options which not avaliable for the toolbar:
    void DisableIconOptions(CByteArray &arDisabledIconOptions);

    - This function is sets text options which not avaliable for the toolbar:
    void CToolBarX::DisableTextOptions(CByteArray &arDisabledTextOptions);

\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "resource.h"
#include "ToolBarX.h"

#include "EditX.h"
#include "Utilities.h"
#include "NewMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CEditX;

// Defenitions for ShowContextMenu function
#define ID_CUSTOMIZE_BAR 0xFF
#define ID_TO_TEXTLABELS 0x100
#define ID_TO_TEXTONRIGHT 0x101
#define ID_TO_NOTEXTLABELS 0x102
#define ID_IO_SMALLICONS 0x103
#define ID_IO_LARGEICONS 0x104

/////////////////////////////////////////////////////////////////////////////
// CToolBarX

IMPLEMENT_DYNCREATE(CToolBarX, CToolBar);

int CToolBarX::m_nBarNumber                  = 0;
CCustomizeDialog* CToolBarX::m_pCustomizeDlg = NULL;
CToolBarX* CToolBarX::m_pToolBar             = NULL;
HHOOK CToolBarX::m_hCBTHook                  = NULL;

#define CX_OVERLAP 0

// For determining version of COMCTL32.DLL
#define VERSION_WIN4 MAKELONG(0, 4)
#define VERSION_IE3 MAKELONG(70, 4)
#define VERSION_IE4 MAKELONG(71, 4)
#define VERSION_IE401 MAKELONG(72, 4)
#define VERSION_IE5 MAKELONG(80, 5)
#define VERSION_IE5_2000 MAKELONG(81, 5)

struct AFX_DLLVERSIONINFO
{
    DWORD cbSize;
    DWORD dwMajorVersion; // Major version
    DWORD dwMinorVersion; // Minor version
    DWORD dwBuildNumber;  // Build number
    DWORD dwPlatformID;   // DLLVER_PLATFORM_*
};

typedef HRESULT(CALLBACK* AFX_DLLGETVERSIONPROC)(AFX_DLLVERSIONINFO*);

static int _ComCtlVersion = -1;

static DWORD AFXAPI
_GetComCtlVersion()
{
    // return cached version if already determined...
    if (_ComCtlVersion != -1)
        return _ComCtlVersion;

    // otherwise determine comctl32.dll version via DllGetVersion
    HINSTANCE hInst = ::GetModuleHandleA("COMCTL32.DLL");
    ASSERT(hInst != NULL);
    AFX_DLLGETVERSIONPROC pfn;
    pfn             = (AFX_DLLGETVERSIONPROC)GetProcAddress(hInst, "DllGetVersion");
    DWORD dwVersion = VERSION_WIN4;

    if (pfn != NULL) {
        AFX_DLLVERSIONINFO dvi;
        memset(&dvi, 0, sizeof(dvi));
        dvi.cbSize = sizeof(dvi);
        HRESULT hr = (*pfn)(&dvi);

        if (SUCCEEDED(hr)) {
            ASSERT(dvi.dwMajorVersion <= 0xFFFF);
            ASSERT(dvi.dwMinorVersion <= 0xFFFF);
            dwVersion = MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);
        }

        TRACE(_T("Using CommCtl32.dll Ver:%d.%d.%d\n"), dvi.dwMajorVersion, dvi.dwMinorVersion, dvi.dwBuildNumber);
    }

    _ComCtlVersion = dwVersion;
    return dwVersion;
}

void
DrawGradient(CDC* pDC, CRect& Rect, COLORREF StartColor, COLORREF EndColor)
{
    int Count = pDC->GetDeviceCaps(NUMCOLORS);

    BYTE StartRed   = GetRValue(StartColor);
    BYTE StartGreen = GetGValue(StartColor);
    BYTE StartBlue  = GetBValue(StartColor);

    BYTE EndRed   = GetRValue(EndColor);
    BYTE EndGreen = GetGValue(EndColor);
    BYTE EndBlue  = GetBValue(EndColor);

    int n = Rect.Width();

    // We need a brush (can be doted)
    CBrush TempBrush(EndColor);
    pDC->FillRect(Rect, &TempBrush);

    int dy = 2;
    n -= dy;

    CRect TempRect;
    BYTE ActRed, ActGreen, ActBlue;

    for (int dn = 0; dn <= n; dn += dy) {
        ActRed   = (BYTE)(MulDiv(int(EndRed) - StartRed, dn, n) + StartRed);
        ActGreen = (BYTE)(MulDiv(int(EndGreen) - StartGreen, dn, n) + StartGreen);
        ActBlue  = (BYTE)(MulDiv(int(EndBlue) - StartBlue, dn, n) + StartBlue);

        TempRect = CRect(CPoint(Rect.left + dn, Rect.top), CSize(dy, Rect.Height()));

        TempBrush.DeleteObject();
        TempBrush.CreateSolidBrush(RGB(ActRed, ActGreen, ActBlue));
        pDC->FillRect(TempRect, &TempBrush);
    }
}

/////////////////////////////////////////////////////////////////////////////
// class CToolBarX

CToolBarX::CToolBarX()
{
    m_pControls        = NULL;
    m_pDropButtons     = NULL; // list of drop-down buttons
    m_lpszResourceName = NULL;
    m_hGlobal          = NULL;

    m_bHideCtrlsOnVertical = TRUE;
    m_bIsCustomizeEnable   = TRUE;
    m_bVertAlign           = FALSE;

    m_clrBtnHilight = ::GetSysColor(COLOR_BTNHILIGHT);
    m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
    m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
    m_clrMask       = RGB_BACKGROUND;

    ++m_nBarNumber; // Increase the Bar number

    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);
    m_strValueName.Format(_T("ToolBarX #%d"), m_nBarNumber);
    m_strSubKey.Format(_T("Software\\%s\\%s\\Settings"), pApp->m_pszRegistryKey, pApp->m_pszProfileName);

    // Set default options
    m_eTextOptionsDef = toNoTextLabels;
    m_eIconOptionsDef = ioSmallIcons;
    m_eTextOptions    = toNoTextLabels;
    m_eIconOptions    = ioSmallIcons;
}

CToolBarX::~CToolBarX()
{
    while (m_pDropButtons) {
        CDropDownButtonInfo* pnext = m_pDropButtons->pNext;
        delete m_pDropButtons;
        m_pDropButtons = pnext;
    }

    if (m_pControls) {
        for (POSITION pos = m_pControls->GetHeadPosition(); pos;)
            delete m_pControls->GetNext(pos);

        delete m_pControls;
    }

    // Free memory, allocated for data stream
    // with the stored save information
    if (m_hGlobal) {
        ::GlobalFree(m_hGlobal);
        m_hGlobal = NULL;
    }

    if (m_lpszResourceName && !IS_INTRESOURCE(m_lpszResourceName))
        delete (LPTSTR)m_lpszResourceName;
}

BEGIN_MESSAGE_MAP(CToolBarX, CToolBar)
//{{AFX_MSG_MAP(CToolBarX)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_SYSCOLORCHANGE()
ON_WM_CONTEXTMENU()
ON_WM_NCPAINT()
//}}AFX_MSG_MAP
ON_NOTIFY_REFLECT_EX(TBN_DROPDOWN, OnBtnDropDown)
ON_NOTIFY_REFLECT(TBN_BEGINADJUST, OnBeginAdjust)
ON_NOTIFY_REFLECT(TBN_CUSTHELP, OnCustomHelp)
ON_NOTIFY_REFLECT(TBN_ENDADJUST, OnEndAdjust)
ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO, OnGetButtonInfo)
ON_NOTIFY_REFLECT(TBN_QUERYDELETE, OnQueryDelete)
ON_NOTIFY_REFLECT(TBN_QUERYINSERT, OnQueryInsert)
ON_NOTIFY_REFLECT(TBN_RESET, OnReset)
ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, OnChange)
ON_NOTIFY_REFLECT(TBN_INITCUSTOMIZE, OnInitCustomize)
ON_NOTIFY_REFLECT(TBN_GETINFOTIP, OnGetInfoTip)

// Saving and restoring toolbar
ON_NOTIFY_REFLECT(TBN_SAVE, OnSave)
ON_NOTIFY_REFLECT(TBN_RESTORE, OnRestore)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarX message handlers

BOOL
CToolBarX::Create(CWnd* pParentWnd, DWORD dwStyle /*= WS_CHILD | WS_VISIBLE | CBRS_TOP*/, UINT nID /*= AFX_IDW_TOOLBAR*/)
{
    dwStyle |= CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY;

    if (!CToolBar::Create(pParentWnd, dwStyle, nID))
        return FALSE;

    ModifyStyle(TBSTYLE_TOOLTIPS, 0);
    ModifyStyle(0, TBSTYLE_FLAT | CCS_ADJUSTABLE);

    return TRUE;
}

BOOL
CToolBarX::CreateEx(CWnd* pParentWnd,
                    DWORD dwCtrlStyle /*= TBSTYLE_FLAT*/,
                    DWORD dwStyle /*= WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP*/,
                    CRect rcBorders /*= CRect(0, 0, 0, 0)*/,
                    UINT nID /*= AFX_IDW_TOOLBAR*/)
{
    dwCtrlStyle &= ~TBSTYLE_TOOLTIPS;
    dwCtrlStyle |= TBSTYLE_FLAT | CCS_ADJUSTABLE;
    dwStyle |= CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY;

    if (!CToolBar::CreateEx(pParentWnd, dwCtrlStyle, dwStyle, rcBorders, nID))
        return FALSE;

    return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 1999 Kirk Stowell - Inserts a control into the toolbar at the given button id.
// This function creates and inserts the control into the Toolbar and
// returns the window inserted.
// In rect parameter, pass only the width and height.
//
CWnd*
CToolBarX::InsertControl(CRuntimeClass* pClass, LPCTSTR lpszWindowName, CRect& rect, UINT nID, DWORD dwStyle)
{
    CWnd* pCtrl = NULL;

    // CComboBox control.
    if (pClass->IsDerivedFrom(RUNTIME_CLASS(CComboBox))) {
        pCtrl = new CComboBox;

        ASSERT_VALID(pCtrl);

        if (!((CComboBox*)pCtrl)->Create(WS_CHILD | WS_VISIBLE | dwStyle, rect, this, nID)) {
            delete pCtrl;
            return NULL;
        }
    } else // CEdit control.
      if (pClass->IsDerivedFrom(RUNTIME_CLASS(CEdit))) {
        pCtrl = new CEditX;

        ASSERT_VALID(pCtrl);

        if (!((CEditX*)pCtrl)->CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | dwStyle, rect, this, nID)) {
            delete pCtrl;
            return NULL;
        }
    } else // CButton control.
      if (pClass->IsDerivedFrom(RUNTIME_CLASS(CButton))) {
        pCtrl = new CButton;

        ASSERT_VALID(pCtrl);

        if (!((CButton*)pCtrl)->Create(lpszWindowName, WS_CHILD | WS_VISIBLE | dwStyle, rect, this, nID)) {
            delete pCtrl;
            return NULL;
        }
    } else // CWnd object.
      if (pClass->IsDerivedFrom(RUNTIME_CLASS(CWnd))) {
        pCtrl = new CWnd;

        ASSERT_VALID(pCtrl);

#ifdef _UNICODE
        TCHAR szClassName[256];

        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pClass->m_lpszClassName, -1, szClassName, 255);

        if (!((CWnd*)pCtrl)->Create(szClassName, lpszWindowName, WS_CHILD | WS_VISIBLE | dwStyle, rect, this, nID)) {
            delete pCtrl;
            return NULL;
        }
#else
        if (!((CWnd*)pCtrl)->Create(pClass->m_lpszClassName, lpszWindowName, WS_CHILD | WS_VISIBLE | dwStyle, rect, this, nID)) {
            delete pCtrl;
            return NULL;
        }
#endif
    } else // An invalid object was passed in
    {
        ASSERT(FALSE);
        return NULL;
    }

    // if our object list has not been allocated, do it now...
    if (m_pControls == NULL) {
        m_pControls = new CObList();
        ASSERT(m_pControls);
    }

    // we have to remember this control, so we can delete it later
    m_pControls->AddTail(pCtrl);

    return InsertControl(pCtrl, rect, nID);
}

/////////////////////////////////////////////////////////////////////////////
// This function inserts the already created control into the Toolbar.
// In rect parameter, pass only the width and height.
//
CWnd*
CToolBarX::InsertControl(CWnd* pCtrl, CRect& rect, UINT nID)
{
    ASSERT_VALID(pCtrl);
    ASSERT(IsWindow(pCtrl->m_hWnd));

    // make sure the id is valid, and set the button
    // style for a seperator.
    int nIndex = CommandToIndex(nID);
    if (nIndex > -1) {
        ASSERT(nIndex >= 0);
        CRect rt;
        pCtrl->GetWindowRect(&rt);

        SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, rt.Width());

        CToolBarCtrl& tbCtrl = GetToolBarCtrl();

        // Insert the control into the toolbar.
        GetItemRect(nIndex, &rect);
        rect.top += max((rect.Height() - rt.Height()) / 2, 0); // move to middle
        pCtrl->SetWindowPos(0, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS);
        pCtrl->SetFont(GetFont());

        if (IsVertDocked() && m_bHideCtrlsOnVertical)
            tbCtrl.SetState(nID, (tbCtrl.GetState(nIndex) | TBSTATE_HIDDEN));
        else
            tbCtrl.SetState(nIndex, (tbCtrl.GetState(nIndex) & ~TBSTATE_HIDDEN));

        ASSERT(m_arTB.GetSize() >= nIndex);
        if (m_arTB.GetSize() >= nIndex) {
            TBBUTTONEX tbiNew;
            tbCtrl.GetButton(nIndex, &tbiNew.tbinfo);

            TBBUTTONEX tbi = m_arTB[nIndex];
            tbi.tbinfo     = tbiNew.tbinfo;
            m_arTB.SetAt(nIndex, tbi);
        }
    }

    pCtrl->ShowWindow(SW_HIDE);
    ModifyStyle(0, WS_CLIPCHILDREN);
    UpdateToolbar();

    return pCtrl;
}

/////////////////////////////////////////////////////////////////////////////
// This function a button to a Dropdown with a menu attached to it.
// Set bArrow to TRUE if you want to show arrow next to it.
//
BOOL
CToolBarX::AddDropDownButton(UINT nButtonID, UINT nMenuID, BOOL bArrow /*=TRUE*/)
{
    ASSERT_VALID(this);

    AddDropDownButtonInfo(nButtonID, nMenuID);

    int nIndex = CommandToIndex(nButtonID);

    if (nIndex == -1) {
        ASSERT(FALSE);
        return FALSE;
    }

    DWORD dwStyle = GetButtonStyle(nIndex);
    dwStyle |= TBSTYLE_DROPDOWN | ((!bArrow) ? BTNS_WHOLEDROPDOWN : 0);
    SetButtonStyle(nIndex, dwStyle);

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    tbCtrl.SetExtendedStyle(tbCtrl.GetExtendedStyle() | TBSTYLE_EX_DRAWDDARROWS);

    ASSERT(m_arTB.GetSize() >= nIndex);
    if (m_arTB.GetSize() < nIndex)
        return FALSE;

    TBBUTTONEX tbiNew;
    tbCtrl.GetButton(nIndex, &tbiNew.tbinfo);

    TBBUTTONEX tbi = m_arTB[nIndex];
    tbi.tbinfo     = tbiNew.tbinfo;
    m_arTB.SetAt(nIndex, tbi);

    return TRUE;
}

void
CToolBarX::AddDropDownButtonInfo(UINT nButtonID, UINT nMenuID)
{
    CDropDownButtonInfo* pb = FindDropDownButtonInfo(nButtonID);

    if (!pb) {
        pb = new CDropDownButtonInfo;
        ASSERT(pb);
        pb->pNext = m_pDropButtons;

        m_pDropButtons = pb;
    }

    pb->idButton = nButtonID;
    pb->idMenu   = nMenuID;
}

CToolBarX::CDropDownButtonInfo*
CToolBarX::FindDropDownButtonInfo(UINT nID)
{
    for (CDropDownButtonInfo* pb = m_pDropButtons; pb; pb = pb->pNext) {
        if (pb->idButton == nID)
            return pb;
    }

    return NULL;
}

BOOL
CToolBarX::OnBtnDropDown(NMHDR* pNMHDR, LRESULT* pRes)
{
    UNUSED_ALWAYS(pRes);

    const NMTOOLBAR& nmtb = *(NMTOOLBAR*)pNMHDR;

    // Get location of button
    CRect rc;
    GetToolBarCtrl().GetRect(nmtb.iItem, rc);
    ClientToScreen(&rc);

    // Call function to display dropdown menu
    return DropDownButton(nmtb.iItem, rc);
}

BOOL
CToolBarX::DropDownButton(UINT nID, CRect rc)
{
    CDropDownButtonInfo* pb = FindDropDownButtonInfo(nID);

    if (pb && pb->idMenu) {
#ifdef NEW_MENU_INCLUDED
        CNewMenu menu;
#else
        CMenu menu;
#endif

        if (!menu.LoadMenu(pb->idMenu))
            return FALSE;

        CMenu* pPopup = (CMenu*)menu.GetSubMenu(0);
        ASSERT(pPopup);
        if (!pPopup)
            return FALSE;

        pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, AfxGetMainWnd(), &rc);

        return TRUE;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// This function saves the state (visible buttons, toolbar position, etc.)
// of the toolbar, using the registry.
//
void
CToolBarX::SaveState()
{
    // If there is an associated registry subkey
    if (m_strSubKey.GetLength()) {
        // Save the toolbar state to the registry
        GetToolBarCtrl().SaveState(HKEY_CURRENT_USER, m_strSubKey, m_strValueName);

        // Free memory, allocated for data stream
        // with the stored save information
        if (m_hGlobal) {
            ::GlobalFree(m_hGlobal);
            m_hGlobal = NULL;
        }
    }

    UpdateToolbar();
}

/////////////////////////////////////////////////////////////////////////////
// This function restores the state (visible buttons, toolbar position, etc.)
// of the toolbar, using the registry.
//
void
CToolBarX::RestoreState()
{
    EIconOptions eIconOptions = GetIconOptions();
    ETextOptions eTextOptions = GetTextOptions();

    // if there is an associated registry subkey
    // restore the toolbar state from the registry
    if (m_strSubKey.GetLength())
        GetToolBarCtrl().RestoreState(HKEY_CURRENT_USER, m_strSubKey, m_strValueName);

    // Set Icon/Text options if changed
    if (eTextOptions != GetTextOptions())
        SetTextOptions(GetTextOptions());

    if (eIconOptions != GetIconOptions())
        SetIconOptions(GetIconOptions());
}

/////////////////////////////////////////////////////////////////////////////
// This function is called when the user begins dragging a toolbar
// button or when the customization dialog is being populated with
// toolbar information.  Basically, *pResult should be populated with
// your answer to the question, "is the user allowed to delete this
// button?".
//
void
CToolBarX::OnQueryDelete(NMHDR* pNMHDR, LRESULT* pResult)
{
    UNUSED_ALWAYS(pNMHDR);

    NMTOOLBAR* tbStruct = (TBNOTIFY*)pNMHDR;

    ASSERT(tbStruct);
    // do not allow hidden button to be deleted as they just do not go
    // to the Add listbox.
    if ((tbStruct->tbButton.idCommand) && GetToolBarCtrl().IsButtonHidden(tbStruct->tbButton.idCommand))
        *pResult = FALSE;
    else if (GetToolBarCtrl().GetButtonCount() == 1)
        *pResult = FALSE;
    else
        *pResult = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// This function is called when the user begins dragging a toolbar
// button or when the customization dialog is being populated with
// toolbar information.  Basically, *pResult should be populated with
// your answer to the question, "is the user allowed to insert a
// button to the left of this one?".
//
void
CToolBarX::OnQueryInsert(NMHDR* pNMHDR, LRESULT* pResult)
{
    UNUSED_ALWAYS(pNMHDR);
    //	if (m_dwStyle & CBRS_ORIENT_VERT)
    //		*pResult = FALSE;
    //	else
    *pResult = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// This function is called whenever the user makes a change to the
// layout of the toolbar.  Calling the mainframe's RecalcLayout forces
// the toolbar to repaint itself.
//
void
CToolBarX::OnChange(NMHDR* pNMHDR, LRESULT* pResult)
{
    UNUSED_ALWAYS(pNMHDR);
    UNUSED_ALWAYS(pResult);

    SetTextOptions(m_eTextOptions, FALSE);
    PositionControls();

    // force the frame window to recalculate the size
    CFrameWnd* pFrameWnd = GetParentFrame();
    if (pFrameWnd)
        pFrameWnd->RecalcLayout();
    else
        GetParent()->RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, CWnd::reposDefault, NULL);

    OnIdleUpdateCmdUI(TRUE, 0L);

    // if this message isn't from Customization Dialog Box
    if (!m_pCustomizeDlg)
        SaveState();
}

/////////////////////////////////////////////////////////////////////////////
// This function is called when the user initially calls up the toolbar
// customization dialog box.
//
void
CToolBarX::OnBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_pToolBar = this;
    m_hCBTHook = ::SetWindowsHookEx(WH_CBT, CBTProc, 0, ::GetCurrentThreadId());
    ASSERT(m_hCBTHook != 0);

    UNUSED_ALWAYS(pNMHDR);
    UNUSED_ALWAYS(pResult);

    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// This function is called when the user clicks on the help button on the
// toolbar customization dialog box.
//
void
CToolBarX::OnCustomHelp(NMHDR* pNMHDR, LRESULT* pResult)
{
    UNUSED_ALWAYS(pNMHDR);
    UNUSED_ALWAYS(pResult);

    TRACE(_T("Help on Customize Toolbar called.\n"));
}

/////////////////////////////////////////////////////////////////////////////
// This function is called when the user dismisses the toolbar customization
// dialog box.
//
void
CToolBarX::OnEndAdjust(NMHDR* pNMHDR, LRESULT* pResult)
{
    VERIFY(::UnhookWindowsHookEx(m_hCBTHook));
    m_hCBTHook = 0;
    m_pToolBar = 0;

    SaveState();
    UpdateToolbar();

    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// This function is called to populate the toolbar customization dialog box
// with information regarding all of the possible toolbar buttons.
//
void
CToolBarX::OnGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMTOOLBAR* lpnmtb = (NMTOOLBAR*)pNMHDR;

    int nItem = lpnmtb->iItem;
    if ((0 <= nItem) && (nItem <= m_arTB.GetUpperBound())) {
        // Copy button info from internal cache
        lpnmtb->tbButton = m_arTB[nItem].tbinfo;
        if (!(lpnmtb->tbButton.fsStyle & TBSTYLE_SEP)) {
            CString strText;
            GetButtonText(lpnmtb->tbButton.idCommand, strText);
            _tcsncpy(lpnmtb->pszText, strText, lpnmtb->cchText);
        }

        *pResult = TRUE;
    } else
        *pResult = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// This function is called when the user clicks on the reset button on the
// toolbar customization dialog box.
//
void
CToolBarX::OnReset(NMHDR* pNMHDR, LRESULT* pResult)
{
    // Delete all buttons
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    while (tbCtrl.DeleteButton(0))
        ;

    // Load buttons from internal cache
    for (int nIndex = 0; nIndex <= m_arTB.GetUpperBound(); nIndex++) {
        if (m_arTB[nIndex].bInitiallyVisible)
            VERIFY(tbCtrl.AddButtons(1, &m_arTB[nIndex].tbinfo));
    }

    // Set default text & icon options
    SetIconOptions(m_eIconOptionsDef, FALSE);
    SetTextOptions(m_eTextOptionsDef, FALSE);

    UpdateToolbar();

    // Synchronize with Customize dialog
    ASSERT(m_pCustomizeDlg);
    if (m_pCustomizeDlg) {
        m_pCustomizeDlg->SetTextOptions(m_eTextOptions, TRUE);
        m_pCustomizeDlg->SetIconOptions(m_eIconOptions, TRUE);
    }

    *pResult = 0;
}

void
CToolBarX::OnSave(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMTBSAVE* lpnmtb = (NMTBSAVE*)pNMHDR;
    if (lpnmtb->iItem == -1) {
        lpnmtb->cbData += sizeof(DWORD) * 2;

        m_hGlobal        = ::GlobalAlloc(GMEM_FIXED, lpnmtb->cbData);
        lpnmtb->pData    = (LPDWORD)m_hGlobal;
        lpnmtb->pCurrent = lpnmtb->pData;

        *lpnmtb->pCurrent++ = (DWORD)m_eTextOptions;
        *lpnmtb->pCurrent++ = (DWORD)m_eIconOptions;
    }

    *pResult = 0;
}

void
CToolBarX::OnRestore(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMTBRESTORE* lpnmtb = (NMTBRESTORE*)pNMHDR;
    if (lpnmtb->iItem == -1) {
        lpnmtb->cButtons = (lpnmtb->cbData - sizeof(DWORD) * 2) / lpnmtb->cbBytesPerRecord;
        lpnmtb->pCurrent = lpnmtb->pData;

        SetTextOptions((ETextOptions)*lpnmtb->pCurrent++, FALSE);
        SetIconOptions((EIconOptions)*lpnmtb->pCurrent++, FALSE);
    } else
        VERIFY(GetButtonInfo(lpnmtb->tbButton.idCommand, lpnmtb->tbButton));

    *pResult = 0;
}

int
CToolBarX::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CToolBar::OnCreate(lpCreateStruct) == -1)
        return -1;

    _GetComCtlVersion();
    ASSERT(_ComCtlVersion >= VERSION_IE5);

    //set version
    //	int nVer=5;
    //	DefWindowProc(CCM_SETVERSION,nVer,0);
    //	ASSERT(nVer==DefWindowProc(CCM_GETVERSION,nVer,0));
    return 0;
}

void
CToolBarX::PositionControls()
{
    GetToolBarCtrl().AutoSize();
    m_bDelayedButtonLayout = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Get the actual size of the button, not what's in sizeButton.
// Make sure to do SendMessage instead of calling MFC's GetItemRect,
// which has all sorts of bad side-effects!
//
CSize
CToolBarX::GetButtonSize(TBBUTTON* pData, int iButton, DWORD dwMode)
{
    ASSERT(_ComCtlVersion > 0);

    CRect rc;
    SendMessage(TB_GETITEMRECT, iButton, (LPARAM)&rc);
    CSize sz = rc.Size();

    DWORD dwStyle    = pData[iButton].fsStyle;
    BOOL bVertDocked = (!(dwMode & LM_HORZ) && !(dwMode & LM_STRETCH));

    // do not allow sepearot to be greater than Button height (Have to check this)
    if (dwStyle & TBSTYLE_SEP)
        sz.cy = min(sz.cy, HIWORD(GetToolBarCtrl().GetButtonSize()));

    if ((dwStyle & TBSTYLE_SEP) && (pData[iButton].idCommand != 0)) {
        if (m_bHideCtrlsOnVertical && bVertDocked)
            sz.cx = sz.cy = 0;
        else {
            // we will get 0,0 on hidden things
            if (GetToolBarCtrl().IsButtonHidden(pData[iButton].idCommand)) {
                CWnd* pWnd = GetDlgItem(pData[iButton].idCommand);

                if (pWnd->GetSafeHwnd()) {
                    ASSERT_VALID(pWnd);
                    CRect rt;
                    pWnd->GetWindowRect(rt);
                    sz = rt.Size();
                }
            }
        }
        return sz;
    }
    /*
	// Drop down arrow check
	if ((dwStyle & TBSTYLE_DROPDOWN) && 
		(bVertDocked) &&	
		!m_bShowDropArrowAlways) 
	{
		// ignore width of dropdown
		sz.cx = sz.cy;
		return sz;
	}
*/
    // Now must do special case for various versions of comctl32.dll,
    if ((pData[iButton].fsState & TBSTATE_WRAP)) {
        if (dwStyle & TBSTYLE_SEP) {
            CWnd* pWnd = GetDlgItem(pData[iButton].idCommand);

            // Check seperator is child window
            if (!pWnd) {
                // this is the last separator in the row (eg vertically docked)
                // fudge the height, and ignore the width. TB_GETITEMRECT will return
                // size = (8 x 22) even for a separator in vertical toolbar
                //
                if (_ComCtlVersion <= VERSION_IE3)
                    sz.cy -= 3; // empircally good fudge factor
                else if (_ComCtlVersion != VERSION_IE4)
                    sz.cy = sz.cx;

                sz.cx = 0; // separator takes no width if it's the last one
            } else {
                // Do not set value in case of the child window
                // We should never get here
                ASSERT_VALID(pWnd);
                ASSERT(FALSE);
            }
        }
    }

    return sz;
}

CSize
CToolBarX::CalcSize(TBBUTTON* pData, int nCount, DWORD dwMode)
{
    ASSERT(pData != NULL && nCount > 0);

    CPoint cur(0, 0);
    CSize sizeResult(0, 0);
    int cyTallestOnRow = 0;
    int nButtons       = 0;
    CSize sizeButton;

    for (int i = 0; i < nCount; i++) {
        if ((pData[i].fsState & TBSTATE_HIDDEN) && !((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0)))
            continue;

        /*		if ((pData[i].fsStyle & TBSTYLE_DROPDOWN) &&
			!(pData[i].fsStyle & BTNS_WHOLEDROPDOWN))
		{
			int x = 0;
		}
*/ // Load actual size of button into a local variable
        sizeButton = GetButtonSize(pData, i, dwMode);

        //  changed the logic below to be more correct.
        cyTallestOnRow = max(cyTallestOnRow, sizeButton.cy);
        sizeResult.cx  = max(cur.x + sizeButton.cx, sizeResult.cx);
        sizeResult.cy  = max(cur.y + sizeButton.cy, sizeResult.cy);

        cur.x += sizeButton.cx - CX_OVERLAP;

        if (!(pData[i].fsState & TBSTATE_HIDDEN))
            nButtons++;

        if (pData[i].fsState & TBSTATE_WRAP) {
            // only one seperator is present
            if ((nButtons == 1) && (pData[i].fsStyle & TBSTYLE_SEP))
                cyTallestOnRow = HIWORD(GetToolBarCtrl().GetButtonSize());

            cur.x = 0;
            cur.y += cyTallestOnRow;
            cyTallestOnRow = 0;

            if (pData[i].fsStyle & TBSTYLE_SEP)
                cur.y += sizeButton.cy;

            nButtons = 0;
        }
    }

    return sizeResult;
}

int
CToolBarX::WrapToolBar(TBBUTTON* pData, int nCount, int nWidth, DWORD dwMode)
{
    ASSERT(pData != NULL && nCount > 0);

    int nResult = 0;
    int x       = 0;
    int dx, dxNext;
    CSize sizeButton;
    BOOL bFound;

    for (int i = 0; i < nCount; i++) {
        pData[i].fsState &= ~TBSTATE_WRAP;

        // also calculate for hidden custom controls
        if ((pData[i].fsState & TBSTATE_HIDDEN) && !((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0)))
            continue;

        /*		if ((pData[i].fsStyle & TBSTYLE_DROPDOWN) &&
			!(pData[i].fsStyle & BTNS_WHOLEDROPDOWN))
		{
			int x = 0;
		}
*/ // Load actual size of button into a local variable
        sizeButton = GetButtonSize(pData, i, dwMode);

        dx     = sizeButton.cx;
        dxNext = dx - CX_OVERLAP;

        if (x + dx > nWidth) {
            bFound = FALSE;
            for (int j = i; j >= 0 && !(pData[j].fsState & TBSTATE_WRAP); j--) {
                // Find last separator that isn't hidden
                // a separator that has a command ID is not
                // a separator, but a custom control.
                if ((pData[j].fsStyle & TBSTYLE_SEP) && (pData[j].idCommand == 0) && !(pData[j].fsState & TBSTATE_HIDDEN)) {
                    bFound = TRUE;
                    i      = j;
                    x      = 0;
                    pData[j].fsState |= TBSTATE_WRAP;
                    nResult++;
                    break;
                }
            }

            if (!bFound) {
                for (int j = i - 1; j >= 0 && !(pData[j].fsState & TBSTATE_WRAP); j--) {
                    // Never wrap anything that is hidden,
                    // or any custom controls
                    if ((pData[j].fsState & TBSTATE_HIDDEN) || ((pData[j].fsStyle & TBSTYLE_SEP) && (pData[j].idCommand != 0)))
                        continue;

                    bFound = TRUE;
                    i      = j;
                    x      = 0;
                    pData[j].fsState |= TBSTATE_WRAP;
                    nResult++;
                    break;
                }

                if (!bFound)
                    x += dxNext;
            }
        } else
            x += dxNext;
    }

    return nResult + 1;
}

void
CToolBarX::SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert, DWORD dwMode)
{
    ASSERT(pData != NULL && nCount > 0);

    if (!bVert) {
        int nMin, nMax, nTarget, nCurrent, nMid;

        // Wrap ToolBar as specified
        nMax    = nLength;
        nTarget = WrapToolBar(pData, nCount, nMax, dwMode);

        // Wrap ToolBar vertically
        nMin     = 0;
        nCurrent = WrapToolBar(pData, nCount, nMin, dwMode);

        if (nCurrent != nTarget) {
            while (nMin < nMax) {
                nMid     = (nMin + nMax) / 2;
                nCurrent = WrapToolBar(pData, nCount, nMid, dwMode);

                if (nCurrent == nTarget)
                    nMax = nMid;
                else {
                    if (nMin == nMid) {
                        WrapToolBar(pData, nCount, nMax, dwMode);
                        break;
                    }

                    nMin = nMid;
                }
            }
        }

        CSize size = CalcSize(pData, nCount, dwMode);
        WrapToolBar(pData, nCount, size.cx, dwMode);
    } else {
        CSize sizeMax, sizeMin, sizeMid;

        // Wrap ToolBar vertically
        WrapToolBar(pData, nCount, 0, dwMode);
        sizeMin = CalcSize(pData, nCount, dwMode);

        // Wrap ToolBar horizontally
        WrapToolBar(pData, nCount, 32767, dwMode);
        sizeMax = CalcSize(pData, nCount, dwMode);

        while (sizeMin.cx < sizeMax.cx) {
            sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
            WrapToolBar(pData, nCount, sizeMid.cx, dwMode);
            sizeMid = CalcSize(pData, nCount, dwMode);

            if (nLength < sizeMid.cy) {
                if (sizeMin == sizeMid) {
                    WrapToolBar(pData, nCount, sizeMax.cx, dwMode);
                    return;
                }

                sizeMin = sizeMid;
            } else if (nLength > sizeMid.cy) {
                if (sizeMax == sizeMid) {
                    WrapToolBar(pData, nCount, sizeMin.cx, dwMode);
                    return;
                }

                sizeMax = sizeMid;
            } else
                return;
        }
    }
}

CSize
CToolBarX::CalcLayout(DWORD dwMode, int nLength)
{
    ASSERT_VALID(this);
    ASSERT(::IsWindow(m_hWnd));

    if (dwMode & LM_HORZDOCK)
        ASSERT(dwMode & LM_HORZ);

    int nCount;
    TBBUTTON* pData = NULL;
    CSize sizeResult(0, 0);

    nCount = DefWindowProc(TB_BUTTONCOUNT, 0, 0);
    if (nCount != 0) {
        int i;
        pData = new TBBUTTON[nCount];
        for (i = 0; i < nCount; i++)
            GetButtonEx(i, &pData[i]);
    }

    if (nCount > 0) {
        if (!(m_dwStyle & CBRS_SIZE_FIXED)) {
            BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;

            if (bDynamic && (dwMode & LM_MRUWIDTH))
                SizeToolBar(pData, nCount, m_nMRUWidth, FALSE, dwMode);
            else if (bDynamic && (dwMode & LM_HORZDOCK))
                SizeToolBar(pData, nCount, 32767, FALSE, dwMode);
            else if (bDynamic && (dwMode & LM_VERTDOCK))
                SizeToolBar(pData, nCount, 0, FALSE, dwMode);
            else if (bDynamic && (nLength != -1)) {
                CRect rect;
                rect.SetRectEmpty();
                CalcInsideRect(rect, (dwMode & LM_HORZ));
                BOOL bVert = (dwMode & LM_LENGTHY);
                int nLen   = nLength + (bVert ? rect.Height() : rect.Width());

                SizeToolBar(pData, nCount, nLen, bVert, dwMode);
            } else if (bDynamic && (m_dwStyle & CBRS_FLOATING))
                SizeToolBar(pData, nCount, m_nMRUWidth, FALSE, dwMode);
            else
                SizeToolBar(pData, nCount, (dwMode & LM_HORZ) ? 32767 : 0, FALSE, dwMode);
        }

        sizeResult = CalcSize(pData, nCount, dwMode);

        if (dwMode & LM_COMMIT) {
            int nControlCount      = 0;
            BOOL bIsDelayed        = m_bDelayedButtonLayout;
            m_bDelayedButtonLayout = FALSE;
            BOOL bVert             = (m_dwStyle & CBRS_ORIENT_VERT) != 0;

            for (int i = 0; i < nCount; i++)
                if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
                    nControlCount++;

            if (nControlCount > 0) {
                for (int i = 0; i < nCount; i++) {
                    if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0)) {
                        CRect rt;
                        CWnd* pWnd = GetDlgItem(pData[i].idCommand);
                        if (pWnd->GetSafeHwnd()) {
                            ASSERT_VALID(pWnd);
                            pWnd->GetWindowRect(rt);
                            pData[i].iBitmap = rt.Width(); // width

                            if (bVert && m_bHideCtrlsOnVertical)
                                pData[i].fsState |= TBSTATE_HIDDEN;
                            else
                                pData[i].fsState &= ~TBSTATE_HIDDEN;
                        }
                    } //if ((pData[i].fsStyle
                }     //for (int i = 0;
            }         //if (nControlCount > 0)

            if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
                m_nMRUWidth = sizeResult.cx;

            for (i = 0; i < nCount; i++)
                SetButtonEx(i, &pData[i]);

            {
                //Now place the windows
                CWnd* pWnd = GetWindow(GW_CHILD);
                while (pWnd) {
                    ASSERT_VALID(pWnd);

                    int id = pWnd->GetDlgCtrlID();

                    // make sure the id is valid, and set the button
                    // style for a seperator.
                    int nIndex = CommandToIndex(id);
                    if (nIndex > -1) {
                        ASSERT(nIndex >= 0);
                        // insert the control into the toolbar.
                        CRect rect;
                        GetItemRect(nIndex, &rect);

                        CRect rt;
                        pWnd->GetWindowRect(&rt);
                        rect.top += max((rect.Height() - rt.Height()) / 2, 0);

                        pWnd->SetWindowPos(0, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS);
                        //pWnd->SetFont(GetFont());

                        if (bVert && m_bHideCtrlsOnVertical) {
                            ASSERT(GetToolBarCtrl().IsButtonHidden(id));
                            pWnd->ShowWindow(SW_HIDE);
                        } else {
                            ASSERT(!GetToolBarCtrl().IsButtonHidden(id));
                            pWnd->ShowWindow(SW_SHOW);
                        }

                    } else
                        pWnd->ShowWindow(SW_HIDE);

                    pWnd = pWnd->GetNextWindow();
                }
            }

            m_bDelayedButtonLayout = bIsDelayed;
        }

        delete[] pData;
    }

    // Adjust Margins
    CRect rect;
    rect.SetRectEmpty();
    CalcInsideRect(rect, (dwMode & LM_HORZ));
    sizeResult.cy -= rect.Height();
    sizeResult.cx -= rect.Width();

    CSize size    = CControlBar::CalcFixedLayout((dwMode & LM_STRETCH), (dwMode & LM_HORZ));
    sizeResult.cx = max(sizeResult.cx, size.cx);
    sizeResult.cy = max(sizeResult.cy, size.cy);

    return sizeResult;
}

CSize
CToolBarX::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    DWORD dwMode = bStretch ? LM_STRETCH : 0;
    dwMode |= bHorz ? LM_HORZ : 0;

    return CalcLayout(dwMode);
}

CSize
CToolBarX::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if ((nLength == -1) && !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) && ((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK))) {
        return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
    }

    return CalcLayout(dwMode, nLength);
}

void
CToolBarX::GetButtonEx(int nIndex, TBBUTTON* pButton) const
{
    CToolBarX* pBar = (CToolBarX*)this;
    VERIFY(pBar->DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)pButton));
    // TBSTATE_ENABLED == TBBS_DISABLED so invert it
    pButton->fsState ^= TBSTATE_ENABLED;
}

void
CToolBarX::SetButtonEx(int nIndex, TBBUTTON* pButton)
{
    // get original button state
    TBBUTTON button;
    VERIFY(DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)&button));

    // prepare for old/new button comparsion
    button.bReserved[0] = 0;
    button.bReserved[1] = 0;
    // TBSTATE_ENABLED == TBBS_DISABLED so invert it
    pButton->fsState ^= TBSTATE_ENABLED;
    pButton->bReserved[0] = 0;
    pButton->bReserved[1] = 0;

    // nothing to do if they are the same
    if (memcmp(pButton, &button, sizeof(TBBUTTON)) != 0) {
        // don't redraw everything while setting the button
        DWORD dwStyle = GetStyle();
        ModifyStyle(WS_VISIBLE, 0);
        VERIFY(DefWindowProc(TB_DELETEBUTTON, nIndex, 0));

        // Force a recalc of the toolbar's layout to work around a comctl bug
        int iTextRows = (int)::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0);
        ::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
        ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows + 1, 0);
        ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows, 0);
        ::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);

        VERIFY(DefWindowProc(TB_INSERTBUTTON, nIndex, (LPARAM)pButton));
        ModifyStyle(0, dwStyle & WS_VISIBLE);

        // invalidate appropriate parts
        if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
            ((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap)) {
            // changing a separator
            Invalidate();
        } else {
            // invalidate just the button
            CRect rect;
            if (DefWindowProc(TB_GETITEMRECT, nIndex, (LPARAM)&rect))
                InvalidateRect(rect);
        }
    }
}

BOOL
CToolBarX::IsVertDocked()
{
    UINT nID = GetParent()->GetDlgCtrlID();
    return ((nID == AFX_IDW_DOCKBAR_LEFT) || (nID == AFX_IDW_DOCKBAR_RIGHT));
}

void
CToolBarX::SetTextOptions(ETextOptions eTextOptions, BOOL bUpdate)
{
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(IsTextOptionAvailable(eTextOptions));

    m_eTextOptions = eTextOptions;

    // Modify toolbar style according to new text options
    ModifyStyle((eTextOptions == toTextOnRight) ? 0 : TBSTYLE_LIST, (eTextOptions == toTextOnRight) ? TBSTYLE_LIST : 0);

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    DWORD dwStyleEx      = tbCtrl.GetExtendedStyle();
    tbCtrl.SetExtendedStyle((eTextOptions == toTextOnRight) ? (dwStyleEx | TBSTYLE_EX_MIXEDBUTTONS)
                                                            : (dwStyleEx & ~TBSTYLE_EX_MIXEDBUTTONS));
    VERIFY(tbCtrl.SetMaxTextRows((eTextOptions == toNoTextLabels) ? 0 : 1));

    // Modify all (even currently hidden ones) buttons in internal cache
    for (int nIndex = 0; nIndex <= m_arTB.GetUpperBound(); nIndex++) {
        TBBUTTON& tbinfo = m_arTB[nIndex].tbinfo;
        if (!(tbinfo.fsStyle & TBSTYLE_SEP)) {
            CString strButtonText;
            GetButtonText(tbinfo.idCommand, strButtonText);
            CString strToAdd(strButtonText, strButtonText.GetLength() + 1);
            tbinfo.iString = tbCtrl.AddStrings(strToAdd);

            switch (eTextOptions) {
                case toTextLabels:
                    tbinfo.fsStyle &= ~(TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT);
                    break;

                case toTextOnRight:
                    tbinfo.fsStyle |= (TBSTYLE_AUTOSIZE | (HasButtonText(tbinfo.idCommand) ? BTNS_SHOWTEXT : 0));
                    break;

                case toNoTextLabels:
                    tbinfo.fsStyle &= ~BTNS_SHOWTEXT;
                    tbinfo.fsStyle |= TBSTYLE_AUTOSIZE;
                    break;
            }
        }
    }

    // If requested, reflect changes immediately
    if (bUpdate)
        UpdateToolbar();
}

void
CToolBarX::SetIconOptions(EIconOptions eIconOptions, BOOL bUpdate)
{
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(IsIconOptionAvailable(eIconOptions));
    ASSERT(eIconOptions != ioNone);
    ASSERT(m_lpszResourceName != NULL);

    m_eIconOptions = eIconOptions;
    m_sizeImage    = (m_eIconOptions == ioSmallIcons) ? m_szImageSmall : m_szImageLarge;

    MakeImages();

    // Force a recalc of the toolbar's layout to work around a comctl bug
    int iTextRows = (int)::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0);
    ::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
    ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows + 1, 0);
    ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows, 0);
    ::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);

    // If requested, reflect changes immediately
    if (bUpdate)
        UpdateToolbar();
}

void
CToolBarX::OnSysColorChange()
{
    m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
    m_clrBtnHilight = ::GetSysColor(COLOR_BTNHILIGHT);
    m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);

    SetIconOptions(m_eIconOptions); // load bitmap again
}

BOOL
CToolBarX::ShowContextMenu(CPoint point)
{
    if (GetToolBarCtrl().GetButtonCount() < 1)
        return FALSE;

    CPoint pt(point);
    ClientToScreen(&pt);

// load and display popup menu
#ifdef NEW_MENU_INCLUDED
    CNewMenu menu, popupText, popupIcon;
#else
    CMenu menu, popupText, popupIcon;
#endif

    if (!menu.CreatePopupMenu())
        return FALSE;

    CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();

    if (pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))) {
        ASSERT_VALID(pFrame);
        POSITION pos = pFrame->m_listControlBars.GetHeadPosition();

        while (pos != NULL) {
            const CControlBar* pBar = (CControlBar*)pFrame->m_listControlBars.GetNext(pos);
            ASSERT(pBar != NULL);

            // Note: Possible put any other type
            if (pBar && pBar->IsKindOf(RUNTIME_CLASS(CToolBar))) {
                ASSERT_VALID(pBar);
                CString strName;
                pBar->GetWindowText(strName);
                menu.AppendMenu((pBar->GetStyle() & WS_VISIBLE ? MF_CHECKED : MF_UNCHECKED) | MF_STRING, pBar->GetDlgCtrlID(), strName);
            }
        }

        // Add a Seperator
        menu.AppendMenu(MF_SEPARATOR);
    } else
        return FALSE;

    //	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // Customize
    menu.AppendMenu(MF_STRING | ((GetStyle() & CCS_ADJUSTABLE) ? 0 : MF_GRAYED), ID_CUSTOMIZE_BAR, uLocalLoadString(IDS_CUSTOMIZE));
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        menu.ModifyODMenu(0, ID_CUSTOMIZE_BAR, IDB_CUSTOMIZE);
    }

    // Add a Seperator
    menu.AppendMenu(MF_SEPARATOR);

    if (popupText.CreatePopupMenu()) {
        // Show text labels
        popupText.AppendMenu(MF_STRING | (m_eTextOptions == toTextLabels) ? MF_CHECKED
                                                                          : 0 | IsTextOptionAvailable(toTextLabels) ? 0 : MF_GRAYED,
                             ID_TO_TEXTLABELS,
                             uLocalLoadString(IDS_TO_TEXTLABELS));

        // Selective text on right
        popupText.AppendMenu(MF_STRING | (m_eTextOptions == toTextOnRight) ? MF_CHECKED
                                                                           : 0 | IsTextOptionAvailable(toTextOnRight) ? 0 : MF_GRAYED,
                             ID_TO_TEXTONRIGHT,
                             uLocalLoadString(IDS_TO_TEXTONRIGHT));

        // No text labels
        popupText.AppendMenu(MF_STRING | (m_eTextOptions == toNoTextLabels) ? MF_CHECKED
                                                                            : 0 | IsTextOptionAvailable(toNoTextLabels) ? 0 : MF_GRAYED,
                             ID_TO_NOTEXTLABELS,
                             uLocalLoadString(IDS_TO_NOTEXTLABELS));

        menu.AppendMenu(MF_POPUP, (UINT)popupText.m_hMenu, uLocalLoadString(IDS_TEXT_OPTIONS));
        {
            AFX_MANAGE_STATE(AfxGetStaticModuleState());
            menu.ModifyODMenu(0, _T(uLocalLoadString(IDS_TEXT_OPTIONS)), IDB_TEXT_OPTIONS);
        }
    }

    if (popupIcon.CreatePopupMenu()) {
        // Small icons
        popupIcon.AppendMenu(MF_STRING | (m_eIconOptions == ioSmallIcons) ? MF_CHECKED
                                                                          : 0 | IsIconOptionAvailable(ioSmallIcons) ? 0 : MF_GRAYED,
                             ID_IO_SMALLICONS,
                             uLocalLoadString(IDS_IO_SMALLICONS));

        // Large icons
        popupIcon.AppendMenu(MF_STRING | (m_eIconOptions == ioLargeIcons) ? MF_CHECKED
                                                                          : 0 | IsIconOptionAvailable(ioLargeIcons) ? 0 : MF_GRAYED,
                             ID_IO_LARGEICONS,
                             uLocalLoadString(IDS_IO_LARGEICONS));

        menu.AppendMenu(MF_POPUP, (UINT)popupIcon.m_hMenu, uLocalLoadString(IDS_ICON_OPTIONS));
        {
            AFX_MANAGE_STATE(AfxGetStaticModuleState());
            menu.ModifyODMenu(0, _T(uLocalLoadString(IDS_ICON_OPTIONS)), IDB_ICON_OPTIONS);
        }
    }

    int nResult = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD, pt.x, pt.y, this);

    switch (nResult) {
        // Nothing clicked  on menu
        case 0:
            break;

        // Open the customization dialog
        case ID_CUSTOMIZE_BAR:
            Customize();
            break;

        case ID_TO_TEXTLABELS:
            if (m_eTextOptions != toTextLabels) {
                SetTextOptions(toTextLabels);
                SaveState();
            }
            break;

        case ID_TO_TEXTONRIGHT:
            if (m_eTextOptions != toTextOnRight) {
                SetTextOptions(toTextOnRight);
                SaveState();
            }
            break;

        case ID_TO_NOTEXTLABELS:
            if (m_eTextOptions != toNoTextLabels) {
                SetTextOptions(toNoTextLabels);
                SaveState();
            }
            break;

        case ID_IO_SMALLICONS:
            if (m_eIconOptions != ioSmallIcons) {
                SetIconOptions(ioSmallIcons);
                SaveState();
            }
            break;

        case ID_IO_LARGEICONS:
            if (m_eIconOptions != ioLargeIcons) {
                SetIconOptions(ioLargeIcons);
                SaveState();
            }
            break;

        default: {
            ASSERT_VALID(pFrame);
            if (pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
                pFrame->OnBarCheck(nResult);
        }
    }

    return TRUE;
}

void
CToolBarX::OnContextMenu(CWnd* pWnd, CPoint point)
{
    ASSERT_VALID(pWnd);
    CPoint pt(point);

    if (WindowFromPoint(pt) != this)
        return;

    ScreenToClient(&pt);

    if (!ShowContextMenu(pt))
        CToolBar::OnContextMenu(pWnd, point);
}

void
CToolBarX::Customize()
{
    if (m_bIsCustomizeEnable)
        GetToolBarCtrl().Customize();
}

void
CToolBarX::OnInitCustomize(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    *pResult = TBNRF_HIDEHELP; // help button  is hidden
}

/////////////////////////////////////////////////////////////////////////////
// This function with definitions got from MFC's barcore.cpp.
// We just add FillSolidRect to fill area around gripper to work around a bug
// with transparent gripper when using XP theme.
//
#define CX_GRIPPER 3
#define CY_GRIPPER 3
#define CX_BORDER_GRIPPER 2
#define CY_BORDER_GRIPPER 2
void
CToolBarX::DrawGripper(CDC* pDC, const CRect& rect)
{
    // only draw the gripper if not floating and gripper is specified
    if ((m_dwStyle & (CBRS_GRIPPER | CBRS_FLOATING)) == CBRS_GRIPPER) {
        // draw the gripper in the border
        if (m_dwStyle & CBRS_ORIENT_HORZ) {
            pDC->FillSolidRect(rect.left,
                               rect.top + m_cyTopBorder,
                               rect.left + CX_GRIPPER + CX_BORDER_GRIPPER + CX_BORDER_GRIPPER,
                               rect.Height(),
                               m_clrBtnFace);

            pDC->Draw3dRect(rect.left + CX_BORDER_GRIPPER,
                            rect.top + m_cyTopBorder,
                            CX_GRIPPER,
                            rect.Height() - m_cyTopBorder - m_cyBottomBorder,
                            m_clrBtnHilight,
                            m_clrBtnShadow);
        } else {
            pDC->FillSolidRect(rect.left, rect.top, rect.Width(), CY_GRIPPER + CX_BORDER_GRIPPER + CX_BORDER_GRIPPER, m_clrBtnFace);

            pDC->Draw3dRect(rect.left + m_cyTopBorder,
                            rect.top + CY_BORDER_GRIPPER,
                            rect.Width() - m_cyTopBorder - m_cyBottomBorder,
                            CY_GRIPPER,
                            m_clrBtnHilight,
                            m_clrBtnShadow);
        }
    }
}

void
CToolBarX::OnPaint()
{
    CToolBar::OnPaint();

    // I have to use this, as there is not customdraw mesasges for seperators
    // draw the controls seperator again to hide it
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    for (int i = 0; i < tbCtrl.GetButtonCount(); i++) {
        TBBUTTON tb;
        tbCtrl.GetButton(i, &tb);

        // Only for seperators controls
        if (((tb.fsStyle & TBSTYLE_SEP) == TBSTYLE_SEP) && (tb.idCommand != 0)) {
            CWnd* pWnd = GetDlgItem(tb.idCommand);
            if (pWnd->GetSafeHwnd()) {
                CClientDC dc(this);
                CRect rc;
                GetItemRect(i, rc);

                COLORREF crStart = dc.GetPixel(rc.TopLeft());
                COLORREF crEnd   = dc.GetPixel(rc.right - 1, rc.top + 1);

                if (crEnd != 0xFFFFFFFF && crStart != crEnd)
                    DrawGradient(&dc, rc, crStart, crEnd);
                else
                    dc.FillSolidRect(rc, crStart);

                // Uncomment it if you want to hides partially clipped control buttons
                // like button with TBSTYLE_EX_HIDECLIPPEDBUTTONS style
                /*				if (crEnd == 0xFFFFFFFF)
					pWnd->ShowWindow(SW_HIDE);
				else
				{
					pWnd->ShowWindow(SW_SHOW);
					pWnd->RedrawWindow();
				}
*/
                pWnd->RedrawWindow();
            }
        }
    }
}

void
CToolBarX::OnNcPaint()
{
    if (GetToolBarCtrl().GetStyle() & TBSTYLE_FLAT) {
        // get window DC that is clipped to the non-client area
        CWindowDC dc(this);
        CRect rectClient;
        GetClientRect(rectClient);
        CRect rectWindow;
        GetWindowRect(rectWindow);
        ScreenToClient(rectWindow);
        rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
        dc.ExcludeClipRect(rectClient);

        // draw borders in non-client area
        rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
        DrawBorders(&dc, rectWindow);

        // erase parts not drawn
        dc.IntersectClipRect(rectWindow);
        SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

        // draw gripper in non-client area
        if (g_Shell == WinXP)
            DrawGripper(&dc, rectWindow);
        else
            CControlBar::DrawGripper(&dc, rectWindow);
    } else
        CToolBar::OnNcPaint();
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the default state of the Toolbar.
// The default state is set when Reset button of Customize Dialog Box is pressed.
//
BOOL
CToolBarX::MarkDefaultState()
{
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();

    // Set intial Visible info
    for (int i = 0; i < m_arTB.GetSize(); i++) {
        if ((m_arTB[i].tbinfo.fsStyle & TBSTYLE_SEP) == 0)               // seperator ?
        {                                                                // Not a seperator
            if (tbCtrl.CommandToIndex(m_arTB[i].tbinfo.idCommand) == -1) // button exists on toolbar
                m_arTB[i].bInitiallyVisible = FALSE;
            else
                m_arTB[i].bInitiallyVisible = TRUE;
        } else {
            if (m_arTB[i].tbinfo.idCommand > 0) {
                if (tbCtrl.CommandToIndex(m_arTB[i].tbinfo.idCommand) == -1)
                    m_arTB[i].bInitiallyVisible = FALSE;
                else
                    m_arTB[i].bInitiallyVisible = TRUE;
            } else
                m_arTB[i].bInitiallyVisible = TRUE; // simple seperatores are visible
        }
    }
    return TRUE;
}

void
CToolBarX::UpdateParentBandInfo()
{
    CWnd* pWnd = GetParent();

    // We need the code below ONLY if toolbar's parent is ReBar
    if (!pWnd->GetSafeHwnd() || !pWnd->IsKindOf(RUNTIME_CLASS(CReBar)))
        return;

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();

    // Calculate desired height and ideal width of the bar
    CRect rcItem;
    int cyChild = 0;
    int cxIdeal = 0;
    for (int nIndex = 0, nButtons = tbCtrl.GetButtonCount(); nIndex < nButtons; nIndex++) {
        if (tbCtrl.GetItemRect(nIndex, rcItem)) {
            cxIdeal += rcItem.Width();
            cyChild = max(cyChild, rcItem.Height());
        }
    }

    CReBarCtrl& rbCtrl = STATIC_DOWNCAST(CReBar, pWnd)->GetReBarCtrl();
    ASSERT(rbCtrl);
    if (rbCtrl.GetSafeHwnd()) {
        // Modify parent band info accordingly
        REBARBANDINFO rbbi;
        rbbi.cbSize     = sizeof(rbbi);
        rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
        rbbi.cxIdeal    = cxIdeal;
        rbbi.cxMinChild = 0;
        rbbi.cyMinChild = cyChild;

        int nBand = rbCtrl.IDToIndex((UINT)GetDlgCtrlID());
        ASSERT(nBand != -1);
        if (nBand != -1) {
            rbCtrl.SetBandInfo(nBand, &rbbi);
            /*			
			REBARBANDINFO rbinfo;
			rbinfo.cbSize = sizeof(rbinfo);
			rbinfo.fMask  = RBBIM_CHILDSIZE;
			
			UINT uBand;
			UINT cyBands  = cyChild;
			BOOL bChanged = FALSE;
			for (uBand = 0; uBand < rbCtrl.GetBandCount(); uBand++)
			{
				if (rbCtrl.GetBandInfo(uBand, &rbinfo))
				{
					if (cyBands != rbinfo.cyMinChild)
						bChanged = TRUE;

					cyBands = max(cyBands, rbinfo.cyMinChild);
				}
			}

			if (bChanged)
			{
				rbinfo.cxMinChild = 0;
				rbinfo.cyMinChild = cyBands;

				for (uBand = 0; uBand < rbCtrl.GetBandCount(); uBand++)
					rbCtrl.SetBandInfo(uBand, &rbinfo);
			}
*/
        }

        Invalidate();
    }
}

BOOL
CToolBarX::GetButtonInfo(UINT nID, TBBUTTON& tbinfo)
{
    if (tbinfo.fsStyle & TBSTYLE_SEP)
        return TRUE;

    for (int nIndex = 0; nIndex <= m_arTB.GetUpperBound(); nIndex++) {
        if ((UINT)m_arTB[nIndex].tbinfo.idCommand == nID) {
            tbinfo = m_arTB[nIndex].tbinfo;
            return TRUE;
        }
    }

    return FALSE;
}

void
CToolBarX::ReloadButtons()
{
    // Reload buttons from internal cache
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    for (int nIndex = 0, nButtons = tbCtrl.GetButtonCount(); nIndex < nButtons; nIndex++) {
        TBBUTTON tbinfo;
        VERIFY(tbCtrl.GetButton(0, &tbinfo));
        VERIFY(GetButtonInfo(tbinfo.idCommand, tbinfo));
        VERIFY(tbCtrl.DeleteButton(0));
        VERIFY(tbCtrl.AddButtons(1, &tbinfo));
    }
}

LRESULT CALLBACK
CToolBarX::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    ASSERT(m_pToolBar != 0);
    HWND hWnd = (HWND)wParam;

    switch (nCode) {
        case HCBT_CREATEWND:
            if (m_pCustomizeDlg == NULL) {
                // This is where we have a good chance to subclass standard
                // "Customize toolbar" dialog in order to add to it some of
                // our new features (ie. text and icon options selectors)
                TCHAR szClassName[7];
                ::GetClassName(hWnd, szClassName, sizeof(szClassName) / sizeof(szClassName[0]));
                if (!lstrcmp(_T("#32770"), szClassName)) {
                    m_pCustomizeDlg = new CCustomizeDialog(m_pToolBar);
                    VERIFY(m_pCustomizeDlg->SubclassWindow(hWnd));
                    return 0;
                }
            }
            break;

        case HCBT_DESTROYWND:
            if ((m_pCustomizeDlg != NULL) && (m_pCustomizeDlg->m_hWnd == hWnd)) {
                m_pCustomizeDlg = NULL;
                return 0;
            }
            break;
    }

    return ::CallNextHookEx(m_hCBTHook, nCode, wParam, lParam);
}

BOOL
CToolBarX::HasButtonTip(UINT nID)
{
    switch (m_eTextOptions) {
        case toTextLabels:
            return FALSE;
        case toTextOnRight:
            return !HasButtonText(nID);
        case toNoTextLabels:
            return TRUE;
        default:
            ASSERT(FALSE);
            return FALSE;
    }
}

void
CToolBarX::GetButtonText(UINT nID, CString& strText)
{
    CString strFull;
    if (strFull.LoadString(nID))
        VERIFY(AfxExtractSubString(strText, strFull, 1, _T('\n')));
}

void
CToolBarX::GetButtonTip(UINT nID, CString& strTip)
{
    GetButtonText(nID, strTip);
}

BOOL
CToolBarX::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    NMHDR* pNMHDR = (NMHDR*)lParam;
    if ((pNMHDR->code == TTN_NEEDTEXTA) || (pNMHDR->code == TTN_NEEDTEXTW)) {
        // If button doesn't have a tip, ignore notification
        *pResult = HasButtonTip(pNMHDR->idFrom) ? Default() : 0;
        return TRUE;
    }

    return CToolBar::OnNotify(wParam, lParam, pResult);
}

void
CToolBarX::OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMTBGETINFOTIP* lptbgit = (NMTBGETINFOTIP*)pNMHDR;
    CString strTip;
    GetButtonTip(lptbgit->iItem, strTip);
    _tcsncpy(lptbgit->pszText, strTip, lptbgit->cchTextMax);

    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Full update of the toolbar, parent frame and parent rebar, if it present.
//
void
CToolBarX::UpdateToolbar()
{
    PositionControls();
    ReloadButtons();
    UpdateParentBandInfo();

    CFrameWnd* pFrameWnd = GetParentFrame();
    if (pFrameWnd)
        pFrameWnd->RecalcLayout();
    else
        GetParent()->RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, CWnd::reposDefault, NULL);

    OnIdleUpdateCmdUI(TRUE, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Check is the given text option is avaliable for the toolbar.
//
BOOL
CToolBarX::IsTextOptionAvailable(ETextOptions eTextOptions) const
{
    for (int i = 0; i < m_arDisabledTextOptions.GetSize(); i++)
        if (eTextOptions == (ETextOptions)m_arDisabledTextOptions.GetAt(i))
            return FALSE;

    return TRUE; // by default all text options avaliable
}

/////////////////////////////////////////////////////////////////////////////
// Set text options which not avaliable for the toolbar.
//
void
CToolBarX::DisableTextOptions(CByteArray& arDisabledTextOptions)
{
    m_arDisabledTextOptions.RemoveAll();
    m_arDisabledTextOptions.Copy(arDisabledTextOptions);
}

/////////////////////////////////////////////////////////////////////////////
// Check is the given icon option is avaliable for the toolbar.
//
BOOL
CToolBarX::IsIconOptionAvailable(EIconOptions eIconOptions) const
{
    for (int i = 0; i < m_arDisabledIconOptions.GetSize(); i++)
        if (eIconOptions == (ETextOptions)m_arDisabledIconOptions.GetAt(i))
            return FALSE;

    return TRUE; // by default all icon options avaliable
}

/////////////////////////////////////////////////////////////////////////////
// Set icon options which not avaliable for the toolbar.
//
void
CToolBarX::DisableIconOptions(CByteArray& arDisabledIconOptions)
{
    m_arDisabledIconOptions.RemoveAll();
    m_arDisabledIconOptions.Copy(arDisabledIconOptions);
}

/////////////////////////////////////////////////////////////////////////////
// Setup buttons which not has text when button text places on the right
// of button bitmaps (TBSTYLE_LIST flag).
//
void
CToolBarX::SetNoTextOnRight(CDWordArray& arID)
{
    m_arNoTextOnRight.RemoveAll();
    m_arNoTextOnRight.Copy(arID);

    ETextOptions eTextOptions = m_eTextOptions;
    SetTextOptions(toNoTextLabels, FALSE);
    SetTextOptions(eTextOptions);
}

/////////////////////////////////////////////////////////////////////////////
// Check is a button with given ID has text when button text places
// on the right of button bitmaps (TBSTYLE_LIST flag).
//
BOOL
CToolBarX::HasButtonText(UINT nID)
{
    for (int i = 0; i < m_arNoTextOnRight.GetSize(); i++)
        if (nID == m_arNoTextOnRight.GetAt(i))
            return FALSE;

    return TRUE; // by default all buttons have text
}

/////////////////////////////////////////////////////////////////////////////
// Call this member function to load the toolbar specified by nIDResource.
//
BOOL
CToolBarX::LoadToolBar(UINT nIDResource, COLORREF clrMask /*=RGB_BACKGROUND*/)
{
    if (!LoadToolBar(MAKEINTRESOURCE(nIDResource)))
        return FALSE;

    m_clrMask = clrMask;

    // Set initial buttons info
    m_arTB.RemoveAll();

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    int nCount           = tbCtrl.GetButtonCount();
    m_arTB.SetSize(nCount);

    for (int i = 0; i < nCount; i++) {
        TBBUTTONEX tbi;

        tbCtrl.GetButton(i, &tbi.tbinfo);
        tbi.bInitiallyVisible = TRUE;

        m_arTB.SetAt(i, tbi);
    }

    SetIconOptions(m_eIconOptions, FALSE);
    SetTextOptions(m_eTextOptions);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Call this member function to load the toolbar specified by lpszResourceName.
//
BOOL
CToolBarX::LoadToolBar(LPCTSTR lpszResourceName)
{
    ASSERT_VALID(this);
    ASSERT(lpszResourceName != NULL);

    // determine location of the bitmap in resource fork
    HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
    HRSRC hRsrc     = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
    if (hRsrc == NULL)
        return FALSE;

    HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
    if (hGlobal == NULL)
        return FALSE;

    CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
    if (pData == NULL)
        return FALSE;
    ASSERT(pData->wVersion == 1);

    UINT* pItems = new UINT[pData->wItemCount];
    for (int i    = 0; i < pData->wItemCount; i++)
        pItems[i] = pData->items()[i];
    BOOL bResult  = CToolBar::SetButtons(pItems, pData->wItemCount);
    delete[] pItems;

    if (bResult) {
        // set new sizes of the buttons
        CSize sizeImage(pData->wWidth, pData->wHeight);
        CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7);
        SetSizes(sizeButton, sizeImage);

        // load bitmap now that sizes are known by the toolbar control
        bResult = LoadBitmapEx(lpszResourceName);
    }

    UnlockResource(hGlobal);
    FreeResource(hGlobal);

    return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// Call this member function to load the bitmap specified by lpszResourceName.
// The bitmap should contain one image for each toolbar button.
//
BOOL
CToolBarX::LoadBitmap(LPCTSTR lpszResourceName)
{
    if (CToolBar::LoadBitmap(lpszResourceName)) {
        m_lpszResourceName = lpszResourceName;
        SetIconOptions(GetIconOptions());
        return TRUE;
    }

    m_lpszResourceName = NULL;
    return FALSE;
}

BOOL
CToolBarX::LoadBitmapEx(LPCTSTR lpszResourceName)
{
    if (CToolBar::LoadBitmap(lpszResourceName)) {
        m_lpszResourceName = lpszResourceName;
        return TRUE;
    }

    m_lpszResourceName = NULL;
    return FALSE;
}

BOOL
CToolBarX::MakeImages()
{
    if (!m_lpszResourceName)
        return FALSE;

    UINT nCount = 0;
    for (int i = 0; i < m_arTB.GetSize(); i++) {
        if ((m_arTB[i].tbinfo.idCommand != 0))
            nCount++;
    }

    CSize sizeImage(nCount * m_sizeImage.cx, m_sizeImage.cy);

    // determine location of the bitmap in resource fork
    HINSTANCE hInstImageWell = NULL;
    HRSRC hRsrcImageWell     = NULL;

    hInstImageWell = AfxFindResourceHandle(m_lpszResourceName, RT_BITMAP);
    ASSERT(hInstImageWell);
    if (!hInstImageWell)
        return FALSE;

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    HBITMAP hBitmap      = NULL;
    CBitmap bmp;

    // Prepare Normal image list
    hRsrcImageWell = ::FindResource(hInstImageWell, m_lpszResourceName, RT_BITMAP);
    ASSERT(hRsrcImageWell);
    if (!hRsrcImageWell)
        return FALSE;

    hBitmap = LoadSysColorBitmapSize(hInstImageWell, hRsrcImageWell, sizeImage);
    if (!bmp.Attach(hBitmap))
        return FALSE;

    CImageList img;
    img.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);
    if (img.Add(&bmp, m_clrMask) == -1)
        return FALSE;

    /////////////////////////////////////////////
    int nImgCount = img.GetImageCount();
    /*
	m_imgNormal.DeleteImageList();
	m_imgNormal.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);
    if (m_imgNormal.Add(&bmp, m_clrMask) == -1)
		return FALSE;

    tbCtrl.SetImageList(&m_imgNormal);

	/////////////////////////////////////////////
	int nImgCount = m_imgNormal.GetImageCount();
*/ if (nImgCount < 1)
        return FALSE;

    m_imgNormal.DeleteImageList();
    m_imgNormal.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);
    m_imgHot.DeleteImageList();
    m_imgHot.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);
    m_imgDisabled.DeleteImageList();
    m_imgDisabled.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);

    HICON hIcon;
    for (int nIndex = 0; nIndex < nImgCount; nIndex++) {
        hIcon = img.ExtractIcon(nIndex);

        m_imgHot.Add(hIcon);
        AddGloomIcon(hIcon);
        AddGrayIcon(hIcon);

        DestroyIcon(hIcon);
    }

    tbCtrl.SetImageList(&m_imgNormal);
    tbCtrl.SetHotImageList(&m_imgHot);
    tbCtrl.SetDisabledImageList(&m_imgDisabled);

    /*
    // clock - start
	clock_t start, finish;
	double  duration;
	start = clock();
*/
    /*	m_imgHot.DeleteImageList();
	m_imgHot.Attach(img.Detach());
	tbCtrl.SetHotImageList(&m_imgHot);

    hBitmap = LoadSysColorBitmapSize(hInstImageWell, hRsrcImageWell, sizeImage);
	MakeNormalImgList(hBitmap, sizeImage);

	hBitmap = LoadSysColorBitmapSize(hInstImageWell, hRsrcImageWell, sizeImage);
	MakeDisabledImgList(hBitmap, sizeImage);
*/
    /*
    // clock - finish	
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	CString strTimer;
	strTimer.Format("\nTIMER VALUE: %2.3f seconds\n", duration);
	TRACE(strTimer);
*/
    return TRUE;
}

int
CToolBarX::AddGloomIcon(HICON hIcon, int nIndex /*=-1*/)
{
    ICONINFO iconInfo;
    ZeroMemory(&iconInfo, sizeof(iconInfo));
    if (!GetIconInfo(hIcon, &iconInfo))
        return -1;

    CDC myDC;
    myDC.CreateCompatibleDC(0);

    CBitmap bmColor;
    bmColor.Attach(iconInfo.hbmColor);
    CBitmap bmMask;
    bmMask.Attach(iconInfo.hbmMask);

    CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
    COLORREF crPixel;
    int j;
    for (int i = 0; i < m_sizeImage.cx; ++i) {
        for (j = 0; j < m_sizeImage.cy; ++j) {
            crPixel = myDC.GetPixel(i, j);
            myDC.SetPixel(i, j, DarkenColor(SCALE_DARK, crPixel));
        }
    }

    myDC.SelectObject(pOldBitmap);

    if (nIndex == -1)
        return m_imgNormal.Add(&bmColor, &bmMask);

    return (m_imgNormal.Replace(nIndex, &bmColor, &bmMask)) ? nIndex : -1;
}

int
CToolBarX::AddGrayIcon(HICON hIcon, int nIndex /*=-1*/)
{
    ICONINFO iconInfo;
    ZeroMemory(&iconInfo, sizeof(iconInfo));
    if (!GetIconInfo(hIcon, &iconInfo))
        return -1;

    CDC myDC;
    myDC.CreateCompatibleDC(0);

    CBitmap bmColor;
    bmColor.Attach(iconInfo.hbmColor);
    CBitmap bmMask;
    bmMask.Attach(iconInfo.hbmMask);

    CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
    COLORREF crPixel;
    for (int i = 0; i < m_sizeImage.cx; ++i) {
        for (int j = 0; j < m_sizeImage.cy; ++j) {
            crPixel = myDC.GetPixel(i, j);
            myDC.SetPixel(i, j, MixedColor(LightenColor(SCALE_LIGHT, GrayColor(crPixel)), m_clrBtnFace));
        }
    }

    myDC.SelectObject(pOldBitmap);

    if (nIndex == -1)
        return m_imgDisabled.Add(&bmColor, &bmMask);

    return (m_imgDisabled.Replace(nIndex, &bmColor, &bmMask)) ? nIndex : -1;
}

BOOL
CToolBarX::MakeDisabledImgList(HBITMAP hbmp, CSize sizeImage)
{
    CDC myDC;
    myDC.CreateCompatibleDC(0);

    CBitmap bmColor;
    bmColor.Attach(hbmp);

    CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
    COLORREF crPixel;
    for (int i = 0; i < sizeImage.cx; ++i) {
        for (int j = 0; j < sizeImage.cy; ++j) {
            crPixel = myDC.GetPixel(i, j);
            if (crPixel != m_clrMask)
                myDC.SetPixel(i, j, MixedColor(LightenColor(SCALE_LIGHT, GrayColor(crPixel)), m_clrBtnFace));
        }
    }

    myDC.SelectObject(pOldBitmap);

    m_imgDisabled.DeleteImageList();
    m_imgDisabled.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);

    if (m_imgDisabled.Add(&bmColor, m_clrMask) == -1)
        return FALSE;

    return TRUE;
}

BOOL
CToolBarX::MakeNormalImgList(HBITMAP hbmp, CSize sizeImage)
{
    CDC myDC;
    myDC.CreateCompatibleDC(0);

    CBitmap bmColor;
    bmColor.Attach(hbmp);

    CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
    COLORREF crPixel;
    for (int i = 0; i < sizeImage.cx; ++i) {
        for (int j = 0; j < sizeImage.cy; ++j) {
            crPixel = myDC.GetPixel(i, j);
            if (crPixel != m_clrMask)
                myDC.SetPixel(i, j, DarkenColor(50, crPixel));
        }
    }

    myDC.SelectObject(pOldBitmap);

    m_imgNormal.DeleteImageList();
    m_imgNormal.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);

    if (m_imgNormal.Add(&bmColor, m_clrMask) == -1)
        return FALSE;

    GetToolBarCtrl().SetHotImageList(&m_imgNormal);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// This member function sets each toolbar button's command ID to the value
// specified by the corresponding element of the TBBUTTONEX array.
// If an element of the array has the value ID_SEPARATOR, a separator is created
// in the corresponding position of the toolbar.
// This function also sets each button's style and assigns an image index to
// each button. The image index specifies the position of the button’s image
// within the bitmap.
//
void
CToolBarX::SetButtons(int nNumButtons, TBBUTTONSETUP* lpButtons, ETextOptions eTextOptionsDef /*=toNoTextLabels*/)
{
    // Delete old buttons
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    while (tbCtrl.DeleteButton(0))
        ;

    m_arTB.RemoveAll();

    // Load buttons from provided array
    for (int nIndex = 0; nIndex < nNumButtons; nIndex++) {
        if (lpButtons[nIndex].bInitiallyVisible)
            VERIFY(tbCtrl.AddButtons(1, &lpButtons[nIndex].tbinfo));

        // Add drop down button
        if (lpButtons[nIndex].tbinfo.fsStyle & TBSTYLE_DROPDOWN) {
            AddDropDownButtonInfo(lpButtons[nIndex].tbinfo.idCommand, lpButtons[nIndex].idMenu);
            tbCtrl.SetExtendedStyle(tbCtrl.GetExtendedStyle() | TBSTYLE_EX_DRAWDDARROWS);
        }

        TBBUTTONEX tbi;
        tbi.tbinfo            = lpButtons[nIndex].tbinfo;
        tbi.bInitiallyVisible = lpButtons[nIndex].bInitiallyVisible;
        m_arTB.Add(tbi);
    }

    m_eTextOptionsDef = eTextOptionsDef; // to be used on reset
    SetTextOptions(m_eTextOptionsDef);   // apply new options
}

/////////////////////////////////////////////////////////////////////////////
// Call this member function to set the bitmap image for the toolbar
// by the given ID.
// Also you can set default icon options and background (transparent) color.
//
BOOL
CToolBarX::SetBitmap(UINT nIDResource, EIconOptions eIconOptionsDef /*=ioSmallIcons*/, COLORREF clrMask /*=RGB_BACKGROUND*/)
{
    if (!LoadBitmapEx(MAKEINTRESOURCE(nIDResource)))
        return FALSE;

    m_clrMask = clrMask;

    m_eIconOptionsDef = eIconOptionsDef; // to be used on reset
    SetIconOptions(m_eIconOptionsDef);   // apply new options
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Call this member function to set the bitmap image for the toolbar.
// NOTE: User should never call this function.
//
BOOL
CToolBarX::SetBitmap(HBITMAP hbmImageWell)
{
    return CToolBar::SetBitmap(hbmImageWell);
}

/////////////////////////////////////////////////////////////////////////////
// Hide or show controls at vertical docking.
//
void
CToolBarX::HideCtrlsOnVertical(BOOL bHide)
{
    m_bHideCtrlsOnVertical = bHide;
}

void
CToolBarX::OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
{
    CToolBar::OnBarStyleChange(dwOldStyle, dwNewStyle);

    // Vertical orientation of the toolbar
    if (m_dwStyle & CBRS_ORIENT_VERT) {
        // We should disable customization
        if (m_bIsCustomizeEnable)
            ModifyStyle(CCS_ADJUSTABLE, 0);

        if (!m_bVertAlign) {
            // Remove all separators
            CToolBarCtrl& tbCtrl = GetToolBarCtrl();
            int nButtons         = tbCtrl.GetButtonCount();
            m_arSep.RemoveAll();
            for (int i = 0; i < nButtons; i++) {
                TBBUTTON tbinfo;
                VERIFY(tbCtrl.GetButton(0, &tbinfo));
                VERIFY(GetButtonInfo(tbinfo.idCommand, tbinfo));
                VERIFY(tbCtrl.DeleteButton(0));
                if (!(tbinfo.fsStyle & TBSTYLE_SEP)) // don't add back separators
                    VERIFY(tbCtrl.AddButtons(1, &tbinfo));
                else // in case separator is control and we should show it
                  if (tbinfo.idCommand != 0 && !m_bHideCtrlsOnVertical)
                    VERIFY(tbCtrl.AddButtons(1, &tbinfo));
                else {
                    TBBUTTONSEP tbsep;
                    tbsep.tbinfo = tbinfo;
                    tbsep.nIndex = i;
                    m_arSep.Add(tbsep);
                }
            }
        }

        m_bVertAlign = TRUE;
    } else // Horizontal orientation of the toolbar
    {
        // We can enable customization
        if (m_bIsCustomizeEnable)
            ModifyStyle(0, CCS_ADJUSTABLE);

        // In case previous toolbar orientation was vertical
        if (m_bVertAlign) {
            // Restore all separators
            CToolBarCtrl& tbCtrl = GetToolBarCtrl();
            for (int i = 0; i < m_arSep.GetSize(); i++)
                tbCtrl.InsertButton(m_arSep[i].nIndex, &m_arSep[i].tbinfo);

            m_arSep.RemoveAll();
        }

        m_bVertAlign = FALSE;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Enable or disable toolbar customization.
// Note: Customization cannot be enabled in case toolbar is vertical docked.
//
BOOL
CToolBarX::EnableCustomize(BOOL bEnable)
{
    // In case vertical align - return false
    if (m_dwStyle & CBRS_ORIENT_VERT)
        return FALSE;

    BOOL bRes;

    if (bEnable)
        bRes = ModifyStyle(0, CCS_ADJUSTABLE);
    else
        bRes = ModifyStyle(CCS_ADJUSTABLE, 0);

    if (!bRes)
        return FALSE;

    m_bIsCustomizeEnable = bEnable;
    return TRUE;
}

///////////////////////////////////
/* Not solved problems:
Если кнопка имеет стиль TBSTYLE_DROPDOWN, то при исползовании XP-тем высота тулбара становится больше, чем необходимо.
Можно удалить все кнопки, но после этого возникает проблема с вызовом кустомизации.
Некорректно работает в 95м
При перезагрузке тулбара - проблема с контролами.
*/
