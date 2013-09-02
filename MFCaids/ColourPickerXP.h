// CColourPickerXP & CColourPopupXP version 1.5
//
// Copyright © 2002-2003 Zorglab
//
// Feel free to remove or otherwise mangle any part.
// Please report any bug, comment, suggestion, etc. to the following address :
//   mailto:zorglab@wanadoo.be
//
// These classes are based on work by Chris Maunder, Alexander Bischofberger,
// James White and Descartes Systems Sciences, Inc.
//   http://www.codeproject.com/miscctrl/colour_picker.asp
//   http://www.codeproject.com/miscctrl/colorbutton.asp
//   http://www.codeproject.com/wtl/wtlcolorbutton.asp
//
// Thanks to Keith Rule for his CMemDC class (see MemDC.h).
// Thanks to Pål Kristian Tønder for his CXPTheme class, which is based on
// the CVisualStyleXP class of David Yuheng Zhao (see XPTheme.cpp).
//
// Many thanks to them all.
//
//                             === HISTORY ===
//
//	version 1.3		- parent window stays active when popup is up on screen
//					- when using track-selection, the initial colour is shown
//					  for an invalid selection instead of black
//					- added bTranslateDefault parameter in GetColor
//
//	version 1.2		- fixed : in release configuration, with neither
//					  'Automatic' nor 'Custom' labels, the pop-up won't work
//					- diasbled combo-box is drawn correctly
//					- combo-box height depends on text size
//					- font support : use SetFont() and GetFont(), for combo-
//					  box style call SetStyle() after changing font
//
//	version 1.1		- fixed some compile errors in VC6
//					- no need anymore to change the defines in stdafx.h
//					  except for multi-monitor support
//
//	version 1.0		first release
//
//                  === ORIGINAL COPYRIGHT STATEMENTS ===
//
// ------------------- Descartes Systems Sciences, Inc. --------------------
//
// Copyright (c) 2000-2002 - Descartes Systems Sciences, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are 
// met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer. 
// 2. Neither the name of Descartes Systems Sciences, Inc nor the names of 
//    its contributors may be used to endorse or promote products derived 
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------- Chris Maunder & Alexander Bischofberger ----------------
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// Updated 30 May 1998 to allow any number of colours, and to
//                     make the appearance closer to Office 97. 
//                     Also added "Default" text area.         (CJM)
//
//         13 June 1998 Fixed change of focus bug (CJM)
//         30 June 1998 Fixed bug caused by focus bug fix (D'oh!!)
//                      Solution suggested by Paul Wilkerson.
//
// ColourPopup is a helper class for the colour picker control
// CColourPicker. Check out the header file or the accompanying 
// HTML doc file for details.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
// -------------------------------------------------------------------------
//
//                   === END OF COPYRIGHT STATEMENTS ===
//

#ifndef COLOURPICKERXP_INCLUDED
#define COLOURPICKERXP_INCLUDED
#pragma once

// Comment this line out if you don't want XP theme support.
#include "XPTheme.h"

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

// CColourPopupXP messages
#define CPN_SELCHANGE        WM_USER + 1001        // Colour Picker Selection change
#define CPN_DROPDOWN         WM_USER + 1002        // Colour Picker drop down
#define CPN_CLOSEUP          WM_USER + 1003        // Colour Picker close up
#define CPN_SELENDOK         WM_USER + 1004        // Colour Picker end OK
#define CPN_SELENDCANCEL     WM_USER + 1005        // Colour Picker end (cancelled)

void AFXAPI DDX_ColourPickerXP(CDataExchange *pDX, int nIDC, COLORREF& crColour);

extern MFCAIDS_API BOOL ShowColorDialog(COLORREF &color, CWnd* pParentWnd = NULL);

class MFCAIDS_API CColourPickerXP : public CButton
{
public:
	DECLARE_DYNCREATE(CColourPickerXP);

	CColourPickerXP(void);
    virtual ~CColourPickerXP(void);

	// Property Accessors
	__declspec(property(get=GetColor,put=SetColor))						COLORREF	Color;
	__declspec(property(get=GetDefaultColor,put=SetDefaultColor))		COLORREF	DefaultColor;
	__declspec(property(get=GetTrackSelection,put=SetTrackSelection))	BOOL		TrackSelection;
	__declspec(property(put=SetCustomText))								LPCTSTR		CustomText;
	__declspec(property(put=SetDefaultText))							LPCTSTR		DefaultText;
	__declspec(property(put=SetColoursName))							UINT		ColoursName;
	__declspec(property(put=SetRegSection))								LPCTSTR		RegSection;
	__declspec(property(put=SetRegSectionStatic))						LPCTSTR		RegSectionStatic;
	__declspec(property(get=GetStyle,put=SetStyle))						BOOL		Style;
	__declspec(property(put=SetRGBText))								LPCTSTR		RGBText;
	__declspec(property(get=GetAlwaysRGB,put=SetAlwaysRGB))				BOOL		ShowRGBAlways;

	// Returns the current color selected in the control.
	virtual COLORREF GetColor(void) const;
	virtual COLORREF GetColor(BOOL bTranslateDefault) const;

	// Sets the current color selected in the control.
	virtual void SetColor(COLORREF Color);

    // Returns the color associated with the 'default' selection.
	virtual COLORREF GetDefaultColor(void) const;

	// Sets the color associated with the 'default' selection.
	virtual void SetDefaultColor(COLORREF Color);

	// Sets the text to display in the 'Custom' selection of the
	virtual void SetCustomText(LPCTSTR tszText);

	// Sets the text to display in the 'Default' selection of the
	// CColourPicker control, the default text is "Automatic". If
	// this value is set to "", the 'Default' selection will not
	// be shown.
	virtual void SetDefaultText(LPCTSTR tszText);

	// Sets the text from the resources of the tooltips to be
	// displayed when the pointer is on a colour.
	// Set this to 0 to use original English names.
	static void SetColoursName(UINT nFirstID = 0);

	// Sets the registry section where to write custom colours.
	// Set this to _T("") to disable.
	virtual void SetRegSection(LPCTSTR tszRegSection = _T(""));

	// Sets the registry section where to write custom colours.
	// Set this to _T("") to disable. This will be applied in
	// all CColourPickerXPs of the application.
	static void SetRegSectionStatic(LPCTSTR tszRegSection = _T(""));

	// Turns on/off the 'Track Selection' option of the control
	// which shows the colors during the process of selection.
	virtual void SetTrackSelection(BOOL bTrack);

	// Returns the state of the 'Track Selection' option.
	virtual BOOL GetTrackSelection(void) const;

	// Sets the style of control to show.
	virtual void SetStyle(BOOL bComboBoxStyle);

	// Description:	Returns TRUE if the style is set on ComboBox and FALSE if
	//				it is set on Button.
	// Parameters:	void
	// Return:		BOOL 
	virtual BOOL GetStyle(void) const;

	// Sets the 3 letters used to display the RGB-value.
	virtual void SetRGBText(LPCTSTR tszRGB = _T("RGB"));

	// If this is set to TRUE the RGB-value of the colour will
	// be shown even if the colour is a base colour.
	virtual void SetAlwaysRGB(BOOL bShow);

	// Returns TRUE if the RGB-value is always shown.
	virtual BOOL GetAlwaysRGB(void) const;

	//{{AFX_VIRTUAL(CColourPickerXP)
	public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
    virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	// DrawArrow
	void DrawArrow(CDC* pDC, RECT* rpItem, BOOL bIsDisabled);
	virtual void DrawHotArrow(BOOL bHot);

	//{{AFX_MSG(CColourPickerXP)
    afx_msg BOOL OnClicked();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	afx_msg void OnNMThemeChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	COLORREF m_Color;
	COLORREF m_DefaultColor;
	CString m_strDefaultText;
	CString m_strCustomText;
	BOOL	m_bPopupActive;
	BOOL	m_bTrackSelection;
	BOOL	m_bMouseOver;
	BOOL	m_bFlatMenus;

	BOOL	m_bComboBoxStyle;
	BOOL	m_bAlwaysRGB;
	CString	m_strRGBText;

	CString	m_strRegSection;
	static CString m_strRegSectionStatic;

#ifdef _THEME_H_
	CXPTheme m_xpButton, m_xpEdit, m_xpCombo;
#endif

private:

	typedef CButton _Inherited;
};



/////////////////////////////////////////////////////////////////////////////
// CColourPopupXP class definition
/////////////////////////////////////////////////////////////////////////////

// To hold the colours and their names
typedef struct {
    COLORREF crColour;
	TCHAR    szName[256];
} ColourTableEntry;

/////////////////////////////////////////////////////////////////////////////
// CColourPopupXP window

class MFCAIDS_API CColourPopupXP : public CWnd
{
public:
	DECLARE_DYNCREATE(CColourPopupXP);
// Construction
public:
    CColourPopupXP();
    CColourPopupXP(CPoint p, COLORREF crColour, CWnd* pParentWnd,
                 LPCTSTR szDefaultText = NULL, 
				 LPCTSTR szCustomText = NULL,
				 UINT nID = 0,
				 BOOL bAutoDestroy = TRUE,
				 LPCTSTR szRegSection = NULL);
    void Initialise();

// Attributes
public:

// Operations
public:
    BOOL Create(CPoint p, COLORREF crColour, 
		        CWnd* pParentWnd, 
                LPCTSTR szDefaultText = NULL,
				LPCTSTR szCustomText = NULL,
				UINT nID = 0,
				BOOL bAutoDestroy = TRUE,
				LPCTSTR szRegSection = NULL);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColourPopupXP)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CColourPopupXP();

protected:
    BOOL GetCellRect(int nIndex, const LPRECT& rect);
    void FindCellFromColour(COLORREF crColour);
    void SetWindowSize();
    void CreateToolTips();
    void ChangeSelection(int nIndex);
    void EndSelection(int nMessage);
    void DrawCell(CDC* pDC, int nIndex);

    int  GetIndex(int row, int col) const;
    int  GetRow(int nIndex) const;
    int  GetColumn(int nIndex) const;

	void DrawBorder(CDC* pDC, CRect rect, UINT nEdge, UINT nBorder);

// public attributes
public:
    static ColourTableEntry m_crColours[];
	static TCHAR m_strInitNames[][256];
    static int m_nNumColours;

    static COLORREF GetColour(int nIndex)              { return m_crColours[nIndex].crColour; }
    static LPCTSTR GetColourName(int nIndex)           { return m_crColours[nIndex].szName; }

	static BOOL    m_bMenuXpStyle;

// protected attributes
protected:
    int            m_nNumColumns, m_nNumRows;
    int            m_nBoxSize, m_nMargin;
    int            m_nCurrentSel;
    int            m_nChosenColourSel;
    CString        m_strDefaultText;
    CString        m_strCustomText;
    CRect          m_CustomTextRect, m_DefaultTextRect, m_WindowRect, m_BoxesRect;
    CFont          m_Font;
    CPalette       m_Palette;
    COLORREF       m_crInitialColour, m_crColour;
    CToolTipCtrl   m_ToolTip;
    CWnd*          m_pParent;
	UINT           m_nID;
	BOOL           m_bAutoDestroy;
    BOOL           m_bChildWindowVisible;
	BOOL		   m_bIsXP, m_bFlatmenus;

	COLORREF	   m_clrBackground, 
				   m_clrHiLightBorder, 
				   m_clrHiLight, 
				   m_clrHiLightText, 
				   m_clrText, 
				   m_clrLoLight;

	CString        m_strRegSection;


    // Generated message map functions
protected:
    //{{AFX_MSG(CColourPopupXP)
    afx_msg void OnNcDestroy();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
#if _MFC_VER >= 0x0700
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#endif
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif //!COLOURPICKERXP_INCLUDED
