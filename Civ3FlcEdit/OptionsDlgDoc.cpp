/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	OptionsDlgDoc.cpp
	CREATED:	09.09.2003

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "OptionsDlgDoc.h"

#include "OptionsDlg.h"
#include "FlcDoc.h"
#include "DocManagerEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgDoc dialog

COptionsDlgDoc::COptionsDlgDoc(CWnd* pParent /*=NULL*/)
	: CDialogX(COptionsDlgDoc::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlgDoc)
	m_nMRU = 0;
	m_bLoadLastDoc = FALSE;
	m_bCheckFileType = FALSE;
	m_bRegFileType = FALSE;
	//}}AFX_DATA_INIT

	m_bDrawGripper = FALSE;

	SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_2, CF_RESIZE_HOR);
	SetControlInfo(IDC_STATIC_3, CF_RESIZE_BOTH);

	SetControlInfo(IDC_REG_FILE_TYPE, CF_RESIZE_HOR);
	SetControlInfo(IDC_CHECK_FILE_TYPE, CF_RESIZE_HOR);
	SetControlInfo(IDC_LOAD_LAST_DOC, CF_RESIZE_HOR);
}


void COptionsDlgDoc::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlgDoc)
	DDX_Control(pDX, IDC_MRU_SPIN, m_wndSpinMRU);
	DDX_Control(pDX, IDC_MRU, m_wndMRU);
	DDX_Text(pDX, IDC_MRU, m_nMRU);
	DDX_Check(pDX, IDC_LOAD_LAST_DOC, m_bLoadLastDoc);
	DDX_Check(pDX, IDC_CHECK_FILE_TYPE, m_bCheckFileType);
	DDX_Check(pDX, IDC_REG_FILE_TYPE, m_bRegFileType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlgDoc, CDialogX)
	//{{AFX_MSG_MAP(COptionsDlgDoc)
	ON_BN_CLICKED(IDC_REG_FILE_TYPE, OnChanged)
	ON_BN_CLICKED(IDC_CHECK_FILE_TYPE, OnChanged)
	ON_BN_CLICKED(IDC_LOAD_LAST_DOC, OnChanged)
	ON_EN_CHANGE(IDC_MRU, OnChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgDoc message handlers

BOOL COptionsDlgDoc::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	CCiv3FlcEditApp* pApp = MAIN_APP;

	m_wndSpinMRU.SetRange(4, 16);
	m_wndMRU.LimitText(2);

	m_nMRU				= pApp->GetProfileInt(REG_OPTIONS, REG_MAX_MRU, 4);
	m_bLoadLastDoc		= pApp->GetProfileInt(REG_OPTIONS, REG_LOAD_LAST_DOC, 0);
	m_bCheckFileType	= pApp->GetProfileInt(REG_OPTIONS, REG_CHECK_FILE_TYPE, 1);
	m_bRegFileType		= pApp->IsFileTypeRegistered(pApp->GetDocManager()->GetDocTemplateFlc());
	m_bRegFileType		&= pApp->IsFileTypeRegistered(pApp->GetDocManager()->GetDocTemplateC3f());

	UpdateData(FALSE);
	return TRUE;
}

void COptionsDlgDoc::OnChangeData()
{
	COptionsDlg *pDlg = (COptionsDlg *)GetParent();
	pDlg->m_btnApply.EnableWindow();
}

void COptionsDlgDoc::Apply()
{
	UpdateData();

	CCiv3FlcEditApp* pApp = MAIN_APP;

	if (m_nMRU < 4 || m_nMRU > 16)
		m_nMRU = 4;

	pApp->WriteProfileInt(REG_OPTIONS, REG_MAX_MRU, m_nMRU);
	pApp->WriteProfileInt(REG_OPTIONS, REG_LOAD_LAST_DOC, m_bLoadLastDoc);
	pApp->WriteProfileInt(REG_OPTIONS, REG_CHECK_FILE_TYPE, m_bCheckFileType);
	pApp->SetFileTypeAssociations(m_bRegFileType);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Reset all values to the default state.
//
void COptionsDlgDoc::SetDefault()
{
	m_nMRU				= 4;
	m_bLoadLastDoc		= FALSE;
	m_bCheckFileType	= TRUE;
	m_bRegFileType		= TRUE;
	m_bRegFileType		= TRUE;
//	MAIN_APP->SetFileTypeAssociations(m_bRegFileType);
	
	UpdateData(FALSE);
}

void COptionsDlgDoc::OnChanged() 
{
	OnChangeData();
}

