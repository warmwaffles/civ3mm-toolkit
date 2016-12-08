// Label.cpp : implementation file
//

#include "stdafx.h"
#include "Label.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLASH_TIMER 1
#define FLASH_TIMER_DELAY 500

/////////////////////////////////////////////////////////////////////////////
// CLabel

CLabel::CLabel()
{
    m_crText = GetSysColor(COLOR_WINDOWTEXT);
    m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

    ::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);

    m_font.CreateFontIndirect(&m_lf);
    m_bTimer  = FALSE;
    m_bState  = FALSE;
    m_bLink   = TRUE;
    m_hCursor = NULL;
    m_Type    = None;

    m_hwndBrush = ::GetSysColorBrush(COLOR_3DFACE);
    //	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CLabel::~CLabel()
{
    m_font.DeleteObject();
    ::DeleteObject(m_hBrush);
}

CLabel&
CLabel::SetText(const CString& sText)
{
    SetWindowText(sText);
    return *this;
}

CLabel&
CLabel::SetTextColor(COLORREF crText)
{
    m_crText = crText;
    RedrawWindow();
    return *this;
}

CLabel&
CLabel::SetFontBold(BOOL bBold)
{
    m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
    ReconstructFont();
    RedrawWindow();
    return *this;
}

CLabel&
CLabel::SetFontUnderline(BOOL bSet)
{
    m_lf.lfUnderline = bSet;
    ReconstructFont();
    RedrawWindow();
    return *this;
}

CLabel&
CLabel::SetFontItalic(BOOL bSet)
{
    m_lf.lfItalic = bSet;
    ReconstructFont();
    RedrawWindow();
    return *this;
}

CLabel&
CLabel::SetSunken(BOOL bSet)
{
    if (!bSet)
        ModifyStyleEx(WS_EX_STATICEDGE, 0, SWP_DRAWFRAME);
    else
        ModifyStyleEx(0, WS_EX_STATICEDGE, SWP_DRAWFRAME);

    return *this;
}

CLabel&
CLabel::SetBorder(BOOL bSet)
{
    if (!bSet)
        ModifyStyle(WS_BORDER, 0, SWP_DRAWFRAME);
    else
        ModifyStyle(0, WS_BORDER, SWP_DRAWFRAME);

    return *this;
}

CLabel&
CLabel::SetFontSize(int nSize)
{
    nSize *= -1;
    m_lf.lfHeight = nSize;
    ReconstructFont();
    RedrawWindow();
    return *this;
}

CLabel&
CLabel::SetBkColor(COLORREF crBkgnd)
{
    if (m_hBrush)
        ::DeleteObject(m_hBrush);

    m_hBrush = ::CreateSolidBrush(crBkgnd);
    return *this;
}

CLabel&
CLabel::SetFontName(const CString& sFont)
{
    strcpy(m_lf.lfFaceName, sFont);
    ReconstructFont();
    RedrawWindow();
    return *this;
}

BEGIN_MESSAGE_MAP(CLabel, CStatic)
//{{AFX_MSG_MAP(CLabel)
ON_WM_CTLCOLOR_REFLECT()
ON_WM_TIMER()
ON_WM_LBUTTONDOWN()
ON_WM_SETCURSOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabel message handlers

HBRUSH
CLabel::CtlColor(CDC* pDC, UINT nCtlColor)
{
    // TODO: Change any attributes of the DC here

    // TODO: Return a non-NULL brush if the parent's handler should not be called

    if (CTLCOLOR_STATIC == nCtlColor) {
        pDC->SelectObject(&m_font);
        pDC->SetTextColor(m_crText);
        pDC->SetBkMode(TRANSPARENT);
    }

    if (m_Type == Background) {
        if (!m_bState)
            return m_hwndBrush;
    }

    return m_hBrush;
}

void
CLabel::ReconstructFont()
{
    m_font.DeleteObject();
    BOOL bCreated = m_font.CreateFontIndirect(&m_lf);

    ASSERT(bCreated);
}

CLabel&
CLabel::FlashText(BOOL bActivate)
{
    if (m_bTimer) {
        SetWindowText(m_sText);
        KillTimer(FLASH_TIMER);
    }

    if (bActivate) {
        GetWindowText(m_sText);
        m_bState = FALSE;

        m_bTimer = TRUE;
        SetTimer(FLASH_TIMER, FLASH_TIMER_DELAY, NULL);
        m_Type = Text;
    }

    return *this;
}

CLabel&
CLabel::FlashBackground(BOOL bActivate)
{

    if (m_bTimer)
        KillTimer(FLASH_TIMER);

    if (bActivate) {
        m_bState = FALSE;

        m_bTimer = TRUE;
        SetTimer(FLASH_TIMER, FLASH_TIMER_DELAY, NULL);

        m_Type = Background;
    }

    return *this;
}

void
CLabel::OnTimer(UINT nIDEvent)
{
    m_bState = !m_bState;

    switch (m_Type) {
        case Text:
            if (m_bState)
                SetWindowText("");
            else
                SetWindowText(m_sText);
            break;

        case Background:
            InvalidateRect(NULL, FALSE);
            UpdateWindow();
            break;
    }

    CStatic::OnTimer(nIDEvent);
}

CLabel&
CLabel::SetLink(BOOL bLink)
{
    m_bLink = bLink;

    if (bLink)
        ModifyStyle(0, SS_NOTIFY);
    else
        ModifyStyle(SS_NOTIFY, 0);

    return *this;
}

void
CLabel::OnLButtonDown(UINT nFlags, CPoint point)
{
    CString sLink;

    GetWindowText(sLink);
    ShellExecute(NULL, "open", sLink, NULL, NULL, SW_SHOWNORMAL);

    CStatic::OnLButtonDown(nFlags, point);
}

BOOL
CLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_hCursor) {
        ::SetCursor(m_hCursor);
        return TRUE;
    }

    return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

CLabel&
CLabel::SetLinkCursor(HCURSOR hCursor)
{
    m_hCursor = hCursor;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CLabelTitle

CLabelTitle::CLabelTitle()
{
    LOGFONT lf;
    ::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);

    lf.lfHeight = -14;
    lf.lfWeight = FW_BOLD;
    m_font.CreateFontIndirect(&lf);

    m_colText = 0x000000;
    m_colName = 0x800000;
    /*	m_colGrad1 = 0xff7800;
	m_colGrad2 = 0x000000;

	m_hInst = ::LoadLibrary("msimg32.dll");
	if(m_hInst)
	{
		m_dllfunc_GradientFill = ((LPFNDLLFUNC1)::GetProcAddress(m_hInst, "GradientFill"));
	}
	else
		m_dllfunc_GradientFill = NULL;
*/
}

CLabelTitle::~CLabelTitle()
{
    //	::FreeLibrary(m_hInst);
}

BEGIN_MESSAGE_MAP(CLabelTitle, CStatic)
//{{AFX_MSG_MAP(CLabelTitle)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabelTitle message handlers

void
CLabelTitle::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rc;
    GetClientRect(&rc);

    // Instead of gradient fill - simple fill all area by default color
    dc.FillSolidRect(&rc, GetSysColor(COLOR_3DFACE));

    /*
	if (m_dllfunc_GradientFill != NULL)
	{
		TRIVERTEX rcVertex[2];
		rc.right--; // exclude this point, like FillRect does 
		rc.bottom--;
		rcVertex[0].x = rc.left;
		rcVertex[0].y = rc.top;
		rcVertex[0].Red = GetRValue(m_colGrad1)<<8;	// color values from 0x0000 to 0xff00 !!!!
		rcVertex[0].Green = GetGValue(m_colGrad1)<<8;
		rcVertex[0].Blue = GetBValue(m_colGrad1)<<8;
		rcVertex[0].Alpha = 0x0000;
		rcVertex[1].x = rc.right; 
		rcVertex[1].y = rc.bottom;
		rcVertex[1].Red = GetRValue(m_colGrad2)<<8;
		rcVertex[1].Green = GetGValue(m_colGrad2)<<8;
		rcVertex[1].Blue = GetBValue(m_colGrad2)<<8;
		rcVertex[1].Alpha = 0;
		GRADIENT_RECT rect;
		rect.UpperLeft=0;
		rect.LowerRight=1;
		
		// fill the area 
		m_dllfunc_GradientFill(dc, rcVertex, 2, &rect, 1, GRADIENT_FILL_RECT_H);
	}
	rc.right++;
	rc.bottom++;
*/
    CString strText;
    GetWindowText(strText);
    CFont* pOldFont = dc.SelectObject(&m_font);
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(m_colText);
    if (m_sName.IsEmpty()) {
        dc.DrawText(strText, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
    } else {
        CSize szText = dc.GetTextExtent(strText);
        CSize szName = dc.GetTextExtent(m_sName);
        CString strBlanc("  ");
        CSize szBlanc = dc.GetTextExtent(strBlanc);

        rc.left += ((rc.right - rc.left) - (szText.cx + szName.cx + szBlanc.cx)) / 2;
        dc.DrawText(strText, &rc, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
        rc.left += szText.cx + szBlanc.cx;
        dc.SetTextColor(m_colName);
        dc.DrawText(m_sName, &rc, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
    }

    dc.SelectObject(pOldFont);
}

void
CLabelTitle::SetName(CString sName)
{
    if (m_sName.Compare(sName) != 0) {
        m_sName = sName;
        if (::IsWindow(m_hWnd))
            RedrawWindow();
    }
}

CString
CLabelTitle::GetName()
{
    return m_sName;
}
