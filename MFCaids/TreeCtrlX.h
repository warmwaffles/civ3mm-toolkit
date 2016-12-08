#if !defined(TREE_CONTROL_X_H__MFCAIDS__INCLUDED_)
#define TREE_CONTROL_X_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlX.h : header file
//

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

#include "MFCdefs.h"
#include <afxtempl.h>

#ifndef TREE_REARRANGE
#define TREE_REARRANGE (WM_USER + 10123)
#endif // !TREE_REARRANGE

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlX window
typedef struct
{
    TV_INSERTSTRUCT* ptv;
    DWORD dwData;
} TDRAGITEM, *PDRAGITEM;

class MFCAIDS_API CTreeCtrlX : public CTreeCtrl
{
    // Construction
  public:
    CTreeCtrlX();

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTreeCtrlX)
    //}}AFX_VIRTUAL
    CImageList m_ImgList;

    // Implementation
  public:
    BOOL IsDragEnabled();
    void EnableDrag(BOOL bEnable = TRUE);
    CSize GetImageSize();
    void SetImageSize(CSize sz);
    CImageList* CreateImageList(UINT nIDResource, COLORREF crBgd = RGB_BACKGROUND, int nImageListType = TVSIL_NORMAL);
    void ClearColorFont();
    BOOL IsItemSelected(HTREEITEM hItem);
    BOOL IsItemExpanded(HTREEITEM hItem);
    BOOL IsItemBold(HTREEITEM hItem);
    BOOL GetItemFont(HTREEITEM hItem, LOGFONT* plogfont);
    COLORREF GetItemColor(HTREEITEM hItem);

    void SetItemExpand(HTREEITEM hItem, BOOL bExpand = TRUE);
    void SetItemBold(HTREEITEM hItem, BOOL bBold = TRUE);
    void SetItemFont(HTREEITEM hItem, LOGFONT& logfont);
    void SetItemColor(HTREEITEM hItem, COLORREF color);
    void SetItemBlue(HTREEITEM hItem);
    void SetItemRed(HTREEITEM hItem);

    HTREEITEM FindItemID(HTREEITEM hParent, DWORD id);

    virtual ~CTreeCtrlX();

  protected:
    void MouseScroll(CPoint point);
    virtual BOOL TransferItem(HTREEITEM hDrag, HTREEITEM hDrop);
    int AddGloomIcon(HICON hIcon);
    struct Color_Font
    {
        COLORREF color;
        LOGFONT logfont;
    };

    CMap<void*, void*, Color_Font, Color_Font&> m_mapColorFont;
    CSize m_sizeImage;

    BOOL m_bDragEnable;
    BOOL m_bDragging;
    HTREEITEM m_hDrag;
    HTREEITEM m_hDrop;
    CImageList* m_pImgListDrag;

    // Generated message map functions
  protected:
    //{{AFX_MSG(CTreeCtrlX)
    afx_msg void OnPaint();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnBeginDrag(LPNMHDR pnmhdr, LRESULT* pLResult);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(TREE_CONTROL_X_H__MFCAIDS__INCLUDED_)
