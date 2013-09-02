#if !defined(_CSURFACE_H_INCULDE_)
#define _CSURFACE_H_INCULDE_

#include <ddraw.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScreen;
class CSurface
{
public:
	CSurface(CScreen* pScreen);
	virtual ~CSurface();

	struct Flags
	{
		short	width;
		short	height;
		BYTE*	pixelData;

		Flags()
		{
			width		= 0;
			height		= 0;
			pixelData	= NULL;
		}
	} flags;

	LPDIRECTDRAWSURFACE	m_lpDDs;
	CScreen*			m_pScreen;

public:

	HRESULT Create(void);
	HRESULT Create(int nWidth, int nHeight);
	void	Draw(int nPosX, int nPosY);

	void	SetDDS(LPDIRECTDRAWSURFACE lpdds) { m_lpDDs = lpdds; }
	void	SetDDS(LPDIRECTDRAWSURFACE lpdds, int nWidth, int nHeight) { m_lpDDs = lpdds; flags.width = nWidth; flags.height = nHeight; }
	
//	HRESULT CopyRectTo(CSurface* pSurface, RECT* rect);
	LPDIRECTDRAWSURFACE GetDDS(void) { return m_lpDDs; }
	CScreen* GetScreen() { return m_pScreen; }

	//DDSURFACEDESC GetDDSD(void){return ddsd;}
	//void SetDDSD(DDSURFACEDESC _ddsd){ddsd=_lpdds;}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CSURFACE_H_INCULDE_)

