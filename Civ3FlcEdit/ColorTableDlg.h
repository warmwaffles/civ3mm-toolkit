#if !defined(COLOR_TABLE_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define COLOR_TABLE_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorTableDlg.h : header file
//

#include "ColorTable.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CColorTableDlg dialog

class CColorTableDlg : public CDialogX, CNoSizePosition
{
	friend class CColorTable;

// Construction
public:
	void ChangeSelectedRGB();
	CColorTableDlg(RGBQUADPAL* pPal, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorTableDlg)
	enum { IDD = IDD_COLOR_TABLE_DLG };
	CButtonX	m_btnOK;
	CButtonX	m_btnSelect;
	CButtonX	m_btnCancel;
	CStatic		m_wndSelColor;
	CStatic		m_wndCurColor;
	CEditX		m_wndRedS;
	CEditX		m_wndGreenS;
	CEditX		m_wndBlueS;
	CString		m_sRed;
	CString		m_sIndex;
	CString		m_sGreen;
	CString		m_sBlue;
	CString		m_sIndexS;
	UINT		m_nBlueS;
	UINT		m_nGreenS;
	UINT		m_nRedS;
	//}}AFX_DATA

	CColorTable	m_wndColorTable;
	RGBQUADPAL*	m_pPal;			// current palette
	COLORREF	m_crCurrent;	// current color
	COLORREF	m_crSelected;	// selected color
	HBRUSH		m_brCurrent;
	HBRUSH		m_brSelected;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorTableDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateColorInfo();
	virtual LPCTSTR RegKey() { return "Dlg Palette"; }

	// Generated message map functions
	//{{AFX_MSG(CColorTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelectColor();
	virtual void OnCancel();
	afx_msg void OnChangeColorBlueS();
	afx_msg void OnChangeColorGreenS();
	afx_msg void OnChangeColorRedS();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(COLOR_TABLE_DLG_H__CIV3FLCEDIT__INCLUDED_)
