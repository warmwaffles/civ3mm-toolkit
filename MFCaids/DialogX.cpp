//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//
// FILE NAME:   DialogX.cpp
// AUTHOR:      Tommy Svensson and Ari Greenberg (???)
// DESCRIPTION:	DialogX implements "cool" dialog.
//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include "stdafx.h"
#include "DialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////
// CDialogX class member functions implementation

CDialogX::CDialogX(UINT nIDTemplate, CWnd* pParentWnd)
  : CDialog(nIDTemplate, pParentWnd)
{
    m_bFirstSize = TRUE;
    m_minWidth = m_minHeight = 0; // flag that GetMinMax wasn't called yet
    m_xDlgSize = m_yDlgSize = 0;
    m_bSizeChanged          = FALSE;
    m_nIDTemplate           = nIDTemplate;

    m_bRememberSize = FALSE;
    m_bDrawGripper  = TRUE;

    m_bRedrawOnResize = TRUE;

    m_xPrevSize = m_yPrevSize = 0;
}

void
CDialogX::SetControlInfo(UINT CtrlId, WORD Anchor, WORD xDivider, WORD xDivPos, WORD yDivider, WORD yDivPos)
{
    if (Anchor == CF_ANCHOR_LEFT)
        return; // Do nothing

    CFormCtrlInfo cfci;
    cfci.m_id       = CtrlId;
    cfci.m_Anchor   = Anchor;
    cfci.m_xDivider = xDivider;
    cfci.m_xDivPos  = xDivPos;
    cfci.m_yDivider = yDivider;
    cfci.m_yDivPos  = yDivPos;

    m_CtrlInfo.Add(cfci);
}

BEGIN_MESSAGE_MAP(CDialogX, CDialog)
//{{AFX_MSG_MAP(CDialogX)
ON_WM_SIZE()
ON_WM_GETMINMAXINFO()
ON_WM_DESTROY()
ON_WM_CREATE()
ON_WM_NCHITTEST()
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// OnInitDialog()
//
BOOL
CDialogX::OnInitDialog()
{
    CDialog::OnInitDialog();
    if (m_bRememberSize) {
        // Load the previous size of the dialog box from the INI/Registry
        CString sDialogName;
        GetDialogProfileEntry(sDialogName);

        int cx = AfxGetApp()->GetProfileInt(sDialogName, "CX", 0);
        int cy = AfxGetApp()->GetProfileInt(sDialogName, "CY", 0);

        if (cx && cy)
            SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE);
    }

    return FALSE; // return TRUE  unless you set the focus to a control
}

//
// OnSize()
// Set the dialog controls new position and size
//
void
CDialogX::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    m_bSizeChanged = TRUE;

    if (nType == SIZE_MINIMIZED)
        return;

    if (!(m_xDlgSize || m_yDlgSize)) {
        m_xDlgSize = m_xPrevSize = cx;
        m_yDlgSize = m_yPrevSize = cy;

        return;
    }

    if (m_bFirstSize) {
        for (int i = 0; i < m_CtrlInfo.GetSize(); i++) {
            if (GetDlgItem(m_CtrlInfo[i].m_id)->GetSafeHwnd()) {
                GetDlgItem(m_CtrlInfo[i].m_id)->GetWindowRect(&m_CtrlInfo[i].m_pos);
                ScreenToClient(&m_CtrlInfo[i].m_pos);
            } else {
                TRACE1("*** Control with id #%d is not exist! ***\r\n", m_CtrlInfo[i].m_id);
                ASSERT(FALSE);
            }
        }
        m_bFirstSize = FALSE;
    }

    int dx = cx - m_xDlgSize;
    int dy = cy - m_yDlgSize;

    // Move and Size the controls using the information
    // we got in SetControlInfo()
    //
    CRect WndRect;
    CWnd* pWnd;
    CFormCtrlInfo cInfo;

    for (int i = 0; i < m_CtrlInfo.GetSize(); i++) {
        cInfo = m_CtrlInfo[i];
        pWnd  = GetDlgItem(cInfo.m_id);

        if (!pWnd) {
            TRACE("Control ID - %d NOT FOUND!!\n", cInfo.m_id);
            continue;
        }

        if (!cInfo.m_Anchor)
            continue; // do nothing if anchored to top and or left

        WndRect = cInfo.m_pos;

        if (cInfo.m_Anchor & CF_RESIZE_HOR)
            WndRect.right += dx / cInfo.m_xDivider;

        if (cInfo.m_Anchor & CF_ANCHOR_RIGHT)
            WndRect.OffsetRect(dx * (cInfo.m_xDivPos) / cInfo.m_xDivider, 0);

        if (cInfo.m_Anchor & CF_RESIZE_VER)
            WndRect.bottom += dy / cInfo.m_yDivider;

        if (cInfo.m_Anchor & CF_ANCHOR_BOTTOM)
            //			WndRect.OffsetRect(0, dy / cInfo.m_yDivider);
            WndRect.OffsetRect(0, dy * (cInfo.m_yDivPos) / cInfo.m_yDivider);

        pWnd->MoveWindow(&WndRect, m_bRedrawOnResize);
        pWnd->Invalidate(m_bRedrawOnResize);
    }

    InvalidateRect(CRect(m_xPrevSize - GetSystemMetrics(SM_CXHSCROLL), m_yPrevSize - GetSystemMetrics(SM_CYVSCROLL), cx, cy),
                   m_bRedrawOnResize);
    UpdateWindow();
    m_xPrevSize = cx;
    m_yPrevSize = cy;
}

void
CDialogX::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    if (!m_minWidth) // first time
    {
        CDialog::OnGetMinMaxInfo(lpMMI);
        return;
    }

    lpMMI->ptMinTrackSize.x = m_minWidth;
    lpMMI->ptMinTrackSize.y = m_minHeight;
}

void
CDialogX::OnDestroy()
{
    // Save the size of the dialog box, so next time
    // we'll start with this size
    if (m_bRememberSize && m_bSizeChanged && m_xDlgSize && m_yDlgSize) {
        CRect rc;
        GetWindowRect(&rc);
        CString sDialogName;
        GetDialogProfileEntry(sDialogName);

        AfxGetApp()->WriteProfileInt(sDialogName, "CX", rc.Width());
        AfxGetApp()->WriteProfileInt(sDialogName, "CY", rc.Height());
    }

    // Important: Reset the internal values in case of reuse of the dialog
    // with out deleting.
    m_minWidth = m_minHeight = m_xDlgSize = m_yDlgSize = m_xPrevSize = m_yPrevSize = 0;
    m_bSizeChanged                                                                 = FALSE;

    CDialog::OnDestroy();
}

//
// OnCreate()
//
int
CDialogX::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Remember the original size so later we can calculate
    // how to place the controls on dialog Resize
    m_minWidth  = lpCreateStruct->cx;
    m_minHeight = lpCreateStruct->cy;
    return 0;
}

//
// OnPaint()
// Override WM_PAINT to draw a gripper
//
// Credit goes to: Tommy Svensson
//
void
CDialogX::OnPaint()
{
    CDialog::OnPaint();

    // Draw a resizing gripper at the lower left corner
    //
    // Note: Make sure you leave enough space in your dialog template
    // for the gripper to be drawn.
    // Don't put any controls on the lower left corner.
    if (m_bDrawGripper) {
        CRect rc;
        GetClientRect(rc);

        rc.left       = rc.right - GetSystemMetrics(SM_CXHSCROLL);
        rc.top        = rc.bottom - GetSystemMetrics(SM_CYVSCROLL);
        m_GripperRect = rc;
        CClientDC dc(this);
        dc.DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
    }
}

//
// OnNcHitTest
// Handle mouse over the gripper
//
// Credit: Tommy Svensson
//
UINT
CDialogX::OnNcHitTest(CPoint point)
{
    UINT ht = CDialog::OnNcHitTest(point);

    if (ht == HTCLIENT && m_bDrawGripper) {
        CRect rc;
        GetWindowRect(rc);
        rc.left = rc.right - GetSystemMetrics(SM_CXHSCROLL);
        rc.top  = rc.bottom - GetSystemMetrics(SM_CYVSCROLL);
        if (rc.PtInRect(point)) {
            ht = HTBOTTOMRIGHT;
        }
    }
    return ht;
}

//
// GetDialogProfileEntry()
// Override this (virtual) function in your derived class
// if you want to store the dialog info under a different entry name.
//
// Credit: Ari Greenberg
void
CDialogX::GetDialogProfileEntry(CString& sEntry)
{
    // By default store the size under the Dialog ID value (Hex)
    sEntry.Format("%x", m_nIDTemplate);
}
