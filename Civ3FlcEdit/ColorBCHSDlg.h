#if !defined(COLOR_BCHS_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define COLOR_BCHS_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBCHSDlg.h : header file
//
#include "FlcImage.h"

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CColorBCHSDlg dialog

class CColorBCHSDlg : public CDialog, CNoSizePosition
{
    // Construction
  public:
    void SetupBitmap();
    void ChangeSaturation();
    void ChangeHue();
    void ChangeContrast();
    void ChangeBrigtness();
    CColorBCHSDlg(RGBQUADPAL* pPalD, CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CColorBCHSDlg)
    enum
    {
        IDD = IDD_COLOR_BCHS
    };
    CStatic m_wndPreview;
    CEditX m_wndB;
    CEditX m_wndC;
    CEditX m_wndH;
    CEditX m_wndS;
    CSpinButtonCtrl m_wndSpinB;
    CSpinButtonCtrl m_wndSpinC;
    CSpinButtonCtrl m_wndSpinH;
    CSpinButtonCtrl m_wndSpinS;
    CButtonX m_btnCancel;
    CButtonX m_btnOK;
    CButtonX m_btnRefresh;
    CButtonX m_btnApply;
    int m_nB;
    int m_nC;
    int m_nH;
    BOOL m_bPreview;
    int m_nS;
    //}}AFX_DATA

    CSliderCtrl m_wndSliderB;
    CSliderCtrl m_wndSliderC;
    CSliderCtrl m_wndSliderH;
    CSliderCtrl m_wndSliderS;

    RGBQUADPAL* m_pPalExtD;
    RGBQUADPAL m_rgbPalD;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorBCHSDlg)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    virtual LPCTSTR RegKey()
    {
        return "Dlg Range";
    }
    void UpdateControlsState();
    void UpdatePreview();

    CString m_sValue;
    CFlcImage m_img;
    CFlcImage m_imgPrev;

    // Generated message map functions
    //{{AFX_MSG(CColorBCHSDlg)
    afx_msg void OnChangeBright();
    afx_msg void OnDeltaposSpinBright(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeContrast();
    afx_msg void OnDeltaposSpinContrast(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeHue();
    afx_msg void OnDeltaposSpinHue(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeSaturation();
    afx_msg void OnDeltaposSpinSaturation(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnApply();
    afx_msg void OnRefresh();
    virtual void OnOK();
    afx_msg void OnPreviewCheck();
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    //}}AFX_MSG
    afx_msg void OnReleasedcaptureSliderB(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureSliderC(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureSliderH(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureSliderS(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(COLOR_BCHS_DLG_H__CIV3FLCEDIT__INCLUDED_)
