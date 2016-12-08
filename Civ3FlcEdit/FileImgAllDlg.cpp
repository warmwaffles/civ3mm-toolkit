/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FileImgAllDlg.cpp
	CREATED:	17.12.2004
	AUTHOR:		Dreyk
	VERSION:	2.0

	MODIFIED:	

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FileImgAllDlg.h"

#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_IMG_DLG "Dlg ImgPreview"

/////////////////////////////////////////////////////////////////////////////
// CFileImgAllDlg

IMPLEMENT_DYNAMIC(CFileImgAllDlg, CFileDialogX)

CFileImgAllDlg::CFileImgAllDlg(BOOL bOpenFileDialog,
                               LPCTSTR lpszDefExt,
                               LPCTSTR lpszFileName,
                               DWORD dwFlags,
                               LPCTSTR lpszFilter,
                               CWnd* pParentWnd)
  : CFileDialogX(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
    // show preview
    m_bPreview = AfxGetApp()->GetProfileInt(REG_IMG_DLG, REG_PREVIEW, 1);
}

BEGIN_MESSAGE_MAP(CFileImgAllDlg, CFileDialogX)
//{{AFX_MSG_MAP(CFileImgAllDlg)
ON_BN_CLICKED(IDC_PREVIEW_CHECK, OnPreview)
ON_WM_SIZE()
ON_WM_PAINT()
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL
CFileImgAllDlg::OnInitDialog()
{
    GetDlgItem(IDC_PREVIEW_CHECK)->SendMessage(BM_SETCHECK, (m_bPreview) ? 1 : 0);

    return CFileDialogX::OnInitDialog();
}

void
CFileImgAllDlg::OnFileNameChange()
{
    CFileDialogX::OnFileNameChange();

    GetDlgItem(IDC_IMG_INFO)->SetWindowText("");

    try {
        if (!GetPathName().IsEmpty()) {
            CWaitCursor wait;
            m_img.LoadFile(GetPathName());

            FILEINFO* pFi = m_img.GetImgFileInfo();

            GetDlgItem(IDC_IMG_INFO)->SetWindowText(uFormatString(IDS_IMG_ALL_INFO, pFi->Width, pFi->Height, pFi->BitsPerPixel));
        }
    } catch (CGraphXException* e) {
        //AfxMessageBox(e->GetErrorMessage(e->GetLastError()));
        GetDlgItem(IDC_PREVIEW)->SetWindowText(e->GetErrorMessage(e->GetLastError()));
        e->Delete();
    } catch (...) {
#ifdef _DEBUG
        AfxMessageBox("Unknown exception in the CFileImgAllDlg::OnFileNameChange()");
#endif
    }

    Invalidate();
}

void
CFileImgAllDlg::OnFolderChange()
{
    CFileDialogX::OnFolderChange();

    m_img.CloseFile();
    Invalidate();
}

void
CFileImgAllDlg::OnPreview()
{
    m_bPreview = !m_bPreview;

    Invalidate();
}

void
CFileImgAllDlg::OnSize(UINT nType, int cx, int cy)
{
    CFileDialogX::OnSize(nType, cx, cy);

    Invalidate();
}

void
CFileImgAllDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CWnd* pWnd = GetDlgItem(IDC_PREVIEW);

    if (pWnd->GetSafeHwnd()) {
        CRect rc;
        pWnd->GetWindowRect(rc);
        ScreenToClient(rc);
        dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));

        if (m_bPreview && m_img.IsLoaded()) {
            rc.DeflateRect(3, 3);
            m_img.Render(&dc, rc, CGraphXImage::PaintProportion2);
        }
    }
}

BOOL
CFileImgAllDlg::OnFileNameOK()
{
    if (!m_img.IsLoaded()) {
        AfxMessageBox(uFormatString(IDS_IMG_UNKNOWN_FORMAT, GetPathName()));
        return TRUE;
    }

    m_sFilePath       = m_img.GetFileName();
    m_ofnEx.lpstrFile = (LPSTR)(LPCTSTR)m_sFilePath;

    return CFileDialogX::OnFileNameOK();
}

void
CFileImgAllDlg::OnDestroy()
{
    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(REG_IMG_DLG, REG_PREVIEW, m_bPreview);

    m_img.CloseFile();

    CFileDialogX::OnDestroy();
}
