/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	27.08.2004
	VERSION:	2.2

	AUTHOR:		Dreyk

	DESCRIPTION: CDialogBarMy.cpp : conversion of the CDialogBarMy class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "DialogBarMy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CX_EDGE 6
#define CX_BORDER 3
#define CY_BORDER 3
#define CX_GRIPPER 20
#define CX_TOP_INDENT 25

/////////////////////////////////////////////////////////////////////////////
// CDialogBarMy

CDialogBarMy::CDialogBarMy()
  : m_clrBtnHilight(::GetSysColor(COLOR_BTNHILIGHT))
  , m_clrBtnShadow(::GetSysColor(COLOR_BTNSHADOW))
{
    m_sizeMin   = CSize(32, 32);
    m_sizeHorz  = CSize(200, 200);
    m_sizeVert  = CSize(200, 200);
    m_sizeFloat = CSize(200, 200);
    m_pDialog   = NULL;
    m_brushBkgd.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

    // Rob Wolpov 10/15/98 Added support for diagonal resizing
    m_cCaptionSize = GetSystemMetrics(SM_CYSMCAPTION);
    m_cMinWidth    = GetSystemMetrics(SM_CXMIN);
    m_cMinHeight   = GetSystemMetrics(SM_CYMIN);

    m_bPressedClose = m_bPressedMax = m_bPushedClose = m_bPushedMax = FALSE;
}

CDialogBarMy::~CDialogBarMy()
{
}

BEGIN_MESSAGE_MAP(CDialogBarMy, CControlBar)
//{{AFX_MSG_MAP(CDialogBarMy)
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_WINDOWPOSCHANGED()
ON_WM_NCPAINT()
ON_WM_NCLBUTTONDOWN()
ON_WM_NCLBUTTONUP()
ON_WM_NCHITTEST()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDBLCLK()
ON_WM_NCLBUTTONDBLCLK()
ON_WM_DESTROY()
ON_WM_SIZE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBarMy message handlers

void
CDialogBarMy::OnUpdateCmdUI(class CFrameWnd* pTarget, int bDisableIfNoHndler)
{
    UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

BOOL
CDialogBarMy::Create(CWnd* pParentWnd, CDialog* pDialog, CString& pTitle, UINT nID, DWORD dwStyle)
{
    ASSERT_VALID(pParentWnd); // must have a parent
    ASSERT(!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

    // save the style -- AMENDED by Holger Thiele - Thank you
    m_dwStyle = dwStyle & CBRS_ALL;

    // create the base window
    CString sWndClass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), m_brushBkgd, 0);
    if (!CWnd::Create(sWndClass, pTitle, dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID))
        return FALSE;

    // create the child dialog
    m_pDialog = pDialog;
    if (!m_pDialog->Create(nID, this))
        return FALSE;

    // use the dialog dimensions as default base dimensions
    CRect rc;
    m_pDialog->GetWindowRect(rc);

    ::SetWindowPos(m_pDialog->m_hWnd, NULL, 0, CX_TOP_INDENT, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);

    CString sReg = RegKey();
    if (sReg.IsEmpty()) {
        m_sizeHorz = m_sizeVert = m_sizeFloat = rc.Size();
    } else {
        m_sizeFloat.cx = AfxGetApp()->GetProfileInt(sReg, "FloatX", rc.Width());
        m_sizeFloat.cy = AfxGetApp()->GetProfileInt(sReg, "FloatY", rc.Height());

        m_sizeHorz.cx = AfxGetApp()->GetProfileInt(sReg, "HorzX", rc.Width());
        m_sizeHorz.cy = AfxGetApp()->GetProfileInt(sReg, "HorzY", rc.Height());

        m_sizeVert.cx = AfxGetApp()->GetProfileInt(sReg, "VertX", rc.Width());
        m_sizeVert.cy = AfxGetApp()->GetProfileInt(sReg, "VertY", rc.Height());
    }

    return TRUE;
}

CSize
CDialogBarMy::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    CRect rc;

    m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_TOP)->GetWindowRect(rc);
    int nHorzDockBarWidth = bStretch ? 32767 : rc.Width() + 4;
    m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT)->GetWindowRect(rc);
    int nVertDockBarHeight = bStretch ? 32767 : rc.Height() + 4;

    if (IsFloating())
        return m_sizeFloat;
    else if (bHorz)
        return CSize(nHorzDockBarWidth, m_sizeHorz.cy);
    else
        return CSize(m_sizeVert.cx, nVertDockBarHeight);
}

CSize
CDialogBarMy::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    // Rob Wolpov 10/15/98 Added support for diagonal sizing
    if (IsFloating()) {
        // Enable diagonal arrow cursor for resizing
        GetParent()->GetParent()->ModifyStyle(MFS_4THICKFRAME, 0);
    }

    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK)) {
        SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        m_pDockSite->RecalcLayout();
        return CControlBar::CalcDynamicLayout(nLength, dwMode);
    }

    if (dwMode & LM_MRUWIDTH)
        return m_sizeFloat;

    if (dwMode & LM_COMMIT) {
        m_sizeFloat.cx = nLength;
        return m_sizeFloat;
    }

    // Rob Wolpov 10/15/98 Added support for diagonal sizing
    if (IsFloating()) {
        RECT window_rect;
        POINT cursor_pt;

        GetCursorPos(&cursor_pt);
        GetParent()->GetParent()->GetWindowRect(&window_rect);

        switch (m_pDockContext->m_nHitTest) {
            case HTTOPLEFT:
                m_sizeFloat.cx                           = max(window_rect.right - cursor_pt.x, m_cMinWidth) - CX_BORDER;
                m_sizeFloat.cy                           = max(window_rect.bottom - m_cCaptionSize - cursor_pt.y, m_cMinHeight) - 1;
                m_pDockContext->m_rectFrameDragHorz.top  = min(cursor_pt.y, window_rect.bottom - m_cCaptionSize - m_cMinHeight) - CY_BORDER;
                m_pDockContext->m_rectFrameDragHorz.left = min(cursor_pt.x, window_rect.right - m_cMinWidth) - 1;
                return m_sizeFloat;

            case HTTOPRIGHT:
                m_sizeFloat.cx                          = max(cursor_pt.x - window_rect.left, m_cMinWidth);
                m_sizeFloat.cy                          = max(window_rect.bottom - m_cCaptionSize - cursor_pt.y, m_cMinHeight) - 1;
                m_pDockContext->m_rectFrameDragHorz.top = min(cursor_pt.y, window_rect.bottom - m_cCaptionSize - m_cMinHeight) - CY_BORDER;
                return m_sizeFloat;

            case HTBOTTOMLEFT:
                m_sizeFloat.cx                           = max(window_rect.right - cursor_pt.x, m_cMinWidth) - CX_BORDER;
                m_sizeFloat.cy                           = max(cursor_pt.y - window_rect.top - m_cCaptionSize, m_cMinHeight);
                m_pDockContext->m_rectFrameDragHorz.left = min(cursor_pt.x, window_rect.right - m_cMinWidth) - 1;
                return m_sizeFloat;

            case HTBOTTOMRIGHT:
                m_sizeFloat.cx = max(cursor_pt.x - window_rect.left, m_cMinWidth);
                m_sizeFloat.cy = max(cursor_pt.y - window_rect.top - m_cCaptionSize, m_cMinHeight);
                return m_sizeFloat;
        }
    }

    if (dwMode & LM_LENGTHY)
        return CSize(m_sizeFloat.cx, m_sizeFloat.cy = max(m_sizeMin.cy, nLength));
    else
        return CSize(max(m_sizeMin.cx, nLength), m_sizeFloat.cy);
}

void
CDialogBarMy::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
    CControlBar::OnWindowPosChanged(lpwndpos);

    if (!::IsWindow(m_hWnd) || m_pDialog == NULL)
        return;

    if (!::IsWindow(m_pDialog->m_hWnd))
        return;

    // Find on which side are we docked
    UINT nDockBarID = GetParent()->GetDlgCtrlID();

    // Return if dropped at same location
    if (nDockBarID == m_nDockBarID // no docking side change
        &&
        (lpwndpos->flags & SWP_NOSIZE) // no size change
        &&
        ((m_dwStyle & CBRS_BORDER_ANY) != CBRS_BORDER_ANY))
        return;

    m_nDockBarID = nDockBarID;

    if (m_pDialog->GetSafeHwnd()) {
        CRect rc;
        GetClientRect(&rc);

        if (IsFloating())
            ::SetWindowPos(m_pDialog->m_hWnd, NULL, 0, CY_BORDER, rc.Width(), rc.Height() - CY_BORDER, SWP_NOZORDER | SWP_NOACTIVATE);
        else
            ::SetWindowPos(
              m_pDialog->m_hWnd, NULL, 0, CX_TOP_INDENT, rc.Width(), rc.Height() - CX_TOP_INDENT, SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void
CDialogBarMy::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bPressedClose) {
        ReleaseCapture();
        m_bPressedClose = FALSE;
        DrawGripper(NULL);

        if (m_bPushedClose) {
            OnCloseBar();
            return;
        }

        m_bPushedClose = FALSE;
    }

    if (m_bPressedMax) {
        ReleaseCapture();
        m_bPressedMax = FALSE;
        DrawGripper(NULL);

        if (m_bPushedMax)
            m_pDockContext->ToggleDocking();

        m_bPushedMax = FALSE;
    }
}

void
CDialogBarMy::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bPressedClose) {
        CRect rc;
        GetWindowRect(rc);
        ScreenToClient(rc);
        CPoint pt(point.x - rc.left, point.y - rc.top);
        m_bPushedClose = m_rectClose.PtInRect(pt);
        DrawGripper(NULL);
    }

    if (m_bPressedMax) {
        CRect rc;
        GetWindowRect(rc);
        ScreenToClient(rc);
        CPoint pt(point.x - rc.left, point.y - rc.top);
        m_bPushedMax = m_rectUndock.PtInRect(pt);
        DrawGripper(NULL);
    }

    if (IsFloating()) {
        CControlBar::OnMouseMove(nFlags, point);
        return;
    }
}

void
CDialogBarMy::OnNcPaint()
{
    EraseNonClient();

    CWindowDC dc(this);
    DrawGripper(&dc);
}

void
CDialogBarMy::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    if ((nHitTest == HTSYSMENU) && !IsFloating()) {
        m_bPressedClose = TRUE;
        m_bPushedClose  = TRUE;
        SetCapture();
        DrawGripper(NULL);
    } else if ((nHitTest == HTMINBUTTON) && !IsFloating()) {
        m_bPressedMax = TRUE;
        m_bPushedMax  = TRUE;
        SetCapture();
        DrawGripper(NULL);
    } else if ((nHitTest == HTCAPTION) && !IsFloating() && (m_pDockBar != NULL)) {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        if (m_pDockContext->m_pDC == NULL)
            m_pDockContext->StartDrag(point);
    } else
        CControlBar::OnNcLButtonDown(nHitTest, point);
}

void
CDialogBarMy::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    if (!IsFloating()) {
        if (nHitTest == HTSYSMENU || nHitTest == HTMINBUTTON) {
        } else if (nHitTest == HTCAPTION && m_pDockBar != NULL) {
            // start the drag
            ASSERT(m_pDockContext != NULL);
            if (m_pDockContext->m_pDC == NULL)
                m_pDockContext->StartDrag(point);
        } else
            CControlBar::OnNcLButtonDown(nHitTest, point);
    } else
        CControlBar::OnNcLButtonDown(nHitTest, point);
}

UINT
CDialogBarMy::OnNcHitTest(CPoint point)
{
    if (IsFloating())
        return CControlBar::OnNcHitTest(point);

    CRect rc;
    GetWindowRect(rc);
    point.Offset(-rc.left, -rc.top);
    if (m_rectClose.PtInRect(point))
        return HTSYSMENU;
    else if (m_rectUndock.PtInRect(point))
        return HTMINBUTTON;
    else if (m_rectGripper.PtInRect(point))
        return HTCAPTION;

    return CControlBar::OnNcHitTest(point);
}

void
CDialogBarMy::OnLButtonDown(UINT nFlags, CPoint point)
{
    // only start dragging if clicked in "void" space
    if (m_pDockBar != NULL) {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
        if (m_pDockContext->m_pDC == NULL)
            m_pDockContext->StartDrag(point);
    } else
        CWnd::OnLButtonDown(nFlags, point);
}

void
CDialogBarMy::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // only toggle docking if clicked in "void" space
    if (m_pDockBar != NULL) {
        // toggle docking
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    } else
        CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL
CDialogBarMy::IsHorz() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP || m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

CPoint&
CDialogBarMy::ClientToWnd(CPoint& point)
{
    if (m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM)
        point.y += CX_EDGE;
    else if (m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT)
        point.x += CX_EDGE;

    return point;
}

void
CDialogBarMy::DrawGripper(CDC* pDC)
{
    // no gripper if floating
    if (m_dwStyle & CBRS_FLOATING)
        return;

    CDC* pLocalDC = pDC;
    if (pDC == NULL)
        pLocalDC = new CWindowDC(this);

    // -==HACK==-
    // in order to calculate the client area properly after docking,
    // the client area must be recalculated twice (I have no idea why)
    if (pDC != NULL)
        m_pDockSite->RecalcLayout();
    // -==END HACK==-

    CRect gripper;
    GetWindowRect(gripper);
    ScreenToClient(gripper);
    gripper.OffsetRect(-gripper.left, -gripper.top);

    if (m_dwStyle & CBRS_ORIENT_VERT) {
        // gripper at top
        m_rectGripper.top    = gripper.top;
        m_rectGripper.bottom = gripper.top + CX_GRIPPER;
        m_rectGripper.left   = gripper.left;
        m_rectGripper.right  = gripper.right - 40;

        // draw close box
        m_rectClose.right  = gripper.right - 5;
        m_rectClose.left   = m_rectClose.right - 11;
        m_rectClose.top    = gripper.top + 7;
        m_rectClose.bottom = m_rectClose.top + 11;
        pLocalDC->DrawFrameControl(m_rectClose, DFC_CAPTION, DFCS_CAPTIONCLOSE | ((m_bPushedClose && m_bPressedClose) ? DFCS_PUSHED : 0));

        // draw docking toggle box
        m_rectUndock = m_rectClose;
        m_rectUndock.OffsetRect(-13, 0);
        pLocalDC->DrawFrameControl(m_rectUndock, DFC_CAPTION, DFCS_CAPTIONMAX | ((m_bPushedMax && m_bPressedMax) ? DFCS_PUSHED : 0));

        gripper.right -= 33;
        gripper.left += 10;
        gripper.top += 10;
        gripper.bottom = gripper.top + 3;
        pLocalDC->Draw3dRect(gripper, m_clrBtnHilight, m_clrBtnShadow);

        gripper.OffsetRect(0, 4);
        pLocalDC->Draw3dRect(gripper, m_clrBtnHilight, m_clrBtnShadow);
    }

    if (pDC == NULL)
        delete pLocalDC;
}

void
CDialogBarMy::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pDockBar != NULL && nHitTest == HTCAPTION) {
        // toggle docking
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    } else
        CWnd::OnNcLButtonDblClk(nHitTest, point);
}

void
CDialogBarMy::OnCloseBar()
{
    GetDockingFrame()->ShowControlBar(this, FALSE, FALSE);
}

LPCTSTR
CDialogBarMy::RegKey()
{
    return "Dlg Bar";
}

void
CDialogBarMy::OnDestroy()
{
    CString sReg = RegKey();

    if (!sReg.IsEmpty()) {
        AfxGetApp()->WriteProfileInt(sReg, "FloatX", m_sizeFloat.cx);
        AfxGetApp()->WriteProfileInt(sReg, "FloatY", m_sizeFloat.cy);

        AfxGetApp()->WriteProfileInt(sReg, "HorzX", m_sizeHorz.cx);
        AfxGetApp()->WriteProfileInt(sReg, "HorzY", m_sizeHorz.cy);

        AfxGetApp()->WriteProfileInt(sReg, "VertX", m_sizeVert.cx);
        AfxGetApp()->WriteProfileInt(sReg, "VertY", m_sizeVert.cy);

        AfxGetApp()->WriteProfileInt(sReg, "FloatPosX", m_pDockContext->m_ptMRUFloatPos.x);
        AfxGetApp()->WriteProfileInt(sReg, "FloatPosY", m_pDockContext->m_ptMRUFloatPos.y);

        AfxGetApp()->WriteProfileInt(sReg, "IsFloating", IsFloating());
    }

    CControlBar::OnDestroy();
}

void
CDialogBarMy::ToggleDocking()
{
    m_pDockContext->ToggleDocking();
}

void
CDialogBarMy::EnableDocking(DWORD dwStyle)
{
    CControlBar::EnableDocking(dwStyle);
    CString sReg = RegKey();

    if (!sReg.IsEmpty()) {
        m_pDockContext->m_ptMRUFloatPos.x = AfxGetApp()->GetProfileInt(sReg, "FloatPosX", 0);
        m_pDockContext->m_ptMRUFloatPos.y = AfxGetApp()->GetProfileInt(sReg, "FloatPosY", 0);
    }
}

void
CDialogBarMy::OnSize(UINT nType, int cx, int cy)
{
    CControlBar::OnSize(nType, cx, cy);

    if (m_pDialog->GetSafeHwnd()) {
        if (IsFloating())
            ::SetWindowPos(m_pDialog->m_hWnd, NULL, 0, CY_BORDER, cx, cy - CY_BORDER, SWP_NOZORDER | SWP_NOACTIVATE);
        else
            ::SetWindowPos(m_pDialog->m_hWnd, NULL, 0, CX_TOP_INDENT, cx, cy - CX_TOP_INDENT, SWP_NOZORDER | SWP_NOACTIVATE);
    }
}
