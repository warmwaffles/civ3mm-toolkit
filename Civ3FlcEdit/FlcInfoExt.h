#if !defined(FLC_INFO_EXT_H__DPDocPageINCLUDED_)
#define FLC_INFO_EXT_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlcInfoExt.h : header file
//

#include "..\MFCaids\ListCtrlX.h"
#include "..\MFCaids\DialogX.h"

/////////////////////////////////////////////////////////////////////////////
// CListCtrlSfEdit list ctrl
class CListFlcInfoExt : public CListCtrlX
{
  public:
    CListFlcInfoExt(){};
    virtual ~CListFlcInfoExt(){};

  protected:
    virtual LPCTSTR RegKey()
    {
        return REG_SF_INFO_DLG;
    }
    virtual BOOL IsEditable(int nRow, int nCol)
    {
        return FALSE;
    }
};

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoExt dialog
class CFlcInfoDlg;
class CFlcInfoExt : public CDialogX
{
    // Construction
  public:
    CFlcInfoExt(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CFlcInfoExt)
    enum
    {
        IDD = IDD_TAB_INFO_EXT
    };
    CListFlcInfoExt m_Table;
    //}}AFX_DATA

    CFlcInfoDlg* m_pParent;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFlcInfoExt)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    CImageList m_imgState;

    // Generated message map functions
    //{{AFX_MSG(CFlcInfoExt)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FLC_INFO_EXT_H__DPDocPageINCLUDED_)
