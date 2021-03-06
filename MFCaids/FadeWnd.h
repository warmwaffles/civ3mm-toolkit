// File:		FadeWnd.h
// Ver:			0.00 - Nov 2003
//
// Copyright:	(C) 2003 John A. Johnson
// Author:		John A. Johnson (Andy)
// E-mail:		andyj@clois.it
// Homepage:	http://www.clois.com
//				http://www.yrrwo.com
//
// Implementation of a Fade effect from COLOR to BW helper function
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
////////////////////////////////////////////////////////////////////////////
//
// Release History - see .Cpp for credits to other developers
//
// . Nov 3 2003 - First Release
// . Nov 4 2003 - A flexible color calculation (added GreateGradient(...))
//                Some security options (OnLButtonCheck(), OnRButtonCheck())
//                Bug solved: focus to the parent, taskbar icon
//                Added region support to use the parent shape
//
////////////////////////////////////////////////////////////////////////////

#if !defined(FADEWND_H__MFCAIDS__INCLUDED_)
#define FADEWND_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////////////
// CFadeWnd window

#define WC_FADEWND _T("FadeWnd")

class MFCAIDS_API CFadeWnd : public CWnd
{
    // Construction
  public:
    CFadeWnd(CWnd* pParentWnd = NULL, BOOL bFade = TRUE, COLORREF clrLight = RGB(255, 255, 255), COLORREF clrDark = RGB(0, 0, 0));
    virtual ~CFadeWnd();

    // Attributes
  public:
    // Operations
  public:
    static void AutoFade(CWnd* pParentWnd, COLORREF clrLight = RGB(255, 255, 255), COLORREF clrDark = RGB(0, 0, 0));
    virtual void CreateGradient(COLORREF clrLight = RGB(255, 255, 255), COLORREF clrDark = RGB(0, 0, 0));

    // Virtuals to override window behaviour
  public:
    virtual BOOL OnLButtonCheck()
    {
        return TRUE;
    } // no behaviour
    virtual BOOL OnRButtonCheck()
    {
        return FALSE;
    } // no behaviour

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFadeWnd)
  public:
    virtual BOOL Create(CWnd* pParentWnd, BOOL bAutoDel = FALSE, CRect rc = CRect(0, 0, 0, 0));

  protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    void OnDraw(CDC* pDC);

    HBITMAP CopyScreenToBitmap(LPRECT lpRect);
    HBITMAP FadeBitmap(HBITMAP hBmp, double dfTrans = 100.0, HDC hBmpDC = NULL);

    // Attributes
  protected:
    CWnd m_wndInvisible;
    BOOL m_bAutoDel;
    HBITMAP m_hBitmap;
    HBITMAP m_hNewBitmap;
    COLORREF m_clrGradient[256];
    CWnd* m_pWndLock;
    BOOL m_bFade;

    // Generated message map functions
  protected:
    //{{AFX_MSG(CFadeWnd)
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FADEWND_H__MFCAIDS__INCLUDED_)
