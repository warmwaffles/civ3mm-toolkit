/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 FlcImage.cpp
	CREATED:	 01.07.2004
	AUTHOR:		 Dreyk
	VERSION:     5.7

	MODIFIED:	

	DESCRIPTION: implementation of the CFlcImage class, wrapper for GraphX
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FlcImage.h"

#include "GraphicsLib.h"

#include "ExportFlcDlg.h"
#include "NewFlcDlg.h"
#include "FileImgDlg.h"
#include "MainFrm.h"

#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL WriteC3F(CString sPath, C3FSTRUCT c3f)
{
	try
	{
		/////////////////////////////////////////////////
		// Make C3F file
		sPath.Delete(sPath.GetLength()-3, 3);
		sPath += C3F_EXT;
		
		CFile fl;
		if (!fl.Open(sPath, CFile::modeCreate | CFile::modeWrite | CFile::modeRead | CFile::typeBinary))
			return FALSE;
		
		fl.Write(&c3f, sizeof(C3FSTRUCT));
		
		fl.Close();
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::WriteC3F"), MB_ICONERROR);
		return FALSE;
	}
	
	return TRUE;
}

BOOL WriteC3F(CString sPath, 
						 UINT nWidth, UINT nHeight, 
						 BYTE nFrmWidth, BYTE nFrmHeight, 
						 WORD nFrmCount, BYTE nDirCount,
						 WORD nDelay,
						 BYTE nXOffset, BYTE nYOffset,
						 BYTE nFileType, BYTE nPalType,
						 BOOL bLeaderHead)
{
	C3FSTRUCT c3f;
	
	CString sName = CFileX::GetShortFileName(sPath);
	memcpy(c3f.Name, sName, sName.GetLength());
	c3f.Name[sName.GetLength()] = 0;
	
	c3f.Width		= nWidth;
	c3f.Height		= nHeight;
	c3f.FrmWidth	= nFrmWidth;
	c3f.FrmHeight	= nFrmHeight;
	c3f.FrmCount	= nFrmCount;
	c3f.DirCount	= nDirCount;
	c3f.Delay		= nDelay;
	c3f.XOffset		= nXOffset;
	c3f.YOffset		= nYOffset;
	c3f.FileType	= nFileType;
	c3f.PalType		= nPalType;
	c3f.LeaderHead	= bLeaderHead ? 1 : 0;

	return WriteC3F(sPath, c3f);
}

BOOL ReadC3F(CString sPath, C3FSTRUCT *c3f)
{
	try
	{
		CFile fl;
		if (!fl.Open(sPath, CFile::modeRead | CFile::typeBinary))
			return FALSE;
		
		UINT nRead = 0;

		if (fl.GetLength() != sizeof(C3FSTRUCT))
		{
			if (fl.GetLength() != sizeof(C3FSTRUCT_OLD))
			{
				fl.Close();
				return FALSE;
			}
			else
			{
				C3FSTRUCT_OLD c3fOld;
				nRead = fl.Read(&c3fOld, sizeof(C3FSTRUCT_OLD));
				
				if (nRead != sizeof(C3FSTRUCT_OLD))
					return FALSE;

				memcpy(c3f->Name, c3fOld.Name, 64);

				c3f->Width		= c3fOld.Width;
				c3f->Height		= c3fOld.Height;
				c3f->FrmWidth	= c3fOld.FrmWidth;
				c3f->FrmHeight	= c3fOld.FrmHeight;
				c3f->FrmCount	= c3fOld.FrmCount;
				c3f->DirCount	= c3fOld.DirCount;
				c3f->Delay		= c3fOld.Delay;
				c3f->XOffset	= c3fOld.XOffset;
				c3f->YOffset	= c3fOld.YOffset;
				c3f->FileType	= c3fOld.FileType;
				c3f->PalType	= c3fOld.PalType;
				c3f->LeaderHead	= c3fOld.LeaderHead;
			}
		}
		else
		{
			nRead = fl.Read(c3f, sizeof(C3FSTRUCT));

			if (nRead != sizeof(C3FSTRUCT))
				return FALSE;
		}
		
		fl.Close();

	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::ReadC3F"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

RGBQUAD CFlcImage::m_rgbLast32Pal[] = 
{
	// SMOKE
	{ 191, 191, 191, 255 },
	{ 195, 195, 195, 255 },
	{ 200, 200, 200, 255 },
	{ 204, 204, 204, 255 },
	{ 208, 208, 208, 255 },
	{ 212, 212, 212, 255 },
	{ 217, 217, 217, 255 },
	{ 221, 221, 221, 255 },
	{ 225, 225, 225, 255 },
	{ 229, 229, 229, 255 },
	{ 234, 234, 234, 255 },
	{ 238, 238, 238, 255 },
	{ 242, 242, 242, 255 },
	{ 246, 246, 246, 255 },
	{ 251, 251, 251, 255 },
	{ 255, 255, 255, 255 },
		
	// SHADOW
//	{   0,   0,   0, 255 },
	{  13,  13,  13, 255 },
	{  26,  26,  26, 255 },
	{  38,  38,  38, 255 },
	{  51,  51,  51, 255 },
	{  64,  64,  64, 255 },
	{  77,  77,  77, 255 },
	{  90,  90,  90, 255 },
	{ 102, 102, 102, 255 },
	{ 115, 115, 115, 255 },
	{ 128, 128, 128, 255 },
	{ 141, 141, 141, 255 },
	{ 154, 154, 154, 255 },
	{ 166, 166, 166, 255 },
	{ 179, 179, 179, 255 },
	{ 192, 192, 192, 255 },
	{ 255,   0, 255, 255 } // transparent color
}; // Total size - 32*4=128 bytes

CFlcImage::CFlcImage()
{
	m_pFlc = NULL;
	
	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
}

CFlcImage::CFlcImage(LPCTSTR sPath)
{
	m_pFlc = NULL;
	
	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
	LoadFile(sPath);
}

CFlcImage::CFlcImage(CFlcView* pFlc)
{
	ASSERT(pFlc);
	m_pFlc = pFlc;

	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
}

CFlcImage::CFlcImage(CFlcView* pFlc, LPCTSTR sPath)
{
	ASSERT(pFlc);
	m_pFlc = pFlc;

	ZeroMemory(&m_hbmp, sizeof(m_hbmp));
	LoadFile(sPath);
}

CFlcImage::~CFlcImage()
{
	CGraphXImage::~CGraphXImage();
}

void CFlcImage::AttachFlcViewer(CFlcView *pFlc)
{
	ASSERT(pFlc);
	m_pFlc = pFlc;
}

BOOL CFlcImage::FlcToStoryboard()
{
	if (!m_pFlc || !m_pFlc->IsLoaded())
		return FALSE;
	
	CExportFlcDlg dlg(m_pFlc);
	if (dlg.DoModal() != IDOK)
		return FALSE;

	CloseFile();

	m_sPath	= dlg.m_sPath;
	CWaitCursor wait;

	// Calculate size of the image:
	// width	= (frame width + 1) * frames number + 1
	// height	= (frame height + 1) * directions number (8) + 1
	BYTE nFileType		= dlg.m_nFileType == 0 ? FILE_PCX : FILE_BMP;
	BYTE nPalType		= dlg.m_nPalType;

	BYTE nFrmWidthCur	= (BYTE)m_pFlc->m_flcHeader.width;
	BYTE nFrmHeightCur	= (BYTE)m_pFlc->m_flcHeader.height;
	WORD nFrmCountCur	= (WORD)m_pFlc->m_civ3Header.anim_length;
	BYTE nDirCount		= (BYTE)m_pFlc->m_civ3Header.num_anims;

	BYTE nFrmWidth		= dlg.m_nFrmWidth;
	BYTE nFrmHeight		= dlg.m_nFrmHeight;
	WORD nFrmCount		= dlg.m_nFrmCount;
	BYTE nIndentX		= 0;
	BYTE nIndentY		= 0;
	BOOL bFrmResize		= dlg.m_bFrmResize;
	BOOL bSizeChanged	= ((nFrmWidthCur != nFrmWidth) || (nFrmHeightCur != nFrmHeight));
	BOOL bLeaderHead	= m_pFlc->IsLeaderHead();
	
	BYTE nXOffset		= dlg.m_nXOffset;
	BYTE nYOffset		= dlg.m_nYOffset;

	BOOL bResizeEnable	= dlg.m_bResizeEnable;
	BYTE nResizeLeft	= dlg.m_nResizeLeft;
	BYTE nResizeRight	= dlg.m_nResizeRight;
	BYTE nResizeTop		= dlg.m_nResizeTop;
	BYTE nResizeBottom	= dlg.m_nResizeBottom;

	BOOL bResizeBorders = FALSE;
	BYTE nXSrc			= 0;
	BYTE nYSrc			= 0;
	BYTE nFrmCX			= 0;
	BYTE nFrmCY			= 0;

	if (bResizeEnable)
	{
		if ((nResizeLeft != 0 || nResizeRight  != nFrmWidth) ||
			(nResizeTop  != 0 || nResizeBottom != nFrmHeight))
			bResizeBorders = TRUE;

		if (bResizeBorders)
		{
			bSizeChanged = TRUE;
			
			nFrmCX		= nFrmWidth;
			nFrmCY		= nFrmHeight;
			nFrmWidth	= (nResizeRight - nResizeLeft);
			nFrmHeight	= (nResizeBottom - nResizeTop);
			
			nXSrc		= nResizeLeft;
			nYSrc		= nResizeTop;

			nXOffset	+= nResizeLeft;
			nYOffset	+= nResizeTop;
		}
	}

	if (bSizeChanged)
	{
		if (!bFrmResize)
		{
			if (nFrmWidth > nFrmWidthCur)
				nIndentX = (nFrmWidth - nFrmWidthCur) / 2;
			
			if (nFrmHeight > nFrmHeightCur)
				nIndentY = (nFrmHeight - nFrmHeightCur) / 2;
		}
	}
	else
		bFrmResize = FALSE; // we don't need to resize image in that case

	WORD nFrmCountNew	= min(nFrmCountCur, nFrmCount);
	int  nFrmCountDiff = nFrmCountCur - nFrmCount;
	if (nFrmCountDiff < 0)
		nFrmCountDiff = 0;

	UINT nWidth			= (nFrmWidth + GRID_SIZE) * nFrmCount + GRID_SIZE;
	UINT nHeight		= (nFrmHeight + GRID_SIZE) * nDirCount + GRID_SIZE;
	WORD i				= 0;

	RGBQUADPAL		palette;			// palette to be saved in file
	BITMAPHANDLE	hTempBmp;			// temporary bitmap
	
	UINT nXDst			= GRID_SIZE;	// horizontal indent
	UINT nYDst			= GRID_SIZE;	// vertical indent
	WORD nCur			= 0;			// current frame of total frames
	WORD nCurFrame		= 0;			// current frame of the current direction
	UINT pos			= 0;			// color position
	BYTE idx;							// color index
	BYTE x, y;
	
	WORD nTotalFrames = m_pFlc->m_arSFrames.GetSize();//min(m_pFlc->m_arSFrames.GetSize(), nFrmCount);
 
	try
	{
		ZeroMemory(&m_rgbPal, sizeof(m_rgbPal));
		
		//////////////////////////////////////////////////////////////////////////
		// Get Civ-specified FLC palette
		
		// default (current) palette
		if (dlg.m_nPalette == m_pFlc->m_nCurPalette) 
		{
			memcpy(palette.pal, m_pFlc->m_rgbPal.pal, RGBQUADPAL_SIZE);
			memcpy(m_rgbPal, m_pFlc->m_rgbPal.pal, RGBQUADPAL_SIZE);
		}
		// the same as above - for any case
		else if (dlg.m_nPalette == 0)
		{
			memcpy(palette.pal, m_pFlc->m_rgbPalDef.pal, RGBQUADPAL_SIZE);
			memcpy(m_rgbPal, m_pFlc->m_rgbPalDef.pal, RGBQUADPAL_SIZE);
		}
		// another civ-specified palette (in that case external palette must exist)
		else
		{
			memcpy(palette.pal, m_pFlc->m_rgbPal.pal, RGBQUADPAL_SIZE);
			memcpy(m_rgbPal, m_pFlc->m_rgbPal.pal, RGBQUADPAL_SIZE);
			
			RGBQUADPAL palTmp;
			if (!m_pFlc->ChangePalette(dlg.m_nPalette, &palTmp))
				return FALSE;

			memcpy(palette.pal, palTmp.pal, sizeof(RGBQUAD) * 64);
		
			// the code below isn't working... :(
//			memcpy(m_rgbPal, m_pFlc->m_rgbPal.pal, RGBQUADPAL_SIZE);
//			
//			if (!m_pFlc->ChangePalette(dlg.m_nPalette, &palette))
//				return FALSE;
//
//			memcpy(m_rgbPal, palette.pal, sizeof(RGBQUAD) * 64);
//			memcpy(palette.pal, m_rgbPal, RGBQUADPAL_SIZE);
		}
		
		//////////////////////////////////////////////////////////////////////////
		// replace last 32 colors of the bitmap palette
		if (!bLeaderHead)
			ReplaceLast32Colors(dlg.m_nLastPalType, dlg.m_sLastPalPath);

		//////////////////////////////////////////////////////////////////////////
		// Create an empty image storyboard file
		if (!CreateStoryboard(nWidth, nHeight, nFrmWidth, nFrmHeight, nDirCount, nFrmCount))
		{
			AfxMessageBox(IDS_ERR_CREATE_STORYBOARD, MB_ICONERROR);
			return FALSE;
		}
		
		/////////////////////////////////////////////////
		// Fill bitmap with data from each frames, one by one
			
		// Get dc where to draw
		CDC* pDC = CDC::FromHandle(GRAPHX::CreateGraphXDC(&m_hbmp));

		///////////////////////////////////////////////////////////////
		// Creaye a temporary bitmap
		GRAPHX::CreateBitmap(&hTempBmp, TYPE_CONV, nFrmWidthCur, nFrmHeightCur, 
			m_constBits, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);
		
		// Set bitmap palette
		GRAPHX::PutBitmapColors(&hTempBmp, 0, COLOR8, m_rgbPal);
		
		BITMAPHANDLE hResizeBmp; // meeded only in case if bSizeChanged and bFrmResize are TRUE

		// Get dc where to draw
		CDC*	pTempDC = CDC::FromHandle(GRAPHX::CreateGraphXDC(&hTempBmp));
		HDC		hTempDC	= pTempDC->m_hDC;
		HDC		hSurfDC = NULL;
		HDC		hDC		= pDC->m_hDC;
		BYTE*	pixelData = NULL;

		/////////////////////////
		//clock - start
#ifdef _DEBUG
		clock_t start, finish;
		double  duration;
		start = clock();
#endif
		/////////////////////////
		// FASTEST WAY! :)
		LPDIRECTDRAWSURFACE lpDDs;
		// Move through frames
		for (nCur = 0; nCur < nTotalFrames; nCur++)
		{
/*
			if (nCurFrame == nFrmCountCur)
			{
				nCurFrame = 0; // to start a new direction
				
				nXDst  = GRID_SIZE; // start from the left end
				nYDst += nFrmHeight + GRID_SIZE; // move to the next row
			}
*/
			// Get current frame's pixels data
			pos = 0;
			pixelData = m_pFlc->m_arSFrames[nCur]->flags.pixelData;

			// Get current surface
			lpDDs = m_pFlc->m_arSFrames[nCur]->GetDDS();

			//////////////////////////////////////////////////////////////////////////
			// To make 1 frame Leader head
//			pixelData = m_pFlc->m_arSFrames[60]->flags.pixelData;
//			lpDDs = m_pFlc->m_arSFrames[60]->GetDDS();
			//////////////////////////////////////////////////////////////////////////

			// Creates a GDI-compatible device context of the surface
			if (FAILED(lpDDs->GetDC(&hSurfDC)))
				return FALSE;

			// Copy whole picture to the temp HDC
			BitBlt(hTempDC, 0, 0, nFrmWidthCur, nFrmHeightCur, 
				hSurfDC, 0, 0, SRCCOPY);

			// For units' animation only!
			if (!bLeaderHead)
			{
				// We should make correction for shadow/smoke
				// palette indexes
				for (y = 0; y < nFrmHeightCur; y++)
				{
					for (x = 0; x < nFrmWidthCur; x++)
					{
						// in case shadow/smoke
						idx = pixelData[pos];
						if (idx > 223 && idx < 255)
							::SetPixelV(hTempDC, x, y, RGB(m_rgbPal[idx].rgbRed, m_rgbPal[idx].rgbGreen, m_rgbPal[idx].rgbBlue));
						
						pos++;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// Copy a bitmap from temp HDC to the bitmap
			if (bSizeChanged) // Change frame sizes
			{
				if (bFrmResize) // Proportionally change the frame and picture sizes
				{
					if (bResizeBorders) // Change frame borders
					{
						GRAPHX::CopyBitmap(&hResizeBmp, &hTempBmp);
//						GRAPHX::SaveBitmap(m_sPath, &hResizeBmp, nFileType, m_constBits);
						GRAPHX::SizeBitmap(&hResizeBmp, nFrmCX, nFrmCY);
//						GRAPHX::SaveBitmap(m_sPath, &hResizeBmp, nFileType, m_constBits);
						GRAPHX::TrimBitmap(&hResizeBmp, nResizeLeft, nResizeTop, nFrmWidth, nFrmHeight);
//						GRAPHX::SaveBitmap(m_sPath, &hResizeBmp, nFileType, m_constBits);
						GRAPHX::CombineBitmap(&m_hbmp, nXDst, nYDst, nFrmWidth, nFrmHeight, &hResizeBmp, 0, 0, 0);
					}
					else
					{
						GRAPHX::CopyBitmap(&hResizeBmp, &hTempBmp);
						GRAPHX::SizeBitmap(&hResizeBmp, nFrmWidth, nFrmHeight);
						GRAPHX::CombineBitmap(&m_hbmp, nXDst, nYDst, nFrmWidth, nFrmHeight, &hResizeBmp, nXSrc, nYSrc, 0);
					}
				}
				else // Change frame sizes without picture changing
				{
					BitBlt(hDC, nXDst + nIndentX, nYDst + nIndentY, nFrmWidth, nFrmHeight, 
						hTempDC, nXSrc, nYSrc, SRCCOPY);
				}
			}
			else // Copy frame without sizes changing
			{
				BitBlt(hDC, nXDst, nYDst, nFrmWidth, nFrmHeight, 
					hTempDC, 0, 0, SRCCOPY);
			}

/*			//////////////////////////////////////////////////////////////////////////
			// Copy a bitmap from temp HDC to the bitmap
			// OLD VARIANT (v.2.0.5.58)
			if (bSizeChanged)
			{
				if (bFrmResize)
				{
					GRAPHX::CopyBitmap(&hResizeBmp, &hTempBmp);
					GRAPHX::SizeBitmap(&hResizeBmp, nFrmWidth, nFrmHeight);
					GRAPHX::CombineBitmap(&m_hbmp, nXDst, nYDst, nFrmWidth, nFrmHeight, &hResizeBmp, 0, 0, 0);
				}
				else
				{
					BitBlt(hDC, nXDst + nIndentX, nYDst + nIndentY, nFrmWidthCur, nFrmHeightCur, 
						hTempDC, 0, 0, SRCCOPY);
				}
			}
			else
			{
				BitBlt(hDC, nXDst, nYDst, nFrmWidth, nFrmHeight, 
					hTempDC, 0, 0, SRCCOPY);
			}
*/

/*			
			///////////////////////////////////////////////////////
			// Copy a bitmap from temp HDC to the bitmap
			// OLD VARIANT (v.2.0.4.56)
			
			if (bSizeChanged)
			{
				if (bFrmResize)
				{
					StretchBlt(
						hDC, nXDst, nYDst, nFrmWidth, nFrmHeight, 
						hTempDC, 0, 0, nFrmWidthCur, nFrmHeightCur,
						SRCCOPY);
				}
				else
				{
					StretchBlt(
						hDC, nXDst + nIndentX, nYDst + nIndentY, nFrmWidthCur, nFrmHeightCur, 
						hTempDC, 0, 0, nFrmWidthCur, nFrmHeightCur,
						SRCCOPY);
				}
			}
			else
			{
				BitBlt(hDC, nXDst, nYDst, nFrmWidth, nFrmHeight, 
					hTempDC, 0, 0, SRCCOPY);
			}
*/
			// Releases the hDC previously obtained with GetDC
			if (FAILED(lpDDs->ReleaseDC(hSurfDC)))
				return FALSE;
			
			nXDst += nFrmWidth + GRID_SIZE; // move to the next column
			nCurFrame++;

			if (nCurFrame == nFrmCountNew)
			{
				nCurFrame = 0; // to start a new direction
				
				nXDst	= GRID_SIZE; // start from the left end
				nYDst	+= nFrmHeight + GRID_SIZE; // move to the next row
				nCur	+= nFrmCountDiff;
			}
		}

/*		
		///////////////////////////////////////////////////////
		// VARIANT FROM THE FIRST RELEASE (v.2.0.3.50)
		for (nCur = 0; nCur < nTotalFrames; nCur++)
		{
			pos = 0;
			pixelData = m_pFlc->m_arSFrames[nCur]->flags.pixelData;

			if (nCurFrame == nFrmCountCur)
			{
				nCurFrame = 0; // to start a new direction
				
				nXDst  = GRID_SIZE; // start from the left end
				nYDst += nFrmHeight + GRID_SIZE; // move to the next row
			}

			// Fill bitmap with transparent color
//			GRAPHX::FillBitmap(&hTempBmp, GetColor(255));

			// Get current frame's data
			for (y = 0; y < nFrmHeightCur; y++)
			{
				for (x = 0; x < nFrmWidthCur; x++)
				{
					idx = pixelData[pos];
					::SetPixelV(pTempDC->m_hDC, x, y, RGB(m_rgbPal[idx].rgbRed, m_rgbPal[idx].rgbGreen, m_rgbPal[idx].rgbBlue));
					
					// note: GRAPHX::PutPixelColor is VERY slow!
					//GRAPHX::PutPixelColor(&hTempBmp, x, y, RGB(m_rgbPal[idx].rgbRed, m_rgbPal[idx].rgbGreen, m_rgbPal[idx].rgbBlue));
					pos++;
				}
			}

			// Copy a bitmap from surface to the bitmap
			if (bSizeChanged)
			{
				if (bFrmResize)
				{
					StretchBlt(
						hDC, nXDst, nYDst, nFrmWidth, nFrmHeight, 
						hTempDC, 0, 0, nFrmWidthCur, nFrmHeightCur,
						SRCCOPY);
				}
				else
				{
					StretchBlt(
						hDC, nXDst + nIndentX, nYDst + nIndentY, nFrmWidthCur, nFrmHeightCur, 
						hTempDC, 0, 0, nFrmWidthCur, nFrmHeightCur,
						SRCCOPY);
				}
			}
			else
			{
				BitBlt(hDC, nXDst, nYDst, nFrmWidth, nFrmHeight, 
					hTempDC, 0, 0, SRCCOPY);
			}

			nXDst += nFrmWidth  + GRID_SIZE; // move to the next column
			nCurFrame++;
		}//for (i = 0; i < nDirCount && nCur < m_pFlc->m_arSFrames.GetSize(); i++)
*/		
		/////////////////////
		// clock - finish
#ifdef _DEBUG
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		TRACE1("\nCFlcImage.FlcToStoryboard: %2.3f seconds\n", duration);
#endif
		//////////////////////

		// Release DC
		GRAPHX::DeleteGraphXDC(*pTempDC);

		// Delete temporary bitmap
		GRAPHX::FreeBitmap (&hTempBmp);

		// Release DC
		GRAPHX::DeleteGraphXDC(*pDC);

		// Save bitmap as specified file type (FILE_BMP, FILE_PCX)
		GRAPHX::SaveBitmap(m_sPath, &m_hbmp, nFileType,
			dlg.m_nBitsPerPixel == BitsPerPixel_8 ? BITS_8 : BITS_24);

		// In case we should make a separate palette file
		if (dlg.m_bMakePalette)
		{
			if (!ExportPaletteToFile(m_sPath, palette, (EPaletteFileType)nPalType))
			{
				AfxMessageBox(IDS_ERR_WRITE_PAL);
				return FALSE;
			}
		}
		// otherwise - try to find palette file with and delete it
		else
			RemovePaletteFile(m_sPath, (EPaletteFileType)nPalType);
		
		if (dlg.m_bDefPalette)
		{
			memcpy(palette.pal, m_rgbPal, RGBQUADPAL_SIZE);
			if (!ExportPaletteToFile(m_sPath, palette, (EPaletteFileType)nPalType, uLoadString(IDS_DEF_PALETTE_SUFFIX)))
			{
				AfxMessageBox(IDS_ERR_WRITE_PAL);
				return FALSE;
			}
		}

		/////////////////////////////////////////////////
		// Make INI file
		if (!WriteC3F(m_sPath, 
			nWidth,
			nHeight,
			nFrmWidth,
			nFrmHeight,
			nFrmCount,
			nDirCount,
			dlg.m_nDelay,
			nXOffset,
			nYOffset,
			nFileType,
			nPalType,
			bLeaderHead))
		{
			AfxMessageBox(IDS_ERR_WRITE_C3F, MB_ICONERROR);
			return FALSE;
		}
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CMemoryException* e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::FlcToStoryboard"), MB_ICONERROR);
		return FALSE;
	}

/*	if (MAIN_APP->IsExpOpen())
	{
		CString sPath(m_sPath);
		sPath.Delete(sPath.GetLength()-3, 3);
		sPath += C3F_EXT;

		if (MAIN_APP->IsExpAsk())
		{
			if (AfxMessageBox(uFormatString(IDS_ASK_OPEN_C3F, sPath), MB_ICONQUESTION|MB_YESNO) != IDYES)
				return TRUE;
		}

		CDocTemplate* pDocTemplate = MAIN_APP->GetDocManager()->GetDocTemplateC3f();
		if (pDocTemplate)
			pDocTemplate->OpenDocumentFile(sPath);
	}
	else
		AfxMessageBox(IDS_FLC_EXP_OK, MB_ICONINFORMATION);
*/
	return TRUE;
}

BOOL CFlcImage::CreateFile(LPCSTR sPath)
{
	CFile fl;
	if (!fl.Open(sPath, CFile::modeCreate | CFile::modeWrite | CFile::modeRead))
		return FALSE;
	
	fl.Close();
	return TRUE;
}

BOOL CFlcImage::CreateStoryboard(UINT nWidth,
								 UINT nHeight,
								 BYTE nFrmWidth,
								 BYTE nFrmHeight,
								 BYTE nDirCount,
								 WORD nFrmCount,
								 BYTE nBitsPerPixel)
{
	if ((nWidth < 1 || nHeight < 1) ||
		(nFrmWidth < 1 || nFrmHeight < 1) ||
		(nDirCount < 1 || nFrmCount < 1))
	{
		AfxMessageBox(IDS_INVALID_PARAMS, MB_ICONERROR);
		return FALSE;
	}

	if (m_hbmp.Flags.Allocated)
		GRAPHX::FreeBitmap(&m_hbmp);

	WORD i = 0;
	UINT x = 0;
	UINT y = 0;

	try
	{
		if (!CreateFile(m_sPath))
			return FALSE;

		GRAPHX::CreateBitmap(&m_hbmp, TYPE_CONV, nWidth, nHeight, 
			nBitsPerPixel, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);
		
		// Set bitmap palette
		if (nBitsPerPixel == BITS_8)
			GRAPHX::PutBitmapColors(&m_hbmp, 0, COLOR8, m_rgbPal);

		// Fill bitmap with transparent color
		GRAPHX::FillBitmap(&m_hbmp, GetColor(255));

		// Get dc where to draw
		CDC* pDC = CDC::FromHandle(GRAPHX::CreateGraphXDC(&m_hbmp));

		/////////////////////////////////////////////////
		// Draw the grid lines
		// Setting of attributes
		// Set pen style
		LOGBRUSH lb;
		lb.lbColor = RGB(0, 0, 0);

		lb.lbStyle = BS_SOLID;
		CPen pen(PS_SOLID | PS_ENDCAP_ROUND | PS_GEOMETRIC, 1, &lb);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&pen);

		nFrmHeight	+= GRID_SIZE;
		nFrmWidth	+= GRID_SIZE;

		// Draw horizontal lines
		for (i = 0; i < nDirCount + 1; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(nWidth, y);
			
			y += nFrmHeight;
		}

		// Draw vertical lines
		x = 0;
		y = 0;
		for (i = 0; i < nFrmCount + 1; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, nHeight);

			x += nFrmWidth;
		}

		// Return old pen
		pDC->SelectObject(pOldPen);

		// Release DC
		GRAPHX::DeleteGraphXDC(*pDC);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::CreateStoryboard"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL CFlcImage::NewStoryboard()
{
	CNewFlcDlg dlg;
	if (dlg.DoModal() != IDOK)
		return FALSE;

	CloseFile();

	m_sPath	= dlg.m_sPath;

	CWaitCursor wait;

	// Calculate size of the image:
	// width	= (frame width + 1) * frames number + 1
	// height	= (frame height + 1) * directions number (8) + 1
	BYTE nFileType		= dlg.m_nFileType == 0 ? FILE_PCX : FILE_BMP;
	BYTE nPalType		= dlg.m_nPalType;

	BYTE nFrmWidth		= dlg.m_nFrmWidth;
	BYTE nFrmHeight		= dlg.m_nFrmHeight;
	WORD nFrmCount		= dlg.m_nFrmCount;

	BYTE nDirCount		= dlg.m_nDirCount;
	UINT nWidth			= (nFrmWidth + 1) * nFrmCount + 1;
	UINT nHeight		= (nFrmHeight + 1) * nDirCount + 1;
	BOOL bLeaderHead	= dlg.m_bLeaderHead;
	
	RGBQUADPAL palette;

	try
	{
		ZeroMemory(&m_rgbPal, sizeof(m_rgbPal));

		// Get palette from external file
		if (dlg.m_bSelPalPath)
		{
			if (!ImportPaletteFromFile(dlg.m_sSelPalPath, palette))
			{
				AfxMessageBox(uFormatString(IDS_ERR_READ_PAL, dlg.m_sSelPalPath));
				return FALSE;
			}

			memcpy(m_rgbPal, palette.pal, RGBQUADPAL_SIZE);
			memcpy(palette.pal, m_rgbPal, RGBQUADPAL_SIZE);
		}

		// Palette changes for units only
		if (!bLeaderHead)
		{
			// Apply civ-specified colors
			if (!m_pFlc->ChangePalette(dlg.m_nPalette, &palette))
			{
				AfxMessageBox(IDS_ERR_PALETTE, MB_ICONERROR);
				return FALSE;
			}
			
			memcpy(m_rgbPal, palette.pal, sizeof(RGBQUAD) * 64);

			if (dlg.m_bSelPalPath)
				memcpy(palette.pal, m_rgbPal, RGBQUADPAL_SIZE);

			// replace last 32 colors of the bitmap palette
			ReplaceLast32Colors(dlg.m_nLastPalType, dlg.m_sLastPalPath);
		}		
		
		if (!CreateStoryboard(nWidth, nHeight, nFrmWidth, nFrmHeight, nDirCount, nFrmCount))
		{
			AfxMessageBox(IDS_ERR_CREATE_STORYBOARD, MB_ICONERROR);
			return FALSE;
		}
		
		// Save bitmap as specified file type (FILE_BMP, FILE_PCX)
		//GRAPHX::SaveBitmap(m_sPath, &m_hbmp, nFileType, m_constBits);
		GRAPHX::SaveBitmap(m_sPath, &m_hbmp, nFileType,
			dlg.m_nBitsPerPixel == BitsPerPixel_8 ? BITS_8 : BITS_24);
		
		// In case we should make a separate palette file
		if (dlg.m_bMakePalette)
		{
			if (!ExportPaletteToFile(m_sPath, palette, (EPaletteFileType)nPalType))
			{
				AfxMessageBox(IDS_ERR_WRITE_PAL);
				return FALSE;
			}
		}
		// otherwise - try to find palette file with and delete it
		else
			RemovePaletteFile(m_sPath, (EPaletteFileType)nPalType);
		
		if (dlg.m_bDefPalette)
		{
			memcpy(palette.pal, &m_rgbPal, RGBQUADPAL_SIZE);
			if (!ExportPaletteToFile(m_sPath, palette, (EPaletteFileType)nPalType, uLoadString(IDS_DEF_PALETTE_SUFFIX)))
			{
				AfxMessageBox(IDS_ERR_WRITE_PAL);
				return FALSE;
			}
		}

		/////////////////////////////////////////////////
		// Make INI file
		if (!WriteC3F(m_sPath, 
			nWidth,
			nHeight,
			nFrmWidth,
			nFrmHeight,
			nFrmCount,
			nDirCount,
			dlg.m_nDelay,
			dlg.m_nXOffset,//(WND_W - nFrmWidth) / 2,
			dlg.m_nYOffset,//(WND_H - nFrmHeight) / 2,
			nFileType,
			nPalType,
			bLeaderHead))
		{
			AfxMessageBox(IDS_ERR_WRITE_C3F, MB_ICONERROR);
			return FALSE;
		}
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CMemoryException* e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::NewStoryboard"), MB_ICONERROR);
		return FALSE;
	}

	AfxMessageBox(IDS_FLC_NEW_OK, MB_ICONINFORMATION);
	return TRUE;
}

BOOL CFlcImage::OpenStoryboard(CString sPath, C3FSTRUCT &c3f)
{
	CloseFile();

	if (!ReadC3F(sPath, &c3f))
	{
		AfxMessageBox(IDS_ERR_READ_C3F);
		return FALSE;
	}
	
	CWaitCursor wait;
	
//	m_sPath			= c3f.Name;
	UINT nWidth		= c3f.Width;
	UINT nHeight	= c3f.Height;
	BYTE nFrmWidth	= c3f.FrmWidth;
	BYTE nFrmHeight	= c3f.FrmHeight;
	WORD nFrmCount	= c3f.FrmCount;
	BYTE nDirCount	= c3f.DirCount;
	BYTE nFileType	= c3f.FileType;
	
	m_sPath.Format("%s%s%s", CFileX::GetFolder(sPath), c3f.Name, nFileType == FILE_PCX ? ".pcx" : ".bmp");
	
	if (!uIsFileExist(m_sPath))
	{
		AfxMessageBox(uFormatString(IDS_ERR_SB_NOTEXIST, m_sPath), MB_ICONERROR);
		return FALSE;
	}
	
	if ((nWidth < 1 || nHeight < 1) ||
		(nFrmWidth < 1 || nFrmHeight < 1) ||
		(nDirCount < 1 || nFrmCount < 1))
	{
		AfxMessageBox(uFormatString(IDS_ERR_SB_INVALIDSIZE, sPath), MB_ICONERROR);
		return FALSE;
	}
	
	try
	{
		GRAPHX::FileInfo(m_sPath, &m_fi, FILEINFO_TOTALPAGES, NULL);

		if (m_fi.BitsPerPixel != BITS_8 && m_fi.BitsPerPixel != BITS_24)
		{
			AfxMessageBox(uFormatString(IDS_ERR_SB_BITSPERPIXEL, m_sPath, m_fi.BitsPerPixel), MB_ICONERROR);
			return FALSE;
		}
		
		if (m_fi.Format != nFileType)
		{
			AfxMessageBox(uFormatString(IDS_ERR_SB_FILEFORMAT, m_sPath), MB_ICONERROR);
			return FALSE;
		}
		
		if (m_fi.Width != (int)nWidth || m_fi.Height != (int)nHeight)
		{
			AfxMessageBox(uFormatString(IDS_ERR_SB_SIZES, m_sPath, nWidth, nHeight, m_fi.Width, m_fi.Height), MB_ICONERROR);
			return FALSE;
		}

		if (GRAPHX::LoadFile((LPTSTR)(LPCTSTR)m_sPath, &m_hbmp, m_fi.BitsPerPixel/*m_constBits*/, ORDER_BGR, LOADFILE_ALLOCATE | LOADFILE_STORE, NULL, NULL, NULL, NULL) != SUCCESS)
		{
			AfxMessageBox(uFormatString(IDS_ERR_SB_LOAD, m_sPath), MB_ICONERROR);
			return FALSE;
		}	
		
		/////////////////////////////////////////////////////////
		// In case it's PCX file - convert it to BMP and reload again
		if (m_fi.Format == FILE_PCX)
		{
			CString  sTempFile = uGetTempFileName("SB");
			
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
/*
		// We need to rotate bitmap
		// Actually, DON'T NEED. Only in case we will use GRAPHX::GetPixelColor
		// in the CC3fView::LoadAnimation function... ;)
		if (GRAPHX::RotateBitmap(&m_hbmp, -9000, ROTATE_RESIZE, NULL) != SUCCESS)
		{
			AfxMessageBox(uFormatString(IDS_ERR_SB_LOAD, m_sPath), MB_ICONERROR);
			return FALSE;
		}

// for tests only!
		// Save bitmap as BMP file type
		GRAPHX::SaveBitmap("D:\\test.bmp", &m_hbmp, FILE_BMP, m_constBits);
*/	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::StoryboardToFlc"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL ImportPalette(RGBQUADPAL &palette)
{
	CFileDialogX dlg(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		_T(uLoadString(IDS_FILTER_PAL_IMP)));
	
	CWinApp* pApp = AfxGetApp();
	CString sTitle(uLoadString(IDS_TITLE_IMP_PAL));
	
	dlg.m_ofn.lpstrTitle	= sTitle;
	dlg.m_ofn.nFilterIndex	= pApp->GetProfileInt(REG_OPTIONS, REG_FILTER_PAL_IMP, 1);
	
	CString sPath = pApp->GetProfileString(REG_OPTIONS, REG_LAST_PAL, "");
	
	if (uIsFileExist(sPath))
		dlg.m_ofn.lpstrInitialDir = sPath;
	
	if (dlg.DoModal() != IDOK)
		return TRUE;
	
	sPath = dlg.GetPathName();
	
	pApp->WriteProfileString(REG_OPTIONS, REG_LAST_PAL, sPath);
	pApp->WriteProfileInt(REG_OPTIONS, REG_FILTER_PAL_IMP, dlg.m_ofn.nFilterIndex);

	if (!ImportPaletteFromFile(sPath, palette))
	{
		AfxMessageBox(uFormatString(IDS_IMP_PAL_ERR, sPath));
		return FALSE;
	}
	
	return TRUE;
}

BOOL ExportPalette(RGBQUADPAL &palette)
{
	CFileDialogX dlg(FALSE, NULL, NULL,
		OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		CString((LPCTSTR)IDS_FILTER_PAL_EXP));
	
	CWinApp* pApp = AfxGetApp();
	CString sTitle(uLoadString(IDS_TITLE_EXP_PAL));
	
	dlg.m_ofn.lpstrTitle	= sTitle;
	dlg.m_ofn.nFilterIndex	= pApp->GetProfileInt(REG_OPTIONS, REG_FILTER_PAL_EXP, 1);
	
	CString sPath = pApp->GetProfileString(REG_OPTIONS, REG_LAST_PAL, "");
	
	if (uIsFileExist(sPath))
		dlg.m_ofn.lpstrInitialDir = sPath;
	
	if (dlg.DoModal() != IDOK)
		return TRUE;
	
	sPath = dlg.GetPathName();
	
	pApp->WriteProfileString(REG_OPTIONS, REG_LAST_PAL, sPath);
	pApp->WriteProfileInt(REG_OPTIONS, REG_FILTER_PAL_EXP, dlg.m_ofn.nFilterIndex);
	
	if (!ExportPaletteToFile(sPath, palette, (EPaletteFileType)(dlg.m_ofn.nFilterIndex-1)))
	{
		AfxMessageBox(uFormatString(IDS_EXP_PAL_ERR, sPath));
		return FALSE;
	}

	AfxMessageBox(IDS_EXP_PAL_OK, MB_ICONINFORMATION);
	return TRUE;
}

BOOL ImportPaletteFromFile(CString sPathName, RGBQUADPAL &palette)
{
	if (!uIsFileExist(sPathName))
	{
//		AfxMessageBox(sPathName);
		return FALSE;
	}

	CString sExt = uGetFileExt(sPathName);

	FILE *fp;
	RGBQUADPAL rgbPal; // current palette
	short i = 0;
	short idx = COLOR8-1;

	try
	{
		fp = fopen(sPathName, "rb");
		
		if (!fp)
		{
//			AfxMessageBox("1");
			return FALSE;
		}

		// get file size
		fseek(fp, 0L, SEEK_END);
		long nSize = ftell(fp);
		fseek(fp, 0L, SEEK_SET); // move to the start
		
		// Adobe Color Table
		if (sExt.CompareNoCase(ACT_EXT) == 0)
		{
			// check file size (must be 256x3=768)
			if (nSize < 768)
			{
				fclose(fp);
//				AfxMessageBox("2");
				return FALSE;
			}

			for (i = 0; i < COLOR8; i++)
			{
				fread(&rgbPal.pal[idx].rgbRed,		1, 1, fp);
				fread(&rgbPal.pal[idx].rgbGreen,	1, 1, fp);
				fread(&rgbPal.pal[idx].rgbBlue,		1, 1, fp);

				idx--;
			}
		}
		// Microsoft Palette or Jasc Animation Shop
		else if (sExt.CompareNoCase(PAL_EXT) == 0)
		{
			CString sBuf;

			// read first 4 bytes to know file type
			fgets(sBuf.GetBuffer(4), 5, fp);
			sBuf.ReleaseBuffer();

			// Jasc Animation Shop
			if (sBuf.CompareNoCase("JASC") == 0)
			{
				idx = 0;
				fseek(fp, 21, SEEK_SET); // skip file header

				BYTE curByte;
				int nValue = 0;
				CString sValue;

				while(!feof(fp) && idx < COLOR8)
				{
					fread(&curByte, 1, 1, fp);
					
					switch (curByte)
					{
					case '\r':
						continue;

					case '\n':
						{
							rgbPal.pal[idx].rgbBlue = atoi(sValue);
							nValue = 0;
							sValue.Empty();
							idx++;
							break;
						}

					case ' ':
						{
							if (nValue == 0)
								rgbPal.pal[idx].rgbRed = atoi(sValue);
							else
								rgbPal.pal[idx].rgbGreen = atoi(sValue);

							nValue++;
							sValue.Empty();
							break;
						}
						
					default:
						{
							sValue += curByte;
						}
					}
				}//while(!feof(fp))
			}//if (sBuf.CompareNoCase("JASC") == 0)
			// Microsoft Palette
			else if (sBuf.CompareNoCase("RIFF") == 0)
			{
				fseek(fp, 24, SEEK_SET); // skip file header
				
				for (i = 0; i < COLOR8; i++)
				{
					fread(&rgbPal.pal[idx].rgbRed,		1, 1, fp);
					fread(&rgbPal.pal[idx].rgbGreen,	1, 1, fp);
					fread(&rgbPal.pal[idx].rgbBlue,		1, 1, fp);
					fseek(fp, 1, SEEK_CUR); // skip alpha
					
					idx--;
				}
			}
			else
			{
				fclose(fp);
//				AfxMessageBox("3");
				return FALSE;
			}
		}
		else // an invalid extension
		{
			fclose(fp);
//			AfxMessageBox("4");
			return FALSE;
		}

		if (fp)
			fclose(fp);

		// Copy current palette to the default palette
		//ZeroMemory(&palette, sizeof(palette));
		memcpy(&palette, &rgbPal, RGBQUADPAL_SIZE);
	}
	catch (CFileException *e)
	{
//#ifdef _DEBUG
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
//#endif
		e->Delete();
		if (fp)
			fclose(fp);
//		AfxMessageBox("5");
		return FALSE;
	}
	catch (...)
	{
		if (fp)
			fclose(fp);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "ImportPaletteFromFile"), MB_ICONERROR);
		
//		AfxMessageBox("6");
		return FALSE;
	}
	
//	AfxMessageBox("OK");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Check is a palette file by the given type exist by the given path.
// In case such palette file is present - remove it.
//
void RemovePaletteFile(CString sPathName, EPaletteFileType type)
{
	if (!uGetFileExt(sPathName).IsEmpty())
		sPathName.Delete(sPathName.GetLength()-4, 4);
	
	switch (type)
	{
	case PalACT:  // Adobe Color Table
		{
			sPathName += ".act";
			break;
		}

	case PalMS:   // Microsoft Palette
	case PalJASC: // Jasc Animation Shop
		{
			sPathName += ".pal";
			break;
		}

	default:
		return;
	}

	try
	{
		if (uIsFileExist(sPathName))
			DeleteFile(sPathName);
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
	}
}

//////////////////////////////////////////////////////////////////////////
// Export palette to the palette file by the given type and path.
//
BOOL ExportPaletteToFile(CString sPathName, RGBQUADPAL &palette, EPaletteFileType type, LPCTSTR sExtName /*= NULL*/)
{
	if (sPathName.IsEmpty())
		return FALSE;

	FILE *fp = NULL;
	short idx = 0;

	try
	{
		if (!uGetFileExt(sPathName).IsEmpty())
			sPathName.Delete(sPathName.GetLength()-4, 4);

		if (sExtName)
			sPathName += sExtName;

		switch (type)
		{
			// Adobe Color Table
		case PalACT:
			{
				sPathName += ".act";
				
				fp = fopen(sPathName, "wb");
				
				if (!fp)
					return E_FAIL;

				// write colors
				for (idx = COLOR8-1; idx > -1; idx--)
				{
					fwrite(&palette.pal[idx].rgbRed,	1, 1, fp);
					fwrite(&palette.pal[idx].rgbGreen,	1, 1, fp);
					fwrite(&palette.pal[idx].rgbBlue,	1, 1, fp);
				}

				fclose(fp);
				break;
			}

			// Microsoft Palette
		case PalMS:
			{
				sPathName += ".pal";
				fp = fopen(sPathName, "wb");
				
				if (!fp)
					return E_FAIL;
				
				// write header
				BYTE header[24] = {0x52, 0x49, 0x46, 0x46, 0x10, 0x04, 0x00, 0x00, 0x50, 0x41, 0x4C, 0x20, 0x64, 0x61, 0x74, 0x61, 0x04, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01};
				fwrite(&header,	sizeof(header), 1, fp);
				
				BYTE alpha = 0x00;

				// write colors
				for (idx = COLOR8-1; idx > -1; idx--)
				{
					fwrite(&palette.pal[idx].rgbRed,	1, 1, fp);
					fwrite(&palette.pal[idx].rgbGreen,	1, 1, fp);
					fwrite(&palette.pal[idx].rgbBlue,	1, 1, fp);
					fwrite(&alpha, 1, 1, fp);
				}
				
				fclose(fp);
				break;
			}
			
			// Jasc Animation Shop
		case PalJASC:
			{
				sPathName += ".pal";

				fp = fopen(sPathName, "wt");
				
				if (!fp)
					return E_FAIL;

				CString sContent = "JASC-PAL\n0100\n256\n"; // add header
				CString sRGB;

				// add colors
				for (idx = 0; idx < COLOR8; idx++)
				{
					sRGB.Format("%d %d %d\n",
						palette.pal[idx].rgbRed,
						palette.pal[idx].rgbGreen,
						palette.pal[idx].rgbBlue);

					sContent += sRGB;
				}

				sContent.Delete(sContent.GetLength(), 1); // remove last "\n"
				
				// write file
				fwrite(sContent, sContent.GetLength(), 1, fp);
				
				fclose(fp);
				break;
			}

			// an invalid format
		default:
			return FALSE;
		}
	}
	catch (CFileException *e)
	{
//#ifdef _DEBUG
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
//#endif
		e->Delete();
		if (fp)
			fclose(fp);
		return FALSE;
	}
	catch (...)
	{
		if (fp)
			fclose(fp);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "ExportPaletteToFile"), MB_ICONERROR);
		
		return FALSE;
	}

	return TRUE;
}

BOOL CFlcImage::CopyToClipboard()
{
	try
	{
		CloseFile();
		
		if (GRAPHX::CaptureArea(&m_hbmp) != SUCCESS)
			return FALSE;
		
		// Copy temporary bitmap to buffer
		GRAPHX::CopyToClipboard(AfxGetMainWnd()->GetSafeHwnd(), &m_hbmp);
		
		// Delete temporary bitmap
		GRAPHX::FreeBitmap(&m_hbmp);
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
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::CopyToClipboard"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL CFlcImage::CopyToFile(CAnimationView* pAnim)
{
	try
	{
		CloseFile();
		
		if (GRAPHX::CaptureArea(&m_hbmp) != SUCCESS)
			return FALSE;

		return SaveToFile(pAnim);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::CopyToFile"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Replace last 32 colors of the bitmap palette
//
void CFlcImage::ReplaceLast32Colors(ELastPalType nLastPalType, LPCSTR sLastPalPath)
{
	switch (nLastPalType)
	{
	case PalHardcoded:
		memcpy(m_rgbPal + 224, &m_rgbLast32Pal, sizeof(m_rgbLast32Pal));
		return;

	case PalSelective:
		{
			RGBQUADPAL rgbPalette;
			if (ImportPaletteFromFile(sLastPalPath, rgbPalette))
				memcpy(m_rgbPal + 224, rgbPalette.pal + 224, sizeof(RGBQUAD) * 32);
			else
				AfxMessageBox(uFormatString(IDS_ERR_READ_PAL, sLastPalPath));

			return;
		}
	}
}

BOOL CFlcImage::SaveToFile(CAnimationView* pAnim)
{
	if (!IsLoaded())
		return FALSE;
	
	CString sFileName;
	
	if (pAnim)
		sFileName = CFileX::GetShortFileName(pAnim->m_sPathName);
	try
	{
		CFileImgDlg dlg(FALSE, NULL, sFileName,
			OFN_OVERWRITEPROMPT |OFN_ENABLESIZING |
			OFN_ENABLETEMPLATE | OFN_EXPLORER |
			OFN_LONGNAMES | OFN_PATHMUSTEXIST,
			_T(uLoadString(IDS_FILTER_IMG_SAVE)),
			AfxGetMainWnd());
		
		// All Supported Files|*.jif;*.jpg;*.jpeg;*.cmp;*.gif;*.tif;*.tiff;*.bmp;*.rle;*.dib;*.ico;*.cur;*.pcx;*.dcx;*.pcd;*.wmf;*.emf;*.xpm;*.iff;*.lbm;*.ilbm;*.pbm;*.clp;*.xwd;*.flc;*.ani;*.psd;*.png;*.tga;*.eps;*.pct;*.avi;*.awd|JPEG (*.jpg;*.jpeg;*.jif)|*.jpg;*.jpeg;*.jif|GIF (*.gif)|*.gif|TIFF (*.tif;*.tiff)|*.tif;*.tiff|BMP (*.bmp;*.rle;*.dib)|*.bmp;*.rle;*.dib|PCX (*.pcx;*.dcx)|*.pcx;*.dcx|Kodak PhotoCD (*.pcd)|*.pcd|Interchange Format (*.iff;*.lbm;*.ilbm)|*.iff;*.lbm;*.ilbm|PhotoShop 3.0 (*.psd)|*.psd|PNG (*.png)|*.png|TARGA (*.tga)|*.tga|EPS (*.eps)|*.eps|PCT (*.pct)|*.pct|AWD (*.awd)|*.awd||"	
		// "JPEG (*.jpg;*.jpeg;*.jif)|*.jpg;*.jpeg;*.jif|TIFF (*.tif;*.tiff)|*.tif;*.tiff|BMP (*.bmp;*.rle;*.dib)|*.bmp;*.rle;*.dib|PCX (*.pcx;*.dcx)|*.pcx;*.dcx|Interchange Format (*.iff;*.lbm;*.ilbm)|*.iff;*.lbm;*.ilbm|PhotoShop 3.0 (*.psd)|*.psd|PNG (*.png)|*.png|TARGA (*.tga)|*.tga|MS FAX (*.awd)|*.awd||" 	
		
		dlg.m_ofn.hInstance = AfxGetInstanceHandle();
		dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_IMG);
		dlg.m_pImg = this;
		
		CString sTitle(uLoadString(IDS_TITLE_IMG));
		dlg.m_ofn.lpstrTitle = sTitle;
		
		CWinApp* pApp = AfxGetApp();
		dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(REG_IMG_DLG, REG_FILTER, 1);
		
		m_sPath = pApp->GetProfileString(REG_IMG_DLG, REG_LAST_OPENED, "");
		
		if (uIsFileExist(m_sPath))
			dlg.m_ofn.lpstrInitialDir = m_sPath;
		
		if (dlg.DoModal() != IDOK)
			return FALSE;
		
		m_sPath = dlg.m_sFilePath;//dlg.GetPathName();
		if (!uGetFileExt(m_sPath).IsEmpty())
			m_sPath.Delete(m_sPath.GetLength()-4, 4);
		m_sPath += dlg.m_sExt;
		
		pApp->WriteProfileString(REG_IMG_DLG, REG_LAST_OPENED, m_sPath);
		
		if (m_hbmp.Width != dlg.m_nWidth || m_hbmp.Height != dlg.m_nHeight)
			GRAPHX::SizeBitmap(&m_hbmp, dlg.m_nWidth, dlg.m_nHeight);
		
		if (!CreateFile(m_sPath))
			return FALSE;
		
		// Save bitmap
		GRAPHX::SaveBitmap(m_sPath, &m_hbmp, dlg.m_nFileType,
			dlg.m_nBitsPerPixel);
		//		GRAPHX::SaveFile((LPTSTR)(LPCTSTR)m_sPath, &m_hbmp, dlg.m_nFileType,
		//			dlg.m_nBitsPerPixel, nQFactor, NULL, NULL, NULL, &sfo);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::SaveToFile"), MB_ICONERROR);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CFlcImage::SaveFrameToFile(CAnimationView* pAnim)
{
	try
	{

//		BYTE nWidth, nHeight;
//		if (!pAnim->PrepareFrameToCopy(nWidth, nHeight))
//			return FALSE;
		
		if (!pAnim->IsLoaded())
			return FALSE;
		
		BYTE nWidth	 = pAnim->m_nFrmWidth;
		BYTE nHeight = pAnim->m_nFrmHeight;

		if (!pAnim->m_bLeaderHead)
		{
			if (pAnim->m_bgdSurface)	// Draw background image
				pAnim->m_bgdSurface->Draw(0, 0);
			else				// Fill background
				pAnim->m_pScreen->Fill(pAnim->m_bgdColor);
		}
		
		pAnim->DrawFrame();

		/////////////////////////
		CloseFile();
		
		if (m_hbmp.Flags.Allocated)
			GRAPHX::FreeBitmap(&m_hbmp);
		
		GRAPHX::CreateBitmap(&m_hbmp, TYPE_CONV, nWidth, nHeight, 
			BITS_24, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);
		
		// Get dc where to draw
		CDC*	pDC		= CDC::FromHandle(GRAPHX::CreateGraphXDC(&m_hbmp));
		HDC		hSurfDC = NULL;
		HDC		hDC		= pDC->m_hDC;
		
		LPDIRECTDRAWSURFACE lpDDs;
		
		// Get current surface
		lpDDs = pAnim->m_pScreen->GetBack()->GetDDS();
		
		// Creates a GDI-compatible device context of the surface
		if (FAILED(lpDDs->GetDC(&hSurfDC)))
			return FALSE;
		
		// Copy whole picture to the temp HDC
		BitBlt(hDC, 0, 0, nWidth, nHeight, 
			hSurfDC, pAnim->m_xPos, pAnim->m_yPos, SRCCOPY);
		
		// Releases the hDC previously obtained with GetDC
		if (FAILED(lpDDs->ReleaseDC(hSurfDC)))
			return FALSE;
		
		// Release DC
		GRAPHX::DeleteGraphXDC(*pDC);
		
		return SaveToFile(pAnim);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcImage::SaveFrameToFile"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BYTE GetPaletteIndex(RGBQUADPAL &rgbPal, COLORREF &color)
{
	BYTE idx = 0;

	BYTE r = GetRValue(color);
	BYTE g = GetGValue(color);
	BYTE b = GetBValue(color);

	for (idx; idx < IDX_MAX; idx++)
	{
		if (rgbPal.pal[idx].rgbRed == r)
		{
			if (rgbPal.pal[idx].rgbGreen == g && 
				rgbPal.pal[idx].rgbBlue == b)
				return idx;
		}
	}

	return idx;
}
