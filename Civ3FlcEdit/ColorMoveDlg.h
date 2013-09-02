#if !defined(COLOR_MOVE_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define COLOR_MOVE_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorMoveDlg.h : header file
//

#include "ColorReplaceDlg.h"
#include "ColorTablePreview.h"

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CColorMoveDlg dialog

class CColorMoveDlg : public CDialog, CNoSizePosition
{
	friend class CColorReplaceDlg;
	friend class CColorTablePreview;

// Construction
public:
	void ChangeVariables();
	CColorMoveDlg(RGBQUADPAL* pPal, CWnd* pParent = NULL);   // standard constructor
	void MoveTo(UINT nFrom, UINT nTo, UINT nNumber);

// Dialog Data
	//{{AFX_DATA(CColorMoveDlg)
	enum { IDD = IDD_COLOR_MOVE_DLG };
	CButtonX	m_btnRefresh;
	CButtonX	m_btnApply;
	CButtonX	m_btnOK;
	CButtonX	m_btnCancel;
	CButtonX	m_btnRight;
	CButtonX	m_btnLeft;
	CButtonX	m_btnMove;
	CEditX		m_wndTo;
	CEditX		m_wndFrom;
	CEditX		m_wndNumber;
	BOOL		m_bAllPalette;
	UINT		m_nFrom;
	UINT		m_nTo;
	UINT		m_nNumber;
	//}}AFX_DATA

	CColorTablePreview	m_wndColorTableD;
	RGBQUADPAL*			m_pPalExtD;
	RGBQUADPAL			m_rgbPalD;
	RGBQUADPAL			m_rgbPalTemp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorMoveDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual LPCTSTR RegKey() { return "Dlg MoveColor"; }
	void UpdateControlsState();

	// Generated message map functions
	//{{AFX_MSG(CColorMoveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeRange();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveLeft();
	afx_msg void OnMove();
	afx_msg void OnChangeFirstSrc();
	afx_msg void OnChangeFirstDest();
	afx_msg void OnAllPalette();
	afx_msg void OnPaint();
	afx_msg void OnApply();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(COLOR_MOVE_DLG_H__CIV3FLCEDIT__INCLUDED_)
