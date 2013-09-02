/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 GraphXImage.cpp
	CREATED:	 09.01.2004
	AUTHOR:		 Dreyk
	VERSION:     1.4

	MODIFIED:	

	DESCRIPTION: implementation of the CGraphXImage class, wrapper for GraphX
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Resource.h"
#include "GraphXImage.h"

#include "Utilities.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphXImage::CGraphXImage()
{
	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
}

CGraphXImage::CGraphXImage(LPCTSTR sPath)
{
	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
	LoadFile(sPath);
}

CGraphXImage::~CGraphXImage()
{
	CloseFile();
}

//////////////////////////////////////////////////////////////////////
// Load image from file
//
void CGraphXImage::LoadFile(LPCTSTR sPath)
{
	CloseFile();
	m_sPath = sPath;

	GRAPHX::FileInfo(sPath, &m_fi, FILEINFO_TOTALPAGES, NULL);
	GRAPHX::LoadFile((LPTSTR)sPath, &m_hbmp, m_fi.BitsPerPixel, ORDER_BGR, LOADFILE_ALLOCATE | LOADFILE_STORE, NULL, NULL, NULL, NULL);
}

//////////////////////////////////////////////////////////////////////
// Load image from file with try-catch...
//
void CGraphXImage::LoadFileSafely(LPCTSTR sPath)
{
	CloseFile();
	m_sPath = sPath;

	try
	{
		GRAPHX::FileInfo(sPath, &m_fi, FILEINFO_TOTALPAGES, NULL);
		GRAPHX::LoadFile((LPTSTR)sPath, &m_hbmp, m_fi.BitsPerPixel, ORDER_BGR, LOADFILE_ALLOCATE | LOADFILE_STORE, NULL, NULL, NULL, NULL);
	}
	catch (CGraphXException* e)
	{
//		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
	}
	catch (CFileException *e)
	{
//		TCHAR szCause[1024];
//		e->GetErrorMessage(szCause, 1024);
//		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
	}
}

void CGraphXImage::CloseFile()
{
	m_sPath.Empty();
//	try
//	{
		if (m_hbmp.Flags.Allocated)
			GRAPHX::FreeBitmap(&m_hbmp);
//	}
//	catch (CGraphXException *e)
//	{
//		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
//		e->Delete();
//	}
//	catch (...)
//	{
//	}
}

//////////////////////////////////////////////////////////////////////
// Draw the picture by the specified mode
//
void CGraphXImage::Render(CDC *pDC, CRect &rcDst, UINT nMode /*= PaintOriginal*/)
{
	if (!IsLoaded())
		return;

	if (nMode == PaintProportion)
	{
		INT nOrgWidth  = rcDst.Width();
		INT nOrgHeight = rcDst.Height();
		CRect rcPlay(0, 0, m_hbmp.Width, m_hbmp.Height);
		
		if (m_hbmp.Width > nOrgWidth)
		{
			if (m_hbmp.Height * nOrgWidth / m_hbmp.Width > nOrgHeight)
			{
				rcPlay.right  = m_hbmp.Width * nOrgHeight / m_hbmp.Height;
				rcPlay.bottom = nOrgHeight;
			}
			else
			{
				rcPlay.bottom = m_hbmp.Height * nOrgWidth / m_hbmp.Width;
				rcPlay.right  = nOrgWidth;
			}
		}
		else
		if (m_hbmp.Height > nOrgHeight)
		{
			rcPlay.right  = m_hbmp.Width * nOrgHeight / m_hbmp.Height;
			rcPlay.bottom = nOrgHeight;
		}
		else
		if (m_hbmp.Width < nOrgWidth)
		{
			if (m_hbmp.Height * nOrgWidth / m_hbmp.Width > nOrgHeight)
			{
				rcPlay.right  = m_hbmp.Width * nOrgHeight / m_hbmp.Height;
				rcPlay.bottom = nOrgHeight;
			}
			else
			{
				rcPlay.bottom = m_hbmp.Height * nOrgWidth / m_hbmp.Width;
				rcPlay.right  = nOrgWidth;
			}
		}
		
		rcPlay.OffsetRect(rcDst.left, rcDst.top);
		rcDst = rcPlay;
	}
	else
	if (nMode == PaintOriginal)
	{
		CSize _DPI(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
		CSize __DPI(m_hbmp.XResolution, m_hbmp.YResolution);
		CRect rect(0, 0, MulDiv(m_hbmp.Width, _DPI.cx, __DPI.cx), MulDiv(m_hbmp.Height, _DPI.cy, __DPI.cy));
		rect.OffsetRect(rcDst.TopLeft());
		rcDst = rect;
	}
	else
	if (nMode == PaintProportion2)
	{
		INT nOrgWidth  = rcDst.Width();
		INT nOrgHeight = rcDst.Height();
		CRect rcPlay(0, 0, m_hbmp.Width, m_hbmp.Height);
		
		if (nOrgWidth > m_hbmp.Width && nOrgHeight > m_hbmp.Height)
		{
			// nothing doing: we should draw with original size... ;)
		}
		else
		if (m_hbmp.Width > nOrgWidth)
		{
			if (m_hbmp.Height * nOrgWidth / m_hbmp.Width > nOrgHeight)
			{
				rcPlay.right  = m_hbmp.Width * nOrgHeight / m_hbmp.Height;
				rcPlay.bottom = nOrgHeight;
			}
			else
			{
				rcPlay.bottom = m_hbmp.Height * nOrgWidth / m_hbmp.Width;
				rcPlay.right  = nOrgWidth;
			}
		}
		else
		if (m_hbmp.Height > nOrgHeight)
		{
			rcPlay.right  = m_hbmp.Width * nOrgHeight / m_hbmp.Height;
			rcPlay.bottom = nOrgHeight;
		}
		else
		if (m_hbmp.Width < nOrgWidth)
		{
			if (m_hbmp.Height * nOrgWidth / m_hbmp.Width > nOrgHeight)
			{
				rcPlay.right  = m_hbmp.Width * nOrgHeight / m_hbmp.Height;
				rcPlay.bottom = nOrgHeight;
			}
			else
			{
				rcPlay.bottom = m_hbmp.Height * nOrgWidth / m_hbmp.Width;
				rcPlay.right  = nOrgWidth;
			}
		}
			
		rcPlay.OffsetRect(rcDst.left, rcDst.top);
		rcDst = rcPlay;
	}
	
//	static L_INT PaintDC ( HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pSrc, LPRECT pClipSrc, LPRECT pDst, LPRECT pClipDst, L_UINT32 uROP3)
	GRAPHX::PaintDC(pDC->m_hDC, &m_hbmp, NULL, NULL, rcDst, NULL, SRCCOPY);
}

//////////////////////////////////////////////////////////////////////
// Draw specified part of the image
//
void CGraphXImage::DrawClipRect(CDC *pDC, CRect rcDst, CRect rcClip, UINT nMode /*= PaintOriginal*/)
{
	if (!IsLoaded())
		return;

//	static L_INT PaintDC ( HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pSrc, LPRECT pClipSrc, LPRECT pDst, LPRECT pClipDst, L_UINT32 uROP3)
	GRAPHX::PaintDC(pDC->m_hDC, &m_hbmp, rcClip, NULL, rcDst, NULL, SRCCOPY);
}

//////////////////////////////////////////////////////////////////////
// Check is image loaded 
//
BOOL CGraphXImage::IsLoaded()
{
	return (m_hbmp.Flags.Allocated);
}

//////////////////////////////////////////////////////////////////////
// Get rect with image sizes
//
void CGraphXImage::GetFrame(CDC *pDC, CRect &rc)
{
	if (IsLoaded())
	{
		rc.left   = rc.top = 0;
		rc.right  = m_hbmp.Width;
		rc.bottom = m_hbmp.Height;
	}
}

//////////////////////////////////////////////////////////////////////
// Get loaded file name
//
CString CGraphXImage::GetFileName()
{
	return m_sPath;
}

//////////////////////////////////////////////////////////////////////
// Get information about image
//
FILEINFO* CGraphXImage::GetImgFileInfo()
{
	return &m_fi;
}

///////////////////////////////////////////////////////////////////
// Changing of color resolution accordint to nBitsPerPixel.
//
BOOL CGraphXImage::SetColorMode(UINT nBitsPerPixel)
{
	if (!IsLoaded())
		return FALSE;

	try
	{
		if (nBitsPerPixel == 0)				// grayscale 8bit
			GRAPHX::GrayScaleBitmap (&m_hbmp, 8);
		else 
		if (nBitsPerPixel == 1)				// black & white
			GRAPHX::ColorResBitmap (&m_hbmp, &m_hbmp, 1, CRF_NODITHERING | CRF_FIXEDPALETTE,
				NULL, NULL, 0, NULL, NULL);
		else 
		if (nBitsPerPixel == 8)				// 256 colors
			GRAPHX::ColorResBitmap (&m_hbmp, &m_hbmp, 8, CRF_NODITHERING | CRF_FIXEDPALETTE,
				NULL, NULL, 0, NULL, NULL);
		else 
		if (nBitsPerPixel == 24)			// true color
			GRAPHX::ColorResBitmap (&m_hbmp, &m_hbmp, 24, CRF_BYTEORDERBGR,
				NULL, NULL, 0, NULL, NULL);
		else
			return FALSE;					// not determined BPP
	}
	catch (CGraphXException* e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////
// Resize image by the given width and height.
//
BOOL CGraphXImage::ResizeImage(UINT cx, UINT cy)
{
	if (!IsLoaded())
		return FALSE;

	try
	{
		GRAPHX::SizeBitmap(&m_hbmp, cx, cy);
		if (m_hbmp.BitsPerPixel != m_fi.BitsPerPixel)
			SetColorMode(m_fi.BitsPerPixel);
	}
	catch (CGraphXException* e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Save image as PCX file 
//
BOOL CGraphXImage::ConvertToPCX()
{
	return ConvertTo(FILE_PCX, "pcx");
}

//////////////////////////////////////////////////////////////////////
// Save image as BMP file 
//
BOOL CGraphXImage::ConvertToBMP()
{
	return ConvertTo(FILE_BMP, "bmp");
}

//////////////////////////////////////////////////////////////////////
// Save changes
//
BOOL CGraphXImage::SaveChangesToFile()
{
	if (!IsLoaded())
		return FALSE;

	BOOL	bRes		= TRUE;
	CString sTempFile	= uGetTempFileName();

	try
	{
		CopyFile(m_sPath, sTempFile, FALSE);

		BITMAPHANDLE* pBmh;
		
		pBmh = &m_hbmp;
		if (pBmh != NULL)
		{
			// Change resolution (to save in original size)
			if (pBmh->XResolution > pBmh->YResolution)
				pBmh->XResolution = pBmh->YResolution;
			else
				pBmh->XResolution = pBmh->YResolution;

			// Save bitmap to the file
			GRAPHX::SaveBitmap(m_sPath, pBmh, m_fi.Format, 0, 0);
		}
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		bRes = FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		bRes = FALSE;
	}

	// Rename temporary file with old name
	if (!bRes)
		CopyFile(sTempFile, m_sPath, FALSE);
	
	DeleteFile(sTempFile);

	return bRes;
}

//////////////////////////////////////////////////////////////////////
// Save image as specified file type
//
BOOL CGraphXImage::ConvertTo(UINT nFileType, LPCTSTR sNewExt /*= NULL*/)
{
	if (!IsLoaded())
		return FALSE;
	
	BOOL	bRes		= TRUE;
	CString sTempFile	= uGetTempFileName();
	
	if (sNewExt != NULL)
	{
		// if current file has a different extention
		if (uGetFileExt(m_sPath).CompareNoCase(sNewExt) != 0)
		{
			CString sNewPath(m_sPath);
			sNewPath = m_sPath.Left(m_sPath.GetLength()-3);
			sNewPath += sNewExt;

			if (uIsFileExist(sNewPath))
			{
				if (AfxMessageBox(uLocalFormatString(IDS_ASK_REWRITE_FILE, sNewPath), MB_YESNO) != IDYES)
					return FALSE;
			}

			m_sPath = sNewPath;
		}
	}

	try
	{
		CopyFile(m_sPath, sTempFile, FALSE); // make temporary copy of file
		
		BITMAPHANDLE* pBmh;
		
		pBmh = &m_hbmp;
		if (pBmh != NULL)
		{
			// Change resolution (to save in original size)
			if (pBmh->XResolution > pBmh->YResolution)
				pBmh->XResolution = pBmh->YResolution;
			else
				pBmh->XResolution = pBmh->YResolution;

			// Save bitmap as specified file type
			GRAPHX::SaveBitmap(m_sPath, pBmh, nFileType);
		}
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		bRes = FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		bRes = FALSE;
	}
	
	// Rename temporary file with old name
	if (!bRes)
		CopyFile(sTempFile, m_sPath, FALSE);
	
	DeleteFile(sTempFile);
	return bRes;
}
