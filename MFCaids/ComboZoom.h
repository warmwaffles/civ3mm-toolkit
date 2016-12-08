#if !defined(COMBOZOOM_H__MFCAIDS__INCLUDED_)
#define COMBOZOOM_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboZoom.h : header file
//

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboZoom window

class MFCAIDS_API CComboZoom : public CComboBox
{
    // Construction
  public:
    CComboZoom();

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CComboZoom)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    void InitComboZoom(int nZoomFactor = 0);
    void SetParentWnd(CWnd* pWnd);
    void ZoomOut();
    void ZoomIn();
    virtual ~CComboZoom();

    // Generated message map functions
  protected:
    CFont m_font;
    CWnd* m_pWnd;

    //{{AFX_MSG(CComboZoom)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(COMBOZOOM_H__MFCAIDS__INCLUDED_)
