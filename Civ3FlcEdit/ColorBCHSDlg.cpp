/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ColorBCHSDlg.cpp
	CREATED:	16.11.2004
	VERSION:	1.0

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "ColorBCHSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CELL_SIZE	12

/////////////////////////////////////////////////////////////////////////////
// CColorBCHSDlg dialog

CColorBCHSDlg::CColorBCHSDlg(RGBQUADPAL* pPalD, CWnd* pParent /*=NULL*/)
	: CDialog(CColorBCHSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorBCHSDlg)
	m_nB = 0;
	m_nC = 0;
	m_nH = 0;
	m_bPreview = FALSE;
	m_nS = 0;
	//}}AFX_DATA_INIT

	ASSERT(pPalD);
	m_pPalExtD = pPalD;
}


void CColorBCHSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBCHSDlg)
	DDX_Control(pDX, IDC_SATURATION, m_wndS);
	DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
	DDX_Control(pDX, IDC_HUE, m_wndH);
	DDX_Control(pDX, IDC_CONTRAST, m_wndC);
	DDX_Control(pDX, IDC_SPIN_SATURATION, m_wndSpinS);
	DDX_Control(pDX, IDC_SPIN_HUE, m_wndSpinH);
	DDX_Control(pDX, IDC_SPIN_CONTRAST, m_wndSpinC);
	DDX_Control(pDX, IDC_SPIN_BRIGHT, m_wndSpinB);
	DDX_Control(pDX, IDC_BRIGHT, m_wndB);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_APPLY, m_btnApply);
	DDX_Text(pDX, IDC_BRIGHT, m_nB);
	DDX_Text(pDX, IDC_CONTRAST, m_nC);
	DDX_Text(pDX, IDC_HUE, m_nH);
	DDX_Check(pDX, IDC_PREVIEW_CHECK, m_bPreview);
	DDX_Text(pDX, IDC_SATURATION, m_nS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorBCHSDlg, CDialog)
	//{{AFX_MSG_MAP(CColorBCHSDlg)
	ON_EN_CHANGE(IDC_BRIGHT, OnChangeBright)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BRIGHT, OnDeltaposSpinBright)
	ON_EN_CHANGE(IDC_CONTRAST, OnChangeContrast)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CONTRAST, OnDeltaposSpinContrast)
	ON_EN_CHANGE(IDC_HUE, OnChangeHue)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HUE, OnDeltaposSpinHue)
	ON_EN_CHANGE(IDC_SATURATION, OnChangeSaturation)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SATURATION, OnDeltaposSpinSaturation)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_PREVIEW_CHECK, OnPreviewCheck)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_B, OnReleasedcaptureSliderB)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_C, OnReleasedcaptureSliderC)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_H, OnReleasedcaptureSliderH)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_S, OnReleasedcaptureSliderS)
END_MESSAGE_MAP()

BOOL CColorBCHSDlg::OnInitDialog() 
{
	if (!m_pPalExtD)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	// Inicialize a palettes
	memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);
	
	SetupBitmap();

	if (!m_img.IsLoaded())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	CDialog::OnInitDialog();
	
	LoadPosition(this);
	
	m_btnOK.SetIcon(IDI_OK);
	m_btnCancel.SetIcon(IDI_CANCEL);
	m_btnApply.SetIcon(IDI_APPLY);
	m_btnRefresh.SetIcon(IDI_REFRESH);
	
	m_wndB.LimitText(4);
	m_wndC.LimitText(4);
	m_wndH.LimitText(4);
	m_wndS.LimitText(4);

	m_wndSpinB.SetRange(-1000, 1000);
	m_wndSpinC.SetRange(-1000, 1000);
	m_wndSpinH.SetRange(-360, 360);
	m_wndSpinS.SetRange(-1000, 1000);

	CRect rect;
	CWnd* pWnd;

	// Brightness
	pWnd = GetDlgItem(IDC_BRIGHT_SLIDER_POS);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	if (!m_wndSliderB.Create(WS_VISIBLE | WS_CHILD | TBS_HORZ | 
		TBS_BOTTOM | TBS_TOOLTIPS | TBS_AUTOTICKS,
		rect, this, IDC_SLIDER_B))
	{
		ASSERT(FALSE);
	}
	
	m_wndSliderB.SetRangeMin(-1000);
	m_wndSliderB.SetRangeMax(1000);
	m_wndSliderB.SetTicFreq(100);
	m_wndSliderB.SetPos(0);

	// Contrast
	pWnd = GetDlgItem(IDC_CONTRAST_SLIDER_POS);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	if (!m_wndSliderC.Create(WS_VISIBLE | WS_CHILD | TBS_HORZ | 
		TBS_BOTTOM | TBS_TOOLTIPS | TBS_AUTOTICKS,
		rect, this, IDC_SLIDER_C))
	{
		ASSERT(FALSE);
	}
	
	m_wndSliderC.SetRangeMin(-1000);
	m_wndSliderC.SetRangeMax(1000);
	m_wndSliderC.SetTicFreq(100);
	m_wndSliderC.SetPos(0);

	// Hue
	pWnd = GetDlgItem(IDC_HUE_SLIDER_POS);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	if (!m_wndSliderH.Create(WS_VISIBLE | WS_CHILD | TBS_HORZ | 
		TBS_BOTTOM | TBS_TOOLTIPS | TBS_AUTOTICKS,
		rect, this, IDC_SLIDER_H))
	{
		ASSERT(FALSE);
	}
	
	m_wndSliderH.SetRangeMin(-360);
	m_wndSliderH.SetRangeMax(360);
	m_wndSliderH.SetTicFreq(30);
	m_wndSliderH.SetPos(0);

	// Saturation
	pWnd = GetDlgItem(IDC_SATURATION_SLIDER_POS);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	if (!m_wndSliderS.Create(WS_VISIBLE | WS_CHILD | TBS_HORZ | 
		TBS_BOTTOM | TBS_TOOLTIPS | TBS_AUTOTICKS,
		rect, this, IDC_SLIDER_S))
	{
		ASSERT(FALSE);
	}
	
	m_wndSliderS.SetRangeMin(-1000);
	m_wndSliderS.SetRangeMax(1000);
	m_wndSliderS.SetTicFreq(100);
	m_wndSliderS.SetPos(0);

	m_bPreview = AfxGetApp()->GetProfileInt(RegKey(), REG_PREVIEW, 1);
	
	UpdateData(FALSE);
	UpdateControlsState();
	return TRUE;
}

void CColorBCHSDlg::OnDestroy() 
{
	SavePosition(this);
	AfxGetApp()->WriteProfileInt(RegKey(), REG_PREVIEW, m_bPreview);
	CDialog::OnDestroy();
}

void CColorBCHSDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_wndPreview.GetSafeHwnd() && m_bPreview)
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);

		CRect rcColor(rc);
		rcColor.right	= rc.left + CELL_SIZE;
		rcColor.bottom	= rc.top + CELL_SIZE;

		BYTE nCur = 0;

		for (int idx = 0; idx < COLOR8; idx++, nCur++)
		{
			if (nCur == 16)
			{
				rcColor.left	= rc.left;
				rcColor.right	= rc.left + CELL_SIZE;
				rcColor.top		+= CELL_SIZE;
				rcColor.bottom	+= CELL_SIZE;
				nCur = 0;
			}

			dc.FillSolidRect(&rcColor, GRAPHX::GetPixelColor(&m_imgPrev.m_hbmp, 0, idx));
			
			rcColor.left	+= CELL_SIZE;
			rcColor.right	+= CELL_SIZE;
		}
	}
}

BOOL CColorBCHSDlg::PreTranslateMessage(MSG* pMsg) 
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

void CColorBCHSDlg::UpdateControlsState()
{
	UpdateData();
	UpdatePreview();
}

void CColorBCHSDlg::OnPreviewCheck() 
{
	UpdateControlsState();
}

/////////////////////////////////////////////////////////////////////////////
// CColorBCHSDlg message handlers

void CColorBCHSDlg::OnChangeBright() 
{
	if (m_wndB.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);

		m_wndB.GetWindowText(m_sValue);
		m_sValue.TrimLeft();

		if (m_sValue.IsEmpty())
		{
			m_nB = 0;

			dx.m_bSaveAndValidate = FALSE;
			DDX_Text(&dx, IDC_BRIGHT, m_nB);
		}
		else
		{
			DDX_Text(&dx, IDC_BRIGHT, m_nB);

			if (m_nB > 1000)
			{
				m_nB = 1000;

				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_BRIGHT, m_nB);
			}
			else if (m_nB < -1000)
			{
				m_nB = -1000;

				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_BRIGHT, m_nB);
			}
		}

		m_wndSliderB.SetPos(m_nB);
		ChangeBrigtness();
	}
}

void CColorBCHSDlg::OnDeltaposSpinBright(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeBright();
	*pResult = 0;
}

void CColorBCHSDlg::OnChangeContrast() 
{
	if (m_wndC.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);

		m_wndC.GetWindowText(m_sValue);
		m_sValue.TrimLeft();

		if (m_sValue.IsEmpty())
		{
			m_nC = 0;

			dx.m_bSaveAndValidate = FALSE;
			DDX_Text(&dx, IDC_CONTRAST, m_nC);
		}
		else
		{
			DDX_Text(&dx, IDC_CONTRAST, m_nC);

			if (m_nC > 1000)
			{
				m_nC = 1000;

				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_CONTRAST, m_nC);
			}
			else if (m_nC < -1000)
			{
				m_nC = -1000;

				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_CONTRAST, m_nC);
			}
		}

		m_wndSliderC.SetPos(m_nC);
		ChangeContrast();
	}
}

void CColorBCHSDlg::OnDeltaposSpinContrast(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeContrast();
	*pResult = 0;
}

void CColorBCHSDlg::OnChangeHue() 
{
	if (m_wndH.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);

		m_wndH.GetWindowText(m_sValue);
		m_sValue.TrimLeft();
		
		if (m_sValue.IsEmpty())
		{
			m_nH = 0;

			dx.m_bSaveAndValidate = FALSE;
			DDX_Text(&dx, IDC_HUE, m_nH);
		}
		else
		{
			DDX_Text(&dx, IDC_HUE, m_nH);
			
			if (m_nH > 360)
			{
				m_nH = 360;
				
				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_HUE, m_nH);
			}
			else if (m_nH < -360)
			{
				m_nH = -360;
				
				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_HUE, m_nH);
			}
		}
		
		m_wndSliderH.SetPos(m_nH);
		ChangeHue();
	}
}

void CColorBCHSDlg::OnDeltaposSpinHue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeHue();
	*pResult = 0;
}

void CColorBCHSDlg::OnChangeSaturation() 
{
	if (m_wndS.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);
		
		m_wndS.GetWindowText(m_sValue);
		m_sValue.TrimLeft();
		
		if (m_sValue.IsEmpty())
		{
			m_nS = 0;
			
			dx.m_bSaveAndValidate = FALSE;
			DDX_Text(&dx, IDC_SATURATION, m_nS);
		}
		else
		{
			DDX_Text(&dx, IDC_SATURATION, m_nS);
			
			if (m_nS > 1000)
			{
				m_nS = 1000;
				
				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_SATURATION, m_nS);
			}
			else if (m_nS < -1000)
			{
				m_nS = -1000;
				
				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_SATURATION, m_nS);
			}
		}

		m_wndSliderS.SetPos(m_nS);
		ChangeSaturation();
	}
}

void CColorBCHSDlg::OnDeltaposSpinSaturation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeSaturation();
	*pResult = 0;
}

void CColorBCHSDlg::OnApply() 
{
	// Apply palette changes
	try
	{
		COLORREF color;
		RGBQUAD* pPal;
		
		// Aply palette changes
		for (int idx = 0; idx < COLOR8; idx++)
		{
			color = GRAPHX::GetPixelColor(&m_imgPrev.m_hbmp, 0, idx);
			
			pPal = &m_rgbPalD.pal[idx];
			
			pPal->rgbRed	= GetRValue(color);
			pPal->rgbGreen	= GetGValue(color);
			pPal->rgbBlue	= GetBValue(color);
		}
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorBCHSDlg::OnApply"), MB_ICONERROR);
		return;
	}
	
	memcpy(m_pPalExtD, &m_rgbPalD, RGBQUADPAL_SIZE);
}

void CColorBCHSDlg::OnRefresh() 
{
	// Discard palette changes
	memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);

	SetupBitmap();

	m_wndSliderB.SetPos(0);
	m_wndSliderC.SetPos(0);
	m_wndSliderH.SetPos(0);
	m_wndSliderS.SetPos(0);

	m_nB = 0;
	m_nC = 0;
	m_nH = 0;
	m_nS = 0;

	UpdateData(FALSE);
	UpdatePreview();
}

void CColorBCHSDlg::OnOK() 
{
	OnApply();

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////
// Invalidate preview window.
//
void CColorBCHSDlg::UpdatePreview()
{
	if (m_wndPreview.GetSafeHwnd())
	{
		CRect rc;
		m_wndPreview.GetWindowRect(rc);
		ScreenToClient(rc);
		InvalidateRect(rc);
	}
}

void CColorBCHSDlg::OnReleasedcaptureSliderB(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nB = m_wndSliderB.GetPos();
	
	CDataExchange dx(this, FALSE);
	DDX_Text(&dx, IDC_BRIGHT, m_nB);

	ChangeBrigtness();
	
	*pResult = 0;
}

void CColorBCHSDlg::OnReleasedcaptureSliderC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nC = m_wndSliderC.GetPos();
	
	CDataExchange dx(this, FALSE);
	DDX_Text(&dx, IDC_CONTRAST, m_nC);
	
	ChangeContrast();

	*pResult = 0;
}

void CColorBCHSDlg::OnReleasedcaptureSliderH(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nH = m_wndSliderH.GetPos();
	
	CDataExchange dx(this, FALSE);
	DDX_Text(&dx, IDC_HUE, m_nH);
	
	ChangeHue();

	*pResult = 0;
}

void CColorBCHSDlg::OnReleasedcaptureSliderS(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nS = m_wndSliderS.GetPos();
	
	CDataExchange dx(this, FALSE);
	DDX_Text(&dx, IDC_SATURATION, m_nS);
	
	ChangeSaturation();
	
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////
// Changes the intensity (brightness)
// Valid values are -1000 to +1000
//
void CColorBCHSDlg::ChangeBrigtness()
{
	try
	{
		GRAPHX::CopyBitmap(&m_imgPrev.m_hbmp, &m_img.m_hbmp);
		GRAPHX::ChangeBitmapIntensity(&m_imgPrev.m_hbmp, m_nB);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorBCHSDlg::ChangeBrigtness"), MB_ICONERROR);
		return;
	}

	UpdatePreview();
}

//////////////////////////////////////////////////////////////////
// Increases or decreases the contrast
// Valid values are -1000 to +1000
//
void CColorBCHSDlg::ChangeContrast()
{
	try
	{
		GRAPHX::CopyBitmap(&m_imgPrev.m_hbmp, &m_img.m_hbmp);
		GRAPHX::ChangeBitmapContrast(&m_imgPrev.m_hbmp, m_nC);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorBCHSDlg::ChangeContrast"), MB_ICONERROR);
		return;
	}

	UpdatePreview();
}

//////////////////////////////////////////////////////////////////
// Changes the hue of colors in a palette by rotating the color wheel
// Angular amount to change the hue, in degrees -360 to 360	
//
void CColorBCHSDlg::ChangeHue()
{
	try
	{
		GRAPHX::CopyBitmap(&m_imgPrev.m_hbmp, &m_img.m_hbmp);
		GRAPHX::ChangeBitmapHue(&m_imgPrev.m_hbmp, m_nH);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorBCHSDlg::ChangeHue"), MB_ICONERROR);
		return;
	}

	UpdatePreview();
}

//////////////////////////////////////////////////////////////////
// Increases or decreases the saturation
// Valid values are -1000 to +1000
//
void CColorBCHSDlg::ChangeSaturation()
{
	try
	{
		GRAPHX::CopyBitmap(&m_imgPrev.m_hbmp, &m_img.m_hbmp);
		GRAPHX::ChangeBitmapSaturation(&m_imgPrev.m_hbmp, m_nS);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorBCHSDlg::ChangeSaturation"), MB_ICONERROR);
		return;
	}

	UpdatePreview();
}

void CColorBCHSDlg::SetupBitmap()
{
	try
	{
		m_img.CloseFile();
		m_imgPrev.CloseFile();
		
		GRAPHX::CreateBitmap(&m_img.m_hbmp, TYPE_CONV, 256, 1, 
			24, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);
		
		COLORREF color;
		RGBQUAD* pPal;
		
		// Set palette to bitmap
		for (int idx = 0; idx < COLOR8; idx++)
		{
			pPal = &m_rgbPalD.pal[idx];

			color = RGB(
				pPal->rgbRed,
				pPal->rgbGreen,
				pPal->rgbBlue
				);
			
			GRAPHX::PutPixelColor(&m_img.m_hbmp, 0, idx, color);
		}

		GRAPHX::CopyBitmap(&m_imgPrev.m_hbmp, &m_img.m_hbmp);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		EndDialog(IDCANCEL);
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorBCHSDlg::SetupBitmap"), MB_ICONERROR);
		EndDialog(IDCANCEL);
	}
}
