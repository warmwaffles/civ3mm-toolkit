//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//
// FILE NAME:        HyperLink.cpp
// AUTHOR:           Copyright Chris Maunder, 1997
//                   Feel free to use and distribute.
// DESCRIPTION:	     HyperLink static control.
//                   If m_bIsURL == TRUE will open the default browser with
//                   the given URL when the user clicks on the link.
//                   Else will open the default mail client with the given
//                   e-mail address when the user clicks on the link.
// REVISION HISTORY:
// Thanks to Pål K. Tønder for auto-size and window caption changes.
// "GotoURL" function by Stuart Patterson
// As seen in the August, 1997 Windows Developer's Journal.
// Copyright 1997 by Miller Freeman, Inc.
// All rights reserved.
// Modified by Chris Maunder to use TCHARs instead of chars.
// Modified by Dreyk to use MailTo function.
//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include "stdafx.h"
#include "HyperLink.h"
#include "Resource.h"

#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLTIP_ID 1

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink()
{
    m_crLinkColour    = RGB(0, 0, 238);   // Blue
    m_crVisitedColour = RGB(85, 26, 139); // Purple
    m_bVisited        = FALSE;            // Hasn't been visited yet
    m_bUnderline      = TRUE;             // Underline the link?
    m_bAdjustToFit    = TRUE;             // Resize the window to fit the text?
    m_bIsURL          = FALSE;            // Is it URL or mail address
    m_sURL.Empty();
}

CHyperLink::~CHyperLink()
{
    if ((HFONT)m_Font)
        m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
//{{AFX_MSG_MAP(CHyperLink)
ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
ON_WM_CTLCOLOR_REFLECT()
ON_WM_SETCURSOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLink message handlers

BOOL
CHyperLink::PreTranslateMessage(MSG* pMsg)
{
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

HBRUSH
CHyperLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    // Set the approriate colour
    if (m_bVisited)
        pDC->SetTextColor(m_crVisitedColour);
    else
        pDC->SetTextColor(m_crLinkColour);

    // Set underline font if required
    if (m_bUnderline) {
        if (!(HFONT)m_Font) { // Create font only once.
            LOGFONT lf;
            GetFont()->GetLogFont(&lf);
            lf.lfUnderline = TRUE;
            m_Font.CreateFontIndirect(&lf);
        }

        pDC->SelectObject(&m_Font);
    }

    // transparent text.
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL
CHyperLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
    ::SetCursor(m_hLinkCursor);
    return TRUE;
}

void
CHyperLink::PreSubclassWindow()
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);

    // Set the URL as the window text
    if (m_sURL.IsEmpty())
        GetWindowText(m_sURL);

    // Check that the window text isn't empty. If it is, set it as the URL.
    CString strWndText;
    GetWindowText(strWndText);
    if (strWndText.IsEmpty()) {
        ASSERT(!m_sURL.IsEmpty()); // Window and URL both empty. DUH!
        SetWindowText(m_sURL);
    }

    // Adjust size of window to fit URL if necessary
    PositionWindow();

    // Load up the (standard) hyperlink cursor
    m_hLinkCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
    if (!m_hLinkCursor)
        m_hLinkCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

    // Create the tooltip
    CRect rect;
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this, m_sURL, rect, TOOLTIP_ID);

    CStatic::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink operations

void
CHyperLink::SetURL(CString sURL)
{
    m_sURL = sURL;
    PositionWindow();
    m_ToolTip.UpdateTipText(sURL, this, TOOLTIP_ID);
}

void
CHyperLink::SetColours(COLORREF crLinkColour, COLORREF crVisitedColour)
{
    m_crLinkColour    = crLinkColour;
    m_crVisitedColour = crVisitedColour;
    Invalidate();
}

void
CHyperLink::SetVisited(BOOL bVisited /* = TRUE */)
{
    m_bVisited = bVisited;
    Invalidate();
}

void
CHyperLink::SetUnderline(BOOL bUnderline /* = TRUE */)
{
    m_bUnderline = bUnderline;
    Invalidate();
}

void
CHyperLink::SetAutoSize(BOOL bAutoSize /* = TRUE */)
{
    m_bAdjustToFit = bAutoSize;
    PositionWindow();
}

// Move and resize the window so that the window is the same size
// as the hyperlink text. This stops the hyperlink cursor being active
// when it is not directly over the text. If the text is left justified
// then the window is merely shrunk, but if it is centred or right
// justified then the window will have to be moved as well.
//
// Suggested by Pål K. Tønder

void
CHyperLink::PositionWindow()
{
    if (!m_bAdjustToFit)
        return;

    // Get the current window position
    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

    // Get the size of the window text
    CString strWndText;
    GetWindowText(strWndText);

    CDC* pDC     = GetDC();
    CSize Extent = pDC->GetTextExtent(strWndText);
    ReleaseDC(pDC);

    // Get the text justification via the window style
    DWORD dwStyle = GetStyle();

    // Recalc the window size and position based on the text justification
    if (dwStyle & SS_CENTERIMAGE)
        rect.DeflateRect(0, (rect.Height() - Extent.cy) / 2);
    else
        rect.bottom = rect.top + Extent.cy;

    if (dwStyle & SS_CENTER)
        rect.DeflateRect((rect.Width() - Extent.cx) / 2, 0);
    else if (dwStyle & SS_RIGHT)
        rect.left = rect.right - Extent.cx;
    else // SS_LEFT = 0, so we can't test for it explicitly
        rect.right = rect.left + Extent.cx;

    // Move the window
    SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink implementation

void
CHyperLink::OnClicked()
{
    int result = HINSTANCE_ERROR;

    // If it is URL
    if (m_bIsURL)
        result = (int)GotoURL(m_sURL, SW_SHOW); // go to the URL
    else
        result = (int)SendMailTo(); // send mail to...

    m_bVisited = (result > HINSTANCE_ERROR);
    if (!m_bVisited) {
        MessageBeep(MB_ICONEXCLAMATION); // Unable to follow link
        ReportError(result);
    } else
        SetVisited(); // Repaint to show visited colour
}

LONG
CHyperLink::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata, data);
        RegCloseKey(hkey);
    }

    return retval;
}

void
CHyperLink::ReportError(int nError)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    UINT id = 0;
    switch (nError) {
        case 0:
            id = IDS_ERR_OS_OUT_OF_RES;
            break;
        case SE_ERR_PNF:
            id = IDS_ERR_PATH_NOT_FOUND;
            break;
        case SE_ERR_FNF:
            id = IDS_ERR_FILE_NOT_FOUND;
            break;
        case ERROR_BAD_FORMAT:
            id = IDS_ERR_INVALID_EXE;
            break;
        case SE_ERR_ACCESSDENIED:
            id = IDS_ERR_CAN_NOT_ACCESS_FILE;
            break;
        case SE_ERR_ASSOCINCOMPLETE:
            id = IDS_ERR_INVALID_FILE_ASSOC;
            break;
        case SE_ERR_DDEBUSY:
            id = IDS_ERR_DDE_TRAN_CANT_COMPLETED;
            break;
        case SE_ERR_DDEFAIL:
            id = IDS_ERR_DDE_TRAN_FAILED;
            break;
        case SE_ERR_DDETIMEOUT:
            id = IDS_ERR_DDE_TRAN_TIMEOUT;
            break;
        case SE_ERR_DLLNOTFOUND:
            id = IDS_ERR_DLL_NOT_FOUND;
            break;
        case SE_ERR_NOASSOC:
            id = IDS_ERR_NO_APP_ASSOC_WITH_EXT;
            break;
        case SE_ERR_OOM:
            id = IDS_ERR_NOT_ENOUGHT_MEMORY;
            break;
        case SE_ERR_SHARE:
            id = IDS_ERR_SHARE_VIOLATION;
            break;
    }
    CString msg;
    if (id)
        msg = uLoadString(id);
    else
        msg = uFormatString(IDS_ERR_UNKNOWN, nError);

    CString title;
    title = uLoadString(IDS_ERR_OPEN_LINK);
    msg   = title + msg;
    AfxMessageBox(msg, MB_ICONEXCLAMATION | MB_OK);
}

HINSTANCE
CHyperLink::GotoURL(LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL, NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) {

        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
            lstrcat(key, _T("\\shell\\open\\command"));

            if (GetRegKey(HKEY_CLASSES_ROOT, key, key) == ERROR_SUCCESS) {
                TCHAR* pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {               // No quotes found
                    pos = strstr(key, _T("%1")); // Check for %1, without quotes
                    if (pos == NULL)             // No parameter at all...
                        pos = key + lstrlen(key) - 1;
                    else
                        *pos = '\0'; // Remove the parameter
                } else
                    *pos = '\0'; // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);
                result = (HINSTANCE)WinExec(key, showcmd);
            }
        }
    }

    return result;
}

HINSTANCE
CHyperLink::SendMailTo()
{
    // HKEY_CLASSES_ROOT\mailto\shell\open\command
    // "C:\Program Files\Microsoft Office\Office\OUTLOOK.EXE" -c IPM.Note /m "%1"

    TCHAR key[MAX_PATH + MAX_PATH];
    CString strEmail = m_sURL;

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), _T(strEmail), NULL, NULL, SW_SHOW);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) {
        CString strIpCmdLine;

        if (GetRegKey(HKEY_CLASSES_ROOT, _T("mailto\\shell\\open\\command"), key) == ERROR_SUCCESS) {
            CString strRegResult = key;
            int Len              = strRegResult.Find("%1");
            if (Len != -1) {
                strIpCmdLine = strRegResult.Left(Len);
                strIpCmdLine += _T(strEmail);
                strIpCmdLine += strRegResult.Mid(Len + 2);
            }

            result = (HINSTANCE)WinExec(_T(strIpCmdLine), SW_SHOW);
        }
    }

    return result;
}

bool
CHyperLink::GotoURL(LPCTSTR szURL)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    const int result    = (int)GotoURL(szURL, SW_SHOW);
    const bool bVisited = (result > HINSTANCE_ERROR);
    if (!bVisited) {
        // Unable to follow link
        MessageBeep(MB_ICONEXCLAMATION);
        ReportError(result);
    }
    return bVisited;
}
