/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FlcInfoGen.cpp
	CREATED:	09.7.2004
	VERSION:	1.0

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FlcInfoGen.h"

#include "FlcInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COL_ID		0
#define COL_INFO	1

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoGen dialog


CFlcInfoGen::CFlcInfoGen(CWnd* pParent /*=NULL*/)
	: CDialogX(CFlcInfoGen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlcInfoGen)
	//}}AFX_DATA_INIT

	SetControlInfo(IDC_LIST, CF_RESIZE_BOTH);
	
	m_pParent = (CFlcInfoDlg*) pParent;
	m_bDrawGripper = FALSE;
}

void CFlcInfoGen::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlcInfoGen)
	DDX_Control(pDX, IDC_LIST, m_Table);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlcInfoGen, CDialogX)
	//{{AFX_MSG_MAP(CFlcInfoGen)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoGen message handlers

BOOL CFlcInfoGen::OnInitDialog() 
{
	CDialogX::OnInitDialog();

	m_Table.ColorSortColumn();
	m_Table.KeepLabelLeft();
	m_Table.SetRowHeight(18);
	
	m_Table.InsertColumn(COL_ID,
		uLoadString(IDS_COL_ID),
		LVCFMT_LEFT,
		150);
	
	m_Table.InsertColumn(COL_INFO,
		uLoadString(IDS_COL_INFO),
		LVCFMT_LEFT,
		100);
	
	m_Table.LoadColumns(2, 150, 100);
	
	int i = 0;
	// File name
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, CFileX::GetFileName(m_pParent->m_sTitle));
	i++;
	// File size
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_flcHeader.size));
	i++;
	// Total frames
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_flcHeader.frames));
	i++;
	// Width
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_flcHeader.width));
	i++;
	// Height
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_flcHeader.height));
	i++;
	// Speed
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_flcHeader.speed));
	i++;
	// Total directions
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_civ3Header.num_anims));
	i++;
	// Frames per direction
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_civ3Header.anim_length));
	i++;
	// Animation time
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_civ3Header.anim_time));
	i++;
	// Original width
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_civ3Header.xs_orig));
	i++;
	// Original height
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_civ3Header.ys_orig));
	i++;
	// Left offset
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_civ3Header.x_offset));
	i++;
	// Top offset
	m_Table.InsertItem(i, uLoadString(IDS_COL_01 + i));
	m_Table.SetItemText(i, COL_INFO, uInt2Str(m_pParent->m_pFlc->m_civ3Header.y_offset));
	
	return TRUE;
}

void CFlcInfoGen::OnDestroy() 
{
	m_Table.SaveColumns();
	CDialogX::OnDestroy();
}
