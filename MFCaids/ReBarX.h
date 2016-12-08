#if !defined(REBAR_X_INCLUDED)
#define REBAR_X_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

#include "MFCdefs.h"
#include "ToolBarX.h"

#define BUTTONS_PER_ROW 4

////////////////////////////////////////////////////////////////////////////////////////
// class CReBarX
class CChevronPopup;
class MFCAIDS_API CReBarX : public CReBar
{
    CChevronPopup* m_pDropWindow;

  public:
    CReBarX();

    // This method is to add a band to the Rebar
    // Toolbars are preferred now. The iID has to be unique for the toolbars that are added
    BOOL AddBar(CWnd* pBar, LPCTSTR lpszText = NULL, CBitmap* pbmp = NULL, DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_USECHEVRON);
    BOOL AddBar(CWnd* pBar,
                COLORREF clrFore,
                COLORREF clrBack,
                LPCTSTR pszText = NULL,
                DWORD dwStyle   = RBBS_GRIPPERALWAYS | RBBS_USECHEVRON);

  public:
    //{{AFX_VIRTUAL(CReBarX)
    //}}AFX_VIRTUAL

  public:
    void SaveState();
    void RestoreState();
    virtual ~CReBarX();

  protected:
    static int m_nBarNumber; // bar number for restoration.
    static LPCTSTR m_lpszStateInfoFormat;
    CString m_strSubKey;    // registry subkey name.
    CString m_strValueName; // registry value.

    void UpdateBandInfo(CWnd* pBar, DWORD dwStyle);
    //{{AFX_MSG(CReBarX)
    //}}AFX_MSG

    //	Reflection handler for the Chevron push
    void OnChevronPushed(NMHDR* pNotifyStruct, LRESULT* result);
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// class CChevronPopup
class CToolBarX;
class CChevronPopup : public CWnd
{
  protected:
    CToolBarX* m_pToolBar;
    CToolBarCtrl m_tbCtrl;
    CImageList m_imgNormal;   // normal Imagelist for the toolbar
    CImageList m_imgHot;      // hot Imagelist for the toolbar
    CImageList m_imgDisabled; // disabled Imagelist for the toolbar
    HWND m_hMsgReceiver;      // this window receives the notifications for the toolbar!
    BOOL m_bTextLabels;       // is toolbar buttons have text or not

    // Helper to create a toolbar
    HWND CreateToolBar(HWND hwndParent, HWND hToolToReplicate);
    HWND CreateToolBarX(HWND hwndParent, CToolBarX* pToolBar);

    // Cleans all toolbar
    BOOL CleanToolBar();

    // Construction
  public:
    CChevronPopup();

    BOOL CreatePopup(CWnd* pParent);

    BOOL ShowPopup(CWnd* pMsgReceiver, CToolBar* pToolBar, CRect rectDisplayed, CPoint ptScreen);

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChevronPopup)
  protected:
    virtual void PostNcDestroy();
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CChevronPopup();

    // Generated message map functions
  protected:
    //{{AFX_MSG(CChevronPopup)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(REBAR_X_INCLUDED)
