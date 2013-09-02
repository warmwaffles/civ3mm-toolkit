#if !defined(SHRINK_FRM_COUNT_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define SHRINK_FRM_COUNT_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShrinkFrmCountDlg.h : header file
//
#include "FlcImage.h"

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CShrinkFrmCountDlg dialog

class CShrinkFrmCountDlg : public CDialog, CFlcImage, CNoSizePosition
{
	friend class CC3fView;

// Construction
public:
	CShrinkFrmCountDlg(CC3fView* pC3fView, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShrinkFrmCountDlg)
	enum { IDD = IDD_SHRINK_FRM_COUNT_DLG };
	CEditX			m_wndFrmCount;
	CSpinButtonCtrl	m_wndFrmCountSpin;
	CButtonX		m_btnCancel;
	CButtonX		m_btnOK;
	UINT			m_nFrmCount;
	CString			m_sFrmCountCur;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShrinkFrmCountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL ShrinkFrmCount();
	virtual LPCTSTR RegKey() { return "Dlg ShrinkFrm"; }

	CC3fView*	m_pC3fView;
	UINT		m_nCurFrmCount;

	// Generated message map functions
	//{{AFX_MSG(CShrinkFrmCountDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnChangeFrmCount();
	afx_msg void OnDeltaposFrmCountSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(SHRINK_FRM_COUNT_DLG_H__CIV3FLCEDIT__INCLUDED_)
