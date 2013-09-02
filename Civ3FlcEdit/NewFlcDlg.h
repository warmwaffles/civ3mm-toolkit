#if !defined(NEW_FLC_H__DPDocPageINCLUDED_)
#define NEW_FLC_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFlcDlg.h : header file
//

#include "NewFlcGen.h"
#include "NewFlcOut.h"
#include "NewFlcFrm.h"
#include "NewFlcPal.h"
#include "PreviewDlg.h"

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\EditX.h"

#define REG_NEW_FLC		"Dlg NewFlc"

/////////////////////////////////////////////////////////////////////////////
// CNewFlcDlg dialog

class CNewFlcDlg : public CDialogX, CFramePosition
{
	friend class CFlc;

// Construction
public:
	void UpdateValues();
	CNewFlcDlg(CWnd* pParent = NULL);   // standard constructor
	void ApplyChanges();

// Dialog Data
	//{{AFX_DATA(CNewFlcDlg)
	enum { IDD = IDD_DLG_NEW_FLC };
	CButtonX	m_btnOK;
	CButtonX	m_btnCancel;
	CTabCtrl	m_wndTab;
	BOOL		m_bPreview;
	BOOL		m_bSaveSettings;
	//}}AFX_DATA

	CNewFlcGen		m_tabG;
	CNewFlcOut		m_tabO;
	CNewFlcFrm		m_tabF;
	CNewFlcPal		m_tabP;

	CString			m_sPath;		// target path
	CString			m_sDir;			// directory
	CString			m_sName;		// base name

	BYTE			m_nFileType;	// file type
	BYTE			m_nPalType;		// palette type
	BOOL			m_bLeaderHead;	// is it leader head animation or not

	BYTE			m_nFrmWidth;	// frame width
	BYTE			m_nFrmHeight;	// frame height
	BYTE			m_nXOffset;		// frame left offset
	BYTE			m_nYOffset;		// frame top offset
	
	WORD			m_nFrmCount;	// frames count
	BYTE			m_nDirCount;	// directions count
	
	BOOL			m_bDefPalette;
	BYTE			m_nPalette;		// number of palette
	CString			m_sPalette;		// palette name

	WORD			m_nDelay;		// frames delay
	int				m_nBitsPerPixel;

	BOOL			m_bMakePalette; // make a separate palette file or not

	ELastPalType	m_nLastPalType; // type of the last 32 colors palette
	CString			m_sLastPalPath; // path of the external palette file for last 32 colors
	
	BOOL			m_bSelPalPath;  // use or not a selective palette
	CString			m_sSelPalPath;  // path of the external palette file for selective palette
	
	void PreviewDlgInit();
	void PreviewDlgStart();
	void PreviewDlgStop();
	CPreviewDlg::PREVIEW_PARAM m_PreviewParam;
	CWinThread*	m_pThread;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFlcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual LPCTSTR RegKey() { return REG_NEW_FLC; }
	void UpdateControlsState();
	void ActivateTab();
	void AdjustSheetSizes();
	CWnd* GetTab();

	// Generated message map functions
	//{{AFX_MSG(CNewFlcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPreviewCheck();
	afx_msg void OnSaveSettings();
	//}}AFX_MSG
	afx_msg LONG OnPreviewCancel(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(NEW_FLC_H__DPDocPageINCLUDED_)
