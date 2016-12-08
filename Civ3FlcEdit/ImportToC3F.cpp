/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ImportToC3F.cpp
	CREATED:	01.11.2004
	VERSION:	1.5

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the CImportToC3F class.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ImportToC3F.h"
#include "DocManagerEx.h"

#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_LAST_SRC "Src"
#define REG_LAST_DEST "Dest"
#define REG_SHOW_LINES "Show Lines"
#define REG_LAST_DELAY "Delay"
#define REG_SHOW_GRID "Show Grid"
#define REG_SAVE_SETTINGS "Save"
#define REG_FRM_W "Width"
#define REG_FRM_H "Height"
#define REG_OFFSET_X "XOffset"
#define REG_OFFSET_Y "YOffset"
#define REG_GRID "Grid"

enum ESrcPalType
{
    ESrcPalDef = 0,
    ESrcPalSel = 1
};

/////////////////////////////////////////////////////////////////////////////
// CImportToC3F dialog

CImportToC3F::CImportToC3F(CWnd* pParent /*=NULL*/)
  : CDialog(CImportToC3F::IDD, pParent)
{
    //{{AFX_DATA_INIT(CImportToC3F)
    m_nFrmWidth     = 50;
    m_nFrmHeight    = 50;
    m_nXOffset      = 95;
    m_nYOffset      = 80;
    m_sOutputPath   = _T("");
    m_nPalType      = -1;
    m_sInfo         = _T("");
    m_sTargetPath   = _T("");
    m_sDelay        = _T("125");
    m_sPalPath      = _T("");
    m_nSrcPalType   = ESrcPalSel;
    m_nFileType     = -1;
    m_bShowLines    = TRUE;
    m_bPreview      = TRUE;
    m_bShowGrid     = FALSE;
    m_bSaveSettings = FALSE;
    m_nBitsPerPixel = -1;
    m_bGrid         = TRUE;
    //}}AFX_DATA_INIT
}

void
CImportToC3F::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CImportToC3F)
    DDX_Control(pDX, IDC_CENTERING, m_btnCentering);
    DDX_Control(pDX, IDC_FILE_TYPE, m_wndFileType);
    DDX_Control(pDX, IDC_PAL_PATH, m_wndPalPath);
    DDX_Control(pDX, IDC_BROWSE_PAL_PATH, m_btnBrowsePal);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_BROWSE_TARGET_PATH, m_btnBrowseTarget);
    DDX_Control(pDX, IDC_DELAY, m_wndDelay);
    DDX_Control(pDX, IDC_BROWSE_OUTPUT_PATH, m_btnBrowseOutput);
    DDX_Control(pDX, IDC_TARGET_PATH, m_wndTargetPath);
    DDX_Control(pDX, IDC_STATIC_INFO, m_wndInfo);
    DDX_Control(pDX, IDC_SPIN_DELAY, m_wndDelaySpin);
    DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
    DDX_Control(pDX, IDC_PAL_TYPE, m_wndPalType);
    DDX_Control(pDX, IDC_OUTPUT_PATH, m_wndOutputPath);
    DDX_Control(pDX, IDC_FRM_WIDTH_SPIN, m_wndFrmWidthSpin);
    DDX_Control(pDX, IDC_FRM_WIDTH, m_wndFrmWidth);
    DDX_Control(pDX, IDC_FRM_TOP_SPIN, m_wndYOffsetSpin);
    DDX_Control(pDX, IDC_FRM_TOP, m_wndYOffset);
    DDX_Control(pDX, IDC_FRM_LEFT_SPIN, m_wndXOffsetSpin);
    DDX_Control(pDX, IDC_FRM_LEFT, m_wndXOffset);
    DDX_Control(pDX, IDC_FRM_HEIGHT_SPIN, m_wndFrmHeightSpin);
    DDX_Control(pDX, IDC_FRM_HEIGHT, m_wndFrmHeight);
    DDX_Text(pDX, IDC_FRM_WIDTH, m_nFrmWidth);
    DDX_Text(pDX, IDC_FRM_HEIGHT, m_nFrmHeight);
    DDX_Text(pDX, IDC_FRM_LEFT, m_nXOffset);
    DDX_Text(pDX, IDC_FRM_TOP, m_nYOffset);
    DDX_Text(pDX, IDC_OUTPUT_PATH, m_sOutputPath);
    DDX_CBIndex(pDX, IDC_PAL_TYPE, m_nPalType);
    DDX_Text(pDX, IDC_STATIC_INFO, m_sInfo);
    DDX_Text(pDX, IDC_TARGET_PATH, m_sTargetPath);
    DDX_Text(pDX, IDC_DELAY, m_sDelay);
    DDX_Text(pDX, IDC_PAL_PATH, m_sPalPath);
    DDX_Radio(pDX, IDC_PAL_DEFAULT, m_nSrcPalType);
    DDX_CBIndex(pDX, IDC_FILE_TYPE, m_nFileType);
    DDX_Check(pDX, IDC_SHOW_LINES, m_bShowLines);
    DDX_Check(pDX, IDC_PREVIEW_CHECK, m_bPreview);
    DDX_Check(pDX, IDC_SHOW_GRID, m_bShowGrid);
    DDX_Check(pDX, IDC_SAVE_SETTINGS, m_bSaveSettings);
    DDX_Radio(pDX, IDC_8_BIT, m_nBitsPerPixel);
    DDX_Check(pDX, IDC_GRID, m_bGrid);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImportToC3F, CDialog)
//{{AFX_MSG_MAP(CImportToC3F)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BROWSE_TARGET_PATH, OnBrowseTargetPath)
ON_BN_CLICKED(IDC_BROWSE_OUTPUT_PATH, OnBrowseOutputPath)
ON_BN_CLICKED(IDC_BROWSE_PAL_PATH, OnBrowsePalPath)
ON_BN_CLICKED(IDC_PAL_DEFAULT, OnPalChanged)
ON_WM_PAINT()
ON_EN_CHANGE(IDC_FRM_WIDTH, OnChangeFrmWidth)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_WIDTH_SPIN, OnDeltaposFrmWidthSpin)
ON_EN_CHANGE(IDC_FRM_HEIGHT, OnChangeFrmHeight)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_HEIGHT_SPIN, OnDeltaposFrmHeightSpin)
ON_EN_CHANGE(IDC_FRM_LEFT, OnChangeFrmLeft)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_LEFT_SPIN, OnDeltaposFrmLeftSpin)
ON_EN_CHANGE(IDC_FRM_TOP, OnChangeFrmTop)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_TOP_SPIN, OnDeltaposFrmTopSpin)
ON_BN_CLICKED(IDC_SHOW_LINES, OnShowLines)
ON_EN_CHANGE(IDC_DELAY, OnChangeDelay)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, OnDeltaposSpinDelay)
ON_BN_CLICKED(IDC_CENTERING, OnCentering)
ON_BN_CLICKED(IDC_PREVIEW_CHECK, OnPreviewCheck)
ON_BN_CLICKED(IDC_SHOW_GRID, OnShowGrid)
ON_BN_CLICKED(IDC_SAVE_SETTINGS, OnSaveSettings)
ON_BN_CLICKED(IDC_8_BIT, OnBitsPerPixel)
ON_BN_CLICKED(IDC_PAL_SELECTED, OnPalChanged)
ON_BN_CLICKED(IDC_24_BIT, OnBitsPerPixel)
ON_BN_CLICKED(IDC_GRID, OnGrid)
//}}AFX_MSG_MAP
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TIMER_SLIDER, OnReleasedcaptureTimerSlider)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportToC3F message handlers

BOOL
CImportToC3F::OnInitDialog()
{
    CDialog::OnInitDialog();

    LoadPosition(this);

    SetIcon(AfxGetApp()->LoadIcon(IDI_IMPORT_C3F), FALSE);

    m_btnOK.SetIcon(IDI_IMPORT_C3F);
    m_btnCancel.SetIcon(IDI_CANCEL);
    m_btnCentering.SetIcon(IDI_CENTERING);
    m_btnCentering.SetTooltipText(uLoadString(IDS_CENTERING));

    m_btnBrowsePal.SetIcon(IDI_BROWSE);
    m_btnBrowseTarget.SetIcon(IDI_BROWSE);
    m_btnBrowseOutput.SetIcon(IDI_BROWSE);

    m_wndFrmWidth.LimitText(3);
    m_wndFrmHeight.LimitText(3);
    m_wndFrmWidthSpin.SetRange(1, WND_W);
    m_wndFrmHeightSpin.SetRange(1, WND_H);

    m_wndXOffset.LimitText(3);
    m_wndYOffset.LimitText(3);
    m_wndXOffsetSpin.SetRange(0, WND_W - 1);
    m_wndYOffsetSpin.SetRange(0, WND_H - 1);

    CWinApp* pApp = AfxGetApp();

    m_bPreview   = pApp->GetProfileInt(RegKey(), REG_PREVIEW, 1);
    m_bShowLines = pApp->GetProfileInt(RegKey(), REG_SHOW_LINES, 1);
    m_bShowGrid  = AfxGetApp()->GetProfileInt(RegKey(), REG_SHOW_GRID, 1);

    m_nFileType = pApp->GetProfileInt(RegKey(), REG_FILE_TYPE, PCX_TYPE);
    if (m_nFileType > MAX_TYPE)
        m_nFileType = PCX_TYPE;
    m_wndFileType.AddString(uLoadString(IDS_FILE_TYPE_PCX));
    m_wndFileType.AddString(uLoadString(IDS_FILE_TYPE_BMP));
    m_wndFileType.SetCurSel(m_nFileType);

    m_nPalType = pApp->GetProfileInt(RegKey(), REG_PAL_TYPE, PalACT);
    if (m_nPalType > PalJASC)
        m_nPalType = PalACT;

    m_wndPalType.AddString(uLoadString(IDS_PAL_TYPE_ACT));
    m_wndPalType.AddString(uLoadString(IDS_FILE_TYPE_MS));
    m_wndPalType.AddString(uLoadString(IDS_FILE_TYPE_JASC));

    m_sPalPath = pApp->GetProfileString(RegKey(), REG_LAST_PAL, "");

    m_wndPreview.SetWindowPos(NULL, 0, 0, WND_CX, WND_CY, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

    m_wndDelay.SetLimitText(4);
    m_wndDelaySpin.SetRange(FRM_DELAY_MIN, FRM_DELAY_MAX);

    // Create timer slider control
    CWnd* pWnd = GetDlgItem(IDC_TIMER_SLIDER_POS);

    CRect rect;
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

    // Initialise controls
    if (!m_wndTimerSlider.Create(
          WS_VISIBLE | WS_CHILD | TBS_HORZ | TBS_BOTTOM | TBS_AUTOTICKS | TBS_TOOLTIPS, rect, this, IDC_TIMER_SLIDER)) {
        ASSERT(FALSE);
    }

    m_wndTimerSlider.SetRangeMin(FRM_DELAY_MIN);
    m_wndTimerSlider.SetRangeMax(FRM_DELAY_MAX);
    m_wndTimerSlider.SetTicFreq(100);

    m_nBitsPerPixel = pApp->GetProfileInt(RegKey(), REG_COL_DEPTH, BitsPerPixel_8);
    m_bSaveSettings = pApp->GetProfileInt(RegKey(), REG_SAVE_SETTINGS, FALSE);

    m_nDelay = 125;
    m_nGrid  = 1;

    if (m_bSaveSettings) {
        m_nFrmWidth  = pApp->GetProfileInt(RegKey(), REG_FRM_W, 50);
        m_nFrmHeight = pApp->GetProfileInt(RegKey(), REG_FRM_H, 50);
        m_nXOffset   = pApp->GetProfileInt(RegKey(), REG_OFFSET_X, 95);
        m_nYOffset   = pApp->GetProfileInt(RegKey(), REG_OFFSET_Y, 80);
        m_nDelay     = pApp->GetProfileInt(RegKey(), REG_LAST_DELAY, 125);
        m_sDelay     = uInt2Str(m_nDelay);
        m_bGrid      = pApp->GetProfileInt(RegKey(), REG_GRID, 1);
        m_nGrid      = m_bGrid ? 1 : 0;
    }

    UpdateData(FALSE);
    m_wndTimerSlider.SetPos(m_nDelay);
    UpdateControlsState();

    return TRUE;
}

void
CImportToC3F::OnDestroy()
{
    ///////////////////////////////////////////////////
    // Save settings changes
    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(RegKey(), REG_FILE_TYPE, m_nFileType);
    pApp->WriteProfileInt(RegKey(), REG_PAL_TYPE, m_nPalType);
    pApp->WriteProfileInt(RegKey(), REG_SAVE_SETTINGS, m_bSaveSettings);
    pApp->WriteProfileInt(RegKey(), REG_PREVIEW, m_bPreview);
    pApp->WriteProfileInt(RegKey(), REG_SHOW_LINES, m_bShowLines);
    //	pApp->WriteProfileInt(RegKey(), REG_LAST_PAL, m_sPalPath);
    pApp->WriteProfileInt(RegKey(), REG_SHOW_GRID, m_bShowGrid);
    pApp->WriteProfileInt(RegKey(), REG_COL_DEPTH, m_nBitsPerPixel);

    if (m_bSaveSettings) {
        CWinApp* pApp = AfxGetApp();
        pApp->WriteProfileInt(RegKey(), REG_FRM_W, m_nFrmWidth);
        pApp->WriteProfileInt(RegKey(), REG_FRM_H, m_nFrmHeight);
        pApp->WriteProfileInt(RegKey(), REG_OFFSET_X, m_nXOffset);
        pApp->WriteProfileInt(RegKey(), REG_OFFSET_Y, m_nYOffset);
        pApp->WriteProfileInt(RegKey(), REG_LAST_DELAY, m_nDelay);
        pApp->WriteProfileInt(RegKey(), REG_GRID, m_bGrid);
    }

    SavePosition(this);
    CDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// Update controls state.
//
void
CImportToC3F::UpdateControlsState()
{
    UpdateData();

    if (IsLoaded()) {
        m_btnOK.EnableWindow(!m_sOutputPath.IsEmpty());
        m_wndDelay.EnableWindow(TRUE);
        m_wndDelaySpin.EnableWindow(TRUE);
        m_wndFileType.EnableWindow(TRUE);
        m_wndPalType.EnableWindow(TRUE);
        m_wndFrmWidthSpin.EnableWindow(TRUE);
        m_wndFrmWidth.EnableWindow(TRUE);
        m_wndYOffsetSpin.EnableWindow(TRUE);
        m_wndYOffset.EnableWindow(TRUE);
        m_wndXOffsetSpin.EnableWindow(TRUE);
        m_wndXOffset.EnableWindow(TRUE);
        m_wndFrmHeightSpin.EnableWindow(TRUE);
        m_wndFrmHeight.EnableWindow(TRUE);
        m_btnCentering.EnableWindow(TRUE);

        m_wndTimerSlider.EnableWindow(TRUE);

        GetDlgItem(IDC_PAL_DEFAULT)->EnableWindow(m_fi.BitsPerPixel == 8);
        GetDlgItem(IDC_PAL_SELECTED)->EnableWindow(TRUE);

        m_btnBrowsePal.EnableWindow(m_nSrcPalType == ESrcPalSel);
        m_wndPalPath.EnableWindow(m_nSrcPalType == ESrcPalSel);
    } else {
        m_btnOK.EnableWindow(FALSE);

        m_wndDelay.EnableWindow(FALSE);
        m_wndDelaySpin.EnableWindow(FALSE);
        m_wndFileType.EnableWindow(FALSE);
        m_wndPalType.EnableWindow(FALSE);
        m_wndFrmWidthSpin.EnableWindow(FALSE);
        m_wndFrmWidth.EnableWindow(FALSE);
        m_wndYOffsetSpin.EnableWindow(FALSE);
        m_wndYOffset.EnableWindow(FALSE);
        m_wndXOffsetSpin.EnableWindow(FALSE);
        m_wndXOffset.EnableWindow(FALSE);
        m_wndFrmHeightSpin.EnableWindow(FALSE);
        m_wndFrmHeight.EnableWindow(FALSE);
        m_btnCentering.EnableWindow(FALSE);

        m_wndTimerSlider.EnableWindow(FALSE);

        GetDlgItem(IDC_PAL_DEFAULT)->EnableWindow(FALSE);
        GetDlgItem(IDC_PAL_SELECTED)->EnableWindow(FALSE);

        m_btnBrowsePal.EnableWindow(FALSE);
        m_wndPalPath.EnableWindow(FALSE);
    }

    UpdateData(FALSE);
    UpdatePreview();
}

//////////////////////////////////////////////////////////////////////////
// Select source image file.
//
void
CImportToC3F::OnBrowseTargetPath()
{
    try {
        CFileDialogX dlg(TRUE,
                         NULL,
                         NULL,
                         OFN_ENABLESIZING | OFN_EXPLORER | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
                         _T(uLoadString(IDS_FILTER_IMG)),
                         this);

        dlg.m_ofn.hInstance = AfxGetInstanceHandle();

        CString sTitle(uLoadString(IDS_TITLE_IMPORT_C3F));
        dlg.m_ofn.lpstrTitle = sTitle;

        CWinApp* pApp          = AfxGetApp();
        dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(RegKey(), REG_FILTER, 1);

        CString sPath = pApp->GetProfileString(RegKey(), REG_LAST_SRC, "");

        if (uIsFileExist(sPath))
            dlg.m_ofn.lpstrInitialDir = sPath;

        if (dlg.DoModal() != IDOK)
            return;

        sPath = dlg.GetPathName();

        pApp->WriteProfileString(RegKey(), REG_LAST_SRC, sPath);
        pApp->WriteProfileInt(RegKey(), REG_FILTER, dlg.m_ofn.nFilterIndex);

        ///////////////////////////////////////////////
        LoadFile(sPath);
        if (!IsLoaded())
            return;

        if (m_fi.BitsPerPixel < 8) {
            AfxMessageBox(IDS_ERR_COLOR_DEPTH);
            return;
        }

        switch (m_fi.Format) {
            case FILE_PCX: // ZSoft PCX
            case FILE_BMP: // Windows BMP
                break;
            default:
                AfxMessageBox(IDS_ERR_INVALID_IMAGE_FORMAT);
                return;
        }

        m_sTargetPath = sPath;
        m_sInfo       = uFormatString(IDS_IMG_INFO, m_fi.Width, m_fi.Height, m_fi.BitsPerPixel);

        if (m_fi.BitsPerPixel != 8)
            m_nSrcPalType = ESrcPalSel;
    } catch (CGraphXException* e) {
        AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
        e->Delete();
        return;
    } catch (CFileException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        return;
    } catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CImportToC3F::OnBrowseTargetPath"), MB_ICONERROR);
        return;
    }

    UpdateData(FALSE);
    UpdateControlsState();
}

//////////////////////////////////////////////////////////////////////////
// Select target path for destination C3F Stiryboard.
//
void
CImportToC3F::OnBrowseOutputPath()
{
    CFileDialogX dlg(FALSE,
                     C3F_EXT,
                     CFileX::GetShortFileName(m_sTargetPath),
                     OFN_ENABLESIZING | OFN_EXPLORER | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
                     _T(uLoadString(IDS_FILTER_C3F)),
                     this);

    dlg.m_ofn.hInstance = AfxGetInstanceHandle();

    CString sTitle(uLoadString(IDS_TITLE_DEST_C3F));
    dlg.m_ofn.lpstrTitle = sTitle;

    CWinApp* pApp = AfxGetApp();
    CString sPath = pApp->GetProfileString(RegKey(), REG_LAST_DEST, "");

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    m_sOutputPath = dlg.GetPathName();
    //	if (m_sOutputPath.Right(4).CompareNoCase(".c3f") != 0)
    //		m_sOutputPath += ".c3f";

    UpdateData(FALSE);

    pApp->WriteProfileString(RegKey(), REG_LAST_DEST, m_sOutputPath);

    UpdateControlsState();
}

BOOL
CImportToC3F::PreTranslateMessage(MSG* pMsg)
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

    return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// Select palette file in case Selective palette type.
//
void
CImportToC3F::OnBrowsePalPath()
{
    CFileDialogX dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T(uLoadString(IDS_FILTER_PAL_IMP)), this);

    CWinApp* pApp = AfxGetApp();
    CString sTitle(uLoadString(IDS_TITLE_IMP_PAL));

    dlg.m_ofn.lpstrTitle   = sTitle;
    dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(RegKey(), REG_FILTER_PAL_IMP, 1);

    CString sPath = pApp->GetProfileString(RegKey(), REG_LAST_PAL, "");

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    sPath = dlg.GetPathName();

    pApp->WriteProfileString(RegKey(), REG_LAST_PAL, sPath);
    pApp->WriteProfileInt(RegKey(), REG_FILTER_PAL_IMP, dlg.m_ofn.nFilterIndex);

    m_sPalPath = sPath;
    UpdateData(FALSE);
    UpdateControlsState();
}

void
CImportToC3F::OnPalChanged()
{
    UpdateControlsState();
}

//////////////////////////////////////////////////////////////////////////
// Draw preview.
//
void
CImportToC3F::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (m_wndPreview.GetSafeHwnd()) {
        CRect rc;
        m_wndPreview.GetWindowRect(rc);
        ScreenToClient(rc);
        //		dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));

        if (m_bPreview && IsLoaded()) {
            rc.DeflateRect(1, 1);
            CRect rcDest(rc);
            rcDest.left   = rcDest.left + m_nXOffset;
            rcDest.top    = rcDest.top + m_nYOffset;
            rcDest.right  = rcDest.left + m_nFrmWidth;
            rcDest.bottom = rcDest.top + m_nFrmHeight;

            GRAPHX::PaintDC(
              dc.m_hDC, &m_hbmp, CRect(m_nGrid, m_nGrid, m_nFrmWidth + m_nGrid, m_nFrmHeight + m_nGrid), NULL, rcDest, rc, SRCCOPY);

            rc.InflateRect(1, 1);
        }

        CPen* pOldPen;
        LOGBRUSH lb;
        lb.lbStyle = BS_SOLID;

        int cx0 = rc.left + rc.Width() / 2;
        int cy0 = rc.top + rc.Height() / 2;

        //////////////////////////////////////////////////////////////////////////
        // Draw Lines
        if (m_bShowLines) {
            rc.DeflateRect(1, 1);

            lb.lbColor = RGB(0, 0, 0);
            CPen pen(PS_SOLID | PS_ENDCAP_ROUND | PS_GEOMETRIC, 1, &lb);
            pOldPen = dc.SelectObject(&pen);

            dc.MoveTo(rc.left, rc.top);
            dc.LineTo(rc.right, rc.bottom);

            dc.MoveTo(rc.right, rc.top);
            dc.LineTo(rc.left, rc.bottom);

            dc.MoveTo(cx0, rc.top);
            dc.LineTo(cx0, rc.bottom);

            dc.MoveTo(rc.left, cy0);
            dc.LineTo(rc.right, cy0);

            // Return old pen
            dc.SelectObject(pOldPen);

            rc.InflateRect(1, 1);
        } //if (m_bShowLines)

        //////////////////////////////////////////////////////////////////////////
        // Draw grid
        if (m_bShowGrid) {
            lb.lbColor = RGB(255, 0, 0);
            CPen penRed(PS_SOLID | PS_ENDCAP_ROUND | PS_GEOMETRIC, 1, &lb);
            pOldPen = dc.SelectObject(&penRed);

            dc.MoveTo(cx0, cy0 - 48);
            dc.LineTo(cx0 - 64, cy0 - 16);
            dc.LineTo(cx0, cy0 + 16);
            dc.LineTo(cx0 + 64, cy0 - 16);
            dc.LineTo(cx0, cy0 - 48);

            dc.MoveTo(cx0 - 32, cy0);
            dc.LineTo(cx0 + 32, cy0 - 32);

            dc.MoveTo(cx0 + 32, cy0);
            dc.LineTo(cx0 - 32, cy0 - 32);

            // Return old pen
            dc.SelectObject(pOldPen);
        } //if (m_bShowGrid)

        dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
    }
}

//////////////////////////////////////////////////////////////////////////
// Invalidate preview window.
//
void
CImportToC3F::UpdatePreview()
{
    if (m_wndPreview.GetSafeHwnd()) {
        CRect rc;
        m_wndPreview.GetWindowRect(rc);
        ScreenToClient(rc);
        InvalidateRect(rc);
    }
}

void
CImportToC3F::OnChangeFrmWidth()
{
    if (m_wndFrmWidth.GetSafeHwnd()) {
        CDataExchange dx(this, TRUE);
        DDX_Text(&dx, IDC_FRM_WIDTH, m_nFrmWidth);

        if (m_nFrmWidth > WND_W) {
            m_nFrmWidth           = WND_W;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_FRM_WIDTH, m_nFrmWidth);
        }

        OnChangeFrmLeft();
    }
}

void
CImportToC3F::OnDeltaposFrmWidthSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeFrmWidth();

    *pResult = 0;
}

void
CImportToC3F::OnChangeFrmHeight()
{
    if (m_wndFrmHeight.GetSafeHwnd()) {
        CDataExchange dx(this, TRUE);
        DDX_Text(&dx, IDC_FRM_HEIGHT, m_nFrmHeight);

        if (m_nFrmHeight > WND_H) {
            m_nFrmHeight          = WND_H;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_FRM_HEIGHT, m_nFrmHeight);
        }

        OnChangeFrmTop();
    }
}

void
CImportToC3F::OnDeltaposFrmHeightSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeFrmHeight();

    *pResult = 0;
}

void
CImportToC3F::OnChangeFrmLeft()
{
    if (m_wndXOffset.GetSafeHwnd()) {
        UpdateData();

        if (m_nXOffset > (WND_W - m_nFrmWidth)) {
            m_nXOffset = WND_W - m_nFrmWidth;
            UpdateData(FALSE);
        }

        UpdatePreview();
    }
}

void
CImportToC3F::OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeFrmLeft();

    *pResult = 0;
}

void
CImportToC3F::OnChangeFrmTop()
{
    if (m_wndYOffset.GetSafeHwnd()) {
        UpdateData();

        if (m_nYOffset > (WND_H - m_nFrmHeight)) {
            m_nYOffset = WND_H - m_nFrmHeight;
            UpdateData(FALSE);
        }

        UpdatePreview();
    }
}

void
CImportToC3F::OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeFrmTop();

    *pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
// Show or hide vertical, horizontal and diagonal lines on preview window.
//
void
CImportToC3F::OnShowLines()
{
    UpdateData();
    UpdatePreview();
}

void
CImportToC3F::OnShowGrid()
{
    UpdateData();
    UpdatePreview();
}

//////////////////////////////////////////////////////////////////////////
// Change timer delay
//
void
CImportToC3F::OnChangeDelay()
{
    if (m_wndDelay.GetSafeHwnd()) {
        UpdateData();

        m_sDelay.Remove(' ');
        m_nDelay = atoi(m_sDelay);
        if (m_nDelay < FRM_DELAY_MIN) {
            m_nDelay = FRM_DELAY_MIN;
            m_sDelay = uInt2Str(m_nDelay);
        } else if (m_nDelay > FRM_DELAY_MAX) {
            m_nDelay = FRM_DELAY_MAX;
            m_sDelay = uInt2Str(m_nDelay);
        }

        CDataExchange dx(this, FALSE);
        DDX_Text(&dx, IDC_DELAY, m_sDelay);

        m_wndTimerSlider.SetPos(m_nDelay);
    }
}

//////////////////////////////////////////////////////////////////////////
// Change timer delay
//
void
CImportToC3F::OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeDelay();

    *pResult = 0;
}

void
CImportToC3F::OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_nDelay = m_wndTimerSlider.GetPos();
    m_sDelay = uInt2Str(m_nDelay);

    CDataExchange dx(this, FALSE);
    DDX_Text(&dx, IDC_DELAY, m_sDelay);

    *pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
// Centering frame by X and Y offsets.
//
void
CImportToC3F::OnCentering()
{
    m_nXOffset = (WND_W - m_nFrmWidth) / 2;
    m_nYOffset = (WND_H - m_nFrmHeight) / 2;
    UpdateData(FALSE);
    UpdatePreview();
}

//////////////////////////////////////////////////////////////////////////
// Show or hide frame preview.
//
void
CImportToC3F::OnPreviewCheck()
{
    UpdateData();
    UpdatePreview();
}

void
CImportToC3F::OnOK()
{
    // Try to convert image to C3F Storyboard
    if (!ImportToC3F())
        return;

    CDialog::OnOK();

    if (MAIN_APP->IsExpOpen()) {
        if (MAIN_APP->IsExpAsk()) {
            if (AfxMessageBox(uFormatString(IDS_ASK_OPEN_C3F_IMP, m_sOutputPath), MB_ICONQUESTION | MB_YESNO) != IDYES)
                return;
        }

        CDocTemplate* pDocTemplate = MAIN_APP->GetDocManager()->GetDocTemplateC3f();
        if (pDocTemplate)
            pDocTemplate->OpenDocumentFile(m_sOutputPath);
    } else
        AfxMessageBox(IDS_IMP_C3F_OK, MB_ICONINFORMATION);
}

//////////////////////////////////////////////////////////////////////////
// Import to C3F Storyboard
//
BOOL
CImportToC3F::ImportToC3F()
{
    if (!IsLoaded())
        return FALSE;

    UpdateData();

    if (m_nFrmWidth < 1) {
        AfxMessageBox(IDS_IMP_WRONG_WIDTH);
        return FALSE;
    }

    if (m_nFrmHeight < 1) {
        AfxMessageBox(IDS_IMP_WRONG_HEIGHT);
        return FALSE;
    }

    if ((UINT)m_fi.Width < m_nFrmWidth + m_nGrid * 2) {
        AfxMessageBox(IDS_IMP_SMALL_WIDTH);
        return FALSE;
    }

    if ((UINT)m_fi.Height < m_nFrmHeight + m_nGrid * 2) {
        AfxMessageBox(IDS_IMP_SMALL_HEIGHT);
        return FALSE;
    }

    // Calculate how many frames per direction
    int nFrmCount = m_fi.Width / m_nFrmWidth;
    if ((nFrmCount * m_nFrmWidth + nFrmCount * m_nGrid + m_nGrid) > (UINT)m_fi.Width)
        nFrmCount--;

    if (nFrmCount < 1) {
        AfxMessageBox(IDS_IMP_SMALL_WIDTH);
        return FALSE;
    }

    // Calculate how many directions
    BOOL bLeaderHead = FALSE;
    int nDirCount    = m_fi.Height / m_nFrmHeight;
    if ((nDirCount * m_nFrmHeight + nDirCount * m_nGrid + m_nGrid) > (UINT)m_fi.Height)
        nDirCount--;

    if (nDirCount < 1) {
        AfxMessageBox(IDS_IMP_SMALL_HEIGHT);
        return FALSE;
    }

    if (nDirCount > 1 && nDirCount < 8) {
        if (AfxMessageBox(IDS_IMP_DIR_LESS_8, MB_ICONQUESTION | MB_YESNO) != IDYES)
            return FALSE;
        nDirCount = 1;
    } else if (nDirCount > 8) {
        if (AfxMessageBox(IDS_IMP_DIR_MORE_8, MB_ICONQUESTION | MB_YESNO) != IDYES)
            return FALSE;
        nDirCount = 8;
    }

    CFlcImage imgDest;     // destination bitmap
    RGBQUADPAL palette;    // palette to be saved in file
    L_UINT32 dwColors = 0; //COLOR8;	// Number of colors in the bitmap

    CWaitCursor wait;

    try {
        // In case selective palette type
        if (m_nSrcPalType == ESrcPalSel) {
            if (m_sPalPath.IsEmpty()) {
                AfxMessageBox(IDS_IMP_SELECT_PAL);
                return FALSE;
            }

            // Import palette from file
            if (!ImportPaletteFromFile(m_sPalPath, palette)) {
                AfxMessageBox(uFormatString(IDS_IMP_PAL_ERR, m_sPalPath));
                return FALSE;
            }

            // Copy palette to destination bitmap palette
            //			memcpy(imgDest.m_rgbPal, palette.pal, RGBQUADPAL_SIZE);
        } //if (m_nSrcPalType == ESrcPalSel)
        else {
            // in case source image has more or less than 8 bits per pixel
            if (m_hbmp.BitsPerPixel != BITS_8) {
                // Convert the current bitmap to 8 bits per pixel with an optimized palette
                GRAPHX::ColorResBitmap(&m_hbmp, &m_hbmp, 8, CRF_FLOYDSTEINDITHERING | CRF_OPTIMIZEDPALETTE, NULL, NULL, 0, NULL, NULL);
            }

            // Get the number of unique colors in an image
            GRAPHX::GetBitmapColorCount(&m_hbmp, dwColors);

            if (dwColors != COLOR8) {
                AfxMessageBox(uFormatString(IDS_ERR_COLORS_COUNT, dwColors));
                return FALSE;
            }

            GRAPHX::GetBitmapColors(&m_hbmp, 0, dwColors, palette.pal);
            // Load palette with selected colors from a bitmap handle's palette
            //			GRAPHX::GetBitmapColors(&m_hbmp, 0, dwColors, imgDest.m_rgbPal);//palette.pal);
        } // else - if (m_nSrcPalType == ESrcPalSel)

        // In case parameters are similar with leaderhead's standards
        if (nDirCount == 1 && (m_nFrmWidth == 200 && m_nFrmHeight == 240)) {
            if (AfxMessageBox(IDS_IMP_ASK_LEADERHEAD, MB_ICONQUESTION | MB_YESNO) == IDYES)
                bLeaderHead = TRUE;
        }

/////////////////////////
//clock - start
#ifdef _DEBUG
        clock_t start, finish;
        double duration;
        start = clock();
#endif
        /////////////////////////

        // Prepare information for C3F file
        C3FSTRUCT c3f;
        CString sName = CFileX::GetShortFileName(m_sOutputPath);
        memcpy(c3f.Name, sName, sName.GetLength());
        c3f.Name[sName.GetLength()] = 0;
        c3f.Width                   = (UINT)nFrmCount * m_nFrmWidth + nFrmCount * GRID_SIZE + GRID_SIZE;
        c3f.Height                  = (UINT)nDirCount * m_nFrmHeight + nDirCount * GRID_SIZE + GRID_SIZE;
        c3f.FrmWidth                = (BYTE)m_nFrmWidth;
        c3f.FrmHeight               = (BYTE)m_nFrmHeight;
        c3f.FrmCount                = (WORD)nFrmCount;
        c3f.DirCount                = (BYTE)nDirCount;
        c3f.Delay                   = (WORD)m_nDelay;
        c3f.XOffset                 = (BYTE)m_nXOffset;
        c3f.YOffset                 = (BYTE)m_nYOffset;
        c3f.FileType                = m_nFileType == 0 ? FILE_PCX : FILE_BMP;
        c3f.PalType                 = (BYTE)m_nPalType;
        c3f.LeaderHead              = bLeaderHead;

        ///////////////////////////////////////////////////////
        imgDest.m_sPath = CFileX::GetFolder(m_sOutputPath) + sName + (m_nFileType == PCX_TYPE ? ".pcx" : ".bmp");

        // Create an empty Storyboard
        if (!imgDest.CreateStoryboard(c3f.Width, c3f.Height, c3f.FrmWidth, c3f.FrmHeight, c3f.DirCount, c3f.FrmCount, BITS_24)) {
            AfxMessageBox(IDS_ERR_CREATE_STORYBOARD, MB_ICONERROR);
            return FALSE;
        }

        /////////////////////////////////////////////////////////
        // In case it's BMP file - convert it to PCX and reload again
        if (m_fi.Format == FILE_BMP) {
            CString sTempFile = uGetTempFileName("SB");

            // create copy of an image file
            if (!CopyFile(m_sPath, sTempFile, FALSE)) {
                AfxMessageBox(uFormatString(IDS_ERR_SB_LOAD, m_sPath), MB_ICONERROR);
                return FALSE;
            }

            // Save bitmap as PCX file type
            GRAPHX::SaveBitmap(sTempFile, &m_hbmp, FILE_PCX, BITS_24);
            if (GRAPHX::LoadFile(
                  (LPTSTR)(LPCTSTR)sTempFile, &m_hbmp, BITS_24, ORDER_BGR, LOADFILE_ALLOCATE | LOADFILE_STORE, NULL, NULL, NULL, NULL) !=
                SUCCESS) {
                AfxMessageBox(uFormatString(IDS_ERR_SB_LOAD, m_sPath), MB_ICONERROR);
                return FALSE;
            }

            DeleteFile(sTempFile);
        }

        if (m_hbmp.BitsPerPixel == BITS_8) {
            GRAPHX::ColorResBitmap(&m_hbmp, &m_hbmp, BITS_24, CRF_BYTEORDERBGR, NULL, NULL, NULL, NULL, NULL);
        }

        /////////////////////////////////////////////////
        // Fill bitmap with data from each frames, one by one

        BITMAPHANDLE hTempBmp; // temporary bitmap

        // Creaye a temporary bitmap
        GRAPHX::CreateBitmap(&hTempBmp, TYPE_CONV, c3f.FrmWidth, c3f.FrmHeight, BITS_24, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);

        WORD nTotalFrames = c3f.DirCount * c3f.FrmCount;
        WORD nCurFrame    = 0;
        WORD nCX          = GRID_SIZE;
        WORD nCY          = GRID_SIZE;
        WORD nSrcCX       = m_nGrid;
        WORD nSrcCY       = m_nGrid;

        // Move through frames
        for (WORD nCur = 0; nCur < nTotalFrames; nCur++) {
            if (nCurFrame == c3f.FrmCount) {
                nCurFrame = 0; // to start a new direction

                nCX = GRID_SIZE;                  // start from the left end
                nCY += c3f.FrmHeight + GRID_SIZE; // move to the next row

                nSrcCX = m_nGrid;                  // start from the left end
                nSrcCY += c3f.FrmHeight + m_nGrid; // move to the next row
            }

            GRAPHX::CopyBitmapRect(&hTempBmp, &m_hbmp, nSrcCX, nSrcCY, c3f.FrmWidth, c3f.FrmHeight);
            GRAPHX::CombineBitmap(
              &imgDest.m_hbmp, nCX, nCY, c3f.FrmWidth, c3f.FrmHeight, &hTempBmp, 0, 0, CB_OP_ADD | CB_DST_0); //CB_OP_ADD | CB_DST_0);

            nCX += c3f.FrmWidth + GRID_SIZE;  // move to the next column
            nSrcCX += c3f.FrmWidth + m_nGrid; // move to the next column
            nCurFrame++;
        }

        /*
		WORD nTotalFrames	= c3f.DirCount * c3f.FrmCount;
		WORD nCurFrame		= 0;
		WORD nCX			= m_nGrid;
		WORD nCY			= m_nGrid;
		
		
		// Move through frames
		for (WORD nCur = 0; nCur < nTotalFrames; nCur++)
		{
			if (nCurFrame == c3f.FrmCount)
			{
				nCurFrame = 0; // to start a new direction
				
				nCX  = m_nGrid; // start from the left end
				nCY += c3f.FrmHeight + m_nGrid; // move to the next row
			}
			
			GRAPHX::CopyBitmapRect(&hTempBmp, &m_hbmp, nCX, nCY, c3f.FrmWidth, c3f.FrmHeight);
			GRAPHX::CombineBitmap(&imgDest.m_hbmp, nCX, nCY, c3f.FrmWidth, c3f.FrmHeight, &hTempBmp, 0, 0, CB_OP_ADD | CB_DST_0);//CB_OP_ADD | CB_DST_0);
			
			nCX += c3f.FrmWidth + m_nGrid; // move to the next column
			nCurFrame++;
		}
*/
        // Delete temporary bitmap
        GRAPHX::FreeBitmap(&hTempBmp);
        /////////////////////////

        // Write palette file
        if (!ExportPaletteToFile(m_sOutputPath, palette, (EPaletteFileType)c3f.PalType)) {
            AfxMessageBox(IDS_ERR_WRITE_PAL);
            return FALSE;
        }

        //////////////////////////////////////////////////////////////////////////
        // Save bitmap as specified file type (FILE_BMP, FILE_PCX)

        // In case 8 bits per pixel
        if (m_nBitsPerPixel == BitsPerPixel_8) {
            //			GRAPHX::SaveBitmap(imgDest.m_sPath, &imgDest.m_hbmp, c3f.FileType, BITS_8);

            GRAPHX::FreeBitmap(&hTempBmp);
            GRAPHX::CreateBitmap(&hTempBmp, TYPE_CONV, c3f.Width, c3f.Height, BITS_8, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);

            GRAPHX::PutBitmapColors(&hTempBmp, 0, COLOR8, palette.pal);

            CDC* pDC = CDC::FromHandle(GRAPHX::CreateGraphXDC(&imgDest.m_hbmp));
            HDC hDC  = pDC->m_hDC;

            CDC* pTempDC = CDC::FromHandle(GRAPHX::CreateGraphXDC(&hTempBmp));
            HDC hTempDC  = pTempDC->m_hDC;

            // Copy whole picture to the temp HDC
            BitBlt(hTempDC, 0, 0, c3f.Width, c3f.Height, hDC, 0, 0, SRCCOPY);

            //			GRAPHX::CopyBitmapRect(&hTempBmp, &imgDest.m_hbmp, 0, 0, c3f.Width, c3f.Height);
            // Release DC
            GRAPHX::DeleteGraphXDC(*pTempDC);

            // Release DC
            GRAPHX::DeleteGraphXDC(*pDC);

            GRAPHX::SaveBitmap(imgDest.m_sPath, &hTempBmp, c3f.FileType, BITS_8);

            // Delete temporary bitmap
            GRAPHX::FreeBitmap(&hTempBmp);
        }
        // In case 24 bits per pixel
        else {
            GRAPHX::SaveBitmap(imgDest.m_sPath, &imgDest.m_hbmp, c3f.FileType, BITS_24);

            // Write 'Default' palette file
            if (!ExportPaletteToFile(m_sOutputPath, palette, (EPaletteFileType)c3f.PalType, uLoadString(IDS_DEF_PALETTE_SUFFIX))) {
                AfxMessageBox(IDS_ERR_WRITE_PAL);
                return FALSE;
            }
        }

        // Write C3F file
        if (!WriteC3F(m_sOutputPath, c3f)) {
            AfxMessageBox(IDS_ERR_WRITE_C3F, MB_ICONERROR);
            return FALSE;
        }

/////////////////////////
// clock - finish
#ifdef _DEBUG
        finish   = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        TRACE1("\nCImportToC3F::ImportToC3F: %2.3f seconds\n", duration);
#endif
        /////////////////////////

    } catch (CGraphXException* e) {
        AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (CFileException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (CMemoryException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CImportToC3F:ImportToC3F"), MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

void
CImportToC3F::OnSaveSettings()
{
    UpdateData();
}

void
CImportToC3F::OnBitsPerPixel()
{
    UpdateData();
}

void
CImportToC3F::OnGrid()
{
    UpdateData();
    m_nGrid = m_bGrid ? 1 : 0;
    UpdatePreview();
}
