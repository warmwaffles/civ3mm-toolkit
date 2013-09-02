#if !defined(OFFSETDLG_H__CIV3FLCEDIT__INCLUDED_)
#define OFFSETDLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OffsetDlg.h : header file
//

#include "AnimationView.h"

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// COffsetDlg dialog

class COffsetDlg : public CDialog, CNoSizePosition
{
	friend class CAnimationView;

// Construction
public:
	COffsetDlg(CAnimationView* pAnim, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COffsetDlg)
	enum { IDD = IDD_DLG_OFFSET };
	CButtonX		m_btnCancel;
	CButtonX		m_btnOK;
	CEditX			m_wndYOffset;
	CEditX			m_wndXOffset;
	UINT			m_nXOffset;
	UINT			m_nYOffset;
	CSpinButtonCtrl	m_wndYOffsetSpin;
	CSpinButtonCtrl	m_wndXOffsetSpin;
	CString			m_sFrmOffsetCur;
	//}}AFX_DATA

	BYTE			m_nXOffsetCur;
	BYTE			m_nYOffsetCur;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COffsetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual LPCTSTR RegKey() { return "Dlg Offset"; }
	void UpdateControlsState();
	
	CAnimationView*	m_pAnim;
	BOOL			m_bChanged;
	BYTE			m_nFrmHeight;
	BYTE			m_nFrmWidth;

	// Generated message map functions
	//{{AFX_MSG(COffsetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnChangeFrmTop();
	afx_msg void OnChangeFrmLeft();
	afx_msg void OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(OFFSETDLG_H__CIV3FLCEDIT__INCLUDED_)
