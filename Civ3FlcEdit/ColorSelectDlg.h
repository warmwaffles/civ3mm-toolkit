#if !defined(COLOR_SELECT_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define COLOR_SELECT_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorSelectDlg.h : header file
//

#include "ColorReplaceDlg.h"
#include "ColorTableSrc.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CColorSelectDlg dialog

class CColorSelectDlg : public CDialogX, CFramePosition, CFlcImage
{
    friend class CColorReplaceDlg;
    friend class CColorTableSrc;

    // Construction
  public:
    void DrawZoomedPreview();
    void ChangeVariables();
    CColorSelectDlg(RGBQUADPAL* pPal, CWnd* pParent = NULL); // standard constructor
    void ApplyColorChanges();

    // Dialog Data
    //{{AFX_DATA(CColorSelectDlg)
    enum
    {
        IDD = IDD_COLOR_SELECT_DLG
    };
    CStatic m_wndPreview;
    CStatic m_wndZoom;
    CScrollBar m_wndScrollVer;
    CScrollBar m_wndScrollHor;
    CEditX m_wndPath;
    CButtonX m_btnBrowse;
    CButtonX m_btnRefresh;
    CButtonX m_btnApply;
    CButtonX m_btnOK;
    CButtonX m_btnCancel;
    CString m_sPath;
    CStatic m_wndCurColorSrc;
    CStatic m_wndSelColor;
    CStatic m_wndCurColor;
    CString m_sRed;
    CString m_sIndex;
    CString m_sGreen;
    CString m_sBlue;
    CString m_sBlueS;
    CString m_sGreenS;
    CString m_sRedS;
    CString m_sIndexS;
    CString m_sBlueSrc;
    CString m_sGreenSrc;
    CString m_sRedSrc;
    //}}AFX_DATA

    CColorTableSrc m_wndColorTableD;
    RGBQUADPAL* m_pPalExtD;
    RGBQUADPAL m_rgbPalD;
    //	RGBQUADPAL			m_rgbPalTemp;

    COLORREF m_crDest;    // destination palette's current color
    COLORREF m_crSrc;     // source palette's current color
    HBRUSH m_brDest;      // destination palette's current color's bruch
    HBRUSH m_brSrc;       // source palette's current color's bruch
    COLORREF m_crCurDest; // destination palette's current color
    HBRUSH m_brCurDest;   // destination palette's current color's bruch

    int m_nScrollHor;
    int m_nScrollVer;
    CRect m_rcCur;
    CRect m_rcPreview;
    CPoint m_ptPreview;
    HCURSOR m_hCursor;
    BOOL m_bInsideRect;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorSelectDlg)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    virtual LPCTSTR RegKey()
    {
        return "Dlg SelectColor";
    }
    void UpdateControlsState();
    void UpdateColorInfo();
    void UpdatePreview();

    // Generated message map functions
    //{{AFX_MSG(CColorSelectDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnPaint();
    afx_msg void OnApply();
    afx_msg void OnRefresh();
    afx_msg void OnBrowsePath();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(COLOR_SELECT_DLG_H__CIV3FLCEDIT__INCLUDED_)
