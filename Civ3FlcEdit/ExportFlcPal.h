#if !defined(EXPORT_FLC_PAL_H__DPDocPageINCLUDED_)
#define EXPORT_FLC_PAL_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportFlcPal.h : header file
//

#include "ComboBoxCivColor.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CExportFlcPal dialog
class CExportFlcDlg;
class CExportFlcPal : public CDialogX
{
// Construction
public:
	void ApplyChanges();
	CExportFlcPal(CWnd* pParent = NULL);   // standard constructor
	void UpdateControlsState();

// Dialog Data
	//{{AFX_DATA(CExportFlcPal)
	enum { IDD = IDD_TAB_EXP_PAL };
	CButton				m_btnMakePalette;
	CButton				m_btnDefPalette;
	CEditX				m_wndLastPalPath;
	CButtonX			m_btnBrowseLastCol;
	CComboBox			m_wndPalType;
	int					m_nPalType;
	CString				m_sLastPalPath;
	int					m_nLastPalType;
	BOOL				m_bDefPalette;
	int					m_nBitsPerPixel;
	BOOL				m_bMakePalette;
	int					m_nPaletteType;
	int					m_nPalette;
	CString				m_sPaletteCur;
	CComboBoxCivColor	m_wndPalettes;
	//}}AFX_DATA

	CExportFlcDlg*	m_pParent;
	BOOL			m_bChanged;

	BYTE			m_nPaletteCur;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportFlcPal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HBRUSH		m_brCurrent;

	// Generated message map functions
	//{{AFX_MSG(CExportFlcPal)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokPalType();
	afx_msg void OnBrowseLastColorsPath();
	afx_msg void OnLastColors();
	afx_msg void OnBitsPerPixel();
	afx_msg void OnMakePalette();
	afx_msg void OnDestroy();
	afx_msg void OnPalette();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelendokComboPalettes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(EXPORT_FLC_PAL_H__DPDocPageINCLUDED_)
