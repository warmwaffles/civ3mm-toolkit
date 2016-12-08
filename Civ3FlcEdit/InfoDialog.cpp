/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 InfoDialog.cpp
	CREATED:	 14.3.2003

	AUTHOR:		 Dreyk
	MODIFIED:	

	DESCRIPTION: This dialog box destined for showing any information
                for uer (like Message box).
				You can set or omit any parameters for that dialog.
				CString m_sReport  - message
				UINT    m_nIconId    - type of the standard Windows icon: 0-IDI_ASTERISK, 1-IDI_EXCLAMATION, 2-IDI_HAND
				CString m_sHeader  - message header
				CString m_sCaption - dialog caption
				HICON   m_hIcon      - icon for the dialog caption
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "InfoDialog.h"

#include "..\MFCaids\FileDialogX.h"

#include <shlobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CString INFO_REPORT_MSG;
static UINT INFO_REPORT_NUM;

////////////////////////////////////////////////////////////////////////////////////////////////
// Show information dialog box.
// You can set or omit any parameters for that dialog.
// lpReport  - message
// nIconId   - type of the standard Windows icon: 0-IDI_ASTERISK, 1-IDI_EXCLAMATION, 2-IDI_HAND
// lpHeader  - message header
// lpCaption - dialog caption
// hIcon     - icon for the dialog caption
//
void
InfoShowDialog(LPCTSTR lpReport, LPCTSTR lpHeader, UINT nIconId, LPCTSTR lpCaption, HICON hIcon)
{
    CInfoDialog dlg(lpReport, lpHeader, nIconId, lpCaption, hIcon);

    dlg.DoModal();

    // Zeroing the report string and report counter
    InfoResetReports();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Show information dialog box.
// You can set or omit any parameters for that dialog.
// nIconId   - type of the standard Windows icon: 0-IDI_ASTERISK, 1-IDI_EXCLAMATION, 2-IDI_HAND
// lpHeader  - message header
// lpCaption - dialog caption
// hIcon     - icon for the dialog caption
//
void
InfoShowReport(LPCTSTR lpHeader, UINT nIconId, LPCTSTR lpCaption, HICON hIcon)
{
    if (!InfoIsReportPresent())
        return;

    CInfoDialog dlg(INFO_REPORT_MSG, lpHeader, nIconId, lpCaption, hIcon);

    dlg.DoModal();

    // Zeroing the report string and report counter
    InfoResetReports();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Add numerated message to the report.
//
void
InfoAddMsg(LPCTSTR lpMsg)
{
    INFO_REPORT_NUM++;

    CString strNum;
    strNum.Format(INFO_REPORT_NUM < 10 ? " %d" : "%d", INFO_REPORT_NUM);

    INFO_REPORT_MSG += strNum + ". ";
    INFO_REPORT_MSG += lpMsg;
    INFO_REPORT_MSG += "\r\n\r\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Add new message to the report.
//
void
InfoAddNewMsg(LPCTSTR lpMsg)
{
    INFO_REPORT_NUM = 0;
    INFO_REPORT_MSG += lpMsg;
    INFO_REPORT_MSG += "\r\n\r\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Add message to the report.
//
void
InfoAddText(LPCTSTR lpMsg)
{
    INFO_REPORT_MSG += lpMsg;
    INFO_REPORT_MSG += "\r\n\r\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Add message to start of the report.
//
void
InfoAddCaption(LPCTSTR lpMsg)
{
    CString sMsg = lpMsg;
    sMsg += "\r\n\r\n";
    INFO_REPORT_MSG.Insert(0, sMsg);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Clear all report's messages.
//
void
InfoResetReports()
{
    INFO_REPORT_MSG.Empty();
    INFO_REPORT_NUM = 0;
}

BOOL
InfoIsReportPresent()
{
    return !INFO_REPORT_MSG.IsEmpty();
}

BOOL
IsReportNum()
{
    return INFO_REPORT_NUM > 0 ? TRUE : FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CInfoDialog dialog

CInfoDialog::CInfoDialog(LPCTSTR lpReport, LPCTSTR lpHeader, UINT nIconId, LPCTSTR lpCaption, HICON hIcon, CWnd* pParent /*=NULL*/)
  : CDialogX(IDD_DLG_INFO, pParent)
{
    //{{AFX_DATA_INIT(CInfoDialog)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_sCaption = lpCaption; // dialog caption
    m_sHeader  = lpHeader;  // message header
    m_sReport  = lpReport;  // message
    m_nIconId  = nIconId;   // type of the standard Windows icon: 0-IDI_ASTERISK, 1-IDI_EXCLAMATION, 2-IDI_HAND
    m_hIcon    = hIcon;     // icon for the dialog caption

    SetControlInfo(IDC_STATIC_HEADER, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_FRAME, CF_RESIZE_BOTH);
    SetControlInfo(IDC_EDIT_REPORT, CF_RESIZE_BOTH);
    SetControlInfo(IDC_SAVE, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT);
    SetControlInfo(IDOK, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT);
}

void
CInfoDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CInfoDialog)
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDC_SAVE, m_btnSave);
    DDX_Control(pDX, IDC_STATIC_HEADER, m_wndHeader);
    DDX_Control(pDX, IDC_STATIC_ICO, m_wndIco);
    DDX_Control(pDX, IDC_EDIT_REPORT, m_wndReport);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInfoDialog, CDialogX)
//{{AFX_MSG_MAP(CInfoDialog)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_SAVE, OnSave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDialog message handlers

BOOL
CInfoDialog::OnInitDialog()
{
    CDialogX::OnInitDialog();

    LoadPosition(this); // load the dialog window position

    // Setup button
    m_btnOK.SetIcon(IDI_OK);
    m_btnSave.SetIcon(IDI_SAVE);

    if (!m_sCaption.IsEmpty())
        SetWindowText(m_sCaption);

    // Set icon to the dialog caption
    SetIcon(AfxGetApp()->LoadIcon(IDI_INFO), FALSE);

    // Set standard icon
    LPCTSTR lpIcon = IDI_ASTERISK; // info

    if (m_nIconId == 1)
        lpIcon = IDI_EXCLAMATION; // warning
    else if (m_nIconId == 2)
        lpIcon = IDI_HAND; // error

    m_wndIco.SetIcon(AfxGetApp()->LoadStandardIcon(lpIcon));

    // Set font and size for message header
    m_wndHeader.SetFontBold().SetFontSize(14).SetTextColor(0x800000);
    m_wndHeader.SetWindowText(m_sHeader);

    m_wndReport.SetWindowText(m_sReport);

    SetTimer(345, 50, 0);
    return TRUE;
}

void
CInfoDialog::OnDestroy()
{
    CDialogX::OnDestroy();
    SavePosition(this); // save the dialog window position
}

LPCTSTR
CInfoDialog::RegKey()
{
    return "Dlg Info";
}

void
CInfoDialog::OnSave()
{
    CFileDialogX dlg(FALSE, "txt", "Info.txt", OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, _T(uLoadString(IDS_FILTER_TXT)));

    CString sTitle(uLoadString(IDS_TITLE_TXT));
    dlg.m_ofn.lpstrTitle = sTitle;

    CWinApp* pApp = AfxGetApp();
    CString sPath = pApp->GetProfileString(RegKey(), REG_LAST_OPENED, "");

    if (!uIsFileExist(sPath))
        sPath = uSHGetFolderPath(CSIDL_PERSONAL); // try to get "My Documents" directory path

    dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    sPath = dlg.GetPathName();

    pApp->WriteProfileString(RegKey(), REG_LAST_OPENED, sPath);

    CString sMsg = m_sHeader + "\r\n\r\n" + m_sReport;
    //	CString sMsg = uFormatString(IDS_INFO_HEADER, m_sHeader) + m_sReport;

    try {
        CFile fl(sPath, CFile::modeCreate | CFile::modeReadWrite); // open file

        fl.SetLength(0);
        fl.Write(sMsg, sMsg.GetLength());
        fl.Close();
    } catch (CFileException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
    }
}
