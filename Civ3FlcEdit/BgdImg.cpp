/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	BgdImg.cpp
	CREATED:	18.01.2005
	VERSION:	1.0

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CBgdImg class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "BgdImg.h"
#include "Civ3FlcEdit.h"

#include "GraphicsLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBgdImg::CBgdImg()
{
	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
}

CBgdImg::CBgdImg(LPCTSTR sPath)
{
	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
	LoadFile(sPath);
}

CBgdImg::~CBgdImg()
{
	CloseFile();
}

//////////////////////////////////////////////////////////////////////////
// Load background image by the given path.
// Get image palette.
// Get image sizes. 
//
BOOL CBgdImg::LoadBgdImgFile(LPCTSTR sPath)
{
	try
	{
		LoadFileSafely(sPath);
		
		if (!IsLoaded())
			return FALSE;
		
		if (m_hbmp.Width != WND_W || m_hbmp.Height != WND_H)
			GRAPHX::SizeBitmap(&m_hbmp, WND_W, WND_H);

		/////////////////////////////////////////////////////////
		// In case it's PCX file - convert it to BMP and reload again
		if (m_fi.Format == FILE_PCX)
		{
			CString  sTempFile = uGetTempFileName("BG");
			
			// create copy of an image file
			if (!CopyFile(m_sPath, sTempFile, FALSE))
			{
				AfxMessageBox(uFormatString(IDS_ERR_SB_LOAD, m_sPath), MB_ICONERROR);
				return FALSE;
			}
			
			// Save bitmap as BMP file type
			GRAPHX::SaveBitmap(sTempFile, &m_hbmp, FILE_BMP, m_fi.BitsPerPixel);//m_constBits);
			if (GRAPHX::LoadFile((LPTSTR)(LPCTSTR)sTempFile, &m_hbmp, m_fi.BitsPerPixel/*m_constBits*/, ORDER_BGR, LOADFILE_ALLOCATE | LOADFILE_STORE, NULL, NULL, NULL, NULL) != SUCCESS)
			{
				AfxMessageBox(uFormatString(IDS_ERR_SB_LOAD, m_sPath), MB_ICONERROR);
				return FALSE;
			}
			
			DeleteFile(sTempFile);
		}

		if (m_hbmp.BitsPerPixel != BITS_8)
		{
			// Convert the current bitmap to 8 bits per pixel with an optimized palette
			GRAPHX::ColorResBitmap(&m_hbmp, &m_hbmp, 8,
				CRF_FLOYDSTEINDITHERING|CRF_OPTIMIZEDPALETTE, 
				NULL, NULL, 0, NULL, NULL );
		}

		switch (m_fi.Format)
		{
		case FILE_PCX:			// ZSoft PCX
		case FILE_BMP:			// Windows Bitmap
		case FILE_GIF:			// CompuServe GIF
		case FILE_WIN_ICO:		// Windows Icon
		case FILE_WMF:			// Windows Meta File
		case FILE_EMF:			// Extended Meta File
			{
				if (GRAPHX::RotateBitmap(&m_hbmp, -9000, ROTATE_RESIZE, NULL) != SUCCESS)
					return FALSE;
				break;
			}

		case FILE_LEAD1JFIF:	// JPEG  4:1:1
		case FILE_LEAD1JTIF:	// JPEG  4:1:1
		case FILE_LEAD2JFIF:	// JPEG  4:2:2
		case FILE_LEAD2JTIF:	// JPEG  4:2:2
		case FILE_EXIF:			// uncompressed RGB Exif file
		case FILE_EXIF_YCC:		// uncompressed YCbCr Exif file
		case FILE_EXIF_JPEG:	// JPEG 4:2:2 compressed Exif file
			{
				if (GRAPHX::FlipBitmap(&m_hbmp) != SUCCESS)
					return FALSE;
				
				if (GRAPHX::RotateBitmap(&m_hbmp, 9000, ROTATE_RESIZE, NULL) != SUCCESS)
					return FALSE;
				break;
			}
		}

		m_nWidth	= m_hbmp.Width;
		m_nHeight	= m_hbmp.Height;

//		GRAPHX::SaveBitmap("D:\\FLICster\\Test\\test.bmp", &m_hbmp, FILE_BMP, m_hbmp.BitsPerPixel);//!!!
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CBgdImg::LoadBgdImgFile"), MB_ICONERROR);
		return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Copy background image to the surface.
//
HRESULT CBgdImg::CopyToSurface(CSurface *pSurface)
{
	if (!IsLoaded())
		return E_FAIL;

	DDSURFACEDESC	ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (pSurface->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
		return E_FAIL;
	
	BYTE*			db			= (BYTE*)ddsd.lpSurface;
	BYTE*			line		= NULL;
	UINT			pos			= 0;

	BYTE			idx			= 0;
	RGBQUAD*		pRgb		= NULL;

	UINT			nPixelsSize	= m_nWidth * m_nHeight;
	BYTE*			pBuffer		= new BYTE[nPixelsSize];

/////////////////////////
//clock - start
#ifdef _DEBUG
	clock_t start, finish;
	double  duration;
	start = clock();
#endif
/////////////////////////

	try
	{
		// Get current frame's data
		UINT x = 0;
		COLORREF color;
		for (UINT y = 0; y < m_nHeight; y++)
		{
			line = db;
			for (x = 0; x < m_nWidth; x++)
			{
				// get color of the current bitmap pixel
				color = GRAPHX::GetPixelColor(&m_hbmp, x, y);

				// set color for current pixel of the surface
				line[0] = GetBValue(color);
				line[1] = GetGValue(color);
				line[2] = GetRValue(color);
				line[3] = 255;
				
				line += 4;
			}//for (x = 0; x < m_nWidth; x++)
			db += (ddsd.lPitch);
		}//for (y = 0; y < m_nHeight; y++)
		
		// Copy pixels data to the Surface
		pSurface->flags.pixelData = new BYTE[nPixelsSize];
		memcpy(pSurface->flags.pixelData, pBuffer, nPixelsSize);
		
		pSurface->GetDDS()->Unlock(NULL);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}
	catch (CMemoryException* e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		SAFE_DELETE_MULTI(pBuffer);
		return E_FAIL;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CBgdImg::CopyToSurface"), MB_ICONERROR);
		SAFE_DELETE_MULTI(pBuffer);
		return E_FAIL;
	}

/////////////////////
// clock - finish
#ifdef _DEBUG
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	TRACE1("\nCBgdImg::CopyToSurface: %2.3f seconds\n", duration);
#endif
//////////////////////

	return S_OK;
}
