#if !defined(INFO_DIALOG_H__CIV3FLCEDIT__INCLUDED_)
#define INFO_DIALOG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoDialog.h : header file
//

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\Label.h"
#include "..\MFCaids\EditX.h"
#include "..\MFCaids\ButtonX.h"

class CInfoDialog;
void InfoShowDialog(LPCTSTR lpReport = NULL, LPCTSTR lpHeader = NULL, UINT nIconId = 0, LPCTSTR lpCaption = NULL, HICON hIcon = NULL);
void InfoShowReport(LPCTSTR lpHeader = NULL, UINT nIconId = 0, LPCTSTR lpCaption = NULL, HICON hIcon = NULL);
void InfoAddMsg(LPCTSTR lpMsg);     // Add numerated message to the report.
void InfoAddNewMsg(LPCTSTR lpMsg);  // Add new message to the report.
void InfoAddText(LPCTSTR lpMsg);    // Add message to the report.
void InfoAddCaption(LPCTSTR lpMsg); // Add message to start of the report.
void InfoResetReports();            // Clear all report's messages.
BOOL InfoIsReportPresent();
BOOL IsReportNum();

/////////////////////////////////////////////////////////////////////////////
// CInfoDialog dialog

class CInfoDialog : public CDialogX, CFramePosition
{
    // Construction
  public:
    CInfoDialog(LPCTSTR lpReport  = NULL,
                LPCTSTR lpHeader  = NULL,
                UINT nIconId      = 0,
                LPCTSTR lpCaption = NULL,
                HICON hIcon       = NULL,
                CWnd* pParent     = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CInfoDialog)
    CButtonX m_btnOK;
    CButtonX m_btnSave;
    CLabel m_wndHeader;
    CStatic m_wndIco;
    CEditX m_wndReport;
    //}}AFX_DATA

    CString m_sCaption; // dialog caption
    CString m_sHeader;  // message header
    CString m_sReport;  // message
    UINT m_nIconId;     // type of the standard Windows icon: 0-IDI_ASTERISK, 1-IDI_EXCLAMATION, 2-IDI_HAND
    HICON m_hIcon;      // icon for the dialog caption

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CInfoDialog)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    virtual LPCTSTR RegKey();
    // Generated message map functions
    //{{AFX_MSG(CInfoDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnSave();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(INFO_DIALOG_H__CIV3FLCEDIT__INCLUDED_)
