/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	NewFlcDlg.cpp
	CREATED:	06.7.2004
	VERSION:	2.2

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "NewFlcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_GEN				0
#define TAB_OUT				1
#define TAB_FRM				2
#define TAB_PAL				3
#define TAB_COUNT			4

/////////////////////////////////////////////////////////////////////////////
// CNewFlcDlg dialog


CNewFlcDlg::CNewFlcDlg(CWnd* pParent /*=NULL*/)
	: CDialogX(CNewFlcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFlcDlg)
	m_bPreview = FALSE;
	m_bSaveSettings = FALSE;
	//}}AFX_DATA_INIT
	
	SetControlInfo(IDC_TAB, CF_RESIZE_BOTH);

	SetControlInfo(IDC_PREVIEW_CHECK, CF_ANCHOR_BOTTOM);
	SetControlInfo(IDC_SAVE_SETTINGS, CF_ANCHOR_BOTTOM);
	SetControlInfo(IDOK, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR, 2);
	SetControlInfo(IDCANCEL, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT | CF_RESIZE_HOR, 2, 1);

	m_pThread = NULL;
}


void CNewFlcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFlcDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	DDX_Check(pDX, IDC_PREVIEW_CHECK, m_bPreview);
	DDX_Check(pDX, IDC_SAVE_SETTINGS, m_bSaveSettings);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFlcDlg, CDialogX)
	//{{AFX_MSG_MAP(CNewFlcDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_BN_CLICKED(IDC_PREVIEW_CHECK, OnPreviewCheck)
	ON_BN_CLICKED(IDC_SAVE_SETTINGS, OnSaveSettings)
	//}}AFX_MSG_MAP
	ON_MESSAGE(PREVIEW_CANCEL, OnPreviewCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFlcDlg message handlers

BOOL CNewFlcDlg::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	LoadPosition(this);
	
	CWinApp *pApp = AfxGetApp();
	SetIcon(pApp->LoadIcon(IDI_FLC_NEW), FALSE);
	m_btnOK.SetIcon(IDI_OK);
	m_btnCancel.SetIcon(IDI_CANCEL);

	/////////////////////////////////////////////////////////////////////////////
	// Get Initial values
	
	m_sName = "Default";
	
	m_bPreview	= pApp->GetProfileInt(RegKey(), REG_PREVIEW, 1);
	m_nFileType = pApp->GetProfileInt(RegKey(), REG_FILE_TYPE, PCX_TYPE);
	if (m_nFileType > MAX_TYPE)
		m_nFileType = PCX_TYPE;

	m_nPalType = pApp->GetProfileInt(RegKey(), REG_PAL_TYPE, PalACT);
	if (m_nPalType > PalJASC)
		m_nPalType = PalACT;
	
	m_bDefPalette	= pApp->GetProfileInt(RegKey(), REG_DEF_PAL_TYPE, TRUE);
	m_bMakePalette	= pApp->GetProfileInt(RegKey(), REG_MAKE_PALETTE, TRUE);
	
	m_nLastPalType = (ELastPalType)pApp->GetProfileInt(RegKey(), REG_LAST_PAL_TYPE, PalCurrent);
	if (m_nLastPalType > PalSelective)
		m_nLastPalType = PalCurrent;
	
	m_sLastPalPath = pApp->GetProfileString(RegKey(), REG_LAST_PAL_PATH, "");
	m_sSelPalPath  = pApp->GetProfileString(RegKey(), REG_SEL_PAL_PATH, "");

	m_sDir = pApp->GetProfileString(RegKey(), REG_INIT_PATH, "");
	if (m_sDir.IsEmpty())
	{
		// get current directory
		char buf[MAX_PATH];
		GetModuleFileName(AfxGetResourceHandle(), buf, MAX_PATH);
		m_sDir = uGetPathName(buf);
		m_sDir += uLoadString(IDS_STORYBOARD_DIR);
	}
	
	m_sPath = m_sDir + "\\" + m_sName + (m_nFileType == PCX_TYPE ? ".pcx" : ".bmp");	
	m_nBitsPerPixel = pApp->GetProfileInt(RegKey(), REG_COL_DEPTH, BitsPerPixel_8);
	
	m_bSaveSettings = pApp->GetProfileInt(RegKey(), REG_SAVE_SETTINGS, FALSE);
	
	if (m_bSaveSettings)
	{
		m_bLeaderHead	= pApp->GetProfileInt(RegKey(), REG_LEADER_HEAD, FALSE);
		m_nDelay		= pApp->GetProfileInt(RegKey(), REG_DELAY, 125);
		m_nFrmCount		= pApp->GetProfileInt(RegKey(), REG_FRM_COUNT, 15);
		m_bSelPalPath	= pApp->GetProfileInt(RegKey(), REG_SEL_PAL, 0);
	}
	else
	{
		m_bLeaderHead	= FALSE;
		m_nDelay		= 125;
		m_nFrmCount		= 15;
		m_bSelPalPath	= 0;
	}
	
	UpdateValues();

	/////////////////////////////////////////////////////////////////////////////
	// INICIALIZE THE TAB CTRL
	m_wndTab.DeleteAllItems();

	// Add a tab for each of the two child dialog boxes.
	TCHAR  szTxt[256];         // text for the tabs header
	TCITEM tie; 
    tie.mask = TCIF_TEXT;// | TCIF_IMAGE; 
	
	lstrcpy(szTxt, uLoadString(IDS_TAB_SB_GEN));
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_GEN, &tie);
	
	lstrcpy(szTxt, uLoadString(IDS_TAB_SB_OUT));
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_OUT, &tie);
	
	lstrcpy(szTxt, uLoadString(IDS_TAB_SB_FRM));
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_FRM, &tie);
	
	lstrcpy(szTxt, uLoadString(IDS_TAB_SB_PAL));
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_PAL, &tie);

	m_tabG.m_pParent = (CNewFlcDlg*)this;
	m_tabO.m_pParent = (CNewFlcDlg*)this;
	m_tabF.m_pParent = (CNewFlcDlg*)this;
	m_tabP.m_pParent = (CNewFlcDlg*)this;

	m_tabO.Create(IDD_TAB_NEW_OUT, this);
	m_tabF.Create(IDD_TAB_NEW_FRM, this);
	m_tabP.Create(IDD_TAB_NEW_PAL, this);
	
	m_tabP.m_wndPalettes.GetLBText(m_tabP.m_wndPalettes.GetCurSel(), m_sPalette);

	m_tabG.Create(IDD_TAB_NEW_GEN, this);
	
	int nCur = AfxGetApp()->GetProfileInt(RegKey(), REG_CURRENT_TAB, 0);
	m_wndTab.SetCurSel((nCur < 0 || nCur > TAB_COUNT) ? 0 : nCur);
	ActivateTab();

//	m_wndTab.SetCurSel(TAB_GEN);
//	ActivateTab();
	
	AdjustSheetSizes();
	
	UpdateData(FALSE);
	UpdateControlsState();
	
	PreviewDlgInit();
	if (m_bPreview)
	{
		Sleep(100);
		PreviewDlgStart();
	}

	return TRUE;
}

void CNewFlcDlg::OnDestroy() 
{
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(RegKey(), REG_PREVIEW, m_bPreview);
	pApp->WriteProfileInt(RegKey(), REG_SAVE_SETTINGS, m_bSaveSettings);
	pApp->WriteProfileInt(RegKey(), REG_CURRENT_TAB, m_wndTab.GetCurSel());
	
	SavePosition(this);
	PreviewDlgStop();

	CDialogX::OnDestroy();
}


void CNewFlcDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ActivateTab();
	ApplyChanges(); // update info
	*pResult = 0;
}

// returns the currently selected tab
CWnd* CNewFlcDlg::GetTab()
{
	switch (m_wndTab.GetCurSel())
	{
	case TAB_GEN:	return &m_tabG;
	case TAB_OUT:	return &m_tabO;
	case TAB_FRM:	return &m_tabF;
	case TAB_PAL:	return &m_tabP;
	}
	
	return NULL;
}

// Activates the selected tab window.
void CNewFlcDlg::ActivateTab()
{
	CWnd* pWnd = GetTab();
	m_tabG.ShowWindow(pWnd == &m_tabG ? SW_SHOW : SW_HIDE);
	m_tabO.ShowWindow(pWnd == &m_tabO ? SW_SHOW : SW_HIDE);
	m_tabF.ShowWindow(pWnd == &m_tabF ? SW_SHOW : SW_HIDE);
	m_tabP.ShowWindow(pWnd == &m_tabP ? SW_SHOW : SW_HIDE);
}

void CNewFlcDlg::AdjustSheetSizes()
{
	if (m_tabG.GetSafeHwnd())
	{
		CRect rc;
		
		m_wndTab.GetClientRect(&rc);
		m_wndTab.AdjustRect(FALSE, &rc);
		m_wndTab.MapWindowPoints(this, &rc);
		
		m_tabG.MoveWindow(&rc, FALSE);
		m_tabO.MoveWindow(&rc, FALSE);
		m_tabF.MoveWindow(&rc, FALSE);
		m_tabP.MoveWindow(&rc, FALSE);
		
		Invalidate();
	}
}

void CNewFlcDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogX::OnSize(nType, cx, cy);
	AdjustSheetSizes();
}

void CNewFlcDlg::UpdateControlsState()
{
	UpdateData();
}

void CNewFlcDlg::OnOK() 
{
	ApplyChanges();

	if (m_sName.IsEmpty())
	{
		AfxMessageBox(IDS_EMPTY_NAME);
		return;
	}
	
	if (m_nLastPalType == PalSelective && !uIsFileExist(m_sLastPalPath))
	{
		if (AfxMessageBox(uFormatString(IDS_PAL_FILE_ABSENT, m_sLastPalPath), MB_YESNO) != IDYES)
			return;
	}

	if (m_bSelPalPath && !uIsFileExist(m_sSelPalPath))
	{
		if (AfxMessageBox(uFormatString(IDS_PAL_FILE_ABSENT, m_sSelPalPath), MB_YESNO) != IDYES)
			return;
	}

	uCreateNewFolder(m_sDir);
	if (!uIsDirExist(m_sDir))
	{
		AfxMessageBox(IDS_INVALID_DIR);
		return;
	}

	if (m_nPalette == 0)
		m_nPalette = 7; // by default

	///////////////////////////////////////////////////
	// Save settings changes
	CWinApp *pApp = AfxGetApp();
	pApp->WriteProfileInt(RegKey(), REG_FILE_TYPE, m_nFileType);
	pApp->WriteProfileInt(RegKey(), REG_PAL_TYPE, m_nPalType);
	pApp->WriteProfileInt(RegKey(), REG_LAST_PAL_TYPE, m_nLastPalType);
	pApp->WriteProfileInt(RegKey(), REG_DEF_PAL_TYPE, m_bDefPalette);
	pApp->WriteProfileInt(RegKey(), REG_MAKE_PALETTE, m_bMakePalette);
	pApp->WriteProfileString(RegKey(), REG_LAST_PAL_PATH, m_sLastPalPath);
	pApp->WriteProfileString(RegKey(), REG_INIT_PATH, m_sDir);
	pApp->WriteProfileInt(RegKey(), REG_COL_DEPTH, m_nBitsPerPixel);
	pApp->WriteProfileString(RegKey(), REG_SEL_PAL_PATH, m_sSelPalPath);
	
	if (m_bSaveSettings)
	{
		pApp->WriteProfileInt(RegKey(), REG_LEADER_HEAD, m_bLeaderHead);
		pApp->WriteProfileInt(RegKey(), REG_DELAY, m_nDelay);
		pApp->WriteProfileInt(RegKey(), REG_FRM_COUNT, m_nFrmCount);
		pApp->WriteProfileInt(RegKey(), REG_FRM_W, m_nFrmWidth);
		pApp->WriteProfileInt(RegKey(), REG_FRM_H, m_nFrmHeight);
		pApp->WriteProfileInt(RegKey(), REG_OFFSET_X, m_nXOffset);
		pApp->WriteProfileInt(RegKey(), REG_OFFSET_Y, m_nYOffset);
		pApp->WriteProfileInt(RegKey(), REG_DIR_COUNT, m_nDirCount);
		pApp->WriteProfileInt(RegKey(), REG_PALETTE, m_nPalette);
		pApp->WriteProfileInt(RegKey(), REG_SEL_PAL, m_bSelPalPath);
	}
	///////////////////////////////////////////////////
	
	PreviewDlgStop();
	CDialogX::OnOK();
}

void CNewFlcDlg::ApplyChanges()
{
	BOOL bChanged = FALSE;

	if (m_tabO.m_bChanged)
	{
		m_tabO.ApplyChanges();
		m_sPath = m_sDir + "\\" + m_sName + (m_nFileType == PCX_TYPE ? ".pcx" : ".bmp");
		bChanged = TRUE;
	}

	if (m_tabF.m_bChanged)
	{
		m_tabF.ApplyChanges();
		bChanged = TRUE;
	}
	
	if (m_tabP.m_bChanged)
	{
		m_tabP.ApplyChanges();
		bChanged = TRUE;
	}

	if (bChanged)
		m_tabG.ApplyChanges();
}

void CNewFlcDlg::UpdateValues()
{
	if (m_bLeaderHead)
	{
		m_nFrmWidth		= 200;
		m_nFrmHeight    = 240;
		m_nXOffset		= 0;
		m_nYOffset		= 0;
		m_nDirCount		= 1;
		m_nPalette		= 0;
	}
	else if (m_bSaveSettings)
	{
		CWinApp* pApp	= AfxGetApp();
		m_nFrmWidth		= pApp->GetProfileInt(RegKey(), REG_FRM_W, 50);
		m_nFrmHeight	= pApp->GetProfileInt(RegKey(), REG_FRM_H, 50);
		m_nXOffset		= pApp->GetProfileInt(RegKey(), REG_OFFSET_X, 95);
		m_nYOffset		= pApp->GetProfileInt(RegKey(), REG_OFFSET_Y, 80);
		m_nDirCount		= pApp->GetProfileInt(RegKey(), REG_DIR_COUNT, 8);
		m_nPalette		= pApp->GetProfileInt(RegKey(), REG_PALETTE, 0);
	}
	else
	{
		m_nFrmWidth		= 50;
		m_nFrmHeight    = 50;
		m_nXOffset		= 95;
		m_nYOffset		= 80;
		m_nDirCount		= 8;
		m_nPalette		= 0;
	}

	m_PreviewParam.nFrmHeight		= m_nFrmHeight;
	m_PreviewParam.nFrmWidth		= m_nFrmWidth;
	m_PreviewParam.nFrmWidthCur		= m_nFrmWidth;
	m_PreviewParam.nFrmHeightCur	= m_nFrmHeight;
	m_PreviewParam.nXOffset			= m_nXOffset;
	m_PreviewParam.nYOffset			= m_nYOffset;
}

void CNewFlcDlg::OnPreviewCheck() 
{
	UpdateData();
	m_PreviewParam.bShow = m_bPreview;

	if (m_bPreview)
		PreviewDlgStart();
}

void CNewFlcDlg::PreviewDlgInit()
{
	// Setup parameters of a preview dialog
	m_PreviewParam.bExit			= FALSE;
	m_PreviewParam.bShow			= m_bPreview;
	m_PreviewParam.nFrmHeight		= m_nFrmHeight;
	m_PreviewParam.nFrmWidth		= m_nFrmWidth;
	m_PreviewParam.nFrmWidthCur		= m_nFrmWidth;
	m_PreviewParam.nFrmHeightCur	= m_nFrmHeight;
	m_PreviewParam.nXOffset			= m_nXOffset;
	m_PreviewParam.nYOffset			= m_nYOffset;
	m_PreviewParam.hWnd				= this->m_hWnd;
	m_PreviewParam.colTransp		= RGB_TRANSPARENT;
}

void CNewFlcDlg::PreviewDlgStart()
{
	if (m_pThread && m_PreviewParam.bExit == FALSE)
		return;
	
	// Start thread with the progress dialog bar
	m_pThread = AfxBeginThread(StartPreviewDlg, (LPVOID)&m_PreviewParam, 0, CREATE_SUSPENDED );
	m_pThread->m_pMainWnd		= NULL;
	m_pThread->m_bAutoDelete	= TRUE;
	m_pThread->ResumeThread();
}

void CNewFlcDlg::PreviewDlgStop()
{
	m_PreviewParam.bExit = TRUE;
}

LONG CNewFlcDlg::OnPreviewCancel(UINT /*lParam*/, LONG /*wParam*/)
{
	m_PreviewParam.bShow = FALSE;
	m_bPreview = FALSE;
	UpdateData(FALSE);

    return TRUE;
}

void CNewFlcDlg::OnSaveSettings() 
{
	UpdateData();
}
