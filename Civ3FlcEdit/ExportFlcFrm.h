#if !defined(EXPORT_FLC_FRM_H__CIV3FLCEDIT__INCLUDED_)
#define EXPORT_FLC_FRM_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportFlcFrm.h : header file
//

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\EditX.h"
#include "..\MFCaids\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CExportFlcFrm dialog

class CExportFlcDlg;
class CExportFlcFrm : public CDialogX
{
    // Construction
  public:
    void UpdatePreview();
    void ApplyChanges();
    CExportFlcFrm(CWnd* pParent = NULL); // standard constructor
    void UpdateControlsState();

    // Dialog Data
    //{{AFX_DATA(CExportFlcFrm)
    enum
    {
        IDD = IDD_TAB_EXP_FRM
    };
    CButton m_wndResizeEnable;
    CSpinButtonCtrl m_wndResizeBottomSpin;
    CEditX m_wndResizeBottom;
    CSpinButtonCtrl m_wndResizeTopSpin;
    CEditX m_wndResizeTop;
    CSpinButtonCtrl m_wndResizeRightSpin;
    CEditX m_wndResizeRight;
    CSpinButtonCtrl m_wndResizeLeftSpin;
    CEditX m_wndResizeLeft;
    CSpinButtonCtrl m_wndDelaySpin;
    CEditX m_wndDelay;
    CSpinButtonCtrl m_wndYOffsetSpin;
    CEditX m_wndYOffset;
    CEditX m_wndXOffset;
    CSpinButtonCtrl m_wndXOffsetSpin;
    CButtonST m_btnLock;
    CButton m_wndFrmResize;
    CStatic m_wndCurColor;
    CSpinButtonCtrl m_wndFrmCountSpin;
    CSpinButtonCtrl m_wndFrmWidthSpin;
    CEditX m_wndFrmWidth;
    CSpinButtonCtrl m_wndFrmHeightSpin;
    CEditX m_wndFrmHeight;
    CEditX m_wndFrmCount;
    UINT m_nFrmCount;
    UINT m_nFrmHeight;
    UINT m_nFrmWidth;
    int m_nCountType;
    int m_nSizeType;
    int m_nDelayType;
    CString m_sDelayCur;
    CString m_sFrmCountCur;
    CString m_sFrmSizeCur;
    BOOL m_bFrmResize;
    BOOL m_bLock;
    UINT m_nXOffset;
    int m_nOffsetType;
    UINT m_nYOffset;
    CString m_sFrmOffsetCur;
    CString m_sDelay;
    BOOL m_bResizeEnable;
    UINT m_nResizeLeft;
    UINT m_nResizeRight;
    UINT m_nResizeTop;
    UINT m_nResizeBottom;
    //}}AFX_DATA

    CSliderCtrl m_wndTimerSlider;
    CExportFlcDlg* m_pParent;
    BOOL m_bChanged;
    WORD m_nDelayCur;
    BYTE m_nFrmWidthCur;
    BYTE m_nFrmHeightCur;
    BYTE m_nXOffsetCur;
    BYTE m_nYOffsetCur;
    WORD m_nFrmCountCur;
    float m_nPropXtoY;
    float m_nPropYtoX;
    UINT m_nDelay;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExportFlcFrm)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CExportFlcFrm)
    virtual BOOL OnInitDialog();
    afx_msg void OnFrmCount();
    afx_msg void OnDeltaposFrmCountSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeFrmCount();
    afx_msg void OnChangeFrmHeight();
    afx_msg void OnDeltaposFrmHeightSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnFrmSize();
    afx_msg void OnChangeFrmWidth();
    afx_msg void OnDeltaposFrmWidthSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDelay();
    afx_msg void OnFrmResize();
    afx_msg void OnLock();
    afx_msg void OnFrmOffset();
    afx_msg void OnChangeFrmTop();
    afx_msg void OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeFrmLeft();
    afx_msg void OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeDelay();
    afx_msg void OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeResizeBottom();
    afx_msg void OnDeltaposResizeBottomSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeResizeLeft();
    afx_msg void OnDeltaposResizeLeftSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeResizeRight();
    afx_msg void OnDeltaposResizeRightSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeResizeTop();
    afx_msg void OnDeltaposResizeTopSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnResizeEnable();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(EXPORT_FLC_FRM_H__CIV3FLCEDIT__INCLUDED_)
