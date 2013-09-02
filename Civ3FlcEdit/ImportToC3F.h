#if !defined(IMPORT_TO_C3F_H__CIV3FLCEDIT__INCLUDED_)
#define IMPORT_TO_C3F_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportToC3F.h : header file
//
#include "FlcImage.h"

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CImportToC3F dialog

class CImportToC3F : public CDialog, CFlcImage, CNoSizePosition
{
// Construction
public:
	CImportToC3F(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImportToC3F)
	enum { IDD = IDD_DLG_IMPORT };
	CButtonX		m_btnCentering;
	CComboBox		m_wndFileType;
	CEditX			m_wndPalPath;
	CButtonX		m_btnBrowsePal;
	CButtonX		m_btnOK;
	CButtonX		m_btnCancel;
	CButtonX		m_btnBrowseTarget;
	CEditX			m_wndDelay;
	CButtonX		m_btnBrowseOutput;
	CEditX			m_wndTargetPath;
	CStatic			m_wndInfo;
	CSpinButtonCtrl	m_wndDelaySpin;
	CStatic			m_wndPreview;
	CComboBox		m_wndPalType;
	CEditX			m_wndOutputPath;
	CSpinButtonCtrl	m_wndFrmWidthSpin;
	CEditX			m_wndFrmWidth;
	CSpinButtonCtrl	m_wndYOffsetSpin;
	CEditX			m_wndYOffset;
	CSpinButtonCtrl	m_wndXOffsetSpin;
	CEditX			m_wndXOffset;
	CSpinButtonCtrl	m_wndFrmHeightSpin;
	CEditX			m_wndFrmHeight;
	UINT			m_nFrmWidth;
	UINT			m_nFrmHeight;
	UINT			m_nXOffset;
	UINT			m_nYOffset;
	CString			m_sOutputPath;
	int				m_nPalType;
	CString			m_sInfo;
	CString			m_sTargetPath;
	CString			m_sDelay;
	CString			m_sPalPath;
	int				m_nSrcPalType;
	int				m_nFileType;
	BOOL			m_bShowLines;
	BOOL			m_bPreview;
	BOOL			m_bShowGrid;
	BOOL			m_bSaveSettings;
	int				m_nBitsPerPixel;
	BOOL			m_bGrid;
	//}}AFX_DATA

	CSliderCtrl		m_wndTimerSlider;
	UINT			m_nDelay;
	UINT			m_nGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportToC3F)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL ImportToC3F();
	virtual LPCTSTR RegKey() { return "Dlg ImpC3F"; }
	void UpdateControlsState();
	void UpdatePreview();

	// Generated message map functions
	//{{AFX_MSG(CImportToC3F)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBrowseTargetPath();
	afx_msg void OnBrowseOutputPath();
	afx_msg void OnBrowsePalPath();
	afx_msg void OnPalChanged();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnChangeFrmWidth();
	afx_msg void OnDeltaposFrmWidthSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeFrmHeight();
	afx_msg void OnDeltaposFrmHeightSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeFrmLeft();
	afx_msg void OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeFrmTop();
	afx_msg void OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowLines();
	afx_msg void OnChangeDelay();
	afx_msg void OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCentering();
	afx_msg void OnPreviewCheck();
	afx_msg void OnShowGrid();
	afx_msg void OnSaveSettings();
	afx_msg void OnBitsPerPixel();
	afx_msg void OnGrid();
	//}}AFX_MSG
	afx_msg void OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(IMPORT_TO_C3F_H__CIV3FLCEDIT__INCLUDED_)
