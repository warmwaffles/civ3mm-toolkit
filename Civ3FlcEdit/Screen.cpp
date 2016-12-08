#include "stdafx.h"
#include "GraphicsLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CScreen::CScreen()
{
    m_pFrontSurf   = NULL;
    m_pBackSurf    = NULL;
    m_hWnd         = NULL;
    m_lpDD         = NULL;
    m_lpClipper    = NULL;
    flags.bWorking = FALSE;
}

CScreen::~CScreen()
{
    if (m_lpDD)
        Shutdown();
}

HRESULT
CScreen::Init(HWND hWnd)
{
    m_hWnd      = hWnd;
    m_lpDD      = NULL;
    m_lpClipper = NULL;

    if (DirectDrawCreate(NULL, &m_lpDD, NULL) != DD_OK)
        return E_FAIL;

    return CreateWindowedMode();
    //	return S_OK;
}

void
CScreen::Shutdown(void)
{
    if (m_lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL) != DD_OK) {
        ASSERT(FALSE);
        //		OutputDebugString("Error: CScreen::Shutdown - m_lpDD->SetCooperativeLevel\r\n");
    }

    SAFE_DELETE(m_pFrontSurf);
    SAFE_DELETE(m_pBackSurf);

    SAFE_RELEASE(m_lpClipper);
    SAFE_RELEASE(m_lpDD);

    flags.bWorking = FALSE;
}
/*
HRESULT CScreen::CreateFullScreenMode(int nWidth, int nHeight, int _bpp)
{
	DDSURFACEDESC ddsd;
	LPDIRECTDRAWSURFACE lpdds;

	flags.fullScreen	= TRUE;
	flags.width			= nWidth;
	flags.height		= nHeight;
	flags.bpp			= _bpp;
	
	if (m_lpDD->SetCooperativeLevel(m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT) != DD_OK) 
		return E_FAIL;
	
	if (m_lpDD->SetDisplayMode(flags.width, flags.height, flags.bpp) != DD_OK) 
		return E_FAIL;
	
	m_pFrontSurf = new CSurface(this);
	m_pBackSurf  = new CSurface(this);
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize				= sizeof(ddsd);
	ddsd.dwFlags			= DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps		= DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY;
	ddsd.dwBackBufferCount	= 1;
	
	if (m_lpDD->CreateSurface(&ddsd, &lpdds, NULL) != DD_OK)
	{
		SAFE_DELETE(m_pFrontSurf);
		SAFE_DELETE(m_pBackSurf);
		
		return E_FAIL;
	}

	m_pFrontSurf->SetDDS(lpdds);
	
	DDSCAPS ddscaps;
	
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	
	if (GetFront()->GetDDS()->GetAttachedSurface(&ddscaps, &lpdds) != DD_OK)
		OutputDebugString("Error: CScreen::CreateFullScreenMode - GetFront()->GetDDS()->GetAttachedSurface\r\n");
	
	GetBack()->SetDDS(lpdds);
	
	return S_OK;
}
*/
HRESULT
CScreen::CreateWindowedMode()
{
    return CreateWindowedMode(WND_W, WND_CY);
}

HRESULT
CScreen::CreateWindowedMode(int nWidth, int nHeight)
{
    //	HDC hDC;
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE lpdds;

    flags.width  = nWidth;
    flags.height = nHeight;

    //	hDC = GetDC(NULL);
    //	if (hDC != NULL)
    //	{
    //		flags.bpp = GetDeviceCaps(hDC, PLANES) * GetDeviceCaps(hDC, BITSPIXEL);
    //		ReleaseDC(NULL, hDC);
    //	}
    //	else
    //		return E_FAIL;

    if (m_lpDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL) != DD_OK)
        return E_FAIL;

    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if (m_lpDD->CreateSurface(&ddsd, &lpdds, NULL) != DD_OK)
        return E_FAIL;

    m_pFrontSurf = new CSurface(this);
    m_pBackSurf  = new CSurface(this);

    m_pFrontSurf->SetDDS(lpdds);

    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; //|DDSCAPS_ALPHA;
    ddsd.dwWidth        = flags.width;
    ddsd.dwHeight       = flags.height;

    if (m_lpDD->CreateSurface(&ddsd, &lpdds, NULL) != DD_OK) {
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

        if (m_lpDD->CreateSurface(&ddsd, &lpdds, NULL) != DD_OK) {
            SAFE_DELETE(m_pFrontSurf);
            SAFE_DELETE(m_pBackSurf);

            return E_FAIL;
        }
    }

    m_pBackSurf->SetDDS(lpdds, flags.width, flags.height);

    if (m_lpDD->CreateClipper(0, &m_lpClipper, NULL) != DD_OK) {
        SAFE_DELETE(m_pFrontSurf);
        SAFE_DELETE(m_pBackSurf);

        return E_FAIL;
    }

    //CDXLOG("Set Hwnd (%i) for clipper", hWnd);
    if (m_lpClipper->SetHWnd(0, m_hWnd) != DD_OK) {
        SAFE_DELETE(m_pFrontSurf);
        SAFE_DELETE(m_pBackSurf);

        SAFE_RELEASE(m_lpClipper);

        return E_FAIL;
    }

    if (m_pFrontSurf->GetDDS()->SetClipper(m_lpClipper) != DD_OK) {
        SAFE_DELETE(m_pFrontSurf);
        SAFE_DELETE(m_pBackSurf);

        SAFE_RELEASE(m_lpClipper);

        return E_FAIL;
    }

    flags.bWorking = TRUE;
    return S_OK;
}

HRESULT
CScreen::Fill(DWORD fillColor)
{
    if (!m_pBackSurf)
        return S_OK;

    DDBLTFX ddBltFx;
    ddBltFx.dwSize      = sizeof(DDBLTFX);
    ddBltFx.dwFillColor = fillColor;

    if (m_pBackSurf->GetDDS()->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddBltFx) != DD_OK)
        return E_FAIL;

    return S_OK;
}

HRESULT
CScreen::Flip(void)
{
    if (!m_pFrontSurf)
        return S_OK;

    HRESULT retVal;
    RECT window, rSrc;
    POINT pt;

    //	if (flags.fullScreen == TRUE)
    //	{
    //		GetFront()->GetDDS()->Flip(NULL, DDFLIP_WAIT);
    //	}
    //	else
    //	{
    GetClientRect(m_hWnd, &window);
    pt.x = pt.y = 0;
    ClientToScreen(m_hWnd, &pt);
    OffsetRect(&window, pt.x, pt.y);

    m_lpDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);

    rSrc.left   = 0;
    rSrc.top    = 0;
    rSrc.right  = window.right - window.left;
    rSrc.bottom = window.bottom - window.top;
    retVal      = m_pFrontSurf->GetDDS()->Blt(&window, m_pBackSurf->GetDDS(), &rSrc, DDBLT_WAIT, NULL);

    if (retVal == DDERR_SURFACELOST) {
        if (m_pFrontSurf->GetDDS()->IsLost() != DD_OK)
            m_pFrontSurf->GetDDS()->Restore();

        if (m_pBackSurf->GetDDS()->IsLost() != DD_OK)
            m_pBackSurf->GetDDS()->Restore();
    }
    //	}

    return S_OK;
}
