#if !defined(ZOOMINGDLG_H__CIV3FLCEDIT__INCLUDED_)
#define ZOOMINGDLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomingDlg.h : header file
//

#include "Surface.h"
#include "FlcImage.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\ComboZoom.h"
#include "..\MFCaids\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CZoomingDlg dialog
class CSurface;

class CZoomingDlg : public CDialogX, CFramePosition, CFlcImage
{
    // Construction
  public:
    void UpdateControlsState();
    CZoomingDlg(CSurface* pSurface, BYTE nWidth, BYTE nHeight, CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CZoomingDlg)
    enum
    {
        IDD = IDD_ZOOMING_DLG
    };
    CButtonST m_btnZoomFit;
    CScrollBar m_wndScrollVer;
    CScrollBar m_wndScrollHor;
    CButtonX m_btnCancel;
    CButtonX m_btnSave;
    CComboZoom m_wndZooming;
    CButtonX m_btnZoomIn;
    CButtonX m_btnZoomOut;
    CStatic m_wndPreview;
    BOOL m_bZoomFit;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CZoomingDlg)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    BOOL GetImage();
    void UpdatePreview();
    virtual LPCTSTR RegKey()
    {
        return "Dlg Zooming";
    }

    CSurface* m_pSurface;
    int m_nZoomFactor;
    int m_nWidth;
    int m_nHeight;
    int m_nScrollHor;
    int m_nScrollVer;
    CRect m_rcCur;

    // Generated message map functions
    //{{AFX_MSG(CZoomingDlg)
    afx_msg void OnZoomIn();
    afx_msg void OnZoomOut();
    afx_msg void OnSelendokZooming();
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSave();
    afx_msg void OnKillfocusZooming();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnZoomFit();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ZOOMINGDLG_H__CIV3FLCEDIT__INCLUDED_)
