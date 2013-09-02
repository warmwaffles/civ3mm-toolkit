#if !defined(NEW_FLC_FRM_H__DPDocPageINCLUDED_)
#define NEW_FLC_FRM_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFlcFrm.h : header file
//

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\EditX.h"
#include "..\MFCaids\BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CNewFlcFrm dialog

class CNewFlcDlg;
class CNewFlcFrm : public CDialogX
{
// Construction
public:
	void UpdatePreview();
	void UpdateValues();
	void ApplyChanges();
	CNewFlcFrm(CWnd* pParent = NULL);   // standard constructor
	void UpdateControlsState();

// Dialog Data
	//{{AFX_DATA(CNewFlcFrm)
	enum { IDD = IDD_TAB_NEW_FRM };
	CSpinButtonCtrl	m_wndDelaySpin;
	CEditX			m_wndDelay;
	CSpinButtonCtrl	m_wndYOffsetSpin;
	CEditX			m_wndYOffset;
	CSpinButtonCtrl	m_wndXOffsetSpin;
	CEditX			m_wndXOffset;
	CComboBox		m_wndDirCount;
	CSpinButtonCtrl	m_wndFrmCountSpin;
	CSpinButtonCtrl	m_wndFrmWidthSpin;
	CEditX			m_wndFrmWidth;
	CSpinButtonCtrl	m_wndFrmHeightSpin;
	CEditX			m_wndFrmHeight;
	CEditX			m_wndFrmCount;
	UINT			m_nFrmCount;
	UINT			m_nFrmHeight;
	UINT			m_nFrmWidth;
	int				m_nCountType;
	int				m_nSizeType;
	int				m_nDelayType;
	CString			m_sDelayCur;
	CString			m_sFrmCountCur;
	CString			m_sFrmSizeCur;
	UINT			m_nDirCount;
	CString			m_sDirCountCur;
	int				m_nDirType;
	CButtonST		m_btnLock;
	BOOL			m_bLock;
	UINT			m_nXOffset;
	int				m_nOffsetType;
	UINT			m_nYOffset;
	CString			m_sFrmOffsetCur;
	CString			m_sDelay;
	//}}AFX_DATA
	
	CSliderCtrl		m_wndTimerSlider;
	CNewFlcDlg*		m_pParent;
	BOOL			m_bChanged;
	WORD			m_nDelayCur;
	BYTE			m_nFrmWidthCur;
	BYTE			m_nFrmHeightCur;
	BYTE			m_nXOffsetCur;
	BYTE			m_nYOffsetCur;
	WORD			m_nFrmCountCur;
	BYTE			m_nDirCountCur;
	float			m_nPropXtoY;
	float			m_nPropYtoX;
	UINT			m_nDelay;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFlcFrm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewFlcFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnFrmCount();
	afx_msg void OnDeltaposFrmCountSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeFrmCount();
	afx_msg void OnChangeFrmHeight();
	afx_msg void OnDeltaposFrmHeightSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFrmSize();
	afx_msg void OnChangeFrmWidth();
	afx_msg void OnDeltaposFrmWidthSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelay();
	afx_msg void OnChangeDirCount();
	afx_msg void OnDirCount();
	afx_msg void OnLock();
	afx_msg void OnFrmOffset();
	afx_msg void OnChangeFrmTop();
	afx_msg void OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeFrmLeft();
	afx_msg void OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDelay();
	afx_msg void OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(NEW_FLC_FRM_H__DPDocPageINCLUDED_)
