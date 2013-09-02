#if !defined(LIST_CTRL_X_H__MFCAIDS__INCLUDED_)
#define LIST_CTRL_X_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListTable.h : header file
//

#include "EditX.h"

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

#define CHOOSE_COL(color, altcolor) (((color & 0xff000000) == 0x0f000000) ? altcolor : color)
#define IDC_EDITCELL 1001

#ifndef ODS_HOTLIGHT
#define ODS_HOTLIGHT 0x0040
#endif

#ifndef COLOR_HOTLIGHT
#define COLOR_HOTLIGHT 26
#endif

#define ES_COL_NUMBER 0x8000L

#define TEXT_INDENT      20
#define ICON_INDENT_LEFT 3
#define ICON_INDENT_TOP  1

#ifndef LIST_REARRANGE
#define LIST_REARRANGE (WM_USER + 8123)
#endif  // !LIST_REARRANGE

/////////////////////////////////////////////////////////////////////////////
// CListCtrlX window

class CListCtrlXCombo;
class CListCtrlXEdit;

class MFCAIDS_API CListCtrlX : public CListCtrl
{
// Construction
public:	
	CListCtrlX();

	struct CDrawParams
	{
		int     nRow;
		int     nColumn;
		int     nIndex;
		BOOL    bDrawCell;
		DWORD   dwBkColor;
		BOOL    bAlwaysSelected;
		BOOL    bLVHasFocus;
		BOOL    bItemHasFocus;
		BOOL    bSelected;
		BOOL    bFullRowSelected;
		BOOL    bHotLight;
		CPen    penGrid;
		DWORD   dwNormalText;
		DWORD   dwNormalTextCell;
		DWORD   dwHiLiteBk;
		DWORD   dwHiLiteBkCell;
		LVITEM* pSubItem;
	};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlX)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CListCtrlXEdit* GetEditCtrl();
	void SelectAll();
	void SetSpinRange(int nLower, int nUpper);
	virtual BOOL IsDraggingAllowed();
	void SetItemImage(int nItem, int nImg);
	void DestroyEdit();
	BOOL SortColumn(int nColumn);
	void EnableDrag(BOOL bEnable = TRUE);
	virtual BOOL IsDragEnabled();
	void ColorSortColumn(BOOL bEnable = TRUE);
	virtual BOOL IsSorted(int nCol);
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	CImageList* GetImageList(int nImageList) const;
	CImageList* SetImageList(CImageList* pImageList, int nImageList);
	void SetEmptyMsg(LPCTSTR strMsg);
	virtual void OnButtonPressed();
	virtual BOOL IsCheck(int nRow, int nCol);
	virtual BOOL IsCombo(int nRow, int nCol);
	virtual BOOL IsButton(int nRow, int nCol);
	virtual BOOL IsSpin(int nRow, int nCol);
	void LoadColumns(int nColumns, ...);
	void SaveColumns();
	virtual void DoubleClickItem(int nRow, int nCol) {};
	int Get1stSelectedItem();
	virtual BOOL SetListText(int nRow, int nCol, int dlgCtrlId, CString strText);
	int FindItem(LPARAM lParam);
	int FindItem(CString sFind, int nCol = 1);
	int GetSelColumn();
	void SelectItem(int nItem, int nSubItem = -1);
	virtual BOOL IsEditable(int nRow, int nCol);
	virtual void StartCellEdit(int nRow, int nCol);
	void DeselectAll();
	virtual CEditX* EditSubItem(int nItem, int nCol, LPCTSTR strInitText = NULL);
	void EnsureVisibleColumn(int nCol);
	int GetSel(int* nCol = NULL);
	int HitTest(CPoint pt, UINT* pFlags = 0, int *pColumn = NULL);
	void SetRowHeight(int nHeight);
	BOOL KeepLabelLeft(BOOL bKeepLeft = TRUE);
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem = -1);
	int InsertColumn(int nCol, const LVCOLUMN* pColumn);
	virtual void DrawSubItemText(CDC* pDC, LVITEM* pItem, LVCOLUMN* pColumn, LPRECT pRect);
	void DrawSmallIcon(CDC* pDC, LVITEM* pItem, LPRECT pRect);
	void DrawStateIcon(CDC* pDC, LVITEM* pItem, LPRECT pRect);
	void EnableSortIcon(BOOL bEnable = TRUE, int nSortColumn = 0);
	BOOL SetKeepLabelLeft(BOOL bKeepLeft);
	int OrderToIndex(int nOrder);
	int IndexToOrder(int nIndex);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CListCtrlX();

	BOOL m_bWarning;
	int  m_nError;

	// Generated message map functions
protected:
	void MouseScroll(CPoint point);
	virtual BOOL DropItemOnList();
	void SetSortIcon();
	virtual void FillCellRect(CDC *pDC, CRect& rc, COLORREF col);
	virtual void PopulateCombo(int nRow, int nCol, CListCtrlXCombo *pCombo);
	virtual void CreateComboList(int nRow, int nCol);
	virtual LPCTSTR RegKey();
	virtual UINT EditLimit(int nRow, int nCol);
	virtual void EditStyle(int nRow, int nCol, DWORD &dwStyle);
	virtual BOOL SelectColumn(int nCol);
	virtual void DrawBrowse(CDC *pDC, CRect &rc);
	virtual void DrawCheckBox(CDC *pDC, CRect &rc, DWORD dwStyle);
	virtual void DrawCombo(CDC *pDC, CRect &rc, CRect &rcText, BOOL bSelected);
	virtual void DrawCheck(CDC *pDC, CRect &rc, CRect &rcText, BOOL bSelected);
	virtual void DrawCellGrid(CDC* pDC, CRect& rc);
	virtual void DrawCellRect(CDC *pDC, CRect &rc, CRect &rcText, BOOL bSelected);
	void DrawSpin(CDC *pDC, CRect &rc, CRect &rcText, BOOL bSelected);
	void DrawBtn(CDC *pDC, CRect &rc, CRect &rcText, BOOL bSelected);
	BOOL GetRealSubItemRect(int iItem, int iSubItem, int nArea, CRect& ref);
	void InvalidateNonItemArea();
	void JustifyFirstColumn(int nFormat);
	void CreateSortIcons();
	BOOL GetStateIconRect(int nItem, LPRECT pRect);
	int GetLabelWidth(CDC* pDC, LVITEM* pItem, int nMaxWidth);
	
	CListCtrlXEdit*  m_pEdit;
	CSpinButtonCtrl* m_pSpin;
	CListCtrlXCombo* m_pCombo;
	CDrawParams      dp;
	CRect            m_rcBtn;
	CString          m_strEmptyMsg;
	int              m_nBtnCol;
	int              m_nBtnRow;
	int              m_nFormatOfSubItem0;
	int			     m_nSortColumn;
	int              m_nColumnSel;
	BOOL             m_bBtnPressed;
	BOOL             m_bBtnPushed;
	BOOL             m_bRepaintNonItem;
	BOOL		     m_bSortIconEnabled;
	BOOL		     m_bAscending;
	BOOL             m_bKeepLabelLeft;
	BOOL             m_bColorSortColumn;
	BOOL             m_bDragEnable;
	DWORD            m_dwColBkCell;
	DWORD            m_dwColBkCellSel;
	DWORD            m_dwColCellSelText;
	DWORD            m_dwColHilightInactive;
	DWORD            m_dwColHilight;
	DWORD            m_dwColItemText;
	DWORD            m_dwColSelItemText;
	DWORD            m_dwColBkNonItem;
	DWORD            m_dwColBkItem;
	DWORD            m_dwColSortColor;
	DWORD            m_dwColGridLines;
	CImageList	     m_ImgListSort;
	CImageList       m_ImageList;
	CImageList*      m_pImgList_Small;
	CImageList*      m_pImgList_Normal;
	CImageList*      m_pImgList_State;
	CImageList*      m_pImgListDrag;	// For creating and managing the drag-image
	BOOL		     m_bDragging;	// TRUE during a drag operation
	int			     m_nDragIndex;	// Index of selected item in the List we are dragging FROM
	int			     m_nDropIndex;	// Index at which to drop item in the List we are dropping ON

	//{{AFX_MSG(CListCtrlX)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnEndtrack(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	static const int m_nFirstColXOff;
	static const int m_nNextColXOff;
	static const int m_nIconXOff;
};

//added by Alex Zhondin
struct SCompare
{
	CListCtrlX*	m_pSource;
	int			m_nColumn;
	BOOL		m_bAscending;
};

//typedef struct 
//{
//	LVITEM* plvi;
//	DWORD   dwData;
//} LDRAGITEM, *PLDRAGITEM;
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// CListCtrlXEdit class

class CListCtrlXEdit : public CEditX
{
// Construction
public:
	CListCtrlXEdit(CListCtrlX* pListTable, int iItem, int iSubItem, CString strInitText, BOOL bColEdit);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlXEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlXEdit();

	// Generated message map functions
protected:
	BOOL SetListText();
	CBrush      m_bkBrush;
	CString     m_strInitText;
	BOOL        m_bColNum;
	BOOL        m_bCancelEdit;
	BOOL        m_bKillFocus;
	int         m_nItem;
	int         m_nSubItem;
	CListCtrlX* m_pListTable;

	//{{AFX_MSG(CListCtrlXEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// CListCtrlXCombo window

class CListCtrlXCombo : public CComboBox
{
	// Construction
public:
	CListCtrlXCombo(CListCtrlX* pListTable, int iItem, int iSubItem, CString strInitText);
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlXCombo)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void Init(int iItem, int iSubItem, CString strInitText);
	virtual ~CListCtrlXCombo();
	
	// Generated message map functions
protected:
	BOOL SetListText();
	CString m_strInitText;
	int m_nItem;
	int m_nSubItem;
	CListCtrlX* m_pListTable;
	//{{AFX_MSG(CListCtrlXCombo)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSelendok();
	afx_msg void OnSelendcancel();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(LIST_CTRL_X_H__MFCAIDS__INCLUDED_)
