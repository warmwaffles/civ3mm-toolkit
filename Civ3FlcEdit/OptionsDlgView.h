#if !defined(OPTIONS_DLG_VIEW_H__CIV3FLCEDIT__INCLUDED_)
#define OPTIONS_DLG_VIEW_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlgView.h : header file
//

#include "..\MFCaids\DialogX.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgView dialog

class COptionsDlgView : public CDialogX
{
    // Construction
  public:
    void SetDefault();
    void Apply();
    COptionsDlgView(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(COptionsDlgView)
    enum
    {
        IDD = IDD_OPTIONSDLG_VIEW
    };
    CButton m_wndShowIcon;
    CButton m_wndGloom;
    BOOL m_bStatusBar;
    BOOL m_bToolBar;
    BOOL m_bDynamicToolBar;
    BOOL m_bDialogBar;
    BOOL m_bBtnShowIcon;
    BOOL m_bBtnVisualEff;
    BOOL m_bMenuSameBorders;
    BOOL m_bMenuBorder;
    BOOL m_bMenuGloom;
    BOOL m_bMenuSelection;
    int m_nMenuStyle;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COptionsDlgView)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    void OnChangeData();

    // Generated message map functions
    //{{AFX_MSG(COptionsDlgView)
    virtual BOOL OnInitDialog();
    afx_msg void OnChanged();
    afx_msg void OnChangedStyle();
    afx_msg void OnChangedVisualEffects();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(OPTIONS_DLG_VIEW_H__CIV3FLCEDIT__INCLUDED_)
