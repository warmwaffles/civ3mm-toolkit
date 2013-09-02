/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	NewFlcFrm.cpp
	CREATED:	06.7.2004
	VERSION:	2.1

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "NewFlcFrm.h"

#include "NewFlcDlg.h"

#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFlcFrm dialog


CNewFlcFrm::CNewFlcFrm(CWnd* pParent /*=NULL*/)
	: CDialogX(CNewFlcFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFlcFrm)
	m_nFrmCount = 1;
	m_nFrmHeight = 1;
	m_nFrmWidth = 1;
	m_nCountType = 0;
	m_nSizeType = 0;
	m_nDelayType = 0;
	m_sDelayCur = _T("");
	m_sFrmCountCur = _T("");
	m_sFrmSizeCur = _T("");
	m_nDirCount = 0;
	m_sDirCountCur = _T("");
	m_nDirType = 0;
	m_bLock = TRUE;
	m_nXOffset = 0;
	m_nOffsetType = 0;
	m_nYOffset = 0;
	m_sFrmOffsetCur = _T("");
	m_sDelay = _T("");
	//}}AFX_DATA_INIT
	
	SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_2, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_3, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_4, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_5, CF_RESIZE_BOTH);

	m_bDrawGripper	= FALSE;
	m_pParent		= (CNewFlcDlg*) pParent;
	m_bChanged		= FALSE;
}


void CNewFlcFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFlcFrm)
	DDX_Control(pDX, IDC_SPIN_DELAY, m_wndDelaySpin);
	DDX_Control(pDX, IDC_DELAY, m_wndDelay);
	DDX_Control(pDX, IDC_FRM_TOP_SPIN, m_wndYOffsetSpin);
	DDX_Control(pDX, IDC_FRM_TOP, m_wndYOffset);
	DDX_Control(pDX, IDC_FRM_LEFT_SPIN, m_wndXOffsetSpin);
	DDX_Control(pDX, IDC_FRM_LEFT, m_wndXOffset);
	DDX_Control(pDX, IDC_DIR_COUNT, m_wndDirCount);
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
	DDX_Text(pDX, IDC_DIR_COUNT, m_nDirCount);
	DDX_Text(pDX, IDC_STATIC_DIR_COUNT_CUR, m_sDirCountCur);
	DDX_Radio(pDX, IDC_DIR_COUNT_CUR, m_nDirType);
	DDX_Control(pDX, IDC_LOCK, m_btnLock);
	DDX_Check(pDX, IDC_LOCK, m_bLock);
	DDX_Text(pDX, IDC_FRM_LEFT, m_nXOffset);
	DDX_Radio(pDX, IDC_FRM_OFFSET_CUR, m_nOffsetType);
	DDX_Text(pDX, IDC_FRM_TOP, m_nYOffset);
	DDX_Text(pDX, IDC_STATIC_FRM_OFFSET, m_sFrmOffsetCur);
	DDX_Text(pDX, IDC_DELAY, m_sDelay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFlcFrm, CDialogX)
	//{{AFX_MSG_MAP(CNewFlcFrm)
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
	ON_CBN_SELENDOK(IDC_DIR_COUNT, OnChangeDirCount)
	ON_BN_CLICKED(IDC_DIR_COUNT_CUR, OnDirCount)
	ON_BN_CLICKED(IDC_LOCK, OnLock)
	ON_BN_CLICKED(IDC_FRM_OFFSET_CUR, OnFrmOffset)
	ON_EN_CHANGE(IDC_FRM_TOP, OnChangeFrmTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_TOP_SPIN, OnDeltaposFrmTopSpin)
	ON_EN_CHANGE(IDC_FRM_LEFT, OnChangeFrmLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_LEFT_SPIN, OnDeltaposFrmLeftSpin)
	ON_EN_CHANGE(IDC_DELAY, OnChangeDelay)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, OnDeltaposSpinDelay)
	ON_BN_CLICKED(IDC_FRM_COUNT_CHANGE, OnFrmCount)
	ON_BN_CLICKED(IDC_FRM_SIZE_CHANGE, OnFrmSize)
	ON_BN_CLICKED(IDC_DELAY_CHANGE, OnDelay)
	ON_BN_CLICKED(IDC_DIR_COUNT_CHANGE, OnDirCount)
	ON_BN_CLICKED(IDC_FRM_OFFSET_CHANGE, OnFrmOffset)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFlcFrm message handlers

BOOL CNewFlcFrm::OnInitDialog() 
{
	CDialogX::OnInitDialog();

	m_wndFrmWidth.LimitText(3);
	m_wndFrmHeight.LimitText(3);
	m_wndFrmCount.LimitText(3);
	
	m_wndFrmCountSpin.SetRange(1, MAX_FRM_COUNT);
	m_wndFrmWidthSpin.SetRange(1, WND_W);
	m_wndFrmHeightSpin.SetRange(1, WND_H);
	
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
	
	m_wndYOffsetSpin.SetRange(0, WND_H-1);
	m_wndXOffsetSpin.SetRange(0, WND_W-1);
	m_wndXOffset.LimitText(3);
	m_wndYOffset.LimitText(3);
	m_wndDelay.SetLimitText(4);
	m_wndDelaySpin.SetRange(FRM_DELAY_MIN, FRM_DELAY_MAX);

	m_wndDirCount.SetCurSel(0);

	if (m_pParent->m_bSaveSettings)
	{
		CWinApp* pApp = AfxGetApp();
		m_nCountType	= pApp->GetProfileInt(REG_NEW_FLC, REG_USE_FRM_COUNT, 0);
		m_nSizeType		= pApp->GetProfileInt(REG_NEW_FLC, REG_USE_SIZE, 0);
		m_nDelayType	= pApp->GetProfileInt(REG_NEW_FLC, REG_USE_DELAY, 0);
		m_nOffsetType	= pApp->GetProfileInt(REG_NEW_FLC, REG_USE_OFFSET, 0);
		m_nDirType		= pApp->GetProfileInt(REG_NEW_FLC, REG_USE_DIR_COUNT, 0);
		m_bLock			= pApp->GetProfileInt(REG_NEW_FLC, REG_LOCK, 1);
	}

	UpdateValues();
	return TRUE;
}

void CNewFlcFrm::UpdateControlsState()
{
	UpdateData();

	BOOL bEnable = m_nSizeType == 1 && !m_pParent->m_bLeaderHead;
	m_wndFrmWidth.EnableWindow(bEnable);
	m_wndFrmWidthSpin.EnableWindow(bEnable);
	m_wndFrmHeight.EnableWindow(bEnable);
	m_wndFrmHeightSpin.EnableWindow(bEnable);
	m_btnLock.EnableWindow(bEnable);
	GetDlgItem(IDC_FRM_SIZE_CHANGE)->EnableWindow(!m_pParent->m_bLeaderHead);

	bEnable = (m_nOffsetType == 1);
	m_wndYOffsetSpin.EnableWindow(bEnable);
	m_wndYOffset.EnableWindow(bEnable);
	m_wndXOffsetSpin.EnableWindow(bEnable);
	m_wndXOffset.EnableWindow(bEnable);
	GetDlgItem(IDC_FRM_OFFSET_CHANGE)->EnableWindow(!m_pParent->m_bLeaderHead);
	
	bEnable = m_nCountType == 1;
	m_wndFrmCount.EnableWindow(bEnable);
	m_wndFrmCountSpin.EnableWindow(bEnable);

	bEnable = m_nDirType == 1 && !m_pParent->m_bLeaderHead;
	m_wndDirCount.EnableWindow(bEnable);
	GetDlgItem(IDC_DIR_COUNT_CHANGE)->EnableWindow(!m_pParent->m_bLeaderHead);

	bEnable = m_nDelayType == 1;
	m_wndTimerSlider.EnableWindow(bEnable);
	m_wndDelay.EnableWindow(bEnable);
	m_wndDelaySpin.EnableWindow(bEnable);
}

void CNewFlcFrm::OnFrmCount() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
}

void CNewFlcFrm::OnDeltaposFrmCountSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_bChanged = TRUE;
	*pResult = 0;
}

void CNewFlcFrm::OnChangeFrmCount() 
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

void CNewFlcFrm::OnChangeFrmHeight() 
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

void CNewFlcFrm::OnDeltaposFrmHeightSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmHeight();
	*pResult = 0;
}

void CNewFlcFrm::OnFrmSize() 
{
	UpdateControlsState();
	OnChangeFrmLeft();
	OnChangeFrmTop();
//	m_bChanged = TRUE;
}

void CNewFlcFrm::OnChangeFrmWidth() 
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

void CNewFlcFrm::OnDeltaposFrmWidthSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmWidth();
	*pResult = 0;
}

void CNewFlcFrm::OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nDelay = m_wndTimerSlider.GetPos();
	m_sDelay = uInt2Str(m_nDelay);
	
	CDataExchange dx(this, FALSE);
	DDX_Text(&dx, IDC_DELAY, m_sDelay);
	m_bChanged = TRUE;
	
	*pResult = 0;
}

void CNewFlcFrm::OnDelay() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
}

void CNewFlcFrm::ApplyChanges()
{
	UpdateData();
	
	m_pParent->m_nDirCount	= m_nDirType == 0 ? m_nDirCountCur : m_nDirCount;
	m_pParent->m_nFrmCount	= m_nCountType == 0 ? m_nFrmCountCur : m_nFrmCount;
	m_pParent->m_nFrmHeight	= m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight;
	m_pParent->m_nFrmWidth	= m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth;
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
	
	m_bChanged = FALSE;
}

void CNewFlcFrm::OnChangeDirCount() 
{
	m_bChanged = TRUE;
}

void CNewFlcFrm::OnDirCount() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
}

void CNewFlcFrm::UpdateValues()
{
	if (m_pParent->m_bLeaderHead)
	{
		m_nSizeType		= 0;
		m_nDirType		= 0;
		m_nOffsetType	= 0;
	}

	m_nDirCount		= m_nDirCountCur = m_pParent->m_nDirCount;
	m_sDirCountCur	= uInt2Str(m_nDirCount);
	m_nFrmCount		= m_nFrmCountCur = m_pParent->m_nFrmCount;
	m_nFrmHeight	= m_nFrmHeightCur = m_pParent->m_nFrmHeight;
	m_nFrmWidth		= m_nFrmWidthCur = m_pParent->m_nFrmWidth;
	m_nDelayCur		= m_nDelay = m_pParent->m_nDelay;
 	m_sDelayCur		= m_sDelay = uInt2Str(m_nDelayCur);
	m_sDelayCur += " ms";
	m_sFrmCountCur	= uInt2Str(m_nFrmCount);
	m_sFrmSizeCur.Format("%d x %d pixels", m_nFrmWidthCur, m_nFrmHeightCur);

	m_nPropXtoY		= (float)m_nFrmWidthCur / (float)m_nFrmHeightCur;
	m_nPropYtoX		= (float)m_nFrmHeightCur / (float)m_nFrmWidthCur;
	
	m_nXOffset = m_nXOffsetCur = m_pParent->m_nXOffset;
	m_nYOffset = m_nYOffsetCur = m_pParent->m_nYOffset;
	m_sFrmOffsetCur.Format("Left: %d, Top: %d pixels", m_nXOffsetCur, m_nYOffsetCur);

	UpdateData(FALSE);
	m_wndTimerSlider.SetPos(m_nDelay);
	UpdateControlsState();
}

void CNewFlcFrm::OnLock() 
{
	UpdateData();
	
	if (m_nPropXtoY > m_nPropYtoX)
		OnChangeFrmHeight();
	else if (m_nPropYtoX > m_nPropXtoY)
		OnChangeFrmWidth();
	
	m_bChanged = TRUE;
}


void CNewFlcFrm::OnFrmOffset() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
	UpdatePreview();
}

void CNewFlcFrm::OnChangeFrmTop() 
{
	if (m_wndYOffset.GetSafeHwnd())
	{
		m_bChanged = TRUE;
		UpdateData();
		
		UINT nFrmHeight = m_nSizeType == 0 ? m_nFrmHeightCur : m_nFrmHeight;
		if (m_nYOffset > (WND_H - nFrmHeight))
		{
			m_nYOffset = WND_H - nFrmHeight;
			UpdateData(FALSE);
		}
		UpdatePreview();
	}
}

void CNewFlcFrm::OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmTop();
	*pResult = 0;
}

void CNewFlcFrm::OnChangeFrmLeft() 
{
	if (m_wndXOffset.GetSafeHwnd())
	{
		m_bChanged = TRUE;
		UpdateData();
		
		UINT nFrmWidth = m_nSizeType == 0 ? m_nFrmWidthCur : m_nFrmWidth;
		if (m_nXOffset > (WND_W - nFrmWidth))
		{
			m_nXOffset = WND_W - nFrmWidth;
			UpdateData(FALSE);
		}
		UpdatePreview();
	}
}

void CNewFlcFrm::OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeFrmLeft();
	*pResult = 0;
}

void CNewFlcFrm::OnChangeDelay() 
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

void CNewFlcFrm::OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	OnChangeDelay();
	
	*pResult = 0;
}

void CNewFlcFrm::UpdatePreview()
{
	if (m_nSizeType == 0)
	{
		m_pParent->m_PreviewParam.nFrmHeight	= m_nFrmHeightCur;
		m_pParent->m_PreviewParam.nFrmWidth		= m_nFrmWidthCur;
	}
	else
	{
		m_pParent->m_PreviewParam.nFrmHeight	= m_nFrmHeight;
		m_pParent->m_PreviewParam.nFrmWidth		= m_nFrmWidth;
	}
	
	if (m_nOffsetType == 0)
	{
		m_pParent->m_PreviewParam.nXOffset	= m_nXOffsetCur;
		m_pParent->m_PreviewParam.nYOffset	= m_nYOffsetCur;
	}
	else
	{
		m_pParent->m_PreviewParam.nXOffset	= m_nXOffset;
		m_pParent->m_PreviewParam.nYOffset	= m_nYOffset;
	}
}

void CNewFlcFrm::OnDestroy() 
{
	if (m_pParent->m_bSaveSettings)
	{
		CWinApp* pApp = AfxGetApp();
		UpdateData();
		pApp->WriteProfileInt(REG_NEW_FLC, REG_USE_FRM_COUNT, m_nCountType);
		pApp->WriteProfileInt(REG_NEW_FLC, REG_USE_SIZE, m_nSizeType);
		pApp->WriteProfileInt(REG_NEW_FLC, REG_USE_DELAY, m_nDelayType);
		pApp->WriteProfileInt(REG_NEW_FLC, REG_USE_OFFSET, m_nOffsetType);
		pApp->WriteProfileInt(REG_NEW_FLC, REG_USE_DIR_COUNT, m_nDirType);
		pApp->WriteProfileInt(REG_NEW_FLC, REG_LOCK, m_bLock);
	}

	CDialogX::OnDestroy();	
}
