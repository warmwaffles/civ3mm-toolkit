/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ExportFlcDlg.cpp
	CREATED:	02.7.2004
	VERSION:	2.2

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ExportFlcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_GEN 0
#define TAB_OUT 1
#define TAB_FRM 2
#define TAB_PAL 3
#define TAB_COUNT 4

/////////////////////////////////////////////////////////////////////////////
// CExportFlcDlg dialog

CExportFlcDlg::CExportFlcDlg(CFlcView* pFlc, CWnd* pParent /*=NULL*/)
  : CDialogX(CExportFlcDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CExportFlcDlg)
    m_bPreview      = FALSE;
    m_bSaveSettings = FALSE;
    //}}AFX_DATA_INIT

    SetControlInfo(IDC_TAB, CF_RESIZE_BOTH);

    SetControlInfo(IDC_PREVIEW_CHECK, CF_ANCHOR_BOTTOM);
    SetControlInfo(IDC_SAVE_SETTINGS, CF_ANCHOR_BOTTOM);
    SetControlInfo(IDOK, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR, 2);
    SetControlInfo(IDCANCEL, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT | CF_RESIZE_HOR, 2, 1);

    ASSERT(pFlc);
    m_pFlc    = pFlc;
    m_pThread = NULL;
}

void
CExportFlcDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExportFlcDlg)
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_TAB, m_wndTab);
    DDX_Check(pDX, IDC_PREVIEW_CHECK, m_bPreview);
    DDX_Check(pDX, IDC_SAVE_SETTINGS, m_bSaveSettings);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExportFlcDlg, CDialogX)
//{{AFX_MSG_MAP(CExportFlcDlg)
ON_WM_DESTROY()
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
ON_WM_SIZE()
ON_BN_CLICKED(IDC_PREVIEW_CHECK, OnPreviewCheck)
ON_BN_CLICKED(IDC_SAVE_SETTINGS, OnSaveSettings)
//}}AFX_MSG_MAP
ON_MESSAGE(PREVIEW_CANCEL, OnPreviewCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportFlcDlg message handlers

BOOL
CExportFlcDlg::OnInitDialog()
{
    if (!m_pFlc || !m_pFlc->IsLoaded()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    //	SetParent(NULL);
    CDialogX::OnInitDialog();

    LoadPosition(this);

    CWinApp* pApp = AfxGetApp();
    SetIcon(pApp->LoadIcon(IDI_FLC_EXP), FALSE);
    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);

    /////////////////////////////////////////////////////////////////////////////
    // Get Initial values
    m_bSaveSettings = pApp->GetProfileInt(RegKey(), REG_SAVE_SETTINGS, FALSE);

    m_bPreview = pApp->GetProfileInt(RegKey(), REG_PREVIEW, 1);
    m_sName    = CFileX::GetShortFileName(m_pFlc->m_sPathName);

    m_nFileType = pApp->GetProfileInt(RegKey(), REG_FILE_TYPE, PCX_TYPE);
    if (m_nFileType > MAX_TYPE)
        m_nFileType = PCX_TYPE;

    m_nPalType = pApp->GetProfileInt(RegKey(), REG_PAL_TYPE, PalACT);
    if (m_nPalType > PalJASC)
        m_nPalType = PalACT;

    m_bDefPalette  = pApp->GetProfileInt(RegKey(), REG_DEF_PAL_TYPE, TRUE);
    m_bMakePalette = pApp->GetProfileInt(RegKey(), REG_MAKE_PALETTE, TRUE);
    m_bLeaderHead  = m_pFlc->IsLeaderHead();

    if (m_bLeaderHead) {
        m_nLastPalType = PalCurrent;
    } else {
        m_nLastPalType = (ELastPalType)pApp->GetProfileInt(RegKey(), REG_LAST_PAL_TYPE, PalCurrent);
        if (m_nLastPalType > PalSelective)
            m_nLastPalType = PalCurrent;
    }

    m_sLastPalPath = pApp->GetProfileString(RegKey(), REG_LAST_PAL_PATH, "");

    m_sDir = pApp->GetProfileString(RegKey(), REG_INIT_PATH, "");
    if (m_sDir.IsEmpty()) {
        // get current directory
        char buf[MAX_PATH];
        GetModuleFileName(AfxGetResourceHandle(), buf, MAX_PATH);
        m_sDir = uGetPathName(buf);
        m_sDir += uLoadString(IDS_STORYBOARD_DIR);
    }

    m_sPath = m_sDir + "\\" + m_sName + (m_nFileType == PCX_TYPE ? ".pcx" : ".bmp");

    m_bFrmResize = pApp->GetProfileInt(RegKey(), REG_FRM_RESIZE, 0);
    m_nFrmWidth  = (BYTE)m_pFlc->m_flcHeader.width;
    m_nFrmHeight = (BYTE)m_pFlc->m_flcHeader.height;
    m_nFrmCount  = (WORD)m_pFlc->m_civ3Header.anim_length;
    m_nPalette   = m_pFlc->m_nCurPalette;
    m_nDelay     = (WORD)(m_pFlc->m_bDefaultTimer ? m_pFlc->m_flcHeader.speed : m_pFlc->m_uElapse);

    m_nXOffset = (BYTE)m_pFlc->m_civ3Header.x_offset;
    m_nYOffset = (BYTE)m_pFlc->m_civ3Header.y_offset;

    RGBQUAD pal = m_pFlc->m_rgbPal.pal[CIV_COLOR];
    m_crDefCiv  = RGB(pal.rgbRed, pal.rgbGreen, pal.rgbBlue);

    m_bResizeEnable = FALSE;
    m_nResizeLeft   = 0;
    m_nResizeRight  = m_nFrmWidth;
    m_nResizeTop    = 0;
    m_nResizeBottom = m_nFrmHeight;
    m_nBitsPerPixel = pApp->GetProfileInt(RegKey(), REG_COL_DEPTH, BitsPerPixel_8);

    PreviewDlgInit();

    /////////////////////////////////////////////////////////////////////////////
    // INICIALIZE THE TAB CTRL
    m_wndTab.DeleteAllItems();

    // Add a tab for each of the two child dialog boxes.
    TCHAR szTxt[256]; // text for the tabs header
    TCITEM tie;
    tie.mask = TCIF_TEXT; // | TCIF_IMAGE;

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

    m_tabG.m_pParent = (CExportFlcDlg*)this;
    m_tabO.m_pParent = (CExportFlcDlg*)this;
    m_tabF.m_pParent = (CExportFlcDlg*)this;
    m_tabP.m_pParent = (CExportFlcDlg*)this;

    m_tabO.Create(IDD_TAB_EXP_OUT, this);
    m_tabF.Create(IDD_TAB_EXP_FRM, this);
    m_tabP.Create(IDD_TAB_EXP_PAL, this);

    m_tabP.m_wndPalettes.GetLBText(m_tabP.m_wndPalettes.GetCurSel(), m_sPalette);

    m_tabG.Create(IDD_TAB_EXP_GEN, this);

    int nCur = AfxGetApp()->GetProfileInt(RegKey(), REG_CURRENT_TAB, 0);
    m_wndTab.SetCurSel((nCur < 0 || nCur > TAB_COUNT) ? 0 : nCur);
    ActivateTab();

    //	m_wndTab.SetCurSel(TAB_GEN);
    //	ActivateTab();

    AdjustSheetSizes();

    UpdateData(FALSE);
    UpdateControlsState();

    if (m_bPreview) {
        Sleep(100);
        PreviewDlgStart();
    }

    return TRUE;
}

void
CExportFlcDlg::OnDestroy()
{
    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(RegKey(), REG_PREVIEW, m_bPreview);
    pApp->WriteProfileInt(RegKey(), REG_SAVE_SETTINGS, m_bSaveSettings);
    pApp->WriteProfileInt(RegKey(), REG_CURRENT_TAB, m_wndTab.GetCurSel());

    SavePosition(this);
    PreviewDlgStop();

    CDialogX::OnDestroy();
}

void
CExportFlcDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
    ActivateTab();
    ApplyChanges(); // update info
    *pResult = 0;
}

// returns the currently selected tab
CWnd*
CExportFlcDlg::GetTab()
{
    switch (m_wndTab.GetCurSel()) {
        case TAB_GEN:
            return &m_tabG;
        case TAB_OUT:
            return &m_tabO;
        case TAB_FRM:
            return &m_tabF;
        case TAB_PAL:
            return &m_tabP;
    }

    return NULL;
}

// Activates the selected tab window.
void
CExportFlcDlg::ActivateTab()
{
    CWnd* pWnd = GetTab();
    m_tabG.ShowWindow(pWnd == &m_tabG ? SW_SHOW : SW_HIDE);
    m_tabO.ShowWindow(pWnd == &m_tabO ? SW_SHOW : SW_HIDE);
    m_tabF.ShowWindow(pWnd == &m_tabF ? SW_SHOW : SW_HIDE);
    m_tabP.ShowWindow(pWnd == &m_tabP ? SW_SHOW : SW_HIDE);
}

void
CExportFlcDlg::AdjustSheetSizes()
{
    if (m_tabG.GetSafeHwnd()) {
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

void
CExportFlcDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogX::OnSize(nType, cx, cy);
    AdjustSheetSizes();
}

void
CExportFlcDlg::UpdateControlsState()
{
    UpdateData();
}

void
CExportFlcDlg::OnOK()
{
    ApplyChanges();

    if (m_sName.IsEmpty()) {
        AfxMessageBox(IDS_EMPTY_NAME);
        return;
    }

    if (m_nLastPalType == PalSelective && !uIsFileExist(m_sLastPalPath)) {
        if (AfxMessageBox(uFormatString(IDS_PAL_FILE_ABSENT, m_sLastPalPath), MB_YESNO) != IDYES)
            return;
    }

    uCreateNewFolder(m_sDir);
    if (!uIsDirExist(m_sDir)) {
        AfxMessageBox(IDS_INVALID_DIR);
        return;
    }

    ///////////////////////////////////////////////////
    // Save settings changes
    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(RegKey(), REG_FILE_TYPE, m_nFileType);
    pApp->WriteProfileInt(RegKey(), REG_PAL_TYPE, m_nPalType);
    pApp->WriteProfileInt(RegKey(), REG_LAST_PAL_TYPE, m_nLastPalType);
    pApp->WriteProfileString(RegKey(), REG_LAST_PAL_PATH, m_sLastPalPath);
    pApp->WriteProfileString(RegKey(), REG_INIT_PATH, m_sDir);
    pApp->WriteProfileInt(RegKey(), REG_FRM_RESIZE, m_bFrmResize);
    pApp->WriteProfileInt(RegKey(), REG_DEF_PAL_TYPE, m_bDefPalette);
    pApp->WriteProfileInt(RegKey(), REG_MAKE_PALETTE, m_bMakePalette);
    pApp->WriteProfileInt(RegKey(), REG_COL_DEPTH, m_nBitsPerPixel);
    ///////////////////////////////////////////////////

    PreviewDlgStop();
    CDialogX::OnOK();
}

void
CExportFlcDlg::ApplyChanges()
{
    BOOL bChanged = FALSE;

    if (m_tabO.m_bChanged) {
        m_tabO.ApplyChanges();
        m_sPath  = m_sDir + "\\" + m_sName + (m_nFileType == PCX_TYPE ? ".pcx" : ".bmp");
        bChanged = TRUE;
    }

    if (m_tabF.m_bChanged) {
        m_tabF.ApplyChanges();
        bChanged = TRUE;
    }

    if (m_tabP.m_bChanged) {
        m_tabP.ApplyChanges();
        bChanged = TRUE;
    }

    if (bChanged)
        m_tabG.ApplyChanges();
}

void
CExportFlcDlg::OnPreviewCheck()
{
    UpdateData();
    m_PreviewParam.bShow = m_bPreview;

    if (m_bPreview)
        PreviewDlgStart();
}

void
CExportFlcDlg::PreviewDlgInit()
{
    // Setup parameters of a preview dialog
    m_PreviewParam.bExit         = FALSE;
    m_PreviewParam.bShow         = m_bPreview;
    m_PreviewParam.bFrmResize    = m_bFrmResize;
    m_PreviewParam.nFrmHeight    = m_nFrmHeight;
    m_PreviewParam.nFrmWidth     = m_nFrmWidth;
    m_PreviewParam.nFrmWidthCur  = m_nFrmWidth;
    m_PreviewParam.nFrmHeightCur = m_nFrmHeight;
    m_PreviewParam.nXOffset      = m_nXOffset;
    m_PreviewParam.nYOffset      = m_nYOffset;
    m_PreviewParam.hWnd          = this->m_hWnd;
    m_PreviewParam.colTransp     = m_pFlc->m_colTransparent;

    //	m_PreviewParam.bResizeEnable	= FALSE;
    //	m_PreviewParam.nResizeTop		= 0;
    m_PreviewParam.nResizeBottom = m_nFrmHeight;
    //	m_PreviewParam.nResizeLeft		= 0;
    m_PreviewParam.nResizeRight = m_nFrmWidth;
    m_PreviewParam.pSurface     = m_pFlc->m_arSFrames.GetAt(m_pFlc->m_curFrame);
}

void
CExportFlcDlg::PreviewDlgStart()
{
    if (m_pThread && m_PreviewParam.bExit == FALSE)
        return;

    // Start thread with the progress dialog bar
    m_pThread                = AfxBeginThread(StartPreviewDlg, (LPVOID)&m_PreviewParam, 0, CREATE_SUSPENDED);
    m_pThread->m_pMainWnd    = NULL;
    m_pThread->m_bAutoDelete = TRUE;
    m_pThread->ResumeThread();
}

void
CExportFlcDlg::PreviewDlgStop()
{
    m_PreviewParam.bExit = TRUE;
}

LONG CExportFlcDlg::OnPreviewCancel(UINT /*lParam*/, LONG /*wParam*/)
{
    m_PreviewParam.bShow = FALSE;
    m_bPreview           = FALSE;
    UpdateData(FALSE);

    return TRUE;
}

void
CExportFlcDlg::OnSaveSettings()
{
    UpdateData();
}
