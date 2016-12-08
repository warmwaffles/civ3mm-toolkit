#if !defined(EDIT_X_INCLUDED)
#define EDIT_X_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

class MFCAIDS_API CEditX : public CEdit
{
    // Construction
  public:
    CEditX();

    // Implementation
  public:
    virtual ~CEditX();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSkmContAttView)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

    // Generated message map functions
  protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //{{AFX_MSG(CEditX)
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(EDIT_X_INCLUDED)
