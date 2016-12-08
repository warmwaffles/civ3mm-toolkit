#if !defined(PANELSOUND_H__CIV3FLCEDIT__INCLUDED_)
#define PANELSOUND_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelSound.h : header file
//

#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelSound dialog

class CAnimationView;
class CDlgBar;

class CPanelSound : public CDialog
{
    friend class CAnimationView;

    // Construction
  public:
    CPanelSound(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPanelSound)
    enum
    {
        IDD = IDD_PANEL_SOUND
    };
    CButton m_btnPreviewWAV;
    CButton m_btnNonStopWAV;
    CButtonX m_btnStopWAV;
    CButtonX m_btnDelPath;
    CButtonX m_btnBrowsePath;
    CButtonX m_btnPlayWAV;
    CEditX m_wndEditPath;
    BOOL m_bNonStopWAV;
    BOOL m_bPreviewWAV;
    CString m_sPathWAV;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPanelSound)
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
    //{{AFX_MSG(CPanelSound)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnBrowsePath();
    afx_msg void OnDelPath();
    afx_msg void OnNonStop();
    afx_msg void OnPlayWav();
    afx_msg void OnPreviewWav();
    afx_msg void OnStopWav();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(PANELSOUND_H__CIV3FLCEDIT__INCLUDED_)
