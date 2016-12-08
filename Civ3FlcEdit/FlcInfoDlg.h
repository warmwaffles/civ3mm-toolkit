#if !defined(FLC_INFO_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define FLC_INFO_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlcInfoDlg.h : header file
//

#include "FlcView.h"

#include "FlcInfoGen.h"
#include "FlcInfoExt.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoDlg dialog

class CFlcInfoDlg : public CDialogX, CFramePosition
{
    friend class FlcView;

    // Construction
  public:
    CFlcInfoDlg(CFlcView* pFlcView, CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CFlcInfoDlg)
    enum
    {
        IDD = IDD_DLG_FLC_INFO
    };
    CButtonX m_btnSave;
    CEditX m_wndTitle;
    CButtonX m_btnOK;
    CString m_sTitle;
    CTabCtrl m_wndTab;
    //}}AFX_DATA

    CFlcInfoGen m_tabG;
    CFlcInfoExt m_tabE;
    CFlcView* m_pFlc;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFlcInfoDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    void ActivateTab();
    void AdjustSheetSizes();
    CWnd* GetTab();
    virtual LPCTSTR RegKey()
    {
        return REG_SF_INFO_DLG;
    }

    // Generated message map functions
    //{{AFX_MSG(CFlcInfoDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSave();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FLC_INFO_DLG_H__CIV3FLCEDIT__INCLUDED_)
