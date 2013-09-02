/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	OptionsDlgFlc.cpp
	CREATED:	21.06.2004

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "OptionsDlgFlc.h"

#include "OptionsDlg.h"
//#include "FlcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgFlc dialog

COptionsDlgFlc::COptionsDlgFlc(CWnd* pParent /*=NULL*/)
	: CDialogX(COptionsDlgFlc::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlgFlc)
	m_nPaletteType = 0;
	m_sPath = _T("");
	m_nAnimType = 0;
	m_bExpOpen = TRUE;
	m_bExpAsk = TRUE;
	m_bIgnoreOffset = FALSE;
	//}}AFX_DATA_INIT

	m_bDrawGripper = FALSE;

	SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);

	SetControlInfo(IDC_PAL_FILE, CF_RESIZE_HOR);
	SetControlInfo(IDC_PAL_RES, CF_RESIZE_HOR);
	
	SetControlInfo(IDC_STATIC_2, CF_RESIZE_HOR);
	SetControlInfo(IDC_ANIM_PLAY, CF_RESIZE_HOR);
	SetControlInfo(IDC_ANIM_STATIC, CF_RESIZE_HOR);
	SetControlInfo(IDC_IGNORE_OFFSET, CF_RESIZE_HOR);
	
	SetControlInfo(IDC_STATIC_3, CF_RESIZE_BOTH);
	SetControlInfo(IDC_EXP_OPEN, CF_RESIZE_HOR);
	SetControlInfo(IDC_EXP_ASK, CF_RESIZE_HOR);
}


void COptionsDlgFlc::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlgFlc)
	DDX_Control(pDX, IDC_DEL_PATH, m_btnDel);
	DDX_Control(pDX, IDC_BROWSE_PATH, m_btnBrowse);
	DDX_Control(pDX, IDC_EDIT_PATH, m_wndPath);
	DDX_Radio(pDX, IDC_PAL_FILE, m_nPaletteType);
	DDX_Text(pDX, IDC_EDIT_PATH, m_sPath);
	DDX_Radio(pDX, IDC_ANIM_PLAY, m_nAnimType);
	DDX_Check(pDX, IDC_EXP_OPEN, m_bExpOpen);
	DDX_Check(pDX, IDC_EXP_ASK, m_bExpAsk);
	DDX_Check(pDX, IDC_IGNORE_OFFSET, m_bIgnoreOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlgFlc, CDialogX)
	//{{AFX_MSG_MAP(COptionsDlgFlc)
	ON_BN_CLICKED(IDC_PAL_FILE, OnChanged)
	ON_BN_CLICKED(IDC_BROWSE_PATH, OnBrowsePath)
	ON_BN_CLICKED(IDC_DEL_PATH, OnDelPath)
	ON_BN_CLICKED(IDC_PAL_RES, OnChanged)
	ON_BN_CLICKED(IDC_PAL_DIR, OnChanged)
	ON_BN_CLICKED(IDC_ANIM_PLAY, OnChanged)
	ON_BN_CLICKED(IDC_ANIM_STATIC, OnChanged)
	ON_BN_CLICKED(IDC_EXP_ASK, OnChanged)
	ON_BN_CLICKED(IDC_EXP_OPEN, OnChanged)
	ON_BN_CLICKED(IDC_IGNORE_OFFSET, OnChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgFlc message handlers

BOOL COptionsDlgFlc::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	m_btnBrowse.SetIcon(IDI_BROWSE);
	m_btnDel.SetIcon(IDI_DELETE);

	CCiv3FlcEditApp* pApp = MAIN_APP;

	m_sPath			= pApp->GetProfileString(REG_OPTIONS, REG_PAL_DIR, "");
	m_nPaletteType	= pApp->GetProfileInt(REG_OPTIONS, REG_PAL_TYPE, PalGame);
	m_nAnimType		= pApp->GetProfileInt(REG_OPTIONS, REG_ANIM_TYPE, 0);
	m_bExpOpen		= pApp->GetProfileInt(REG_OPTIONS, REG_EXP_OPEN, 1);
	m_bExpAsk		= pApp->GetProfileInt(REG_OPTIONS, REG_EXP_ASK, 1);
	m_bIgnoreOffset	= pApp->GetProfileInt(REG_OPTIONS, REG_IGNORE_OFFSET, 0);

	if (m_nPaletteType == PalDir)
	{
		m_btnBrowse.EnableWindow(TRUE);
		m_btnDel.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
	return TRUE;
}

void COptionsDlgFlc::OnChangeData()
{
	COptionsDlg *pDlg = (COptionsDlg *)GetParent();
	pDlg->m_btnApply.EnableWindow();
}

void COptionsDlgFlc::Apply()
{
	UpdateData();

	CCiv3FlcEditApp* pApp = MAIN_APP;

	pApp->WriteProfileInt(REG_OPTIONS, REG_PAL_TYPE, m_nPaletteType);
	pApp->WriteProfileString(REG_OPTIONS, REG_PAL_DIR, m_sPath);
	pApp->WriteProfileInt(REG_OPTIONS, REG_ANIM_TYPE, m_nAnimType);
	pApp->WriteProfileInt(REG_OPTIONS, REG_EXP_OPEN, m_bExpOpen);
	pApp->WriteProfileInt(REG_OPTIONS, REG_EXP_ASK, m_bExpAsk);
	pApp->WriteProfileInt(REG_OPTIONS, REG_IGNORE_OFFSET, m_bIgnoreOffset);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Reset all values to the default state.
//
void COptionsDlgFlc::SetDefault()
{
	m_nPaletteType	= PalGame;
	m_nAnimType		= 0;
	m_bExpOpen		= TRUE;
	m_bExpAsk		= TRUE;
	m_bIgnoreOffset	= FALSE;
	
	UpdateData(FALSE);
}

void COptionsDlgFlc::OnChanged() 
{
	UpdateData();
	
	if (m_nPaletteType == PalDir)
	{
		m_btnBrowse.EnableWindow(TRUE);
		m_btnDel.EnableWindow(TRUE);
	}
	else
	{
		m_btnBrowse.EnableWindow(FALSE);
		m_btnDel.EnableWindow(FALSE);
	}

	OnChangeData();
}


void COptionsDlgFlc::OnBrowsePath() 
{
	UpdateData();
	
	// Set a new tapget path
	uBrowseFolder(this->m_hWnd, &m_sPath, uLoadString(IDS_SELECT_PAL_DIR), BIF_DONTGOBELOWDOMAIN);
	UpdateData(FALSE);
}

void COptionsDlgFlc::OnDelPath() 
{
	m_sPath.Empty();
	UpdateData(FALSE);
}
