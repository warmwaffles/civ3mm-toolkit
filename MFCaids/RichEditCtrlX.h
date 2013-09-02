#if !defined(RICHEDIT_CTRL_X_H__INCLUDED_)
#define RICHEDIT_CTRL_X_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RichEditCtrlX.h : header file
//

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlX window

class MFCAIDS_API CRichEditCtrlX : public CRichEditCtrl
{
// Construction
public:
	CRichEditCtrlX();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlX)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	long GetSelectionFontSize();
	CString GetSelectionFontName();
	CStringArray m_saFontList;
	void GetSystemFonts(CStringArray &saFontList);
	void SetFontSize(int nPointSize);
	void SetFontName(CString sFontName);
	void SelectColor();

	BOOL IsBulleted();
	void SetBulleted();
	
	PARAFORMAT GetParagraphFormat();
	
	BOOL IsRightAlign();
	BOOL IsLeftAlign();
	BOOL IsCenteredAlign();
	
	void SetRightAlign();
	void SetLeftAlign();
	void SetCenterAlign();
	
	CHARFORMAT GetCharFormat(DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK);
	
	BOOL IsBold();
	BOOL IsItalic();
	BOOL IsUnderlined();
	BOOL IsLink();

	void SetBold();
	void SetItalic();
	void SetUnderlined();
	void SetLink();

	void SetRTF(CString sRTF);
	CString GetRTF();
	virtual ~CRichEditCtrlX();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichEditCtrlX)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void SetCharStyle(int MASK, int STYLE, int nStart, int nEnd);
	static DWORD CALLBACK CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);
	static BOOL CALLBACK CBEnumFonts(LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(RICHEDIT_CTRL_X_H__INCLUDED_)
