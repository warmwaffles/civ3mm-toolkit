#if !defined(MENUMANAGER_H__MFCAIDS__INCLUDED_)
#define MENUMANAGER_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

namespace PxLib {
extern void FillRect(CDC& dc, const CRect& rc, COLORREF color);
extern void DrawEmbossed(CDC& dc, CImageList& il, int i, CPoint p, BOOL bColor = FALSE);

extern HBITMAP LoadSysColorBitmap(LPCTSTR lpResName, BOOL bMono = FALSE);
inline HBITMAP
LoadSysColorBitmap(UINT nResID, BOOL bMono = FALSE)
{
    return LoadSysColorBitmap(MAKEINTRESOURCE(nResID), bMono);
}

} // end namespace

//////////////////
// Generic class to hook messages on behalf of a CWnd.
// Once hooked, all messages go to CSubclassWnd::WindowProc before going
// to the window. Specific subclasses can trap messages and do something.
//
// To use:
//
// * Derive a class from CSubclassWnd.
//
// * Override CSubclassWnd::WindowProc to handle messages. Make sure you call
//   CSubclassWnd::WindowProc if you don't handle the message, or your
//   window will never get messages. If you write seperate message handlers,
//   you can call Default() to pass the message to the window.
//
// * Instantiate your derived class somewhere and call HookWindow(pWnd)
//   to hook your window, AFTER it has been created.
//	  To unhook, call HookWindow(NULL).
//
// This is a very important class, crucial to many of the widgets Window
// widgets implemented in PixieLib. To see how it works, look at the HOOK
// sample program.
//
class CSubclassWnd : public CObject
{
  public:
    DECLARE_DYNAMIC(CSubclassWnd);
    CSubclassWnd();
    ~CSubclassWnd();

    // Subclass a window. Hook(NULL) to unhook (automatic on WM_NCDESTROY)
    BOOL HookWindow(CWnd* pRealWnd);
    BOOL IsHooked()
    {
        return m_pWndHooked != NULL;
    }

    friend LRESULT CALLBACK HookWndProc(HWND, UINT, WPARAM, LPARAM);
    friend class CSubclassWndMap;

  protected:
    CWnd* m_pWndHooked;    // the window hooked
    WNDPROC m_pOldWndProc; // ..and original window proc
    CSubclassWnd* m_pNext; // next in chain of hooks for this window

    // Override this to handle messages in specific handlers
    virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
    LRESULT Default(); // call this at the end of handler fns
};

//////////////////
// CMenuManager implements "cool" menus with buttons in them. To use:
//
//  * Instantiate in your CMainFrame.
//	* Call Install to install it
//  * Call LoadToolbars or LoadToolbar to load toolbars
//
//  Don't forget to link with CoolMenu.cpp, Subclass.cpp and DrawTool.cpp!
//
class MFCAIDS_API CMenuManager : private CSubclassWnd
{
  public:
    void ShowButtons(BOOL bShow);
    void ShowDisabledButtonsInColor(BOOL bShow);
    void ShowAutoAccel(BOOL bShow);
    void SetBackgroundColor(COLORREF bgdColor);
    DECLARE_DYNAMIC(CMenuManager)
    CMenuManager();
    ~CMenuManager();

    // public functions to use
    void Install(CFrameWnd* pWnd);                                            // connect to main frame
    BOOL LoadToolbar(UINT nID);                                               // load one toolbar
    BOOL AddToolbar(UINT nIdBase, UINT nIdNormal = NULL, UINT nIdHot = NULL); // load one toolbar

    BOOL SetNormalImageList(UINT id, DWORD rgbBg = NULL);   // set new image list for non-selected menu options.
    BOOL SetHotImageList(UINT id, DWORD rgbBg = NULL);      // set new image list for selected menu options.
    BOOL SetDisabledImageList(UINT id, DWORD rgbBg = NULL); // set new image list for for disabled menu options.

    // should never need to call:
    virtual void Destroy();           // destroys everything--to re-load new toolbars?
    virtual void Refresh();           // called when system colors, etc change
    static HBITMAP GetMFCDotBitmap(); // get..
    static void FixMFCDotBitmap();    // and fix MFC's dot bitmap

    static BOOL bTRACE; // Set TRUE to see extra diagnostics in DEBUG code

  protected:
    CFrameWnd* m_pFrame;          // window I belong to
    CUIntArray m_arToolbarID;     // array of toolbar IDs loaded
    CImageList m_ilNormal;        // image list for non-selected menu options
    CImageList m_ilHot;           // image list for selected menu options
    CImageList m_ilDisabled;      // image list for disabled menu options
    CMapWordToPtr m_mapIDtoImage; // maps command ID -> image list index
    CMapWordToPtr m_mapIDtoAccel; // maps command ID -> ACCEL*
    HACCEL m_hAccel;              // current accelerators, if any
    ACCEL* m_pAccel;              // ..and table in memory
    CPtrList m_menuList;          // list of HMENU's initialized
    CSize m_szImg;                // button image size
    CSize m_szBitmap;             // size of button bitmap
    CSize m_szButton;             // size of button (including shadow)
    CFont m_fontMenu;             // menu font

    COLORREF m_bgdRGB;     // background color (transparent)
    BOOL m_bShowButtons;   // use to control whether buttons are shown
    BOOL m_bAutoAccel;     // generate auto accelerators
    BOOL m_bUseDrawState;  // use ::DrawState for disabled buttons
    BOOL m_bColorDisabled; // draw disabled buttons in color
                           // (only if m_bUseDrawState = FALSE)

    // helpers
    BOOL AddToImageList(CImageList* pImg, UINT nId, INT nBtnCount);
    void DestroyAccel();
    void DrawMenuText(CDC& dc, CRect rc, CString text, COLORREF color);
    BOOL Draw3DCheckmark(CDC& dc, const CRect& rc, BOOL bSelected, HBITMAP hbmCheck = NULL);
    void ConvertMenu(CMenu* pMenu, UINT nIndex, BOOL bSysMenu, BOOL bShowButtons);
    void LoadAccel(HACCEL hAccel);
    BOOL AppendAccelName(CString& sItemName, UINT nID);
    BOOL RemoveAccelName(CString& sItemName);
    CFont* GetMenuFont();

    // Get button index for given command ID, or -1 if not found
    int GetButtonIndex(WORD nID)
    {
        void* val;
        return m_mapIDtoImage.Lookup(nID, val) ? (int)val : -1;
    }

    // Get ACCEL structure associated with a given command ID
    ACCEL* GetAccel(WORD nID)
    {
        void* val;
        return m_mapIDtoAccel.Lookup(nID, val) ? (ACCEL*)val : NULL;
    }

    // window proc to hook frame using CSubclassWnd implementation
    virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);

    // CSubclassWnd message handlers
    virtual void OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu);
    virtual BOOL OnMeasureItem(LPMEASUREITEMSTRUCT lpms);
    virtual BOOL OnDrawItem(LPDRAWITEMSTRUCT lpds);
    virtual LONG OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
    virtual void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
};

//////////////////
// Friendly version of MENUITEMINFO initializes itself
//
struct CMenuItemInformation : public MENUITEMINFO
{
    CMenuItemInformation()
    {
        memset(this, 0, sizeof(MENUITEMINFO));
        cbSize = sizeof(MENUITEMINFO);
    }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(MENUMANAGER_H__MFCAIDS__INCLUDED_)
