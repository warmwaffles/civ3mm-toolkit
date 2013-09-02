/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	NewFlcOut.cpp
	CREATED:	06.7.2004
	VERSION:	2.2

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "NewFlcOut.h"

#include "NewFlcDlg.h"
#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFlcOut dialog


CNewFlcOut::CNewFlcOut(CWnd* pParent /*=NULL*/)
	: CDialogX(CNewFlcOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFlcOut)
	m_sFileName = _T("");
	m_nNameType = 0;
	m_sDir = _T("");
	m_sCurName = _T("");
	m_nAnimType = 0;
	//}}AFX_DATA_INIT
	
	SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_2, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_3, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_4, CF_RESIZE_BOTH);
	
	SetControlInfo(IDC_TARGET_PATH, CF_RESIZE_HOR);
	SetControlInfo(IDC_BROWSE_PATH, CF_ANCHOR_RIGHT);
	SetControlInfo(IDC_STATIC_NAME_CUR, CF_RESIZE_HOR);
	SetControlInfo(IDC_FILE_NAME, CF_RESIZE_HOR);
	
	m_bDrawGripper	= FALSE;
	m_pParent		= (CNewFlcDlg*) pParent;
	m_bChanged		= FALSE;
}


void CNewFlcOut::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFlcOut)
	DDX_Control(pDX, IDC_TARGET_PATH, m_wndDir);
	DDX_Control(pDX, IDC_FILE_TYPE, m_wndFileType);
	DDX_Control(pDX, IDC_FILE_NAME, m_wndFileName);
	DDX_Control(pDX, IDC_BROWSE_PATH, m_btnBrowse);
	DDX_Text(pDX, IDC_FILE_NAME, m_sFileName);
	DDX_Radio(pDX, IDC_NAME_CUR, m_nNameType);
	DDX_Text(pDX, IDC_TARGET_PATH, m_sDir);
	DDX_Text(pDX, IDC_STATIC_NAME_CUR, m_sCurName);
	DDX_Radio(pDX, IDC_TYPE_UNIT, m_nAnimType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFlcOut, CDialogX)
	//{{AFX_MSG_MAP(CNewFlcOut)
	ON_BN_CLICKED(IDC_NAME_CUR, OnName)
	ON_BN_CLICKED(IDC_BROWSE_PATH, OnBrowsePath)
	ON_CBN_SELENDOK(IDC_FILE_TYPE, OnSelendokFileType)
	ON_EN_CHANGE(IDC_FILE_NAME, OnChangeFileName)
	ON_BN_CLICKED(IDC_TYPE_UNIT, OnAnimationType)
	ON_BN_CLICKED(IDC_NAME_CHANGE, OnName)
	ON_BN_CLICKED(IDC_TYPE_LEADER, OnAnimationType)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFlcOut message handlers

BOOL CNewFlcOut::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	m_btnBrowse.SetIcon(IDI_BROWSE);

	m_wndFileType.AddString(uLoadString(IDS_FILE_TYPE_PCX));
	m_wndFileType.AddString(uLoadString(IDS_FILE_TYPE_BMP));
	m_wndFileType.SetCurSel(m_pParent->m_nFileType);
	
	m_nAnimType = m_pParent->m_bLeaderHead ? 1 : 0;

	m_wndFileName.LimitText(128);

	m_sCurName = m_sFileName = m_pParent->m_sName;
	m_sDir = m_pParent->m_sDir;

	m_nNameType = m_pParent->m_bSaveSettings ? AfxGetApp()->GetProfileInt(REG_NEW_FLC, REG_USE_NAME, 0) : 0;
	
	UpdateData(FALSE);
	UpdateControlsState();
	return TRUE;
}

void CNewFlcOut::OnBrowsePath() 
{
	uBrowseFolder(this->m_hWnd, &m_sDir, uLoadString(IDS_SELECT_SB_DIR), BIF_DONTGOBELOWDOMAIN);
	UpdateData(FALSE);
	m_bChanged = TRUE;
}

void CNewFlcOut::OnName() 
{
	UpdateControlsState();
	m_bChanged = TRUE;
}

void CNewFlcOut::UpdateControlsState()
{
	UpdateData();

	m_wndFileName.EnableWindow(m_nNameType == 1);
}

void CNewFlcOut::ApplyChanges()
{
	UpdateData();
	
	m_pParent->m_nFileType		= m_wndFileType.GetCurSel();
	m_pParent->m_sName			= m_nNameType == 0 ? m_sCurName : m_sFileName;
	m_pParent->m_sDir			= m_sDir;
	m_pParent->m_bLeaderHead	= m_nAnimType == 1 ? TRUE : FALSE;

	m_bChanged = FALSE;
}

void CNewFlcOut::OnSelendokFileType() 
{
	m_bChanged = TRUE;
}

void CNewFlcOut::OnChangeFileName() 
{
	m_bChanged = TRUE;
}

void CNewFlcOut::OnAnimationType() 
{
	UpdateData();
	m_bChanged = TRUE;
	
	if (m_nAnimType == 1)
		m_pParent->m_bLeaderHead = TRUE;
	else
		m_pParent->m_bLeaderHead = FALSE;

	m_pParent->UpdateValues();
	m_pParent->m_tabF.UpdateValues();	
	m_pParent->m_tabP.UpdateValues();
	m_pParent->ApplyChanges();
	UpdateControlsState();
}

void CNewFlcOut::OnDestroy() 
{
	if (m_pParent->m_bSaveSettings)
		AfxGetApp()->WriteProfileInt(REG_NEW_FLC, REG_USE_NAME, m_nNameType);
	
	CDialogX::OnDestroy();
}
