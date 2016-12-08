#if !defined(FILE_IMG_ALL_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define FILE_IMG_ALL_DLG_H__CIV3FLCEDIT__INCLUDED_

//#include "FileDialogX.h"
#include "..\MFCaids\FileDialogX.h"
#include "..\MFCaids\GraphXImage.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileImgAllDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileImgAllDlg dialog

class CFileImgAllDlg : public CFileDialogX
{
    DECLARE_DYNAMIC(CFileImgAllDlg)

  public:
    CFileImgAllDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
                   LPCTSTR lpszDefExt   = NULL,
                   LPCTSTR lpszFileName = NULL,
                   DWORD dwFlags        = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                   LPCTSTR lpszFilter   = NULL,
                   CWnd* pParentWnd     = NULL);

    CString m_sFilePath;

  protected:
    virtual BOOL OnFileNameOK();
    virtual void OnFolderChange();
    virtual void OnFileNameChange();

    CGraphXImage m_img;
    BOOL m_bPreview;

    //{{AFX_MSG(CFileImgAllDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPreview();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FILE_IMG_ALL_DLG_H__CIV3FLCEDIT__INCLUDED_)
