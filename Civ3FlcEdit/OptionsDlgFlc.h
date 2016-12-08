#if !defined(OPTIONS_DLG_FLC_H__CIV3FLCEDIT__INCLUDED_)
#define OPTIONS_DLG_FLC_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlgFlc.h : header file
//

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgFlc dialog

class COptionsDlgFlc : public CDialogX
{
    // Construction
  public:
    void SetDefault();
    void Apply();
    COptionsDlgFlc(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(COptionsDlgFlc)
    enum
    {
        IDD = IDD_OPTIONSDLG_FLC
    };
    CButtonX m_btnDel;
    CButtonX m_btnBrowse;
    CEditX m_wndPath;
    int m_nPaletteType;
    CString m_sPath;
    int m_nAnimType;
    BOOL m_bExpOpen;
    BOOL m_bExpAsk;
    BOOL m_bIgnoreOffset;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COptionsDlgFlc)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    void OnChangeData();

    // Generated message map functions
    //{{AFX_MSG(COptionsDlgFlc)
    virtual BOOL OnInitDialog();
    afx_msg void OnChanged();
    afx_msg void OnBrowsePath();
    afx_msg void OnDelPath();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(OPTIONS_DLG_FLC_H__CIV3FLCEDIT__INCLUDED_)
