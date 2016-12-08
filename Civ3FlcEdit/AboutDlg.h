#if !defined(ABOUT_DLG_UNIT_VIEWER_H__CIV3FLCEDIT__INCLUDED_)
#define ABOUT_DLG_UNIT_VIEWER_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//
#include "..\MFCaids\HyperLink.h"
#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\RichEditCtrlX.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogX, CFramePosition
{
  public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum
    {
        IDD = IDD_ABOUTBOX
    };
    CAnimateCtrl m_wndAnimate;
    CRichEditCtrlX m_wndNote;
    CStatic m_wndBlink;
    CButtonX m_btnOK;
    CString m_sFileVersion;
    CString m_sCopyright;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    void ShowTextResource(LPCTSTR lpName);
    virtual LPCTSTR RegKey()
    {
        return "Dlg About";
    }

    CHyperLink m_HyperLink;
    CHyperLink m_MailTo;
    CString m_sThanks;
    BOOL m_bThanksShowed;

    //{{AFX_MSG(CAboutDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ABOUT_DLG_UNIT_VIEWER_H__CIV3FLCEDIT__INCLUDED_)
