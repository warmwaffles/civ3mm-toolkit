/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ColorReplaceDlg.cpp
	CREATED:	04.11.2004
	VERSION:	1.4

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the CColorReplaceDlg class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ColorReplaceDlg.h"

#include "ReplaceRangeDlg.h"
#include "ColorMoveDlg.h"
#include "ColorBCHSDlg.h"
#include "ColorSelectDlg.h"

#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_SETFOCUS 321

#define REG_LAST_SRC "Src"
#define REG_LAST_DEST "Dest"

#define CIV_PALETTE 0
#define SEL_PALETTE 1

static COLORREF DEF_COLOR = RGB(255, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CColorReplaceDlg dialog

CColorReplaceDlg::CColorReplaceDlg(RGBQUADPAL* pPal, CWnd* pParent /*=NULL*/)
  : CDialogX(CColorReplaceDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CColorReplaceDlg)
    m_sRed        = _T("");
    m_sIndex      = _T("");
    m_sGreen      = _T("");
    m_sBlue       = _T("");
    m_sIndexS     = _T("");
    m_nBlueS      = 0;
    m_nGreenS     = 0;
    m_nRedS       = 0;
    m_sPathD      = _T("");
    m_sPathS      = _T("");
    m_sBlueSrc    = _T("");
    m_sGreenSrc   = _T("");
    m_sIndexSrc   = _T("");
    m_sRedSrc     = _T("");
    m_sBlueSrcS   = _T("");
    m_sGreenSrcS  = _T("");
    m_sIndexSrcS  = _T("");
    m_sRedSrcS    = _T("");
    m_nPalette    = 0;
    m_nSrcPalType = 0;
    //}}AFX_DATA_INIT

    m_bDrawGripper = FALSE;
    m_pPalExt      = pPal;
}

void
CColorReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CColorReplaceDlg)
    DDX_Control(pDX, IDC_SELECT_IMG, m_btnImgSelect);
    DDX_Control(pDX, IDC_REFRESH, m_btnRefresh);
    DDX_Control(pDX, IDC_APPLY, m_btnApply);
    DDX_Control(pDX, IDC_BCHS, m_btnBCHS);
    DDX_Control(pDX, IDC_MOVE_COLORS, m_btnMove);
    DDX_Control(pDX, IDC_STATIC_CURRENTSRC, m_wndCurColorSrc);
    DDX_Control(pDX, IDC_STATIC_SELECTED_SRC, m_wndSelColorSrc);
    DDX_Control(pDX, IDC_SRC_PATH, m_wndPathS);
    DDX_Control(pDX, IDC_SAVE, m_btnSave);
    DDX_Control(pDX, IDC_REPLACE_RANGE, m_btnReplaceRange);
    DDX_Control(pDX, IDC_REPLACE_COLOR, m_btnReplaceColor);
    DDX_Control(pDX, IDC_DEST_PATH, m_wndPathD);
    DDX_Control(pDX, IDC_BROWSE_SRC_PATH, m_btnBrowseS);
    DDX_Control(pDX, IDC_BROWSE_DEST_PATH, m_btnBrowseD);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDC_SELECT_COLOR, m_btnSelect);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_STATIC_SELECTED, m_wndSelColor);
    DDX_Control(pDX, IDC_STATIC_CURRENT, m_wndCurColor);
    DDX_Control(pDX, IDC_COLOR_RED_S, m_wndRedS);
    DDX_Control(pDX, IDC_COLOR_GREEN_S, m_wndGreenS);
    DDX_Control(pDX, IDC_COLOR_BLUE_S, m_wndBlueS);
    DDX_Text(pDX, IDC_COLOR_RED, m_sRed);
    DDX_Text(pDX, IDC_COLOR_INDEX, m_sIndex);
    DDX_Text(pDX, IDC_COLOR_GREEN, m_sGreen);
    DDX_Text(pDX, IDC_COLOR_BLUE, m_sBlue);
    DDX_Text(pDX, IDC_COLOR_INDEX_S, m_sIndexS);
    DDX_Text(pDX, IDC_COLOR_BLUE_S, m_nBlueS);
    DDX_Text(pDX, IDC_COLOR_GREEN_S, m_nGreenS);
    DDX_Text(pDX, IDC_COLOR_RED_S, m_nRedS);
    DDX_Text(pDX, IDC_DEST_PATH, m_sPathD);
    DDX_Text(pDX, IDC_SRC_PATH, m_sPathS);
    DDX_Text(pDX, IDC_COLOR_BLUE_SRC, m_sBlueSrc);
    DDX_Text(pDX, IDC_COLOR_GREEN_SRC, m_sGreenSrc);
    DDX_Text(pDX, IDC_COLOR_INDEX_SRC, m_sIndexSrc);
    DDX_Text(pDX, IDC_COLOR_RED_SRC, m_sRedSrc);
    DDX_Text(pDX, IDC_COLOR_BLUE_SRC_S, m_sBlueSrcS);
    DDX_Text(pDX, IDC_COLOR_GREEN_SRC_S, m_sGreenSrcS);
    DDX_Text(pDX, IDC_COLOR_INDEX_SRC_S, m_sIndexSrcS);
    DDX_Text(pDX, IDC_COLOR_RED_SRC_S, m_sRedSrcS);
    DDX_Control(pDX, IDC_COMBO_PALETTES, m_wndPalettes);
    DDX_CBIndex(pDX, IDC_COMBO_PALETTES, m_nPalette);
    DDX_Radio(pDX, IDC_CIV_PALETTE, m_nSrcPalType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorReplaceDlg, CDialogX)
//{{AFX_MSG_MAP(CColorReplaceDlg)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_SELECT_COLOR, OnSelectColor)
ON_EN_CHANGE(IDC_COLOR_BLUE_S, OnChangeColorBlueS)
ON_EN_CHANGE(IDC_COLOR_GREEN_S, OnChangeColorGreenS)
ON_EN_CHANGE(IDC_COLOR_RED_S, OnChangeColorRedS)
ON_WM_CTLCOLOR()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_REPLACE_RANGE, OnReplaceRange)
ON_BN_CLICKED(IDC_BROWSE_DEST_PATH, OnBrowseDestPath)
ON_BN_CLICKED(IDC_BROWSE_SRC_PATH, OnBrowseSrcPath)
ON_BN_CLICKED(IDC_REPLACE_COLOR, OnReplaceColor)
ON_BN_CLICKED(IDC_SAVE, OnSave)
ON_CBN_SELENDOK(IDC_COMBO_PALETTES, OnSelendokComboPalettes)
ON_BN_CLICKED(IDC_MOVE_COLORS, OnMoveColors)
ON_BN_CLICKED(IDC_SEL_PALETTE, OnSelPalette)
ON_BN_CLICKED(IDC_CIV_PALETTE, OnCivPalette)
ON_BN_CLICKED(IDC_BCHS, OnBCHS)
ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
ON_BN_CLICKED(IDC_APPLY, OnApply)
ON_BN_CLICKED(IDC_SELECT_IMG, OnSelectImg)
//}}AFX_MSG_MAP
ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
ON_MESSAGE(CPN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorReplaceDlg message handlers

BOOL
CColorReplaceDlg::OnInitDialog()
{
    CDialogX::OnInitDialog();

    LoadPosition(this);

    // Set icon to the dialog caption
    SetIcon(AfxGetApp()->LoadIcon(IDI_COLOR_REPLACE), FALSE);

    // Fill the Palettes colors combo box
    m_wndPalettes.Initialize();

    // Inicialize a destination palette
    if (m_pPalExt)
        memcpy(&m_rgbPalCur, m_pPalExt, RGBQUADPAL_SIZE);
    else
        ClearPalette(m_rgbPalCur);

    memcpy(&m_rgbPalD, &m_rgbPalCur, RGBQUADPAL_SIZE);

    // Inicialize a source palette
    m_nSrcPalType = AfxGetApp()->GetProfileInt(RegKey(), REG_PAL_TYPE, SEL_PALETTE);
    if (m_nSrcPalType == CIV_PALETTE) {
        if (!ChangePalette(0)) {
            m_nSrcPalType = SEL_PALETTE;
            ClearPalette(m_rgbPalS);
        }
    } else
        ClearPalette(m_rgbPalS);

    // Setup buttons
    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);
    m_btnSelect.SetIcon(IDI_SELECT_COLOR);
    m_btnBrowseD.SetIcon(IDI_BROWSE);
    m_btnBrowseS.SetIcon(IDI_BROWSE);
    m_btnSave.SetIcon(IDI_SAVE);
    m_btnReplaceRange.SetIcon(IDI_COLOR_RANGE);
    m_btnReplaceColor.SetIcon(IDI_REPLACE_COLOR);
    m_btnMove.SetIcon(IDI_MOVE_COLOR);
    m_btnBCHS.SetIcon(IDI_BCHS);
    m_btnApply.SetIcon(IDI_APPLY);
    m_btnRefresh.SetIcon(IDI_REFRESH);
    m_btnImgSelect.SetIcon(IDI_COLOR_SELECT);

    CWnd* pWnd = NULL;
    CRect rect;

    pWnd = GetDlgItem(IDC_DEST_PALETTE_POS);
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (!m_wndColorTableD.Create(rect.TopLeft(), &m_rgbPalD, this, IDC_COLOR_TABLE)) {
        TRACE("Cannot create destination ColorTable window\r\n");
        EndDialog(IDCANCEL);
        return FALSE;
    }

    m_crDest = m_crCurDest = m_wndColorTableD.GetColour(0);
    m_brDest               = CreateSolidBrush(m_crDest);
    m_brCurDest            = CreateSolidBrush(m_crDest);

    pWnd = GetDlgItem(IDC_SRC_PALETTE_POS);
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (!m_wndColorTableS.Create(rect.TopLeft(), &m_rgbPalS, this, IDC_COLOR_TABLE_S)) {
        TRACE("Cannot create source ColorTable window\r\n");
        EndDialog(IDCANCEL);
        return FALSE;
    }

    m_crSrc = m_crCurSrc = m_wndColorTableS.GetColour(0);
    m_brSrc              = CreateSolidBrush(m_crSrc);
    m_brCurSrc           = CreateSolidBrush(m_crSrc);

    m_wndRedS.LimitText(3);
    m_wndGreenS.LimitText(3);
    m_wndBlueS.LimitText(3);

    UpdateColorInfo(IDC_COLOR_TABLE);
    UpdateColorInfo(IDC_COLOR_TABLE_S);

    SetTimer(TIMER_SETFOCUS, 100, NULL);

    UpdateControlsState();
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorReplaceDlg::OnSelEndOK()
//
LONG
CColorReplaceDlg::OnSelEndOK(UINT lParam, LONG wParam)
{
    COLORREF col = (COLORREF)lParam;
    if (wParam == IDC_COLOR_TABLE) {
        if (m_crCurDest != col) {
            m_crCurDest = col;

            if (m_brCurDest)
                DeleteObject(m_brCurDest);

            m_brCurDest = CreateSolidBrush(m_crCurDest);

            ApplyColorChanges();
        }

        UpdateColorInfo(IDC_COLOR_TABLE);
        m_wndSelColor.Invalidate();
    } else {
        if (m_crCurSrc != col) {
            m_crCurSrc = col;

            if (m_brCurSrc)
                DeleteObject(m_brCurSrc);

            m_brCurSrc = CreateSolidBrush(m_crCurSrc);
        }

        UpdateColorInfo(IDC_COLOR_TABLE_S);
        m_wndSelColorSrc.Invalidate();
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorReplaceDlg::OnSelEndCancel()
//
LONG
CColorReplaceDlg::OnSelEndCancel(UINT lParam, LONG wParam)
{
    OnCancel();
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorReplaceDlg::OnSelChange()
//
LONG
CColorReplaceDlg::OnSelChange(UINT lParam, LONG wParam)
{
    COLORREF col = (COLORREF)lParam;
    if (wParam == IDC_COLOR_TABLE) {
        if (m_crDest != col) {
            m_crDest = col;

            if (m_brDest)
                DeleteObject(m_brDest);

            m_brDest = CreateSolidBrush(m_crDest);
        }

        UpdateColorInfo(IDC_COLOR_TABLE);
        m_wndCurColor.Invalidate();
    } else {
        if (m_crSrc != col) {
            m_crSrc = col;

            if (m_brSrc)
                DeleteObject(m_brSrc);

            m_brSrc = CreateSolidBrush(m_crSrc);
        }

        UpdateColorInfo(IDC_COLOR_TABLE_S);
        m_wndCurColorSrc.Invalidate();
    }

    return TRUE;
}

void
CColorReplaceDlg::UpdateColorInfo(UINT nID)
{
    if (nID == IDC_COLOR_TABLE) {
        // Current color
        m_sRed   = uInt2Str(GetRValue(m_crDest));
        m_sGreen = uInt2Str(GetGValue(m_crDest));
        m_sBlue  = uInt2Str(GetBValue(m_crDest));

        int nIndex = m_wndColorTableD.m_nCurrent;
        if (nIndex != -1)
            m_sIndex = uInt2Str(nIndex);
        else
            m_sIndex.Empty();

        // Selected color
        m_nRedS   = GetRValue(m_crCurDest);
        m_nGreenS = GetGValue(m_crCurDest);
        m_nBlueS  = GetBValue(m_crCurDest);

        nIndex = m_wndColorTableD.m_nSelected;
        if (nIndex != -1)
            m_sIndexS = uInt2Str(nIndex);
        else
            m_sIndexS.Empty();
    } else {
        // Current color
        m_sRedSrc   = uInt2Str(GetRValue(m_crSrc));
        m_sGreenSrc = uInt2Str(GetGValue(m_crSrc));
        m_sBlueSrc  = uInt2Str(GetBValue(m_crSrc));

        int nIndex = m_wndColorTableS.m_nCurrent;
        if (nIndex != -1)
            m_sIndexSrc = uInt2Str(nIndex);
        else
            m_sIndexSrc.Empty();

        // Selected color
        m_sRedSrcS   = uInt2Str(GetRValue(m_crCurSrc));
        m_sGreenSrcS = uInt2Str(GetGValue(m_crCurSrc));
        m_sBlueSrcS  = uInt2Str(GetBValue(m_crCurSrc));

        nIndex = m_wndColorTableS.m_nSelected;
        if (nIndex != -1)
            m_sIndexSrcS = uInt2Str(nIndex);
        else
            m_sIndexSrcS.Empty();
    }

    UpdateData(FALSE);
}

void
CColorReplaceDlg::OnDestroy()
{
    AfxGetApp()->WriteProfileInt(RegKey(), REG_PAL_TYPE, m_nSrcPalType);

    CDialogX::OnDestroy();

    if (m_brDest)
        DeleteObject(m_brDest);

    if (m_brCurDest)
        DeleteObject(m_brCurDest);

    if (m_brSrc)
        DeleteObject(m_brSrc);

    if (m_brCurSrc)
        DeleteObject(m_brCurSrc);

    SavePosition(this);
}

void
CColorReplaceDlg::OnSelectColor()
{
    COLORREF col = m_crCurDest;
    if (ShowColorDialog(col, this)) {
        m_crCurDest = col;
        ChangeSelectedRGB();
        ApplyColorChanges();
    }
}

void
CColorReplaceDlg::OnOK()
{
    OnApply();
    CDialogX::OnOK();
}

void
CColorReplaceDlg::OnCancel()
{
    // Nothing changed...
    CDialogX::OnCancel();
}

void
CColorReplaceDlg::OnChangeColorRedS()
{
    CDataExchange dx(this, TRUE);

    CString sValue;
    m_wndRedS.GetWindowText(sValue);

    if (!sValue.IsEmpty()) {
        DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);

        if (m_nRedS < 0) {
            m_nRedS               = 0;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);
        } else if (m_nRedS > 255) {
            m_nRedS               = 255;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);
        }
    } else {
        m_nRedS               = 0;
        dx.m_bSaveAndValidate = FALSE;
        DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);
    }

    m_crCurDest = RGB(m_nRedS, m_nGreenS, m_nBlueS);
    ChangeSelectedRGB();
}

void
CColorReplaceDlg::OnChangeColorGreenS()
{
    CDataExchange dx(this, TRUE);

    CString sValue;
    m_wndGreenS.GetWindowText(sValue);

    if (!sValue.IsEmpty()) {
        DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);

        if (m_nGreenS < 0) {
            m_nGreenS             = 0;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);
        } else if (m_nGreenS > 255) {
            m_nGreenS             = 255;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);
        }
    } else {
        m_nGreenS             = 0;
        dx.m_bSaveAndValidate = FALSE;
        DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);
    }

    m_crCurDest = RGB(m_nRedS, m_nGreenS, m_nBlueS);
    ChangeSelectedRGB();
}

void
CColorReplaceDlg::OnChangeColorBlueS()
{
    CDataExchange dx(this, TRUE);

    CString sValue;
    m_wndBlueS.GetWindowText(sValue);

    if (!sValue.IsEmpty()) {
        DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);

        if (m_nBlueS < 0) {
            m_nBlueS              = 0;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);
        } else if (m_nBlueS > 255) {
            m_nBlueS              = 255;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);
        }
    } else {
        m_nBlueS              = 0;
        dx.m_bSaveAndValidate = FALSE;
        DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);
    }

    m_crCurDest = RGB(m_nRedS, m_nGreenS, m_nBlueS);
    ChangeSelectedRGB();
}

//////////////////
// Got WM_CTLCOLOR: return handle of background brush to use.
//
HBRUSH
CColorReplaceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC) {
        // Return handle to our CBrush object
        switch (pWnd->GetDlgCtrlID()) {
            case IDC_STATIC_CURRENT:
                return m_brDest;

            case IDC_STATIC_SELECTED:
                return m_brCurDest;

            case IDC_STATIC_CURRENTSRC:
                return m_brSrc;

            case IDC_STATIC_SELECTED_SRC:
                return m_brCurSrc;
        }
    }

    // Normal processing: don't do anything
    return CDialogX::OnCtlColor(pDC, pWnd, nCtlColor);
}

void
CColorReplaceDlg::ChangeSelectedRGB()
{
    m_wndColorTableD.ChangeSelectedColor(m_crCurDest);

    if (m_brCurDest)
        DeleteObject(m_brCurDest);

    m_brCurDest = CreateSolidBrush(m_crCurDest);

    UpdateColorInfo(IDC_COLOR_TABLE);
    m_wndSelColor.Invalidate();
}

BOOL
CColorReplaceDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
            case VK_RETURN:
            case VK_ESCAPE:
                ::TranslateMessage(pMsg);
                ::DispatchMessage(pMsg);
                return TRUE;
        }
    }

    return CDialogX::PreTranslateMessage(pMsg);
}

void
CColorReplaceDlg::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == TIMER_SETFOCUS) {
        KillTimer(TIMER_SETFOCUS);
        m_wndColorTableD.SetFocus();
        return;
    }

    CDialogX::OnTimer(nIDEvent);
}

void
CColorReplaceDlg::UpdateControlsState()
{
    UpdateData();

    m_wndPalettes.EnableWindow(m_nSrcPalType == CIV_PALETTE);
    m_wndPathS.EnableWindow(m_nSrcPalType == SEL_PALETTE);
    m_btnBrowseS.EnableWindow(m_nSrcPalType == SEL_PALETTE);
}

void
CColorReplaceDlg::OnBrowseDestPath()
{
    CFileDialogX dlg(
      TRUE, NULL, CFileX::GetShortFileName(m_sPathD), OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, _T(uLoadString(IDS_FILTER_GET_PALETTE)), this);

    dlg.m_ofn.hInstance = AfxGetInstanceHandle();

    CString sTitle(uLoadString(IDS_TITLE_DEST_PALETTE));
    dlg.m_ofn.lpstrTitle = sTitle;

    CWinApp* pApp          = AfxGetApp();
    CString sPath          = pApp->GetProfileString(RegKey(), REG_LAST_DEST, "");
    dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(RegKey(), REG_FILTER, 1);

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    sPath = dlg.GetPathName();

    pApp->WriteProfileString(RegKey(), REG_LAST_DEST, sPath);
    pApp->WriteProfileInt(RegKey(), REG_FILTER, dlg.m_ofn.nFilterIndex);

    if (GetPalette(sPath, m_rgbPalCur)) {
        memcpy(&m_rgbPalD, &m_rgbPalCur, RGBQUADPAL_SIZE);
        m_sPathD = sPath;
    } else {
        ClearPalette(m_rgbPalD);
        m_sPathD.Empty();
    }

    UpdateData(FALSE);

    SetPaletteD();

    UpdateControlsState();
}

void
CColorReplaceDlg::OnBrowseSrcPath()
{
    CFileDialogX dlg(
      TRUE, NULL, CFileX::GetShortFileName(m_sPathS), OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, _T(uLoadString(IDS_FILTER_GET_PALETTE)), this);

    dlg.m_ofn.hInstance = AfxGetInstanceHandle();

    CString sTitle(uLoadString(IDS_TITLE_SRC_PALETTE));
    dlg.m_ofn.lpstrTitle = sTitle;

    CWinApp* pApp          = AfxGetApp();
    CString sPath          = pApp->GetProfileString(RegKey(), REG_LAST_SRC, "");
    dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(RegKey(), REG_FILTER, 1);

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    sPath = dlg.GetPathName();

    pApp->WriteProfileString(RegKey(), REG_LAST_SRC, sPath);
    pApp->WriteProfileInt(RegKey(), REG_FILTER, dlg.m_ofn.nFilterIndex);

    if (GetPalette(sPath, m_rgbPalS))
        m_sPathS = sPath;
    else {
        ClearPalette(m_rgbPalD);
        m_sPathD.Empty();
    }

    UpdateData(FALSE);

    SetPaletteS();

    UpdateControlsState();
}

void
CColorReplaceDlg::OnReplaceColor()
{
    m_crCurDest = m_crCurSrc;
    ChangeSelectedRGB();
    ApplyColorChanges();
}

void
CColorReplaceDlg::OnSave()
{
    ExportPalette(m_rgbPalD);
}

BOOL
CColorReplaceDlg::GetPalette(CString sPath, RGBQUADPAL& palette)
{
    CWaitCursor wait;

    ClearPalette(palette);

    CString sExt = uGetFileExt(sPath);

    if (sExt.CompareNoCase(ACT_EXT) == 0 || sExt.CompareNoCase(PAL_EXT) == 0) {
        return ImportPaletteFromFile(sPath, palette);
    } else if (sExt.CompareNoCase(FLC_EXT) == 0) {
        FILE* fp;
        BYTE* pBuffer = NULL;
        FlcHeader flcHeader;
        FlicAnimHeader civ3Header;

        try {
            fp = fopen(sPath, "rb");

            if (!fp) {
                AfxMessageBox(IDS_ERR_GET_PALETTE, MB_ICONERROR);
                return FALSE;
            }

            fread(&flcHeader, sizeof(flcHeader), 1, fp);
            fread(&civ3Header, sizeof(civ3Header), 1, fp);

            WORD nFrmHeight = flcHeader.height;
            WORD nFrmWidth  = flcHeader.width;

            if (nFrmHeight > WND_H || nFrmWidth > WND_W) {
                AfxMessageBox(IDS_ERR_GET_PALETTE, MB_ICONERROR);
                return FALSE;
            }

            pBuffer           = new BYTE[nFrmWidth * nFrmHeight];
            WORD nCurFrame    = 0;                // current frame
            WORD nTotalFrames = flcHeader.frames; // total frames
            WORD nChunk;
            UINT size;

            FRAMEHEADER frameh;
            CHUNKHEADER chunkh;

            while (nCurFrame < nTotalFrames) {
                // Read FLC frame header
                fread(&frameh, sizeof(FRAMEHEADER), 1, fp);

                if (frameh.Type != FRAME_TYPE) {
                    // FLC's info data for only Autodesk Animator Pro
                    // Skip this frame (and subtract total frames this)
                    fseek(fp, frameh.Size - sizeof(frameh), SEEK_CUR);
                    nTotalFrames--;
                    continue;
                }

                size = 0;

                for (nChunk = 0; nChunk < frameh.Count; nChunk++) {
                    if (nChunk > 4) {
                        ASSERT(FALSE);
                        break;
                    }

                    // Read FLC chunk header
                    fread(&chunkh, sizeof(CHUNKHEADER), 1, fp);

                    switch (chunkh.Type) {
                        // FLC Color chunk 256
                        case COLOR_256: {
                            chunkh.Size = 778;
                            CFlcView::ReadCOLOR_256(pBuffer, fp, palette);
                            fclose(fp);
                            SAFE_DELETE_MULTI(pBuffer);
                            return TRUE;
                        }
                        default:
                            fseek(fp, chunkh.Size - sizeof(CHUNKHEADER), SEEK_CUR);
                            continue;
                    }

                    size += chunkh.Size;
                } //for (nChunk = 0; nChunk < frameh.Chunks; nChunk++)

                nCurFrame++;
            } //while (nCurFrame < nTotalFrames)
        } catch (CMemoryException* e) {
            TCHAR szCause[1024];
            e->GetErrorMessage(szCause, 1024);
            AfxMessageBox(szCause, MB_ICONERROR);
            e->Delete();

            SAFE_DELETE_MULTI(pBuffer);
            return FALSE;
        } catch (...) {
            AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorReplaceDlg::GetPalette"), MB_ICONERROR);
            if (fp)
                fclose(fp);

            SAFE_DELETE_MULTI(pBuffer);
            return FALSE;
        }

        fclose(fp);
        SAFE_DELETE_MULTI(pBuffer);

        AfxMessageBox(IDS_ERR_GET_PALETTE, MB_ICONERROR);
        return FALSE;
    } else if (sExt.CompareNoCase(C3F_EXT) == 0) {
        C3FSTRUCT c3f;
        if (!ReadC3F(sPath, &c3f))
            return FALSE;

        CString sPalPath(sPath);
        sPalPath.Delete(sPalPath.GetLength() - 3, 3);
        switch (c3f.PalType) {
            case PalACT:
                sPalPath += ACT_EXT;
                break;
            case PalMS:
            case PalJASC:
                sPalPath += PAL_EXT;
                break;
            default:
                ASSERT(FALSE);
        }

        return ImportPaletteFromFile(sPalPath, palette);
    } else if (sExt.CompareNoCase(PCX_EXT) == 0 || sExt.CompareNoCase(BMP_EXT) == 0) {
        CFlcImage img;
        try {
            img.LoadFile(sPath);
            if (!img.IsLoaded()) {
                AfxMessageBox(IDS_ERR_GET_PALETTE, MB_ICONERROR);
                return FALSE;
            }

            if (img.m_fi.BitsPerPixel < 8) {
                AfxMessageBox(IDS_ERR_COLOR_DEPTH);
                return FALSE;
            }

            // Convert the current bitmap to 8 bits per pixel with an optimized palette
            GRAPHX::ColorResBitmap(&img.m_hbmp, &img.m_hbmp, 8, CRF_FLOYDSTEINDITHERING | CRF_OPTIMIZEDPALETTE, NULL, NULL, 0, NULL, NULL);

            DWORD dwColors = 0;

            // Get the number of unique colors in an image
            GRAPHX::GetBitmapColorCount(&img.m_hbmp, dwColors);

            // Load palette with selected colors from a bitmap handle's palette
            GRAPHX::GetBitmapColors(&img.m_hbmp, 0, dwColors, palette.pal);
        } catch (CGraphXException* e) {
            AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
            e->Delete();
            return FALSE;
        } catch (...) {
            ASSERT(FALSE);
            AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorReplaceDlg::GetPalette"), MB_ICONERROR);
            return FALSE;
        }

        return TRUE;
    }

    AfxMessageBox(IDS_ERR_GET_PALETTE, MB_ICONERROR);
    return FALSE;
}

void
CColorReplaceDlg::OnReplaceRange()
{
    CReplaceRangeDlg dlg(&m_rgbPalD, &m_rgbPalS);
    dlg.DoModal();

    SetPaletteD();
}

void
CColorReplaceDlg::OnSelendokComboPalettes()
{
    // Here should be changed FLC file's palette
    int nOldPalette = m_nPalette;

    UpdateData();

    if (nOldPalette == m_nPalette)
        return;

    ClearPalette(m_rgbPalS);

    if (!ChangePalette(m_nPalette)) {
        ClearPalette(m_rgbPalS);
        AfxMessageBox(IDS_CANNOT_GET_PAL);
        return;
    }

    SetPaletteS();
}

void
CColorReplaceDlg::OnMoveColors()
{
    CColorMoveDlg dlg(&m_rgbPalD);
    dlg.DoModal();

    SetPaletteD();
}

void
CColorReplaceDlg::OnSelPalette()
{
    UpdateControlsState();

    if (m_sPathS.IsEmpty())
        ClearPalette(m_rgbPalS);
    else if (!GetPalette(m_sPathS, m_rgbPalS))
        ClearPalette(m_rgbPalS);

    UpdateData(FALSE);

    SetPaletteS();
}

void
CColorReplaceDlg::OnCivPalette()
{
    UpdateControlsState();

    ClearPalette(m_rgbPalS);

    if (!ChangePalette(m_nPalette)) {
        ClearPalette(m_rgbPalS);
        m_nPalette = 0;
        m_wndPalettes.SetItemColor(0, DEF_COLOR);
        UpdateData(FALSE);
    }

    SetPaletteS();
}

BOOL
CColorReplaceDlg::ChangePalette(BYTE nColor)
{
    ASSERT(nColor < 33);

    // Copy default palette to the current palette
    if (nColor == 0) {
        ClearPalette(m_rgbPalS);
        m_wndPalettes.SetItemColor(0, DEF_COLOR);

        return TRUE;
    }

    nColor--;

    CString sImgPath, sFileName;
    sFileName.Format("ntp%d.pcx", nColor);
    if (nColor < 10)
        sFileName.Insert(3, "0");

    switch (MAIN_APP->GetPaletteType()) {
        case PalGame: {
            if (!MAIN_APP->IsCivRegistered()) {
                AfxMessageBox(IDS_ERR_PAL_GAME);
                return FALSE;
            }

            sImgPath = MAIN_APP->GetCivInstallPath();
            sImgPath += "\\Art\\Units\\Palettes\\" + sFileName;
            break;
        }

        case PalRes: {
            UINT nId = IDR_PCX_00 + nColor;

            HMODULE hInst = AfxGetApp()->m_hInstance;

            if (hInst == 0)
                hInst = AfxGetInstanceHandle();

            // Get current directory
            char buf[MAX_PATH];
            if (GetModuleFileName(hInst, buf, MAX_PATH) == 0) {
                AfxMessageBox(IDS_ERR_PAL_RES);
                return FALSE;
            }

            sImgPath = uGetPathName(buf) + "Palettes\\";
            sFileName.Insert(0, sImgPath);

            // In case the file isn't exist - make it from resources
            if (!uIsFileExist(sFileName)) {
                // In case directory isn't exist - try to create it
                if (!uIsDirExist(sImgPath) && !uCreateNewFolder(sImgPath)) {
                    AfxMessageBox(IDS_ERR_PAL_RES);
                    return FALSE;
                }

                BOOL bRes        = TRUE;
                DWORD dwSize     = 0;
                BOOL bCompressed = FALSE;
                LPBYTE lpBuf     = NULL;

                // Create file from resources
                HRSRC resInfo = FindResource(hInst, MAKEINTRESOURCE(nId), "PCX"); // find the resource

                if (resInfo == NULL) // if can't find the resourse
                {
                    AfxMessageBox(IDS_ERR_PAL_RES);
                    return FALSE;
                }

                HGLOBAL hRes = LoadResource(hInst, resInfo); // load the resource

                if (hRes == NULL) // if can't load the resourse
                {
                    AfxMessageBox(IDS_ERR_PAL_RES);
                    return FALSE;
                }

                // Retrive resource data
                dwSize           = SizeofResource(hInst, resInfo); // get the resource size
                UINT FAR* lpnRes = (UINT FAR*)LockResource(hRes);
                lpBuf            = new BYTE[dwSize + 1];
                memcpy(lpBuf, lpnRes, dwSize);
                lpBuf[dwSize] = 0;

                UnlockResource(hRes);
                FreeResource(hRes); // free the resource

                // Create a file
                CFile fl;
                try {
                    fl.Open(sFileName, CFile::modeCreate | CFile::modeWrite | CFile::modeRead);

                    // Set data to the file
                    fl.Write(lpBuf, dwSize);
                    fl.Close();
                } catch (...) {
                    bRes = FALSE;
                    AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CAnimationView::PaletteFromRes"), MB_ICONERROR);
                }

                SAFE_DELETE_MULTI(lpBuf);

                if (!bRes) {
                    AfxMessageBox(IDS_ERR_PAL_RES);
                    return FALSE;
                }
            }

            sImgPath = sFileName;
            break;
        }

        case PalDir: {
            sImgPath = MAIN_APP->GetPaletteDirectory();

            if (!uIsDirExist(sImgPath)) {
                AfxMessageBox(IDS_ERR_PAL_DIR);
                return FALSE;
            }

            sImgPath += "\\" + sFileName;
            if (!uIsFileExist(sImgPath)) {
                AfxMessageBox(IDS_ERR_PAL_DIR);
                return FALSE;
            }
            break;
        }
    }

    if (!uIsFileExist(sImgPath)) {
        AfxMessageBox(uFormatString(IDS_ERR_PALETTE_FILE, sImgPath), MB_ICONERROR);
        return FALSE;
    }

    return SetCivColorPalette(sImgPath);
}

BOOL
CColorReplaceDlg::GetCivColorPalette(RGBQUADPAL* pPal, LPCTSTR lpszImgPath /*=NULL*/)
{
    if (lpszImgPath != NULL) {
        if (!uIsFileExist(lpszImgPath))
            return FALSE;

        CPcx pcxImg;

        // Load related file with specified civilization color palette
        if (!pcxImg.LoadBgdImgFile(lpszImgPath))
            return FALSE;

        // Copy pcx palette to the current palette
        //ZeroMemory(pPal, RGBQUADPAL_SIZE);
        memcpy(pPal, &pcxImg.m_rgbPal, RGBQUADPAL_SIZE);
    } else {
        // Copy destination palette to the current palette
        //ZeroMemory(pPal, RGBQUADPAL_SIZE);
        memcpy(pPal, &m_rgbPalS, RGBQUADPAL_SIZE);
    }

    return TRUE;
}

BOOL
CColorReplaceDlg::SetCivColorPalette(LPCTSTR lpszImgPath /*=NULL*/)
{
    BYTE nColor = 0;
    if (lpszImgPath != NULL) {
        if (!uIsFileExist(lpszImgPath))
            return FALSE;

        CPcx pcxImg;

        // Load related file with specified civilization color palette
        if (!pcxImg.LoadBgdImgFile(lpszImgPath))
            return FALSE;

        // Get first 64 colors (civilization related) from PCX and set them to the FLC color palette
        memcpy(m_rgbPalS.pal, pcxImg.m_rgbPal.pal, sizeof(RGBQUAD) * 64);
        //		for (BYTE i = 0; i < 64; i++)
        //		{
        //			memcpy(&m_rgbPalS.pal[i], &pcx.m_rgbPal.pal[i], sizeof(RGBQUAD) * 64);
        //		}

        CString sColor(lpszImgPath);
        sColor = sColor.Mid(sColor.GetLength() - 6, 2);
        nColor = atoi(sColor) + 1;
    }

    return TRUE;
}

void
CColorReplaceDlg::ClearPalette(RGBQUADPAL& palette)
{
    RGBQUAD colorDef;
    colorDef.rgbRed      = 255;
    colorDef.rgbGreen    = 0;
    colorDef.rgbBlue     = 0;
    colorDef.rgbReserved = 0;

    for (int i = 0; i < COLOR8; i++)
        memcpy(&palette.pal[i], &colorDef, sizeof(RGBQUAD));
}

void
CColorReplaceDlg::SetPaletteD()
{
    if (!m_wndColorTableD.SetPalette(&m_rgbPalD))
        return;

    if (m_brDest)
        DeleteObject(m_brDest);

    if (m_brCurDest)
        DeleteObject(m_brCurDest);

    m_crDest = m_crCurDest = m_wndColorTableD.GetColour(0);
    m_brDest               = CreateSolidBrush(m_crDest);
    m_brCurDest            = CreateSolidBrush(m_crDest);

    m_wndSelColor.Invalidate();
    m_wndCurColor.Invalidate();
}

void
CColorReplaceDlg::SetPaletteS()
{
    if (!m_wndColorTableS.SetPalette(&m_rgbPalS))
        return;

    if (m_brSrc)
        DeleteObject(m_brSrc);

    if (m_brCurSrc)
        DeleteObject(m_brCurSrc);

    m_crSrc = m_crCurSrc = m_wndColorTableS.GetColour(0);
    m_brSrc              = CreateSolidBrush(m_crSrc);
    m_brCurSrc           = CreateSolidBrush(m_crSrc);

    m_wndCurColorSrc.Invalidate();
    m_wndSelColorSrc.Invalidate();
}

void
CColorReplaceDlg::OnBCHS()
{
    CColorBCHSDlg dlg(&m_rgbPalD);
    dlg.DoModal();

    SetPaletteD();
}

void
CColorReplaceDlg::ApplyColorChanges()
{
    int idx                     = m_wndColorTableD.m_nSelected;
    m_rgbPalD.pal[idx].rgbRed   = GetRValue(m_crCurDest);
    m_rgbPalD.pal[idx].rgbGreen = GetGValue(m_crCurDest);
    m_rgbPalD.pal[idx].rgbBlue  = GetBValue(m_crCurDest);
}

void
CColorReplaceDlg::OnRefresh()
{
    // Discard palette changes
    if (m_pPalExt)
        memcpy(&m_rgbPalD, m_pPalExt, RGBQUADPAL_SIZE);
    else
        memcpy(&m_rgbPalD, &m_rgbPalCur, RGBQUADPAL_SIZE);

    SetPaletteD();
}

void
CColorReplaceDlg::OnApply()
{
    // applay palette changes
    for (int i = 0; i < COLOR8; i++) {
        m_rgbPalD.pal[i].rgbRed   = GetRValue(m_wndColorTableD.m_crColours[i].crColour);
        m_rgbPalD.pal[i].rgbGreen = GetGValue(m_wndColorTableD.m_crColours[i].crColour);
        m_rgbPalD.pal[i].rgbBlue  = GetBValue(m_wndColorTableD.m_crColours[i].crColour);
    }

    if (m_pPalExt)
        memcpy(m_pPalExt, &m_rgbPalD, RGBQUADPAL_SIZE);
    else
        memcpy(&m_rgbPalCur, &m_rgbPalD, RGBQUADPAL_SIZE);
}

void
CColorReplaceDlg::OnSelectImg()
{
    CColorSelectDlg dlg(&m_rgbPalD);
    dlg.DoModal();

    SetPaletteD();
}
