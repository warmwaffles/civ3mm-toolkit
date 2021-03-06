#if !defined(NEW_FLC_GEN_H__DPDocPageINCLUDED_)
#define NEW_FLC_GEN_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFlcGen.h : header file
//

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CNewFlcGen dialog
class CNewFlcDlg;
class CNewFlcGen : public CDialogX
{
    // Construction
  public:
    CNewFlcGen(CWnd* pParent = NULL); // standard constructor
    void ApplyChanges();

    // Dialog Data
    //{{AFX_DATA(CNewFlcGen)
    enum
    {
        IDD = IDD_TAB_NEW_GEN
    };
    CEditX m_wndOutPath;
    CString m_sFrmCount;
    CString m_sDelay;
    CString m_sOutDir;
    CString m_sFrmHeight;
    CString m_sOutName;
    CString m_sPalette;
    CString m_sOutType;
    CString m_sFrmWidth;
    CString m_sOutPath;
    CString m_sDirCount;
    CString m_sPalType;
    CString m_sAnimType;
    CString m_sXOffset;
    CString m_sYOffset;
    CString m_sLastColors;
    CString m_sColDepth;
    //}}AFX_DATA

    CNewFlcDlg* m_pParent;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewFlcGen)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CNewFlcGen)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(NEW_FLC_GEN_H__DPDocPageINCLUDED_)
