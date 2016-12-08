/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	SelectBgdImgDlg.cpp
	CREATED:	18.01.2005
	VERSION:	1.0

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CSelectBgdImgDlg class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "SelectBgdImgDlg.h"

#include "FileImgAllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectBgdImgDlg dialog

CSelectBgdImgDlg::CSelectBgdImgDlg(CWnd* pParent /*=NULL*/)
  : CDialogX(CSelectBgdImgDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSelectBgdImgDlg)
    m_sTargetPath = _T("");
    m_sInfo       = _T("");
    //}}AFX_DATA_INIT

    SetControlInfo(IDC_PREVIEW, CF_RESIZE_BOTH);

    SetControlInfo(IDOK, CF_ANCHOR_BOTTOM);
    SetControlInfo(IDCANCEL, CF_ANCHOR_BOTTOM);

    //	m_bDrawGripper	= FALSE;
}

void
CSelectBgdImgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelectBgdImgDlg)
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_PREVIEW, m_wndPreview);
    DDX_Control(pDX, IDC_EDIT_PATH, m_wndPath);
    DDX_Control(pDX, IDC_DELETE, m_btnDelete);
    DDX_Control(pDX, IDC_BROWSE, m_btnBrowse);
    DDX_Text(pDX, IDC_EDIT_PATH, m_sTargetPath);
    DDX_Text(pDX, IDC_STATIC_INFO, m_sInfo);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSelectBgdImgDlg, CDialogX)
//{{AFX_MSG_MAP(CSelectBgdImgDlg)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
ON_BN_CLICKED(IDC_DELETE, OnDelete)
ON_WM_PAINT()
ON_WM_SIZE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectBgdImgDlg message handlers

BOOL
CSelectBgdImgDlg::OnInitDialog()
{
    CDialogX::OnInitDialog();

    LoadPosition(this);

    // Setup buttons
    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);
    m_btnBrowse.SetIcon(IDI_BROWSE);
    m_btnDelete.SetIcon(IDI_DELETE);

    CString sTargetPath = AfxGetApp()->GetProfileString(REG_OPTIONS, REG_BGD_IMG, "");

    if (uIsFileExist(sTargetPath))
        LoadBgdImgFile(sTargetPath);

    return TRUE;
}

void
CSelectBgdImgDlg::OnDestroy()
{
    SavePosition(this);
    CDialogX::OnDestroy();
}

BOOL
CSelectBgdImgDlg::PreTranslateMessage(MSG* pMsg)
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
CSelectBgdImgDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogX::OnSize(nType, cx, cy);

    if (m_wndPreview.GetSafeHwnd())
        UpdatePreview();
}

void
CSelectBgdImgDlg::OnOK()
{
    if (m_fi.Format != FILE_GIF && (m_fi.Width != WND_W || m_fi.Height != WND_H)) {
        int nRes = AfxMessageBox(uFormatString(IDS_ERR_IMG_SIZE, m_fi.Width, m_fi.Height), MB_YESNOCANCEL);

        if (nRes == IDYES) {
            try {
                GRAPHX::SaveBitmap(m_sTargetPath, &m_hbmp, m_fi.Format, m_hbmp.BitsPerPixel);
            } catch (CGraphXException* e) {
                AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
                e->Delete();
                return;
            } catch (...) {
                ASSERT(FALSE);
                AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CSelectBgdImgDlg::OnOK"), MB_ICONERROR);
                return;
            }
        } else if (nRes == IDCANCEL)
            return;
    }

    AfxGetApp()->WriteProfileString(REG_OPTIONS, REG_BGD_IMG, m_sTargetPath);

    CDialogX::OnOK();
}

void
CSelectBgdImgDlg::OnBrowse()
{
    try {
        CFileImgAllDlg dlg(TRUE,
                           NULL,
                           NULL,
                           OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_LONGNAMES |
                             OFN_PATHMUSTEXIST,
                           _T(uLoadString(IDS_FILTER_BGD)),
                           this);

        dlg.m_ofn.hInstance      = AfxGetInstanceHandle();
        dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_IMG_ALL);

        CString sTitle(uLoadString(IDS_TITLE_IMAGE));
        dlg.m_ofn.lpstrTitle = sTitle;

        // last used filter
        //		dlg.m_ofn.nFilterIndex = AfxGetApp()->GetProfileInt(REG_IMG_DLG, REG_FILTER, 1);

        if (uIsFileExist(m_sPath))
            dlg.m_ofn.lpstrInitialDir = m_sPath;

        dlg.m_ofn.hInstance = AfxGetInstanceHandle();

        if (dlg.DoModal() != IDOK)
            return;

        LoadBgdImgFile(dlg.GetPathName());
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
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CSelectBgdImgDlg::OnBrowse"), MB_ICONERROR);
        return;
    }

    UpdateData(FALSE);
    UpdateControlsState();
}

void
CSelectBgdImgDlg::OnDelete()
{
    CloseFile();
    m_sTargetPath.Empty();
    UpdateData(FALSE);

    UpdateControlsState();
}

void
CSelectBgdImgDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (m_wndPreview.GetSafeHwnd()) {
        CRect rc;
        m_wndPreview.GetWindowRect(rc);
        ScreenToClient(rc);
        dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));

        if (IsLoaded()) {
            rc.DeflateRect(3, 3);
            Render(&dc, rc, CGraphXImage::PaintProportion2);
        }
    }
}

void
CSelectBgdImgDlg::UpdateControlsState()
{
    UpdateData();
    UpdatePreview();
}

void
CSelectBgdImgDlg::UpdatePreview()
{
    CRect rc;

    if (m_wndPreview.GetSafeHwnd()) {
        m_wndPreview.GetWindowRect(rc);
        ScreenToClient(rc);
        InvalidateRect(rc);
    }
}

BOOL
CSelectBgdImgDlg::LoadBgdImgFile(LPCTSTR sPath)
{
    try {
        LoadFile(sPath);

        if (!IsLoaded()) {
            CloseFile();
            m_sInfo = uLoadString(IDS_ERR_LOAD_IMG);
            UpdateData(FALSE);
            UpdateControlsState();
            return FALSE;
        }

        if (m_hbmp.Width != WND_W || m_hbmp.Height != WND_H)
            GRAPHX::SizeBitmap(&m_hbmp, WND_W, WND_H);
    } catch (CGraphXException* e) {
        AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CSelectBgdImgDlg::LoadBgdImgFile"), MB_ICONERROR);
        return FALSE;
    }

    m_sTargetPath = sPath;
    m_sInfo       = uFormatString(IDS_IMG_INFO, m_fi.Width, m_fi.Height, m_hbmp.BitsPerPixel);

    UpdateData(FALSE);
    UpdateControlsState();

    return TRUE;
}