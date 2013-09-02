#if !defined(_CPCX_H_INCLUDE_)
#define _CPCX_H_INCLUDE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Definitions.h"

class CPcx
{
public:
	CPcx();
	~CPcx();
	
	BOOL LoadBgdImgFile(LPCTSTR lpszPathName);
	void DestroyPcx(void);
	HRESULT CopyToSurface(CSurface *pSurface);
	
	RGBQUADPAL      m_rgbPal;
	
protected:
	typedef struct _PCX_HEADER
	{
		char	Manufacturer;
		char	Version;
		char	Encoding;
		char	BitsPerPixel;
		short	Xmin;
		short	Ymin;
		short	Xmax;
		short	Ymax;
		short	HDpi;
		short	VDpi;
		char	Colormap[48];
		char	Reserved;
		char	NPlanes;
		short	BytesPerLine;
		short	PaletteInfo;
		short	HscreenSize;
		short	VscreenSize;
		char	Filler[54];
	} PCX_HEADER;
	
	int DecodePcxLine(char *dest, BYTE* &src, int bytes);
	void SetupBitmapInfo(void);

	BITMAPINFO*		m_pBitmapInfo;
	BYTE*			m_pStorage;
	BYTE*			m_pData;
	PCX_HEADER		m_pcxHeader;

	short			m_nWidth;
	short			m_nHeight;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CPCX_H_INCLUDE_)
