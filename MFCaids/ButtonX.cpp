/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ButtonX.cpp
	CREATED:	April 03.2003
	AUTHOR:		Dreyk
	VERSION:	1.3
	MODIFIED:	08.06.2004

	DESCRIPTION: A fully featured owner-draw button class.
	CButtonX is a class derived from the MFC CButton class. 
	With this class your applications can have standard buttons or 
	new and modern theme-aware buttons.
	
	This class is based on work by CColourPickerXP, CXPTheme, CMemDC and CButtonST classes.

	Thanks to Zorglab (mailto:zorglab@wanadoo.be) for his CColourPickerXP class.
    Thanks to Davide Calabro' (mailto:davide_calabro@yahoo.com) for his CButtonST class.
    Thanks to Pål Kristian Tønder for his CXPTheme class, which is based on
    the CVisualStyleXP class of David Yuheng Zhao (see XPTheme.cpp).
    Thanks to Keith Rule for his CMemDC class (see MemDC.h).
	Thanks to Chris Maunder, Alexander Bischofberger, James White 
	and Descartes Systems Sciences, Inc.
	Many thanks to them all.

	-------------------------------------------------------------------------
	    Instructions on how to add CButtonX to your application:
	-------------------------------------------------------------------------
	
	1. Copy ButtonX.h, ButtonX.cpp, XPTheme.h, XPTheme.cpp and MemDC.h 
	into your application directory.
	  
	2. Add the five files into your project.
	  
	3. If you don't want XP theme support, comment out the '#include "XPTheme.h"'
	in the ButtonX.h file.
			
	4. Add a button to the dialog in question using the resource editor.
	You don't have to make and style adjustments to the button.
			  
	5. Add a variable for that button.
				
	6. Add '#include "ButtonX.h"' in the dialog's header file and change the 
	definition of the button from 'CButton ...' to 'CButtonX ...'.
				  
	7. Inside your OnInitDialog for the dialog, you can change the button style:
	   - This function sets the alignment type between icon and text:
	      BOOL SetAlign(BYTE byAlign, BOOL bRepaint);
	   - This function sets the cursor to be used when the mouse is over the button:
	      BOOL SetBtnCursor(int nCursorId, BOOL bRepaint);
       - This functions sets the text colors to use for a particular state:
	      void SetTextColors(COLORREF crColorIn, COLORREF crColorOut, COLORREF crColorFocus, BOOL bRepaint);
          BOOL SetTextColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint);
          void SetTextColorIn(COLORREF crColor, BOOL bRepaint);
		  void SetTextColorOut(COLORREF crColor, BOOL bRepaint);
		  void SetTextColorFocus(COLORREF crColor, BOOL bRepaint);
	   - This function sets arrow on right side of the button:
	      void SetDrawArrow(BOOL bDraw);
	   - This functions sets the text to show in the button tooltip:
	      void SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
          void SetTooltipText(int nText, BOOL bActivate);
	   - This functions assigns icons to the button:
	      BOOL SetIcon(UINT nIDResource);
          BOOL SetIcon(HICON hIcon);
	   - This function sets the sound that must be played on particular button states:
	      void SetSound(LPCTSTR lpszSound, HMODULE hMod, BOOL bPlayOnClick, BOOL bPlayAsync);
       - This function allow to show or not the button's caption:
	      void SetShowText(BOOL bShow, BOOL bRepaint);

	8. Compile and enjoy.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "ButtonX.h"

#include "MemDC.h"

#include "Utilities.h"

#ifdef _THEME_H_
#include <atlconv.h>
#endif

#ifdef BX_USE_SOUND
#pragma comment(lib, "winmm.lib")
#include <Mmsystem.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Windows 98/Me, Windows 2000/XP: The item is being hot-tracked, 
// that is, the item will be highlighted when the mouse is on the item
#ifndef ODS_HOTLIGHT
#define ODS_HOTLIGHT    0x0040
#endif

// Windows 2000/XP: The control is drawn without the 
// keyboard accelerator cues
#ifndef ODS_NOACCEL
#define ODS_NOACCEL     0x0100
#endif

// Windows 2000/XP: The control is drawn without focus indicator cues
#ifndef ODS_NOFOCUSRECT
#define ODS_NOFOCUSRECT 0x0200
#endif

// Constants
#define gArrowSizeX     4
#define gArrowSizeY     2
#define gImageOrgX      3
#define gImageOrgY      4
#define gPressedOffsetX 1
#define gPressedOffsetY 0

#ifdef _THEME_H_
static	CXPTheme m_XpButton;
#endif

// color of disabled elements: text and arrow
static COLORREF m_crDisabled;

UINT CButtonX::m_nSettings = 0;

/////////////////////////////////////////////////////////////////////////////
// CButtonX

CButtonX::CButtonX()
{
#ifdef _THEME_H_
	m_xpButton			= &m_XpButton;
#endif

	m_bShowIcon			= TRUE;
	m_bShowText			= TRUE;
	m_bShowIconOnly		= FALSE;
	m_bMouseOver		= FALSE;
	m_bDrawArrow		= FALSE;
	m_bIsFlat			= FALSE;
	m_bIsDefault		= FALSE;
	m_bIsColorChanged	= FALSE;
	m_bIsPressed		= FALSE;
	m_bIsDisabled		= FALSE;
	m_bIsFocused		= FALSE;
	m_IconSize.cx		= 16;
	m_IconSize.cy		= 16;
	m_bIsIcon			= FALSE;
	
	m_nFormat			= 0;

	FreeResources(FALSE);
	
	// Set default colors
	SetDefaultColors(FALSE);
	
	// By default icon is aligned horizontally
	m_byAlign = BX_ALIGN_LEFT;

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;

	// No cursor defined
	m_hCursor = NULL;

#ifdef BX_USE_SOUND
	// No defined sounds
	::ZeroMemory(&m_csSounds, sizeof(m_csSounds));
#endif
}

CButtonX::~CButtonX()
{
	FreeResources();

	// Destroy the cursor (if any)
	if (m_hCursor) 
		::DestroyCursor(m_hCursor);
}


BEGIN_MESSAGE_MAP(CButtonX, CButton)
	//{{AFX_MSG_MAP(CButtonX)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
#ifdef BX_USE_SETTINGS
	ON_MESSAGE(WM_BX_SETTINGCHANGE, OnBxpSettingChange)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonX message handlers

/////////////////////////////////////////////////////////////////////////////
// Init theme.
//
#ifdef _THEME_H_
BOOL CButtonX::InitTheme()
{
	// Open theme (theme will be closed before opening in case if it is already opend)
	BOOL bRes = m_XpButton.Open(GetSafeHwnd(), L"BUTTON");

	// Get RGB value of the disabled text color
	if(m_XpButton.IsAppThemed())
		m_XpButton.GetColor(BP_PUSHBUTTON, PBS_DISABLED, TMT_TEXTCOLOR, &m_crDisabled);
	else
		m_crDisabled = ::GetSysColor(COLOR_GRAYTEXT);

	return bRes;
}
#endif

void CButtonX::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC*    pDC   = CDC::FromHandle(lpDIS->hDC);
	CMemDC  dcMem(pDC);
	CRect   rDraw = lpDIS->rcItem;
	UINT    state = lpDIS->itemState;
	CRect   rect;

	// button state
	m_bIsPressed  = (state & ODS_SELECTED);
	m_bIsDisabled = (state & ODS_DISABLED);
	m_bIsFocused  = (state & ODS_FOCUS);

	m_nCXedge = ::GetSystemMetrics(SM_CXEDGE);
	m_nCYedge = ::GetSystemMetrics(SM_CYEDGE);

#ifdef _THEME_H_
	if(!m_XpButton)
		InitTheme();

	// Draw Outer Edge
	if (m_xpButton->IsAppThemed())
	{
		int nFrameState = (m_bIsPressed) ? PBS_PRESSED : 0;

		if (m_bIsDisabled)
			nFrameState |= PBS_DISABLED;

		if ((state & ODS_HOTLIGHT) || m_bMouseOver)
			nFrameState |= PBS_HOT;
		else 
		if (m_bIsFocused)// || (state & ODS_DEFAULT))
			nFrameState |= PBS_DEFAULTED;
		
		m_xpButton->DrawBackground(dcMem.GetSafeHdc(), BP_PUSHBUTTON, nFrameState, &rDraw, NULL);
		m_xpButton->GetBackgroundContentRect(dcMem.GetSafeHdc(), BP_PUSHBUTTON, nFrameState, &rDraw, &rDraw);
		rDraw.InflateRect(0, 0, 1, 0);
	}
	else
#endif
	{
		UINT uFrameState = DFCS_BUTTONPUSH | DFCS_ADJUSTRECT;
		
		if (m_bIsFlat)
			uFrameState |= DFCS_FLAT;

		if (m_bIsPressed)
			uFrameState |= DFCS_PUSHED;
		
		if (m_bIsDisabled)
			uFrameState |= DFCS_INACTIVE;

		if (m_bIsFocused)// || (state & ODS_DEFAULT))
		{
			CBrush br(RGB(0,0,0));  
			dcMem.FrameRect(&rDraw, &br);
			rDraw.DeflateRect(1, 1);
			
			if (m_bIsPressed)
			{
				br.DeleteObject();
				br.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
				dcMem.FrameRect(&rDraw, &br);
				rDraw.DeflateRect(m_nCXedge, m_nCYedge);
			}
			else
			{
				dcMem.DrawFrameControl(&rDraw,
					DFC_BUTTON,
					uFrameState);
			}
			
			rDraw.InflateRect(1, 1);
		}
		else
		{
			dcMem.DrawFrameControl(&rDraw,
				DFC_BUTTON,
				uFrameState);
		}

		if (m_bIsPressed)
			rDraw.OffsetRect(1,1);
	}

	// Draw Focus
	// Windows 2000 and higher includes new flags for existing flag sets. 
	// The ODS_NOFOCUSRECT flags must be checked along with the 
	// ODS_FOCUS flag to make sure that the control is drawn correctly
	if (m_bIsFocused && !(state & ODS_NOFOCUSRECT)) 
    {
		rect = lpDIS->rcItem;
		rect.DeflateRect(3, 3);
        dcMem.DrawFocusRect(&rect);
    }

#ifdef _THEME_H_
	if (m_xpButton->IsAppThemed())
		rDraw.DeflateRect(0, 0, 1, 0);
	else
#endif	
	{
		rDraw.DeflateRect(m_nCXedge, m_nCYedge);
		rDraw.InflateRect(0, 1, 0, 0);
	}

	// Draw the arrow
	if(m_bDrawArrow)
		DrawArrow(&dcMem, &rDraw);

	rect.CopyRect(rDraw);

	// Get the button's caption
	if (m_bShowText)
	{
		GetWindowText(m_sCaption);
		
		// The caption is drawn without the keyboard accelerator cues
		if (state & ODS_NOACCEL)
			m_sCaption.Replace("&", "");
	}
	else // If we don't want the caption displayed
		m_sCaption.Empty();

	if (m_bIsIcon && m_bShowIcon)
	{
		DrawTheIcon(&dcMem, !m_sCaption.IsEmpty(), &rDraw, &rect);
		
		if (!m_sCaption.IsEmpty() && !m_bShowIconOnly)
			DrawTheText(&dcMem, &rDraw, &rect);
	}
	else
	{
		if (!m_sCaption.IsEmpty())
			DrawTheText(&dcMem, &rDraw, &rect);
	}
/*
// OLD VARIANT:

	// Draw the icon
	if (m_bIsIcon && m_bShowIcon)
		DrawTheIcon(&dcMem, !m_sCaption.IsEmpty(), &rDraw, &rect);

	// Draw the button caption (if any)
	if (!m_sCaption.IsEmpty() && !m_bShowIconOnly)
		DrawTheText(&dcMem, &rDraw, &rect);
*/
}

void CButtonX::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bMouseOver)
	{
		m_bMouseOver = TRUE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);

		Invalidate(FALSE);

#ifdef BX_USE_SOUND
			// Play sound ?
			if (m_csSounds[0].lpszSound)
				::PlaySound(m_csSounds[0].lpszSound, m_csSounds[0].hMod, m_csSounds[0].dwFlags);
#endif
	}

	CButton::OnMouseMove(nFlags, point);
}

void CButtonX::PreSubclassWindow() 
{
	// Check if this is the default button
	UINT nStyle;
	
	nStyle = GetButtonStyle();
	
	// Check if this is the default button
	if (nStyle & BS_DEFPUSHBUTTON)
		m_bIsDefault = TRUE;
	
	// Add BS_OWNERDRAW style
	SetButtonStyle(nStyle | BS_OWNERDRAW);
	
	nStyle = GetStyle();
	
	// Get horizontal alignment
	if (nStyle & BS_LEFT)
		m_nFormat |= DT_LEFT;
	else if (nStyle & BS_RIGHT)
		m_nFormat |= DT_RIGHT;
	else // BS_CENTER
		m_nFormat |= DT_CENTER;
	
	// Get vertical alignment
	if (nStyle & BS_TOP)
		m_nFormat |= DT_TOP;
	else if (nStyle & BS_BOTTOM)
		m_nFormat |= DT_BOTTOM;
	else // BS_VCENTER
		m_nFormat |= DT_VCENTER;
	
	// Multiline or singleline
	if (nStyle & BS_MULTILINE)
		m_nFormat |= DT_WORDBREAK;
	else
		m_nFormat |= DT_SINGLELINE;
	
	// Is it flat button?
	if (nStyle & BS_FLAT)
		m_bIsFlat = TRUE;
	
#ifdef BX_USE_SETTINGS
	SetSettings(m_nSettings);
#endif
	
#ifdef _THEME_H_
	if(!m_XpButton)
		InitTheme();
#endif
	
	m_pParent	= GetParent();
	m_nID		= GetDlgCtrlID();

	CButton::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// Draw Arrow
//
void CButtonX::DrawArrow(CDC* pDC, RECT* rpItem)
{
	CRect rect;
#ifdef _THEME_H_
	if (m_xpButton->IsAppThemed())
		rect.left = rpItem->right - gArrowSizeX - m_nCXedge - 1;
	else
#endif	
	{
		rect.left = rpItem->right - gArrowSizeX - m_nCXedge / 2;
	}
	rect.right  = rect.left + gArrowSizeX;
	rect.top	= (rpItem->bottom + rpItem->top) / 2 - gArrowSizeY / 2;
	rect.bottom = (rpItem->bottom + rpItem->top) / 2 + gArrowSizeY / 2;

	POINT ptsArrow[3];

	ptsArrow[0].x = rect.left;
	ptsArrow[0].y = rect.top;
	ptsArrow[1].x = rect.right;
	ptsArrow[1].y = rect.top;
	ptsArrow[2].x = (rect.left + rect.right)/2;
	ptsArrow[2].y = rect.bottom;
	
	COLORREF cr = m_bIsDisabled ? m_crDisabled : RGB(0,0,0);
	CBrush brush(cr);
	CPen pen(PS_SOLID, 1 , cr);

	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen*   pOldPen   = pDC->SelectObject(&pen);

	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(ptsArrow, 3);

	rpItem->right = rect.left - m_nCXedge/2 - 2;

	// Draw Separator
#ifdef _THEME_H_
	// use the code below only if we have enabled theme
	if (m_xpButton->IsAppThemed())
	{
		if (!m_bIsDisabled)
		{
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1 , ::GetSysColor(COLOR_BTNSHADOW));
			pDC->SelectObject(&pen);
		}
		pDC->MoveTo(rpItem->right-1, rpItem->top);
		pDC->LineTo(rpItem->right-1, rpItem->bottom);
		
		if (!m_bIsDisabled)
		{
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1 , ::GetSysColor(COLOR_3DLIGHT));
			pDC->SelectObject(&pen);
			pDC->MoveTo(rpItem->right-2, rpItem->top);
			pDC->LineTo(rpItem->right-2, rpItem->bottom);
		}
	}
	else // otherwise draw the right side of border rectangle
#endif	
	{
		pDC->DrawEdge(rpItem, EDGE_ETCHED, BF_RIGHT | BF_FLAT);
	}

	rpItem->right -= (m_nCXedge * 2) + 1 ;

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the text to show in the button tooltip.
//
// Parameters:
//		[IN]	nText
//				ID number of the string resource containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CButtonX::SetTooltipText(int nText, BOOL bActivate)
{
	CString sText;

	// Load string resource
	sText.LoadString(nText);

	// If string resource is not empty
	if (!sText.IsEmpty()) 
		SetTooltipText((LPCTSTR)sText, bActivate);
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the text to show in the button tooltip.
//
// Parameters:
//		[IN]	lpszText
//				Pointer to a null-terminated string containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CButtonX::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL)
		return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
}

/////////////////////////////////////////////////////////////////////////////
// This function enables or disables the button tooltip.
//
void CButtonX::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) 
		return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
}

void CButtonX::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this);
		m_ToolTip.Activate(FALSE);

		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	}
}

BOOL CButtonX::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
		CDialog *pDialog = (CDialog *)GetOwner();
		if (pDialog && ::IsWindow(pDialog->m_hWnd))
		{
			OnClicked();
			pDialog->PostMessage(WM_COMMAND, m_nID);
		}
		return TRUE;
	}
	
	return CButton::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the alignment type between icon and text.
//
// Parameters:
//		[IN]	byAlign
//				Alignment type. Can be one of the following values:
//				BX_ALIGN_LEFT	  Icon on the left, text on the right
//				BX_ALIGN_VERT	  Icon on the top, text on the bottom
//				BX_ALIGN_RIGHT	  Icon on the right, text on the left
//				BX_ALIGN_OVERLAP Icon on the same space as text
//				By default, button have BX_ALIGN_LEFT alignment.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
BOOL CButtonX::SetAlign(BYTE byAlign, BOOL bRepaint)
{
	switch (byAlign)
	{    
		case BX_ALIGN_LEFT:
		case BX_ALIGN_RIGHT:
		case BX_ALIGN_VERT:
		case BX_ALIGN_OVERLAP:
			m_byAlign = byAlign;
			if (bRepaint)	
				Invalidate();
			return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//		[IN]	bHasCaption
//				TRUE if the button has a text
//		[IN]	rpItem
//				A pointer to a RECT structure indicating the allowed paint area
//		[IN/OUT]rpCaption
//				A pointer to a CRect object indicating the paint area reserved for the
//				text. This structure will be modified if necessary.
//		[IN]	dwWidth
//				Width of the image
//		[IN]	dwHeight
//				Height of the image
//		[OUT]	rpImage
//				A pointer to a CRect object that will receive the area available to the image
//
void CButtonX::PrepareImageRect(BOOL bHasCaption, RECT* rpItem, CRect* rpCaption, DWORD dwWidth, DWORD dwHeight, CRect* rpImage)
{
	rpImage->CopyRect(rpItem);

	switch (m_byAlign)
	{
		case BX_ALIGN_LEFT:
			if (bHasCaption == FALSE)
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpImage->left += gImageOrgX;  
				rpCaption->left += dwWidth + gImageOrgX;
			}
			// Center image vertically
			rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			break;

		case BX_ALIGN_RIGHT:
			if (bHasCaption == FALSE)
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpCaption->right = rpCaption->Width() - dwWidth;
				rpImage->left = rpItem->right - dwWidth;
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			break;
		
		case BX_ALIGN_VERT:
			// Center image horizontally
			rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			if (bHasCaption == FALSE)
			{
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);           
			}
			else
			{
				rpImage->top = gImageOrgY + 1;
				rpCaption->top += dwHeight;
#ifdef _THEME_H_
				if (!m_xpButton->IsAppThemed() && m_bIsPressed)
					rpImage->OffsetRect(0, 1);
#else
				if (m_bIsPressed)
					rpImage->OffsetRect(0, 1);
#endif  
			}
			break;

		case BX_ALIGN_OVERLAP:
			break;
	} // switch

#ifdef _THEME_H_
#else
	// If button is pressed then press image also
	if (m_bIsPressed)
		rpImage->OffsetRect(gPressedOffsetX, gPressedOffsetY);
#endif   
}

void CButtonX::DrawTheIcon(CDC* pDC, BOOL bHasCaption, RECT* rpItem, CRect* rpCaption)
{
	CRect rImage;
	PrepareImageRect(bHasCaption, rpItem, rpCaption, m_IconSize.cx, m_IconSize.cy, &rImage);

	int nImage = 1;

	if (m_bIsFocused || m_bMouseOver)
		nImage = 0;
	else if (m_bIsDisabled)
		nImage = 2;

	m_IconsList.Draw(pDC, nImage, rImage.TopLeft(), ILD_TRANSPARENT);
}

/////////////////////////////////////////////////////////////////////////////
// This function assigns icons to the button.
// Any previous icon will be removed.
//
// Parameters:
//		[IN]	nIDResource
//				ID number of the icon resource to show when the mouse is over the button.
//				Pass NULL to remove any icon from the button.
//
BOOL CButtonX::SetIcon(UINT nIDResource)
{
	if (!nIDResource)
	{
		// Free any loaded resource
		FreeResources();
		return FALSE;
	}
		
	HICON		hIcon		  = NULL;
	HINSTANCE	hInstResource = NULL;

	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_GROUP_ICON);
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIDResource), IMAGE_ICON, 0, 0, 0);

	return SetIcon(hIcon);
}

/////////////////////////////////////////////////////////////////////////////
// This function assigns icons to the button.
// Any previous icon be removed.
//
// Parameters:
//		[IN]	hIcon
//				Pass NULL to remove any icon from the button.
//
BOOL CButtonX::SetIcon(HICON hIcon /*=NULL*/)
{
	// Free any loaded resource
	FreeResources();

	ICONINFO iconInfo;
	ZeroMemory(&iconInfo, sizeof(iconInfo));
	if (!GetIconInfo(hIcon, &iconInfo))
	{
		FreeResources();
		return FALSE;
	}

	m_IconSize.cx = (DWORD)(iconInfo.xHotspot * 2);
	m_IconSize.cy = (DWORD)(iconInfo.yHotspot * 2);
	m_IconsList.Create(m_IconSize.cx, m_IconSize.cy, ILC_COLORDDB | ILC_MASK, 0, 0);
	m_IconsList.Add(hIcon);
	
	CDC myDC;
	myDC.CreateCompatibleDC(0);
	
	CBitmap bmColor;
	bmColor.Attach(iconInfo.hbmColor);
	CBitmap bmMask;
	bmMask.Attach(iconInfo.hbmMask);
	
	CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
	COLORREF crPixel;
	for (int i = 0; i < m_IconSize.cx; ++i)
	{
		for (int j = 0; j < m_IconSize.cy; ++j)
		{
			crPixel = myDC.GetPixel(i, j);
			myDC.SetPixel(i, j, DarkenColor(SCALE_DARK, crPixel));
		}
	}
	myDC.SelectObject(pOldBitmap);
	m_IconsList.Add(&bmColor, &bmMask);
	bmColor.DeleteObject();
	bmMask.DeleteObject();
	
	///////////////////////////////////////////
	GetIconInfo(hIcon, &iconInfo);
	bmColor.Attach(iconInfo.hbmColor);
	bmMask.Attach(iconInfo.hbmMask);
	pOldBitmap = myDC.SelectObject(&bmColor);

	COLORREF crTo = ::GetSysColor(COLOR_BTNFACE);
	for (i = 0; i < m_IconSize.cx; ++i)
	{
		for (int j = 0; j < m_IconSize.cy; ++j)
		{
			crPixel = myDC.GetPixel(i, j);     
			myDC.SetPixel(i, j, MixedColor(LightenColor(SCALE_LIGHT, GrayColor(crPixel)), crTo));      
		}
	}
	myDC.SelectObject(pOldBitmap);
	m_IconsList.Add(&bmColor, &bmMask);
	///////////////////////////////////////////
	
	m_bIsIcon = TRUE;
	if (GetSafeHwnd())
		Invalidate();
	return TRUE;
}

void CButtonX::FreeResources(BOOL bCheckForNULL /*=TRUE*/)
{
	m_IconsList.DeleteImageList();
	m_bIsIcon = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// This function creates a grayscale icon starting from a given icon.
// The resulting icon will have the same size of the original one.
//
// Parameters:
//		[IN]	hIcon
//				Handle to the original icon.
//
// Return value:
//		If the function succeeds, the return value is the handle to the newly created
//		grayscale icon.
//		If the function fails, the return value is NULL.
//
HICON CButtonX::CreateGrayscaleIcon(HICON hIcon)
{
	HICON		hGrayIcon = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	BITMAP		bmp;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	ICONINFO	csII, csGrayII;
	BOOL		bRetValue = ::GetIconInfo(hIcon, &csII);
	
	if (!bRetValue)	
		return NULL;

	hMainDC = ::GetDC(NULL);
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	
	if (hMainDC == NULL || hMemDC1 == NULL || hMemDC2 == NULL)	
		return NULL;
  
	if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
	{
		DWORD	dwWidth  = csII.xHotspot*2;
		DWORD	dwHeight = csII.yHotspot*2;

		csGrayII.hbmColor = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
		if (csGrayII.hbmColor)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);

			::BitBlt(hMemDC2, 0, 0, dwWidth, dwHeight, hMemDC1, 0, 0, SRCCOPY);

			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;
			BYTE		byNewPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC1, dwLoopX, dwLoopY);

					byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));
					if (crPixel)	
						::SetPixel(hMemDC2, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
				}
			}

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);

			csGrayII.hbmMask = csII.hbmMask;

			csGrayII.fIcon = TRUE;
			hGrayIcon = ::CreateIconIndirect(&csGrayII);
		}

		::DeleteObject(csGrayII.hbmColor);
		//::DeleteObject(csGrayII.hbmMask);
	}

	::DeleteObject(csII.hbmColor);
	::DeleteObject(csII.hbmMask);
	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(NULL, hMainDC);

	return hGrayIcon;
}

/////////////////////////////////////////////////////////////////////////////
// Draw the button's caption
//
void CButtonX::DrawTheText(CDC* pDC, RECT* rpItem, CRect* rpCaption)
{
	CFont *pOldFont = pDC->SelectObject(GetFont());
	pDC->SetBkMode(TRANSPARENT);

// decomment '#include <atlconv.h>' to use it!
#ifdef _THEME_H_
	if (m_xpButton->IsAppThemed() && 
	   (!m_bIsColorChanged || m_bIsDisabled))
	{
		// use it to call OLE here
		USES_CONVERSION;
		
		// (LPCSTR) -> (LPCWSTR)
		LPCWSTR lpCaption = A2CW(m_sCaption);

		m_xpButton->DrawText(pDC->GetSafeHdc(), 
			BP_PUSHBUTTON, 
			m_bIsDisabled ? PBS_DISABLED : 0, 
			lpCaption,
			-1, 
			m_nFormat, 
			0, 
			rpCaption);
	}
	else	
#endif // _THEME_H_
	{
#ifdef _THEME_H_
#else
		// If button is pressed then "press" caption also
		if (m_bIsPressed)
			rpCaption->OffsetRect(gPressedOffsetX, gPressedOffsetY);
#endif

		if (m_bIsDisabled)
		{
			rpCaption->OffsetRect(1, 1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
			pDC->DrawText(m_sCaption, -1, rpCaption, m_nFormat);
			
			rpCaption->OffsetRect(-1, -1);
			pDC->SetTextColor(m_crDisabled);
			pDC->DrawText(m_sCaption, -1, rpCaption, m_nFormat);
		}
		else
		{
			if (m_bMouseOver || m_bIsPressed)
				pDC->SetTextColor(m_crColors[BX_COLOR_TXT_IN]);
			else 
			{
				if (m_bIsFocused)
					pDC->SetTextColor(m_crColors[BX_COLOR_TXT_FOCUS]); 
				else
					pDC->SetTextColor(m_crColors[BX_COLOR_TXT_OUT]);
			}
			
			pDC->DrawText(m_sCaption, -1, rpCaption, m_nFormat);
		}
	}

	pDC->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// This function sets all colors to a default value.
//
// Parameters:
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CButtonX::SetDefaultColors(BOOL bRepaint /*=TRUE*/)
{
	m_crColors[BX_COLOR_TXT_IN]    = ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BX_COLOR_TXT_OUT]	= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BX_COLOR_TXT_FOCUS] = ::GetSysColor(COLOR_BTNTEXT);

	m_bIsColorChanged = FALSE;

#ifdef _THEME_H_
#else
	m_crDisabled = ::GetSysColor(COLOR_GRAYTEXT);
#endif

	if (bRepaint)
		Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the text color to use for a particular state.
//
// Parameters:
//		[IN]	byColorIndex
//				Index of the color to set. Can be one of the following values:
//				BX_COLOR_TXT_IN	Text color when mouse is over the button
//				BX_COLOR_TXT_OUT	Text color when mouse is outside the button
//				BX_COLOR_TXT_FOCUS	Text color when the button is focused
//		[IN]	crColor
//				New color.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
BOOL CButtonX::SetTextColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint /*=TRUE*/)
{
	if (byColorIndex >= BX_COLOR_MAX)
		return FALSE;

	// Set new color
	m_crColors[byColorIndex] = crColor;

	m_bIsColorChanged = TRUE;

	if (bRepaint)
		Invalidate();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the text colors to use for a particular state.
//
// Parameters:
//		[IN]	crColor
//				Text color when mouse is over the button
//		[IN]	crColor
//				Text color when mouse is outside the button
//		[IN]	crColor
//				Text color when the button is focused
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Note: Pass NULL instead of RGB value in order that reset text color to it default value.
//
void CButtonX::SetTextColors(COLORREF crColorIn /*=NULL*/, COLORREF crColorOut /*=NULL*/, COLORREF crColorFocus /*=NULL*/, BOOL bRepaint /*=TRUE*/)
{
	m_crColors[BX_COLOR_TXT_IN] = crColorIn ? crColorIn : ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BX_COLOR_TXT_OUT] = crColorOut ? crColorOut : ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BX_COLOR_TXT_FOCUS] = crColorFocus ? crColorFocus : ::GetSysColor(COLOR_BTNTEXT);

	m_bIsColorChanged = TRUE;

	if (bRepaint)
		Invalidate();
}

BOOL CButtonX::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// If a cursor was specified then use it!
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the cursor to be used when the mouse is over the button.
//
// Parameters:
//		[IN]	nCursorId
//				ID number of the cursor resource.
//				Pass NULL to remove a previously loaded cursor.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
BOOL CButtonX::SetBtnCursor(int nCursorId, BOOL bRepaint /*=TRUE*/)
{
	HINSTANCE hInstResource = NULL;

	// Destroy any previous cursor
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	}

	// Load cursor
	if (nCursorId)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId), RT_GROUP_CURSOR);
		
		// Load cursor resource
		m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		
		// Repaint the button
		if (bRepaint) 
			Invalidate();

		// If something wrong
		if (m_hCursor == NULL) 
			return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Set button's text color when mouse is INside.
//
// Parameters:
//		[IN]	crColor
//				New color.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CButtonX::SetTextColorIn(COLORREF crColor, BOOL bRepaint /*= TRUE*/)
{
	m_crColors[BX_COLOR_TXT_IN] = crColor;

	m_bIsColorChanged = TRUE;

	if (bRepaint)
		Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// Set button's text color when mouse is OUTside.
//
// Parameters:
//		[IN]	crColor
//				New color.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CButtonX::SetTextColorOut(COLORREF crColor, BOOL bRepaint /*= TRUE*/)
{
	m_crColors[BX_COLOR_TXT_OUT] = crColor;

	m_bIsColorChanged = TRUE;

	if (bRepaint)
		Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// Set button's text color when button is focused.
//
// Parameters:
//		[IN]	crColor
//				New color.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CButtonX::SetTextColorFocus(COLORREF crColor, BOOL bRepaint /*= TRUE*/)
{
	m_crColors[BX_COLOR_TXT_FOCUS] = crColor;

	m_bIsColorChanged = TRUE;

	if (bRepaint)
		Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// This function allow to show or not the button's caption.
//
// Parameters:
//		[IN]	bShow
//				Show or not button's caption.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CButtonX::SetShowText(BOOL bShow /*=TRUE*/, BOOL bRepaint /*=TRUE*/)
{
  m_bShowText = bShow;

  if (bRepaint)
	  Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// This function allow to show or not the button's icon.
//
// Parameters:
//		[IN]	bShow
//				Show or not button's icon.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CButtonX::SetShowIcon(BOOL bShow /*=TRUE*/, BOOL bRepaint /*=TRUE*/)
{
  m_bShowIcon = bShow;

  if (bRepaint)
	  Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// This function allow to show or not the button caption when the icon is present.
//
// Parameters:
//		[IN]	bShow
//				Show or not only the button's icon.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CButtonX::SetShowIconOnly(BOOL bShow /*=TRUE*/, BOOL bRepaint /*=TRUE*/)
{
	m_bShowIconOnly = bShow;
	
	if (bRepaint)
		Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// Setup button with all possible parameters.
//
// Parameters:
//		[IN]	nIconIn
//				ID number of the icon resource to show when the mouse is over the button.
//				Pass NULL to remove any icon from the button.
//		[IN]	nIconOut
//				ID number of the icon resource to show when the mouse is outside the button.
//				Can be NULL.
//		[IN]	byAlign
//				Alignment type. Can be one of the following values:
//				BX_ALIGN_LEFT	  Icon on the left, text on the right
//				BX_ALIGN_VERT	  Icon on the top, text on the bottom
//				BX_ALIGN_RIGHT	  Icon on the right, text on the left
//				BX_ALIGN_OVERLAP Icon on the same space as text
//				By default, button have BX_ALIGN_LEFT alignment.
//		[IN]	lpszText
//				Pointer to a null-terminated string containing the text to show.
//		[IN]	nCursorId
//				ID number of the cursor resource.
//				Pass NULL to remove a previously loaded cursor.
//		[IN]	bDrawArrow
//				Draw or not arrow at right side of the button.
//		[IN]	crTxtIn
//				RGB value of the text color to set when mouse is over the button.
//              Can be NULL.
//		[IN]	crTxtIn
//				RGB value of the text color to set when mouse is outside the button.
//              Can be NULL.
//		[IN]	crTxtIn
//				RGB value of the text color to set when the button is focused.
//              Can be NULL.
//
void CButtonX::Setup(int nIcon /*= NULL*/,
				 BYTE byAlign /*= BX_ALIGN_LEFT*/,
				 LPCTSTR lpszTooltip /*= NULL*/,
				 int nCursor /*= NULL*/,
				 BOOL bDrawArrow /*= FALSE*/,
				 COLORREF crTxtIn /*= NULL*/, 
				 COLORREF crTxtOut /*= NULL*/, 
				 COLORREF crTxtFocus /*= NULL*/)
{
	// Set icon(s), if any
	SetIcon(nIcon);

	// Set icon alignment
	m_byAlign = byAlign;

	// Set tooltip
	if (lpszTooltip)
		SetTooltipText(lpszTooltip);

	// Set cursor
	if (nCursor)
		SetBtnCursor(nCursor, FALSE);

	// Is we should draw arrow
	m_bDrawArrow = bDrawArrow;

	// Set default colors
	SetDefaultColors(FALSE);

	// Set text colors
	if (crTxtIn)
	{
		m_crColors[BX_COLOR_TXT_IN] = crTxtIn;
		m_bIsColorChanged = TRUE;
	}
	
	if (crTxtOut)
	{
		m_crColors[BX_COLOR_TXT_OUT] = crTxtOut;
		m_bIsColorChanged = TRUE;
	}
	
	if (crTxtFocus)
	{
		m_crColors[BX_COLOR_TXT_FOCUS] = crTxtFocus;
		m_bIsColorChanged = TRUE;
	}

	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// This function sets the sound that must be played on particular button states.
//
// Parameters:
//		[IN]	lpszSound
//				A string that specifies the sound to play.
//				If hMod is NULL this string is interpreted as a filename, else it
//				is interpreted as a resource identifier.
//				Pass NULL to remove any previously specified sound.
//		[IN]	hMod
//				Handle to the executable file that contains the resource to be loaded.
//				This parameter must be NULL unless lpszSound specifies a resource identifier.
//		[IN]	bPlayOnClick
//				TRUE if the sound must be played when the button is clicked.
//				FALSE if the sound must be played when the mouse is moved over the button.
//		[IN]	bPlayAsync
//				TRUE if the sound must be played asynchronously.
//				FALSE if the sound must be played synchronously. The application takes control
//				when the sound is completely played.
//
#ifdef BX_USE_SOUND
void CButtonX::SetSound(LPCTSTR lpszSound, HMODULE hMod, BOOL bPlayOnClick, BOOL bPlayAsync)
{
	BYTE byIndex = bPlayOnClick ? 1 : 0;

	// Store new sound
	if (lpszSound)
	{
		if (hMod) // From resource identifier ?
			m_csSounds[byIndex].lpszSound = lpszSound;
		else
		{
			_tcscpy(m_csSounds[byIndex].szSound, lpszSound);
			m_csSounds[byIndex].lpszSound = m_csSounds[byIndex].szSound;
		}

		m_csSounds[byIndex].hMod = hMod;
		m_csSounds[byIndex].dwFlags = SND_NODEFAULT | SND_NOWAIT;
		m_csSounds[byIndex].dwFlags |= hMod ? SND_RESOURCE : SND_FILENAME;
		m_csSounds[byIndex].dwFlags |= bPlayAsync ? SND_ASYNC : SND_SYNC;
	}
	else // Or remove any existing
		::ZeroMemory(&m_csSounds[byIndex], sizeof(STRUCT_SOUND));
}
#endif

BOOL CButtonX::OnClicked() 
{
#ifdef BX_USE_SOUND
	// Play sound ?
	if (m_csSounds[1].lpszSound)
		::PlaySound(m_csSounds[1].lpszSound, m_csSounds[1].hMod, m_csSounds[1].dwFlags);
#endif

	if (m_bIsDefault)
		SetFocus();

	return FALSE;
}

LRESULT CButtonX::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
#ifdef _THEME_H_
		// Theme setting changed
	case WM_THEMECHANGED:
		InitTheme();
		break;
#endif	

		// System color setting changed
		// or system-wide setting changed
	case WM_SYSCOLORCHANGE:
	case WM_SETTINGCHANGE:
		if (!m_bIsColorChanged)
			SetDefaultColors();
		break;

	    // Cursor leaves the client area of the window 
	case WM_MOUSELEAVE:
		if (m_bMouseOver)
		{
			m_bMouseOver = FALSE;
			Invalidate(FALSE);
		}
		break;
	}
	
	return CButton::WindowProc(message, wParam, lParam);
}

#ifdef BX_USE_SETTINGS
/////////////////////////////////////////////////////////////////////////////
// This function sets is we should show icon and (or) text on the button.
//
// This function can be used for change button's visual effects.
// Use the SendMessageToDescendants( UINT message, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE )
// from main window to call this function.
// Sends the specified message to all immediate children of the CWindow object. 
// If bDeep is TRUE, the message is additionally sent to all other descendant windows.
// Example:
// SendMessageToDescendants(WM_BX_SETTINGCHANGE, NULL, (LPARAM)BX_OFF_ICON);
//
// Parameters:
//		[IN]	wParam
//				NOT USED.
//		[IN]	lParam
//				Settings flag. Can has the following values:
//              0                            - Use all visual effects
//              BX_OFF_ICON                 - Hide icon
//              BX_OFF_OWNERDRAW            - Not use DrawItem
//
LRESULT CButtonX::OnBxpSettingChange(WPARAM wParam, LPARAM lParam)
{
	SetSettings((UINT)lParam);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// This function returns current button's settings.
//
// Return value:
//              0                             - Use all visual effects
//              BX_OFF_ICON                  - Hide icon
//              BX_OFF_OWNERDRAW             - Not use DrawItem
//
UINT CButtonX::GetSettings()
{
	return m_nSettings;
}

/////////////////////////////////////////////////////////////////////////////
// This function sets is we should show icon and (or) text on the button.
//
// Parameters:
//		[IN]	nFlags
//				Settings flag. Can has the following values:
//              0                            - Use all visual effects
//              BX_OFF_ICON                  - Hide icon
//              BX_OFF_OWNERDRAW             - Not use DrawItem
//
void CButtonX::SetSettings(UINT nFlags /*=0*/)
{
	m_nSettings = nFlags;
	UINT nStyle = GetButtonStyle();

	if ((nStyle & BS_OWNERDRAW) && (m_nSettings & BX_OFF_OWNERDRAW))
	{
		nStyle &= ~BS_OWNERDRAW;
		SetButtonStyle(nStyle);
	}
	else
	if (!(nStyle & BS_OWNERDRAW) && !(m_nSettings & BX_OFF_OWNERDRAW))
	{
		nStyle |= BS_OWNERDRAW;
		SetButtonStyle(nStyle);
	}
	
	SetShowIcon((!m_nSettings & BX_OFF_ICON));
}

/////////////////////////////////////////////////////////////////////////////
// This function sets style of visual effects for all buttons.
//
// Parameters:
//		[IN]	nFlags
//				Settings flag. Can has the following values:
//              0                            - Use all visual effects
//              BX_OFF_ICON                  - Hide icon
//              BX_OFF_OWNERDRAW             - Not use DrawItem
//
void CButtonX::SetButtonsSettings(UINT nFlags /*=0*/)
{
	m_nSettings = nFlags;
}
#endif // BX_USE_SETTINGS

int CButtonX::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Check if this is the default button
	UINT nStyle = GetButtonStyle();
	
	// Add BS_OWNERDRAW style
	SetButtonStyle(nStyle | BS_OWNERDRAW);
	
	return 0;
}
