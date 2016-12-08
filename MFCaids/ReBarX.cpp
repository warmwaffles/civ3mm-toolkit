/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 ReBarX.cpp
	CREATED:	 25.4.2003

	AUTHOR:		 Dreyk
	VERSION:     1.0

	DESCRIPTION: This is the rebar class.
	Version 5.80 of the common control library added support for a cool feature 
	to the rebar control called chevrons. 
	When the length of a band is lesser than the size of it's child control,
	the rebar displays a chevron to indicate that there's more to show.
	The rebar to support chevrons have to derive from this class and have to 
	call AddBar(). 
	It is preferred to have toolbars as child windows.

	Heavily based on the CChevBar and CChevDrop classes of Rajasekar Shanmugam.

	-------------------------------------------------------------------------
	    Instructions on how to use CReBarX in your application:
	-------------------------------------------------------------------------
    
	1. Include the following strings in your MainFrm.h:
	#include "ReBarX.h"
	CReBarX  m_wndReBar;

    2. Add those strings to the CMainFrame::OnCreate(..) function:
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	m_wndReBar.RestoreState(); // load ReBar state

    3. Add this string to the CMainFrame::OnClose() function:
	m_wndReBar.SaveState();    // save ReBar state

\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "ReBarX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReBarX
LPCTSTR CReBarX::m_lpszStateInfoFormat = _T("wID=%04X,cx=%d,fStyle=%08X");

int CReBarX::m_nBarNumber = 0;

CReBarX::CReBarX()
{
    m_pDropWindow = NULL;

    ++m_nBarNumber; // Increase the Bar number

    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);
    m_strValueName.Format(_T("ReBarX #%d"), m_nBarNumber);
    m_strSubKey = _T("Settings");
}

CReBarX::~CReBarX()
{
    if (m_pDropWindow) {
        delete m_pDropWindow;
        m_pDropWindow = NULL;
    }
}

BEGIN_MESSAGE_MAP(CReBarX, CReBar)
//{{AFX_MSG_MAP(CReBarX)
//}}AFX_MSG_MAP

// Reflection message entry for Chevron push
ON_NOTIFY_REFLECT(RBN_CHEVRONPUSHED, OnChevronPushed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReBarX

/////////////////////////////////////////////////////////////////////////////
// This method is to add a band to the Rebar
// Toolbars are preferred now. The iID has to be unique for the
// *toolbar bands* that are added.
//
BOOL
CReBarX::AddBar(CWnd* pBar, LPCTSTR lpszText, CBitmap* pbmp, DWORD dwStyle)
{
    CWnd* pTBar = FromHandle(pBar->m_hWnd);
    ASSERT(pTBar);

    DWORD dwStyleNew = dwStyle;

    // Check if the window to be added is from CToolBar
    // Chevron support is only for the control with toolbar as child window
    // and proper id is provided
    if (pTBar->IsKindOf(RUNTIME_CLASS(CToolBar)))
        dwStyleNew |= RBBS_USECHEVRON;
    else
        dwStyleNew &= ~RBBS_USECHEVRON;

    // Add band
    if (!CReBar::AddBar(pBar, lpszText, pbmp, dwStyleNew))
        return FALSE;

    UpdateBandInfo(pBar, dwStyleNew);

    return TRUE;
}

BOOL
CReBarX::AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack, LPCTSTR pszText, DWORD dwStyle)
{
    CWnd* pTBar = FromHandle(pBar->m_hWnd);
    ASSERT(pTBar);

    DWORD dwStyleNew = dwStyle;

    // Check if the window to be added is from CToolBar
    // Chevron support is only for the control with toolbar as child window
    // and proper id is provided
    if (pTBar->IsKindOf(RUNTIME_CLASS(CToolBar)))
        dwStyleNew |= RBBS_USECHEVRON;
    else
        dwStyleNew &= ~RBBS_USECHEVRON;

    // Add band
    if (!CReBar::AddBar(pBar, clrFore, clrBack, pszText, dwStyleNew))
        return FALSE;

    UpdateBandInfo(pBar, dwStyleNew);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Toolbar is in child band, so set band properties for chevron to show
// properly.
//
void
CReBarX::UpdateBandInfo(CWnd* pBar, DWORD dwStyle)
{
    if (dwStyle & RBBS_USECHEVRON) {
        // This looks like toolbar
        CToolBar* pToolBar = (CToolBar*)pBar;

        CReBarCtrl& refBar = GetReBarCtrl();

        pToolBar->ModifyStyle(0, TBSTYLE_TRANSPARENT);
        pToolBar->GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_HIDECLIPPEDBUTTONS);

        // BUG:
        // CToolBarCtrl::GetMaxSize( ) doest seem to return proper size when
        // TBSTYLE_DROPDOWN style is set for buttons.. Anybody knows any way ?
        CSize sizeMax;
        pToolBar->GetToolBarCtrl().GetMaxSize(&sizeMax);

        // Set band prop
        REBARBANDINFO rbbi;
        rbbi.cbSize     = sizeof(rbbi);
        rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE | RBBIM_ID;
        rbbi.cxMinChild = 0;
        rbbi.cyMinChild = sizeMax.cy;
        rbbi.cx         = sizeMax.cx;
        rbbi.cxIdeal    = sizeMax.cx;
        rbbi.wID        = (UINT)pToolBar->GetDlgCtrlID();

        refBar.SetBandInfo(refBar.GetBandCount() - 1, &rbbi);
    }
}

/////////////////////////////////////////////////////////////////////////////
// This is the handler for chevron push.
//
void
CReBarX::OnChevronPushed(NMHDR* pNotifyStruct, LRESULT* result)
{
    NMREBARCHEVRON* pChev = (NMREBARCHEVRON*)pNotifyStruct;

    // Has the band id of the chevron that generated this message
    int iBand = pChev->uBand;

    // Have to get the child window handle this band holds
    REBARBANDINFO rbinfo;
    rbinfo.cbSize = sizeof(rbinfo);
    rbinfo.fMask  = RBBIM_CHILD;
    GetReBarCtrl().GetBandInfo(iBand, &rbinfo);

    // Check if the child window is a toolbar
    // Some rebar bands may have other windows, so that is left as an exercise
    // to the user :-)
    CWnd* pBar = FromHandle(rbinfo.hwndChild);
    ASSERT(pBar);
    ASSERT(pBar->IsKindOf(RUNTIME_CLASS(CToolBar)));

    CToolBar* pTBar = (CToolBar*)pBar;

    // Get band rectangle.
    CRect rectBand;
    GetReBarCtrl().GetRect(iBand, &rectBand);

    // We are concerned only with the size, but GetRect() returns the actual band position
    // So we have to translate its position to 0,0
    rectBand.right  = rectBand.Width();
    rectBand.left   = 0;
    rectBand.bottom = rectBand.Height();
    rectBand.top    = 0;

    // It depends on the user to decide if this subtraction is necessary, if they feel
    // happy about the chevron hiding a part of a button then remove this code
    CRect rectChevron;
    rectChevron = pChev->rc;
    rectBand.right -= rectChevron.Width();

    // Screen coordinates for Menu to be displayed
    CPoint ptMenu;
    ptMenu.x = rectChevron.left;
    ptMenu.y = rectChevron.bottom;
    ClientToScreen(&ptMenu);

    // Create the drop down popup window
    if (!m_pDropWindow) {
        m_pDropWindow = new CChevronPopup;
        if (m_pDropWindow)
            m_pDropWindow->CreatePopup(this);
    }

    // this CChevronPopup object deletes itself when it has been created/shown properly
    if (m_pDropWindow)
        m_pDropWindow->ShowPopup(GetParent(), pTBar, rectBand, ptMenu);
}

/////////////////////////////////////////////////////////////////////////////
// This function restores index, width and style from the registry for
// each band in the rebar.
//
void
CReBarX::RestoreState()
{
    CString strValue = AfxGetApp()->GetProfileString(m_strSubKey, m_strValueName);
    if (!strValue.IsEmpty()) {
        REBARBANDINFO rbbi;
        rbbi.cbSize = sizeof(rbbi);
        rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

        CReBarCtrl& rbCtrl = GetReBarCtrl();
        for (UINT nBand = 0; nBand < rbCtrl.GetBandCount(); nBand++) {
            CString strBandState;
            VERIFY(AfxExtractSubString(strBandState, strValue, nBand, _T('\n')));

            UINT nID, cx, nStyle;
            int nResult = _stscanf(strBandState, m_lpszStateInfoFormat, &nID, &cx, &nStyle);
            ASSERT(nResult == 3);

            rbCtrl.MoveBand(rbCtrl.IDToIndex(nID), nBand);
            VERIFY(rbCtrl.GetBandInfo(nBand, &rbbi));
            rbbi.cx     = cx;
            rbbi.fStyle = (rbbi.fStyle & ~(RBBS_HIDDEN | RBBS_BREAK)) | nStyle;
            VERIFY(rbCtrl.SetBandInfo(nBand, &rbbi));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// This function saves index, width and style in the registry for each
// band in the rebar, so that it could be possible to restore all these
// settings when the user runs the program next time.
//
void
CReBarX::SaveState()
{
    CString strValue;

    REBARBANDINFO rbbi;
    rbbi.cbSize = sizeof(rbbi);
    rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

    CReBarCtrl& rbCtrl = GetReBarCtrl();
    for (UINT nBand = 0; nBand < rbCtrl.GetBandCount(); nBand++) {
        VERIFY(rbCtrl.GetBandInfo(nBand, &rbbi));

        CString strBandState;
        strBandState.Format(m_lpszStateInfoFormat, rbbi.wID, rbbi.cx, rbbi.fStyle);
        strValue += (strValue.IsEmpty() ? _T("") : _T("\n")) + strBandState;
    }

    VERIFY(AfxGetApp()->WriteProfileString(m_strSubKey, m_strValueName, strValue));
}

/////////////////////////////////////////////////////////////////////////////
// CChevronPopup

CChevronPopup::CChevronPopup()
{
    m_pToolBar     = NULL;
    m_hMsgReceiver = NULL;
    m_bTextLabels  = FALSE;
}

CChevronPopup::~CChevronPopup()
{
}

BEGIN_MESSAGE_MAP(CChevronPopup, CWnd)
//{{AFX_MSG_MAP(CChevronPopup)
ON_WM_KILLFOCUS()
ON_WM_KEYDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL
CChevronPopup::CreatePopup(CWnd* pParent)
{
    if (!m_hWnd) {
        // Register for our popup window
        LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
        return CWnd::CreateEx(WS_EX_TOOLWINDOW, lpszClassName, "", WS_POPUP | WS_DLGFRAME, CRect(0, 0, 0, 0), pParent, NULL);
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Helper to create a new ToolBarX.
//
HWND
CChevronPopup::CreateToolBarX(HWND hwndParent, CToolBarX* pToolBar)
{
    m_pToolBar = pToolBar;

    // There could be only two modes, depending on text options
    // of the original toolbar: with or without text labels.
    ETextOptions eTextOptions = pToolBar->GetTextOptions();
    m_bTextLabels             = (eTextOptions == toTextLabels) || (eTextOptions == toTextOnRight);

    // Make sure common control lib is loaded
    InitCommonControls();
    HWND hwndTB = CreateWindowEx(0,
                                 TOOLBARCLASSNAME,
                                 (LPSTR)NULL,
                                 WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TBSTYLE_FLAT | TBSTYLE_WRAPABLE | TBSTYLE_LIST |
                                   (m_bTextLabels ? 0 : TBSTYLE_TOOLTIPS) | CCS_NODIVIDER | CBRS_NOALIGN,
                                 0,
                                 0,
                                 0,
                                 0,
                                 hwndParent,
                                 (HMENU)AFX_IDW_TOOLBAR + 40,
                                 AfxGetInstanceHandle(),
                                 NULL);

    ASSERT(hwndTB);
    if (!hwndTB)
        return NULL;

    // Doc says, required for backward compatibility
    ::SendMessage(hwndTB, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    // Our toolbar may have dropdown buttons, so got to set the extended style
    ::SendMessage(hwndTB, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);

    // Attach to a object.. just for the convenience
    m_tbCtrl.Attach(hwndTB);
    m_tbCtrl.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS);

    // Copy all required information from the original toolbar
    CToolBarCtrl& tbCtrl = pToolBar->GetToolBarCtrl();
    m_tbCtrl.SetImageList(tbCtrl.GetImageList());
    m_tbCtrl.SetHotImageList(tbCtrl.GetHotImageList());
    m_tbCtrl.SetDisabledImageList(tbCtrl.GetDisabledImageList());

    CRect rcItem, rcClient;
    tbCtrl.GetClientRect(rcClient);

    TBBUTTON tbinfo;
    int nMaxWidth = 0;
    int nButtons  = tbCtrl.GetButtonCount();
    int nIndex;

    for (nIndex = 0; nIndex < nButtons; nIndex++) {
        tbCtrl.GetItemRect(nIndex, rcItem);
        if (rcItem.right > rcClient.right) {
            VERIFY(tbCtrl.GetButton(nIndex, &tbinfo));
            if (!(tbinfo.fsStyle & TBSTYLE_SEP) && !(tbinfo.fsState & TBSTATE_HIDDEN)) {
                CString strButtonText;
                pToolBar->GetButtonText(tbinfo.idCommand, strButtonText);
                CString strToAdd(strButtonText, strButtonText.GetLength() + 1);
                tbinfo.iString = m_tbCtrl.AddStrings(strToAdd);
                tbinfo.fsStyle |= TBSTYLE_AUTOSIZE | (m_bTextLabels ? BTNS_SHOWTEXT : 0);
                VERIFY(m_tbCtrl.AddButtons(1, &tbinfo));
                VERIFY(m_tbCtrl.GetItemRect(m_tbCtrl.CommandToIndex(tbinfo.idCommand), rcItem));
                nMaxWidth = max(nMaxWidth, rcItem.Width());
            }
        }
    }

    nButtons = m_tbCtrl.GetButtonCount();
    if (nButtons == 0) {
        ASSERT(false); // it should never happen
        return NULL;
    }

    if (m_bTextLabels) {
        TBBUTTONINFO tbbi;
        tbbi.cbSize = sizeof(tbbi);
        tbbi.dwMask = TBIF_SIZE | TBIF_STYLE;
        for (nIndex = 0; nIndex < nButtons; nIndex++) {
            VERIFY(m_tbCtrl.GetButton(nIndex, &tbinfo));
            tbbi.cx      = (WORD)nMaxWidth;
            tbbi.fsStyle = (BYTE)(tbinfo.fsStyle & ~TBSTYLE_AUTOSIZE);
            VERIFY(m_tbCtrl.SetButtonInfo(tbinfo.idCommand, &tbbi));
        }
    }

    return hwndTB;
}

/////////////////////////////////////////////////////////////////////////////
// Helper to create a new toolbar.
//
HWND
CChevronPopup::CreateToolBar(HWND hwndParent, HWND hToolToReplicate)
{
    // Make sure common control lib is loaded
    InitCommonControls();
    HWND hwndTB =
      CreateWindowEx(0,
                     TOOLBARCLASSNAME,
                     (LPSTR)NULL,
                     WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE | //TBSTYLE_LIST |
                       CCS_NODIVIDER |
                       CBRS_NOALIGN,
                     0,
                     0,
                     0,
                     0,
                     hwndParent,
                     (HMENU)AFX_IDW_TOOLBAR + 40,
                     AfxGetInstanceHandle(),
                     NULL);

    ASSERT(hwndTB);
    if (!hwndTB)
        return NULL;

    // Doc says, required for backward compatibility
    ::SendMessage(hwndTB, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    // Our toolbar may have dropdown buttons, so got to set the extended style
    ::SendMessage(hwndTB, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);

    // Attach to a object.. just for the convenience
    m_tbCtrl.Attach(hwndTB);

    // Get normal ImageList from the toolbar
    HIMAGELIST hHot = (HIMAGELIST)::SendMessage(hToolToReplicate, TB_GETIMAGELIST, 0, 0);

    // Create a duplicate of the imagelist
    HIMAGELIST hImageList = ImageList_Duplicate(hHot);

    // Set the imagelist for our new toolbar
    ::SendMessage(hwndTB, TB_SETIMAGELIST, 0, (LPARAM)hImageList);

    // Attach it to an MFC object so that it automagically deletes the handle
    // when it goes out of scope, too lazy to delete it
    m_imgNormal.Attach(hImageList);

    // Get hot ImageList from the toolbar
    hHot = (HIMAGELIST)::SendMessage(hToolToReplicate, TB_GETHOTIMAGELIST, 0, 0);
    if (hHot) {
        hImageList = ImageList_Duplicate(hHot);
        ::SendMessage(hwndTB, TB_SETHOTIMAGELIST, 0, (LPARAM)hImageList);
        m_imgHot.Attach(hImageList);
    }

    // Get disabled ImageList from the toolbar
    hHot = (HIMAGELIST)::SendMessage(hToolToReplicate, TB_GETDISABLEDIMAGELIST, 0, 0);
    if (hHot) {
        hImageList = ImageList_Duplicate(hHot);
        ::SendMessage(hwndTB, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)hImageList);
        m_imgDisabled.Attach(hImageList);
    }

    return hwndTB;
}

BOOL
CChevronPopup::CleanToolBar()
{
    BOOL bReturn = TRUE;

    if (m_tbCtrl.GetSafeHwnd()) {
        HWND hTool = m_tbCtrl.Detach();
        bReturn    = ::DestroyWindow(hTool) || bReturn;
    }

    if (m_imgNormal.GetSafeHandle())
        bReturn = m_imgNormal.DeleteImageList() || bReturn;

    if (m_imgHot.GetSafeHandle())
        bReturn = m_imgHot.DeleteImageList() || bReturn;

    if (m_imgDisabled.GetSafeHandle())
        bReturn = m_imgDisabled.DeleteImageList() || bReturn;

    m_bTextLabels = FALSE;
    m_pToolBar    = NULL;

    return bReturn;
}

/////////////////////////////////////////////////////////////////////////////
// CChevronPopup message handlers

BOOL
CChevronPopup::ShowPopup(CWnd* pMsgReceiver, CToolBar* pToolBar, CRect rectDisplayed, CPoint ptScreen)
{
    // Somebody should be there to receive the message from toolbar
    ASSERT(pMsgReceiver != NULL);
    // The receiving handle should not be empty
    ASSERT(pMsgReceiver->GetSafeHwnd() != NULL);
    // The source toolbar should exist
    ASSERT(pToolBar != NULL);
    // It is really a toolbar ??
    ASSERT(pToolBar->IsKindOf(RUNTIME_CLASS(CToolBar)));

    // Clean if any previous objects have been attached
    CleanToolBar();

    // Create a tool bar with the popup as parent
    if (pToolBar->IsKindOf(RUNTIME_CLASS(CToolBarX))) {
        if (CreateToolBarX(GetSafeHwnd(), (CToolBarX*)pToolBar) == NULL)
            return FALSE;

        m_hMsgReceiver = pMsgReceiver->GetSafeHwnd();
    } else {
        CreateToolBar(GetSafeHwnd(), pToolBar->GetSafeHwnd());

        // Store the window which receives notifications from the toolbar
        // We have to redirect a few later
        m_hMsgReceiver = pMsgReceiver->GetSafeHwnd();

        // This flag indicates if atleast one has been added to the menu
        // POPUP Menu is shown only if atleast one item has to be shown
        BOOL bAtleastOne = FALSE;

        // Run along all the buttons, find hidden ones and add them to new toolbar
        int iCount, iButtonCount = pToolBar->GetToolBarCtrl().GetButtonCount();
        for (iCount = 0; iCount < iButtonCount; iCount++) {
            TBBUTTON tbinfo;
            pToolBar->GetToolBarCtrl().GetButton(iCount, &tbinfo);

            // If the button is a separator then we can also add a separator to the
            // popup menu
            if (tbinfo.fsStyle & TBSTYLE_SEP) {
                // It wouldnt be nice if there is a separator as the first item in the menu
                if (bAtleastOne) {
                    // add to our toolbar

                    // NOTE: Adding separators changes the way in which the toolbar
                    // shows the popup. The program uses CToolBarCtl::SetRows( ) to
                    // wrap buttons, the behaviour of this method is different when
                    // separators and grouping is there, so uncomment the following line
                    // to see how it works

                    //  				m_tbCtrl.AddButtons ( 1, &tbinfo );
                }
            } else {
                // Get the button rectangle
                CRect rectButton;
                pToolBar->GetItemRect(iCount, &rectButton);

                // Check the intersection of the button and the band
                CRect interRect;
                interRect.IntersectRect(&rectButton, &rectDisplayed);

                // if the intersection is not the same as button then
                // the button is not completely visible, so add to menu
                if (interRect != rectButton) {
                    // Yeah buttons seem to be hidden now
                    m_tbCtrl.AddButtons(1, &tbinfo);

                    // Yeah, have added one, so can show the menu
                    bAtleastOne = TRUE;
                }
            }
        }

        // Show the window only if atleast one item has been added
        if (!bAtleastOne)
            return FALSE;
    }

    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE);

    // Better call this after modifying buttons.. dunno what will go wrong if i dont
    // use this
    m_tbCtrl.AutoSize();

    CSize szBar;
    m_tbCtrl.GetMaxSize(&szBar);

    CRect recWindow;
    m_tbCtrl.SetRows(m_tbCtrl.GetButtonCount(), m_bTextLabels, &recWindow);
    if (m_tbCtrl.GetRows() == 1)
        recWindow.bottom = szBar.cy;
    /*
	if (m_bTextLabels)
		m_tbCtrl.SetRows(m_tbCtrl.GetButtonCount(), TRUE, &recWindow);
	else
	{
		// Calculate the number of approx rows required
		int iRows = max((m_tbCtrl.GetButtonCount() + BUTTONS_PER_ROW - 1) / BUTTONS_PER_ROW, 1);
		// Set rows calculated. recWindow is used in resizing the parent popup window
		m_tbCtrl.SetRows(iRows, FALSE, &recWindow);
	
		if (iRows == 1)
			recWindow.bottom = szBar.cy;
	}
*/
    // BUG: When the toolbar had one button, recWindow had 0 width, so have to
    // get width from GetMaxSize(). This may be wrong when the button
    // had TBSTYLE_DROPDOWN style...
    int iWidth = recWindow.Width() == 0 ? szBar.cx : recWindow.Width();

    // Get the top and bottom spacing for the toolbar
    DWORD dwPad = ::SendMessage(m_tbCtrl.GetSafeHwnd(), TB_GETPADDING, 0, 0);
    CRect rectWindow(ptScreen.x, ptScreen.y, ptScreen.x + iWidth + LOWORD(dwPad), ptScreen.y + recWindow.Height() + HIWORD(dwPad));

    // Have to adjust to screen pos
    int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);

    // Move little left to show the complete toolbar
    if (rectWindow.right > cxScreen) {
        int diff = rectWindow.right - cxScreen;
        rectWindow.left -= diff;
        rectWindow.right -= diff;
    }

    // Move little up to show the complete toolbar
    if (rectWindow.bottom > cyScreen) {
        int diff = rectWindow.bottom - cyScreen;
        rectWindow.top -= diff;
        rectWindow.bottom -= diff;
    }

    // Should we check for top and left positions?
    // Move the parent popup window and show
    MoveWindow(&rectWindow, TRUE);

    // Move toolbar to the top corner and show
    m_tbCtrl.MoveWindow( //hWndToolbar,
      rectWindow.left,
      rectWindow.top,
      rectWindow.Width(),
      rectWindow.Height(),
      TRUE);

    ShowWindow(SW_SHOW);
    m_tbCtrl.ShowWindow(SW_SHOW);

    // ****************	Uncomment the following to get a weird scroll effect :-)) ********

    /*
#define SPI_GETMENUANIMATION                0x1002
	// Sliding effect
	BOOL	bSlide;
	SystemParametersInfo(SPI_GETMENUANIMATION, 0, &bSlide, 0);
	if (bSlide)
	{
		CDC			memDC;
		CClientDC	wndDC(this);
		CWindowDC	screenDC(NULL);

		memDC.CreateCompatibleDC(&screenDC);

		CBitmap		Bit, *pOld;
		Bit.CreateCompatibleBitmap(&screenDC, 
									 rectWindow.Width(), 
									 rectWindow.Height());
		pOld = memDC.SelectObject(&Bit);

		GetWindowRect(&rectWindow);
		memDC.PatBlt (0,0,
					  rectWindow.Width (), 
					  rectWindow.Height(), BLACKNESS);

		SendMessage(WM_PRINT, 
					(WPARAM)memDC.GetSafeHdc(), 
					(LPARAM)PRF_CHILDREN|PRF_CLIENT|PRF_OWNED|PRF_ERASEBKGND);
		SendMessage(WM_PRINT, 
					(WPARAM)memDC.GetSafeHdc(), 
					(LPARAM)PRF_NONCLIENT);

		
		int iSteps = rectWindow.Height();
		for (; iSteps > 5; iSteps -= 5)
		{
			screenDC.BitBlt(rectWindow.left,
							rectWindow.top,
							rectWindow.Width(),
							rectWindow.Height() - iSteps,
							&memDC,
							0, iSteps, SRCCOPY);
			Sleep(3);
		}

		screenDC.BitBlt(	rectWindow.left,
							rectWindow.top,
							rectWindow.Width(),
							rectWindow.Height(),
							&memDC,
							0, 0, SRCCOPY);

		memDC.SelectObject(pOld);
	}
*/
    // All went ok !
    return TRUE;
}

void
CChevronPopup::OnKillFocus(CWnd* pNewWnd)
{
    //TRACE0("KILLFOCUS\n");
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE);
}

void
CChevronPopup::PostNcDestroy()
{
    CleanToolBar();
    CWnd::PostNcDestroy();

    //	TRACE0("NC destroy\n");

    // Commit suicide :-(
    //delete this;
}

BOOL
CChevronPopup::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Pass on the message to the Window that needs it
    BOOL bReturn = ::SendMessage(m_hMsgReceiver, WM_COMMAND, wParam, lParam);

    return bReturn;
}

BOOL
CChevronPopup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    NMHDR* pNMHDR = (NMHDR*)lParam;
    if (pNMHDR->hwndFrom == m_tbCtrl.m_hWnd && (m_pToolBar && m_pToolBar->GetSafeHwnd())) {
        // Handle certain notifications from embedded toolbar control
        switch (pNMHDR->code) {
            case TBN_DROPDOWN: {
                // Get the id of the button
                UINT iItem = (WPARAM)((LPNMTOOLBAR)lParam)->iItem;

                // Get location of button
                CRect rc;
                m_tbCtrl.GetRect(iItem, rc);
                ClientToScreen(&rc);

                // Call virtual function to display dropdown menu
                m_pToolBar->DropDownButton(iItem, rc);

                // Hide toolbar
                SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE);
                return TRUE;
            }

            case TBN_GETINFOTIP:
                if (!m_bTextLabels) {
                    NMTBGETINFOTIP* lptbgit = (NMTBGETINFOTIP*)pNMHDR;
                    CString strTip;
                    m_pToolBar->GetButtonTip(lptbgit->iItem, strTip);
                    _tcsncpy(lptbgit->pszText, strTip, lptbgit->cchTextMax);
                    *pResult = 0;

                    return TRUE;
                }
        }
    }

    // Notifications are sent by the toolbar control.
    // A typical message is TBN_DROPDOWN which should be passed to the window
    // to handle the dropdown.. which may show another menu :-)
    BOOL bReturn = ::SendMessage(m_hMsgReceiver, WM_NOTIFY, wParam, lParam);
    return bReturn;
}

void
CChevronPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // If Escape key was pressed, close popup window
    if (nChar == VK_ESCAPE)
        SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE);
    else
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
