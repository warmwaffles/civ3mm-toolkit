// ImgConvertDlg.h : header file
//

#if !defined(IMG_CONVERT_DLG_H__INCLUDED_)
#define IMG_CONVERT_DLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\Utilities.h"
#include "..\MFCaids\ButtonX.h"
#include "..\MFCaids\BtnST.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CImgConvertDlg dialog

class CImgConvertDlg : public CDialogX, CNoSizePosition
{
// Construction
public:
	CImgConvertDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImgConvertDlg)
	enum { IDD = IDD_IMGCONVERT_DIALOG };
	CButton	m_wndUseSize;
	CEditX			m_wndImgCX;
	CEditX			m_wndImgCY;
	CEditX			m_wndFrmPath;
	CEditX			m_wndFrmCX;
	CEditX			m_wndFrmCY;
	CButtonX		m_btnFrmBrowse;
	CButton			m_wndCivPalette;
	CButton			m_wndUseBitsPerPixel;
	CButton			m_wndUsePropResize;
	CEdit			m_wndSuffix;
	CButtonX		m_btnCancel;
	CButtonX		m_btnOK;
	CEditX			m_wndTargetPath;
	CButtonX		m_btnBrowseTargetPath;
	CButtonST		m_btnLock;
	CSpinButtonCtrl	m_wndWidthSpin;
	CEditX			m_wndWidth;
	CSpinButtonCtrl	m_wndHeightSpin;
	CEditX			m_wndHeight;
	UINT			m_nHeight;
	UINT			m_nWidth;
	BOOL			m_bLock;
	CComboBox		m_wndBitsPerPixel;
	CComboBox		m_wndFileType;
	CButtonX		m_btnBrowse;
	CString			m_sTargetPath;
	BOOL			m_bUseBitsPerPixel;
	BOOL			m_bUseFileType;
	BOOL			m_bUseSize;
	BOOL			m_bUseSuffix;
	CString			m_sSuffix;
	BOOL			m_bUsePropResize;
	BOOL			m_bCivPalette;
	UINT			m_nFrmCX;
	UINT			m_nFrmCY;
	CString			m_sFrmPath;
	UINT			m_nImgCX;
	UINT			m_nImgCY;
	BOOL			m_bUseFrm;
	int				m_nFrmConvertType;
	//}}AFX_DATA
	
	enum COLOR_MODE
	{
		cm_256Colors  = 8,  // 256 colors
		cm_TrueColor  = 24  // true color
	};
/*
	enum FILE_TYPE
	{
		ft_PCX = 0,
		ft_BMP = 1,
		ft_JPG = 2
	};
*/
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgConvertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual LPCTSTR RegKey() { return "Dlg ExpFlc"; }
	void UpdateControlsState();

	HICON	m_hIcon;
	float	m_nPropXtoY;
	float	m_nPropYtoX;

	CStringArray m_arPaths;

	// Generated message map functions
	//{{AFX_MSG(CImgConvertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLock();
	afx_msg void OnChangeHeight();
	afx_msg void OnDeltaposHeightSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeWidth();
	afx_msg void OnDeltaposWidthSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBrowsePath();
	afx_msg void OnSelendokFileType();
	virtual void OnOK();
	afx_msg void OnChanged();
	afx_msg void OnBrowseTargetPath();
	afx_msg void OnSelendokBitsPerPixel();
	afx_msg void OnDestroy();
	afx_msg void OnCivPal();
	afx_msg void OnFrmBrowseTargetPath();
	afx_msg void OnFrmTypeChanged();
	afx_msg void OnChangeFrmCx();
	afx_msg void OnChangeFrmCy();
	afx_msg void OnChangeImgCx();
	afx_msg void OnChangeImgCy();
	afx_msg void OnUseFrm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(IMG_CONVERT_DLG_H__INCLUDED_)
