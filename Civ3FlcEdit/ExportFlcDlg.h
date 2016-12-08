#if !defined(EXPORT_FLC_H__DPDocPageINCLUDED_)
#define EXPORT_FLC_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportFlcDlg.h : header file
//

#include "ExportFlcGen.h"
#include "ExportFlcOut.h"
#include "ExportFlcFrm.h"
#include "ExportFlcPal.h"
#include "PreviewDlg.h"

#include "FlcView.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

#define REG_EXP_FLC "Dlg ExpFlc"

/////////////////////////////////////////////////////////////////////////////
// CExportFlcDlg dialog

class CExportFlcDlg : public CDialogX, CFramePosition
{
    friend class CFlcView;

    // Construction
  public:
    CExportFlcDlg(CFlcView* pFlc, CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CExportFlcDlg)
    enum
    {
        IDD = IDD_DLG_EXPORT_FLC
    };
    CButtonX m_btnOK;
    CButtonX m_btnCancel;
    CTabCtrl m_wndTab;
    BOOL m_bPreview;
    BOOL m_bSaveSettings;
    //}}AFX_DATA

    CExportFlcGen m_tabG;
    CExportFlcOut m_tabO;
    CExportFlcFrm m_tabF;
    CExportFlcPal m_tabP;

    CString m_sPath; // target path
    CString m_sDir;  // directory
    CString m_sName; // base name

    BYTE m_nFileType;   // file type
    BYTE m_nPalType;    // palette type
    BOOL m_bLeaderHead; // is it leader head animation or not

    BYTE m_nFrmWidth;  // frame width
    BYTE m_nFrmHeight; // frame height
    BYTE m_nXOffset;   // frame left offset
    BYTE m_nYOffset;   // frame top offset
    BOOL m_bFrmResize; // is frame should be resized or not

    WORD m_nFrmCount; // frames count

    BYTE m_nPalette;    // number of palette
    CString m_sPalette; // palette name

    WORD m_nDelay; // frames delay

    BOOL m_bMakePalette;
    ELastPalType m_nLastPalType;
    CString m_sLastPalPath;

    BOOL m_bResizeEnable;
    BYTE m_nResizeLeft;
    BYTE m_nResizeRight;
    BYTE m_nResizeTop;
    BYTE m_nResizeBottom;
    BOOL m_bDefPalette;
    int m_nBitsPerPixel;

    COLORREF m_crDefCiv;

    void PreviewDlgInit();
    void PreviewDlgStart();
    void PreviewDlgStop();
    CPreviewDlg::PREVIEW_PARAM m_PreviewParam;
    CWinThread* m_pThread;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExportFlcDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    virtual LPCTSTR RegKey()
    {
        return REG_EXP_FLC;
    }
    void UpdateControlsState();
    void ActivateTab();
    void AdjustSheetSizes();
    CWnd* GetTab();
    void ApplyChanges();

    CFlcView* m_pFlc;

    // Generated message map functions
    //{{AFX_MSG(CExportFlcDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    virtual void OnOK();
    afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPreviewCheck();
    afx_msg void OnSaveSettings();
    //}}AFX_MSG
    afx_msg LONG OnPreviewCancel(UINT lParam, LONG wParam);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(EXPORT_FLC_H__DPDocPageINCLUDED_)
