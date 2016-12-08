#if !defined(TOOLBAR_X_INCLUDED)
#define TOOLBAR_X_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ToolBarEx.h : header file
//

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

#include "MFCdefs.h"
#include "CustomizeDialog.h"

// Image sizes
const CSize m_szImageSmall(16, 16);
const CSize m_szImageLarge(24, 24);

struct TBBUTTONEX
{
    TBBUTTON tbinfo;
    BOOL bInitiallyVisible;
};

struct TBBUTTONSETUP
{
    TBBUTTON tbinfo;
    BOOL bInitiallyVisible;
    UINT idMenu;
};

struct TBBUTTONSEP
{
    TBBUTTON tbinfo;
    UINT nIndex;
};

/////////////////////////////////////////////////////////////////////////////
// class CToolBarX

class MFCAIDS_API CToolBarX : public CToolBar
{
  protected:
    friend class CCustomizeDialog;

    // One of these for each drop-down button
    struct CDropDownButtonInfo
    {
        CDropDownButtonInfo* pNext;
        UINT idButton; // command ID of button
        UINT idMenu;   // popup menu to display
    };

    // Construction
  public:
    CToolBarX();

    virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR);
    virtual BOOL CreateEx(CWnd* pParentWnd,
                          DWORD dwCtrlStyle = TBSTYLE_FLAT,
                          DWORD dwStyle     = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
                          CRect rcBorders   = CRect(0, 0, 0, 0),
                          UINT nID          = AFX_IDW_TOOLBAR);

    BOOL LoadToolBar(UINT nIDResource, COLORREF clrMask = RGB_BACKGROUND);
    BOOL LoadBitmap(UINT nIDResource)
    {
        return LoadBitmap(MAKEINTRESOURCE(nIDResource));
    }
    BOOL SetBitmap(UINT nIDResource, EIconOptions eIconOptionsDef = ioSmallIcons, COLORREF clrMask = RGB_BACKGROUND);
    void SetButtons(int nNumButtons, TBBUTTONSETUP* lpButtons, ETextOptions eTextOptionsDef = toNoTextLabels);

    // Attributes
  public:
    BOOL IsCustomizeEnable()
    {
        return m_bIsCustomizeEnable;
    }
    virtual BOOL IsTextOptionAvailable(ETextOptions eTextOptions) const;
    virtual BOOL IsIconOptionAvailable(EIconOptions eIconOptions) const;
    ETextOptions GetTextOptions()
    {
        return m_eTextOptions;
    }
    EIconOptions GetIconOptions()
    {
        return m_eIconOptions;
    }

    // Operations
  public:
    // This member function is called to insert a control into the toolbar
    // and returns a pointer to the newly inserted control
    CWnd* InsertControl(CRuntimeClass* pClass, LPCTSTR lpszWindowName, CRect& rect, UINT nID, DWORD dwStyle = NULL);
    CWnd* InsertControl(CWnd* pCtrl, CRect& rect, UINT nID);

    // Call to add drop-down buttons
    BOOL AddDropDownButton(UINT nButtonID, UINT nMenuID, BOOL bArrow = TRUE);

    // State functions
    void SaveState();
    void RestoreState();

    virtual BOOL HasButtonText(UINT nID);
    virtual BOOL HasButtonTip(UINT nID);
    virtual void GetButtonText(UINT nID, CString& strText);
    virtual void GetButtonTip(UINT nID, CString& strTip);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CToolBarX)
  protected:
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    void DisableTextOptions(CByteArray& arDisabledTextOptions);
    void DisableIconOptions(CByteArray& arDisabledIconOptions);
    virtual ~CToolBarX();
    BOOL EnableCustomize(BOOL bEnable = TRUE);
    void AddDropDownButtonInfo(UINT nButtonID, UINT nMenuID);
    void HideCtrlsOnVertical(BOOL bHide = TRUE);
    BOOL DropDownButton(UINT nID, CRect rc);
    void UpdateParentBandInfo();
    void SetNoTextOnRight(CDWordArray& arID);
    BOOL MarkDefaultState();
    void Customize();
    void SetTextOptions(ETextOptions eTextOptions, BOOL bUpdate = TRUE);
    void SetIconOptions(EIconOptions eIconOptions, BOOL bUpdate = TRUE);
    virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);
    void DrawGripper(CDC* pDC, const CRect& rect);

    // Generated message map functions
  protected:
    BOOL SetBitmap(HBITMAP hbmImageWell);
    BOOL LoadToolBar(LPCTSTR lpszResourceName);
    BOOL LoadBitmap(LPCTSTR lpszResourceName);
    BOOL MakeNormalImgList(HBITMAP hbmp, CSize sizeImage);
    BOOL MakeDisabledImgList(HBITMAP hbmp, CSize sizeImage);
    BOOL MakeImages();
    int AddGloomIcon(HICON hIcon, int nIndex = -1);
    int AddGrayIcon(HICON hIcon, int nIndex = -1);
    BOOL LoadBitmapEx(LPCTSTR lpszResourceName);
    void UpdateToolbar();
    void ReloadButtons();
    BOOL GetButtonInfo(UINT nID, TBBUTTON& tbinfo);
    BOOL ShowContextMenu(CPoint pt);
    BOOL IsVertDocked();
    void PositionControls();

    static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
    CSize GetButtonSize(TBBUTTON* pData, int iButton, DWORD dwMode);
    CSize CalcSize(TBBUTTON* pData, int nCount, DWORD dwMode);
    int WrapToolBar(TBBUTTON* pData, int nCount, int nWidth, DWORD dwMode);
    void SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert, DWORD dwMode);
    CSize CalcLayout(DWORD dwMode, int nLength = -1);
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
    void GetButtonEx(int nIndex, TBBUTTON* pButton) const;
    void SetButtonEx(int nIndex, TBBUTTON* pButton);
    CDropDownButtonInfo* FindDropDownButtonInfo(UINT nID);

    virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
    {
        if (message == WM_COMMAND) {
        }
        return CToolBar::OnChildNotify(message, wParam, lParam, pLResult);
    }

    CArray<TBBUTTONEX, TBBUTTONEX&> m_arTB;
    CArray<TBBUTTONSEP, TBBUTTONSEP&> m_arSep;

    static CToolBarX* m_pToolBar;
    static CCustomizeDialog* m_pCustomizeDlg;
    static HHOOK m_hCBTHook;
    static int m_nBarNumber; // used for customization, holds bar number for restoration.

    ETextOptions m_eTextOptionsDef;      // default text options
    EIconOptions m_eIconOptionsDef;      // default icon options
    ETextOptions m_eTextOptions;         // current text options
    EIconOptions m_eIconOptions;         // current icon options
    CObList* m_pControls;                // CObject pointer list which contains controls added to the toolbar
    CDropDownButtonInfo* m_pDropButtons; // list of dropdown button/menu pairs
    CString m_strSubKey;                 // used for customization, holds registry subkey name.
    CString m_strValueName;              // used for customization, holds registry value.

    COLORREF m_clrBtnHilight;
    COLORREF m_clrBtnShadow;
    COLORREF m_clrBtnFace;
    COLORREF m_clrMask; // transparent color for image lists
    HGLOBAL m_hGlobal;
    LPCTSTR m_lpszResourceName; // pointer to the resource name of the bitmap to be loaded
    CImageList m_imgNormal;     // normal image list
    CImageList m_imgHot;        // hot image list
    CImageList m_imgDisabled;   // disabled image list

    CDWordArray m_arNoTextOnRight;      // array of buttons, which have not text with TBSTYLE_LIST
    CByteArray m_arDisabledTextOptions; // array of disabled text options
    CByteArray m_arDisabledIconOptions; // array of disabled icon options
    BOOL m_bHideCtrlsOnVertical;        // hide the windows when docked vertical
    BOOL m_bIsCustomizeEnable;
    BOOL m_bVertAlign;

    //{{AFX_MSG(CToolBarX)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnSysColorChange();
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnNcPaint();
    //}}AFX_MSG
    afx_msg void OnQueryDelete(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnQueryInsert(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChange(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnCustomHelp(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnEndAdjust(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReset(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnInitCustomize(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult);
    BOOL OnBtnDropDown(NMHDR* pNMHDR, LRESULT* pRes);
    // Saving and restoring toolbar
    afx_msg void OnSave(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRestore(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
    DECLARE_DYNCREATE(CToolBarX)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(TOOLBAR_X_INCLUDED)
