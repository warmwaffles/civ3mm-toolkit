#if !defined(SELECT_BGD_IMG_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define SELECT_BGD_IMG_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectBgdImgDlg.h : header file
//

#include "..\MFCaids\GraphXImage.h"
#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectBgdImgDlg dialog

class CSelectBgdImgDlg : public CDialogX, CGraphXImage, CFramePosition
{
// Construction
public:
	CSelectBgdImgDlg(CWnd* pParent = NULL);   // standard constructor
	
	BOOL LoadBgdImgFile(LPCTSTR sPath);

// Dialog Data
	//{{AFX_DATA(CSelectBgdImgDlg)
	enum { IDD = IDD_DLG_SELECT_BGD_IMG };
	CButtonX	m_btnOK;
	CButtonX	m_btnCancel;
	CStatic		m_wndPreview;
	CEditX		m_wndPath;
	CButtonX	m_btnDelete;
	CButtonX	m_btnBrowse;
	CString		m_sTargetPath;
	CString		m_sInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectBgdImgDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual LPCTSTR RegKey() { return "Dlg SelectBgdImg"; }
	void UpdateControlsState();
	void UpdatePreview();

	// Generated message map functions
	//{{AFX_MSG(CSelectBgdImgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnBrowse();
	afx_msg void OnDelete();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(SELECT_BGD_IMG_DLG_H__CIV3FLCEDIT__INCLUDED_)
