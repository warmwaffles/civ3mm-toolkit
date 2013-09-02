/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FlcInfoDlg.cpp
	CREATED:	?.6.2004
	VERSION:	2.0

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FlcInfoDlg.h"

#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_GEN		0
#define TAB_EXT		1
#define TAB_COUNT	2

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoDlg dialog


CFlcInfoDlg::CFlcInfoDlg(CFlcView* pFlcView, CWnd* pParent /*=NULL*/)
	: CDialogX(CFlcInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlcInfoDlg)
	m_sTitle = _T("");
	//}}AFX_DATA_INIT

	ASSERT(pFlcView);
	m_pFlc = pFlcView;

	SetControlInfo(IDC_TITLE, CF_RESIZE_HOR);
	SetControlInfo(IDC_TAB, CF_RESIZE_BOTH);
	SetControlInfo(IDOK, CF_ANCHOR_RIGHT | CF_ANCHOR_BOTTOM, 2);
	SetControlInfo(IDC_SAVE, CF_ANCHOR_RIGHT | CF_ANCHOR_BOTTOM, 2, 1);
}

void CFlcInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlcInfoDlg)
	DDX_Control(pDX, IDC_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_TITLE, m_wndTitle);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_TITLE, m_sTitle);
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlcInfoDlg, CDialogX)
	//{{AFX_MSG_MAP(CFlcInfoDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoDlg message handlers

BOOL CFlcInfoDlg::OnInitDialog() 
{
	if (!m_pFlc || !m_pFlc->IsLoaded())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CDialogX::OnInitDialog();
	
	SetIcon(AfxGetApp()->LoadIcon(IDI_INFO), FALSE);
	LoadPosition(this);

//	m_wndTitle.SetName(m_pFlc->m_sPathName);
	m_wndTitle.SetWindowText(m_pFlc->m_sPathName);
	m_sTitle = m_pFlc->m_sPathName;
	
	/////////////////////////////////////////////////////////////////////////////
	// INICIALIZE THE TAB CTRL
	m_wndTab.DeleteAllItems();

	// Add a tab for each of the two child dialog boxes.
	TCHAR  szTxt[256];         // text for the tabs header
	TCITEM tie; 
    tie.mask = TCIF_TEXT;// | TCIF_IMAGE; 
	
	lstrcpy(szTxt, uLoadString(IDS_TAB_INFO_GEN));
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_GEN, &tie);
	
	lstrcpy(szTxt, uLoadString(IDS_TAB_INFO_EXT));
    tie.pszText = szTxt; 
    m_wndTab.InsertItem(TAB_EXT, &tie);

	m_tabG.m_pParent = (CFlcInfoDlg*)this;
	m_tabE.m_pParent = (CFlcInfoDlg*)this;

	m_tabG.Create(IDD_TAB_INFO_GEN, this);
	m_tabE.Create(IDD_TAB_INFO_EXT, this);
	
	m_wndTab.SetCurSel(TAB_GEN);
	ActivateTab();
	
	AdjustSheetSizes();
	
	m_btnOK.SetIcon(IDI_OK);
	m_btnSave.SetIcon(IDI_SAVE);

	return TRUE;
}

void CFlcInfoDlg::OnDestroy() 
{
	SavePosition(this);
	CDialogX::OnDestroy();
}


void CFlcInfoDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ActivateTab();
	*pResult = 0;
}

// returns the currently selected tab
CWnd* CFlcInfoDlg::GetTab()
{
	switch (m_wndTab.GetCurSel())
	{
	case TAB_GEN:	return &m_tabG;
	case TAB_EXT:	return &m_tabE;
	}
	
	return NULL;
}

// Activates the selected tab window.
void CFlcInfoDlg::ActivateTab()
{
	CWnd* pWnd = GetTab();
	m_tabG.ShowWindow(pWnd == &m_tabG ? SW_SHOW : SW_HIDE);
	m_tabE.ShowWindow(pWnd == &m_tabE ? SW_SHOW : SW_HIDE);
}

void CFlcInfoDlg::AdjustSheetSizes()
{
	if (m_tabG.GetSafeHwnd())
	{
		CRect rc;
		
		m_wndTab.GetClientRect(&rc);
		m_wndTab.AdjustRect(FALSE, &rc);
		m_wndTab.MapWindowPoints(this, &rc);
		
		m_tabG.MoveWindow(&rc, FALSE);
		m_tabE.MoveWindow(&rc, FALSE);
		
		Invalidate();
	}
}

void CFlcInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogX::OnSize(nType, cx, cy);
	AdjustSheetSizes();
}

void CFlcInfoDlg::OnSave() 
{
	CFileDialogX dlg(FALSE, NULL, "Info.txt",
		OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		CString((LPCTSTR)IDS_FILTER_TXT));
	
	CWinApp* pApp = AfxGetApp();
//	CString sTitle(uLoadString(IDS_TITLE_EXP_PAL));
	
//	dlg.m_ofn.lpstrTitle	= sTitle;

	CString sPath = pApp->GetProfileString(RegKey(), REG_LAST_OPENED, "");
	
	if (!uIsFileExist(sPath))
		sPath = uSHGetFolderPath(CSIDL_PERSONAL); // try to get "My Documents" directory path
	
	dlg.m_ofn.lpstrInitialDir = sPath;
	
	if (dlg.DoModal() != IDOK)
		return;
	
	sPath = dlg.GetPathName();
	
	pApp->WriteProfileString(RegKey(), REG_LAST_OPENED, sPath);
	
	CString sContent, sType;
	int i = 0;
	int j = 0;
	FRAME frame;
	
	sContent = uFormatString(IDS_INFO_HEADER, m_sTitle);
	
	///////////////
	sContent += uLoadString(IDS_INFO_GEN);

	// File name
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += CFileX::GetFileName(m_sTitle) + "\r\n";
	i++;

	// File size
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_flcHeader.size) + "\r\n";
	i++;

	// Total frames
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_flcHeader.frames) + "\r\n";
	i++;

	// Width
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_flcHeader.width) + "\r\n";
	i++;

	// Height
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_flcHeader.height) + "\r\n";
	i++;

	// Speed
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_flcHeader.speed) + "\r\n";
	i++;

	// Total directions
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_civ3Header.num_anims) + "\r\n";
	i++;

	// Frames per direction
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_civ3Header.anim_length) + "\r\n";
	i++;

	// Animation time
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_civ3Header.anim_time) + "\r\n";
	i++;

	// Original width
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_civ3Header.xs_orig) + "\r\n";
	i++;

	// Original height
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_civ3Header.ys_orig) + "\r\n";
	i++;

	// Left offset
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_civ3Header.x_offset) + "\r\n";
	i++;
	
	// Top offset
	sContent += uLoadString(IDS_COL_01 + i) + " - ";
	sContent += uInt2Str(m_pFlc->m_civ3Header.y_offset) + "\r\n";
	///////////////

	sContent += uLoadString(IDS_INFO_EXT);

	for (i = 0; i < m_pFlc->m_arFrames.GetSize(); i++)
	{
		frame = m_pFlc->m_arFrames.GetAt(i);
		sContent += uFormatString(IDS_INFO_FRAME, frame.id, frame.numch, frame.pos, frame.size);
		
		for (j = 0; j < frame.numch; j++)
		{
			switch (frame.chunks[j].type)
			{
			case ChDELTA_FLC:	sType = "DELTA_FLC";	break;
			case ChBYTE_RUN:	sType = "BYTE_RUN";		break;
			case ChCOLOR_256:	sType = "COLOR_256";	break;
			case ChBLACK:		sType = "BLACK";		break;
			case ChFLI_COPY:	sType = "FLI_COPY";		break;
			default:			sType = "unknown";
			}
			
			sContent += uFormatString(IDS_INFO_CHUNK, j, sType, frame.chunks[j].pos, frame.chunks[j].size);
		}
	}

	try
	{
		CFile fl(sPath, CFile::modeCreate | CFile::modeWrite | CFile::modeRead);
		fl.Write(sContent, sContent.GetLength());
		fl.Close();
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
	}
}
