/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\

    FILE NAME:	    CustomizeDialog.cpp

	AUTHOR:		    Nikolay Denisov
	MODIFIED:	    Dreyk
	LAST REVERSION: 4.9.2003

	DESCRIPTION:    implementation of the CCustomizeDialog class 

\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/
/////////////////////////////////////////////////////////////////////////////
// CustomizeDialog.cpp: implementation of the CCustomizeDialog class.
//
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 by Nikolay Denisov. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Please email bug reports, bug fixes, enhancements, requests and
// comments to: nick@actor.ru
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomizeDialog.h"

#include "ToolBarX.h"
#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*
#ifdef _DEBUG
#define MODULE_TRACE()  TRACE("%s(%d) : Module State nInst = 0x%X\n", __FILE__, __LINE__, AfxGetModuleState()->m_hCurrentInstanceHandle)
#define THREAD_TRACE()  TRACE("%s(%d) : Thread State Address = 0x%X\n", __FILE__, __LINE__, AfxGetThreadState())
#else
#define MODULE_TRACE()
#define THREAD_TRACE()
#endif  //_DEBUG 
*/

// Custom ROPs
#define ROP_PSDPxax 0x00B8074A

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog

COptionsDialog::COptionsDialog(ETextOptions eTextOptions,
                                EIconOptions eIconOptions)
{
    //{{AFX_DATA_INIT(COptionsDialog)
    //}}AFX_DATA_INIT

    m_eTextOptions = eTextOptions;
    m_eIconOptions = eIconOptions;
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptionsDialog)
    DDX_Control(pDX, IDC_CB_TEXTOPTIONS, m_cbTextOptions);
    DDX_Control(pDX, IDC_CB_ICONOPTIONS, m_cbIconOptions);
    //}}AFX_DATA_MAP
}

BOOL COptionsDialog::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CDialog::Create(nIDTemplate, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// Operations

BOOL COptionsDialog::SelectTextOption(ETextOptions eTextOptions)
{
    for (int nIndex = 0; nIndex < m_cbTextOptions.GetCount(); nIndex++)
    {
        if (eTextOptions == (ETextOptions)m_cbTextOptions.GetItemData(nIndex))
        {
            m_cbTextOptions.SetCurSel(nIndex);
            m_eTextOptions = eTextOptions;
            return TRUE;
        }
    }

    return FALSE;
}

BOOL COptionsDialog::SelectIconOption(EIconOptions eIconOptions)
{
	for (int nIndex = 0; nIndex < m_cbIconOptions.GetCount(); nIndex++)
    {
		if (eIconOptions == (EIconOptions)m_cbIconOptions.GetItemData(nIndex))
        {
            m_cbIconOptions.SetCurSel(nIndex);
            m_eIconOptions = eIconOptions;
            return TRUE;
        }
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

CCustomizeDialog* COptionsDialog::GetCustomizeDialog() const
{
    return STATIC_DOWNCAST(CCustomizeDialog, GetParent());
}

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers

BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
    //{{AFX_MSG_MAP(COptionsDialog)
    ON_CBN_SELENDOK(IDC_CB_TEXTOPTIONS, OnTextOptions)
    ON_CBN_SELENDOK(IDC_CB_ICONOPTIONS, OnIconOptions)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COptionsDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    CCustomizeDialog* pCustomizeDialog = GetCustomizeDialog();

    pCustomizeDialog->AddTextOption(m_cbTextOptions, toTextLabels,   IDS_TO_TEXTLABELS);
    pCustomizeDialog->AddTextOption(m_cbTextOptions, toTextOnRight,  IDS_TO_TEXTONRIGHT);
    pCustomizeDialog->AddTextOption(m_cbTextOptions, toNoTextLabels, IDS_TO_NOTEXTLABELS);
    VERIFY(SelectTextOption(m_eTextOptions));

    pCustomizeDialog->AddIconOption(m_cbIconOptions, ioSmallIcons, IDS_IO_SMALLICONS);
    pCustomizeDialog->AddIconOption(m_cbIconOptions, ioLargeIcons, IDS_IO_LARGEICONS);
    VERIFY(SelectIconOption(m_eIconOptions));

    return TRUE;
}

void COptionsDialog::OnTextOptions()
{
    int nSel = m_cbTextOptions.GetCurSel();
    ASSERT(nSel != CB_ERR);
    m_eTextOptions = (ETextOptions)m_cbTextOptions.GetItemData(nSel);
    GetCustomizeDialog()->SetTextOptions(m_eTextOptions, FALSE);
}

void COptionsDialog::OnIconOptions()
{
    int nSel = m_cbIconOptions.GetCurSel();
    ASSERT(nSel != CB_ERR);
    m_eIconOptions = (EIconOptions)m_cbIconOptions.GetItemData(nSel);
    GetCustomizeDialog()->SetIconOptions(m_eIconOptions, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomizeDialog dialog

#define IDC_LB_AVAILABLE    0x00C9  // determined with Spy++
#define IDC_LB_CURRENT      0x00CB

static const int cxPadding = 3;
static const int cyPadding = 3;

IMPLEMENT_DYNAMIC(CCustomizeDialog, CWnd)

CCustomizeDialog::CCustomizeDialog(CToolBarX* pToolBar)
    : m_dlgOptions(pToolBar->m_eTextOptions, pToolBar->m_eIconOptions)
{
    m_pToolBar = pToolBar;
}

/////////////////////////////////////////////////////////////////////////////
// Operations

void CCustomizeDialog::SetTextOptions(ETextOptions eTextOptions, BOOL bInDialog)
{
    if (bInDialog)
        VERIFY(m_dlgOptions.SelectTextOption(eTextOptions));
    else
        m_pToolBar->SetTextOptions(eTextOptions);
}

void CCustomizeDialog::SetIconOptions(EIconOptions eIconOptions, BOOL bInDialog)
{
    if (bInDialog)
        VERIFY(m_dlgOptions.SelectIconOption(eIconOptions));
    else
        m_pToolBar->SetIconOptions(eIconOptions);

    int nHeight = GetButtonSize().cy;

    CWnd* pWnd = GetDlgItem(IDC_LB_AVAILABLE);
    if (pWnd->GetSafeHwnd())
    {
        pWnd->SendMessage(LB_SETITEMHEIGHT, 0, nHeight);
        pWnd->Invalidate();

//		CRect r;
//		pWnd->GetClientRect(r);
//		pWnd->MapWindowPoints(this, r);
//		InvalidateRect(r);
//		pWnd->GetClientRect(&r);
//		ScreenToClient(&r);
//		InvalidateRect(r);
//		pWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);
    }

    pWnd = GetDlgItem(IDC_LB_CURRENT);
    if (pWnd->GetSafeHwnd())
    {
        pWnd->SendMessage(LB_SETITEMHEIGHT, 0, nHeight);
        pWnd->Invalidate();
    }
}

/////////////////////////////////////////////////////////////////////////////
// Overrides

void CCustomizeDialog::PostNcDestroy()
{
    delete this;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

void CCustomizeDialog::AddTextOption(CComboBox& cbTextOptions, ETextOptions eTextOptions,
                                      UINT nStringID)
{
    if (m_pToolBar->IsTextOptionAvailable(eTextOptions))
    {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CString strText;
        VERIFY(strText.LoadString(nStringID));
        int nItem = cbTextOptions.AddString(strText);
        ASSERT(nItem >= 0);
        cbTextOptions.SetItemData(nItem, (DWORD)eTextOptions);
    }
}

void CCustomizeDialog::AddIconOption(CComboBox& cbIconOptions, EIconOptions eIconOptions,
                                      UINT nStringID)
{
    if (m_pToolBar->IsIconOptionAvailable(eIconOptions))
    {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CString strText;
        VERIFY(strText.LoadString(nStringID));
        int nItem = cbIconOptions.AddString(strText);
        ASSERT(nItem >= 0);
        cbIconOptions.SetItemData(nItem, (DWORD)eIconOptions);
    }
}

CSize CCustomizeDialog::GetButtonSize() const
{
    CSize szImage = (m_pToolBar->GetIconOptions() == ioSmallIcons) ?
        m_szImageSmall : m_szImageLarge;
    return szImage + CSize(cxPadding * 2, cyPadding * 2);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomizeDialog message handlers

BEGIN_MESSAGE_MAP(CCustomizeDialog, CWnd)
    //{{AFX_MSG_MAP(CCustomizeDialog)
    ON_WM_DRAWITEM()
    ON_WM_MEASUREITEM()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
END_MESSAGE_MAP()

LRESULT CCustomizeDialog::OnInitDialog(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CRect rcClient;
    GetClientRect(rcClient);
    CRect rcWindow;
    GetWindowRect(rcWindow);

    // Create Options dialog as a child window of Customize Toolbar dialog
    if (!m_dlgOptions.Create(IDD_DLG_CUSTOMIZE, this))
		return Default();

	ModifyStyleEx(WS_EX_CONTEXTHELP, 0);

    CRect rcDlg;
    m_dlgOptions.GetWindowRect(rcDlg);

    // Place Options dialog at the bottom
    VERIFY(m_dlgOptions.SetWindowPos(0, 0, rcClient.Height(), rcClient.Width(), rcDlg.Height(),
        SWP_NOZORDER | SWP_SHOWWINDOW));

    // Increase height of Customize Toolbar dialog accordingly
    rcWindow.bottom += rcDlg.Height();
    VERIFY(SetWindowPos(0, 0, 0, rcWindow.Width(), rcWindow.Height(),
        SWP_NOZORDER | SWP_NOMOVE));

	//Set Title
	ASSERT(m_pToolBar);
	CString strName, strOrgName;
	m_pToolBar->GetWindowText(strName);

	if (!strName.IsEmpty())
	{
		GetWindowText(strOrgName);
		SetWindowText(strOrgName + _T(" - ") + strName);
	}

    return Default();
}

void CCustomizeDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if (lpDrawItemStruct->CtlType == ODT_LISTBOX)
    {
        if ((nIDCtl == IDC_LB_AVAILABLE) || (nIDCtl == IDC_LB_CURRENT))
        {
            CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
            int nSavedDC = pDC->SaveDC();
            ASSERT(nSavedDC);

            // Get all needed info about the item being drawn
            CRect rcItem(&lpDrawItemStruct->rcItem);
            int nItem  = (SHORT)HIWORD(lpDrawItemStruct->itemData);
            int nImage = (SHORT)LOWORD(lpDrawItemStruct->itemData);
            int nIndex = (int)lpDrawItemStruct->itemID;
            BOOL bSelected  = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
            BOOL bFocused   = (lpDrawItemStruct->itemState & ODS_FOCUS   ) != 0;
            BOOL bSeparator = ((nItem < 0) || (nItem > m_pToolBar->m_arTB.GetUpperBound()));

            // Calculate rectangles for image, text and focus frame
            CSize szButton = GetButtonSize();
            CRect rcImage(rcItem);
            rcImage.right = rcImage.left + szButton.cx;
            CRect rcText(rcItem);
            rcText.left  += szButton.cx + 5;
            rcText.right -= 2;
            CRect rcLabel(rcItem);
            rcLabel.left += szButton.cx + 1;

            // Cache system colors
            COLORREF clrText     = ::GetSysColor((bFocused && bSelected) ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT);
			COLORREF clrBack     = ::GetSysColor((bFocused && bSelected) ? COLOR_HIGHLIGHT : COLOR_WINDOW);
            COLORREF clrGrayText = ::GetSysColor(COLOR_GRAYTEXT);
            COLORREF clrBtnFace  = ::GetSysColor(COLOR_BTNFACE);
            COLORREF clr3dShadow = ::GetSysColor(COLOR_3DSHADOW);
            COLORREF clr3dHilite = ::GetSysColor(COLOR_3DHILIGHT);

            pDC->FillSolidRect(rcItem, clrBack);

            // Draw focus rect, if needed
            if (bSelected)
            {
				if (bFocused)
					pDC->DrawFocusRect(rcItem);
				else
				{
					CBrush brush(::GetSysColor(COLOR_HIGHLIGHT));
					pDC->FrameRect(rcItem, &brush);
					VERIFY(brush.DeleteObject());
				}
            }

            // Get item text to be drawn
            CString strText;
            if (!bSeparator)
            {
                UINT nID = m_pToolBar->m_arTB[nItem].tbinfo.idCommand;
                m_pToolBar->GetButtonText(nID, strText);
            }
            else
				strText = uLocalLoadString(IDS_SEPARATOR);

            // Prepare to draw item text
            int nFormat = DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS;
            pDC->SetBkMode(TRANSPARENT);
            pDC->SetTextColor(clrText);

            if (!bSeparator)
            {
                // Draw item image, if any
                if (nImage >= 0)
                {
                    CToolBarCtrl& tbCtrl = m_pToolBar->GetToolBarCtrl();
                    CImageList* pImageList = tbCtrl.GetHotImageList();

                    if (!bSelected || !pImageList)
                        pImageList = tbCtrl.GetImageList();
					
					if (pImageList)
					{
						rcImage.DeflateRect(cxPadding, cyPadding);
						VERIFY(pImageList->Draw(pDC, nImage, rcImage.TopLeft(),
							ILD_NORMAL | ILD_TRANSPARENT));
					}
                }

                if (m_pToolBar->m_arTB[nItem].tbinfo.fsStyle & TBSTYLE_DROPDOWN)
                {
                    // This button is drop-down button.  To indicate this simple fact
                    // we draw an arrow on right like one that submenu items have.

                    CRect rcArrow(rcText);
                    rcArrow.left = rcArrow.right - ::GetSystemMetrics(SM_CXMENUCHECK);
                    rcText.right -= rcArrow.Width();

                    CBrush brush(clrText);
                    CBrush* pOldBrush = pDC->SelectObject(&brush);

                    CDC dc;
                    VERIFY(dc.CreateCompatibleDC(pDC));

                    CBitmap bmpArrow;
                    VERIFY(bmpArrow.CreateCompatibleBitmap(pDC, rcArrow.Width(), rcArrow.Height()));

                    CBitmap* pOldBitmap = dc.SelectObject(&bmpArrow);
                    CRect rc(0, 0, rcArrow.Width(), rcArrow.Height());
                    VERIFY(dc.DrawFrameControl(rc, DFC_MENU, DFCS_MENUARROW));
                    VERIFY(pDC->BitBlt(rcArrow.left, rcArrow.top, rcArrow.Width(), rcArrow.Height(),
                        &dc, 0, 0, ROP_PSDPxax));

                    dc.SelectObject(pOldBitmap);
                    VERIFY(bmpArrow.DeleteObject());

                    pDC->SelectObject(pOldBrush);
                    VERIFY(brush.DeleteObject());
                }//if (m_pToolBar->m_arTB[nItem].tbinfo.fsStyle & TBSTYLE_DROPDOWN)
            }//if (!bSeparator)
            else
            {
                if ((nIDCtl == IDC_LB_CURRENT) &&
                     (nIndex == SendDlgItemMessage(nIDCtl, LB_GETCOUNT) - 1))
                {
                    // The last item in the list of current buttons is a separator
                    // which cannot be removed (guess what is the purpose of it?).
                    // To indicate this fact we draw it as disabled item.

                    if (bSelected)
                        pDC->SetTextColor(clrGrayText);
                    else
                    {
                        CRect rcTemp(rcText);
                        rcTemp.OffsetRect(1, 1);
                        pDC->SetTextColor(clr3dHilite);
                        pDC->DrawText(strText, rcTemp, nFormat);
                        pDC->SetTextColor(clr3dShadow);
                    }
                }
                else
                {
                    // Draw the separator bar in the middle
                    rcImage.DeflateRect(cxPadding, cyPadding);
                    int y = (rcImage.top + rcImage.bottom) / 2;
                    rcImage.top    = y - 1;
                    rcImage.bottom = y + 1;

                    pDC->Draw3dRect(rcImage, clr3dShadow, clr3dHilite);
                }
            }//else: if (!bSeparator)

            pDC->DrawText(strText, rcText, nFormat);

            VERIFY(pDC->RestoreDC(nSavedDC));
            return;
        }//if ((nIDCtl == IDC_LB_AVAILABLE) || (nIDCtl == IDC_LB_CURRENT))
    }//if (lpDrawItemStruct->CtlType == ODT_LISTBOX)

    CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CCustomizeDialog::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if (lpMeasureItemStruct->CtlType == ODT_LISTBOX)
    {
        if ((nIDCtl == IDC_LB_AVAILABLE) ||
             (nIDCtl == IDC_LB_CURRENT))
        {
            lpMeasureItemStruct->itemHeight = GetButtonSize().cy;
            return;
        }
    }

    CWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

