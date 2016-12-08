#if !defined(EXPORT_FLC_OUT_H__DPDocPageINCLUDED_)
#define EXPORT_FLC_OUT_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportFlcOut.h : header file
//

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CExportFlcOut dialog
class CExportFlcDlg;
class CExportFlcOut : public CDialogX
{
    // Construction
  public:
    void ApplyChanges();
    CExportFlcOut(CWnd* pParent = NULL); // standard constructor
    void UpdateControlsState();

    // Dialog Data
    //{{AFX_DATA(CExportFlcOut)
    enum
    {
        IDD = IDD_TAB_EXP_OUT
    };
    CEditX m_wndDir;
    CComboBox m_wndFileType;
    CEditX m_wndFileName;
    CButtonX m_btnBrowse;
    CString m_sFileName;
    int m_nNameType;
    CString m_sDir;
    CString m_sCurName;
    //}}AFX_DATA

    CExportFlcDlg* m_pParent;
    BOOL m_bChanged;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExportFlcOut)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CExportFlcOut)
    virtual BOOL OnInitDialog();
    afx_msg void OnName();
    afx_msg void OnBrowsePath();
    afx_msg void OnSelendokFileType();
    afx_msg void OnChangeFileName();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(EXPORT_FLC_OUT_H__DPDocPageINCLUDED_)
