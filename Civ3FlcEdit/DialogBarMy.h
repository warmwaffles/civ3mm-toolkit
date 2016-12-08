#if !defined(DIALOG_BAR_MY_H__CIV3FLCEDIT__INCLUDED_)
#define DIALOG_BAR_MY_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// DialogBarMy.h : header file
///////////////////////////////////////////////////////////////////////////////
// CDialogBarMy window

class CDialogBarMy : public CControlBar
{
    const COLORREF m_clrBtnHilight;
    const COLORREF m_clrBtnShadow;

    // Construction / destruction
  public:
    CDialogBarMy();

    // Attributes
  public:
    BOOL IsHorz() const;

    // Operations
  public:
    // Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

    // Overrides
  public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDialogBarMy)
  public:
    virtual BOOL Create(CWnd* pParentWnd, CDialog* pDialog, CString& pTitle, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_LEFT);
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual LPCTSTR RegKey();
    virtual void EnableDocking(DWORD dwStyle);
    void ToggleDocking();
    virtual void OnCloseBar();
    virtual ~CDialogBarMy();

    // implementation helpers
    CPoint& ClientToWnd(CPoint& point);

  protected:
    void DrawGripper(CDC* pDC);

    BOOL m_bPushedClose;
    BOOL m_bPushedMax;
    BOOL m_bPressedClose;
    BOOL m_bPressedMax;

    CSize m_sizeMin;
    CSize m_sizeHorz;
    CSize m_sizeVert;
    CSize m_sizeFloat;
    UINT m_nDockBarID;
    CPoint m_ptOld;
    CRect m_rectUndock;
    CRect m_rectClose;
    CRect m_rectGripper;
    CDialog* m_pDialog;
    CBrush m_brushBkgd;

    // Rob Wolpov 10/15/98 Added support for diagonal resizing
    int m_cMinWidth;
    int m_cMinHeight;
    int m_cCaptionSize;

    // Generated message map functions
  protected:
    //{{AFX_MSG(CDialogBarMy)
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnNcPaint();
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    afx_msg UINT OnNcHitTest(CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(DIALOG_BAR_MY_H__CIV3FLCEDIT__INCLUDED_)
