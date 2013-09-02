#if !defined(FLC_INFO_GEN_H__DPDocPageINCLUDED_)
#define FLC_INFO_GEN_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlcInfoGen.h : header file
//

#include "..\MFCaids\ListCtrlX.h"
#include "..\MFCaids\DialogX.h"

/////////////////////////////////////////////////////////////////////////////
// CListCtrlSfEdit list ctrl
class CListFlcInfoGen : public CListCtrlX  
{
public:
	CListFlcInfoGen() {};
	virtual ~CListFlcInfoGen() {};
	
protected:
	virtual LPCTSTR RegKey() { return REG_SF_INFO_DLG; }
	virtual BOOL IsEditable(int nRow, int nCol) { return FALSE; }
};

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoGen dialog
class CFlcInfoDlg;
class CFlcInfoGen : public CDialogX
{
// Construction
public:
	CFlcInfoGen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlcInfoGen)
	enum { IDD = IDD_TAB_INFO_GEN };
	CListFlcInfoGen	m_Table;
	//}}AFX_DATA

	CFlcInfoDlg*	m_pParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlcInfoGen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlcInfoGen)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FLC_INFO_GEN_H__DPDocPageINCLUDED_)
