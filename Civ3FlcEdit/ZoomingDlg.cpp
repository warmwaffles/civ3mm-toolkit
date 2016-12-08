/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ZoomingDlg.cpp
	CREATED:	20.09.2004
	VERSION:	1.0

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the ZoomingDlg class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "ZoomingDlg.h"

#include "FileImgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_ZOOM "Zoom"
#define REG_FIT "Fit"

/////////////////////////////////////////////////////////////////////////////
// CZoomingDlg dialog

CZoomingDlg::CZoomingDlg(CSurface* pSurface, BYTE nWidth, BYTE nHeight, CWnd* pParent /*=NULL*/)
  : CDialogX(CZoomingDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CZoomingDlg)
    m_bZoomFit = FALSE;
    //}}AFX_DATA_INIT

    ASSERT(pSurface);
    m_pSurface   = pSurface;
    m_nWidth     = nWidth;
    m_nHeight    = nHeight;
    m_nScrollHor = 0;
    m_nScrollVer = 0;

    SetControlInfo(IDC_SCROLLBAR_VERT, CF_ANCHOR_RIGHT | CF_RESIZE_VER);
    SetControlInfo(IDC_SCROLLBAR_HORIZ, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR);
    SetControlInfo(IDC_ZOOM_FIT, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT);
    SetControlInfo(IDC_PREVIEW, CF_RESIZE_BOTH);

    SetControlInfo(IDC_ZOOM_IN, CF_ANCHOR_BOTTOM);
    SetControlInfo(IDC_ZOOMING, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR);
    SetControlInfo(IDC_ZOOM_OUT, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT);

    SetControlInfo(IDC_SAVE, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT, 2);
    SetControlInfo(IDCANCEL, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT, 2, 1);
}

void
CZoomingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CZoomingDlg)
    DDX_Control(pDX, IDC_ZOOM_FIT, m_btnZoomFit);
    DDX_Control(pDX, IDC_SCROLLBAR_VERT, m_wndScrollVer);
    DDX_Control(pDX, IDC_SCROLLBAR_HORIZ, m_wndScrollHor);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_SAVE, m_btnSave);
    DDX_Control(pDX, IDC_ZOOMING, m_wndZooming);
    DDX_Control(pDX, IDC_ZOOM_IN, m_btnZoomIn);
    DDX_Control(pDX, IDC_ZOOM_OUT, m_btnZoomOut);
    DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
    DDX_Check(pDX, IDC_ZOOM_FIT, m_bZoomFit);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CZoomingDlg, CDialogX)
//{{AFX_MSG_MAP(CZoomingDlg)
ON_BN_CLICKED(IDC_ZOOM_IN, OnZoomIn)
ON_BN_CLICKED(IDC_ZOOM_OUT, OnZoomOut)
ON_CBN_SELENDOK(IDC_ZOOMING, OnSelendokZooming)
ON_WM_DESTROY()
ON_WM_PAINT()
ON_WM_SIZE()
ON_BN_CLICKED(IDC_SAVE, OnSave)
ON_CBN_KILLFOCUS(IDC_ZOOMING, OnKillfocusZooming)
ON_WM_VSCROLL()
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_ZOOM_FIT, OnZoomFit)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomingDlg message handlers

BOOL
CZoomingDlg::OnInitDialog()
{
    CDialogX::OnInitDialog();

    LoadPosition(this);

    // Set icon to the dialog caption
    SetIcon(AfxGetApp()->LoadIcon(IDI_ZOOM), FALSE);

    m_btnZoomIn.SetIcon(IDI_ZOOM_IN);
    m_btnZoomOut.SetIcon(IDI_ZOOM_OUT);

    m_btnSave.SetIcon(IDI_SAVE);
    m_btnCancel.SetIcon(IDI_OK);

    // Set SCROLLINFO for the scroll bars
    SCROLLINFO info;
    info.cbSize    = sizeof(SCROLLINFO);
    info.fMask     = SIF_ALL;
    info.nMin      = 0;
    info.nMax      = 100;
    info.nPage     = 10;
    info.nPos      = 0;
    info.nTrackPos = 0;

    m_wndScrollVer.SetScrollInfo(&info);
    m_wndScrollHor.SetScrollInfo(&info);

    m_btnZoomFit.SetIcon(IDI_ZOOM_FIT);
    m_btnZoomFit.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
    m_btnZoomFit.SetTooltipText(IDS_CONSTRAIN_PROP);

    if (!GetImage()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    CWinApp* pApp = AfxGetApp();
    m_nZoomFactor = pApp->GetProfileInt(RegKey(), REG_ZOOM, 100);
    m_bZoomFit    = pApp->GetProfileInt(RegKey(), REG_FIT, 0);

    if (m_nZoomFactor < 1 && m_nZoomFactor > 9999)
        m_nZoomFactor = 100;

    m_wndZooming.InitComboZoom(m_nZoomFactor);

    UpdateData(FALSE);

    UpdateControlsState();
    return TRUE;
}

void
CZoomingDlg::OnDestroy()
{
    SavePosition(this);

    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(RegKey(), REG_ZOOM, m_nZoomFactor);
    pApp->WriteProfileInt(RegKey(), REG_FIT, m_bZoomFit);

    CloseFile();

    CDialogX::OnDestroy();
}

void
CZoomingDlg::OnZoomIn()
{
    m_wndZooming.ZoomIn();
    UpdatePreview();
}

void
CZoomingDlg::OnZoomOut()
{
    m_wndZooming.ZoomOut();
    UpdatePreview();
}

void
CZoomingDlg::OnSelendokZooming()
{
    CString str;
    // Get scale factor from the scale combobox
    if (m_wndZooming.GetCurSel() != CB_ERR)
        m_wndZooming.GetLBText(m_wndZooming.GetCurSel(), str);
    else
        m_wndZooming.GetWindowText(str);

    if (str.IsEmpty()) {
        m_wndZooming.SelectString(-1, _T("100%"));
        m_wndZooming.GetWindowText(str);
    }

    m_nZoomFactor = atoi(str);
    UpdatePreview();
}

void
CZoomingDlg::OnKillfocusZooming()
{
    if (GetSafeHwnd()) {
        CString str;
        str.Format("%d%%", m_nZoomFactor);
        m_wndZooming.SetWindowText(str);
    }
}

BOOL
CZoomingDlg::GetImage()
{
    CWaitCursor wait();
    CloseFile();

    try {
        if (m_nWidth == 0 || m_nHeight == 0) {
            AfxMessageBox(IDS_INVALID_PARAMS, MB_ICONERROR);
            return FALSE;
        }

        if (m_hbmp.Flags.Allocated)
            GRAPHX::FreeBitmap(&m_hbmp);

        GRAPHX::CreateBitmap(&m_hbmp, TYPE_CONV, m_nWidth, m_nHeight, 24, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);

        // Get dc where to draw
        CDC* pDC    = CDC::FromHandle(GRAPHX::CreateGraphXDC(&m_hbmp));
        HDC hSurfDC = NULL;
        HDC hDC     = pDC->m_hDC;

        LPDIRECTDRAWSURFACE lpDDs;

        // Get current surface
        lpDDs = m_pSurface->GetDDS();

        // Creates a GDI-compatible device context of the surface
        if (FAILED(lpDDs->GetDC(&hSurfDC)))
            return FALSE;

        // Copy whole picture to the temp HDC
        BitBlt(hDC, 0, 0, m_nWidth, m_nHeight, hSurfDC, 0, 0, SRCCOPY);

        // Releases the hDC previously obtained with GetDC
        if (FAILED(lpDDs->ReleaseDC(hSurfDC)))
            return FALSE;

        // Release DC
        GRAPHX::DeleteGraphXDC(*pDC);
    } catch (CGraphXException* e) {
        AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (CMemoryException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CZoomingDlg::GetImage"), MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

void
CZoomingDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (m_wndPreview.GetSafeHwnd()) {
        CRect rc;
        m_wndPreview.GetWindowRect(rc);
        ScreenToClient(rc);
        dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));

        if (IsLoaded()) {
            rc.DeflateRect(3, 3);

            if (!m_bZoomFit) {
                m_rcCur.CopyRect(rc);
                m_rcCur.right  = m_rcCur.left + (m_nWidth * m_nZoomFactor) / 100;
                m_rcCur.bottom = m_rcCur.top + (m_nHeight * m_nZoomFactor) / 100;

                int nHor = (m_nWidth * (m_nZoomFactor / 100)) * m_nScrollHor / 100;
                int nVer = (m_nHeight * (m_nZoomFactor / 100)) * m_nScrollVer / 100;

                CRect rcDest(m_rcCur);
                rcDest.OffsetRect(-nHor, -nVer);

                GRAPHX::PaintDC(dc.m_hDC, &m_hbmp, NULL, NULL, rcDest, rc, SRCCOPY);
            } else {
                Render(&dc, rc, CGraphXImage::PaintProportion);
                m_rcCur.CopyRect(rc);
            }
        }
    }
}

void
CZoomingDlg::OnSize(UINT nType, int cx, int cy)
{
    UpdatePreview();
    CDialogX::OnSize(nType, cx, cy);
}

void
CZoomingDlg::OnSave()
{
    int nCX = m_bZoomFit ? m_rcCur.Width() : (m_nWidth * m_nZoomFactor) / 100;
    int nCY = m_bZoomFit ? m_rcCur.Height() : (m_nHeight * m_nZoomFactor) / 100;

    GRAPHX::SizeBitmap(&m_hbmp, nCX, nCY);

    SaveToFile();

    OnOK();
}

void
CZoomingDlg::UpdatePreview()
{
    if (m_wndPreview.GetSafeHwnd()) {
        CRect rc;
        m_wndPreview.GetWindowRect(rc);
        ScreenToClient(rc);
        InvalidateRect(rc);
    }
}

BOOL
CZoomingDlg::PreTranslateMessage(MSG* pMsg)
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

    return CDialogX::PreTranslateMessage(pMsg);
}

void
CZoomingDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Get the minimum and maximum scroll-bar positions.
    int minpos;
    int maxpos;
    pScrollBar->GetScrollRange(&minpos, &maxpos);
    maxpos = pScrollBar->GetScrollLimit();

    // Get the current position of scroll box.
    m_nScrollVer = pScrollBar->GetScrollPos();

    // Determine the new position of scroll box.
    switch (nSBCode) {
        case SB_TOP: // Scroll to far left.
            m_nScrollVer = minpos;
            break;

        case SB_BOTTOM: // Scroll to far right.
            m_nScrollVer = maxpos;
            break;

        case SB_ENDSCROLL: // End scroll.
            break;

        case SB_LINEUP: // Scroll left.
            if (m_nScrollVer > minpos)
                m_nScrollVer--;
            break;

        case SB_LINEDOWN: // Scroll right.
            if (m_nScrollVer < maxpos)
                m_nScrollVer++;
            break;

        case SB_PAGEUP: // Scroll one page left.
        {
            // Get the page size.
            SCROLLINFO info;
            pScrollBar->GetScrollInfo(&info, SIF_ALL);

            if (m_nScrollVer > minpos)
                m_nScrollVer = max(minpos, m_nScrollVer - (int)info.nPage);
        } break;

        case SB_PAGEDOWN: // Scroll one page right.
        {
            // Get the page size.
            SCROLLINFO info;
            pScrollBar->GetScrollInfo(&info, SIF_ALL);

            if (m_nScrollVer < maxpos)
                m_nScrollVer = min(maxpos, m_nScrollVer + (int)info.nPage);
        } break;

        case SB_THUMBPOSITION:   // Scroll to absolute position. nPos is the position
            m_nScrollVer = nPos; // of the scroll box at the end of the drag operation.
            break;

        case SB_THUMBTRACK:      // Drag scroll box to specified position. nPos is the
            m_nScrollVer = nPos; // position that the scroll box has been dragged to.
            break;
    }

    // Set the new position of the thumb (scroll box).
    pScrollBar->SetScrollPos(m_nScrollVer);
    UpdatePreview();

    //	CDialogX::OnVScroll(nSBCode, nPos, pScrollBar);
}

void
CZoomingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Get the minimum and maximum scroll-bar positions.
    int minpos;
    int maxpos;
    pScrollBar->GetScrollRange(&minpos, &maxpos);
    maxpos = pScrollBar->GetScrollLimit();

    // Get the current position of scroll box.
    m_nScrollHor = pScrollBar->GetScrollPos();

    // Determine the new position of scroll box.
    switch (nSBCode) {
        case SB_LEFT: // Scroll to far left.
            m_nScrollHor = minpos;
            break;

        case SB_RIGHT: // Scroll to far right.
            m_nScrollHor = maxpos;
            break;

        case SB_ENDSCROLL: // End scroll.
            break;

        case SB_LINELEFT: // Scroll left.
            if (m_nScrollHor > minpos)
                m_nScrollHor--;
            break;

        case SB_LINERIGHT: // Scroll right.
            if (m_nScrollHor < maxpos)
                m_nScrollHor++;
            break;

        case SB_PAGELEFT: // Scroll one page left.
        {
            // Get the page size.
            SCROLLINFO info;
            pScrollBar->GetScrollInfo(&info, SIF_ALL);

            if (m_nScrollHor > minpos)
                m_nScrollHor = max(minpos, m_nScrollHor - (int)info.nPage);
        } break;

        case SB_PAGERIGHT: // Scroll one page right.
        {
            // Get the page size.
            SCROLLINFO info;
            pScrollBar->GetScrollInfo(&info, SIF_ALL);

            if (m_nScrollHor < maxpos)
                m_nScrollHor = min(maxpos, m_nScrollHor + (int)info.nPage);
        } break;

        case SB_THUMBPOSITION:   // Scroll to absolute position. nPos is the position
            m_nScrollHor = nPos; // of the scroll box at the end of the drag operation.
            break;

        case SB_THUMBTRACK:      // Drag scroll box to specified position. nPos is the
            m_nScrollHor = nPos; // position that the scroll box has been dragged to.
            break;
    }

    // Set the new position of the thumb (scroll box).
    pScrollBar->SetScrollPos(m_nScrollHor);
    UpdatePreview();

    //	CDialogX::OnHScroll(nSBCode, nPos, pScrollBar);
}

void
CZoomingDlg::OnZoomFit()
{
    UpdateControlsState();
}

void
CZoomingDlg::UpdateControlsState()
{
    UpdateData();

    m_wndZooming.EnableWindow(!m_bZoomFit);
    m_btnZoomIn.EnableWindow(!m_bZoomFit);
    m_btnZoomOut.EnableWindow(!m_bZoomFit);

    m_wndScrollHor.EnableWindow(!m_bZoomFit);
    m_wndScrollVer.EnableWindow(!m_bZoomFit);

    UpdatePreview();
}
