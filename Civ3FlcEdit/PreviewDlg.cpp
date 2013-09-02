/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	PreviewDlg.cpp
	CREATED:	05.11.2004
	VERSION:	1.2

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the CPreviewDlg class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "PreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_SHOW_LINES	"Show Lines"
#define REG_SHOW_GRID	"Show Grid"

#define PROC_TIMER		51104

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDlg::IDD, pParent), m_Mutex(TRUE, NULL)
{
	//{{AFX_DATA_INIT(CPreviewDlg)
	m_bShowLines = FALSE;
	m_bShowGrid = FALSE;
	//}}AFX_DATA_INIT

	m_pSurface		= NULL;
	m_nXOffset		= 0;
	m_nYOffset		= 0;
	m_nFrmWidth		= 0;
	m_nFrmHeight	= 0;
	m_bFrmResize	= FALSE;
	
	m_pParam		= NULL;
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewDlg)
	DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
	DDX_Check(pDX, IDC_SHOW_LINES, m_bShowLines);
	DDX_Check(pDX, IDC_SHOW_GRID, m_bShowGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CPreviewDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SHOW_LINES, OnShowLines)
	ON_BN_CLICKED(IDC_SHOW_GRID, OnShowGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg message handlers

BOOL CPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ShowWindow(SW_HIDE);
//	::ShowWindow(this->m_hWnd, SW_HIDE);

	if (!m_pParam)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_Mutex.Lock();

	LoadPosition(this);
	
	m_pSurface		= m_pParam->pSurface;
	m_nXOffset		= m_pParam->nXOffset;
	m_nYOffset		= m_pParam->nYOffset;
	m_nFrmWidth		= m_pParam->nFrmWidth;
	m_nFrmHeight	= m_pParam->nFrmHeight;
	m_nFrmWidthCur	= m_pParam->nFrmWidthCur;
	m_nFrmHeightCur	= m_pParam->nFrmHeightCur;
	m_bFrmResize	= m_pParam->bFrmResize;
	
	m_bResizeEnable	= m_pParam->bResizeEnable;
	m_nResizeTop	= m_pParam->nResizeTop;
	m_nResizeBottom	= m_pParam->nResizeBottom;
	m_nResizeLeft	= m_pParam->nResizeLeft;
	m_nResizeRight	= m_pParam->nResizeRight;
	m_colTransp		= m_pParam->colTransp;

	try
	{
		CloseFile();
		
		if (m_pSurface)
		{
			GRAPHX::CreateBitmap(&m_hbmp, TYPE_CONV, m_nFrmWidthCur, m_nFrmHeightCur, 
				24, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);

			// Get dc where to draw
			CDC*	pDC		= CDC::FromHandle(GRAPHX::CreateGraphXDC(&m_hbmp));
			HDC		hSurfDC = NULL;
			HDC		hDC		= pDC->m_hDC;
			
			LPDIRECTDRAWSURFACE lpDDs;

			// Get current surface
			lpDDs = m_pSurface->GetDDS();

//			DDCOLORKEY ddck;
//			lpDDs->GetColorKey(DDCKEY_SRCBLT, &ddck);
//			m_colTransp = ddck.dwColorSpaceLowValue;

			// Creates a GDI-compatible device context of the surface
			if (FAILED(lpDDs->GetDC(&hSurfDC)))
				return FALSE;

			// Copy whole picture to the temp HDC
			BitBlt(hDC, 0, 0, m_nFrmWidthCur, m_nFrmHeightCur, 
				hSurfDC, 0, 0, SRCCOPY);

			// Releases the hDC previously obtained with GetDC
			if (FAILED(lpDDs->ReleaseDC(hSurfDC)))
				return FALSE;

			// Release DC
			GRAPHX::DeleteGraphXDC(*pDC);
		}
		else
		{
			GRAPHX::CreateBitmap(&m_hbmp, TYPE_CONV, WND_W, WND_H,
				24, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);
			GRAPHX::FillBitmap(&m_hbmp, RGB(255,0,255));
		}
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		EndDialog(IDCANCEL);
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		EndDialog(IDCANCEL);
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CPreviewDlg::OnInitDialog"), MB_ICONERROR);
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_bShowLines = AfxGetApp()->GetProfileInt(RegKey(), REG_SHOW_LINES, 1);
	m_bShowGrid  = AfxGetApp()->GetProfileInt(RegKey(), REG_SHOW_GRID, 1);

	m_wndPreview.SetWindowPos(NULL, 0, 0, WND_CX, WND_CY,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	ShowPreviewWindow(m_pParam->bShow);
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SetTimer(PROC_TIMER, 100, NULL); // start the process timer

	UpdateData(FALSE);
	UpdatePreview();

	m_Mutex.Unlock();
	return TRUE;
}

void CPreviewDlg::OnDestroy() 
{
	m_Mutex.Lock();

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(RegKey(), REG_SHOW_LINES, m_bShowLines);
	pApp->WriteProfileInt(RegKey(), REG_SHOW_GRID, m_bShowGrid);

	SavePosition(this);
	KillTimer(PROC_TIMER);

	m_Mutex.Unlock();

	CDialog::OnDestroy();
}

void CPreviewDlg::OnShowLines() 
{
	m_Mutex.Lock();

	UpdateData();
	UpdatePreview();

	m_Mutex.Unlock();
}

void CPreviewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_wndPreview.GetSafeHwnd() && IsWindowVisible())
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);

		if (IsLoaded())
		{
			rc.DeflateRect(1, 1);
			CRect rcDest(rc);
			rcDest.left		= rcDest.left	+ m_nXOffset;
			rcDest.top		= rcDest.top	+ m_nYOffset;
			rcDest.right	= rcDest.left	+ m_nFrmWidth;
			rcDest.bottom	= rcDest.top	+ m_nFrmHeight;

			if (m_bFrmResize)
				GRAPHX::PaintDC(dc.m_hDC, &m_hbmp, CRect(0,0,m_nFrmWidthCur,m_nFrmHeightCur), NULL, rcDest, rc, SRCCOPY);//Render(&dc, rcDest, CGraphXImage::PaintProportion2);
			else
			{
				dc.FillSolidRect(&rcDest, m_colTransp);
				GRAPHX::PaintDC(dc.m_hDC, &m_hbmp, CRect(0,0,m_nFrmWidth,m_nFrmHeight), NULL, rcDest, rc, SRCCOPY);	
			}

			if (m_bResizeEnable)
			{
				CRect rcResize;
				rcResize.left	= rcDest.left	+ m_nResizeLeft;
				rcResize.top	= rcDest.top	+ m_nResizeTop;
				rcResize.right	= rcDest.left	+ m_nResizeRight;
				rcResize.bottom	= rcDest.top	+ m_nResizeBottom;

//				dc.DrawFocusRect(&rcResize);

				dc.MoveTo(rcResize.left, rcResize.top);
				dc.LineTo(rcResize.right, rcResize.top);
				dc.LineTo(rcResize.right, rcResize.bottom);
				dc.LineTo(rcResize.left, rcResize.bottom);
				dc.LineTo(rcResize.left, rcResize.top);

//				dc.SetBkMode(TRANSPARENT);
//				dc.Rectangle(&rcResize);
				
			}//if (m_bResizeEnable)
			
			rc.InflateRect(1, 1);
		}//if (IsLoaded())

		CPen* pOldPen;
		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;

		int cx0 = rc.left + rc.Width()  / 2;
		int cy0 = rc.top  + rc.Height() / 2;

		//////////////////////////////////////////////////////////////////////////
		// Draw Lines
		if (m_bShowLines)
		{
			rc.DeflateRect(1, 1);
			
			lb.lbColor = RGB(0, 0, 0);
			CPen pen(PS_SOLID | PS_ENDCAP_ROUND | PS_GEOMETRIC, 1, &lb);
			pOldPen = dc.SelectObject(&pen);
			
			dc.MoveTo(rc.left, rc.top);
			dc.LineTo(rc.right, rc.bottom);
			
			dc.MoveTo(rc.right, rc.top);
			dc.LineTo(rc.left, rc.bottom);
			
			dc.MoveTo(cx0, rc.top);
			dc.LineTo(cx0, rc.bottom);
			
			dc.MoveTo(rc.left, cy0);
			dc.LineTo(rc.right, cy0);
			
			// Return old pen
			dc.SelectObject(pOldPen);

			rc.InflateRect(1, 1);
		}//if (m_bShowLines)
		
		//////////////////////////////////////////////////////////////////////////
		// Draw grid
		if (m_bShowGrid)
		{
			lb.lbColor = RGB(255, 0, 0);
			CPen penRed(PS_SOLID | PS_ENDCAP_ROUND | PS_GEOMETRIC, 1, &lb);
			pOldPen = dc.SelectObject(&penRed);
			
			dc.MoveTo(cx0, cy0 - 48);
			dc.LineTo(cx0 - 64, cy0 - 16);
			dc.LineTo(cx0, cy0 + 16);
			dc.LineTo(cx0 + 64, cy0 - 16);
			dc.LineTo(cx0, cy0 - 48);
			
			dc.MoveTo(cx0 - 32,	cy0);
			dc.LineTo(cx0 + 32,	cy0 - 32);
			
			dc.MoveTo(cx0 + 32,	cy0);
			dc.LineTo(cx0 - 32,	cy0 - 32);
			
			// Return old pen
			dc.SelectObject(pOldPen);
		}//if (m_bShowGrid)

		dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		
	}//if (m_wndPreview.GetSafeHwnd() && IsWindowVisible())
}

BOOL CPreviewDlg::PreTranslateMessage(MSG* pMsg) 
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

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// Invalidate preview window.
//
void CPreviewDlg::UpdatePreview()
{
	if (m_wndPreview.GetSafeHwnd())
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);
		InvalidateRect(rc);
	}
}

void CPreviewDlg::ShowPreviewWindow(BOOL bShow)
{
	ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

void CPreviewDlg::OnTimer(UINT nIDEvent) 
{
	// Check if it is our timer event
	if (nIDEvent == PROC_TIMER)
	{
		m_Mutex.Lock();

		// Pointer to a structure with parameters should be set
		if (m_pParam != NULL)
		{
			if (m_pParam->bExit)
				EndDialog(IDCANCEL);
			else
			{
				BOOL bChanged = FALSE;
				if (m_nFrmWidth != m_pParam->nFrmWidth)
				{
					m_nFrmWidth = m_pParam->nFrmWidth;
					bChanged = TRUE;
				}

				if (m_nFrmHeight != m_pParam->nFrmHeight)
				{
					m_nFrmHeight = m_pParam->nFrmHeight;
					bChanged = TRUE;
				}

				if (m_nXOffset != m_pParam->nXOffset)
				{
					m_nXOffset = m_pParam->nXOffset;
					bChanged = TRUE;
				}

				if (m_nYOffset != m_pParam->nYOffset)
				{
					m_nYOffset = m_pParam->nYOffset;
					bChanged = TRUE;
				}
				
				if (m_bFrmResize != m_pParam->bFrmResize)
				{
					m_bFrmResize = m_pParam->bFrmResize;
					bChanged = TRUE;
				}

				if (m_bResizeEnable != m_pParam->bResizeEnable)
				{
					m_bResizeEnable = m_pParam->bResizeEnable;
					bChanged = TRUE;
				}

				if (m_bResizeEnable)
				{
					if (m_nResizeTop != m_pParam->nResizeTop)
					{
						m_nResizeTop = m_pParam->nResizeTop;
						bChanged = TRUE;
					}

					if (m_nResizeBottom != m_pParam->nResizeBottom)
					{
						m_nResizeBottom = m_pParam->nResizeBottom;
						bChanged = TRUE;
					}

					if (m_nResizeLeft != m_pParam->nResizeLeft)
					{
						m_nResizeLeft = m_pParam->nResizeLeft;
						bChanged = TRUE;
					}

					if (m_nResizeRight != m_pParam->nResizeRight)
					{
						m_nResizeRight = m_pParam->nResizeRight;
						bChanged = TRUE;
					}
				}
				
				if (IsWindowVisible() != m_pParam->bShow)
					ShowPreviewWindow(m_pParam->bShow);

				if (bChanged)
					UpdatePreview();
			}
		}//if (m_pParam != NULL)
		else
			EndDialog(IDCANCEL);
		
		m_Mutex.Unlock();
		return;
	}//if (nIDEvent == PROC_TIMER)
	
	CDialog::OnTimer(nIDEvent);
}

void CPreviewDlg::SetParam(PREVIEW_PARAM * pParam)
{
	m_Mutex.Lock();
	m_pParam = pParam;
	m_Mutex.Unlock();
}

void CPreviewDlg::OnCancel() 
{
	CDialog::OnCancel();
	if (m_pParam && ::IsWindow(m_pParam->hWnd))
		::SendMessage(m_pParam->hWnd, PREVIEW_CANCEL, NULL, NULL);
}

void CPreviewDlg::OnShowGrid() 
{
	m_Mutex.Lock();
	
	UpdateData();
	UpdatePreview();
	
	m_Mutex.Unlock();
}
