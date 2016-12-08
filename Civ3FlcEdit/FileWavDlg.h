#if !defined(FILE_WAV_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define FILE_WAV_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileWavDlg.h : header file
//

//#include "FileDialogX.h"
#include "..\MFCaids\FileDialogX.h"
#include "..\MFCaids\ButtonX.h"

/////////////////////////////////////////////////////////////////////////////
// CFileWavDlg dialog

class CFileWavDlg : public CFileDialogX
{
    DECLARE_DYNAMIC(CFileWavDlg)

  public:
    CFileWavDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
                LPCTSTR lpszDefExt   = NULL,
                LPCTSTR lpszFileName = NULL,
                DWORD dwFlags        = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                LPCTSTR lpszFilter   = NULL,
                CWnd* pParentWnd     = NULL);

    // Dialog Data
    //{{AFX_DATA(CFileWavDlg)
    CButtonX m_btnPlay;
    CButtonX m_btnStop;
    CButton m_btnPreview;
    CButton m_btnNonStop;
    BOOL m_bPreview;
    BOOL m_bNonStop;
    //}}AFX_DATA

    // Overrides
    //{{AFX_VIRTUAL(CFileWavDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

  protected:
    void StopSoundFile();
    void PlaySoundFile();
    virtual void OnFolderChange();
    virtual void OnFileNameChange();

    CString m_sFile;

    //{{AFX_MSG(CFileWavDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPlay();
    afx_msg void OnStop();
    afx_msg void OnPreview();
    afx_msg void OnNonStop();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FILE_WAV_DLG_H__CIV3FLCEDIT__INCLUDED_)
