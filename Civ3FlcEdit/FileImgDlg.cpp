/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FileImgDlg.cpp
	CREATED:	12.03.2003
	MODIFIED:	16.07.2004
	AUTHOR:		Dreyk
	VERSION:	3.0

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FileImgDlg.h"

#include "FlcImage.h"

#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SIZE_MAX	9999
#define REG_BITS_PER_PIXELS		"Last File BpP"

/////////////////////////////////////////////////////////////////////////////
// CFileImgDlg

IMPLEMENT_DYNAMIC(CFileImgDlg, CFileDialogX)

CFileImgDlg::CFileImgDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialogX(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	//{{AFX_DATA_INIT(CFileImgDlg)
	m_nHeight = 1;
	m_nWidth = 1;
	m_bLock = TRUE;
	//}}AFX_DATA_INIT

	m_pImg = NULL;
}

void CFileImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileImgDlg)
	DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
	DDX_Control(pDX, IDC_BITS_PER_PIXEL, m_wndBitsPerPixel);
	DDX_Control(pDX, IDC_LOCK, m_btnLock);
	DDX_Control(pDX, IDC_FRM_WIDTH_SPIN, m_wndWidthSpin);
	DDX_Control(pDX, IDC_FRM_WIDTH, m_wndWidth);
	DDX_Control(pDX, IDC_FRM_HEIGHT_SPIN, m_wndHeightSpin);
	DDX_Control(pDX, IDC_FRM_HEIGHT, m_wndHeight);
	DDX_Text(pDX, IDC_FRM_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_FRM_WIDTH, m_nWidth);
	DDX_Check(pDX, IDC_LOCK, m_bLock);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileImgDlg, CFileDialogX)
	//{{AFX_MSG_MAP(CFileImgDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_FRM_HEIGHT, OnChangeHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_HEIGHT_SPIN, OnDeltaposHeightSpin)
	ON_EN_CHANGE(IDC_FRM_WIDTH, OnChangeWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_WIDTH_SPIN, OnDeltaposWidthSpin)
	ON_BN_CLICKED(IDC_LOCK, OnLock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFileImgDlg::OnInitDialog() 
{
	if (!m_pImg || !m_pImg->IsLoaded())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if (!CFileDialogX::OnInitDialog())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	int nBitsPerPixels	= AfxGetApp()->GetProfileInt(REG_IMG_DLG, REG_BITS_PER_PIXELS, 0);
	m_wndBitsPerPixel.AddString(uLoadString(IDS_COLOR_256));
	m_wndBitsPerPixel.AddString(uLoadString(IDS_COLOR_TRUE));
	m_wndBitsPerPixel.SetCurSel(nBitsPerPixels);

	m_nBitsPerPixel = cm_256Colors;
	
	m_wndWidth.LimitText(4);
	m_wndHeight.LimitText(4);

	m_wndWidthSpin.SetRange(1, 9999);
	m_wndHeightSpin.SetRange(1, 9999);
	
	m_nFileType		= FILE_PCX;
	m_nWidth		= m_pImg->m_hbmp.Width;			// default width in pixels
	m_nHeight		= m_pImg->m_hbmp.Height;		// default height in pixels

	m_nPropXtoY		= (float)m_nWidth / (float)m_nHeight;
	m_nPropYtoX		= (float)m_nHeight / (float)m_nWidth;

	m_btnLock.SetIcon(IDI_LOCK, IDI_UNLOCK);
	m_btnLock.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
//	m_btnLock.SetTooltipText(IDS_CONSTRAIN_PROP);

	UpdateData(FALSE);
	return TRUE;
}

void CFileImgDlg::OnSize(UINT nType, int cx, int cy) 
{
	CFileDialogX::OnSize(nType, cx, cy);

	Invalidate();
}

void CFileImgDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_wndPreview.GetSafeHwnd())
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);
		dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		
		if (m_pImg->IsLoaded())
		{
			rc.DeflateRect(3, 3);
			m_pImg->Render(&dc, rc, CGraphXImage::PaintProportion2);
		}
	}
}

BOOL CFileImgDlg::OnFileNameOK()
{
	if (!m_pImg->IsLoaded())
	{
		AfxMessageBox(uFormatString(IDS_IMG_UNKNOWN_FORMAT, GetPathName()));
		return TRUE;
	}
	
	UpdateData();

	int nSel = m_wndBitsPerPixel.GetCurSel();
	switch (nSel)
	{
	case 1:
		m_nBitsPerPixel = cm_TrueColor;
		break;
	default:
		m_nBitsPerPixel = cm_256Colors;
	}

	switch (m_ofn.nFilterIndex)
	{
	case 2:
		m_nFileType = FILE_BMP;
		m_sExt		= ".bmp";
		break;

	case 3:
		m_nFileType = FILE_LEAD1JFIF;
		m_sExt		= ".jpg";
		m_nBitsPerPixel = cm_TrueColor;
		break;

	default:
		m_nFileType = FILE_PCX;
		m_sExt		= ".pcx";
	}

//ZSoft PCX (.pcx)|*.pcx|Windows BMP (.bmp)|*.bmp|JPEG File Interchange Format (.jpg)|*.jpg|CompuServe GIF (.gif)|*.gif||
	
	m_sFilePath				= GetPathName();
	m_ofnEx.lpstrFile		= (LPSTR)(LPCTSTR)m_sFilePath;

	return CFileDialogX::OnFileNameOK();
}

///////////////////////////////////////////////////////////////////
// Return name of the given color mode.
//
CString CFileImgDlg::GetColorModeName(int nBitsPerPixel)
{
	CString sColorMode;
	switch (m_nBitsPerPixel)
	{
	    case cm_256Colors:  sColorMode = uLoadString(IDS_COLOR_256);  break;
		case cm_TrueColor:  sColorMode = uLoadString(IDS_COLOR_TRUE); break;
	    default: sColorMode = uLoadString(IDS_COLOR_256);
	}

	return sColorMode;
}

void CFileImgDlg::OnDestroy() 
{
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(REG_IMG_DLG, REG_FILTER, m_ofn.nFilterIndex);
	pApp->WriteProfileInt(REG_IMG_DLG, REG_BITS_PER_PIXELS, m_wndBitsPerPixel.GetCurSel());
	
	CFileDialogX::OnDestroy();
}

void CFileImgDlg::OnLock() 
{
	UpdateData();
	
	if (m_nPropXtoY > m_nPropYtoX)
		OnChangeHeight();
	else if (m_nPropYtoX > m_nPropXtoY)
		OnChangeWidth();
}

void CFileImgDlg::OnChangeHeight() 
{
	if (m_wndHeight.GetSafeHwnd() && m_bLock)
	{
		UpdateData();
		
		CString res;
		res.Format("%.0f", (float)m_nHeight * m_nPropXtoY);
		m_nWidth = atoi(res);
		
		if (m_nWidth > SIZE_MAX)
		{
			m_nWidth = SIZE_MAX;
			UpdateData(FALSE);
			OnChangeWidth();
		}
		else
			UpdateData(FALSE);
	}
}

void CFileImgDlg::OnDeltaposHeightSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeHeight();
	*pResult = 0;
}

void CFileImgDlg::OnChangeWidth()
{
	if (m_wndWidth.GetSafeHwnd() && m_bLock)
	{
		UpdateData();
		
		CString res;
		res.Format("%.0f", (float)m_nWidth * m_nPropYtoX);
		m_nHeight = atoi(res);
		
		if (m_nHeight > SIZE_MAX)
		{
			m_nHeight = SIZE_MAX;
			UpdateData(FALSE);
			OnChangeHeight();
		}
		else
			UpdateData(FALSE);
	}
}

void CFileImgDlg::OnDeltaposWidthSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeWidth();
	*pResult = 0;
}