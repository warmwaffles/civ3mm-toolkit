#if !defined(FILE_IMG_DLG_H__DPDocPageINCLUDED_)
#define FILE_IMG_DLG_H__DPDocPageINCLUDED_

//#include "FileDialogX.h"
#include "..\MFCaids\FileDialogX.h"
#include "..\MFCaids\BtnST.h"
#include "..\MFCaids\EditX.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileImgDlg.h : header file
//

#define REG_IMG_DLG "Dlg Img"
#define REG_FILTER "Filter"
//#define REG_LOCKED	"FixedSize"
//#define REG_DEPTH	"Depth"

class CFlcImage;

/////////////////////////////////////////////////////////////////////////////
// CFileImgDlg dialog

class CFileImgDlg : public CFileDialogX
{
    DECLARE_DYNAMIC(CFileImgDlg)
    friend class CFlcImage;

  public:
    CFileImgDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
                LPCTSTR lpszDefExt   = NULL,
                LPCTSTR lpszFileName = NULL,
                DWORD dwFlags        = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                LPCTSTR lpszFilter   = NULL,
                CWnd* pParentWnd     = NULL);

    // Dialog Data
    //{{AFX_DATA(CFileImgDlg)
    CButtonST m_btnLock;
    CSpinButtonCtrl m_wndWidthSpin;
    CEditX m_wndWidth;
    CSpinButtonCtrl m_wndHeightSpin;
    CEditX m_wndHeight;
    INT m_nHeight;
    INT m_nWidth;
    BOOL m_bLock;
    CComboBox m_wndBitsPerPixel;
    CStatic m_wndPreview;
    //}}AFX_DATA

    CString m_sFilePath;
    CString m_sExt;
    int m_nFileType;
    int m_nBitsPerPixel;
    CFlcImage* m_pImg;

    enum COLOR_MODE
    {
        cm_256Colors = 8, // 256 colors
        cm_TrueColor = 24 // true color
    };

    // Overrides
    //{{AFX_VIRTUAL(CFileImgDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

  protected:
    CString GetColorModeName(int nBitsPerPixel);
    virtual BOOL OnFileNameOK();

    float m_nPropXtoY;
    float m_nPropYtoX;

    //{{AFX_MSG(CFileImgDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnLock();
    afx_msg void OnChangeHeight();
    afx_msg void OnDeltaposHeightSpin(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeWidth();
    afx_msg void OnDeltaposWidthSpin(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FILE_IMG_DLG_H__DPDocPageINCLUDED_)
