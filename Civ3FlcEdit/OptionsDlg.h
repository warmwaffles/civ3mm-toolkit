#if !defined(OPTIONS_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define OPTIONS_DLG_H__CIV3FLCEDIT__INCLUDED_

#include "OptionsDlgView.h"
#include "OptionsDlgDoc.h"
#include "OptionsDlgFlc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

#include "MainFrm.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog
class CPostelMainFrame;

class COptionsDlg : public CDialogX, CFramePosition
{
// Construction
public:
	BOOL m_bChanged;
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_DLG_OPTIONS };
	CButtonX	m_btnApply;
	CButtonX	m_btnDefault;
	CButtonX	m_btnCancel;
	CButtonX	m_btnOK;
	CTabCtrl	m_wndTab;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual LPCTSTR RegKey() { return "Dlg Options"; }
	void ActivateTab();
	void AdjustSheetSizes();
	CWnd* GetTab();

	COptionsDlgView		m_tabV;
	COptionsDlgDoc		m_tabD;
	COptionsDlgFlc		m_tabF;

	CImageList			m_TabImg;

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(OPTIONS_DLG_H__CIV3FLCEDIT__INCLUDED_)
