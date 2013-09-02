/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	OptionsDlg.cpp
	CREATED:	14.3.2003
	VERSION:	2.2

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_DOC		0
#define TAB_VIEW	1
#define TAB_FLC		2
#define TAB_COUNT	3

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
: CDialogX(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	//}}AFX_DATA_INIT

	SetControlInfo(IDC_TAB, CF_RESIZE_BOTH);

	SetControlInfo(IDOK, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR, 4);
	SetControlInfo(IDC_APPLY, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT | CF_RESIZE_HOR, 4, 1);
	SetControlInfo(IDC_DEFAULT, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT | CF_RESIZE_HOR, 4, 2);
	SetControlInfo(IDCANCEL, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT | CF_RESIZE_HOR, 4, 3);
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_DEFAULT, m_btnDefault);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialogX)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	CDialogX::OnInitDialog();

	if (!MAIN_FRAME)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	LoadPosition(this); // load the dialog window position

	// Setup buttons
	m_btnOK.SetIcon(IDI_OK);
	m_btnApply.SetIcon(IDI_APPLY);
	m_btnDefault.SetIcon(IDI_DEFAULT);
	m_btnCancel.SetIcon(IDI_CANCEL);

	// Set icon to the dialog caption
	SetIcon(AfxGetApp()->LoadIcon(IDI_OPTIONS), FALSE);

	/////////////////////////////////////////////////////////////////////////////
	// INICIALIZE THE TAB CTRL
	m_wndTab.DeleteAllItems();

	// Create image list for CTabCtrl
	CBitmap bmp;
	m_TabImg.DeleteImageList();
	m_TabImg.Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 0); // create image list
	bmp.LoadBitmap(IDB_OPTIONS_TAB);                       // load bitmap for image list
	m_TabImg.Add(&bmp, RGB_BACKGROUND);
	m_wndTab.SetImageList(&m_TabImg);  // set image list

	// Add a tab for each of the two child dialog boxes.
	TCHAR  szTxt[256];         // text for the tabs header
	TCITEM tie; 
    tie.mask = TCIF_TEXT | TCIF_IMAGE; 

	lstrcpy(szTxt, uLoadString(IDS_OPTIONSTAB_DOC));
    tie.iImage  = TAB_DOC; 
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_DOC, &tie);

	lstrcpy(szTxt, uLoadString(IDS_OPTIONSTAB_VIEW));
    tie.iImage  = TAB_VIEW; 
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_VIEW, &tie);
	
	lstrcpy(szTxt, uLoadString(IDS_OPTIONSTAB_FLC));
    tie.iImage  = TAB_FLC; 
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_FLC, &tie);

	m_tabD.Create(IDD_OPTIONSDLG_DOC, this);
	m_tabV.Create(IDD_OPTIONSDLG_VIEW, this);
	m_tabF.Create(IDD_OPTIONSDLG_FLC, this);

	int nCur = AfxGetApp()->GetProfileInt(RegKey(), REG_CURRENT_TAB, 0);
	m_wndTab.SetCurSel((nCur < 0 || nCur > TAB_COUNT) ? 0 : nCur);
	ActivateTab();

	AdjustSheetSizes();

	m_bChanged = FALSE;
	m_btnApply.EnableWindow(FALSE);

	return TRUE;
}

void COptionsDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ActivateTab();
	*pResult = 0;
}

// returns the currently selected tab
CWnd* COptionsDlg::GetTab()
{
	switch (m_wndTab.GetCurSel())
	{
	case TAB_DOC:		return &m_tabD;
	case TAB_VIEW:		return &m_tabV;
	case TAB_FLC:		return &m_tabF;
	}

	return NULL;
}

// Activates the selected tab window.
void COptionsDlg::ActivateTab()
{
	CWnd* pWnd = GetTab();
	m_tabD.ShowWindow(pWnd == &m_tabD ? SW_SHOW : SW_HIDE);
	m_tabV.ShowWindow(pWnd == &m_tabV ? SW_SHOW : SW_HIDE);
	m_tabF.ShowWindow(pWnd == &m_tabF ? SW_SHOW : SW_HIDE);
}

void COptionsDlg::AdjustSheetSizes()
{
	if (m_tabD.GetSafeHwnd())
	{
		CRect rc;
		
		m_wndTab.GetClientRect(&rc);
		m_wndTab.AdjustRect(FALSE, &rc);
		m_wndTab.MapWindowPoints(this, &rc);
		
		m_tabD.MoveWindow(&rc, FALSE);
		m_tabV.MoveWindow(&rc, FALSE);
		m_tabF.MoveWindow(&rc, FALSE);
		
		Invalidate();
	}
}

void COptionsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogX::OnSize(nType, cx, cy);
	AdjustSheetSizes();
}

void COptionsDlg::OnOK() 
{
	OnApply();

	CDialogX::OnOK();
}

void COptionsDlg::OnApply() 
{
	m_bChanged |= GetDlgItem(IDC_APPLY)->IsWindowEnabled();

	m_tabD.Apply();
	m_tabV.Apply();
	m_tabF.Apply();
	
	MAIN_APP->SetAppParameters();

	if (MAIN_FRAME)
		MAIN_FRAME->ApplyOptions();

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

void COptionsDlg::OnDestroy() 
{
	AfxGetApp()->WriteProfileInt(RegKey(), REG_CURRENT_TAB, m_wndTab.GetCurSel());
	
	CDialogX::OnDestroy();
	SavePosition(this);
}

void COptionsDlg::OnDefault() 
{
	if (AfxMessageBox(uLoadString(IDS_SET_DEFAULT), MB_ICONQUESTION | MB_YESNOCANCEL) != IDYES)
		return;

	m_tabD.SetDefault();
	m_tabV.SetDefault();
	m_tabF.SetDefault();

	OnApply();
}
