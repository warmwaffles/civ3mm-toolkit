#if !defined(PANELDIRETCION_H__CIV3FLCEDIT__INCLUDED_)
#define PANELDIRETCION_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelDirection.h : header file
//

#include "..\MFCaids\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelDirection dialog

class CAnimationView;
class CDlgBar;

class CPanelDirection : public CDialog
{
    friend class CAnimationView;

    // Construction
  public:
    CPanelDirection(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPanelDirection)
    enum
    {
        IDD = IDD_PANEL_DIRECTION
    };
    CButtonST m_btnW;
    CButtonST m_btnSW;
    CButtonST m_btnSE;
    CButtonST m_btnS;
    CButtonST m_btnNW;
    CButtonST m_btnNE;
    CButtonST m_btnN;
    CButtonST m_btnE;
    CButtonST m_btnAll;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPanelDirection)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

  public:
    BOOL IsAttached();
    void SetView(CAnimationView* pView);
    void UpdateControlsState();

    CAnimationView* m_pView;
    CDlgBar* m_pParent;

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CPanelDirection)
    virtual BOOL OnInitDialog();
    afx_msg void OnDirectionSW();
    afx_msg void OnDirectionW();
    afx_msg void OnDirectionNW();
    afx_msg void OnDirectionN();
    afx_msg void OnDirectionNE();
    afx_msg void OnDirectionE();
    afx_msg void OnDirectionSE();
    afx_msg void OnDirectionS();
    afx_msg void OnDirectionAll();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(PANELDIRETCION_H__CIV3FLCEDIT__INCLUDED_)
