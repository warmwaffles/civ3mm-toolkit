#if !defined(BUTTON_X_INCLUDED)
#define BUTTON_X_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonX.h : header file
//

// Comment this line out if you don't want XP theme support.
#include "XPTheme.h"
#include "MFCdefs.h"

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

// Comment this line out to disable support for sound effects.
#define	BX_USE_SOUND

// Comment this line out to disable using settings.
#define	BX_USE_SETTINGS

#ifdef BX_USE_SETTINGS
// This message means that some button settings was changed.
#define WM_BX_SETTINGCHANGE WM_USER + 777

#define BX_OFF_ICON         0x0001 // Hide icon
#define BX_OFF_OWNERDRAW    0x0002 // Not use DrawItem

#define BX_REG_KEY          "ButtonX"
#define BX_REG_SETTINGS     "Settings"
#endif // BX_USE_SETTINGS

enum	
{	
	BX_ALIGN_LEFT	= 0, // Icon on the left, text on the right
	BX_ALIGN_VERT,      // Icon on the top, text on the bottom
	BX_ALIGN_RIGHT,     // Icon on the right, text on the left
	BX_ALIGN_OVERLAP    // Icon on the same space as text
};

enum	
{	
	BX_COLOR_TXT_IN = 0,// Text color when mouse is INside
	BX_COLOR_TXT_OUT,   // Text color when mouse is OUTside
	BX_COLOR_TXT_FOCUS, // Text color when the button is focused
	BX_COLOR_MAX
};

/////////////////////////////////////////////////////////////////////////////
// CButtonX window

class MFCAIDS_API CButtonX : public CButton
{
// Construction
public:
	CButtonX();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonX)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetShowIconOnly(BOOL bShow = TRUE, BOOL bRepaint = TRUE);
	static void SetButtonsSettings(UINT nFlags = 0);
	BOOL SetIcon(HICON hIcon = NULL);
	BOOL SetIcon(UINT nIDResource);
	void Setup(int nIcon = NULL,
		  BYTE byAlign = BX_ALIGN_LEFT,
		  LPCTSTR lpszTooltip = NULL,
          int nCursor = NULL,
		  BOOL bDrawArrow = FALSE,
		  COLORREF crTxtIn = NULL, 
		  COLORREF crTxtOut = NULL, 
		  COLORREF crTxtFocus = NULL);

	void SetDrawArrow(BOOL bDraw = TRUE) { m_bDrawArrow = bDraw; }
	BOOL SetAlign(BYTE byAlign, BOOL bRepaint = TRUE);

	BOOL SetBtnCursor(int nCursorId = NULL, BOOL bRepaint = TRUE);
	void SetShowIcon(BOOL bShow = TRUE, BOOL bRepaint = TRUE);
	void SetShowText(BOOL bShow = TRUE, BOOL bRepaint = TRUE);
	void SetTextColorFocus(COLORREF crColor, BOOL bRepaint = TRUE);
	void SetTextColorOut(COLORREF crColor, BOOL bRepaint = TRUE);
	void SetTextColorIn(COLORREF crColor, BOOL bRepaint = TRUE);
	BOOL SetTextColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE);
	void SetTextColors(COLORREF crColorIn = NULL, COLORREF crColorOut = NULL, COLORREF crColorFocus = NULL, BOOL bRepaint = TRUE);
	void SetDefaultColors(BOOL bRepaint = TRUE);
	void SetTooltipText(int nText, BOOL bActivate = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);

	virtual ~CButtonX();

#ifdef BX_USE_SOUND
	void SetSound(LPCTSTR lpszSound, HMODULE hMod = NULL, BOOL bPlayOnClick = FALSE, BOOL bPlayAsync = TRUE);
#endif

#ifdef BX_USE_SETTINGS
	UINT GetSettings();
#endif

	// Generated message map functions
protected:
	virtual void DrawArrow(CDC* pDC, RECT* rpItem);
	virtual void DrawTheIcon(CDC* pDC, BOOL bHasCaption, RECT* rpItem, CRect* rpCaption);
	virtual void DrawTheText(CDC* pDC, RECT* rpItem, CRect* rpCaption);
	void PrepareImageRect(BOOL bHasCaption, RECT* rpItem, CRect* rpCaption, DWORD dwWidth, DWORD dwHeight, CRect* rpImage);
	void InitToolTip();
	void FreeResources(BOOL bCheckForNULL = TRUE);
	HICON CreateGrayscaleIcon(HICON hIcon);
	
	BOOL         m_bShowIcon;  // show or not the button's icon
	BOOL         m_bShowText;  // show or not the button's caption
	BOOL         m_bShowIconOnly;  // show the button's caption only if icon is absent
	BOOL         m_bMouseOver; // mouse over button
	BOOL         m_bDrawArrow; // draw arrow on the right side of the button
	BOOL         m_bIsFlat;    // flat button
	BOOL         m_bIsColorChanged; // is some of the text colors was changed
    BOOL         m_bIsPressed; // is button pressed
	BOOL         m_bIsDisabled;// is button disabled
	BOOL         m_bIsFocused; // is button focused
	BOOL         m_bIsDefault; // default button

	CString      m_sCaption;
	int          m_nCXedge;
	int          m_nCYedge;

	BYTE         m_byAlign;    // Align mode
	UINT         m_nFormat;    // text format

	CToolTipCtrl m_ToolTip;    // Tooltip
	HCURSOR		 m_hCursor;    // Handle to cursor

	CWnd*		 m_pParent;
	UINT		 m_nID;

#ifdef BX_USE_SETTINGS
	void SetSettings(UINT nFlags = 0);
	static UINT  m_nSettings;
#endif

	CSize        m_IconSize;
	CImageList   m_IconsList;
	BOOL         m_bIsIcon;
	COLORREF	 m_crColors[BX_COLOR_MAX]; // colors to be used

#ifdef BX_USE_SOUND
#pragma pack(1)
	typedef struct _STRUCT_SOUND
	{
		TCHAR    szSound[_MAX_PATH];
		LPCTSTR  lpszSound;
		HMODULE  hMod;
		DWORD    dwFlags;
	} STRUCT_SOUND;
#pragma pack()

	STRUCT_SOUND  m_csSounds[2]; // Index 0 = Over	1 = Clicked
#endif

#ifdef _THEME_H_
	BOOL InitTheme();
	CXPTheme*      m_xpButton;
#endif
	
	//{{AFX_MSG(CButtonX)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnClicked();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
#ifdef BX_USE_SETTINGS
	afx_msg LRESULT OnBxpSettingChange(WPARAM wParam, LPARAM lParam);
#endif

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(BUTTON_X_INCLUDED)
