#if !defined(HYPERLINK_H__MFCAIDS__INCLUDED_)
#define HYPERLINK_H__MFCAIDS__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

// HyperLink.h : header file
/////////////////////////////////////////////////////////////////////////////
// CHyperLink class

class MFCAIDS_API CHyperLink : public CStatic
{
// Construction/destruction
public:
	CHyperLink();
	virtual ~CHyperLink();

// Attributes
public:
	BOOL m_bIsURL;

// Operations
public:
	HINSTANCE SendMailTo();
	static bool GotoURL( LPCTSTR szURL );

	void SetURL(CString sURL);
	CString GetURL()			{ return m_sURL;   }

	void SetColours(COLORREF crLinkColour, COLORREF crVisitedColour);
	COLORREF GetLinkColour()	{ return m_crLinkColour; }
	COLORREF GetVisitedColour()	{ return m_crVisitedColour; }

	void SetVisited(BOOL bVisited = TRUE);
	BOOL GetVisited()			{ return m_bVisited; }

	void SetLinkCursor(HCURSOR hCursor) { m_hLinkCursor = hCursor; }

	void SetUnderline(BOOL bUnderline = TRUE);
	BOOL GetUnderline()			{ return m_bUnderline; }

	void SetAutoSize(BOOL bAutoSize = TRUE);
	BOOL GetAutoSize()			{ return m_bAdjustToFit; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLink)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	static HINSTANCE GotoURL(LPCTSTR url, int showcmd);
	static void ReportError(int nError);
	static LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	void PositionWindow();

// Protected attributes
protected:
	COLORREF m_crLinkColour, m_crVisitedColour;		// Hyperlink colours
	BOOL     m_bVisited;							// Has it been visited?
	BOOL	 m_bUnderline;							// underline hyperlink?
	BOOL	 m_bAdjustToFit;						// Adjust window size to fit text?
	CString  m_sURL;								// hyperlink URL
	CFont	 m_Font;								// Underline font if necessary
	HCURSOR  m_hLinkCursor;							// Cursor for hyperlink
	CToolTipCtrl m_ToolTip;							// The tooltip

	// Generated message map functions
protected:
	//{{AFX_MSG(CHyperLink)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnClicked();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(HYPERLINK_H__MFCAIDS__INCLUDED_)
