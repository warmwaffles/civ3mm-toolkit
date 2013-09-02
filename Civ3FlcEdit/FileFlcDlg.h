#if !defined(FILE_FLC_DIALOG_H__INCLUDED)
#define FILE_FLC_DIALOG_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileFlcDlg.h : header file
//

//#include "FileDialogX.h"
#include "..\MFCaids\FileDialogX.h"
#include "..\MFCaids\GraphXImage.h"

/////////////////////////////////////////////////////////////////////////////
// CFileFlcDlg dialog

class CFileFlcDlg : public CFileDialogX
{
	DECLARE_DYNAMIC(CFileFlcDlg)

public:
	CFileFlcDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt   = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags        = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter   = NULL,
		CWnd* pParentWnd     = NULL);

protected:
	void LoadFileForPreview();
	virtual BOOL OnFileNameOK();
	virtual void OnFolderChange();
	virtual void OnFileNameChange();
	
	CGraphXImage	m_img;
	BOOL			m_bPreview;
	BOOL			m_bFlc;

	//{{AFX_MSG(CFileFlcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPreview();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FILE_FLC_DIALOG_H__INCLUDED)
