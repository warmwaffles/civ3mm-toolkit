/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ColorSelectDlg.cpp
	CREATED:	17.12.2004
	VERSION:	1.0

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ColorSelectDlg.h"

#include "FileImgAllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_ALL_PALETTE		"MoveAll"
#define PREV_SIZE			15

/////////////////////////////////////////////////////////////////////////////
// CColorSelectDlg dialog

CColorSelectDlg::CColorSelectDlg(RGBQUADPAL* pPal, CWnd* pParent /*=NULL*/)
	: CDialogX(CColorSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorSelectDlg)
	m_sPath = _T("");
	m_sRed = _T("");
	m_sIndex = _T("");
	m_sGreen = _T("");
	m_sBlue = _T("");
	m_sBlueS = _T("");
	m_sGreenS = _T("");
	m_sRedS = _T("");
	m_sIndexS = _T("");
	m_sBlueSrc = _T("");
	m_sGreenSrc = _T("");
	m_sRedSrc = _T("");
	//}}AFX_DATA_INIT

	SetControlInfo(IDC_SCROLLBAR_VERT, CF_ANCHOR_RIGHT | CF_RESIZE_VER);
	SetControlInfo(IDC_SCROLLBAR_HORIZ, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR);
	SetControlInfo(IDC_PREVIEW, CF_RESIZE_BOTH);

	SetControlInfo(IDC_APPLY, CF_ANCHOR_BOTTOM);
	SetControlInfo(IDC_REFRESH, CF_ANCHOR_BOTTOM);
	SetControlInfo(IDOK, CF_ANCHOR_BOTTOM);
	SetControlInfo(IDCANCEL, CF_ANCHOR_BOTTOM);
	
	SetControlInfo(IDC_PATH, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR);
	SetControlInfo(IDC_BROWSE_PATH, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT);

	m_nScrollHor	= 0;
	m_nScrollVer	= 0;
	m_bInsideRect	= FALSE;
	m_hCursor		= AfxGetApp()->LoadCursor(IDC_CROSS_SELECT);
	
	m_rcPreview.SetRectEmpty();

	ASSERT(pPal);
	m_pPalExtD = pPal;
}

void CColorSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorSelectDlg)
	DDX_Control(pDX, IDC_STATIC_CURRENTSRC, m_wndCurColorSrc);
	DDX_Control(pDX, IDC_STATIC_SELECTED, m_wndSelColor);
	DDX_Control(pDX, IDC_STATIC_CURRENT, m_wndCurColor);
	DDX_Text(pDX, IDC_COLOR_RED, m_sRed);
	DDX_Text(pDX, IDC_COLOR_INDEX, m_sIndex);
	DDX_Text(pDX, IDC_COLOR_GREEN, m_sGreen);
	DDX_Text(pDX, IDC_COLOR_BLUE, m_sBlue);
	DDX_Text(pDX, IDC_COLOR_BLUE_S, m_sBlueS);
	DDX_Text(pDX, IDC_COLOR_GREEN_S, m_sGreenS);
	DDX_Text(pDX, IDC_COLOR_RED_S, m_sRedS);
	DDX_Text(pDX, IDC_COLOR_INDEX_S, m_sIndexS);
	DDX_Text(pDX, IDC_COLOR_BLUE_SRC, m_sBlueSrc);
	DDX_Text(pDX, IDC_COLOR_GREEN_SRC, m_sGreenSrc);
	DDX_Text(pDX, IDC_COLOR_RED_SRC, m_sRedSrc);
	DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
	DDX_Control(pDX, IDC_ZOOM, m_wndZoom);
	DDX_Control(pDX, IDC_SCROLLBAR_VERT, m_wndScrollVer);
	DDX_Control(pDX, IDC_SCROLLBAR_HORIZ, m_wndScrollHor);
	DDX_Control(pDX, IDC_PATH, m_wndPath);
	DDX_Control(pDX, IDC_BROWSE_PATH, m_btnBrowse);
	DDX_Control(pDX, IDC_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_APPLY, m_btnApply);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_PATH, m_sPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorSelectDlg, CDialogX)
	//{{AFX_MSG_MAP(CColorSelectDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_BROWSE_PATH, OnBrowsePath)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorSelectDlg message handlers

BOOL CColorSelectDlg::OnInitDialog() 
{
	if (!m_pPalExtD)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Inicialize a palette
	memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);

	CDialogX::OnInitDialog();
	
	LoadPosition(this);
	
	m_btnOK.SetIcon(IDI_OK);
	m_btnCancel.SetIcon(IDI_CANCEL);
	m_btnApply.SetIcon(IDI_APPLY);
	m_btnRefresh.SetIcon(IDI_REFRESH);
	m_btnBrowse.SetIcon(IDI_BROWSE);

	// Set SCROLLINFO for the scroll bars 
	SCROLLINFO info;
	info.cbSize		= sizeof(SCROLLINFO);
	info.fMask		= SIF_ALL;
	info.nMin		= 0;
	info.nMax		= 100;
	info.nPage		= 10;
	info.nPos		= 0;
	info.nTrackPos	= 0;
	
	m_wndScrollVer.SetScrollInfo(&info);
	m_wndScrollHor.SetScrollInfo(&info);

	CWnd* pWnd = NULL;
	CRect rect;
	
	pWnd = GetDlgItem(IDC_DEST_PALETTE_POS);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	if (!m_wndColorTableD.Create(rect.TopLeft(), &m_rgbPalD, this, IDC_COLOR_TABLE))
	{
		TRACE("Cannot create destination ColorTable window\r\n");
		EndDialog(IDCANCEL);
		return FALSE;
	}
/*	
	m_wndPreview.GetWindowRect(m_rcPreview);
	ScreenToClient(m_rcPreview);
	m_rcPreview.right	= m_rcPreview.left + 5;
	m_rcPreview.bottom	= m_rcPreview.top + 5;
	m_ptPreview.x = m_rcPreview.left + 3;
	m_ptPreview.y = m_rcPreview.top + 3;
*/
	m_crDest	= m_crCurDest = m_wndColorTableD.GetColour(0);
	m_brDest	= CreateSolidBrush(m_crDest);
	m_brCurDest	= CreateSolidBrush(m_crDest);
	
	m_crSrc		= GetSysColor(COLOR_3DFACE);
	m_brSrc		= CreateSolidBrush(m_crSrc);

	UpdateData(FALSE);
	UpdateControlsState();
	UpdateColorInfo();
	return TRUE;
}

void CColorSelectDlg::OnDestroy() 
{
	SavePosition(this);
	CDialogX::OnDestroy();

	if (m_hCursor)
		::DestroyCursor(m_hCursor);
}

void CColorSelectDlg::OnOK() 
{
	OnApply();

	CDialogX::OnOK();
}

void CColorSelectDlg::OnCancel() 
{
	CDialogX::OnCancel();
}

void CColorSelectDlg::UpdateControlsState()
{
	UpdateData();

	m_wndPreview.GetClientRect(m_rcCur);

	if (IsLoaded())
	{
		if (m_hbmp.Width > m_rcCur.Width())
			m_wndScrollHor.EnableWindow(TRUE);
		else
		{
			m_nScrollHor = 0;
			m_wndScrollHor.SetScrollPos(0);
			m_wndScrollHor.EnableWindow(FALSE);
		}

		if (m_hbmp.Height > m_rcCur.Height())
			m_wndScrollVer.EnableWindow(TRUE);
		else
		{
			m_nScrollVer = 0;
			m_wndScrollVer.SetScrollPos(0);
			m_wndScrollVer.EnableWindow(FALSE);
		}
	}
	else
	{
		m_wndScrollHor.EnableWindow(FALSE);
		m_wndScrollVer.EnableWindow(FALSE);
	}

	UpdatePreview();
}

void CColorSelectDlg::ChangeVariables()
{
	UpdateData();
	
	m_wndColorTableD.Invalidate();
}

void CColorSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_wndPreview.GetSafeHwnd())
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);

		dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		
		if (IsLoaded())
		{
			// Draw Preview Image
			rc.DeflateRect(2, 2);
			
//			int nHor = m_hbmp.Width * m_nScrollHor / 100;
//			int nVer = m_hbmp.Height * m_nScrollVer / 100;
			
			CRect rcDest(rc);
//			rcDest.OffsetRect(-nHor, -nVer);

			rcDest.OffsetRect(-m_nScrollHor, -m_nScrollVer);
			
			GRAPHX::PaintDC(dc.m_hDC, &m_hbmp, /*CRect(0, 0, m_hbmp.Width, m_hbmp.Height)*/NULL, NULL, CRect(rcDest.left, rcDest.top, rcDest.left + m_hbmp.Width, rcDest.top + m_hbmp.Height)/*rcDest*/, rc, SRCCOPY);
			
			//////////////////////////////////////////////////////////////////////////
			// Draw zoomed colors preview
			DrawZoomedPreview();
/*			m_wndZoom.GetWindowRect(rc);
			ScreenToClient(rc);

			rc.right = rc.left + PREV_SIZE;
			rc.bottom = rc.top + PREV_SIZE;

			COLORREF color;
			int cx;
			
			for (int cy = 0; cy < 5; cy++)
			{
				for (cx = 0; cx < 5; cx++)
				{
					color = ::GetPixel(dc.m_hDC, m_rcPreview.left + cx, m_rcPreview.top + cy);
					dc.FillSolidRect(&rc, color);

					if (cx == 2 && cy == 2)
						dc.Draw3dRect(&rc, RGB(255,0,0), RGB(255,0,0));

					rc.OffsetRect(PREV_SIZE, 0);
				}

				rc.OffsetRect(-(PREV_SIZE * 5), PREV_SIZE);
				cx = 0;
			}
*/
		}//if (IsLoaded())
	}//if (m_wndPreview.GetSafeHwnd())
}

void CColorSelectDlg::OnApply() 
{
	// Apply palette changes
	memcpy(m_pPalExtD, &m_rgbPalD, RGBQUADPAL_SIZE);
}

void CColorSelectDlg::OnRefresh() 
{
	// Discard palette changes
	memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);
	m_wndColorTableD.SetPalette(&m_rgbPalD);
}

BOOL CColorSelectDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
    {
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
    }
	
	return CDialogX::PreTranslateMessage(pMsg);
}

void CColorSelectDlg::OnBrowsePath() 
{
	try
	{
		CFileImgAllDlg dlg(TRUE, NULL, NULL,
			OFN_ENABLESIZING | OFN_ENABLETEMPLATE | 
			OFN_EXPLORER | OFN_FILEMUSTEXIST | 
			OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST,
			_T(uLoadString(IDS_FILTER_IMAGE)),
			this);
		
		dlg.m_ofn.hInstance = AfxGetInstanceHandle();
		dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_IMG_ALL);
		
		CString sTitle(uLoadString(IDS_TITLE_IMAGE));
		dlg.m_ofn.lpstrTitle = sTitle;
		
		// last used filter
//		dlg.m_ofn.nFilterIndex = AfxGetApp()->GetProfileInt(REG_IMG_DLG, REG_FILTER, 1);
		
		if (uIsFileExist(m_sPath))
			dlg.m_ofn.lpstrInitialDir = m_sPath;
		
		dlg.m_ofn.hInstance = AfxGetInstanceHandle();

		if (dlg.DoModal() != IDOK)
			return;
		
		CString sPath = dlg.GetPathName();
		
//		pApp->WriteProfileString(RegKey(), REG_LAST_SRC, sPath);
//		pApp->WriteProfileInt(RegKey(), REG_FILTER, dlg.m_ofn.nFilterIndex);
		
		///////////////////////////////////////////////
		LoadFile(sPath);
		if (!IsLoaded())
			return;

		m_sPath = sPath;

		SCROLLINFO info;
		info.cbSize		= sizeof(SCROLLINFO);
		info.fMask		= SIF_ALL;
		info.nMin		= 0;
		info.nPos		= 0;
		info.nTrackPos	= 0;
		
		info.nMax		= m_hbmp.Width;
		info.nPage		= m_rcCur.Width();
		m_wndScrollHor.SetScrollInfo(&info);
		
		info.nMax		= m_hbmp.Height;
		info.nPage		= m_rcCur.Height();
		m_wndScrollVer.SetScrollInfo(&info);

	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CImportToC3F::OnBrowseTargetPath"), MB_ICONERROR);
		return;
	}
	
	UpdateData(FALSE);
	UpdateControlsState();
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorSelectDlg::OnSelEndOK()
// 
LONG CColorSelectDlg::OnSelEndOK(UINT lParam, LONG wParam)
{
	COLORREF col = (COLORREF)lParam;

	if (m_crCurDest != col)
	{
		m_crCurDest = col;
		
		if (m_brCurDest)
			DeleteObject(m_brCurDest);
		
		m_brCurDest = CreateSolidBrush(m_crCurDest);
		
		ApplyColorChanges();
	}
	
	UpdateColorInfo();
	m_wndSelColor.Invalidate();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorSelectDlg::OnSelEndCancel()
// 
LONG CColorSelectDlg::OnSelEndCancel(UINT lParam, LONG wParam)
{
	OnCancel();
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorSelectDlg::OnSelChange()
// 
LONG CColorSelectDlg::OnSelChange(UINT lParam, LONG wParam)
{
	COLORREF col = (COLORREF)lParam;
	if (m_crDest != col)
	{
		m_crDest = col;
		
		if (m_brDest)
			DeleteObject(m_brDest);
		
		m_brDest = CreateSolidBrush(m_crDest);
	}
	
	UpdateColorInfo();
	m_wndCurColor.Invalidate();

    return TRUE;
}

void CColorSelectDlg::UpdateColorInfo()
{
	// Current color
	m_sRed		= uInt2Str(GetRValue(m_crDest));
	m_sGreen	= uInt2Str(GetGValue(m_crDest));
	m_sBlue		= uInt2Str(GetBValue(m_crDest));
	
	int nIndex = m_wndColorTableD.m_nCurrent;
	if (nIndex != -1)
		m_sIndex = uInt2Str(nIndex);
	else
		m_sIndex.Empty();
	
	// Selected color
	m_sRedS		= uInt2Str(GetRValue(m_crCurDest));
	m_sGreenS	= uInt2Str(GetGValue(m_crCurDest));
	m_sBlueS	= uInt2Str(GetBValue(m_crCurDest));
	
	nIndex = m_wndColorTableD.m_nSelected;
	if (nIndex != -1)
		m_sIndexS = uInt2Str(nIndex);
	else
		m_sIndexS.Empty();
	
	UpdateData(FALSE);
}

void CColorSelectDlg::ApplyColorChanges()
{
	int idx = m_wndColorTableD.m_nSelected;
	m_rgbPalD.pal[idx].rgbRed	= GetRValue(m_crCurDest);
	m_rgbPalD.pal[idx].rgbGreen	= GetGValue(m_crCurDest);
	m_rgbPalD.pal[idx].rgbBlue	= GetBValue(m_crCurDest);
}

//////////////////////////////////////////////////////////////////////////
// Got WM_CTLCOLOR: return handle of background brush to use.
//
HBRUSH CColorSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		// Return handle to our CBrush object
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_STATIC_CURRENT:
			return m_brDest;
			
		case IDC_STATIC_SELECTED:
			return m_brCurDest;
			
		case IDC_STATIC_CURRENTSRC:
			return m_brSrc;
		}
	}
	
	// Normal processing: don't do anything
	return CDialogX::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CColorSelectDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogX::OnSize(nType, cx, cy);

	if (m_wndPreview.GetSafeHwnd())
	{
		m_wndPreview.GetClientRect(m_rcCur);
		SCROLLINFO   info;

		m_wndScrollHor.GetScrollInfo(&info, SIF_ALL);
		info.nMax		= m_hbmp.Width;
		info.nPage		= m_rcCur.Width();
		m_wndScrollHor.SetScrollInfo(&info);
		
		m_wndScrollVer.GetScrollInfo(&info, SIF_ALL);
		info.nMax		= m_hbmp.Height;
		info.nPage		= m_rcCur.Height();
		m_wndScrollVer.SetScrollInfo(&info);

		m_wndPreview.ClientToScreen(m_rcCur);
		ScreenToClient(m_rcCur);
		InvalidateRect(m_rcCur);

		UpdateControlsState();
	}
}

void CColorSelectDlg::UpdatePreview()
{
	CRect rc;

	if (m_wndPreview.GetSafeHwnd())
	{
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);
		InvalidateRect(rc);
	}

	if (m_wndZoom.GetSafeHwnd())
	{
		m_wndZoom.GetWindowRect(rc);
		ScreenToClient(rc);
		InvalidateRect(rc);
	}
}

void CColorSelectDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();
	
	// Get the current position of scroll box.
	m_nScrollVer = pScrollBar->GetScrollPos();
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_TOP:      // Scroll to far left.
		m_nScrollVer = minpos;
		break;
		
	case SB_BOTTOM:      // Scroll to far right.
		m_nScrollVer = maxpos;
		break;
		
	case SB_ENDSCROLL:   // End scroll.
		break;
		
	case SB_LINEUP:      // Scroll left.
		if (m_nScrollVer > minpos)
			m_nScrollVer -= 5;
		break;
		
	case SB_LINEDOWN:   // Scroll right.
		if (m_nScrollVer < maxpos)
			m_nScrollVer += 5;
		break;
		
	case SB_PAGEUP:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (m_nScrollVer > minpos)
				m_nScrollVer = max(minpos, m_nScrollVer - (int) info.nPage);
		}
		break;
		
	case SB_PAGEDOWN:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (m_nScrollVer < maxpos)
				m_nScrollVer = min(maxpos, m_nScrollVer + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		m_nScrollVer = nPos;      // of the scroll box at the end of the drag operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		m_nScrollVer = nPos;     // position that the scroll box has been dragged to.
		break;

	default:
		CDialogX::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	
	// Set the new position of the thumb (scroll box).
	pScrollBar->SetScrollPos(m_nScrollVer);
	UpdatePreview();

//	CDialogX::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CColorSelectDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();
	
	// Get the current position of scroll box.
	m_nScrollHor = pScrollBar->GetScrollPos();
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		m_nScrollHor = minpos;
		break;
		
	case SB_RIGHT:      // Scroll to far right.
		m_nScrollHor = maxpos;
		break;
		
//	case SB_ENDSCROLL:   // End scroll.
//		break;
		
	case SB_LINELEFT:      // Scroll left.
		if (m_nScrollHor > minpos)
			m_nScrollHor -= 5;
		break;
		
	case SB_LINERIGHT:   // Scroll right.
		if (m_nScrollHor < maxpos)
			m_nScrollHor += 5;
		break;
		
	case SB_PAGELEFT:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (m_nScrollHor > minpos)
				m_nScrollHor = max(minpos, m_nScrollHor - (int) info.nPage);
		}
		break;
		
	case SB_PAGERIGHT:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (m_nScrollHor < maxpos)
				m_nScrollHor = min(maxpos, m_nScrollHor + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		m_nScrollHor = nPos;      // of the scroll box at the end of the drag operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		m_nScrollHor = nPos;     // position that the scroll box has been dragged to.
		break;

	default:
		CDialogX::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	
	// Set the new position of the thumb (scroll box).
	pScrollBar->SetScrollPos(m_nScrollHor);
	UpdatePreview();

//	CDialogX::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorSelectDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (IsLoaded())
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);
		rc.DeflateRect(2, 2);
		
		if (PtInRect(&rc, point))
		{
			m_rcPreview.left	= point.x - 2;
			m_rcPreview.right	= point.x + 3;
			m_rcPreview.top		= point.y - 2;
			m_rcPreview.bottom	= point.y + 3;
			
			m_ptPreview = point;
			
			CDC *pDC = GetDC();

			COLORREF color = ::GetPixel(pDC->m_hDC, point.x, point.y);
			
			if (m_crSrc != color)
			{
				m_crSrc = color;
				
				if (m_brSrc)
					DeleteObject(m_brSrc);
				
				m_brSrc = CreateSolidBrush(m_crSrc);
				
				// Current color
				m_sRedSrc		= uInt2Str(GetRValue(m_crSrc));
				m_sGreenSrc		= uInt2Str(GetGValue(m_crSrc));
				m_sBlueSrc		= uInt2Str(GetBValue(m_crSrc));
				
				UpdateData(FALSE);
				m_wndCurColorSrc.Invalidate();
			}
			
			m_bInsideRect = TRUE;
			DrawZoomedPreview();
		}
		else
			m_bInsideRect = FALSE;
	}//if (IsLoaded()
	else
		m_bInsideRect = FALSE;

//	TRACE2("CColorSelectDlg::OnMouseMove X: %d, Y: %d\r\n", point.x, point.y);
	CDialogX::OnMouseMove(nFlags, point);
}

void CColorSelectDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (IsLoaded())
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);
		rc.DeflateRect(2, 2);
		
		if (PtInRect(&rc, point))
		{
			int idx = m_wndColorTableD.m_nSelected;
			
			m_rgbPalD.pal[idx].rgbRed	= GetRValue(m_crSrc);
			m_rgbPalD.pal[idx].rgbBlue	= GetBValue(m_crSrc);
			m_rgbPalD.pal[idx].rgbGreen	= GetGValue(m_crSrc);
			
			m_wndColorTableD.ChangeSelectedColor(m_crSrc);
			m_wndColorTableD.ChangeSelectedIdx(idx+1);
		}
	}
//	TRACE2("CColorSelectDlg::OnLButtonUp X: %d, Y: %d\r\n", point.x, point.y);
	CDialogX::OnLButtonUp(nFlags, point);
}

BOOL CColorSelectDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bInsideRect && m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CDialogX::OnSetCursor(pWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////
// Draw zoomed colors preview
//
void CColorSelectDlg::DrawZoomedPreview()
{
	CDC *pDC = GetDC();
	
	CRect rc;
	m_wndZoom.GetWindowRect(rc);
	ScreenToClient(rc);
	
	rc.right = rc.left + PREV_SIZE;
	rc.bottom = rc.top + PREV_SIZE;
	
	COLORREF color;
	int cx;
	
	for (int cy = 0; cy < 5; cy++)
	{
		for (cx = 0; cx < 5; cx++)
		{
			color = ::GetPixel(pDC->m_hDC, m_rcPreview.left + cx, m_rcPreview.top + cy);
			pDC->FillSolidRect(&rc, color);
			
			if (cx == 2 && cy == 2)
				pDC->Draw3dRect(&rc, RGB(255,0,0), RGB(255,0,0));
			
			rc.OffsetRect(PREV_SIZE, 0);
		}
		
		rc.OffsetRect(-(PREV_SIZE * 5), PREV_SIZE);
		cx = 0;
	}
}
