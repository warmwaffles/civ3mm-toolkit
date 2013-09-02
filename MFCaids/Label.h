#if !defined(LABEL_H__MFCAIDS__INCLUDED_)
#define LABEL_H__MFCAIDS__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Label.h : header file
//

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////////////
// CLabel window
enum FlashType {None, Text, Background };

class MFCAIDS_API CLabel : public CStatic
{
// Construction
public:
	CLabel();
	CLabel& SetBkColor(COLORREF crBkgnd);
	CLabel& SetTextColor(COLORREF crText);
	CLabel& SetText(const CString& sText);
	CLabel& SetFontBold(BOOL bBold = TRUE);
	CLabel& SetFontName(const CString& sFont);
	CLabel& SetFontUnderline(BOOL bSet = TRUE);
	CLabel& SetFontItalic(BOOL bSet = TRUE);
	CLabel& SetFontSize(int nSize);
	CLabel& SetSunken(BOOL bSet = TRUE);
	CLabel& SetBorder(BOOL bSet = TRUE);
	CLabel& FlashText(BOOL bActivate = TRUE);
	CLabel& FlashBackground(BOOL bActivate = TRUE);
	CLabel& SetLink(BOOL bLink = TRUE);
	CLabel& SetLinkCursor(HCURSOR hCursor);

// Attributes
public:
protected:
	void ReconstructFont();
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	CString		m_sText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;
			// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLabel)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CLabelTitle window

typedef UINT (CALLBACK* LPFNDLLFUNC1)(HDC,CONST PTRIVERTEX,DWORD,CONST PVOID,DWORD,DWORD);

class MFCAIDS_API CLabelTitle : public CStatic
{
// Construction
public:
	CLabelTitle();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabelTitle)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetName();
	void SetName(CString sName);
	virtual ~CLabelTitle();

	// Generated message map functions
protected:
//	LPFNDLLFUNC1 m_dllfunc_GradientFill;
//	HINSTANCE m_hInst;
	CString m_sName;
	CFont m_font;
	COLORREF m_colText;
	COLORREF m_colName;
//	COLORREF m_colGrad1;
//	COLORREF m_colGrad2;
	//{{AFX_MSG(CLabelTitle)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(LABEL_H__MFCAIDS__INCLUDED_)
