#if !defined(OPTIONS_DLG_DOC_H__CIV3FLCEDIT__INCLUDED_)
#define OPTIONS_DLG_DOC_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlgDoc.h : header file
//

#include "..\MFCaids\DialogX.h"
#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlgDoc dialog

class COptionsDlgDoc : public CDialogX
{
// Construction
public:
	void SetDefault();
	void Apply();
	COptionsDlgDoc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlgDoc)
	enum { IDD = IDD_OPTIONSDLG_DOC };
	CEditX	m_wndMRU;
	CSpinButtonCtrl	m_wndSpinMRU;
	UINT	m_nMRU;
	BOOL	m_bLoadLastDoc;
	BOOL	m_bCheckFileType;
	BOOL	m_bRegFileType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlgDoc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnChangeData();

	// Generated message map functions
	//{{AFX_MSG(COptionsDlgDoc)
	virtual BOOL OnInitDialog();
	afx_msg void OnChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(OPTIONS_DLG_DOC_H__CIV3FLCEDIT__INCLUDED_)
