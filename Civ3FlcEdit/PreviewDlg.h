#if !defined(PREVIEW_DLG_H__CIV3FLCEDIT__INCLUDED_)
#define PREVIEW_DLG_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewDlg.h : header file
//
#include "FlcImage.h"

#include "..\MFCaids\FramePosition.h"

#include <afxmt.h>

#define PREVIEW_CANCEL     WM_USER + 1005        // dialog end (cancelled)

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog

class CPreviewDlg : public CDialog, CFlcImage, CNoSizePosition
{
// Construction
public:
	void ShowPreviewWindow(BOOL bShow = TRUE);
	CPreviewDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdatePreview();
	
// Dialog Data
	//{{AFX_DATA(CPreviewDlg)
	enum { IDD = IDD_PREVIEW_DLG };
	CStatic	m_wndPreview;
	BOOL	m_bShowLines;
	BOOL	m_bShowGrid;
	//}}AFX_DATA

	typedef struct _PREVIEW_PARAM
	{
		CSurface*	pSurface;
		BOOL		bShow;
		BOOL		bExit;
		BOOL		bFrmResize;
		BOOL		bResizeEnable;
		UINT		nFrmWidth;
		UINT		nFrmHeight;
		UINT		nFrmWidthCur;
		UINT		nFrmHeightCur;
		UINT		nXOffset;
		UINT		nYOffset;
		
		UINT		nResizeLeft;
		UINT		nResizeRight;
		UINT		nResizeTop;
		UINT		nResizeBottom;
		HWND		hWnd;
		COLORREF	colTransp;
		
		_PREVIEW_PARAM()
		{
			pSurface		= NULL;
			bShow			= TRUE;
			bExit			= FALSE;
			bFrmResize		= FALSE;
			bResizeEnable	= FALSE;
			nFrmWidth		= 0;
			nFrmHeight		= 0;
			nFrmWidthCur	= 0;
			nFrmHeightCur	= 0;
			nXOffset		= 0;
			nYOffset		= 0;
			nResizeLeft		= 0;
			nResizeRight	= 0;
			nResizeTop		= 0;
			nResizeBottom	= 0;
			hWnd			= NULL;
			colTransp		= RGB_TRANSPARENT;
		}
	}PREVIEW_PARAM;
	
	void SetParam(PREVIEW_PARAM * pParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual LPCTSTR RegKey() { return "Dlg Preview"; }
	
	CMutex          m_Mutex;
	PREVIEW_PARAM*	m_pParam;
	CSurface*		m_pSurface;
	UINT			m_nFrmWidth;
	UINT			m_nFrmHeight;
	UINT			m_nXOffset;
	UINT			m_nYOffset;
	BOOL			m_bFrmResize;
	UINT			m_nFrmWidthCur;
	UINT			m_nFrmHeightCur;
	BOOL			m_bResizeEnable;
	UINT			m_nResizeLeft;
	UINT			m_nResizeRight;
	UINT			m_nResizeTop;
	UINT			m_nResizeBottom;
	COLORREF		m_colTransp;

	// Generated message map functions
	//{{AFX_MSG(CPreviewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnShowLines();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowGrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

static UINT StartPreviewDlg(LPVOID pParam)
{
	if (pParam != NULL)
	{
		CPreviewDlg dlg;
		CPreviewDlg::PREVIEW_PARAM* pParamCur;
		
		pParamCur = (CPreviewDlg::PREVIEW_PARAM*)pParam;
		pParamCur->bExit = FALSE;

		dlg.SetParam(pParamCur);
		dlg.DoModal();
		
		pParamCur->bExit = TRUE;
	}
	
	return 0;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(PREVIEW_DLG_H__CIV3FLCEDIT__INCLUDED_)
