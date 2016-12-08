/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:		TreeCtrlX.cpp
	CREATED:		?.?.2002
	VERSION:		1.2

	AUTHOR:			Dreyk
	MODIFIED:		
	LAST REVERSION:	12.12.2003

	DESCRIPTION:	Modified tree control.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "TreeCtrlX.h"

#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlX

CTreeCtrlX::CTreeCtrlX()
{
    // Default size of the image list's icons
    m_sizeImage.cx = 16;
    m_sizeImage.cy = 16;

    m_bDragEnable  = FALSE;
    m_bDragging    = FALSE;
    m_pImgListDrag = NULL;
}

CTreeCtrlX::~CTreeCtrlX()
{
}

BEGIN_MESSAGE_MAP(CTreeCtrlX, CTreeCtrl)
//{{AFX_MSG_MAP(CTreeCtrlX)
ON_WM_PAINT()
ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlX message handlers

HTREEITEM
CTreeCtrlX::FindItemID(HTREEITEM hParent, DWORD id)
{
    HTREEITEM hItem = GetChildItem(hParent);
    while (hItem) {
        if (this->GetItemData(hItem) == id)
            return hItem;

        HTREEITEM hFound = FindItemID(hItem, id);
        if (hFound)
            return hFound;

        hItem = GetNextSiblingItem(hItem);
    }

    return NULL;
}

void
CTreeCtrlX::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
    Color_Font cf;

    if (!m_mapColorFont.Lookup(hItem, cf))
        cf.color = (COLORREF)-1;

    cf.logfont            = logfont;
    m_mapColorFont[hItem] = cf;
}

void
CTreeCtrlX::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
    SetItemState(hItem, bBold ? TVIS_BOLD : 0, TVIS_BOLD);
}

void
CTreeCtrlX::SetItemColor(HTREEITEM hItem, COLORREF color)
{
    Color_Font cf;

    if (!m_mapColorFont.Lookup(hItem, cf))
        cf.logfont.lfFaceName[0] = '\0';

    cf.color              = color;
    m_mapColorFont[hItem] = cf;
}

BOOL
CTreeCtrlX::GetItemFont(HTREEITEM hItem, LOGFONT* plogfont)
{
    Color_Font cf;

    if (!m_mapColorFont.Lookup(hItem, cf))
        return FALSE;

    if (cf.logfont.lfFaceName[0] == '\0')
        return FALSE;

    *plogfont = cf.logfont;

    return TRUE;
}

BOOL
CTreeCtrlX::IsItemBold(HTREEITEM hItem)
{
    return GetItemState(hItem, TVIS_BOLD) & TVIS_BOLD;
}

COLORREF
CTreeCtrlX::GetItemColor(HTREEITEM hItem)
{
    // Returns (COLORREF)-1 if color was not set
    Color_Font cf;

    if (!m_mapColorFont.Lookup(hItem, cf))
        return (COLORREF)-1;

    return cf.color;
}

void
CTreeCtrlX::OnPaint()
{
    //	CTreeCtrl::OnPaint();
    //	return;
    CPaintDC dc(this); // device context for painting

    // Create a memory DC compatible with the paint DC
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    CRect rcClip, rcClient;
    dc.GetClipBox(&rcClip);
    GetClientRect(&rcClient);

    // Select a compatible bitmap into the memory DC
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
    memDC.SelectObject(&bitmap);

    // Set clip region to be same as that in paint DC
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&rcClip);
    memDC.SelectClipRgn(&rgn);
    rgn.DeleteObject();

    // First let the control do its default drawing.
    CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

    HTREEITEM hItem = GetFirstVisibleItem();

    int n = GetVisibleCount() + 1;

    while (hItem && n--) {
        CRect rect;

        // Do not meddle with selected items or drop highlighted items
        Color_Font cf;

        if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
            SetItemBold(hItem);
        else
            SetItemBold(hItem, FALSE);

        if (!(GetItemState(hItem, selflag) & selflag) && m_mapColorFont.Lookup(hItem, cf))
        //if (m_mapColorFont.Lookup(hItem, cf))
        {
            CFont* pFontDC;
            CFont fontDC;
            LOGFONT logfont;

            if (cf.logfont.lfFaceName[0] != '\0')
                logfont = cf.logfont;
            else {
                // No font specified, so use window font
                CFont* pFont = GetFont();
                pFont->GetLogFont(&logfont);
            }

            if (IsItemBold(hItem))
                logfont.lfWeight = 700;

            fontDC.CreateFontIndirect(&logfont);
            pFontDC = memDC.SelectObject(&fontDC);

            if (cf.color != (COLORREF)-1)
                memDC.SetTextColor(cf.color);

            CString sItem = GetItemText(hItem);

            GetItemRect(hItem, &rect, TRUE);

            //if ((GetItemState(hItem, selflag) & selflag))
            //	memDC.SetBkColor(GetSysColor((GetFocus() == this) ? COLOR_HIGHLIGHT : COLOR_3DLIGHT));
            //else
            memDC.SetBkColor(GetSysColor(COLOR_WINDOW));

            memDC.TextOut(rect.left + 2, rect.top + 2, sItem);
            memDC.SelectObject(pFontDC);
        }
        hItem = GetNextVisibleItem(hItem);
    }

    dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, rcClip.left, rcClip.top, SRCCOPY);

    memDC.DeleteDC();
}

BOOL
CTreeCtrlX::IsItemExpanded(HTREEITEM hItem)
{
    return GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED;
}

void
CTreeCtrlX::SetItemExpand(HTREEITEM hItem, BOOL bExpand)
{
    Expand(hItem, bExpand ? TVE_EXPAND : TVE_COLLAPSE);
}

BOOL
CTreeCtrlX::IsItemSelected(HTREEITEM hItem)
{
    return GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED;
}

void
CTreeCtrlX::SetItemBlue(HTREEITEM hItem)
{
    Color_Font cf;

    if (!m_mapColorFont.Lookup(hItem, cf))
        cf.logfont.lfFaceName[0] = '\0';

    cf.color              = RGB(0, 0, 128);
    m_mapColorFont[hItem] = cf;
}

void
CTreeCtrlX::SetItemRed(HTREEITEM hItem)
{
    Color_Font cf;

    if (!m_mapColorFont.Lookup(hItem, cf))
        cf.logfont.lfFaceName[0] = '\0';

    cf.color              = RGB(128, 0, 0);
    m_mapColorFont[hItem] = cf;
}

void
CTreeCtrlX::ClearColorFont()
{
    m_mapColorFont.RemoveAll();
}

CImageList*
CTreeCtrlX::CreateImageList(UINT nIDResource, COLORREF crBgd /*= RGB_BACKGROUND*/, int nImageListType /*= TVSIL_NORMAL*/)
{
    CBitmap bmp;
    if (!bmp.LoadBitmap(nIDResource))
        return NULL;

    CImageList img;
    img.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_MASK | ILC_COLOR24, 0, 0);
    if (img.Add(&bmp, crBgd) == -1)
        return NULL;

    int nImgCount = img.GetImageCount();
    if (nImgCount < 1)
        return NULL;

    m_ImgList.DeleteImageList();
    m_ImgList.Create(&img);

    HICON hIcon;
    for (int nIndex = 0; nIndex < nImgCount; nIndex++) {
        hIcon = img.ExtractIcon(nIndex);
        AddGloomIcon(hIcon);
        DestroyIcon(hIcon);
    }

    return SetImageList(&m_ImgList, TVSIL_NORMAL);
}

int
CTreeCtrlX::AddGloomIcon(HICON hIcon)
{
    ICONINFO iconInfo;
    ZeroMemory(&iconInfo, sizeof(iconInfo));
    if (!GetIconInfo(hIcon, &iconInfo))
        return -1;

    CDC myDC;
    myDC.CreateCompatibleDC(0);

    CBitmap bmColor;
    bmColor.Attach(iconInfo.hbmColor);
    CBitmap bmMask;
    bmMask.Attach(iconInfo.hbmMask);

    CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
    COLORREF crPixel;
    int j;
    for (int i = 0; i < m_sizeImage.cx; ++i) {
        for (j = 0; j < m_sizeImage.cy; ++j) {
            crPixel = myDC.GetPixel(i, j);
            myDC.SetPixel(i, j, DarkenColor(SCALE_DARK, crPixel));
        }
    }

    myDC.SelectObject(pOldBitmap);

    return m_ImgList.Add(&bmColor, &bmMask);
}

void
CTreeCtrlX::SetImageSize(CSize sz)
{
    m_sizeImage = sz;
}

CSize
CTreeCtrlX::GetImageSize()
{
    return m_sizeImage;
}

BOOL
CTreeCtrlX::IsDragEnabled()
{
    return m_bDragEnable;
}

void
CTreeCtrlX::EnableDrag(BOOL bEnable /*= TRUE*/)
{
    m_bDragEnable = bEnable;
}

////////////////////////////////////////////////////////////////////////////
// This routine performs the actual drop of the item dragged.
// Function send TREE_REARRANGE message to the parent window
//
// The sample function is placed below:
// This routine performs the actual drop after the item dragged.
// Function get TREE_REARRANGE message from the list control window
//
//afx_msg LRESULT OnRearrange(WPARAM wParam, LPARAM lParam);
//ON_MESSAGE(TREE_REARRANGE, OnRearrange)
//LRESULT CDlgBar::OnRearrange(WPARAM wParam, LPARAM lParam)
//{
//	if ((HWND)wParam != m_Tree.m_hWnd)
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
CTreeCtrlX::TransferItem(HTREEITEM hDrag, HTREEITEM hDrop)
{
    if (!GetSafeHwnd())
        return FALSE;

    // We should get the parent
    CWnd* pWnd = GetParent();
    if (!pWnd || !pWnd->GetSafeHwnd())
        return FALSE;

    HTREEITEM hParent = GetParentItem(hDrag);
    if (!hParent)
        return FALSE;

    CWaitCursor wait;

    DWORD dwDragData = GetItemData(hDrag);
    DWORD dwDropData = GetItemData(hDrop);
    DWORD dwDataCur;

    //	SetItemState(hDrag, 0, TVIS_DROPHILITED);
    //	SetItemState(hDrop, 0, TVIS_SELECTED);

    // Retreave data of the all child items
    HTREEITEM hItem = GetChildItem(hParent);
    HTREEITEM hNext = NULL;
    CDWordArray arData;

    while (hItem) {
        hNext     = GetNextItem(hItem, TVGN_NEXT);
        dwDataCur = GetItemData(hItem);

        if (dwDataCur == dwDropData)
            arData.Add(dwDragData);
        else if (dwDataCur == dwDragData) {
            hItem = hNext;
            continue;
        }

        arData.Add(dwDataCur);

        hItem = hNext;
    }

    // At last, sent message to the parent
    if (::SendMessage(pWnd->m_hWnd, TREE_REARRANGE, (WPARAM)this->m_hWnd, (LPARAM)&arData) >= 0) {
        hItem = GetChildItem(hParent);
        while (hItem != NULL) {
            if (GetItemData(hItem) == dwDragData) {
                SelectItem(hItem);
                return TRUE;
            }
            hItem = GetNextItem(hItem, TVGN_NEXT);
        }
    }

    return FALSE;

    ///////////////////////////////////////////////////////////////
    // OLD VARIANT
    //	TV_INSERTSTRUCT	tvstruct;
    //	TCHAR			sztBuffer[256];
    //	HTREEITEM		hNew;
    //	HTREEITEM		hAfter = GetNextItem(hDrop, TVGN_PREVIOUS);
    //	if (!hAfter || hAfter == hParent)
    //		hAfter = TVI_FIRST;
    //
    //	DWORD dwData = GetItemData(hDrag);
    //	// avoid an infinite recursion situation
    //	tvstruct.item.hItem			= hDrag;
    //	tvstruct.item.cchTextMax	= 255;
    //	tvstruct.item.pszText		= sztBuffer;
    //	tvstruct.item.mask			= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    //
    //	GetItem(&tvstruct.item);  // get information of the dragged element
    //
    //	tvstruct.hParent			= hParent;
    //	tvstruct.hInsertAfter		= hAfter;//hDrop;
    //	tvstruct.item.mask			= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; // TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    //
    //	hNew = InsertItem(&tvstruct);
    //
    //	if (!hNew)
    //		return FALSE;
    //
    ////CString sDrag  = GetItemText(hDrag);
    ////CString sDrop  = GetItemText(hDrop);
    ////CString sAfter = hAfter != TVI_FIRST ? GetItemText(hAfter) : "TVI_FIRST";
    ////CString sNew   = GetItemText(hNew);
    ////CString sName  = tvstruct.item.pszText;
    //
    //    DeleteItem(hDrag);
    ////	BOOL bRes = SetItemData(hNew, dwData);
    //
    //	// We should get the parent
    //	CWnd* pWnd = GetParent();
    //	if (!pWnd || !pWnd->GetSafeHwnd())
    //		return FALSE;
    //	// At last, sent message to the parent
    //    if (::SendMessage(pWnd->m_hWnd, TREE_REARRANGE, (WPARAM)this->m_hWnd, (LPARAM)hParent) >= 0)
    //	{
    //		SelectItem(hNew);
    //		return TRUE;
    //	}
    //
    //	return FALSE;
}

void
CTreeCtrlX::OnBeginDrag(LPNMHDR pnmhdr, LRESULT* pLResult)
{
    if (!m_bDragEnable)
        return;

    ASSERT(!m_bDragging);
    CPoint ptAction;
    GetCursorPos(&ptAction);
    ScreenToClient(&ptAction);

    HTREEITEM hItem   = HitTest(ptAction);
    HTREEITEM hParent = GetParentItem(hItem);
    if (hParent == NULL) // don't drag the root items
        return;

    m_hDrag = hItem;
    m_hDrop = NULL;

    ASSERT(m_pImgListDrag == NULL);
    m_pImgListDrag = CreateDragImage(m_hDrag); // get the image list for dragging

    if (!m_pImgListDrag)
        return;

    m_pImgListDrag->DragShowNolock(TRUE);
    m_pImgListDrag->SetDragCursorImage(0, CPoint(0, 0));
    m_pImgListDrag->BeginDrag(0, CPoint(0, 0));
    m_pImgListDrag->DragMove(ptAction);
    m_pImgListDrag->DragEnter(GetDesktopWindow(), ptAction);

    m_bDragging = TRUE;

    SetCapture();
}

void
CTreeCtrlX::OnMouseMove(UINT nFlags, CPoint point)
{
    HTREEITEM hItem;

    if (m_bDragging) {
        MouseScroll(point);

        ASSERT(m_pImgListDrag != NULL);

        // Drag the item to the current position
        CPoint pt(point);
        ClientToScreen(&pt);
        m_pImgListDrag->DragMove(pt);
        m_pImgListDrag->DragShowNolock(FALSE);

        if ((hItem = HitTest(point)) != NULL && (GetParentItem(m_hDrag) == GetParentItem(hItem))) {
            // Note that we can drop here
            SetCursor(LoadCursor(NULL, IDC_ARROW));

            m_pImgListDrag->DragLeave(this);
            SelectDropTarget(hItem);
            m_hDrop = hItem;
            m_pImgListDrag->DragEnter(this, point);
        } else {
            // Note that we cannot drop here
            SetCursor(LoadCursor(NULL, IDC_NO));
        }

        m_pImgListDrag->DragShowNolock(TRUE);
        return;
    }

    CTreeCtrl::OnMouseMove(nFlags, point);
}

void
CTreeCtrlX::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging) {
        ASSERT(m_pImgListDrag != NULL);
        m_pImgListDrag->DragLeave(GetDesktopWindow());
        m_pImgListDrag->EndDrag();
        delete m_pImgListDrag;
        m_pImgListDrag = NULL;

        ReleaseCapture();
        m_bDragging = FALSE;
        SelectDropTarget(NULL);

        CRect client;
        GetClientRect(client);
        if (!client.PtInRect(point)) {
            MessageBeep(MB_ICONEXCLAMATION);
            return;
        }

        if (m_hDrag != m_hDrop && (GetParentItem(m_hDrag) == GetParentItem(m_hDrop))) {
            if (!TransferItem(m_hDrag, m_hDrop))
                MessageBeep(MB_ICONEXCLAMATION);
        } else
            MessageBeep(MB_ICONEXCLAMATION);
    }

    CTreeCtrl::OnLButtonUp(nFlags, point);
}

void
CTreeCtrlX::MouseScroll(CPoint point)
{
    CRect client;
    GetClientRect(client);

    if (client.PtInRect(point))
        return;

    if (point.x < client.left)
        PostMessage(WM_HSCROLL, SB_LINELEFT);
    else if (point.x > client.right)
        PostMessage(WM_HSCROLL, SB_LINERIGHT);

    if (point.y > client.bottom)
        PostMessage(WM_VSCROLL, SB_LINEDOWN);
    else if (point.y < client.top) {
        PostMessage(WM_VSCROLL, SB_LINEUP);

        HTREEITEM hFirst = GetFirstVisibleItem();
        if (!hFirst)
            return;

        CRect rc;
        GetItemRect(hFirst, &rc, FALSE);
        InvalidateRect(rc);
    }
}