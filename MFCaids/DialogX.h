#if !defined(DIALOG_X_H__MFCAIDS__INCLUDED_)
#define DIALOG_X_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DialogX.h : header file
/*
#include <afxtempl.h>

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
*/
#include "MFCdefs.h"

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogX dialog
class CFormCtrlInfo;
class MFCAIDS_API CDialogX : public CDialog
{
// Construction
public:
	CDialogX(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	void SetControlInfo(UINT CtrlId, WORD Anchor, WORD xDivider = 1, WORD xDivPos = 1, WORD yDivider = 1, WORD yDivPos = 1);
	BOOL GetRememberSize() { return  m_bRememberSize;}
	void SetRememberSize(BOOL bRemember) { m_bRememberSize = bRemember;}
	virtual void GetDialogProfileEntry(CString &sEntry);

// Dialog Data
protected:
	// Set m_bRememberSize to FALSE if you don't want the dialog box
	// to remember its size between sessions.
	BOOL	m_bRememberSize;
	// Set m_bDrawGipper if you don't want a Resizing Gripper
	BOOL	m_bDrawGripper;

private:
	BOOL    m_bFirstSize;
	int		m_minWidth;
	int		m_minHeight;
	int		m_xDlgSize;
	int		m_yDlgSize;
	BOOL	m_bSizeChanged;
	CArray<CFormCtrlInfo, CFormCtrlInfo&> m_CtrlInfo;
	UINT	m_nIDTemplate;
	CRect	m_GripperRect;

protected:
	int     m_yPrevSize;
	int     m_xPrevSize;
	BOOL    m_bRedrawOnResize;

	// Generated message map functions
	//{{AFX_MSG(CDialogX)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* pMMI);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(DIALOG_X_H__MFCAIDS__INCLUDED_)
