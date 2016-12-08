#include "stdafx.h"
#include "GraphicsLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSurface::CSurface(CScreen* pScreen)
{
    m_lpDDs   = NULL;
    m_pScreen = pScreen;
}

CSurface::~CSurface()
{
    SAFE_DELETE_MULTI(flags.pixelData);
}

HRESULT
CSurface::Create(void)
{
    return S_OK;
}

HRESULT
CSurface::Create(int nWidth, int nHeight)
{
    DDSURFACEDESC ddsd; // A DDSURFACEDESC object, stores surface information

    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; // | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;               // | DDSCAPS_PALETTE;
    ddsd.dwWidth        = nWidth;
    ddsd.dwHeight       = nHeight;

    /*	ZeroMemory(&ddsd.ddpfPixelFormat, sizeof(DDPIXELFORMAT));
	ddsd.ddpfPixelFormat.dwSize				= sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags			= DDPF_RGB;
	ddsd.ddpfPixelFormat.dwFourCC			= 0;
	ddsd.ddpfPixelFormat.dwRGBBitCount		= BITS_8;
	ddsd.ddpfPixelFormat.dwRBitMask			= 0x00FF0000;
    ddsd.ddpfPixelFormat.dwGBitMask			= 0x0000FF00;
    ddsd.ddpfPixelFormat.dwBBitMask			= 0x000000FF;	
	ddsd.ddpfPixelFormat.dwRGBAlphaBitMask	= 0x00FF00FF;
*/
    flags.width  = nWidth;
    flags.height = nHeight;

    if (m_pScreen->GetDD()->CreateSurface(&ddsd, &m_lpDDs, NULL) != DD_OK)
        return E_FAIL;
    /*	
	HRESULT hRes = g_Screen->GetDD()->CreateSurface(&ddsd, &m_lpDDs, NULL);
	
	int x = 0;
	switch (hRes)
	{
	case DDERR_INVALIDPIXELFORMAT:
		x = 0;
		break;
	case DDERR_INCOMPATIBLEPRIMARY:
		x = 0;
		break;
	case DDERR_INVALIDCAPS:
		x = 0;
		break;
	case DDERR_INVALIDOBJECT:
		x = 0;
		break;
	case DDERR_INVALIDPARAMS:
		x = 0;
		break;
	case DDERR_NOALPHAHW:
		x = 0;
		break;
	case DDERR_NOCOOPERATIVELEVELSET:
		x = 0;
		break;
	case DDERR_NODIRECTDRAWHW:
		x = 0;
		break;
	case DDERR_NOEMULATION:
		x = 0;
		break;
	case DDERR_NOEXCLUSIVEMODE:
		x = 0;
		break;
	case DDERR_NOFLIPHW:
		x = 0;
		break;
	case DDERR_NOMIPMAPHW:
		x = 0;
		break;
	case DDERR_NOOVERLAYHW:
		x = 0;
		break;
	case DDERR_NOZBUFFERHW:
		x = 0;
		break;
	case DDERR_OUTOFMEMORY:
		x = 0;
		break;
	case DDERR_OUTOFVIDEOMEMORY:
		x = 0;
		break;
	case DDERR_PRIMARYSURFACEALREADYEXISTS:
		x = 0;
		break;
	case DDERR_UNSUPPORTEDMODE:
		x = 0;
		break;
	}

	if (hRes != DD_OK)
		return E_FAIL;
*/
    // set default transparent colors
    DDCOLORKEY ddck;
    ddck.dwColorSpaceLowValue  = RGB_TRANSPARENT;
    ddck.dwColorSpaceHighValue = RGB_TRANSPARENT;

    GetDDS()->SetColorKey(DDCKEY_SRCBLT, &ddck);

    return S_OK;
}

void
CSurface::Draw(int nPosX, int nPosY)
{
    RECT rect;

    rect.top    = 0;
    rect.left   = 0;
    rect.right  = flags.width;
    rect.bottom = flags.height;

    if (nPosX < 0) {
        if (nPosX > 0 - flags.width) {
            rect.left = -nPosX;
            nPosX     = 0;
        }
    }

    if (nPosX > 1024 - flags.width)
        rect.right = (nPosX + flags.width) - 1024;

    /*HRESULT retVal = */ m_pScreen->GetBack()->GetDDS()->BltFast(nPosX, nPosY, GetDDS(), &rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}
/*
HRESULT CSurface::CopyRectTo(CSurface* pSurface, RECT* rect)
{
	//D3DLOCKED_RECT srclr, destlr;
	DDSURFACEDESC	srcDDSD, destDDSD;

	ZeroMemory(&srcDDSD, sizeof(srcDDSD));
	srcDDSD.dwSize = sizeof(srcDDSD);

	// Lock this source texture
	if (m_lpDDs->Lock(NULL, &srcDDSD, DDLOCK_WAIT, NULL) != DD_OK)
		return E_FAIL;

	ZeroMemory(&destDDSD, sizeof(destDDSD));
	destDDSD.dwSize = sizeof(destDDSD);

	// Lock the dest texture
	if (pSurface->GetDDS()->Lock(NULL, &destDDSD, DDLOCK_WAIT, NULL) != DD_OK)
		return E_FAIL;

	BYTE *sb = (BYTE*)srcDDSD.lpSurface;
	BYTE *db = (BYTE*)destDDSD.lpSurface;

	// Get the start of the source texture
	sb += (rect->left + ( rect->top * flags.width));

	int x, y;

	for (y = 0; y < rect->bottom-rect->top; y++)
	{
		sb = (BYTE*)srcDDSD.lpSurface;
		sb += (rect->left + ( (rect->top+y) * (flags.width)))*4;

		for (x=0; x < rect->right-rect->left; x++)
		{
			db[0] = sb[0];
			db[1] = sb[1];
			db[2] = sb[2];
			db[3] = sb[3];

			db+=4;
			sb+=4;
		}
	}

	// Now unlock the two textures
	m_lpDDs->Unlock(NULL);
	pSurface->GetDDS()->Unlock(NULL);

	return S_OK;
}
*/