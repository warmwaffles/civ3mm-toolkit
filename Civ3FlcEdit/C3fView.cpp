/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	C3fView.cpp
	CREATED:	02.09.2004
	VERSION:	5.5

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CC3fView class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "C3fView.h"

#include "MainFrm.h"
#include "FileFlcDlg.h"
#include "C3fDoc.h"
#include "DocManagerEx.h"
#include "InfoDialog.h"
#include "OffsetDlg.h"
#include "ShrinkFrmCountDlg.h"

//#include "..\MFCaids\FadeWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CC3fView

IMPLEMENT_DYNCREATE(CC3fView, CAnimationView)

BEGIN_MESSAGE_MAP(CC3fView, CAnimationView)
	//{{AFX_MSG_MAP(CC3fView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_C3F_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_C3F_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_OFFSET, OnOffset)
	ON_UPDATE_COMMAND_UI(ID_OFFSET, OnUpdateOffset)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_SHRINK_FRM_COUNT, OnShrinkFrmCount)
	ON_UPDATE_COMMAND_UI(ID_SHRINK_FRM_COUNT, OnUpdateShrinkFrmCount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC3fView construction/destruction

CC3fView::CC3fView()
{
}

CC3fView::~CC3fView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CC3fView message handlers
void CC3fView::OnInitialUpdate()
{
	CC3fDoc* pDoc = (CC3fDoc*)m_pDocument;//(CC3fDoc*)GetDocument();
	ASSERT(pDoc);
	m_c3f		= pDoc->m_c3f;
	m_pFlcImage	= (CFlcImage*)pDoc;
	
	CAnimationView::OnInitialUpdate();
}

void CC3fView::ShowAnimInfo() 
{
	CString sInfo = 
		uFormatString(IDS_C3F_INFO,
		CFileX::GetFileName(m_sPathName),				// Storyboard file name
		m_c3f.Name,										// Base file name
		m_c3f.FileType == FILE_PCX ? "PCX" : "BMP",		// Image file type
		uLoadString(IDS_PAL_TYPE_ACT + m_c3f.PalType),	// Palette file type
		m_c3f.FrmCount * m_c3f.DirCount,				// Total frames
		m_c3f.FrmWidth,									// Width
		m_c3f.FrmHeight,								// Height
		m_c3f.Delay,									// Speed
		m_c3f.DirCount,									// Total directions
		m_c3f.FrmCount,									// Frames per direction
		m_c3f.XOffset,									// Left offset
		m_c3f.YOffset);									// Top offset
	
	InfoShowDialog(sInfo,
		m_sPathName, 0, 
		uLoadString(IDS_C3F_INFO_CAP));
}

void CC3fView::OnExport() 
{
	if (!IsLoaded())
		return;

//	CFadeWnd wndFade(MAIN_FRAME, FALSE);

	CFileFlcDlg dlg(
		FALSE,
		FLC_EXT,
		CFileX::GetShortFileName(m_sPathName),
		OFN_ENABLESIZING | OFN_ENABLETEMPLATE |
		OFN_EXPLORER | OFN_OVERWRITEPROMPT |
		OFN_HIDEREADONLY | OFN_LONGNAMES |
		OFN_PATHMUSTEXIST,
		_T(uLoadString(IDS_FILTER_FLC)));
	
	dlg.m_ofn.hInstance = AfxGetInstanceHandle();
	dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_FLC);

	CString sTitle(uLoadString(IDS_TITLE_EXPORT_TO_FLC));
	dlg.m_ofn.lpstrTitle = sTitle;

	CWinApp* pApp = AfxGetApp();
	CString sPath = pApp->GetProfileString(REG_OPTIONS, REG_LAST_OPENED, "");
	
	if (uIsFileExist(sPath))
		dlg.m_ofn.lpstrInitialDir = sPath;
	
	if (dlg.DoModal() != IDOK)
		return;

	sPath = dlg.GetPathName();

	// .. save FLC
	if (StoryboardToFlc(sPath) != TRUE)
		return;
	
	pApp->WriteProfileString(REG_OPTIONS, REG_LAST_OPENED, sPath);

	if (MAIN_APP->IsExpOpen())
	{
		if (MAIN_APP->IsExpAsk())
		{
			if (AfxMessageBox(uFormatString(IDS_ASK_OPEN_FLC, m_sPathName, sPath), MB_ICONQUESTION|MB_YESNO) != IDYES)
				return;
		}
		
		CDocTemplate* pDocTemplate = MAIN_APP->GetDocManager()->GetDocTemplateFlc();
		if (pDocTemplate)
			pDocTemplate->OpenDocumentFile(sPath);
	}
	else
		AfxMessageBox(uFormatString(IDS_SB_TO_FLC_SUCCES, m_sPathName, sPath), MB_ICONINFORMATION);
}

void CC3fView::OnUpdateExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
}

//////////////////////////////////////////////////////////////////////////
// Load Storyboard by the given path.
// In case bColorTable == FALSE color chunk will be skipped.
//
BOOL CC3fView::LoadAnimation(LPCTSTR lpszPathName, BOOL bColorTable /*=TRUE*/)
{
//	CFadeWnd wndFade(MAIN_FRAME, FALSE);

	if (!CAnimationView::LoadAnimation(lpszPathName))
		return FALSE;
	
	DDSURFACEDESC	ddsd;
	BYTE*			db		= NULL;
	BYTE*			line	= NULL;
	
	WORD	nTotalFrames	= m_c3f.FrmCount * m_c3f.DirCount; // total frames
	WORD	nCurDirectFrame = 0; // frame order by the current direction
	
	BYTE nFrmHeight			= m_c3f.FrmHeight;
	BYTE nFrmWidth			= m_c3f.FrmWidth;
	m_nPixelsSize			= nFrmWidth * nFrmHeight;
	BYTE* pBuffer			= new BYTE[m_nPixelsSize];

	CString sPalPath(lpszPathName);
	sPalPath.Delete(sPalPath.GetLength()-3, 3);
	switch (m_c3f.PalType)
	{
	case PalACT:
		sPalPath += ACT_EXT;
		break;
	case PalMS:
	case PalJASC:
		sPalPath += PAL_EXT;
		break;
	default:
		ASSERT(FALSE);
	}

	//////////////////////////////////////////////////////////////////////////
	// NEW VARIANT - MUCH FASTER THAN ONE BELOW :)
	BYTE			nCur		= 0;			// current frame of the current direction
	WORD			nCurFrame	= 0;			// current frame of total frames
	UINT			pos			= 0;
	BYTE			idx			= 0;			// color index
	RGBQUAD*		pRgb		= NULL;
	CSurface*		pSurface	= NULL;
	WORD			nGrowX		= m_c3f.FrmWidth  + GRID_SIZE;
	WORD			nGrowY		= m_c3f.FrmHeight + GRID_SIZE;
	UINT			nIndentX	= GRID_SIZE;	// horizontal indent
	UINT			nIndentY	= m_c3f.Height - nGrowY - GRID_SIZE; // vertical indent
	BYTE			y;

	BITMAPHANDLE	hbmp		= m_pFlcImage->m_hbmp;
	L_UCHAR L_FAR*	pBuf		= NULL; // buffer to hold the row
	HGLOBAL			hBuf;				// handle to the buffer
	L_UINT			uByte;				// byte counter

	try
	{
		// Load palette from file
		if (!ImportPaletteFromFile(sPalPath, m_rgbPalDef))
		{
			// palette file is absent, but we have 8 bits storyboard
			// and can trying to get palette directly from storyboard
			if (hbmp.BitsPerPixel == BITS_8)
			{
/*				L_UINT32 dwColors = 0;

				// Get the number of unique colors in an image
				GRAPHX::GetBitmapColorCount(&hbmp, dwColors);
				
				if (dwColors != COLOR8)
				{
					CString sMsg = uFormatString(IDS_ERR_READ_PAL, sPalPath) + "\r\n\r\n";
					sMsg += uFormatString(IDS_ERR_COLORS_COUNT, dwColors);
					AfxMessageBox(sMsg);
					return FALSE;
				}
*/				
				// Load palette with selected colors from a bitmap handle's palette
				GRAPHX::GetBitmapColors(&hbmp, 0, COLOR8, m_rgbPalDef.pal);
			}
			else if (hbmp.BitsPerPixel == BITS_24)
			{
				// Try to load default palette
				CString sPalDef(sPalPath);
				sPalDef.Insert(sPalDef.GetLength() - 4, uLoadString(IDS_DEF_PALETTE_SUFFIX));
				
				// Default palette file MUST present in case 24-bit storyboard
				if (!ImportPaletteFromFile(sPalDef, m_rgbPalDef))
				{
					AfxMessageBox(uFormatString(IDS_ERR_READ_PAL, sPalDef));
					return FALSE;
				}
/*				
				// It's not working with 24 bits... :(
				BITMAPHANDLE hbmp1;
				// Convert the current bitmap to 8 bits per pixel with an optimized palette
				GRAPHX::ColorResBitmap(&hbmp, &hbmp1, 8,
					CRF_OPTIMIZEDPALETTE, 
					NULL, NULL, COLOR8, NULL, NULL);

				L_UINT32 dwColors = 0;
				
				// Get the number of unique colors in an image
				GRAPHX::GetBitmapColorCount(&hbmp1, dwColors);
				
				if (dwColors != COLOR8)
				{
					CString sMsg = uFormatString(IDS_ERR_READ_PAL, sPalPath) + "\r\n\r\n";
					sMsg += uFormatString(IDS_ERR_COLORS_COUNT, dwColors);
					AfxMessageBox(sMsg);
					return FALSE;
				}

				// Load palette with selected colors from a bitmap handle's palette
				GRAPHX::GetBitmapColors(&hbmp1, 0, dwColors, m_rgbPalDef.pal);
*/			}
			else
			{
				AfxMessageBox(uFormatString(IDS_ERR_READ_PAL, sPalPath));
				return FALSE;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// In case 24-bit storyboard we need convert it to 8-bit first
		if (hbmp.BitsPerPixel == BITS_24)
		{
			RGBQUADPAL rgbPalDef; // original image file's palette
			sPalPath.Insert(sPalPath.GetLength() - 4, uLoadString(IDS_DEF_PALETTE_SUFFIX));
		
			// Default palette file MUST present in case 24-bit storyboard
			if (!ImportPaletteFromFile(sPalPath, rgbPalDef))
			{
				AfxMessageBox(uFormatString(IDS_ERR_READ_PAL, sPalPath));
				return FALSE;
			}

			// Create temporary bitmap
			BITMAPHANDLE hTmpBbmp;
			GRAPHX::CreateBitmap(&hTmpBbmp, TYPE_CONV, m_c3f.Width, m_c3f.Height, 
				m_constBits, ORDER_BGR, NULL, TOP_LEFT, NULL, 0);
			
			// Set bitmap palette
			GRAPHX::PutBitmapColors(&hTmpBbmp, 0, COLOR8, rgbPalDef.pal);

			// Copy whole picture from 24-bit image to 8-bit
			//GRAPHX::CombineBitmap(&hTmpBbmp, 0, 0, m_c3f.Width, m_c3f.Height, &hbmp, 0, 0, 0);
			CDC* pDC		= CDC::FromHandle(GRAPHX::CreateGraphXDC(&hbmp));
			CDC* pTempDC	= CDC::FromHandle(GRAPHX::CreateGraphXDC(&hTmpBbmp));

			BitBlt(pTempDC->m_hDC, 0, 0, m_c3f.Width, m_c3f.Height, 
				pDC->m_hDC, 0, 0, SRCCOPY);

			// Copies a temp image of a source bitmap
			GRAPHX::CopyBitmap(&hbmp, &hTmpBbmp);

			GRAPHX::DeleteGraphXDC(*pDC);
			GRAPHX::DeleteGraphXDC(*pTempDC);

			// Flips a specified bitmap image from top to bottom
			GRAPHX::FlipBitmap(&hbmp);
//			GRAPHX::SaveBitmap("D:\\test.bmp", &hbmp, FILE_BMP, m_constBits);
		}
		//////////////////////////////////////////////////////////////////////////

		// Copy default palette to the current palette
		//ZeroMemory(&m_rgbPal, RGBQUADPAL_SIZE);
		memcpy(&m_rgbPal, &m_rgbPalDef, RGBQUADPAL_SIZE);
	
		// Get transparent color
		m_colTransparent = RGB(m_rgbPalDef.pal[255].rgbBlue, m_rgbPalDef.pal[255].rgbGreen, m_rgbPalDef.pal[255].rgbRed);

		if (m_bUseAlphaBlending)
		{
			m_ddcTransparent.dwColorSpaceLowValue  = m_colTransparent;
			m_ddcTransparent.dwColorSpaceHighValue = m_colTransparent;
		}
		else
		{
			m_ddcTransparent.dwColorSpaceLowValue  = RGB_ALPHA_ERROR;
			m_ddcTransparent.dwColorSpaceHighValue = RGB_ALPHA_ERROR;
		}
		
		/////////////////////////
		//clock - start
#ifdef _DEBUG
			clock_t start, finish;
			double  duration;
			start = clock();
#endif
		/////////////////////////

/*		///////////////////////////////////////////////////
		// VARIANT 1
		// Simple, but not too good, IMHO... :(
		// Yes, but it's working! ;)
		hbmp = m_pFlcImage->m_hbmp;
		
		///////////////////////////////////////////////////
		// VARIANT 2
		// To safetly overcome possible palette changes
		// in the storyboard bitmap
		
		// Copy from source bitmap to temporary
		GRAPHX::CopyBitmap(&hbmp, &m_pFlcImage->m_hbmp);
		
		// Set bitmap palette
		GRAPHX::PutBitmapColors(&hbmp, 0, COLOR8, m_rgbPal.pal);
		
		CDC* pDcD = CDC::FromHandle(GRAPHX::CreateGraphXDC(&hbmp));
		CDC* pDcS = CDC::FromHandle(GRAPHX::CreateGraphXDC(&m_pFlcImage->m_hbmp));
		
		// Copy data from source to destination
		BitBlt(pDcD->m_hDC, 0, 0, m_c3f.FrmWidth, m_c3f.FrmHeight, 
			pDcS->m_hDC, 0, 0, SRCCOPY);
		
		// Release DC
		GRAPHX::DeleteGraphXDC(*pDcD);
		GRAPHX::DeleteGraphXDC(*pDcS);
		// VARIANT 2
*/		///////////////////////////////////////////////////

		// Allocate and lock the buffer
		hBuf = GlobalAlloc(GMEM_MOVEABLE, hbmp.BytesPerLine);
		pBuf = (L_UCHAR L_FAR *)GlobalLock(hBuf);
		
		// Process each row in the bitmap
		GRAPHX::AccessBitmap(&hbmp);

		// Move through frames
		for (nCurFrame = 0; nCurFrame < nTotalFrames; nCurFrame++)
		{
			// add a new surface
			pSurface = new CSurface(m_pScreen);

			if (pSurface->Create(nFrmWidth, nFrmHeight) != S_OK)
			{
				SAFE_DELETE_MULTI(pBuffer);
				return FALSE;
			}

			m_arSFrames.Add(pSurface);

			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);

			// set transparent color key
			pSurface->GetDDS()->SetColorKey(DDCKEY_SRCBLT, &m_ddcTransparent);

			if (pSurface->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
			{
				SAFE_DELETE_MULTI(pBuffer);
				return FALSE;
			}

			db		= (BYTE*)ddsd.lpSurface;
			line	= NULL;
			pos		= 0;

			///////////////////////////////////////
			// end of the current direction
			if (nCur == m_c3f.FrmCount) 
			{
				nCur		= 0;			// to start a new direction
				nIndentX	= GRID_SIZE;	// start from the left end
				nIndentY   -= nGrowY;		// move to the next row
			}

			// Get current frame's data
			for (y = nFrmHeight; y > 0; y--)
			{
				line = db;
				GRAPHX::GetBitmapRowCol(&hbmp, pBuf, nIndentY + y, nIndentX, nFrmWidth);
				
				for (uByte = 0; uByte < nFrmWidth; uByte++)
				{
					// get aproximated color index in the bitmap palette
					idx = pBuf[uByte];
					
					// assign the color index with current pixel of the Surface
					pBuffer[pos++] = idx;
					pRgb = &m_rgbPal.pal[idx];
					
					// set color for current pixel of the surface
					line[0] = pRgb->rgbBlue;
					line[1] = pRgb->rgbGreen;
					line[2] = pRgb->rgbRed;
					line[3] = 255;
					
					line += 4;
				}

				db	+= (ddsd.lPitch);
			}
		
			// Copy pixels data to the Surface
			pSurface->flags.pixelData = new BYTE[m_nPixelsSize];
			memcpy(pSurface->flags.pixelData, pBuffer, m_nPixelsSize);
			
			pSurface->GetDDS()->Unlock(NULL);
			
			nCur++;
			nIndentX += nGrowX; // move to the next column
		}//for (nCurFrame = 0; nCurFrame < nTotalFrames; nCurFrame++)

		GRAPHX::ReleaseBitmap(&hbmp);
		
		// Free memory that we no longer need
		GlobalUnlock(hBuf);
		GlobalFree(hBuf);

		/////////////////////
		// clock - finish
#ifdef _DEBUG
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		TRACE1("\nCC3fView.LoadAnimation: %2.3f seconds\n", duration);
#endif
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		ClearSFrames();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}
	catch (CMemoryException* e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		ClearSFrames();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CC3fView::LoadAnimation"), MB_ICONERROR);
		ClearSFrames();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}

/*
	/////////////////////////////////////////////////////////////////////
	// OLD VARIANT - MUCH SLOWEST THAN ONE ABOVE
	COLORREF	color;						// current pixel's color
	WORD		nIndentX	= GRID_SIZE;	// horizontal indent
	WORD		nIndentY	= GRID_SIZE;	// vertical indent
	BYTE		nCur		= 0;			// current frame of the current direction
	WORD		nCurFrame	= 0;			// current frame of total frames
	UINT		pos			= 0;
	BYTE		idx			= 0;			// color index
	HPALETTE	hPalette	= hbmp.hPalette;
	RGBQUAD*	pRgb		= NULL;
	CSurface*	pSurface	= NULL;
	WORD		nGrowX		= m_c3f.FrmWidth  + GRID_SIZE;
	WORD		nGrowY		= m_c3f.FrmHeight + GRID_SIZE;
	BYTE		x, y;
		
	try
	{
		hbmp = m_pFlcImage->m_hbmp;
		
		// Load palette from file
		if (!ImportPaletteFromFile(sPalPath, m_rgbPalDef))
		{
			AfxMessageBox(uFormatString(IDS_ERR_READ_PAL, sPalPath));
			return FALSE;
		}

		// Get default palette from the bitmap
//		GRAPHX::GetBitmapColors (&hbmp, 0, COLOR8, m_rgbPalDef.pal);
		
		// Copy default palette to the current palette
		//ZeroMemory(&m_rgbPal, RGBQUADPAL_SIZE);
		memcpy(&m_rgbPal, &m_rgbPalDef, RGBQUADPAL_SIZE);
	
		// Get transparent color
		m_colTransparent = RGB(m_rgbPalDef.pal[255].rgbBlue, m_rgbPalDef.pal[255].rgbGreen, m_rgbPalDef.pal[255].rgbRed);

		if (m_bUseAlphaBlending)
		{
			m_ddcTransparent.dwColorSpaceLowValue  = m_colTransparent;
			m_ddcTransparent.dwColorSpaceHighValue = m_colTransparent;
		}
		else
		{
			m_ddcTransparent.dwColorSpaceLowValue  = RGB_ALPHA_ERROR;
			m_ddcTransparent.dwColorSpaceHighValue = RGB_ALPHA_ERROR;
		}

		/////////////////////////
		//clock - start
#ifdef _DEBUG
			clock_t start, finish;
			double  duration;
			start = clock();
			int test = 0;
#endif
		/////////////////////////

		// Move through frames
		for (nCurFrame = 0; nCurFrame < nTotalFrames; nCurFrame++)
		{
			// add a new surface
			pSurface = new CSurface(m_pScreen);

			if (pSurface->Create(nFrmWidth, nFrmHeight) != S_OK)
			{
				SAFE_DELETE_MULTI(pBuffer);
				return FALSE;
			}

			m_arSFrames.Add(pSurface);

			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);

			// set transparent color key
			pSurface->GetDDS()->SetColorKey(DDCKEY_SRCBLT, &m_ddcTransparent);

			if (pSurface->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
			{
				SAFE_DELETE_MULTI(pBuffer);
				return FALSE;
			}

			db		= (BYTE*)ddsd.lpSurface;
			line	= NULL;
			pos		= 0;

			///////////////////////////////////////
			// end of the current direction
			if (nCur == m_c3f.FrmCount) 
			{
				nCur		= 0;			// to start a new direction
				nIndentX	= GRID_SIZE;	// start from the left end
				nIndentY   += nGrowY;		// move to the next row
			}

			// Get current frame's data
			for (y = 0; y < nFrmHeight; y++)
			{
				line = db;
				for (x = 0; x < nFrmWidth; x++)
				{
#ifdef _DEBUG
					test++;
#endif
					// get color of the current bitmap pixel
					// NOTE: GRAPHX::GetPixelColor is faster!
					// search for 'GRAPHX::RotateBitmap' in the FlcImage.cpp
					// for additional info
//					color = ::GetPixel(hDC, nIndentX + x, nIndentY + y);
					color = GRAPHX::GetPixelColor(&hbmp, nIndentX + x, nIndentY + y);
					
					// get aproximated color index in the bitmap palette
					idx = GetNearestPaletteIndex(hPalette, color);
					
					// assign the color index with current pixel of the Surface
					pBuffer[pos++] = idx;
					pRgb = &m_rgbPal.pal[idx];

//					// Alternate variant for GetNearestPaletteIndex (slowest) :(
//					idx = GetPaletteIndex(m_rgbPal, color);
//					pBuffer[pos++] = idx;
//					pRgb = &m_rgbPal.pal[idx];

					// set color for current pixel of the surface
					line[0] = pRgb->rgbBlue;
					line[1] = pRgb->rgbGreen;
					line[2] = pRgb->rgbRed;
					line[3] = 255;
				
					line += 4;
				}//for (x = 0; x < m_c3f.FrmWidth; x++)
				db += (ddsd.lPitch);
			}//for (y = 0; y < m_c3f.FrmHeight; y++)
			
			// Copy pixels data to the Surface
			pSurface->flags.pixelData = new BYTE[m_nPixelsSize];
			memcpy(pSurface->flags.pixelData, pBuffer, m_nPixelsSize);
			
			pSurface->GetDDS()->Unlock(NULL);
			
			nCur++;
			nIndentX += nGrowX; // move to the next column
		}//for (nCurFrame = 0; nCurFrame < nTotalFrames; nCurFrame++)
		
		/////////////////////
		// clock - finish
#ifdef _DEBUG
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		TRACE1("\nCC3fView.LoadAnimation: %2.3f seconds\n", duration);
		TRACE1("\nCC3fView.LoadAnimation: %d times\n", test);
#endif
		//////////////////////

		// Release DC
//		GRAPHX::DeleteGraphXDC(*pDC);
	}
	catch (CGraphXException *e)
	{
		AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
		e->Delete();
		ClearSFrames();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}
	catch (CMemoryException* e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		ClearSFrames();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CC3fView::LoadAnimation"), MB_ICONERROR);
		ClearSFrames();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}
*/
	SAFE_DELETE_MULTI(pBuffer);

	// in case the are no normal frames..
	if (m_arSFrames.GetSize() == 0)
		return FALSE;
	
	m_sPathName = lpszPathName;

	// for leaderheads
	if (m_c3f.Delay == 0)
		m_c3f.Delay = 10;

	m_animLenth		= m_c3f.FrmCount;
	m_uSpeed		= m_c3f.Delay;
	m_speed			= (float)m_uSpeed;
	
	m_nTotalFrames	= m_arSFrames.GetSize();
	m_nDirCount		= m_c3f.DirCount;
	m_nFrmHeight	= m_c3f.FrmHeight;
	m_nFrmWidth		= m_c3f.FrmWidth;
	m_xOffset		= m_c3f.XOffset;
	m_yOffset		= m_c3f.YOffset;
	m_xPos			= m_bIgnoreOffset ? 0 : m_xOffset;
	m_yPos			= m_bIgnoreOffset ? 0 : m_yOffset;

	// for leader heads
	if (m_c3f.LeaderHead == TRUE)
	{
		// in case leader heads we should not use background image
		// and background color must be black
		SAFE_DELETE(m_bgdSurface);
		m_bgdColor = RGB(0,0,0); // background must be black
		
		m_bLeaderHead = TRUE;
		
		// change window position with leader head sizes
		SetWindowPos(NULL, 0, 0, m_c3f.FrmWidth + WND_ACX, m_c3f.FrmHeight + WND_ACY,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		UpdateBackground();
		m_bLeaderHead = FALSE;
		
		if (!m_bUseAlphaBlending)
		{
			// in case leader heads we should not use background image
			// and background color must be black
			SAFE_DELETE(m_bgdSurface);
			m_bgdColor = m_bgdDefaultColor; // background must be default
		}
		
		SetWindowPos(NULL, 0, 0, WND_CX, WND_CY,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	m_pFlcImage->CloseFile();
	return TRUE;
}

BOOL CC3fView::StoryboardToFlc(CString sPathName)
{
	if (!IsLoaded())
		return FALSE;

	CWaitCursor wait;

	FlcHeader		flcHeader;
	FlicAnimHeader	civ3Header;

	ZeroMemory(&flcHeader, sizeof(FlcHeader));

//	flcHeader.size			= 0; // Size of FLIC including this header
	flcHeader.type			= FLC_TYPE; // File type 0xAF11, 0xAF12, 0xAF30, 0xAF44, ...
	flcHeader.frames		= m_c3f.FrmCount * m_c3f.DirCount; // Number of frames
	flcHeader.width			= m_c3f.FrmWidth; // FLIC width in pixels
	flcHeader.height		= m_c3f.FrmHeight; // FLIC height in pixels
	flcHeader.depth			= 0x08; // Bits per pixel (usually 8)
//	flcHeader.flags			= 0; // Set to zero or to three
//	flcHeader.speed			= m_c3f.Delay; // Delay between frames
//	flcHeader.reserved1		= 0; // Set to zero

	// Delay between frames
	if (m_bDefaultTimer)
		flcHeader.speed	= m_c3f.Delay;
	else
		flcHeader.speed	= m_uElapse;

	if (!m_bLeaderHead)
	{
		flcHeader.created		= 0xF2F20000; // Date of FLIC creation
		flcHeader.creator		= 0xF1F1; // Serial number or compiler id
//		flcHeader.creator		= 0xCD;
	}
	else
	{
		flcHeader.created		= 0x028A0000; // Date of FLIC creation
//		flcHeader.creator		= 0xCD;
//		flcHeader.creator		= 0xF1F1; // Serial number or compiler id
	}
//	flcHeader.updated		= 0; // Date of FLIC update
//	flcHeader.updater		= 0; // Serial number, see creator
//	flcHeader.aspect_dx		= 0; // Width of square rectangle
//	flcHeader.aspect_dy		= 0; // Height of square rectangle
//	ZeroMemory(&flcHeader.reserved2, sizeof(flcHeader.reserved2));
	flcHeader.oframe1		= 128; // Offset to frame 1 (Header size - 128)
	flcHeader.oframe2		= 0; // Offset to frame 2

	if (m_bLeaderHead)
	{
		// In case leader heads we don't need to use this structure...
		ZeroMemory(&civ3Header, sizeof(FlicAnimHeader));
	}
	else
	{
		civ3Header.size			= 0x001C; // Set to the size of the FlicAnimHeader structure (currently 28 bytes or 0x001C)
		civ3Header.flags		= 0; // This field is used internally and by our FlicAnim creator and should be set to 0.
		civ3Header.num_anims	= m_c3f.DirCount; // This is equivalent to the number of directions stored in the FlicAnim.
		civ3Header.anim_length	= m_c3f.FrmCount; // This is the number of frames in each direction.
		civ3Header.x_offset		= m_c3f.XOffset; // The pixel offset from the left edge of the image to the clipped sprite.
		civ3Header.y_offset		= m_c3f.YOffset; // The pixel offset from the top edge of the image to the clipped sprite.
		civ3Header.xs_orig		= WND_W; // The pixel width of the non-clipped sprite.
		civ3Header.ys_orig		= WND_H; // The pixel height of the non-clipped sprite.
		civ3Header.anim_time	= m_c3f.FrmCount * m_c3f.Delay; // This is the frame rate of the animation.
		civ3Header.directions	= m_c3f.DirCount == 8 ? 255 : 1; // Bit flags representing which directions are present.
		ZeroMemory(&civ3Header.reserved, sizeof(civ3Header.reserved));
	}

	CMemFile mf;
	BYTE* pBuffer = new BYTE[m_nPixelsSize+m_nPixelsSize]; // allocate memory buffer

	// We will try to make temporary memory file.
	// In case all right we'll copy temporary file
	// to a specified target path...

	try
	{	
		// Write FLC header
		mf.Write(&flcHeader, sizeof(FlcHeader));
		mf.Write(&civ3Header, sizeof(FlicAnimHeader));

		WORD nFrm = 0;
		FRAMEHEADER  frameh; // frame header
		frameh.Type = FRAME_TYPE;
		ZeroMemory(&frameh.Reserved, sizeof(frameh.Reserved)); // not used (Set to 00h)
		
		CHUNKHEADER  chunkh; // chunk header
//		chunkh.Count = 0; // is not contains subchunks
//		ZeroMemory(&chunkh.Reserved, sizeof(chunkh.Reserved)); // Not used (Set to 00h)
		
		WORD nFrmHeader	= sizeof(FRAMEHEADER); // size of the frame header
		WORD nChHeader	= sizeof(CHUNKHEADER); // size of the chunk header
		DWORD size		= 0; // size of chunk data without header

		for (BYTE nDir = 0; nDir < m_c3f.DirCount; nDir++)
		{
			if (!m_bLeaderHead)
			{
				chunkh.Type = BYTE_RUN;
				size = WriteBYTE_RUN(pBuffer, nDir);
				chunkh.Size = size + nChHeader;
				
				if (nDir == 0)
				{
					// ChunkSize is the size of the chunk in bytes.
					// This value includes the size of the header 
					// itself and any subchunks contained within 
					// the chunk.
					frameh.Size	 = nFrmHeader + chunkh.Size + nChHeader + 772; // 772 - constant size of the COLOR_256 chunk
					frameh.Count = 2; // BYTE_RUN and COLOR_256
					
					// Write current frame's header
					mf.Write(&frameh, nFrmHeader);
					
					// Write BYTE_RUN chunk header
					mf.Write(&chunkh, nChHeader);
					
					// Write BYTE_RUN chunk data
					mf.Write(pBuffer, size);
					
					chunkh.Type = COLOR_256;
					size = WriteCOLOR_256(pBuffer);
					chunkh.Size = size +  nChHeader;
					
					// Write COLOR_256 chunk header
					mf.Write(&chunkh, nChHeader);
					
					// Write COLOR_256 chunk data
					mf.Write(pBuffer, size);
				}//if (nDir == 0)
				else
				{
					frameh.Size	 = nFrmHeader + chunkh.Size;
					frameh.Count = 1; // BYTE_RUN
					
					// Write current frame's header
					mf.Write(&frameh, nFrmHeader);
					
					// Write BYTE_RUN chunk header
					mf.Write(&chunkh, nChHeader);
					
					// Write BYTE_RUN chunk data
					mf.Write(pBuffer, size);
				}//else
			}// if (!m_bLeaderHead)
			else
			{
				BYTE* pBufferTmp = new BYTE[m_nPixelsSize+m_nPixelsSize];
				int nPos = 0;

				chunkh.Type = COLOR_256;
				size = WriteCOLOR_256(pBuffer);
				chunkh.Size = size +  nChHeader;
				
				// Write COLOR_256 chunk header
				memcpy(&pBufferTmp[nPos], &chunkh, nChHeader);
				nPos += nChHeader;
				
				// Write COLOR_256 chunk data
				memcpy(&pBufferTmp[nPos], pBuffer, size);
				nPos += size;
				
				chunkh.Type = BYTE_RUN;
				size = WriteBYTE_RUN(pBuffer, nDir);
				chunkh.Size = size + nChHeader;
				
				// Write BYTE_RUN chunk header
				memcpy(&pBufferTmp[nPos], &chunkh, nChHeader);
				nPos += nChHeader;
				
				// Write BYTE_RUN chunk data
				memcpy(&pBufferTmp[nPos], pBuffer, size);
				nPos += size;

				// ChunkSize is the size of the chunk in bytes.
				// This value includes the size of the header 
				// itself and any subchunks contained within 
				// the chunk.
				frameh.Size	 = nFrmHeader + chunkh.Size + nChHeader + 772; // 772 - constant size of the COLOR_256 chunk
				frameh.Count = 2; // BYTE_RUN and COLOR_256
				
				// Write current frame's header
				mf.Write(&frameh, nFrmHeader);
				mf.Write(pBufferTmp, nPos);

				SAFE_DELETE_MULTI(pBufferTmp);
			}
/*
			//////////////////////////////////////////////////////////////////////////
			// USING FLI_COPY
			chunkh.Type = FLI_COPY;
			frameh.Count = 1; // FLI_COPY
			
			for (nFrm = 0; nFrm < m_c3f.FrmCount; nFrm++)
			{
				size = WriteFLI_COPY(pBuffer, nFrm+nDir*m_c3f.FrmCount);
				chunkh.Size = size + nChHeader;
				
				frameh.Size	 = nFrmHeader + chunkh.Size;
				
				// Write current frame's header
				mf.Write(&frameh, nFrmHeader);
				
				// Write FLI_COPY chunk header
				mf.Write(&chunkh, nChHeader);
				
				// Write FLI_COPY chunk data
				mf.Write(pBuffer, size);
			}//for (nFrm = 0; nFrm < m_c3f.FrmCount; nFrm++)
*/

			//////////////////////////////////////////////////////////////////////////
			// USING DELTA_FLC
			chunkh.Type  = DELTA_FLC;
			frameh.Count = 1; // DELTA_FLC

//			ZeroMemory(pBuffer, m_nPixelsSize+m_nPixelsSize);// for tests only!
			for (nFrm = 0; nFrm < m_c3f.FrmCount; nFrm++)
			{
				size = WriteDELTA_FLC(pBuffer, nDir, nFrm+nDir*m_c3f.FrmCount);
				chunkh.Size = size + nChHeader;

				frameh.Size	 = nFrmHeader + chunkh.Size;
				
				// Write current frame's header
				mf.Write(&frameh, nFrmHeader);
				
				// Write DELTA_FLC chunk header
				mf.Write(&chunkh, nChHeader);
				
				// Write DELTA_FLC chunk data
				mf.Write(pBuffer, size);
			}//for (nFrm = 0; nFrm < m_c3f.FrmCount; nFrm++)
	
		}//for (int nDir = 0; nDir < m_c3f.DirCount; nDir++)

		// write total file size to the FLC header
		mf.SeekToEnd();
		size = mf.GetPosition();
		mf.SeekToBegin();
		mf.Write(&size, sizeof(DWORD));

		SAFE_DELETE_MULTI(pBuffer);
		pBuffer = new BYTE[size]; // allocate necessary memory

		// read all data to the buffer
		mf.SeekToBegin();
		mf.Read(pBuffer, size);
		mf.Close(); // now we should close the file

		// now we should write data on a disk
		CFile fl;
		if (!fl.Open(sPathName, 
			CFile::modeCreate | 
			CFile::modeRead | 
			CFile::modeWrite | 
			CFile::typeBinary | 
			CFile::shareDenyWrite))
		{
			AfxMessageBox(uFormatString(IDS_ERR_COPY_TMP_FILE, sPathName), MB_ICONERROR);
			return FALSE;
		}

		// write data to the file
		fl.Write(pBuffer, size);
		fl.Close();

		SAFE_DELETE_MULTI(pBuffer);
	}
	catch (CFileException *e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
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
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CC3fView::StoryboardToFlc"), MB_ICONERROR);
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Write current palette info.
//
UINT CC3fView::WriteCOLOR_256(BYTE *pBuffer)
{	
	UINT pos = 0;

	pBuffer[pos++] = 1;	// Number of color elements in map
	pBuffer[pos++] = 0;
	pBuffer[pos++] = 0;	// Color index skip count
	pBuffer[pos++] = 0;	// Number of colors in this element 0==256

	UINT size = 4;		// see above

	// Write color table
	for (short idx = 0; idx < COLOR8; idx++)
	{
		pBuffer[pos++] = m_rgbPal.pal[idx].rgbRed;
		pBuffer[pos++] = m_rgbPal.pal[idx].rgbGreen;
		pBuffer[pos++] = m_rgbPal.pal[idx].rgbBlue;
		size += 3;
	}

	return size;
}

//////////////////////////////////////////////////////////////////////////
// Write full image (byte run length compression).
//
UINT CC3fView::WriteBYTE_RUN(BYTE *pBuffer, BYTE nDir)
{
	UINT	size	 = 0; // total chunk size without header
	UINT	pos		 = 0; // current buffer position
	UINT	nPos	 = 0; // current pixel data position
	BYTE	idx		 = 0; // current color index
	char	nRes	 = 0; // resulting value
	BYTE	nLinePos = 0; // horizontal line position
	BYTE	buf[127];
	WORD	n;

	// Get pixel data from first frame of the current direction
	BYTE* pixelData = m_arSFrames[nDir * m_c3f.FrmCount]->flags.pixelData;

	for (BYTE i = 0; i < m_c3f.FrmHeight; i++)
	{
		// Unused packet data -
		// should be stored, but skipped while reading...
		pBuffer[pos++] = 0xCD; // it's always 205... 
		size += 1;

		nLinePos = 0;
		while (nLinePos < m_c3f.FrmWidth)
		{
			nRes	= 0;
			idx		= pixelData[nPos]; // get current color index

			if (pixelData[nPos+1] == idx) // next index the same as current
			{
				while (pixelData[nPos] == idx &&
					nLinePos < m_c3f.FrmWidth &&
					nRes < FLI_MAX_RUN)
				{
					nLinePos++;
					nPos++;
					nRes++;
				}
				
				pBuffer[pos++] = nRes; // size in bytes
				pBuffer[pos++] = idx;  // color index
				size += 2;
			}
			else
			{
				n = 0;
				while (pixelData[nPos+1] != idx &&
					nLinePos < m_c3f.FrmWidth &&
					nRes < FLI_MAX_RUN)
				{
					nLinePos++;
					nPos++;
					nRes++;

					// get current pixel index
					buf[n++] = idx;
					idx		 = pixelData[nPos];
				}

				nRes = -nRes; // must be negative value
				pBuffer[pos++] = nRes; // size in bytes
				size += 1;
			
				// copy pixels indexes
				memcpy(&pBuffer[pos], buf, n);
				pos  += n;
				size += n;
			}//else
		}//for (nLinePos = 0; nLinePos < m_c3f.FrmWidth; nLinePos++)
	}//for (int i = 0; i < m_c3f.FrmHeight; i++)

	return size;
}

/*
Chunk Type  7 - DELTA_FLC (FLI_SS2) - Word Aligned Delta Compression

This format contains the differences between consecutive frames.
This is the format most often used by Animator Pro for frames
other than the first frame of an animation.  It is similar to the
line coded delta (LC) compression, but is word oriented instead
of byte oriented.  The data is organized into lines and each line
is organized into packets.

The first word in the data following the chunk header contains
the number of lines in the chunk.  Each line can begin with some
optional words that are used to skip lines and set the last byte
in the line for animations with odd widths.  These optional words
are followed by a count of the packets in the line.  The line
count does not include skipped lines.


The high order two bits of the word is used to determine the
contents of the word.

Bit 15  Bit 14			Meaning

	0		0           The word contains the packet count.  The
packets follow this word.  The packet count 
can be zero; this occurs when only the last 
pixel on a line changes.

	1		0           The low order byte is to be stored in the 
last byte of the current line.  The packet 
count always follows this word.

	1		1           The word contains a line skip count.  The 
number of lines skipped is given by the 
absolute value of the word.  This word can 
be followed by more skip counts, by a last 
byte word, or by the packet count.

The packets in each line are similar to the packets for the line
coded chunk.  The first byte of each packet is a column skip count.  
The second byte is a packet type. If the packet type is positive, 
the packet type is a count of words to be copied from the packet to 
the animation image.  If the packet type is negative, the packet 
contains one more word which is to be replicated. The absolute value
of the packet type gives the number of times the word is to be 
replicated.  The high and low order byte in the replicated word do 
not necessarily have the same value.
*/
/*
The first word following the chunk header is the number of lines in the chunk. 

*/

UINT CC3fView::WriteDELTA_FLC(BYTE *pBuffer, BYTE nDir, WORD nFrm)
{
	UINT	pos			= 0;	// current buffer position
	UINT	nPos		= 0;	// current pixel data position
	char	nRes		= 0;	// resulting value
	BYTE	nLinePos	= 0;	// horizontal line position
	WORD	nBuf		= 0;	// packet buffer current size
	WORD	nLines		= 0;	// number of encoded lines in chunk
	WORD	nSkipLines	= 0;	// number of initial lines to skip
	BOOL	bDifferent	= FALSE;// is at least one difference between current and previous frame is present
	BYTE	nSkipCount	= 0;	// number of pixels in the current line to skip
	BYTE	nPacketPos	= 0;	// end position of the last packet
	WORD	nPrevFrm	= 0;	// previous frame
	BYTE	idx1		= 0;	// 1st pixels' color index
	BYTE	idx2		= 0;	// 2d pixels' color index
	BYTE	pLineBuf[10240];	// temporary line buffer
	WORD	nLineBufPos	= 0;	// line buffer current size
	BYTE	pBuf[1020];			// temporary packet buffer
	WORD	nPackets	= 0;	// packet count, skip count, or last byte value	
//	BYTE	pLineBuf[102400];	// temporary line buffer
//	UINT	nLineBufPos	= 0;	// line buffer current size
//	BYTE	pBuf[10240];		// temporary packet buffer
//	UINT	nPackets	= 0;	// packet count, skip count, or last byte value	
	
	EPacketType nPacketType;	// current packet's type

	if (nDir > 0)
		nPrevFrm = nFrm - (nDir * m_c3f.FrmCount);
	else
		nPrevFrm = nFrm;
		
	if (nPrevFrm > 0)
		nPrevFrm = nFrm - 1;
	else 
	{
		// we will compare with last frame of the same direction
		nPrevFrm = nFrm + m_c3f.FrmCount - 1;

		// in case 'preview' frame the same as 'current'
		// then we don't need to compare them...
		// NOTE: WRONG conception... :)
//		return WriteFLI_COPY(pBuffer, nFrm);
	}

//	nFrm = 1; nPrevFrm = 0;//!!! for tests only!

	// Data from this frame
	BYTE* pixelCur  = m_arSFrames[nFrm]->flags.pixelData;

	// Data from previous frame
	BYTE* pixelPrev = m_arSFrames[nPrevFrm]->flags.pixelData;

	// Store for total number of lines
	pBuffer[pos++] = 0;
	pBuffer[pos++] = 0;

	// move through lines from top to bottom
	for (BYTE nCurLine = 0; nCurLine < m_c3f.FrmHeight; nCurLine++)
	{
		nLinePos	= 0; // to start from first column of the line
		nPackets	= 0; // zeroing packets' count
		nSkipCount	= 0;
		nPacketPos	= 0;
		nLineBufPos	= 0; // zeroing current position of the temporary line buffer
		bDifferent	= FALSE;
		
//		nPos = nCurLine * m_c3f.FrmWidth;//!!! to calculate right start position
//		ZeroMemory(&pLineBuf, sizeof(pLineBuf));

		while (nLinePos < m_c3f.FrmWidth)
		{
			// The first byte of each packet is the column skip count;
			// the second byte is the RLE count byte.
			// Zero or more data words follow the RLE count byte.
			// If the count is positive, that number of words of
			// data is copied to the image;
			// if the count is negative, one data word follows and
			// the absolute value of the count tells how many times
			// that word must be replicated in the image.

			// in case some differences were found
			if (pixelCur[nPos] != pixelPrev[nPos])
			{
				if (nSkipLines > 0)
				{
					nSkipLines = -nSkipLines;
					memcpy(&pBuffer[pos], &nSkipLines, 2);
					pos += 2;

					nSkipLines	= 0;
					nPacketPos	= 0;
				}

				bDifferent  = TRUE; // some changes found
				nBuf		= 0; // "zeroing" temporary packet buffer
//				ZeroMemory(&pBuf, sizeof(pBuf));

				nSkipCount = nLinePos - nPacketPos; // set skip count before last changed and current pixels

				nPackets++; // at least one packet
				
				// copy skip count to the temporary buffer
				memcpy(&pLineBuf[nLineBufPos], &nSkipCount, 1);
				nLineBufPos++;
				
				idx1 = pixelCur[nPos++]; // get 1st color index
				idx2 = pixelCur[nPos++]; // get 2d color index
				nLinePos += 2;

				// store pixels data to the temporary buffer
				pBuf[nBuf++] = idx1;
				pBuf[nBuf++] = idx2;

				nRes = 1; // at least one repeat
				
				// we don't know the packet's type yet
				nPacketType = UnknownPacket; 

				// move through changed pixels
				while (pixelCur[nPos] != pixelPrev[nPos] &&
					nLinePos < m_c3f.FrmWidth)
				{
					// in case the next two pixels have the same
					// indexes as current pair of pixels...
					if (pixelCur[nPos] == idx1 && 
						pixelCur[nPos+1] == idx2)
					{
						if (nPacketType != LiterallyPacket)
						{
							// now we know that we should repeat pixels
							nPacketType	= RepeatPacket;
							nPos		+= 2; // to compare the next pixels' index
							nLinePos	+= 2;
							nRes++; // increase counter
						}
						else
							break; // end of the packet
					}
					else
					{
						if (nPacketType != RepeatPacket)
						{
							// now we know that we should read pixels literally
							nPacketType = LiterallyPacket;

							idx1 = pixelCur[nPos++]; // get 1st color index
							idx2 = pixelCur[nPos++]; // get 2d color index

							// store pixels data to the temporary buffer
							pBuf[nBuf++] = idx1;
							pBuf[nBuf++] = idx2;
							nLinePos	+= 2;
							nRes++; // increase counter
						}
						else
							break; // end of the packet
					}
				}//while (pixelCur[nPos] != pixelPrev[nPos] &&

				// in case repeated packet type the packet value 
				// must be negative!
				if (nPacketType == RepeatPacket)
					nRes = -nRes;

				// save type of encoding used on this packet 
				memcpy(&pLineBuf[nLineBufPos], &nRes, 1);
				nLineBufPos++;

				// save current packet data
				memcpy(&pLineBuf[nLineBufPos], pBuf, nBuf);
				nLineBufPos += nBuf;

				nPacketPos = nLinePos; // current indent (skip position)
			}//if (pixelCur[nLinePos] != pixelPrev[nLinePos])
			else
			{
				nPos++;
				nLinePos++;
			}
		}//while (nLinePos < m_c3f.FrmWidth)

		if (bDifferent)
		{
			// set total number of packets in the current line
			memcpy(&pBuffer[pos], &nPackets, 2);
			pos += 2;

			// set encoded packets data
			memcpy(&pBuffer[pos], pLineBuf, nLineBufPos);
			pos += nLineBufPos;

			nLines++; // increase number of encoded lines in chunk
		}
		else // in case lines are similar - add it to the 'skip lines'
			nSkipLines++;
		
	}//while (nLinePos < m_c3f.FrmWidth)

	// The first word following the chunk header is the number of lines
	// in the chunk. This count does not include "skipped" lines.
	
	// set total number of lines
	memcpy(pBuffer, &nLines, 2);
	pos += 2;

	return pos;
}

//////////////////////////////////////////////////////////////////////////
// This chunk contains a single, uncompressed frame of data.
// When a frame is stored uncompressed, the FLI_COPY chunk is used.
//
UINT CC3fView::WriteFLI_COPY(BYTE *pBuffer, WORD nFrm)
{
	BYTE* pixelData = m_arSFrames[nFrm]->flags.pixelData;
	memcpy(pBuffer, pixelData, m_nPixelsSize);
	
	return m_nPixelsSize;
}

void CC3fView::OnDestroy() 
{
	CAnimationView::OnDestroy();
}

void CC3fView::OnTimer(UINT nIDEvent) 
{
	CAnimationView::OnTimer(nIDEvent);
}

void CC3fView::Refresh()
{
	if (!IsLoaded())
		return;
	
	CC3fDoc* pDoc = (CC3fDoc*)m_pDocument;
	ASSERT(pDoc);
	if (!pDoc)
		return;

	if (m_pFlcImage->OpenStoryboard(m_sPathName, pDoc->m_c3f))
	{
		Shutdown();
		SAFE_DELETE(m_bgdSurface);
		SAFE_DELETE(m_pScreen);

		OnInitialUpdate();
	}
	else
	{
		AfxMessageBox(uFormatString(IDS_ERR_REFRESH, m_sPathName), MB_ICONERROR);
		pDoc->OnCloseDocument();
	}
}

void CC3fView::OnOffset() 
{
	COffsetDlg dlg(this);
	if (dlg.DoModal() != IDOK)
		return;

	m_c3f.XOffset = m_xOffset = m_xPos;
	m_c3f.YOffset = m_yOffset = m_yPos;
}

void CC3fView::OnUpdateOffset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!m_bLeaderHead && !m_bIgnoreOffset));
}

void CC3fView::OnFileSave() 
{
	if (!IsLoaded())
		return;
	
	CDocument* pDoc = GetDocument();
	if (!pDoc)
		return;
	
	if (!SaveC3F(pDoc->GetPathName()))
		return;
	
	// .. save FLC
	pDoc->DoFileSave();
}

void CC3fView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
}

//////////////////////////////////////////////////////////////////////////
// Save C3F by the given path.
// 1. Save changes to FlcHeader and FlicAnimHeader
// 4. Save current palette
//
BOOL CC3fView::SaveC3F(LPCTSTR lpszPathName)
{
	if (!ExportPaletteToFile(lpszPathName, m_rgbPal, (EPaletteFileType)m_c3f.PalType))
	{
		AfxMessageBox(IDS_ERR_WRITE_PAL);
		return FALSE;
	}

	/////////////////////////////////////////////////
	// Change FlcHeader and FlicAnimHeader
	// Note: Only for unit animation
	if (!m_bLeaderHead && !m_bDefaultTimer)
		m_c3f.Delay		= m_uElapse; // Delay between frames

	/////////////////////////////////////////////////
	// Make INI file
	if (!WriteC3F(lpszPathName, 
		m_c3f.Width,
		m_c3f.Height,
		m_c3f.FrmWidth,
		m_c3f.FrmHeight,
		m_c3f.FrmCount,
		m_c3f.DirCount,
		m_c3f.Delay,
		m_c3f.XOffset,
		m_c3f.YOffset,
		m_c3f.FileType,
		m_c3f.PalType,
		m_c3f.LeaderHead))
	{
		AfxMessageBox(IDS_ERR_WRITE_C3F, MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

void CC3fView::OnFileSaveAs() 
{
	if (!IsLoaded())
		return;
	
	CDocument* pDoc = GetDocument();
	if (!pDoc)
		return;
	
	CFileDialogX dlg(
		FALSE,
		C3F_EXT,
		pDoc->GetTitle(),
		OFN_ENABLESIZING |
		OFN_EXPLORER | OFN_OVERWRITEPROMPT |
		OFN_HIDEREADONLY | OFN_LONGNAMES |
		OFN_PATHMUSTEXIST,
		_T(uLoadString(IDS_FILTER_C3F)));
	
	dlg.m_ofn.hInstance = AfxGetInstanceHandle();

	CString sTitle(uLoadString(IDS_TITLE_SAVE_AS));
	dlg.m_ofn.lpstrTitle = sTitle;
	
	CString sPath = pDoc->GetPathName();
	CWinApp* pApp = AfxGetApp();
	
	if (uIsFileExist(sPath))
		dlg.m_ofn.lpstrInitialDir = sPath;
	else
	{
		sPath = pApp->GetProfileString(REG_OPTIONS, REG_LAST_OPENED, "");
		
		if (uIsFileExist(sPath))
			dlg.m_ofn.lpstrInitialDir = sPath;
	}
	
	if (dlg.DoModal() != IDOK)
		return;
	
	sPath = dlg.GetPathName();

	CString sImgPath, sImgPathOld;
	sImgPathOld.Format("%s%s%s", CFileX::GetFolder(pDoc->GetPathName()), m_c3f.Name, m_c3f.FileType == FILE_PCX ? ".pcx" : ".bmp");
	sImgPath.Format("%s%s%s", CFileX::GetFolder(sPath), CFileX::GetShortFileName(sPath), m_c3f.FileType == FILE_PCX ? ".pcx" : ".bmp");

	if (!CopyFile(sImgPathOld, sImgPath, FALSE))
	{
		AfxMessageBox(uFormatString(IDS_ERR_COPY_FILE, sImgPathOld, sImgPath), MB_ICONERROR);
		return;
	}
	
	// .. save C3F
	if (!SaveC3F(sPath))
		return;
	
	pDoc->DoSave(sPath);
	pApp->WriteProfileString(REG_OPTIONS, REG_LAST_OPENED, sPath);
	
	pDoc->SetModifiedFlag(FALSE);
}

void CC3fView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
}

void CC3fView::OnUpdateShrinkFrmCount(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_c3f.FrmCount > 1);
}

void CC3fView::OnShrinkFrmCount() 
{
	if (!IsLoaded() || m_c3f.FrmCount == 1)
		return;

	CShrinkFrmCountDlg dlg(this);

	if (dlg.DoModal() == IDOK && dlg.m_nFrmCount != m_c3f.FrmCount)
	{
		Shutdown();
		SAFE_DELETE(m_bgdSurface);
		SAFE_DELETE(m_pScreen);

		if (m_pDocument->OnOpenDocument(m_sPathName))
			OnInitialUpdate();
//		Refresh();
	}
}