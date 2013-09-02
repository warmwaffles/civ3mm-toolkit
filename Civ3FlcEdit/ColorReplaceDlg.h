#if !defined(COLOR_REPLACE_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define COLOR_REPLACE_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorReplaceDlg.h : header file
//

#include "ColorTable.h"
#include "ColorTableSrc.h"
#include "ComboBoxCivColor.h"
#include "FlcImage.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CColorReplaceDlg dialog

class CColorReplaceDlg : public CDialogX, CNoSizePosition
{
	friend class CColorTable;
	friend class CColorTableSrc;
	friend class CFlcImage;

// Construction
public:
	void ApplyColorChanges();
	void SetPaletteS();
	void SetPaletteD();
	BOOL GetPalette(CString sPath, RGBQUADPAL& palette);
	void ChangeSelectedRGB();
	CColorReplaceDlg(RGBQUADPAL* pPal = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorReplaceDlg)
	enum { IDD = IDD_COLOR_REPLACE_DLG };
	CButtonX			m_btnImgSelect;
	CButtonX			m_btnRefresh;
	CButtonX			m_btnApply;
	CButtonX			m_btnBCHS;
	CButtonX			m_btnMove;
	CStatic				m_wndCurColorSrc;
	CStatic				m_wndSelColorSrc;
	CEditX				m_wndPathS;
	CButtonX			m_btnSave;
	CButtonX			m_btnReplaceRange;
	CButtonX			m_btnReplaceColor;
	CEditX				m_wndPathD;
	CButtonX			m_btnBrowseS;
	CButtonX			m_btnBrowseD;
	CButtonX			m_btnOK;
	CButtonX			m_btnSelect;
	CButtonX			m_btnCancel;
	CStatic				m_wndSelColor;
	CStatic				m_wndCurColor;
	CEditX				m_wndRedS;
	CEditX				m_wndGreenS;
	CEditX				m_wndBlueS;
	CString				m_sRed;
	CString				m_sIndex;
	CString				m_sGreen;
	CString				m_sBlue;
	CString				m_sIndexS;
	UINT				m_nBlueS;
	UINT				m_nGreenS;
	UINT				m_nRedS;
	CString				m_sPathD;
	CString				m_sPathS;
	CString				m_sBlueSrc;
	CString				m_sGreenSrc;
	CString				m_sIndexSrc;
	CString				m_sRedSrc;
	CString				m_sBlueSrcS;
	CString				m_sGreenSrcS;
	CString				m_sIndexSrcS;
	CString				m_sRedSrcS;
	CComboBoxCivColor	m_wndPalettes;
	int					m_nPalette;
	int					m_nSrcPalType;
	//}}AFX_DATA

	CColorTable			m_wndColorTableD; // destination color table
	CColorTableSrc		m_wndColorTableS; // source color table
	RGBQUADPAL			m_rgbPalD; // destination palette
	RGBQUADPAL			m_rgbPalS; // source palette
	RGBQUADPAL*			m_pPalExt; // external palette
	RGBQUADPAL			m_rgbPalCur;
	COLORREF			m_crDest; // destination palette's current color
	COLORREF			m_crSrc; // source palette's current color
	HBRUSH				m_brDest; // destination palette's current color's bruch
	HBRUSH				m_brSrc; // source palette's current color's bruch
	COLORREF			m_crCurDest; // destination palette's current color
	COLORREF			m_crCurSrc; // source palette's current color
	HBRUSH				m_brCurDest; // destination palette's current color's bruch
	HBRUSH				m_brCurSrc; // source palette's current color's bruch

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorReplaceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ClearPalette(RGBQUADPAL& palette);
	void UpdateControlsState();
	void UpdateColorInfo(UINT nID);
	virtual LPCTSTR RegKey() { return "Dlg ColorReplace"; }
	
	BOOL ChangePalette(BYTE nColor);
	BOOL GetCivColorPalette(RGBQUADPAL *pPal, LPCTSTR lpszImgPath = NULL);
	BOOL SetCivColorPalette(LPCTSTR lpszImgPath = NULL);

	// Generated message map functions
	//{{AFX_MSG(CColorReplaceDlg)
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
	afx_msg void OnReplaceRange();
	afx_msg void OnBrowseDestPath();
	afx_msg void OnBrowseSrcPath();
	afx_msg void OnReplaceColor();
	afx_msg void OnSave();
	afx_msg void OnSelendokComboPalettes();
	afx_msg void OnMoveColors();
	afx_msg void OnSelPalette();
	afx_msg void OnCivPalette();
	afx_msg void OnBCHS();
	afx_msg void OnRefresh();
	afx_msg void OnApply();
	afx_msg void OnSelectImg();
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(COLOR_REPLACE_DLG_H__CIV3FLCEDIT__INCLUDED_)
