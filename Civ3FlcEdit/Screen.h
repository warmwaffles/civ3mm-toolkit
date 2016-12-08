#if !defined(_CSCREEN_H_INCULDE_)
#define _CSCREEN_H_INCULDE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScreen
{
  public:
    CScreen();
    virtual ~CScreen();

  private:
    struct Flags
    {
        int width;
        int height;
        BOOL bWorking;
        //		BOOL	fullScreen;
        //		int		bpp;
    } flags;

    HWND m_hWnd;

    LPDIRECTDRAW m_lpDD;
    LPDIRECTDRAWCLIPPER m_lpClipper; // the DirectDraw clipper, used for window mode only

    CSurface* m_pFrontSurf;
    CSurface* m_pBackSurf;

  public:
    HRESULT Init(HWND hWnd);
    void Shutdown(void);
    BOOL IsWorking()
    {
        return flags.bWorking;
    }

    HRESULT Fill(DWORD fillColor);
    HRESULT Flip(void);

    HRESULT CreateWindowedMode(int nWidth, int nHeight);
    HRESULT CreateWindowedMode();
    //	HRESULT CreateFullScreenMode(int nWidth, int nHeight, int _bpp);

    LPDIRECTDRAW GetDD(void)
    {
        return m_lpDD;
    }

    CSurface* GetBack(void)
    {
        return m_pBackSurf;
    }
    CSurface* GetFront(void)
    {
        return m_pFrontSurf;
    }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CSCREEN_H_INCULDE_)