/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	NewFlcPal.cpp
	CREATED:	15.12.2004
	VERSION:	1.0

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "NewFlcPal.h"

#include "NewFlcDlg.h"
#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFlcPal dialog

CNewFlcPal::CNewFlcPal(CWnd* pParent /*=NULL*/)
  : CDialogX(CNewFlcPal::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNewFlcPal)
    m_nPalType      = 0;
    m_sLastPalPath  = _T("");
    m_nLastPalType  = 0;
    m_bDefPalette   = FALSE;
    m_nBitsPerPixel = BitsPerPixel_8;
    m_bMakePalette  = FALSE;
    m_nPaletteType  = 0;
    m_nPalette      = 0;
    m_sPaletteCur   = _T("");
    m_sSelPalPath   = _T("");
    m_bSelPalPath   = FALSE;
    //}}AFX_DATA_INIT

    SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_2, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_3, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_4, CF_RESIZE_BOTH);

    SetControlInfo(IDC_LAST_PAL_PATH, CF_RESIZE_HOR);
    SetControlInfo(IDC_BROWSE_LAST_PAL_PATH, CF_ANCHOR_RIGHT);

    SetControlInfo(IDC_PAL_PATH, CF_RESIZE_HOR);
    SetControlInfo(IDC_BROWSE_PAL_PATH, CF_ANCHOR_RIGHT);

    m_bDrawGripper = FALSE;
    m_pParent      = (CNewFlcDlg*)pParent;
    m_bChanged     = FALSE;
}

void
CNewFlcPal::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewFlcPal)
    DDX_Control(pDX, IDC_MAKE_DEF_PALETTE, m_btnDefPalette);
    DDX_Control(pDX, IDC_LAST_PAL_PATH, m_wndLastPalPath);
    DDX_Control(pDX, IDC_BROWSE_LAST_PAL_PATH, m_btnBrowseLastCol);
    DDX_Control(pDX, IDC_PAL_TYPE, m_wndPalType);
    DDX_CBIndex(pDX, IDC_PAL_TYPE, m_nPalType);
    DDX_Text(pDX, IDC_LAST_PAL_PATH, m_sLastPalPath);
    DDX_Radio(pDX, IDC_LAST_PAL_ORIGINAL, m_nLastPalType);
    DDX_Check(pDX, IDC_MAKE_DEF_PALETTE, m_bDefPalette);
    DDX_Radio(pDX, IDC_8_BIT, m_nBitsPerPixel);
    DDX_Check(pDX, IDC_MAKE_PALETTE, m_bMakePalette);
    DDX_Control(pDX, IDC_PAL_PATH, m_wndSelPalPath);
    DDX_Control(pDX, IDC_BROWSE_PAL_PATH, m_btnBrowseSelPalPath);
    DDX_Radio(pDX, IDC_PALETTE_CUR, m_nPaletteType);
    DDX_CBIndex(pDX, IDC_COMBO_PALETTES, m_nPalette);
    DDX_Text(pDX, IDC_STATIC_PALETTE_CUR, m_sPaletteCur);
    DDX_Control(pDX, IDC_COMBO_PALETTES, m_wndPalettes);
    DDX_Text(pDX, IDC_PAL_PATH, m_sSelPalPath);
    DDX_Check(pDX, IDC_SELECTIVE_PAL, m_bSelPalPath);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewFlcPal, CDialogX)
//{{AFX_MSG_MAP(CNewFlcPal)
ON_CBN_SELENDOK(IDC_PAL_TYPE, OnSelendokPalType)
ON_BN_CLICKED(IDC_BROWSE_LAST_PAL_PATH, OnBrowseLastColorsPath)
ON_BN_CLICKED(IDC_LAST_PAL_ORIGINAL, OnLastColors)
ON_BN_CLICKED(IDC_8_BIT, OnBitsPerPixel)
ON_BN_CLICKED(IDC_LAST_PAL_HARDCODED, OnLastColors)
ON_BN_CLICKED(IDC_LAST_PAL_SELECTIVE, OnLastColors)
ON_BN_CLICKED(IDC_24_BIT, OnBitsPerPixel)
ON_BN_CLICKED(IDC_MAKE_PALETTE, OnMakePalette)
ON_BN_CLICKED(IDC_PALETTE_CUR, OnPalette)
ON_WM_DESTROY()
ON_WM_CTLCOLOR()
ON_CBN_SELENDOK(IDC_COMBO_PALETTES, OnSelendokComboPalettes)
ON_BN_CLICKED(IDC_PALETTE_CHANGE, OnPalette)
ON_BN_CLICKED(IDC_SELECTIVE_PAL, OnSelectivePal)
ON_BN_CLICKED(IDC_BROWSE_PAL_PATH, OnBrowsePalPath)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFlcPal message handlers

BOOL
CNewFlcPal::OnInitDialog()
{
    CDialogX::OnInitDialog();

    // Fill the Palettes colors combo box
    m_wndPalettes.Initialize();

    //////////////////////////////////////////////////////////////////////////
    m_btnBrowseSelPalPath.SetIcon(IDI_BROWSE);
    m_btnBrowseLastCol.SetIcon(IDI_BROWSE);

    m_wndPalType.AddString(uLoadString(IDS_PAL_TYPE_ACT));
    m_wndPalType.AddString(uLoadString(IDS_FILE_TYPE_MS));
    m_wndPalType.AddString(uLoadString(IDS_FILE_TYPE_JASC));

    m_nPalType      = m_pParent->m_nPalType;
    m_bDefPalette   = m_pParent->m_bDefPalette;
    m_bMakePalette  = m_pParent->m_bMakePalette;
    m_nLastPalType  = m_pParent->m_nLastPalType;
    m_sLastPalPath  = m_pParent->m_sLastPalPath;
    m_nBitsPerPixel = m_pParent->m_nBitsPerPixel;

    if (m_pParent->m_bLeaderHead) {
        GetDlgItem(IDC_LAST_PAL_HARDCODED)->EnableWindow(FALSE);
        GetDlgItem(IDC_LAST_PAL_SELECTIVE)->EnableWindow(FALSE);
    }

    m_nPalette = m_nPaletteCur = m_pParent->m_nPalette;
    m_wndPalettes.GetLBText(m_nPalette, m_sPaletteCur);
    m_brCurrent   = CreateSolidBrush(m_wndPalettes.GetItemColor(m_nPalette));
    m_bSelPalPath = m_pParent->m_bSelPalPath;
    m_sSelPalPath = m_pParent->m_sSelPalPath;

    UpdateData(FALSE);
    UpdateControlsState();
    return TRUE;
}

void
CNewFlcPal::UpdateControlsState()
{
    UpdateData();

    m_btnBrowseLastCol.EnableWindow(m_nLastPalType == PalSelective);
    m_btnBrowseSelPalPath.EnableWindow(m_bSelPalPath);
    m_wndPalettes.EnableWindow(m_nPaletteType == 1 && !m_pParent->m_bLeaderHead);

    BOOL bEnable = !m_pParent->m_bLeaderHead;
    GetDlgItem(IDC_PALETTE_CUR)->EnableWindow(bEnable);
    GetDlgItem(IDC_PALETTE_CHANGE)->EnableWindow(bEnable);
    GetDlgItem(IDC_LAST_PAL_HARDCODED)->EnableWindow(bEnable);
    GetDlgItem(IDC_LAST_PAL_SELECTIVE)->EnableWindow(bEnable);

    if (m_nBitsPerPixel == BitsPerPixel_8) {
        m_btnDefPalette.EnableWindow(TRUE);
    } else {
        m_btnDefPalette.EnableWindow(FALSE);
        m_bDefPalette = TRUE;
        UpdateData(FALSE);
    }
}

void
CNewFlcPal::ApplyChanges()
{
    UpdateData();

    m_pParent->m_nPalType      = m_nPalType;
    m_pParent->m_nLastPalType  = (ELastPalType)m_nLastPalType;
    m_pParent->m_sLastPalPath  = m_sLastPalPath;
    m_pParent->m_bDefPalette   = m_bDefPalette;
    m_pParent->m_bMakePalette  = m_bMakePalette;
    m_pParent->m_nBitsPerPixel = m_nBitsPerPixel;

    m_pParent->m_nPalette = m_nPaletteType == 0 ? m_nPaletteCur : m_nPalette;

    CString sPalette;
    m_wndPalettes.GetLBText(m_nPalette, sPalette);
    m_pParent->m_sPalette = m_nPaletteType == 0 ? m_sPaletteCur : sPalette;

    m_pParent->m_bSelPalPath = m_bSelPalPath;
    m_pParent->m_sSelPalPath = m_sSelPalPath;

    m_bChanged = FALSE;
}

void
CNewFlcPal::OnBrowseLastColorsPath()
{
    CFileDialogX dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T(uLoadString(IDS_FILTER_PAL_IMP)));

    CWinApp* pApp = AfxGetApp();
    CString sTitle(uLoadString(IDS_TITLE_IMP_PAL));

    dlg.m_ofn.lpstrTitle   = sTitle;
    dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(REG_OPTIONS, REG_FILTER_PAL_IMP, 1);

    CString sPath = pApp->GetProfileString(REG_OPTIONS, REG_LAST_PAL, "");

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    m_sLastPalPath = dlg.GetPathName();
    UpdateData(FALSE);

    pApp->WriteProfileString(REG_OPTIONS, REG_LAST_PAL, m_sLastPalPath);
    pApp->WriteProfileInt(REG_OPTIONS, REG_FILTER_PAL_IMP, dlg.m_ofn.nFilterIndex);

    m_bChanged = TRUE;
}

void
CNewFlcPal::OnLastColors()
{
    m_bChanged = TRUE;
    UpdateControlsState();
}

void
CNewFlcPal::OnBitsPerPixel()
{
    m_bChanged = TRUE;

    UpdateControlsState();
}

void
CNewFlcPal::OnMakePalette()
{
    m_bChanged = TRUE;
}

void
CNewFlcPal::OnSelendokPalType()
{
    m_bChanged = TRUE;
}

void
CNewFlcPal::OnSelectivePal()
{
    m_bChanged = TRUE;
    UpdateControlsState();
}

void
CNewFlcPal::OnBrowsePalPath()
{
    CFileDialogX dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T(uLoadString(IDS_FILTER_PAL_IMP)));

    CWinApp* pApp = AfxGetApp();
    CString sTitle(uLoadString(IDS_TITLE_IMP_PAL));

    dlg.m_ofn.lpstrTitle   = sTitle;
    dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(REG_OPTIONS, REG_FILTER_PAL_IMP, 1);

    CString sPath = pApp->GetProfileString(REG_OPTIONS, REG_LAST_PAL, "");

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    m_sSelPalPath = dlg.GetPathName();
    UpdateData(FALSE);

    pApp->WriteProfileString(REG_OPTIONS, REG_LAST_PAL, m_sSelPalPath);
    pApp->WriteProfileInt(REG_OPTIONS, REG_FILTER_PAL_IMP, dlg.m_ofn.nFilterIndex);

    m_bChanged = TRUE;
}

void
CNewFlcPal::OnPalette()
{
    UpdateControlsState();
    m_bChanged = TRUE;
}

HBRUSH
CNewFlcPal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC) {
        // Return handle to our CBrush object
        if (pWnd->GetDlgCtrlID() == IDC_STATIC_CURRENT)
            return m_brCurrent;
    }

    // Normal processing: don't do anything
    return CDialogX::OnCtlColor(pDC, pWnd, nCtlColor);
}

void
CNewFlcPal::OnSelendokComboPalettes()
{
    m_bChanged = TRUE;
    //	UpdateData();
    //
    //	if (m_brCurrent)
    //		DeleteObject(m_brCurrent);
    //
    //	m_brCurrent = CreateSolidBrush(m_wndPalettes.GetItemColor(m_nPalette));
    //	m_wndCurColor.Invalidate();
}

void
CNewFlcPal::OnDestroy()
{
    CDialogX::OnDestroy();

    if (m_brCurrent)
        DeleteObject(m_brCurrent);
}

void
CNewFlcPal::UpdateValues()
{
    if (m_pParent->m_bLeaderHead) {
        m_nLastPalType = PalCurrent;
        UpdateData(FALSE);
    }

    UpdateControlsState();
}