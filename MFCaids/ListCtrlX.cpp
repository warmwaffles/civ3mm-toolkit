/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
   FILE NAME:		ListCtrlX.cpp
   VERSION:			1.11

   AUTHOR:			Rat AKA Oleg Glabuchek
   MODIFIED:		Alex Zhondin, Dreyk

   DESCRIPTION:		ListCtrlX - full featured CListCtrl class. 
                    Heavily based on the CListCtrlX class of Rat.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "MFCaids.h"
#include "ListCtrlX.h"

#include "Utilities.h"
#include "MFCdefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Definition of "workhorse" class "CListBase"
const int CListCtrlX::m_nFirstColXOff = 2; // x-offset of first column
const int CListCtrlX::m_nNextColXOff  = 6; // x-offset of other columns
const int CListCtrlX::m_nIconXOff     = 4; // x-offset of icon

//*************************************************************************//
// CListCtrlX class
//*************************************************************************//

CListCtrlX::CListCtrlX()
{
    m_bKeepLabelLeft   = FALSE;
    m_bColorSortColumn = FALSE;
    m_bSortIconEnabled = TRUE;
    m_nSortColumn      = 0;
    //	m_bSortIconEnabled     = FALSE;
    //	m_nSortColumn          = 1;
    m_nColumnSel = -1;

    m_dwColBkNonItem       = 0x0f000000;
    m_dwColBkItem          = 0x0f000000;
    m_dwColGridLines       = 0x0f000000;
    m_dwColItemText        = 0x0f000000;
    m_dwColHilightInactive = 0x0f000000;
    m_dwColHilight         = 0x0f000000;
    m_dwColSelItemText     = 0x0f000000;
    m_dwColBkCellSel       = 0x0f000000;
    m_dwColCellSelText     = 0x0f000000;
    m_dwColBkCell          = 0x0f000000;

    CreateSortIcons();

    m_strEmptyMsg     = uLocalLoadString(IDS_LISTTABLE_NOITEMS);
    m_bRepaintNonItem = TRUE;
    m_bWarning        = FALSE;
    m_nError          = 0;

    m_pSpin  = NULL;
    m_pEdit  = NULL;
    m_pCombo = NULL;

    m_bBtnPressed = FALSE;
    m_bBtnPushed  = FALSE;

    m_rcBtn.SetRect(0, 0, 0, 0);

    m_pImgList_Small  = NULL;
    m_pImgList_Normal = NULL;
    m_pImgList_State  = NULL;

    ///////////////////////////////////////////////////////////
    // Drag and Drop
    m_pImgListDrag = NULL;
    m_bDragEnable  = FALSE;
    m_bDragging    = FALSE;
    m_nDragIndex   = -1;
    m_nDropIndex   = -1;
}

CListCtrlX::~CListCtrlX()
{
    SAFE_DELETE(m_pSpin);
    SAFE_DELETE(m_pCombo);
    SAFE_DELETE(m_pImgListDrag);

    DestroyEdit();
}

BEGIN_MESSAGE_MAP(CListCtrlX, CListCtrl)
//{{AFX_MSG_MAP(CListCtrlX)
ON_WM_ERASEBKGND()
ON_WM_SYSCOLORCHANGE()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_KEYDOWN()
ON_WM_LBUTTONDBLCLK()
ON_WM_PAINT()
ON_NOTIFY(HDN_ENDTRACKW, 0, OnEndtrack)
ON_NOTIFY(HDN_ITEMCHANGEDW, 0, OnItemchanged)
ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
ON_WM_CHAR()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_CAPTURECHANGED()
ON_WM_VSCROLL()
ON_WM_MOUSEWHEEL()
ON_NOTIFY(HDN_ENDTRACKA, 0, OnEndtrack)
ON_NOTIFY(HDN_ITEMCHANGEDA, 0, OnItemchanged)
ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
ON_NOTIFY(UDN_DELTAPOS, IDC_LISTTAB_SPINCELL, OnDeltaposSpin)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void
CListCtrlX::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    LVITEM item;
    CRect rcItem;      // rectangle bounding complete item
    CRect rcSubItem0;  // rectangle bounding subitem 0
    CRect rcLabel;     // rectangle bounding item label
    CRect rcSelection; // rectangle bounding selection

    item.mask       = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
    item.iItem      = lpDrawItemStruct->itemID;
    item.iSubItem   = 0;
    item.stateMask  = ~0U;
    item.cchTextMax = 256;
    item.pszText    = new TCHAR[256];
    GetItem(&item);

    //	TRACE("DrawItem %d\n", lpDrawItemStruct->itemID);
    dp.nRow             = lpDrawItemStruct->itemID;
    dp.bAlwaysSelected  = item.state & LVIS_SELECTED && (GetStyle() & LVS_SHOWSELALWAYS) != 0;
    dp.bLVHasFocus      = GetFocus() == this;
    dp.bItemHasFocus    = item.state & LVIS_FOCUSED && dp.bLVHasFocus;
    dp.bSelected        = item.state & LVIS_SELECTED && dp.bLVHasFocus;
    dp.bFullRowSelected = (GetExtendedStyle() & LVS_EX_FULLROWSELECT) != 0;
    //	dp.bHotLight        = lpDrawItemStruct->itemState & ODS_HOTLIGHT;
    dp.bHotLight = item.state & LVIS_DROPHILITED;

    GetItemRect(item.iItem, rcItem, LVIR_BOUNDS);
    GetRealSubItemRect(item.iItem, 0, LVIR_BOUNDS, rcSubItem0);
    GetRealSubItemRect(item.iItem, 0, LVIR_LABEL, rcLabel);

    if (dp.bFullRowSelected) {
        rcSelection = rcItem;
        if (IndexToOrder(0) == 0 || m_bKeepLabelLeft) {
            //rcSelection.left = rcLabel.left;
            int nWidthOfCol0 = GetColumnWidth(OrderToIndex(0));
            if (rcSelection.left > nWidthOfCol0)
                rcSelection.left = nWidthOfCol0;
        }
    }

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    if (dp.bSelected) {
        dp.dwNormalText     = CHOOSE_COL(m_dwColItemText, GetSysColor(COLOR_HIGHLIGHTTEXT));
        dp.dwNormalTextCell = CHOOSE_COL(m_dwColCellSelText, GetSysColor(COLOR_HIGHLIGHTTEXT));
        dp.dwHiLiteBk       = CHOOSE_COL(m_dwColHilight, GetSysColor(COLOR_HIGHLIGHT));
        dp.dwHiLiteBkCell   = CHOOSE_COL(m_dwColBkCellSel, GetSysColor(COLOR_HOTLIGHT));
    } else if (dp.bAlwaysSelected) {
        dp.dwNormalText     = CHOOSE_COL(m_dwColItemText, GetSysColor(COLOR_WINDOWTEXT));
        dp.dwNormalTextCell = CHOOSE_COL(m_dwColCellSelText, GetSysColor(COLOR_WINDOWTEXT));
        dp.dwHiLiteBk       = CHOOSE_COL(m_dwColHilightInactive, GetSysColor(COLOR_3DFACE));
        dp.dwHiLiteBkCell   = CHOOSE_COL(m_dwColBkCell, GetSysColor(COLOR_3DSHADOW));
    } else {
        dp.dwNormalText     = CHOOSE_COL(m_dwColItemText, GetSysColor(COLOR_WINDOWTEXT));
        dp.dwNormalTextCell = CHOOSE_COL(m_dwColCellSelText, GetSysColor(COLOR_WINDOWTEXT));
    }

    dp.penGrid.CreatePen(PS_SOLID, 1, CHOOSE_COL(m_dwColGridLines, GetSysColor(COLOR_BTNSHADOW)));

    LVCOLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH;

    // display all subitems
    LVITEM SubItem;
    CRect rcSubItem; // rectangle bounding subitem
    CRect rcText;    // output rectangle
    CRect rcSubItemSel;
    CRect rcIcon;
    int nFormat;
    int nLabelWidth;
    BOOL bHiLite;

    for (dp.nColumn = 0; GetColumn(dp.nIndex = OrderToIndex(dp.nColumn), &lvc); dp.nColumn++) {
        if (dp.nIndex > 0) {
            SubItem.mask       = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
            SubItem.iItem      = lpDrawItemStruct->itemID;
            SubItem.iSubItem   = dp.nIndex;
            SubItem.stateMask  = ~0U;
            SubItem.cchTextMax = 256;
            SubItem.pszText    = new TCHAR[256];
            GetItem(&SubItem);
            dp.pSubItem = &SubItem;
        } else
            dp.pSubItem = &item;

        dp.bDrawCell = (GetSelectedCount() <= 1) && (dp.nIndex == m_nColumnSel) && (dp.bSelected || dp.bAlwaysSelected);

        // background color of curremt column
        dp.dwBkColor = CHOOSE_COL(m_dwColBkItem, GetSysColor(COLOR_WINDOW));
        if (m_bColorSortColumn && (dp.nIndex == m_nSortColumn))
            dp.dwBkColor = m_dwColSortColor;

        // consider column margins
        if (dp.nColumn > 0) {
            // move output rectangle over next column
            rcSubItem.left = rcSubItem.right;
            rcSubItem.right += lvc.cx;
        } else {
            rcSubItem       = rcItem;
            rcSubItem.right = rcSubItem.left + lvc.cx;
        }

        if (dp.nIndex == 0 && !m_bKeepLabelLeft || dp.nColumn == 0 && m_bKeepLabelLeft) {
            rcText = rcLabel;
            rcText.left += m_nFirstColXOff;
            rcText.right -= dp.nIndex > 0 ? m_nNextColXOff : m_nFirstColXOff;
        } else {
            rcText = rcSubItem;
            rcText.left += m_nNextColXOff;
            rcText.right -= m_nNextColXOff;
        }

        nLabelWidth = GetLabelWidth(pDC, dp.pSubItem, rcText.Width());
        bHiLite     = FALSE;
        int nSelWidth;
        CRect rc;
        CBrush brush(dp.dwBkColor);

        if (dp.bSelected || dp.bAlwaysSelected || dp.bItemHasFocus && !dp.bFullRowSelected) {
            if (dp.nIndex == 0 && dp.nColumn != 0 && !m_bKeepLabelLeft && !dp.bFullRowSelected ||
                dp.nColumn == 0 && (m_bKeepLabelLeft || dp.nIndex == 0)) {
                // calculate selection area
                rcSubItemSel = rcItem;

                if (!dp.bFullRowSelected) {
                    nFormat = dp.nIndex == 0 && dp.nColumn == 0 || !m_bKeepLabelLeft ? m_nFormatOfSubItem0 : lvc.fmt & LVCFMT_JUSTIFYMASK;

                    switch (nFormat) {
                        case LVCFMT_LEFT:
                            rcSubItemSel.right = rcSubItemSel.left + m_nFirstColXOff + nLabelWidth + m_nNextColXOff;
                            break;
                        case LVCFMT_RIGHT:
                            rcSubItemSel.left = rcSubItemSel.right - m_nFirstColXOff - nLabelWidth - m_nNextColXOff;
                            break;
                        case LVCFMT_CENTER: {
                            nSelWidth          = m_nNextColXOff + nLabelWidth + m_nNextColXOff;
                            rcSubItemSel.left  = rcLabel.left + (rcLabel.Width() - nSelWidth) / 2;
                            rcSubItemSel.right = rcSubItemSel.left + nSelWidth;
                            break;
                        }
                        default:
                            ASSERT(FALSE);
                            break;
                    }

                    if (rcSubItemSel.left < rcLabel.left)
                        rcSubItemSel.left = rcLabel.left;

                    if (rcSubItemSel.right > rcLabel.right)
                        rcSubItemSel.right = rcLabel.right;

                    rcSelection = rcSubItemSel;
                }

                if (dp.bSelected || dp.bAlwaysSelected) {
                    if (rcSubItemSel.left > rcSubItem.left) {
                        // fill area left from selection with background color
                        rc       = rcSubItem;
                        rc.right = rcSubItemSel.left - 1;
                        pDC->FillRect(rc, &brush);
                    }

                    DrawCellRect(pDC, rcSubItemSel, rcText, TRUE);

                    // fill area right from selection with background color
                    if (rcSubItemSel.right < rcSubItem.right) {
                        rc      = rcSubItem;
                        rc.left = rcSubItemSel.right + 1;
                        pDC->FillRect(rc, &brush);
                    }

                    pDC->SetBkColor(dp.bDrawCell ? dp.dwHiLiteBkCell : dp.dwHiLiteBk);
                    pDC->SetTextColor(dp.bDrawCell ? dp.dwNormalTextCell : dp.dwNormalText);
                    bHiLite = TRUE;
                }
            } else {
                if (dp.bFullRowSelected) {
                    DrawCellRect(pDC, rcSubItem, rcText, TRUE);
                    pDC->SetBkColor(dp.bDrawCell ? dp.dwHiLiteBkCell : dp.dwHiLiteBk);
                    pDC->SetTextColor(dp.bDrawCell ? dp.dwNormalTextCell : dp.dwNormalText);
                    bHiLite = TRUE;
                }
            }
        }

        if (!bHiLite) {
            DrawCellRect(pDC, rcSubItem, rcText, FALSE);
            pDC->SetBkColor(dp.dwBkColor);
            pDC->SetTextColor(dp.dwNormalText);
        }

        if (dp.nIndex == 0 && !m_bKeepLabelLeft || dp.nColumn == 0 && m_bKeepLabelLeft) {
            if (GetStateIconRect(item.iItem, rcIcon))
                DrawStateIcon(pDC, &item, rcIcon);

            if (GetRealSubItemRect(item.iItem, 0, LVIR_ICON, rcIcon))
                DrawSmallIcon(pDC, &item, rcIcon);
        }

        DrawSubItemText(pDC, dp.pSubItem, &lvc, rcText);
        DrawCellGrid(pDC, rcSubItem);

        if (dp.pSubItem != &item)
            delete[] dp.pSubItem->pszText;
    } // for (dp.nColumn = 0; GetColumn(dp.nIndex = OrderToIndex(dp.nColumn), &lvc); dp.nColumn++)

    dp.penGrid.DeleteObject();

    if (dp.bItemHasFocus || dp.bHotLight)
        pDC->DrawFocusRect(rcSelection);

    delete[] item.pszText;
}

BOOL
CListCtrlX::GetRealSubItemRect(int iItem, int iSubItem, int nArea, CRect& ref)
{
    ASSERT(iSubItem >= 0);

    switch (nArea) {
        case LVIR_BOUNDS:
            if (GetSubItemRect(iItem, iSubItem, LVIR_BOUNDS, ref)) {
                if (iSubItem == 0) {
                    CRect rcLabel;
                    if (GetSubItemRect(iItem, 0, LVIR_LABEL, rcLabel)) {
                        ref.right = rcLabel.right;

                        int nOrder = IndexToOrder(0);
                        if (nOrder > 0) {
                            // The left edge of subitem 0 is identical with the right edge of
                            // the subitem left of subitem 0.
                            CRect rcSubItem;

                            if (GetSubItemRect(iItem, OrderToIndex(nOrder - 1), LVIR_BOUNDS, rcSubItem)) {
                                ref.left = rcSubItem.right;
                                return TRUE;
                            }
                        } else
                            return TRUE;
                    }
                } else
                    return TRUE;
            }
            break;

        case LVIR_ICON: {
            CRect rcIcon; // rectangle bounding small icon of subitem 0
            if (GetSubItemRect(iItem, 0, LVIR_ICON, rcIcon)) {
                CRect rcSubItem0; // rectangle bounding subitem 0
                if (GetRealSubItemRect(iItem, 0, LVIR_BOUNDS, rcSubItem0)) {
                    if (IndexToOrder(0) > 0 && m_bKeepLabelLeft) {
                        int nIndex = OrderToIndex(0);

                        if (GetRealSubItemRect(iItem, nIndex, LVIR_BOUNDS, ref)) {
                            int nSmallIconXOff = rcIcon.left - rcSubItem0.left;

                            ref.left += nSmallIconXOff;
                            ref.right = ref.left + rcIcon.Width();

                            // clip rectangle at right edge if necessary
                            int nWidth = GetColumnWidth(nIndex);
                            if (nSmallIconXOff + ref.Width() >= nWidth)
                                ref.right = ref.left - nSmallIconXOff + nWidth - 1;
                            return TRUE;
                        }
                    } else {
                        // clip rectangle at right edge if necessary
                        if (rcIcon.right > rcSubItem0.right)
                            rcIcon.right = rcSubItem0.right;
                        ref              = rcIcon;
                        return TRUE;
                    }
                }
            }
            break;
        }

        case LVIR_LABEL: {
            CRect rcLabel; // rectangle bounding label of subitem 0

            if (GetSubItemRect(iItem, 0, LVIR_LABEL, rcLabel)) {
                CRect rcSubItem0; // rectangle bounding subitem 0
                if (GetRealSubItemRect(iItem, 0, LVIR_BOUNDS, rcSubItem0)) {
                    if (IndexToOrder(0) > 0 && m_bKeepLabelLeft) {
                        if (GetRealSubItemRect(iItem, OrderToIndex(0), LVIR_BOUNDS, ref)) {
                            ref.left += rcLabel.left - rcSubItem0.left;
                            return TRUE;
                        }
                    } else {
                        ref = rcLabel;
                        return TRUE;
                    }
                }
            }
            break;
        }
        default:
            ASSERT(FALSE);
            break;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Return the order in the header control of a subitem, based on its index
//
int
CListCtrlX::IndexToOrder(int nIndex)
{
    ASSERT(GetHeaderCtrl());

    HDITEM headerItem = { HDI_ORDER };
    return GetHeaderCtrl()->GetItem(nIndex, &headerItem) ? headerItem.iOrder : -1;
}

/////////////////////////////////////////////////////////////////////////////
// Return the index of a subitem, based in its order in the header control
//
int
CListCtrlX::OrderToIndex(int nOrder)
{
    ASSERT(GetHeaderCtrl());

    return GetHeaderCtrl()->OrderToIndex(nOrder);
}

/////////////////////////////////////////////////////////////////////////////
// Small icon always should be kept left
//
BOOL
CListCtrlX::SetKeepLabelLeft(BOOL bKeepLeft)
{
    BOOL bSuccess = TRUE;

    if (bKeepLeft) {
        if ((GetStyle() & LVS_OWNERDRAWFIXED) == 0) {
            bKeepLeft = FALSE;
            bSuccess  = FALSE;
        }
    }

    m_bKeepLabelLeft = bKeepLeft;
    return bSuccess;
}

void
CListCtrlX::EnableSortIcon(BOOL bEnable, int nSortColumn)
{
    ASSERT(GetHeaderCtrl());
    m_bSortIconEnabled = bEnable;
    m_nSortColumn      = nSortColumn;
    GetHeaderCtrl()->SetImageList(&m_ImgListSort);
    SetSortIcon(); // display or hide sort icon
}

/////////////////////////////////////////////////////////////////////////////
// Display or hide sort icon on column to be sorted
//
void
CListCtrlX::SetSortIcon()
{
    CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
    ASSERT(pHeaderCtrl);

    int nCols = pHeaderCtrl->GetItemCount();
    HDITEM hdrItem;
    hdrItem.mask = HDI_FORMAT | HDI_IMAGE;

    for (int col = 0; col < nCols; col++) {
        pHeaderCtrl->GetItem(col, &hdrItem);
        if (IsSorted(col) && (m_bSortIconEnabled && m_nSortColumn - 1 == col)) {
            hdrItem.iImage = 1;
            hdrItem.fmt    = hdrItem.fmt & HDF_JUSTIFYMASK | HDF_IMAGE | HDF_STRING | HDF_BITMAP_ON_RIGHT;
        } else if (IsSorted(col) && (m_bSortIconEnabled && -m_nSortColumn - 1 == col)) {
            hdrItem.iImage = 0;
            hdrItem.fmt    = hdrItem.fmt & HDF_JUSTIFYMASK | HDF_IMAGE | HDF_STRING | HDF_BITMAP_ON_RIGHT;
        } else
            hdrItem.fmt = hdrItem.fmt & HDF_JUSTIFYMASK | HDF_STRING;
        pHeaderCtrl->SetItem(col, &hdrItem);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Calculate width of item or subitem label
//
int
CListCtrlX::GetLabelWidth(CDC* pDC, LVITEM* pItem, int nMaxWidth)
{
    if (nMaxWidth > 0 && *pItem->pszText) {
        CString strLabel(pItem->pszText);

        uMakeShortString(pDC, strLabel, nMaxWidth);
        return pDC->GetTextExtent(strLabel).cx;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Retrieves the bounding rectangle for the state icon of an item
//
BOOL
CListCtrlX::GetStateIconRect(int nItem, LPRECT pRect)
{
    CRect rcSubItem;

    if (GetRealSubItemRect(nItem, m_bKeepLabelLeft ? OrderToIndex(0) : 0, LVIR_BOUNDS, rcSubItem)) {
        CRect rcSmallIcon;

        if (GetRealSubItemRect(nItem, 0, LVIR_ICON, rcSmallIcon)) {
            *pRect = rcSubItem;
            pRect->left += m_nIconXOff;
            pRect->right = rcSmallIcon.left - 1;

            // clip at right column border
            int nWidth = rcSubItem.Width();
            if (pRect->right >= rcSubItem.left + rcSubItem.Width())
                pRect->right = pRect->left + nWidth - 1;

            return TRUE;
        }
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Draw state icon
//
void
CListCtrlX::DrawStateIcon(CDC* pDC, LVITEM* pItem, LPRECT pRect)
{
    int nImage = (pItem->state & LVIS_STATEIMAGEMASK) >> 12;

    if (nImage > 0) {
        CImageList* pimglst = GetImageList(LVSIL_STATE);

        if (pimglst) {
            IMAGEINFO imgInfo;
            // image indices are zero-based
            if (pimglst->GetImageInfo(--nImage, &imgInfo)) {
                pimglst->DrawIndirect(pDC,
                                      nImage,
                                      CPoint(pRect->left, pRect->top),
                                      CSize(__min(pRect->right - pRect->left + 1, imgInfo.rcImage.right - imgInfo.rcImage.left),
                                            __min(pRect->bottom - pRect->top + 1, imgInfo.rcImage.bottom - imgInfo.rcImage.top)),
                                      CPoint(0, 0),
                                      //pItem->state & LVIS_SELECTED &&
                                      //GetFocus() == this ? ILD_SELECTED :
                                      ILD_NORMAL,
                                      SRCCOPY,
                                      CLR_NONE);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Draw small icon
//
void
CListCtrlX::DrawSmallIcon(CDC* pDC, LVITEM* pItem, LPRECT pRect)
{
    if (pItem->iImage >= 0) {
        CImageList* pimglst = GetImageList(LVSIL_SMALL);

        if (pimglst) {
            IMAGEINFO imgInfo;

            if (pimglst->GetImageInfo(pItem->iImage, &imgInfo)) {
                CSize szImg(__min(pRect->right - pRect->left + 1, imgInfo.rcImage.right - imgInfo.rcImage.left),
                            __min(pRect->bottom - pRect->top + 1, imgInfo.rcImage.bottom - imgInfo.rcImage.top));
                pimglst->DrawIndirect(pDC,
                                      pItem->iImage,
                                      CPoint(pRect->left, pRect->top + ((pRect->bottom - pRect->top - szImg.cy) >> 1)),
                                      szImg,
                                      CPoint(0, 0),
                                      //pItem->state & LVIS_SELECTED &&
                                      //GetFocus() == this ? ILD_SELECTED :
                                      ILD_NORMAL,
                                      SRCCOPY,
                                      CLR_NONE);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Draw the label of an item or subitem
//
void
CListCtrlX::DrawSubItemText(CDC* pDC, LVITEM* pItem, LVCOLUMN* pColumn, LPRECT pRect)
{
    if (*pItem->pszText) {
        if (IsCheck(dp.nRow, dp.nIndex))
            return;

        int nWidth = pRect->right - pRect->left;

        if (nWidth > 0) {
            CString strLabel(pItem->pszText);

            uMakeShortString(pDC, strLabel, nWidth);
            pDC->DrawText(strLabel,
                          -1,
                          pRect,
                          (pColumn->fmt & LVCFMT_CENTER ? DT_CENTER : pColumn->fmt & LVCFMT_RIGHT ? DT_RIGHT : DT_LEFT) | DT_SINGLELINE |
                            DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Create image list with sort icons
//
void
CListCtrlX::CreateSortIcons()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HIMAGELIST imglist = ImageList_Create(13, 12, ILC_COLOR24 | ILC_MASK, 2, 0);
    ASSERT(imglist);
    if (imglist == NULL)
        return;

    m_ImgListSort.m_hImageList = imglist;

    CBitmap bmp;
    bmp.LoadBitmap(IDB_HDRDOWN);
    m_ImgListSort.Add(&bmp, RGB_BACKGROUND);
    bmp.DeleteObject();
    bmp.LoadBitmap(IDB_HDRUP);
    m_ImgListSort.Add(&bmp, RGB_BACKGROUND);

    /////////////////////////////////////////////////////////
    // OLD VARIANT
    //	// Use the code below in case MFC's function CImageList::Create(...)
    //	// will cause an error.
    //	HIMAGELIST imglist = ImageList_Create(9, 5, ILC_COLOR24 | ILC_MASK, 2, 0);
    //	ASSERT(imglist);
    //	if (imglist == NULL)
    //		return;
    //
    //	m_ImgListSort.m_hImageList = imglist;
    ////	m_ImgListSort.DeleteImageList();
    ////	if (!m_ImgListSort.Create(9, 5, ILC_COLOR24 | ILC_MASK, 2, 0))
    ////		return;
    //
    //	CBitmap bmp;
    //	COLORMAP cm = {RGB(0, 0, 0), GetSysColor(COLOR_GRAYTEXT)}; // black to gray text
    //	bmp.LoadMappedBitmap(IDB_HDRDOWN, 0, &cm, 1);
    //	m_ImgListSort.Add(&bmp, RGB_BACKGROUND);
    //	bmp.DeleteObject();
    //	bmp.LoadMappedBitmap(IDB_HDRUP, 0, &cm, 1);
    //	m_ImgListSort.Add(&bmp, RGB_BACKGROUND);
}

void
CListCtrlX::ColorSortColumn(BOOL bEnable)
{
    if (bEnable == m_bColorSortColumn)
        return;

    m_bColorSortColumn     = bEnable;
    DWORD dwColNormalColor = CHOOSE_COL(m_dwColBkItem, GetSysColor(COLOR_WINDOW));

    if (bEnable) {
        // emulate sort column coloring of Windows XP explorer
        UINT nRed   = GetRValue(dwColNormalColor);
        UINT nGreen = GetGValue(dwColNormalColor);
        UINT nBlue  = GetBValue(dwColNormalColor);

        if (nRed > 240 && nGreen > 240 && nBlue > 240) {
            nRed -= 30;
            nGreen -= 30;
            nBlue -= 30;
        } else {
            if (nRed < 232)
                nRed += nRed / 10;
            else
                nRed = 255;

            if (nGreen < 232)
                nGreen += nGreen / 10;
            else
                nGreen = 255;

            if (nBlue < 232)
                nBlue += nBlue / 10;
            else
                nBlue = 255;
        }
        m_dwColSortColor = RGB(nRed, nGreen, nBlue);
    } else
        m_dwColSortColor = dwColNormalColor;
}

/////////////////////////////////////////////////////////////////////////////
// Add a new column to the listview control
//
int
CListCtrlX::InsertColumn(int nCol, const LVCOLUMN* pColumn)
{
    if (nCol == 0)
        m_nFormatOfSubItem0 = pColumn->fmt;

    nCol = CListCtrl::InsertColumn(nCol, pColumn);
    if (nCol == 0)
        JustifyFirstColumn(pColumn->fmt);

    return nCol;
}

/////////////////////////////////////////////////////////////////////////////
// Rejustify first column of listview control to enable a right-
// justified or centerd first column
//
void
CListCtrlX::JustifyFirstColumn(int nFormat)
{
    if (GetStyle() & LVS_OWNERDRAWFIXED || GetExtendedStyle() & LVS_EX_FULLROWSELECT) {
        CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();

        ASSERT(pHeaderCtrl);
        HDITEM hdrItem;

        hdrItem.mask = HDI_FORMAT;
        if (pHeaderCtrl->GetItem(0, &hdrItem)) {
            hdrItem.fmt = hdrItem.fmt & ~HDF_JUSTIFYMASK | nFormat & HDF_JUSTIFYMASK;
            pHeaderCtrl->SetItem(0, &hdrItem);
        }
    }
}

int
CListCtrlX::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
    if (nCol == 0)
        m_nFormatOfSubItem0 = nFormat;

    nCol = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);

    if (nCol == 0)
        JustifyFirstColumn(nFormat);

    return nCol;
}

BOOL
CListCtrlX::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);

    pDC->FillSolidRect(rc, CHOOSE_COL(m_dwColBkNonItem, GetSysColor(COLOR_BTNFACE)));

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// System colors have been changed
//
void
CListCtrlX::OnSysColorChange()
{
    ASSERT(GetHeaderCtrl());

    // update color of sort icon
    CreateSortIcons();
    EnableSortIcon(m_bSortIconEnabled, m_nSortColumn);

    // force update of column colors
    m_bColorSortColumn = !m_bColorSortColumn;
    ColorSortColumn(!m_bColorSortColumn);
}

BOOL
CListCtrlX::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    BOOL bRet = CListCtrl::OnNotify(wParam, lParam, pResult);

    ASSERT(GetHeaderCtrl());

    NMHEADER* pNMHdr = reinterpret_cast<NMHEADER*>(lParam);

    if (pNMHdr->hdr.hwndFrom == GetHeaderCtrl()->m_hWnd) {
        switch (pNMHdr->hdr.code) {
            // added by Alex Zhondin
            case HDN_ITEMCLICKA:
            case HDN_ITEMCLICKW:
                if (0 == pNMHdr->iButton)
                    SortColumn(pNMHdr->iItem);
                break;

            //		case HDN_ENDTRACKW:
            //		case HDN_ENDTRACKA:
            //		case HDN_ITEMCHANGEDW:
            //		case HDN_ITEMCHANGEDA:
            //			if (m_bColorSortColumn)
            //				InvalidateNonItemArea();
            //			break;

            case HDN_DIVIDERDBLCLICKW:
            case HDN_DIVIDERDBLCLICKA:
                if (m_bKeepLabelLeft &&
                    (pNMHdr->iItem > 0 && IndexToOrder(pNMHdr->iItem) == 0 || pNMHdr->iItem == 0 && IndexToOrder(pNMHdr->iItem) > 0)) {
                    // calculate needed column width
                    int nOptWidth = 0;
                    int nWidth;

                    for (int nItem = GetItemCount(); --nItem >= 0;) {
                        nWidth = GetStringWidth(GetItemText(nItem, pNMHdr->iItem));
                        if (nWidth > nOptWidth)
                            nOptWidth = nWidth;
                    }

                    if (pNMHdr->iItem > 0) {
                        // add space for state icon and small icon
                        CRect rcSubItem;
                        if (GetRealSubItemRect(0, pNMHdr->iItem, LVIR_BOUNDS, rcSubItem)) {
                            CRect rcLabel;
                            if (GetRealSubItemRect(0, pNMHdr->iItem, LVIR_LABEL, rcLabel))
                                nOptWidth += rcLabel.left - rcSubItem.left;
                        }

                        // add left offset
                        nOptWidth += m_nFirstColXOff;
                    } else {
                        // add left offset
                        nOptWidth += m_nNextColXOff;
                    }

                    // add right offset
                    nOptWidth += m_nNextColXOff;

                    SetColumnWidth(pNMHdr->iItem, nOptWidth);
                    return TRUE;
                }
                break;

            default:
                break;
        }
    }

    return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Invalidate client area not covered by list control items
//
void
CListCtrlX::InvalidateNonItemArea()
{
    int nTopIndex = GetTopIndex();

    if (nTopIndex >= 0) {
        // update coloring of sort column
        CRect rectHdrItem;
        ASSERT(GetHeaderCtrl());
        if (GetHeaderCtrl()->GetItemRect(abs(m_nSortColumn) - 1, rectHdrItem)) {
            // erase area above top item
            CRect rectThis;
            GetClientRect(rectThis);

            CRect rectItem;
            GetItemRect(nTopIndex, rectItem, LVIR_BOUNDS);

            CRect rectToBeErased(rectThis.left, rectHdrItem.bottom, rectThis.right, rectItem.top);
            InvalidateRect(rectToBeErased);

            // erase area below bottom item
            GetItemRect(GetItemCount() - 1, rectItem, LVIR_BOUNDS);

            if (rectItem.bottom < rectThis.bottom) {
                rectToBeErased.top    = rectItem.bottom;
                rectToBeErased.bottom = rectThis.bottom;
            }

            InvalidateRect(rectToBeErased);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Small icon always should be kept left
//
BOOL
CListCtrlX::KeepLabelLeft(BOOL bKeepLeft)
{
    BOOL bSuccess = TRUE;

    if (bKeepLeft) {
        if ((GetStyle() & LVS_OWNERDRAWFIXED) == 0) {
            bKeepLeft = FALSE;
            bSuccess  = FALSE;
        }
    }

    m_bKeepLabelLeft = bKeepLeft;
    return bSuccess;
}

void
CListCtrlX::SetRowHeight(int nHeight)
{
    CListCtrl::SetImageList(NULL, LVSIL_SMALL);
    m_ImageList.DeleteImageList();

    int nWidth;
    if (m_pImgList_Small != NULL) {
        IMAGEINFO img_info;
        m_pImgList_Small->GetImageInfo(0, &img_info);
        nWidth = img_info.rcImage.right - img_info.rcImage.left;
    } else
        nWidth = 1;

    m_ImageList.Create(nWidth, nHeight, ILC_COLOR, 0, 0);

    CListCtrl::SetImageList(&m_ImageList, LVSIL_SMALL);
}

/////////////////////////////////////////////////////////////////////////////
// Determines which list view item, if any, is at a specified position
//
int
CListCtrlX::HitTest(CPoint pt, UINT* pFlags, int* pColumn)
{
    LVHITTESTINFO info;
    info.pt = pt;
    CListCtrl::SubItemHitTest(&info);

    if (GetStyle() & LVS_OWNERDRAWFIXED && info.iItem >= 0 && pFlags) {
        if (*pFlags == LVHT_ONITEM) {
            CRect rc;
            if (GetRealSubItemRect(info.iItem, 0, LVIR_LABEL, rc) && rc.PtInRect(pt))
                *pFlags = LVHT_ONITEMLABEL;

            else if (GetRealSubItemRect(info.iItem, 0, LVIR_ICON, rc) && rc.PtInRect(pt))
                *pFlags = LVHT_ONITEMICON;

            else if (GetStateIconRect(info.iItem, rc) && rc.PtInRect(pt))
                *pFlags = LVHT_ONITEMSTATEICON;
        }
    }

    if (pColumn)
        *pColumn = info.iSubItem;

    return info.iItem;
}

void
CListCtrlX::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
    int nCol           = 0;
    int nRow           = HitTest(point, 0, &nCol);
    BOOL bClickTheSame = (nCol == m_nColumnSel) && (GetSelectedCount() == 1) && (GetSel() == nRow);
    BOOL bSel          = SelectColumn(nCol);

    RedrawItems(nRow, nRow);

    if (bSel && bClickTheSame && (nRow != -1)) {
        if (m_rcBtn.PtInRect(point)) {
            if (IsButton(nRow, nCol)) {
                m_bBtnPressed = TRUE;
                m_bBtnPushed  = TRUE;
                SetCapture();
                InvalidateRect(m_rcBtn);
                return;
            }

            if (IsCombo(nRow, nCol)) {
                m_bBtnPressed = TRUE;
                m_bBtnPushed  = TRUE;
                SetCapture();
                InvalidateRect(m_rcBtn);

                if (IsEditable(nRow, nCol))
                    StartCellEdit(nRow, nCol);

                return;
            }
        }

        if (IsEditable(nRow, nCol)) {
            CListCtrl::OnLButtonDown(nFlags, point);
            StartCellEdit(nRow, nCol);
            return;
        }
    }

    CListCtrl::OnLButtonDown(nFlags, point);
}

void
CListCtrlX::OnRButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
    int nCol = 0;
    int nRow = HitTest(point, 0, &nCol);

    SelectColumn(nCol);
    RedrawItems(nRow, nRow);

    CListCtrl::OnRButtonDown(nFlags, point);
}

void
CListCtrlX::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    INT nRow = GetSel();
    switch (nChar) {
        case VK_LEFT:
        case VK_RIGHT:
            if ((GetSelectedCount() <= 1) && (nRow != -1)) {
                int nOrder = IndexToOrder(m_nColumnSel);
                nOrder += (nChar == VK_LEFT) ? -1 : 1;

                if (nOrder < 0)
                    nOrder = 0;

                if (nOrder >= GetHeaderCtrl()->GetItemCount())
                    nOrder = GetHeaderCtrl()->GetItemCount() - 1;

                SelectColumn(OrderToIndex(nOrder));

                EnsureVisibleColumn(m_nColumnSel);
                RedrawItems(nRow, nRow);
                return;
            }
            break;
    }

    CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void
CListCtrlX::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (!(nFlags & (MK_CONTROL | MK_SHIFT))) {
        int nCol = 0;
        int nRow = HitTest(point, 0, &nCol);
        if ((nCol != -1) && (nRow != -1)) {
            if (m_rcBtn.PtInRect(point)) {
                if (IsButton(nRow, nCol))
                    return;

                if (IsCombo(nRow, nCol))
                    return;
            }

            if (IsEditable(nRow, nCol)) {
                CListCtrl::OnLButtonDblClk(nFlags, point);
                StartCellEdit(nRow, nCol);
                return;
            } else
                DoubleClickItem(nRow, nCol);
        }
    }

    CListCtrl::OnLButtonDblClk(nFlags, point);
}

int
CListCtrlX::GetSel(int* nCol)
{
    POSITION pos = GetFirstSelectedItemPosition();
    if (nCol)
        *nCol = -1;

    if (pos) {
        if (nCol)
            *nCol = m_nColumnSel;
        return GetNextSelectedItem(pos);
    } else
        return -1;
}

void
CListCtrlX::EnsureVisibleColumn(int nCol)
{
    if (nCol < 0)
        return;

    // Get the order array to total the column offset.
    CHeaderCtrl* pHeader = GetHeaderCtrl();

    int nColCount = pHeader->GetItemCount();
    ASSERT(nCol < nColCount);
    int* pOrderarray = new int[nColCount];
    Header_GetOrderArray(pHeader->m_hWnd, nColCount, pOrderarray);
    // Get the column offset
    int offset = 0;
    for (int i = 0; pOrderarray[i] != nCol; i++)
        offset += GetColumnWidth(pOrderarray[i]);

    int colwidth = GetColumnWidth(nCol);
    delete[] pOrderarray;

    CRect rect;
    GetItemRect(0, &rect, LVIR_BOUNDS);
    // Now scroll if we need to show the column
    CRect rcClient;
    GetClientRect(&rcClient);

    if (offset + rect.left < 0 || offset + colwidth + rect.left > rcClient.right) {
        CSize size(offset + rect.left, 0);
        Scroll(size);
        InvalidateRect(NULL);
        UpdateWindow();
    }
}

void
CListCtrlX::OnPaint()
{
    if (GetItemCount() == 0) {
        m_bRepaintNonItem = TRUE;
        CPaintDC dc(this);
        CRect clip;
        dc.GetClipBox(&clip);
        //		TRACE("Clip: %d, %d, %d, %d\n", clip.left, clip.top, clip.right, clip.bottom);
        int nSavedDC = dc.SaveDC();

        CRect rc;
        GetWindowRect(&rc);
        ScreenToClient(&rc);

        CHeaderCtrl* pHC;
        pHC = GetHeaderCtrl();

        if (pHC != NULL) {
            CRect rcH;
            pHC->GetItemRect(0, &rcH);
            rc.top += rcH.bottom;
        }
        rc.top += 10;

        dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
        dc.SetBkColor(CHOOSE_COL(m_dwColBkNonItem, GetSysColor(COLOR_BTNFACE)));
        dc.SelectStockObject(ANSI_VAR_FONT);
        dc.DrawText(m_strEmptyMsg, -1, rc, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);

        dc.RestoreDC(nSavedDC);

        // we must hide spin in that case
        if (m_pSpin) {
            if (m_pSpin->IsWindowVisible())
                m_pSpin->ShowWindow(SW_HIDE);
        }
    } else {
        if (m_bRepaintNonItem) {
            m_bRepaintNonItem = FALSE;
            Invalidate(TRUE);
        }

        Default();
    }
}

void
CListCtrlX::OnEndtrack(NMHDR* pNMHDR, LRESULT* pResult)
{
    HD_NOTIFY* phdn = (HD_NOTIFY*)pNMHDR;

    if (GetItemCount() <= 0) {
        CRect rc;
        GetWindowRect(&rc);
        ScreenToClient(&rc);

        InvalidateRect(rc);
        *pResult = 1;
        return;
    }

    Default();
    *pResult = 0;
}

void
CListCtrlX::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    HD_NOTIFY* phdn = (HD_NOTIFY*)pNMHDR;

    if ((GetItemCount() <= 0) && phdn->pitem) {
        if (phdn->pitem->mask & HDI_HEIGHT) {
            Default();
            CRect rc;
            GetWindowRect(&rc);
            ScreenToClient(&rc);

            InvalidateRect(rc);
            *pResult = 1;
            return;
        }
    }

    Default();
    *pResult = 0;
}

CEditX*
CListCtrlX::EditSubItem(int nItem, int nCol, LPCTSTR strInitText)
{
    // Make sure that the item is visible
    if (!EnsureVisible(nItem, TRUE))
        return NULL;

    if (IndexToOrder(nCol) == -1)
        return NULL;

    EnsureVisibleColumn(nCol);

    // Get Column alignment
    LV_COLUMN lvCol;
    lvCol.mask = LVCF_FMT;
    GetColumn(nCol, &lvCol);
    DWORD dwEditStyle;

    if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
        dwEditStyle = ES_LEFT;
    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
        dwEditStyle = ES_RIGHT;
    else
        dwEditStyle = ES_CENTER;

    CRect rcEdit;
    GetRealSubItemRect(nItem, nCol, LVIR_BOUNDS, rcEdit);

    dwEditStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
    EditStyle(nItem, nCol, dwEditStyle);

    BOOL bColNum = FALSE;
    if (dwEditStyle & ES_COL_NUMBER) {
        dwEditStyle &= ~ES_COL_NUMBER;
        bColNum = TRUE;
    }

    DestroyEdit();
    m_pEdit = new CListCtrlXEdit(this, nItem, nCol, strInitText ? strInitText : GetItemText(nItem, nCol), bColNum);
    m_pEdit->Create(dwEditStyle, rcEdit, this, IDC_EDITCELL);
    m_pEdit->SetLimitText(EditLimit(nItem, nCol));

    return m_pEdit;
}

void
CListCtrlX::DestroyEdit()
{
    SAFE_DELETE(m_pEdit);
}

CListCtrlXEdit*
CListCtrlX::GetEditCtrl()
{
    return m_pEdit;
}

void
CListCtrlX::DrawCellRect(CDC* pDC, CRect& rc, CRect& rcText, BOOL bSelected)
{
    BOOL bDrawn     = FALSE;
    BOOL bClearSpin = TRUE;
    if (dp.bDrawCell) {
        bDrawn = TRUE;

        if (IsButton(dp.nRow, dp.nIndex))
            DrawBtn(pDC, rc, rcText, bSelected);
        else if (IsCombo(dp.nRow, dp.nIndex))
            DrawCombo(pDC, rc, rcText, bSelected);
        else if (IsSpin(dp.nRow, dp.nIndex)) {
            DrawSpin(pDC, rc, rcText, bSelected);
            bClearSpin = FALSE;
        } else
            bDrawn = FALSE;
    }

    if (IsCheck(dp.nRow, dp.nIndex)) {
        DrawCheck(pDC, rc, rcText, bSelected);
        bDrawn = TRUE;
    }

    if (m_pSpin && bClearSpin) {
        if (m_pSpin->IsWindowVisible() && (dp.nRow == m_nBtnRow && dp.nColumn == m_nBtnCol))
            m_pSpin->ShowWindow(SW_HIDE);
    }

    if (!bDrawn)
        FillCellRect(pDC, rc, bSelected ? (dp.bDrawCell ? dp.dwHiLiteBkCell : dp.dwHiLiteBk) : dp.dwBkColor);
}

void
CListCtrlX::DrawCellGrid(CDC* pDC, CRect& rc)
{
    CPen* prevPen = pDC->SelectObject(&dp.penGrid);
    pDC->MoveTo(rc.left, rc.bottom - 1);
    pDC->LineTo(rc.right - 1, rc.bottom - 1);
    pDC->LineTo(rc.right - 1, rc.top - 1);
    pDC->SelectObject(prevPen);
}

/////////////////////////////////////////////////////////////////////////////
// Draw '...' button
//
void
CListCtrlX::DrawBrowse(CDC* pDC, CRect& rc)
{
    CRect rcBtn(rc);
    rcBtn.left = rcBtn.right - rcBtn.Height();

    rcBtn.DeflateRect(1, 1);
    pDC->DrawFrameControl(rcBtn, DFC_BUTTON, DFCS_BUTTONPUSH);
    pDC->DrawText("...", 3, rcBtn, DT_CENTER | DT_VCENTER);
}

/////////////////////////////////////////////////////////////////////////////
// Draw check box
//
void
CListCtrlX::DrawCheckBox(CDC* pDC, CRect& rc, DWORD dwStyle)
{
    CRect rcBtn(rc);
    rcBtn.left  = ((rc.left + rc.right) >> 1) - (rc.Height() >> 1);
    rcBtn.right = ((rc.left + rc.right) >> 1) + (rc.Height() >> 1);

    rcBtn.DeflateRect(1, 1);
    pDC->DrawFrameControl(rcBtn, DFC_BUTTON, DFCS_BUTTONCHECK | dwStyle);
}

/////////////////////////////////////////////////////////////////////////////
// Deselect all list control items
//
void
CListCtrlX::DeselectAll()
{
    int nItems = GetItemCount();
    for (int i = 0; i < nItems; i++)
        SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
}

void
CListCtrlX::PreSubclassWindow()
{
    SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_TRACKSELECT);
    SetHoverTime(0x00f00000);

    CListCtrl::PreSubclassWindow();
}

void
CListCtrlX::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

    *pResult = 1;
}

BOOL
CListCtrlX::SelectColumn(int nCol)
{
    m_nColumnSel = nCol;
    return TRUE;
}

void
CListCtrlX::StartCellEdit(int nRow, int nCol)
{
    int nItems = GetItemCount();
    for (int i = 0; i < nItems; i++)
        SetItemState(i, (nRow == i) ? (LVIS_SELECTED | LVIS_FOCUSED) : 0, LVIS_SELECTED | LVIS_FOCUSED);

    SelectColumn(nCol);

    if (IsCombo(nRow, nCol)) {
        CreateComboList(nRow, nCol);
        return;
    }

    if (IsCheck(nRow, nCol)) {
        SetListText(nRow, nCol, GetDlgCtrlID(), (GetItemText(nRow, nCol).GetLength() == 0) ? "X" : "");
        return;
    }

    EditSubItem(nRow, nCol);
}

void
CListCtrlX::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar) {
        case VK_RETURN:
            if ((GetSelectedCount() == 1) && (GetSel() >= 0) && m_nColumnSel >= 0 && IsEditable(GetSel(), m_nColumnSel))
                StartCellEdit(GetSel(), m_nColumnSel);
            break;
        default:
            CListCtrl::OnChar(nChar, nRepCnt, nFlags);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Select all list control items
//
void
CListCtrlX::SelectAll()
{
    int nTotal = GetItemCount();
    if (nTotal > 0) {
        for (int i = 0; i < nTotal; i++)
            SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

        SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
}

BOOL
CListCtrlX::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
            case 'A':
                if (GetKeyState(VK_CONTROL) < 0) {
                    SelectAll();
                    return TRUE;
                }

                //		case VK_F5:
                //			Invalidate();
                //			break;
        }
    }
    /*    if (pMsg->message == WM_KEYDOWN)
    {
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);

	    if ((pMsg->wParam != VK_UP) ||
			(pMsg->wParam != VK_DOWN) ||
			(pMsg->wParam == VK_RETURN))
			return TRUE;		    	// DO NOT process further
    }
*/
    return CListCtrl::PreTranslateMessage(pMsg);
}

BOOL
CListCtrlX::IsEditable(int nRow, int nCol)
{
    return TRUE;
}

void
CListCtrlX::SelectItem(int nItem, int nSubItem)
{
    int nItems = GetItemCount();

    for (int i = 0; i < nItems; i++)
        SetItemState(i, (nItem == i) ? (LVIS_SELECTED | LVIS_FOCUSED) : 0, LVIS_SELECTED | LVIS_FOCUSED);

    if (nSubItem != -1)
        SelectColumn(nSubItem);

    RedrawItems(nItem, nItem);
}

int
CListCtrlX::GetSelColumn()
{
    return m_nColumnSel;
}

int
CListCtrlX::FindItem(LPARAM lParam)
{
    LVFINDINFO lvfi;

    lvfi.flags       = LVFI_PARAM;
    lvfi.lParam      = lParam;
    lvfi.vkDirection = VK_DOWN;

    return CListCtrl::FindItem(&lvfi);
}

int
CListCtrlX::FindItem(CString sFind, int nCol /*=1*/)
{
    if (sFind.IsEmpty())
        return -1;

    int nCount = GetItemCount();

    if (nCount < 1)
        return -1;

    CString sItem;
    int nItem = -1;
    int nRes;

    for (int i = 0; i < nCount; i++) {
        sItem = GetItemText(i, nCol);

        if (sItem.GetLength() > sFind.GetLength())
            sItem = sItem.Left(sFind.GetLength());

        nRes = sItem.CompareNoCase(sFind);

        //		if (nRes > 0)
        //			nItem = i;
        /*		else*/ if (nRes == 0) {
            nItem = i;
            break;
        }
    }

    if (nItem > -1 && (nItem != GetSel() || nCol != m_nColumnSel)) {
        SelectItem(nItem, nCol);
        EnsureVisible(nItem, FALSE);
        return nItem;
    }

    return -1;
}

void
CListCtrlX::EditStyle(int nRow, int nCol, DWORD& dwStyle)
{
}

UINT
CListCtrlX::EditLimit(int nRow, int nCol)
{
    return 0;
}

BOOL
CListCtrlX::SetListText(int nRow, int nCol, int dlgCtrlId, CString strText)
{
    CString Text;
    GetWindowText(Text);

    // Send Notification to parent of ListView ctrl
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom    = GetSafeHwnd();
    dispinfo.hdr.idFrom      = dlgCtrlId;
    dispinfo.hdr.code        = LVN_ENDLABELEDIT;
    dispinfo.item.mask       = LVIF_TEXT;
    dispinfo.item.iItem      = nRow;
    dispinfo.item.iSubItem   = nCol;
    dispinfo.item.pszText    = (LPTSTR)(LPCTSTR)strText;
    dispinfo.item.cchTextMax = strText.GetLength();

    m_nError  = GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo);
    BOOL bRes = m_nError == 0 ? TRUE : FALSE; //!GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo);

    if (!bRes && !m_bWarning)
        m_bWarning = TRUE;

    return bRes; //!GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo);
}

int
CListCtrlX::Get1stSelectedItem()
{
    POSITION pos = GetFirstSelectedItemPosition();
    return (pos != NULL) ? GetNextSelectedItem(pos) : -1;
}

LPCTSTR
CListCtrlX::RegKey()
{
    return "";
}

void
CListCtrlX::SaveColumns()
{
    CHeaderCtrl* pHeader = GetHeaderCtrl();
    CString strCol;

    for (int i = 0; i < pHeader->GetItemCount(); i++) {
        strCol.Format("Col%d", i);
        AfxGetApp()->WriteProfileInt(RegKey(), strCol, GetColumnWidth(i));
    }
}

void
CListCtrlX::LoadColumns(int nColumns, ...)
{
    CHeaderCtrl* pHeader = GetHeaderCtrl();
    va_list argList;
    va_start(argList, nColumns);
    CString strCol;
    int width;

    for (int i = 0; i < pHeader->GetItemCount(); i++) {
        if (i >= nColumns)
            break;

        strCol.Format("Col%d", i);
        width = va_arg(argList, int);

        width = AfxGetApp()->GetProfileInt(RegKey(), strCol, width);
        if (GetColumnWidth(i) != width)
            SetColumnWidth(i, width);
    }
    va_end(argList);
}

void
CListCtrlX::DrawSpin(CDC* pDC, CRect& rc, CRect& rcText, BOOL bSelected)
{
    m_rcBtn      = rc;
    m_rcBtn.left = m_rcBtn.right - m_rcBtn.Height();

    rcText.right = rcText.right - m_rcBtn.Width();
    CRect rcDraw(rc);
    rcDraw.right = m_rcBtn.left;
    FillCellRect(pDC, rc, bSelected ? (dp.bDrawCell ? dp.dwHiLiteBkCell : dp.dwHiLiteBk) : dp.dwBkColor);

    m_nBtnRow = dp.nRow;
    m_nBtnCol = dp.nColumn;

    if (m_pSpin == NULL) {
        m_pSpin = new CSpinButtonCtrl;

        CRect rc(10, 10, 20, 20);

        if (!m_pSpin->Create(WS_CHILD | WS_BORDER, rc, this, IDC_LISTTAB_SPINCELL))
            ASSERT(FALSE);
    }

    m_pSpin->MoveWindow(m_rcBtn);
    m_pSpin->ShowWindow(SW_SHOW);
}

void
CListCtrlX::DrawBtn(CDC* pDC, CRect& rc, CRect& rcText, BOOL bSelected)
{
    m_rcBtn      = rc;
    m_rcBtn.left = m_rcBtn.right - m_rcBtn.Height();

    rcText.right = rcText.right - m_rcBtn.Width();
    CRect rcDraw(rc);
    rcDraw.right = m_rcBtn.left;
    FillCellRect(pDC, rc, bSelected ? (dp.bDrawCell ? dp.dwHiLiteBkCell : dp.dwHiLiteBk) : dp.dwBkColor);

    m_nBtnRow = dp.nIndex;
    m_nBtnCol = dp.nColumn;

    CRect rcBtn(m_rcBtn);
    rcBtn.DeflateRect(1, 1);
    pDC->DrawFrameControl(rcBtn, DFC_BUTTON, DFCS_BUTTONPUSH | (m_bBtnPushed ? DFCS_PUSHED : 0));
    COLORREF col = pDC->GetTextColor();
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->DrawText("...", 3, rcBtn, DT_CENTER | DT_VCENTER);
    pDC->SetTextColor(col);
}

void
CListCtrlX::DrawCombo(CDC* pDC, CRect& rc, CRect& rcText, BOOL bSelected)
{
    m_rcBtn      = rc;
    m_rcBtn.left = m_rcBtn.right - m_rcBtn.Height();

    rcText.right = rcText.right - m_rcBtn.Width();
    CRect rcDraw(rc);
    rcDraw.right = m_rcBtn.left;
    FillCellRect(pDC, rc, bSelected ? (dp.bDrawCell ? dp.dwHiLiteBkCell : dp.dwHiLiteBk) : dp.dwBkColor);

    m_nBtnRow = dp.nRow;
    m_nBtnCol = dp.nColumn;

    CRect rcBtn(m_rcBtn);
    rcBtn.DeflateRect(1, 1);
    pDC->DrawFrameControl(rcBtn, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | (m_bBtnPushed ? DFCS_PUSHED : 0));
}

void
CListCtrlX::DrawCheck(CDC* pDC, CRect& rc, CRect& rcText, BOOL bSelected)
{
    FillCellRect(pDC, rc, bSelected ? (dp.bDrawCell ? dp.dwHiLiteBkCell : dp.dwHiLiteBk) : dp.dwBkColor);
    DrawCheckBox(pDC, rc, DFCS_FLAT | ((lstrlen(dp.pSubItem->pszText) > 0) ? DFCS_CHECKED : 0));
}

BOOL
CListCtrlX::IsButton(int nRow, int nCol)
{
    return FALSE;
}

BOOL
CListCtrlX::IsCombo(int nRow, int nCol)
{
    return FALSE;
}

BOOL
CListCtrlX::IsCheck(int nRow, int nCol)
{
    return FALSE;
}

BOOL
CListCtrlX::IsSpin(int nRow, int nCol)
{
    return FALSE;
}

BOOL
CListCtrlX::IsDragEnabled()
{
    return m_bDragEnable;
}

void
CListCtrlX::EnableDrag(BOOL bEnable /*= TRUE*/)
{
    m_bDragEnable = bEnable;
}

///////////////////////////////////////////////////////////////////////////
// Virtual function, should be overloaded in case you want to enable/disable dragging
//
BOOL
CListCtrlX::IsDraggingAllowed()
{
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// This routine sets the parameters for a Drag and Drop operation.
// It sets some variables to track the Drag/Drop as well
// as creating the drag image to be shown during the drag.
//
void
CListCtrlX::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // Note: we can drag only if selected a first row
    if ((m_bDragEnable && IsDraggingAllowed()) && pNMListView->iSubItem == 0) {
        // Save the index of the item being dragged in m_nDragIndex
        // This will be used later for retrieving the info dragged
        m_nDragIndex = pNMListView->iItem;

        // Create a drag image
        POINT pt;
        int nOffset = -10; // offset in pixels for drag image (positive is up and to the left; neg is down and to the right)

        if (GetSelectedCount() > 1) // more than one item is selected
            pt.x = nOffset;
        pt.y     = nOffset;

        ASSERT(m_pImgListDrag == NULL);
        m_pImgListDrag = new CImageList;

        if (!m_pImgListDrag->Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 0))
            return;

        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        m_pImgListDrag->Add(AfxGetApp()->LoadIcon(GetSelectedCount() > 1 ? IDI_DRAG_MULTI : IDI_DRAG_ONE));

        // Change the cursor to the drag image
        // (still must perform DragMove() in OnMouseMove() to show it moving)
        m_pImgListDrag->BeginDrag(0, CPoint(nOffset, nOffset - 4));
        m_pImgListDrag->DragEnter(GetDesktopWindow(), pNMListView->ptAction);

        // Set dragging flag and others
        m_bDragging  = TRUE; // we are in a drag and drop operation
        m_nDropIndex = -1;   // we don't have a drop index yet

        // Capture all mouse messages
        SetCapture();
    } //if (m_bDragEnable)

    *pResult = 0;
}

void
CListCtrlX::MouseScroll(CPoint point)
{
    CRect client, rc;

    GetClientRect(client);
    GetHeaderCtrl()->GetItemRect(0, rc);
    client.top += rc.Height();

    if (client.PtInRect(point))
        return;

    if (point.x < client.left)
        PostMessage(WM_HSCROLL, SB_LINELEFT);
    else if (point.x > client.right)
        PostMessage(WM_HSCROLL, SB_LINERIGHT);

    BOOL bRedraw = FALSE;

    if (point.y < client.top) {
        PostMessage(WM_VSCROLL, SB_LINEUP);
        bRedraw = TRUE;
    } else if (point.y > client.bottom) {
        PostMessage(WM_VSCROLL, SB_LINEDOWN);
        bRedraw = TRUE;
    }

    if (bRedraw) {
        int nItem = HitTest(point);
        if (nItem > 0)
            RedrawItems(nItem, nItem < GetItemCount() - 3 ? nItem + 3 : nItem);
    }
}

/////////////////////////////////////////////////////////////////////////
// While the mouse is moving, this routine is called.
// This routine will redraw the drag image at the present
// mouse location to display the dragging.
// Also, while over a CListCtrlX, this routine will highlight
// the item we are hovering over.
//
void
CListCtrlX::OnMouseMove(UINT nFlags, CPoint point)
{
    // If we are in a drag/drop procedure (m_bDragging is true)
    if (m_bDragging) {
        MouseScroll(point);

        ASSERT(m_pImgListDrag != NULL);

        // Move the drag image
        CPoint pt(point);    // get our current mouse coordinates
        ClientToScreen(&pt); // convert to screen coordinates

        m_pImgListDrag->DragMove(pt);          // move the drag image to those coordinates
        m_pImgListDrag->DragShowNolock(FALSE); // unlock window updates (this allows the dragging image to be shown smoothly)

        // If we drag outside current window we need to adjust the highlights displayed
        if (m_nDropIndex != -1)
            VERIFY(SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED));

        // If we are hovering over a CListCtrlX we need to adjust the highlights
        // Get the CWnd pointer of the window that is under the mouse cursor
        CWnd* pDropWnd = WindowFromPoint(pt);

        if (pDropWnd->GetSafeHwnd() && (pDropWnd->m_hWnd == this->m_hWnd)) {
            // Note that we can drop here
            SetCursor(LoadCursor(NULL, IDC_ARROW));

            // Get the item that is below cursor
            ScreenToClient(&pt);
            m_nDropIndex = HitTest(pt);

            // Highlight it
            SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
        } else {
            // If we are not hovering over a CListCtrlX, change the cursor
            // to note that we cannot drop here
            SetCursor(LoadCursor(NULL, IDC_NO));
            m_nDropIndex = -1;
        }

        if (m_nDropIndex == -1) //If we drag out of the CListCtrl altogether
        {
            int nCount = GetItemCount();
            for (int i = 0; i < nCount; i++)
                SetItemState(i, 0, LVIS_DROPHILITED);
        }

        // Lock window updates
        m_pImgListDrag->DragShowNolock(TRUE);
        return;
    } //if (m_bDragging)

    /////////////////////////////////////////////////////////////
    CListCtrl::OnMouseMove(nFlags, point);

    if (m_bBtnPressed) {
        BOOL bPush = m_rcBtn.PtInRect(point);
        if ((bPush && !m_bBtnPushed) || (!bPush && m_bBtnPushed)) {
            m_bBtnPushed = bPush;
            InvalidateRect(m_rcBtn);
        }
    }
}

/////////////////////////////////////////////////////////////////////////
// This routine is the end of the drag/drop operation.
// When the button is released, we are to drop the item.
// There are a few things we need to do to clean up and
// finalize the drop:
//	1) Release the mouse capture
//	2) Set m_bDragging to false to signify we are not dragging
//	3) Actually drop the item (we call a separate function to do that)
//
void
CListCtrlX::OnLButtonUp(UINT nFlags, CPoint point)
{
    // If we are in a drag and drop operation (otherwise we don't do anything)
    if (m_bDragging) {
        SortColumn(m_nSortColumn);
        SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED);

        // Release mouse capture, so that other controls can get control/messages
        ReleaseCapture();

        // Note that we are NOT in a drag operation
        m_bDragging = FALSE;

        // End dragging image
        ASSERT(m_pImgListDrag != NULL);
        m_pImgListDrag->DragLeave(GetDesktopWindow());
        m_pImgListDrag->EndDrag();
        SAFE_DELETE(m_pImgListDrag);

        CRect rc;
        GetClientRect(rc);

        // If window is the same CListCtrlX, we perform the drop
        if (rc.PtInRect(point))
            DropItemOnList(); // Call routine to perform the actual drop
        else
            MessageBeep(MB_ICONEXCLAMATION);
        /*		
		CPoint pt(point);    // Get current mouse coordinates
		ClientToScreen(&pt); // Convert to screen coordinates

		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint(pt);
//		ASSERT(pDropWnd); // make sure we have a window pointer
		
		// If window is the same CListCtrlX, we perform the drop
		if (pDropWnd->GetSafeHwnd() && (pDropWnd->m_hWnd == this->m_hWnd))
			DropItemOnList(); // Call routine to perform the actual drop
		else
			MessageBeep(MB_ICONEXCLAMATION);
*/
    }

    ///////////////////////////////////////////////////////////////////
    if (m_bBtnPressed) {
        BOOL bPush    = m_bBtnPushed;
        m_bBtnPressed = FALSE;
        m_bBtnPushed  = FALSE;
        ReleaseCapture();
        InvalidateRect(m_rcBtn);

        if (IsButton(m_nBtnRow, m_nBtnCol) && bPush)
            OnButtonPressed();

        return;
    }

    CListCtrl::OnLButtonUp(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////
// This routine performs the actual drop of the item dragged.
// Function send LIST_REARRANGE message to the parent window
// with pointer to the CDWordArray as LPARAM.
//
// The sample function is placed below:
// This routine performs the actual drop after the item dragged.
// Function get LIST_REARRANGE message from the list control window
// with pointer to the CDWordArray as LPARAM.
//
//afx_msg LRESULT OnRearrange(WPARAM wParam, LPARAM lParam);
//ON_MESSAGE(LIST_REARRANGE, OnRearrange)
//LRESULT CBldgsView::OnRearrange(WPARAM wParam, LPARAM lParam)
//{
//	if ((HWND)wParam != m_Table.m_hWnd)
//		return -1;
//
//	CDWordArray *pData = (CDWordArray*)lParam;
//	if (pData)
//	{
//		// Do something with data...
//		return 0;
//	}
//
//	return -1;
//}
BOOL
CListCtrlX::DropItemOnList()
{
    if (!GetSafeHwnd())
        return FALSE;

    // We should get the parent
    CWnd* pWnd = GetParent();
    if (!pWnd || !pWnd->GetSafeHwnd())
        return FALSE;

    CWaitCursor wait;

    int i;
    int nItem  = 0;
    int nCount = GetItemCount();
    CDWordArray arData;
    DWORD dwDataCur;
    DWORD dwDropData;
    BOOL bRes = FALSE;

    if (GetSelectedCount() == 1) {
        // in case drop target is the same item
        if (m_nDragIndex == m_nDropIndex)
            return FALSE;

        DWORD dwDragData = GetItemData(m_nDragIndex);
        dwDropData       = GetItemData(m_nDropIndex == -1 ? nCount - 1 : m_nDropIndex);

        for (i = 0; i < nCount; i++) {
            dwDataCur = GetItemData(i);

            if (dwDataCur == dwDropData) {
                if (m_nDropIndex == -1) {
                    arData.Add(dwDataCur);
                    arData.Add(dwDragData);
                    nItem = nCount - 1;
                } else {
                    arData.Add(dwDragData);
                    nItem = arData.GetSize() - 1;
                    arData.Add(dwDataCur);
                }
            } else if (dwDataCur != dwDragData)
                arData.Add(dwDataCur);
        }

        // At last, sent message to the parent
        bRes = (::SendMessage(pWnd->m_hWnd, LIST_REARRANGE, (WPARAM)this->m_hWnd, (LPARAM)&arData) >= 0);

        SelectItem(nItem);
        return bRes;

        ///////////////////////////////////////////////////////////////
        // OLD VARIANT
        //		// Get item that was dragged
        //		GetItem(lvi.plvi);
        //		lvi.dwData = GetItemData(lvi.plvi->iItem);
        //
        //		// Delete the original item (for Move operation)
        //		// This is optional. If you want to implement a Copy operation, don't delete.
        //		// This works very well though for re-arranging items within a CListCtrlX.
        //		// It is written at present such that when dragging from one list to the other
        //		// the item is copied, but if dragging within one list, the item is moved.
        //		DeleteItem(m_nDragIndex);
        //		// decrement drop index to account for item
        //		// being deleted above it
        //		if (m_nDragIndex < m_nDropIndex)
        //			m_nDropIndex--;
        //
        //		// Insert item into pDropList
        //		// if m_nDropIndex == -1, iItem = GetItemCount() (inserts at end of list), else iItem = m_nDropIndex
        //		lvi.plvi->iItem = (m_nDropIndex == -1) ? GetItemCount() : m_nDropIndex;
        //		InsertItem (lvi.plvi);
        //		SetItemData(lvi.plvi->iItem, lvi.dwData);
        //
        //		// Select the new item we just inserted
        //
        //		SelectItem(lvi.plvi->iItem);
        ////		SetItemState(lvi.plvi->iItem, LVIS_SELECTED, LVIS_SELECTED);
    } else // more than 1 item is being dropped
    {
        CDWordArray arDrag;
        int nDropIndex = m_nDropIndex;

        // Retrieve the selected items and get data
        POSITION pos = GetFirstSelectedItemPosition(); // iterator for the CListCtrlX
        while (pos)                                    // so long as we have a valid POSITION, we keep iterating
        {
            m_nDragIndex = GetNextSelectedItem(pos);
            arDrag.Add(GetItemData(m_nDragIndex));
        } //while (pos) -- at this point we have deleted the moving items and stored them in memory

        // Delete the selected items
        pos = GetFirstSelectedItemPosition();
        while (pos) {
            pos          = GetFirstSelectedItemPosition();
            m_nDragIndex = GetNextSelectedItem(pos);

            DeleteItem(m_nDragIndex); // since we are MOVING, delete the item
            if (m_nDragIndex < nDropIndex)
                nDropIndex--; // must decrement the drop index to account
                              // for the deleted items
        }                     //while (pos)

        nCount     = GetItemCount();
        nDropIndex = (m_nDropIndex == -1) ? nCount - 1 : nDropIndex;

        dwDropData = GetItemData(nDropIndex);

        // Retreave data of the all items
        for (i = 0; i < nCount; i++) {
            dwDataCur = GetItemData(i);
            SetItemState(i, 0, LVIS_FOCUSED);

            if (dwDataCur == dwDropData) {
                if (m_nDropIndex == -1) {
                    nItem = i + 1;
                    arData.Add(dwDataCur);
                    for (int j = 0; j < arDrag.GetSize(); j++)
                        arData.Add(arDrag.GetAt(j));
                } else {
                    nItem = i;
                    for (int j = 0; j < arDrag.GetSize(); j++)
                        arData.Add(arDrag.GetAt(j));
                    arData.Add(dwDataCur);
                }
            } else
                arData.Add(dwDataCur);
        }

        // At last, sent message to the parent
        bRes = (::SendMessage(pWnd->m_hWnd, LIST_REARRANGE, (WPARAM)this->m_hWnd, (LPARAM)&arData) >= 0);

        SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
        for (i = 0; i < arDrag.GetSize(); i++)
            SetItemState(nItem + i, LVIS_SELECTED, LVIS_SELECTED);
        return bRes;

        ///////////////////////////////////////////////////////////////
        // OLD VARIANT
        //		// Set up the LV_ITEM for retrieving item from pDragList and adding the new item to the pDropList
        //		char szLabel[256];
        //		LVITEM lviT;
        //
        //		ZeroMemory(&lviT, sizeof (LVITEM)); // allocate and clear memory space for LV_ITEM
        //		lviT.iItem				= m_nDragIndex;
        //		lviT.mask				= LVIF_TEXT;
        //		lviT.pszText			= szLabel;
        //		lviT.cchTextMax			= 255;
        //
        //		LDRAGITEM* pItem;
        //		LDRAGITEM lvi;
        //		lvi.plvi				= &lviT;
        //		lvi.plvi->iItem			= m_nDragIndex;
        //		lvi.plvi->mask			= LVIF_TEXT;
        //		lvi.plvi->pszText		= szLabel;
        //		lvi.plvi->cchTextMax	= 255;
        //
        //		// We have to parse through all of the selected items from the DragList
        //		// 1) Retrieve the info for the items and store them in memory
        //		// 2) If we are reordering, delete the items from the list
        //		// 3) Insert the items into the list (either same list or different list)
        //
        //		LVITEM* PLDRAGITEM;
        //		CList<LDRAGITEM*, LDRAGITEM*> listItems;
        //		POSITION listPos;
        //
        //		// Retrieve the selected items
        //		POSITION pos = GetFirstSelectedItemPosition(); // iterator for the CListCtrlX
        //		while (pos) // so long as we have a valid POSITION, we keep iterating
        //		{
        //			PLDRAGITEM = new LVITEM;
        //			ZeroMemory(PLDRAGITEM, sizeof(LVITEM));
        //			pItem                   = new LDRAGITEM;
        //			// ZeroMemory(pItem, sizeof(LDRAGITEM)); // If you use ZeroMemory on the LDRAGITEM struct, it creates an error when you try to set dwData
        //			pItem->plvi             = PLDRAGITEM;
        //			pItem->plvi->iItem      = m_nDragIndex;
        //			pItem->plvi->mask       = LVIF_TEXT;
        //			pItem->plvi->pszText    = new char; // since this is a pointer to the string, we need a new pointer to a new string on the heap
        //			pItem->plvi->cchTextMax = 255;
        //
        //			m_nDragIndex = GetNextSelectedItem(pos);
        //
        //			// Get the item
        //			pItem->plvi->iItem = m_nDragIndex; // set the index in the drag list to the selected item
        //			GetItem(pItem->plvi);   // retrieve the information
        //			pItem->dwData = GetItemData(pItem->plvi->iItem);
        //
        //			// Save the pointer to the new item in our CList
        //			listItems.AddTail(pItem);
        //		} //while (pos) -- at this point we have deleted the moving items and stored them in memory
        //
        //		// Delete the selected items
        //		pos = GetFirstSelectedItemPosition();
        //		while (pos)
        //		{
        //			pos = GetFirstSelectedItemPosition();
        //			m_nDragIndex = GetNextSelectedItem(pos);
        //
        //			DeleteItem(m_nDragIndex); // since we are MOVING, delete the item
        //			if (m_nDragIndex < m_nDropIndex)
        //				m_nDropIndex--; // must decrement the drop index to account
        //			// for the deleted items
        //		} //while (pos)
        //
        //		// Iterate through the items stored in memory and add them back into the CListCtrlX at the drop index
        //		listPos = listItems.GetHeadPosition();
        //		while (listPos)
        //		{
        //			pItem = listItems.GetNext(listPos);
        //
        //			m_nDropIndex = (m_nDropIndex == -1) ? GetItemCount() : m_nDropIndex;
        //			pItem->plvi->iItem = m_nDropIndex;
        //			InsertItem(pItem->plvi); //add the item
        //			SetItemData(pItem->plvi->iItem, pItem->dwData);
        //
        //			SetItemState(pItem->plvi->iItem, LVIS_SELECTED, LVIS_SELECTED); //highlight/select the item we just added
        //
        //			m_nDropIndex++; // increment the index we are dropping at to keep the dropped items in the same order they were in in the Drag List
        //			// If we dont' increment this, the items are added in reverse order
        //		} //while (listPos)
        //
        //		pos = GetFirstSelectedItemPosition();
        //		int nItem  = GetNextSelectedItem(pos);
        //		for (i = 0; i < GetItemCount(); i++)
        //			SetItemState(i, (nItem == i) ? LVIS_FOCUSED : 0, LVIS_FOCUSED);
        //
        //		RedrawItems(nItem, nItem);
        //
        //		// Retreave data of the all items
        //		for (i = 0; i < nCount; i++)
        //			arData.Add(GetItemData(i));
    }

    return bRes;
    // At last, sent message to the parent
    //	return (::SendMessage(pWnd->m_hWnd, LIST_REARRANGE, (WPARAM)this->m_hWnd, (LPARAM)&arData) >= 0);
}

void
CListCtrlX::OnButtonPressed()
{
}

void
CListCtrlX::CreateComboList(int nRow, int nCol)
{
    if (m_pCombo == NULL)
        m_pCombo = new CListCtrlXCombo(this, nRow, nCol, GetItemText(nRow, nCol));
    else
        m_pCombo->Init(nRow, nCol, GetItemText(nRow, nCol));

    CRect rc;
    GetRealSubItemRect(nRow, nCol, LVIR_BOUNDS, rc);

    rc.bottom += 120;

    if (m_pCombo->GetSafeHwnd() == NULL)
        m_pCombo->Create(WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_HSCROLL, rc, this, IDC_LISTTAB_COMBOCELL);
    else
        m_pCombo->MoveWindow(rc);

    m_pCombo->SetFont(GetFont());

    m_pCombo->ResetContent();
    PopulateCombo(nRow, nCol, m_pCombo);

    m_pCombo->SelectString(-1, GetItemText(nRow, nCol));

    m_pCombo->ShowDropDown();

    m_pCombo->SetFocus();
}

void
CListCtrlX::PopulateCombo(int nRow, int nCol, CListCtrlXCombo* pCombo)
{
    pCombo->ResetContent();
}

void
CListCtrlX::OnCaptureChanged(CWnd* pWnd)
{
    if (pWnd != this && m_bBtnPressed) {
        BOOL bPush    = m_bBtnPushed;
        m_bBtnPressed = FALSE;
        m_bBtnPushed  = FALSE;
        InvalidateRect(m_rcBtn);
    }

    CListCtrl::OnCaptureChanged(pWnd);
}

void
CListCtrlX::SetEmptyMsg(LPCTSTR strMsg)
{
    m_strEmptyMsg = strMsg;
}

CImageList*
CListCtrlX::SetImageList(CImageList* pImageList, int nImageList)
{
    CImageList* pImgList = NULL;

    switch (nImageList) {
        case LVSIL_NORMAL:
            pImgList          = m_pImgList_Normal;
            m_pImgList_Normal = pImageList;
            break;
        case LVSIL_SMALL:
            pImgList         = m_pImgList_Small;
            m_pImgList_Small = pImageList;
            break;
        case LVSIL_STATE:
            pImgList         = m_pImgList_State;
            m_pImgList_State = pImageList;
            break;
    }

    if (pImageList != NULL) {
        IMAGEINFO img_info;
        pImageList->GetImageInfo(0, &img_info);

        CListCtrl::SetImageList(NULL, nImageList);
        m_ImageList.DeleteImageList();

        m_ImageList.Create(img_info.rcImage.right - img_info.rcImage.left, img_info.rcImage.bottom - img_info.rcImage.top, ILC_COLOR, 0, 0);

        CListCtrl::SetImageList(&m_ImageList, nImageList);
    } else
        CListCtrl::SetImageList(NULL, nImageList);

    return pImgList;
}

CImageList*
CListCtrlX::GetImageList(int nImageList) const
{
    CImageList* pImgList = NULL;
    switch (nImageList) {
        case LVSIL_NORMAL:
            pImgList = m_pImgList_Normal;
            break;
        case LVSIL_SMALL:
            pImgList = m_pImgList_Small;
            break;
        case LVSIL_STATE:
            pImgList = m_pImgList_State;
            break;
    }

    return pImgList;
}

BOOL
CListCtrlX::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
    if (GetItemText(nItem, nSubItem).Compare(lpszText) != 0)
        return CListCtrl::SetItemText(nItem, nSubItem, lpszText);

    return FALSE;
}

void
CListCtrlX::FillCellRect(CDC* pDC, CRect& rc, COLORREF col)
{
    pDC->FillSolidRect(rc, col);
}

void
CListCtrlX::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    //	CWnd *pWnd = GetFocus();
    //	if (pWnd)
    //		pWnd = pWnd->GetParent();
    //
    //	if (pWnd == this)
    //		SetFocus();

    //	if (GetFocus()->GetParent() == this)
    //		SetFocus();

    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL
CListCtrlX::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    //	if (GetFocus()->GetParent() == this)
    //		SetFocus();

    return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

// added by Alex Zhondin
int CALLBACK
CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    SCompare* compare = (SCompare*)lParamSort;
    if (compare->m_nColumn == 0)
        return (lParam1 - lParam2) * (1 - 2 * !compare->m_bAscending); //(dwItem1 - dwItem2);
    else {
        CString str1 = compare->m_pSource->GetItemText(compare->m_pSource->FindItem(lParam1), compare->m_nColumn);
        CString str2 = compare->m_pSource->GetItemText(compare->m_pSource->FindItem(lParam2), compare->m_nColumn);

        return (str1.CompareNoCase(str2)) * (1 - 2 * !compare->m_bAscending);
    }
}

// added by Alex Zhondin
BOOL
CListCtrlX::SortColumn(int nColumn)
{
    if (!IsSorted(nColumn))
        return FALSE;

    DestroyEdit();

    CHeaderCtrl* header = GetHeaderCtrl();
    HDITEM hditem;
    memset(&hditem, 0, sizeof(hditem));
    hditem.mask = HDI_IMAGE | HDI_FORMAT;

    header->GetItem(nColumn, &hditem);
    if (m_bDragging) {
        hditem.fmt = HDF_STRING;
        header->SetItem(nColumn, &hditem);
        return FALSE;
    }

    if (nColumn != m_nSortColumn) {
        header->GetItem(m_nSortColumn, &hditem);
        hditem.fmt = (hditem.fmt & ~HDF_IMAGE) | HDF_STRING;
        header->SetItem(m_nSortColumn, &hditem);
        m_bAscending = TRUE;
    } else
        m_bAscending = !m_bAscending;

    m_nSortColumn = nColumn;

    //	header->GetItem(nColumn, &hditem);
    hditem.fmt    = hditem.fmt | HDF_IMAGE | HDF_STRING;
    hditem.iImage = m_bAscending;
    header->SetItem(nColumn, &hditem);

    SCompare compare;
    compare.m_pSource    = this;
    compare.m_nColumn    = nColumn;
    compare.m_bAscending = m_bAscending;

    SortItems(CompareFunc, (LPARAM)&compare);

    // We should get the parent
    CWnd* pWnd = GetParent();
    if (!pWnd || !pWnd->GetSafeHwnd())
        return FALSE;

    // Retreave data of the all items
    CDWordArray arData;
    for (int i = 0; i < GetItemCount(); i++)
        arData.Add(GetItemData(i));

    // At last, sent message to the parent
    return (::SendMessage(pWnd->m_hWnd, LIST_REARRANGE, (WPARAM)this->m_hWnd, (LPARAM)&arData) >= 0);
}

BOOL
CListCtrlX::IsSorted(int nCol)
{
    return FALSE;
}

void
CListCtrlX::SetItemImage(int nItem, int nImg)
{
    LVITEM item;
    item.mask     = LVIF_IMAGE;
    item.iItem    = nItem;
    item.iSubItem = 0;

    if (GetItem(&item)) {
        if (item.iImage != nImg) {
            item.iImage = nImg;
            SetItem(&item);
        }
    }
}

void
CListCtrlX::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    *pResult             = 0;

    SetListText(m_nBtnRow, m_nBtnCol, GetDlgCtrlID(), '\t' + uInt2Str(pNMUpDown->iDelta));
}

void
CListCtrlX::SetSpinRange(int nLower, int nUpper)
{
    if (m_pSpin == NULL) {
        m_pSpin = new CSpinButtonCtrl;

        CRect rc(10, 10, 20, 20);

        if (!m_pSpin->Create(WS_CHILD | WS_BORDER, rc, this, IDC_LISTTAB_SPINCELL))
            ASSERT(FALSE);
    }

    m_pSpin->SetRange(nLower, nUpper);
}

//*************************************************************************//
// CListCtrlXEdit class
//*************************************************************************//

CListCtrlXEdit::CListCtrlXEdit(CListCtrlX* pListTable, int iItem, int iSubItem, CString strInitText, BOOL bColNum)
{
    ASSERT(pListTable);

    m_pListTable  = pListTable;
    m_nItem       = iItem;
    m_nSubItem    = iSubItem;
    m_strInitText = strInitText;
    m_bCancelEdit = FALSE;
    m_bKillFocus  = FALSE;
    m_bColNum     = bColNum;

    m_bkBrush.CreateSolidBrush(GetSysColor(COLOR_HOTLIGHT));
}

CListCtrlXEdit::~CListCtrlXEdit()
{
}

BEGIN_MESSAGE_MAP(CListCtrlXEdit, CEditX)
//{{AFX_MSG_MAP(CListCtrlXEdit)
ON_WM_CREATE()
ON_WM_KEYDOWN()
ON_WM_KILLFOCUS()
ON_WM_NCDESTROY()
ON_WM_NCCALCSIZE()
ON_WM_CTLCOLOR_REFLECT()
ON_WM_GETDLGCODE()
ON_WM_CHAR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int
CListCtrlXEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CEditX::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Set the proper font
    CFont* pFont = GetParent()->GetFont();
    SetFont(pFont);

    SetWindowText(m_strInitText);
    SetFocus();
    SetSel(0, -1);

    return 0;
}

void
CListCtrlXEdit::OnKillFocus(CWnd* pNewWnd)
{
    if (!pNewWnd)
        return;

    if ((!m_bKillFocus && !m_bCancelEdit) && !m_pListTable->m_bWarning)
        SetListText();

    CEditX::OnKillFocus(pNewWnd);

    m_bKillFocus = TRUE;

    if (m_pListTable->m_bWarning) {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());

        if (m_pListTable->m_nError == 1)
            MessageBox(CString((LPCSTR)IDS_ERR_NAME), uLoadString(IDS_WARNING), MB_ICONWARNING); // (!) don't use the uLoadStringLocal
        else if (m_pListTable->m_nError == 2)
            MessageBox(
              CString((LPCSTR)IDS_ERR_DUPLICATE_NAME), uLoadString(IDS_WARNING), MB_ICONWARNING); // (!) don't use the uLoadStringLocal

        m_pListTable->m_bWarning = FALSE;
        m_pListTable->m_nError   = 0;
    }

    DestroyWindow();
}

void
CListCtrlXEdit::OnNcDestroy()
{
    CEditX::OnNcDestroy();
    m_pListTable->DestroyEdit();
    //	delete this;
}

void
CListCtrlXEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
    // calculate the size of the client area and the button.
    CEditX::OnNcCalcSize(bCalcValidRects, lpncsp);

    //	lpncsp->rgrc[0].left += (lpncsp->rgrc[0].bottom - lpncsp->rgrc[0].top) * 8/10;
}

HBRUSH
CListCtrlXEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    //	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    //	pDC->SetBkColor(GetSysColor(COLOR_HOTLIGHT));

    return NULL;
}

UINT
CListCtrlXEdit::OnGetDlgCode()
{
    return CEditX::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}

BOOL
CListCtrlXEdit::SetListText()
{
    CString Text;
    GetWindowText(Text);

    return m_pListTable->SetListText(m_nItem, m_nSubItem, GetDlgCtrlID(), Text);
}

BOOL
CListCtrlXEdit::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE) || GetKeyState(VK_CONTROL) || (pMsg->wParam == VK_INSERT) ||
            (pMsg->wParam == VK_DELETE)) {
            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return TRUE; // DO NOT process further
        }
    }

    return CEditX::PreTranslateMessage(pMsg);
}

void
CListCtrlXEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    INT Shift        = (GetKeyState(VK_SHIFT) < 0) ? -1 : 1;
    INT nColumns     = m_pListTable->GetHeaderCtrl()->GetItemCount();
    INT stackItem    = m_nItem;
    INT stackSubItem = m_nSubItem;

    CString stackText;
    GetWindowText(stackText);

    CListCtrlX* pTable = m_pListTable;

    switch (nChar) {
        case VK_ESCAPE: {
            m_bCancelEdit = TRUE;
            GetParent()->SetFocus();
            return;
        }
        case VK_RETURN: {
            m_bCancelEdit     = TRUE;
            BOOL bSetListText = SetListText();
            pTable->SetFocus();

            if (!bSetListText)
                pTable->EditSubItem(stackItem, stackSubItem, stackText);
            return;
        }
        case VK_TAB: {
            BOOL bSelListText = SetListText();
            GetParent()->SetFocus();
            if (bSelListText) {
                INT nRow = stackItem;
                INT nCol = stackSubItem;
                do {
                    nCol += Shift;
                    if ((nCol < 0) || (nCol >= nColumns)) {
                        nCol = (Shift > 0) ? 0 : nColumns - 1;
                        nRow += Shift;

                        if ((nRow < 0) || (nRow >= pTable->GetItemCount()))
                            return;
                    }
                } while (!pTable->IsEditable(nRow, nCol));

                pTable->StartCellEdit(nRow, nCol);
            } else
                pTable->EditSubItem(stackItem, stackSubItem, stackText);

            return;
        }
    }

    // If was set the ES_COL_NUMBER style
    if (m_bColNum) {
        BOOL bValid = FALSE;
        switch (nChar) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case ' ':
            case ',':
            case ';': //case '-':
            case VK_ESCAPE:
            case VK_RETURN:
            case VK_TAB:
            case VK_INSERT:
            case VK_DELETE:
            case VK_BACK:
                bValid = TRUE;
        }

        if (GetKeyState(VK_CONTROL) < 0)
            bValid = TRUE;

        if (!bValid) {
            MessageBeep(MB_ICONEXCLAMATION);
            return;
        }
    }

    CEditX::OnChar(nChar, nRepCnt, nFlags);

    // Resize edit control if needed

    // Get text extent
    CString Text;

    GetWindowText(Text);
    CWindowDC DC(this);
    CFont* pFont   = GetParent()->GetFont();
    CFont* pFontDC = DC.SelectObject(pFont);
    CSize Size     = DC.GetTextExtent(Text);
    DC.SelectObject(pFontDC);
    Size.cx += 5; // add some extra buffer

    // Get client rect
    CRect Rect, ParentRect;
    GetClientRect(&Rect);
    GetParent()->GetClientRect(&ParentRect);

    // Transform rect to parent coordinates
    ClientToScreen(&Rect);
    GetParent()->ScreenToClient(&Rect);

    // Check whether control needs to be resized and whether there is space to grow
    if (Size.cx > Rect.Width()) {
        if (Size.cx + Rect.left < ParentRect.right)
            Rect.right = Rect.left + Size.cx;
        else
            Rect.right = ParentRect.right;
        MoveWindow(&Rect);
    }
}

void
CListCtrlXEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Up and down are in the OnKeyDown so that the user can hold down the arrow
    // keys to scroll through the entries.
    BOOL Control     = GetKeyState(VK_CONTROL) < 0;
    INT i            = 0;
    INT stackItem    = m_nItem;
    INT stackSubItem = m_nSubItem;

    CListCtrlX* pTable = m_pListTable;

    CString stackText;
    GetWindowText(stackText);

    //	TRACE("WM_KEY_DOWN: char=%c, nFlags=%d\n", nChar, nFlags);

    switch (nChar) {
        case VK_UP: {
            BOOL bSetListText = SetListText();
            GetParent()->SetFocus();
            if (bSetListText) {
                do {
                    i++;
                    if (stackItem - i < 0)
                        return;
                } while (!pTable->IsEditable(stackItem - i, stackSubItem));

                pTable->StartCellEdit(stackItem - i, stackSubItem);
                return;
            }
            pTable->EditSubItem(stackItem, stackSubItem, stackText);
            return;
        }
        case VK_DOWN: {
            BOOL bSetListText = SetListText();
            GetParent()->SetFocus();
            if (bSetListText) {
                do {
                    i++;
                    if (stackItem + i >= pTable->GetItemCount())
                        return;
                } while (!pTable->IsEditable(stackItem + i, stackSubItem));
                pTable->StartCellEdit(stackItem + i, stackSubItem);
                return;
            }
            pTable->EditSubItem(stackItem, stackSubItem, stackText);
            return;
        }
        case VK_HOME: {
            if (!Control)
                break;

            BOOL bSetListText = SetListText();
            GetParent()->SetFocus();
            if (bSetListText) {
                pTable->StartCellEdit(0, stackSubItem);
                return;
            }
            pTable->EditSubItem(stackItem, stackSubItem, stackText);
            return;
        }
        case VK_END: {
            if (!Control)
                break;

            BOOL bSetListText = SetListText();
            GetParent()->SetFocus();
            if (bSetListText) {
                int Count = pTable->GetItemCount() - 1;
                pTable->StartCellEdit(Count, stackSubItem);
                return;
            }
            pTable->EditSubItem(stackItem, stackSubItem, stackText);
            return;
        }
    }

    CEditX::OnKeyDown(nChar, nRepCnt, nFlags);
}

//*************************************************************************//
// CListCtrlXCombo class
//*************************************************************************//

CListCtrlXCombo::CListCtrlXCombo(CListCtrlX* pListTable, int iItem, int iSubItem, CString strInitText)
{
    m_pListTable  = pListTable;
    m_nItem       = iItem;
    m_nSubItem    = iSubItem;
    m_strInitText = strInitText;
}

CListCtrlXCombo::~CListCtrlXCombo()
{
}

BEGIN_MESSAGE_MAP(CListCtrlXCombo, CComboBox)
//{{AFX_MSG_MAP(CListCtrlXCombo)
ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlXCombo message handlers

BOOL
CListCtrlXCombo::SetListText()
{
    if (GetCurSel() == LB_ERR)
        return TRUE;

    CString Text;
    GetLBText(GetCurSel(), Text);
    return m_pListTable->SetListText(m_nItem, m_nSubItem, GetDlgCtrlID(), Text);
}

void
CListCtrlXCombo::OnSelendok()
{
    SetListText();
    m_pListTable->SetFocus();
}

void
CListCtrlXCombo::OnSelendcancel()
{
    m_pListTable->SetFocus();
}

void
CListCtrlXCombo::Init(int iItem, int iSubItem, CString strInitText)
{
    m_nItem       = iItem;
    m_nSubItem    = iSubItem;
    m_strInitText = strInitText;
}
