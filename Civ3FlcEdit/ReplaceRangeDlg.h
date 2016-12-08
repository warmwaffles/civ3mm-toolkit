#if !defined(REPLACE_RANGE_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define REPLACE_RANGE_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplaceRangeDlg.h : header file
//

#include "ColorReplaceDlg.h"
#include "ColorTablePreview.h"

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CReplaceRangeDlg dialog

class CReplaceRangeDlg : public CDialog, CNoSizePosition
{
    friend class CColorReplaceDlg;
    friend class CColorTablePreview;

    // Construction
  public:
    void ChangeVariables();
    CReplaceRangeDlg(RGBQUADPAL* pPalD, RGBQUADPAL* pPalS, CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CReplaceRangeDlg)
    enum
    {
        IDD = IDD_REPLACE_RANGE_DLG
    };
    CButtonX m_btnRefresh;
    CButtonX m_btnApply;
    CButtonX m_btnOK;
    CButtonX m_btnCancel;
    CButtonX m_btnMove;
    CEditX m_wndTo;
    CEditX m_wndFrom;
    CEditX m_wndNumber;
    UINT m_nFrom;
    UINT m_nTo;
    UINT m_nNumber;
    //}}AFX_DATA

    CColorTablePreview m_wndColorTableD;
    RGBQUADPAL* m_pPalExtD;
    RGBQUADPAL m_rgbPalD;

    CColorTablePreview m_wndColorTableS;
    RGBQUADPAL* m_pPalExtS;
    RGBQUADPAL m_rgbPalS;

    BOOL m_bAllPalette;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CReplaceRangeDlg)
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

    // Generated message map functions
    //{{AFX_MSG(CReplaceRangeDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnChangeRange();
    afx_msg void OnMove();
    afx_msg void OnChangeFirstSrc();
    afx_msg void OnChangeFirstDest();
    afx_msg void OnPaint();
    afx_msg void OnApply();
    afx_msg void OnRefresh();
    //}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(REPLACE_RANGE_DLG_H__CIV3FLCEDIT__INCLUDED_)
