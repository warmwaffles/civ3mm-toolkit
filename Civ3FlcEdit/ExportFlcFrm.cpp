/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ExportFlcFrm.cpp
	CREATED:	02.7.2004
	VERSION:	2.2

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ExportFlcFrm.h"

#include "ExportFlcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportFlcFrm dialog


CExportFlcFrm::CExportFlcFrm(CWnd* pParent /*=NULL*/)
	: CDialogX(CExportFlcFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportFlcFrm)
	m_nFrmCount = 1;
	m_nFrmHeight = 1;
	m_nFrmWidth = 1;
	m_nCountType = 0;
	m_nSizeType = 0;
	m_nDelayType = 0;
	m_sDelayCur = _T("");
	m_sFrmCountCur = _T("");
	m_sFrmSizeCur = _T("");
	m_bFrmResize = FALSE;
	m_bLock = TRUE;
	m_nXOffset = 0;
	m_nOffsetType = 0;
	m_nYOffset = 0;
	m_sFrmOffsetCur = _T("");
	m_sDelay = _T("");
	m_bResizeEnable = FALSE;
	m_nResizeLeft = 0;
	m_nResizeRight = 0;
	m_nResizeTop = 0;
	m_nResizeBottom = 0;
	//}}AFX_DATA_INIT
	
	SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_2, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_3, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_4, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_5, CF_RESIZE_BOTH);

	m_bDrawGripper	= FALSE;
	m_pParent		= (CExportFlcDlg*) pParent;
	m_bChanged		= FALSE;
}


void CExportFlcFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportFlcFrm)
	DDX_Control(pDX, IDC_RESIZE_ENABLE, m_wndResizeEnable);
	DDX_Control(pDX, IDC_RESIZE_BOTTOM_SPIN, m_wndResizeBottomSpin);
	DDX_Control(pDX, IDC_RESIZE_BOTTOM, m_wndResizeBottom);
	DDX_Control(pDX, IDC_RESIZE_TOP_SPIN, m_wndResizeTopSpin);
	DDX_Control(pDX, IDC_RESIZE_TOP, m_wndResizeTop);
	DDX_Control(pDX, IDC_RESIZE_RIGHT_SPIN, m_wndResizeRightSpin);
	DDX_Control(pDX, IDC_RESIZE_RIGHT, m_wndResizeRight);
	DDX_Control(pDX, IDC_RESIZE_LEFT_SPIN, m_wndResizeLeftSpin);
	DDX_Control(pDX, IDC_RESIZE_LEFT, m_wndResizeLeft);
	DDX_Control(pDX, IDC_SPIN_DELAY, m_wndDelaySpin);
	DDX_Control(pDX, IDC_DELAY, m_wndDelay);
	DDX_Control(pDX, IDC_FRM_TOP_SPIN, m_wndYOffsetSpin);
	DDX_Control(pDX, IDC_FRM_TOP, m_wndYOffset);
	DDX_Control(pDX, IDC_FRM_LEFT, m_wndXOffset);
	DDX_Control(pDX, IDC_FRM_LEFT_SPIN, m_wndXOffsetSpin);
	DDX_Control(pDX, IDC_LOCK, m_btnLock);
	DDX_Control(pDX, IDC_FRM_RESIZE, m_wndFrmResize);
	DDX_Control(pDX, IDC_FRM_COUNT_SPIN, m_wndFrmCountSpin);
	DDX_Control(pDX, IDC_FRM_WIDTH_SPIN, m_wndFrmWidthSpin);
	DDX_Control(pDX, IDC_FRM_WIDTH, m_wndFrmWidth);
	DDX_Control(pDX, IDC_FRM_HEIGHT_SPIN, m_wndFrmHeightSpin);
	DDX_Control(pDX, IDC_FRM_HEIGHT, m_wndFrmHeight);
	DDX_Control(pDX, IDC_FRM_COUNT, m_wndFrmCount);
	DDX_Text(pDX, IDC_FRM_COUNT, m_nFrmCount);
	DDX_Text(pDX, IDC_FRM_HEIGHT, m_nFrmHeight);
	DDX_Text(pDX, IDC_FRM_WIDTH, m_nFrmWidth);
	DDX_Radio(pDX, IDC_FRM_COUNT_CUR, m_nCountType);
	DDX_Radio(pDX, IDC_FRM_SIZE_CUR, m_nSizeType);
	DDX_Radio(pDX, IDC_DELAY_CUR, m_nDelayType);
	DDX_Text(pDX, IDC_STATIC_DELAY_CUR, m_sDelayCur);
	DDX_Text(pDX, IDC_STATIC_FRM_COUNT_CUR, m_sFrmCountCur);
	DDX_Text(pDX, IDC_STATIC_FRM_SIZE_CUR, m_sFrmSizeCur);
	DDX_Check(pDX, IDC_FRM_RESIZE, m_bFrmResize);
	DDX_Check(pDX, IDC_LOCK, m_bLock);
	DDX_Text(pDX, IDC_FRM_LEFT, m_nXOffset);
	DDX_Radio(pDX, IDC_FRM_OFFSET_CUR, m_nOffsetType);
	DDX_Text(pDX, IDC_FRM_TOP, m_nYOffset);
	DDX_Text(pDX, IDC_STATIC_FRM_OFFSET, m_sFrmOffsetCur);
	DDX_Text(pDX, IDC_DELAY, m_sDelay);
	DDX_Check(pDX, IDC_RESIZE_ENABLE, m_bResizeEnable);
	DDX_Text(pDX, IDC_RESIZE_LEFT, m_nResizeLeft);
	DDX_Text(pDX, IDC_RESIZE_RIGHT, m_nResizeRight);
	DDX_Text(pDX, IDC_RESIZE_TOP, m_nResizeTop);
	DDX_Text(pDX, IDC_RESIZE_BOTTOM, m_nResizeBottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportFlcFrm, CDialogX)
	//{{AFX_MSG_MAP(CExportFlcFrm)
	ON_BN_CLICKED(IDC_FRM_COUNT_CUR, OnFrmCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_COUNT_SPIN, OnDeltaposFrmCountSpin)
	ON_EN_CHANGE(IDC_FRM_COUNT, OnChangeFrmCount)
	ON_EN_CHANGE(IDC_FRM_HEIGHT, OnChangeFrmHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_HEIGHT_SPIN, OnDeltaposFrmHeightSpin)
	ON_BN_CLICKED(IDC_FRM_SIZE_CUR, OnFrmSize)
	ON_EN_CHANGE(IDC_FRM_WIDTH, OnChangeFrmWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_WIDTH_SPIN, OnDeltaposFrmWidthSpin)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TIMER_SLIDER, OnReleasedcaptureTimerSlider)
	ON_BN_CLICKED(IDC_DELAY_CUR, OnDelay)
	ON_BN_CLICKED(IDC_FRM_RESIZE, OnFrmResize)
	ON_BN_CLICKED(IDC_LOCK, OnLock)
	ON_BN_CLICKED(IDC_FRM_OFFSET_CUR, OnFrmOffset)
	ON_EN_CHANGE(IDC_FRM_TOP, OnChangeFrmTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_TOP_SPIN, OnDeltaposFrmTopSpin)
	ON_EN_CHANGE(IDC_FRM_LEFT, OnChangeFrmLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_LEFT_SPIN, OnDeltaposFrmLeftSpin)
	ON_EN_CHANGE(IDC_DELAY, OnChangeDelay)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, OnDeltaposSpinDelay)
	ON_EN_CHANGE(IDC_RESIZE_BOTTOM, OnChangeResizeBottom)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RESIZE_BOTTOM_SPIN, OnDeltaposResizeBottomSpin)
	ON_EN_CHANGE(IDC_RESIZE_LEFT, OnChangeResizeLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RESIZE_LEFT_SPIN, OnDeltaposResizeLeftSpin)
	ON_EN_CHANGE(IDC_RESIZE_RIGHT, OnChangeResizeRight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RESIZE_RIGHT_SPIN, OnDeltaposResizeRightSpin)
	ON_EN_CHANGE(IDC_RESIZE_TOP, OnChangeResizeTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RESIZE_TOP_SPIN, OnDeltaposResizeTopSpin)
	ON_BN_CLICKED(IDC_RESIZE_ENABLE, OnResizeEnable)
	ON_BN_CLICKED(IDC_FRM_COUNT_CHANGE, OnFrmCount)
	ON_BN_CLICKED(IDC_FRM_SIZE_CHANGE, OnFrmSize)
	ON_BN_CLICKED(IDC_DELAY_CHANGE, OnDelay)
	ON_BN_CLICKED(IDC_FRM_OFFSET_CHANGE, OnFrmOffset)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportFlcFrm message handlers

BOOL CExportFlcFrm::OnInitDialog() 
{
	CDialogX::OnInitDialog();

	m_wndFrmWidth.LimitText(3);
	m_wndFrmHeight.LimitText(3);
	m_wndFrmCount.LimitText(3);
	
	m_wndFrmCountSpin.SetRange(1, MAX_FRM_COUNT);
	m_wndFrmWidthSpin.SetRange(1, WND_W);
	m_wndFrmHeightSpin.SetRange(1, WND_H);

	m_wndResizeBottomSpin.SetRange(0, WND_W);
	m_wndResizeBottom.LimitText(3);
	m_wndResizeTopSpin.SetRange(0, WND_W);
	m_wndResizeTop.LimitText(3);
	m_wndResizeRightSpin.SetRange(0, WND_W);
	m_wndResizeRight.LimitText(3);
	m_wndResizeLeftSpin.SetRange(0, WND_W);
	m_wndResizeLeft.LimitText(3);

	m_btnLock.SetIcon(IDI_LOCK, IDI_UNLOCK);
	m_btnLock.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLock.SetTooltipText(IDS_CONSTRAIN_PROP);

	// Create timer slider control
	CWnd* pWnd = GetDlgItem(IDC_TIMER_SLIDER_POS);
	
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	// Initialise controls
	if (!m_wndTimerSlider.Create(WS_VISIBLE | WS_CHILD | TBS_HORZ | TBS_BOTTOM | TBS_AUTOTICKS | TBS_TOOLTIPS,
		rect, this, IDC_TIMER_SLIDER))
	{
		ASSERT(FALSE);
	}
	
	m_wndTimerSlider.SetRangeMin(FRM_DELAY_MIN);
	m_wndTimerSlider.SetRangeMax(FRM_DELAY_MAX);
	m_wndTimerSlider.SetTicFreq(100);

	m_wndDelay.SetLimitText(4);
	m_wndDelaySpin.SetRange(FRM_DELAY_MIN, FRM_DELAY_MAX);

	m_nFrmCountCur	= m_pParent->m_nFrmCount;
	m_nFrmWidthCur	= m_pParent->m_nFrmWidth;
	m_nFrmHeightCur	= m_pParent->m_nFrmHeight;
	m_bFrmResize	= m_pParent->m_bFrmResize;
	m_nDelayCur		= m_pParent->m_nDelay;
	m_sDelayCur		= uInt2Str(m_nDelayCur) + " ms";  
	m_sFrmCountCur	= uInt2Str(m_nFrmCountCur);
	m_sFrmSizeCur.Format("%d x %d pixels", m_nFrmWidthCur, m_nFrmHeightCur);
	m_nXOffsetCur	= m_pParent->m_nXOffset;
	m_nYOffsetCur	= m_pParent->m_nYOffset;

	if (m_pParent->m_bSaveSettings)
	{
		CWinApp* pApp = AfxGetApp();

		m_nDelay		= pApp->GetProfileInt(REG_EXP_FLC, REG_DELAY, m_pParent->m_nDelay);
		m_nFrmCount		= pApp->GetProfileInt(REG_EXP_FLC, REG_FRM_COUNT, m_pParent->m_nFrmCount);
		m_nFrmWidth		= pApp->GetProfileInt(REG_EXP_FLC, REG_FRM_W, m_pParent->m_nFrmWidth);
		m_nFrmHeight	= pApp->GetProfileInt(REG_EXP_FLC, REG_FRM_H, m_pParent->m_nFrmHeight);
		m_nXOffset		= pApp->GetProfileInt(REG_EXP_FLC, REG_OFFSET_X, m_pParent->m_nXOffset);
		m_nYOffset		= pApp->GetProfileInt(REG_EXP_FLC, REG_OFFSET_Y, m_pParent->m_nYOffset);

		m_nResizeTop	= pApp->GetProfileInt(REG_EXP_FLC, REG_TRIM_TOP, 0);
		m_nResizeBottom	= pApp->GetProfileInt(REG_EXP_FLC, REG_TRIM_BOTTOM, m_nFrmHeightCur);
		m_nResizeLeft	= pApp->GetProfileInt(REG_EXP_FLC, REG_TRIM_LEFT, 0);
		m_nResizeRight	= pApp->GetProfileInt(REG_EXP_FLC, REG_TRIM_RIGHT, m_nFrmWidthCur);

		if (!m_pParent->m_bLeaderHead)
		{
			m_bResizeEnable	= pApp->GetProfileInt(REG_EXP_FLC, REG_USE_TRIM, 0);
			m_nSizeType		= pApp->GetProfileInt(REG_EXP_FLC, REG_USE_SIZE, 0);
			m_nOffsetType	= pApp->GetProfileInt(REG_EXP_FLC, REG_USE_OFFSET, 0);
		}

		m_nCountType	= pApp->GetProfileInt(REG_EXP_FLC, REG_USE_FRM_COUNT, 0);
		m_nDelayType	= pApp->GetProfileInt(REG_EXP_FLC, REG_USE_DELAY, 0);

		m_bLock			= pApp->GetProfileInt(REG_EXP_FLC, REG_LOCK, 1);

		UpdatePreview();
	}
	else
	{
		m_nDelay		= m_pParent->m_nDelay;
		m_nFrmCount		= m_pParent->m_nFrmCount;
		m_nFrmWidth		= m_pParent->m_nFrmWidth;
		m_nFrmHeight	= m_pParent->m_nFrmHeight;
		m_nXOffset		= m_pParent->m_nXOffset;
		m_nYOffset		= m_pParent->m_nYOffset;

		m_nResizeTop	= 0;
		m_nResizeBottom	= m_nFrmHeightCur;
		m_nResizeLeft	= 0;
		m_nResizeRight	= m_nFrmWidthCur;
	}

	if (m_pParent->m_bLeaderHead)
		m_wndResizeEnable.EnableWindow(FALSE);

	m_sDelay = uInt2Str(m_nDelay);

	m_nPropXtoY		= (float)m_nFrmWidthCur / (float)m_nFrmHeightCur;
	m_nPropYtoX		= (float)m_nFrmHeightCur / (float)m_nFrmWidthCur;

	m_wndYOffsetSpin.SetRange(0, WND_H-1);
	m_wndXOffsetSpin.SetRange(0, WND_W-1);
	m_wndXOffset.LimitText(3);
	m_wndYOffset.LimitText(3);
	m_sFrmOffsetCur.Format("Left: %d, Top: %d pixels", m_nXOffsetCur, m_nYOffsetCur);

	GetDlgItem(IDC_FRM_SIZE_CHANGE)->EnableWindow(!m_pParent->m_bLeaderHead);
	GetDlgItem(IDC_FRM_OFFSET_CHANGE)->EnableWindow(!m_pParent->m_bLeaderHead);
	
	UpdateData(FALSE);
	m_wndTimerSlider.SetPos(m_nDelay);
	UpdateControlsState();
	return TRUE;
}

void CExportFlcFrm::UpdateControlsState()
{
	UpdateData();

	BOOL bEnable = (m_nSizeType == 1);
	m_wndFrmWidth.EnableWindow(bEnable);
	m_wndFrmWidthSpin.EnableWindow(bEnable);
	m_wndFrmHeight.EnableWindow(bEnable);
	m_wndFrmHeightSpin.EnableWindow(bEnable);
	m_wndFrmResize.EnableWindow(bEnable);
	m_btnLock.EnableWindow(bEnable);

	bEnable = (m_nCountType == 1);
	m_wndFrmCount.EnableWindow(bEnable);
	m_wndFrmCountSpin.EnableWindow(bEnable);
	
	bEnable = (m_nOffsetType == 1);
	m_wndYOffsetSpin.EnableWindow(bEnable);
	m_wndYOffset.EnableWindow(bEnable);
	m_wndXOffsetSpin.EnableWindow(bEnable);
	m_wndXOffset.EnableWindow(bEnable);

	bEnable = m_nDelayType == 1;
	m_wndTimerSlider.EnableWindow(bEnable);
	m_wndDelay.EnableWindow(bEnable);
	m_wndDelaySpin.EnableWindow(bEnable);

	bEnable = !m_pParent->m_bLeaderHead && m_bResizeEnable;
	m_wndResizeBottomSpin.EnableWindow(bEnable);
	m_wndResizeBottom.EnableWindow(bEnable);
	m_wndResizeTopSpin.EnableWindow(bEnable);
	m_wndResizeTop.EnableWindow(bEnable);
	m_wndResizeRightSpin.EnableWindow(bEnable);
	m_wndResizeRight.EnableWindow(bEnable);
	m_wndResizeLeftSpin.EnableWindow(bEnable);
	m_wndResizeLeft.EnableWindow(bEnable);
}

void CExportFlcFrm::OnFrmCount() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
}

void CExportFlcFrm::OnDeltaposFrmCountSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_bChanged = TRUE;
	*pResult = 0;
}

void CExportFlcFrm::OnChangeFrmCount() 
{
	m_bChanged = TRUE;

	if (m_wndFrmCount.GetSafeHwnd())
	{
		UpdateData();
		if (m_nFrmCount > MAX_FRM_COUNT)
		{
			m_nFrmCount = MAX_FRM_COUNT;
			UpdateData(FALSE);
		}
	}
}

void CExportFlcFrm::OnChangeFrmHeight() 
{
	if (m_wndFrmHeight.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);
		DDX_Text(&dx, IDC_FRM_HEIGHT, m_nFrmHeight);
		
//		UpdateData();
		if (m_nFrmHeight > WND_H)
		{
			m_nFrmHeight = WND_H;
			dx.m_bSaveAndValidate = FALSE;
			DDX_Text(&dx, IDC_FRM_HEIGHT, m_nFrmHeight);
		}
		
		m_bChanged = TRUE;
		
		if (m_bLock)
		{
			CString res;
			res.Format("%.0f", (float)m_nFrmHeight * m_nPropXtoY);
			m_nFrmWidth = atoi(res);
			
			if (m_nFrmWidth > WND_W)
			{
				m_nFrmWidth = WND_W;
				
				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_FRM_WIDTH, m_nFrmWidth);
				
				OnChangeFrmWidth();
			}
			else
			{
				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_FRM_WIDTH, m_nFrmWidth);
			}
			
			OnChangeFrmLeft();
			OnChangeFrmTop();
		}
		else
			OnChangeFrmTop();
	}
}

void CExportFlcFrm::OnDeltaposFrmHeightSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmHeight();
	*pResult = 0;
}

void CExportFlcFrm::OnFrmSize() 
{
	UpdateControlsState();
	OnChangeFrmLeft();
	OnChangeFrmTop();
//	m_bChanged = TRUE;
}

void CExportFlcFrm::OnChangeFrmWidth()
{
	if (m_wndFrmWidth.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);
		DDX_Text(&dx, IDC_FRM_WIDTH, m_nFrmWidth);
		
		m_bChanged = TRUE;
		
		if (m_nFrmWidth > WND_W)
		{
			m_nFrmWidth = WND_W;
			dx.m_bSaveAndValidate = FALSE;
			DDX_Text(&dx, IDC_FRM_WIDTH, m_nFrmWidth);
		}
		
		if (m_bLock)
		{
			CString res;
			res.Format("%.0f", (float)m_nFrmWidth * m_nPropYtoX);
			m_nFrmHeight = atoi(res);
			
			if (m_nFrmHeight > WND_H)
			{
				m_nFrmHeight = WND_H;
				dx.m_bSaveAndValidate = FALSE;
				
				DDX_Text(&dx, IDC_FRM_HEIGHT, m_nFrmHeight);
				OnChangeFrmHeight();
			}
			else
			{
				dx.m_bSaveAndValidate = FALSE;
				DDX_Text(&dx, IDC_FRM_HEIGHT, m_nFrmHeight);
			}
			
			OnChangeFrmLeft();
			OnChangeFrmTop();
		}
		else
			OnChangeFrmLeft();
	}
}

void CExportFlcFrm::OnDeltaposFrmWidthSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmWidth();
	*pResult = 0;
}

void CExportFlcFrm::OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nDelay = m_wndTimerSlider.GetPos();
	m_sDelay = uInt2Str(m_nDelay);
	
	CDataExchange dx(this, FALSE);
	DDX_Text(&dx, IDC_DELAY, m_sDelay);
	m_bChanged = TRUE;
	
	*pResult = 0;
}

void CExportFlcFrm::OnDelay() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
}

void CExportFlcFrm::ApplyChanges()
{
	UpdateData();
	
	m_pParent->m_nFrmCount	= m_nCountType == 0 ? m_nFrmCountCur : m_nFrmCount;
	m_pParent->m_nFrmHeight	= m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight;
	m_pParent->m_nFrmWidth	= m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth;
	m_pParent->m_bFrmResize	= m_nSizeType == 0 ? FALSE : m_bFrmResize;
	m_pParent->m_nDelay		= m_nDelayType == 0 ? m_nDelayCur : m_wndTimerSlider.GetPos();

	if (m_nSizeType == 0)
	{
		m_pParent->m_nXOffset	= m_nOffsetType == 0 ? m_nXOffsetCur : m_nXOffset;
		m_pParent->m_nYOffset	= m_nOffsetType == 0 ? m_nYOffsetCur : m_nYOffset;
	}
	else if (m_nOffsetType == 0)
	{
		UINT nFrmHeight	= WND_H - m_pParent->m_nFrmHeight;
		UINT nFrmWidth	= WND_H - m_pParent->m_nFrmWidth;
		UINT nXOffset	= m_nXOffsetCur;
		UINT nYOffset	= m_nYOffsetCur;
		
		if (nXOffset > nFrmWidth)
			nXOffset = nFrmWidth;

		if (nYOffset > nFrmHeight)
			nYOffset = nFrmHeight;

		m_pParent->m_nXOffset	= nXOffset;
		m_pParent->m_nYOffset	= nYOffset;
	}
	else
	{
		m_pParent->m_nXOffset	= m_nXOffset;
		m_pParent->m_nYOffset	= m_nYOffset;
	}

	m_pParent->m_bResizeEnable	= m_bResizeEnable;
	m_pParent->m_nResizeLeft	= m_bResizeEnable ? m_nResizeLeft : 0;
	m_pParent->m_nResizeRight	= m_bResizeEnable ? m_nResizeRight : (m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth);
	m_pParent->m_nResizeTop		= m_bResizeEnable ? m_nResizeTop : 0;
	m_pParent->m_nResizeBottom	= m_bResizeEnable ? m_nResizeBottom : (m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight);

	m_bChanged = FALSE;
}

void CExportFlcFrm::OnFrmResize() 
{
	UpdateData();
	m_bChanged = TRUE;
	UpdatePreview();
}

void CExportFlcFrm::OnLock() 
{
	UpdateData();
	
	if (m_nPropXtoY > m_nPropYtoX)
		OnChangeFrmHeight();
	else if (m_nPropYtoX > m_nPropXtoY)
		OnChangeFrmWidth();
	
	m_bChanged = TRUE;
}

void CExportFlcFrm::OnFrmOffset() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
	UpdatePreview();
}

void CExportFlcFrm::OnChangeFrmTop() 
{
	if (m_wndYOffset.GetSafeHwnd())
	{
		m_bChanged = TRUE;
		UpdateData();
	
		CDataExchange dx(this, FALSE);
		UINT nFrmHeight = WND_H - (m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight);
		
		if (m_nYOffset > nFrmHeight)
		{
			m_nYOffset = nFrmHeight;
			DDX_Text(&dx, IDC_FRM_TOP, m_nYOffset);
		}

		nFrmHeight = m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight;
		if (m_bResizeEnable)
		{
			if (m_nResizeTop >= nFrmHeight)
			{
				m_nResizeTop	= nFrmHeight-1;
				m_nResizeBottom = nFrmHeight;

				DDX_Text(&dx, IDC_RESIZE_TOP, m_nResizeTop);
				DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
			}
			else if (m_nResizeTop >= m_nResizeBottom)
			{
				m_nResizeBottom = m_nResizeTop + 1;
				
				DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
			}
			else if (m_nResizeBottom > nFrmHeight)
			{
				m_nResizeBottom = nFrmHeight - 1;

				DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
			}
		}//if (m_bResizeEnable)
		else
		{
			m_nResizeTop	= 0;
			m_nResizeBottom	= nFrmHeight;
			
			DDX_Text(&dx, IDC_RESIZE_TOP, m_nResizeTop);
			DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
		}
		
		UpdatePreview();
	}
}

void CExportFlcFrm::OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmTop();
	*pResult = 0;
}

void CExportFlcFrm::OnChangeFrmLeft() 
{
	if (m_wndXOffset.GetSafeHwnd())
	{
		m_bChanged = TRUE;
		UpdateData();
		
		CDataExchange dx(this, FALSE);
		UINT nFrmWidth = WND_W - (m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth);
		
		if (m_nXOffset > nFrmWidth)
		{
			m_nXOffset = nFrmWidth;
			DDX_Text(&dx, IDC_FRM_LEFT, m_nXOffset);
		}

		nFrmWidth = m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth;
		if (m_bResizeEnable)
		{
			if (m_nResizeLeft > nFrmWidth)
			{
				m_nResizeLeft	= nFrmWidth - 1;
				m_nResizeRight	= nFrmWidth;

				DDX_Text(&dx, IDC_RESIZE_LEFT, m_nResizeLeft);
				DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
			}
			else if (m_nResizeLeft >= m_nResizeRight)
			{
				m_nResizeRight = m_nResizeLeft + 1;
				
				DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
			}
			else if (m_nResizeRight > nFrmWidth)
			{
				m_nResizeRight	= nFrmWidth;
				
				DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
			}
		}
		else//if (m_bResizeEnable)
		{
			m_nResizeLeft	= 0;
			m_nResizeRight	= nFrmWidth;
			
			DDX_Text(&dx, IDC_RESIZE_LEFT, m_nResizeLeft);
			DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
		}

		UpdatePreview();
	}
}

void CExportFlcFrm::OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmLeft();
	*pResult = 0;
}

void CExportFlcFrm::OnChangeDelay() 
{
	if (m_wndDelay.GetSafeHwnd())
	{
		UpdateData();
		m_bChanged = TRUE;
		
		m_sDelay.Remove(' ');
		m_nDelay = atoi(m_sDelay);
		if (m_nDelay < FRM_DELAY_MIN)
		{
			m_nDelay = FRM_DELAY_MIN;
			m_sDelay = uInt2Str(m_nDelay);
		}
		else if (m_nDelay > FRM_DELAY_MAX)
		{
			m_nDelay = FRM_DELAY_MAX;
			m_sDelay = uInt2Str(m_nDelay);
		}
		
		CDataExchange dx(this, FALSE);
		DDX_Text(&dx, IDC_DELAY, m_sDelay);
		
		m_wndTimerSlider.SetPos(m_nDelay);
	}
}

void CExportFlcFrm::OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeDelay();
	
	*pResult = 0;
}

void CExportFlcFrm::UpdatePreview()
{
	if (m_nSizeType == 0)
	{
		m_pParent->m_PreviewParam.nFrmHeight	= m_nFrmHeightCur;
		m_pParent->m_PreviewParam.nFrmWidth		= m_nFrmWidthCur;
		m_pParent->m_PreviewParam.bFrmResize	= FALSE;
	}
	else
	{
		m_pParent->m_PreviewParam.nFrmHeight	= m_nFrmHeight;
		m_pParent->m_PreviewParam.nFrmWidth		= m_nFrmWidth;
		m_pParent->m_PreviewParam.bFrmResize	= m_bFrmResize;
	}

	if (m_nOffsetType == 0)
	{
		m_pParent->m_PreviewParam.nXOffset		= m_nXOffsetCur;
		m_pParent->m_PreviewParam.nYOffset		= m_nYOffsetCur;
	}
	else
	{
		m_pParent->m_PreviewParam.nXOffset		= m_nXOffset;
		m_pParent->m_PreviewParam.nYOffset		= m_nYOffset;
	}

	m_pParent->m_PreviewParam.bResizeEnable		= m_bResizeEnable;

	if (m_bResizeEnable)
	{
		m_pParent->m_PreviewParam.nResizeTop	= m_nResizeTop;
		m_pParent->m_PreviewParam.nResizeBottom	= m_nResizeBottom;
		m_pParent->m_PreviewParam.nResizeLeft	= m_nResizeLeft;
		m_pParent->m_PreviewParam.nResizeRight	= m_nResizeRight;
	}
}

void CExportFlcFrm::OnChangeResizeTop() 
{
	if (m_wndResizeTop.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);
		DDX_Text(&dx, IDC_RESIZE_TOP, m_nResizeTop);
		
//		UpdateData();
		dx.m_bSaveAndValidate = FALSE;
		UINT nHeight = m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight;
		
		if (m_nResizeTop >= nHeight)
		{
			m_nResizeTop	= nHeight-1;
			m_nResizeBottom = nHeight;
			DDX_Text(&dx, IDC_RESIZE_TOP, m_nResizeTop);
			DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
		}
		else if (m_nResizeTop >= m_nResizeBottom)
		{
			m_nResizeBottom = m_nResizeTop + 1;
			DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
		}

		m_bChanged = TRUE;
		UpdatePreview();
	}
}

void CExportFlcFrm::OnDeltaposResizeTopSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeResizeTop();
	*pResult = 0;
}

void CExportFlcFrm::OnChangeResizeBottom() 
{
	if (m_wndResizeBottom.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);
		DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
		
//		UpdateData();
		dx.m_bSaveAndValidate = FALSE;
		UINT nHeight = m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight;
		
		if (m_nResizeBottom > nHeight)
		{
			m_nResizeBottom = nHeight-1;
			DDX_Text(&dx, IDC_RESIZE_BOTTOM, m_nResizeBottom);
		}
		else if (m_nResizeTop >= m_nResizeBottom)
		{
			m_nResizeTop = m_nResizeBottom - 1;
			DDX_Text(&dx, IDC_RESIZE_TOP, m_nResizeTop);
		}
		
		m_bChanged = TRUE;
		UpdatePreview();
	}
}

void CExportFlcFrm::OnDeltaposResizeBottomSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeResizeBottom();
	
	*pResult = 0;
}

void CExportFlcFrm::OnChangeResizeLeft() 
{
	if (m_wndResizeLeft.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);
		DDX_Text(&dx, IDC_RESIZE_LEFT, m_nResizeLeft);
		
//		UpdateData();
		dx.m_bSaveAndValidate = FALSE;
		UINT nWidth = m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth;
	
		if (m_nResizeLeft > nWidth)
		{
			m_nResizeLeft	= nWidth-1;
			m_nResizeRight	= nWidth;
			DDX_Text(&dx, IDC_RESIZE_LEFT, m_nResizeLeft);
			DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
		}
		else if (m_nResizeLeft >= m_nResizeRight)
		{
			m_nResizeRight = m_nResizeLeft + 1;
			DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
		}

		m_bChanged = TRUE;
		UpdatePreview();
	}
}

void CExportFlcFrm::OnDeltaposResizeLeftSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeResizeLeft();
	*pResult = 0;
}

void CExportFlcFrm::OnChangeResizeRight() 
{
	if (m_wndResizeRight.GetSafeHwnd())
	{
		CDataExchange dx(this, TRUE);
		DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
		
//		UpdateData();
		UINT nWidth = m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth;
		dx.m_bSaveAndValidate = FALSE;

		if (m_nResizeRight > nWidth)
		{
			m_nResizeRight	= nWidth;

			DDX_Text(&dx, IDC_RESIZE_RIGHT, m_nResizeRight);
		}
		else if (m_nResizeLeft >= m_nResizeRight)
		{
			m_nResizeLeft = m_nResizeRight - 1;

			DDX_Text(&dx, IDC_RESIZE_LEFT, m_nResizeLeft);
		}

		m_bChanged = TRUE;
		UpdatePreview();
	}
}

void CExportFlcFrm::OnDeltaposResizeRightSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeResizeRight();
	*pResult = 0;
}

void CExportFlcFrm::OnResizeEnable() 
{
	UpdateControlsState();
	UpdatePreview();
}

void CExportFlcFrm::OnDestroy() 
{
	if (m_pParent->m_bSaveSettings)
	{
		CWinApp* pApp = AfxGetApp();
		UpdateData();
		
		pApp->WriteProfileInt(REG_EXP_FLC, REG_DELAY, m_nDelay);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_FRM_COUNT, m_nFrmCount);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_FRM_W, m_nFrmWidth);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_FRM_H, m_nFrmHeight);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_OFFSET_X, m_nXOffset);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_OFFSET_Y, m_nYOffset);
		
		pApp->WriteProfileInt(REG_EXP_FLC, REG_TRIM_TOP, m_nResizeTop);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_TRIM_BOTTOM, m_nResizeBottom);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_TRIM_LEFT, m_nResizeLeft);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_TRIM_RIGHT, m_nResizeRight);
		
		pApp->WriteProfileInt(REG_EXP_FLC, REG_USE_TRIM, m_bResizeEnable);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_USE_FRM_COUNT, m_nCountType);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_USE_SIZE, m_nSizeType);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_USE_DELAY, m_nDelayType);
		pApp->WriteProfileInt(REG_EXP_FLC, REG_USE_OFFSET, m_nOffsetType);

		pApp->WriteProfileInt(REG_EXP_FLC, REG_LOCK, m_bLock);
	}

	CDialogX::OnDestroy();
}

