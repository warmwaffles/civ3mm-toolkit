/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FlcView.cpp
	CREATED:	26.08.2004
	VERSION:	5.4

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CFlcView class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FlcView.h"

#include "MainFrm.h"
#include "FlcInfoDlg.h"
#include "FlcImage.h"
#include "FileFlcDlg.h"
#include "DocManagerEx.h"
#include "OffsetDlg.h"

//#include "..\MFCaids\FadeWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlcView

IMPLEMENT_DYNCREATE(CFlcView, CAnimationView)

BEGIN_MESSAGE_MAP(CFlcView, CAnimationView)
	//{{AFX_MSG_MAP(CFlcView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FLC_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_FLC_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_OFFSET, OnOffset)
	ON_UPDATE_COMMAND_UI(ID_OFFSET, OnUpdateOffset)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_SHRINK_FRM_COUNT, OnUpdateShrinkFrmCount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlcView construction/destruction

CFlcView::CFlcView()
{
}

CFlcView::~CFlcView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CFlcView message handlers
void CFlcView::OnInitialUpdate()
{
	CAnimationView::OnInitialUpdate();
}

void CFlcView::ShowAnimInfo() 
{
	CFlcInfoDlg dlg(this);
	dlg.DoModal();
}

void CFlcView::OnFileSave() 
{
	if (!IsLoaded())
		return;

	CDocument* pDoc = GetDocument();
	if (!pDoc)
		return;
	
	if (!SaveFLC(pDoc->GetPathName()))
		return;

	// .. save FLC
	pDoc->DoFileSave();
}

void CFlcView::OnFileSaveAs() 
{
	if (!IsLoaded())
		return;

	CDocument* pDoc = GetDocument();
	if (!pDoc)
		return;

	CFileFlcDlg dlg(
		FALSE,
		FLC_EXT,
		pDoc->GetTitle(),
		OFN_ENABLESIZING | OFN_ENABLETEMPLATE |
		OFN_EXPLORER | OFN_OVERWRITEPROMPT |
		OFN_HIDEREADONLY | OFN_LONGNAMES |
		OFN_PATHMUSTEXIST,
		_T(uLoadString(IDS_FILTER_FLC)));
	
	dlg.m_ofn.hInstance = AfxGetInstanceHandle();
	dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_FLC);

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

	// .. save FLC
	if (!SaveFLC(sPath))
		return;

	pDoc->DoSave(sPath);
	pApp->WriteProfileString(REG_OPTIONS, REG_LAST_OPENED, sPath);

	pDoc->SetModifiedFlag(FALSE);
}

void CFlcView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
}

void CFlcView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
}

void CFlcView::OnExport() 
{
//	CFadeWnd wndFade(MAIN_FRAME, FALSE);

	if (!IsPaused())
		OnPause();

	CFlcImage img(this);
	if (!img.FlcToStoryboard())
		return;

//	wndFade.DestroyWindow();

	if (MAIN_APP->IsExpOpen())
	{
		CString sPath(img.m_sPath);
		sPath.Delete(sPath.GetLength()-3, 3);
		sPath += C3F_EXT;
		
		if (MAIN_APP->IsExpAsk())
		{
			if (AfxMessageBox(uFormatString(IDS_ASK_OPEN_C3F, sPath), MB_ICONQUESTION|MB_YESNO) != IDYES)
				return;
		}
		
		CDocTemplate* pDocTemplate = MAIN_APP->GetDocManager()->GetDocTemplateC3f();
		if (pDocTemplate)
			pDocTemplate->OpenDocumentFile(sPath);
	}
	else
		AfxMessageBox(IDS_FLC_EXP_OK, MB_ICONINFORMATION);
}

void CFlcView::OnUpdateExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
}

//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Load FLC by the given path.
// In case bColorTable == FALSE color chunk will be skipped.
//
BOOL CFlcView::LoadAnimation(LPCTSTR lpszPathName, BOOL bColorTable /*=TRUE*/)
{
	if (!CAnimationView::LoadAnimation(lpszPathName))
		return FALSE;
	
	FILE *fp;
	BYTE* pBuffer = NULL;
	
	try
	{
		fp = fopen(lpszPathName, "rb");
		
		if (!fp)
			return FALSE;
		
		fread(&m_flcHeader, sizeof(m_flcHeader), 1, fp);
		fread(&m_civ3Header, sizeof(m_civ3Header), 1, fp);
		
		WORD nFrmHeight			= m_flcHeader.height;
		WORD nFrmWidth			= m_flcHeader.width;
		
		if (nFrmHeight > WND_H || nFrmWidth > WND_W)
			return FALSE;

		m_nPixelsSize			= nFrmWidth * nFrmHeight;
		pBuffer					= new BYTE[m_nPixelsSize];
		
		DDSURFACEDESC	ddsd;
		BYTE*			sb		= NULL;
		BYTE*			db		= NULL;
		WORD			nChunk;
		UINT			read, size;
		WORD			x, y;
		
		BYTE*			line	= NULL;
		RGBQUAD*		Ptr		= NULL;
		
		BOOL			bFrameByteRun	= FALSE; // is it was BYTE_RUN chunk 
		
		WORD			nCurFrame		= 0; // current frame
		WORD			nTotalFrames	= m_flcHeader.frames; // total frames
		WORD			nCurDirectFrame = 0; // frame order by the current direction
		
		FRAMEHEADER		frameh;
		CHUNKHEADER		chunkh;
		CHUNK			chunk;
		FRAME			frame;
		CSurface*		pSurface;
		
		while (nCurFrame < nTotalFrames)
		{
			frame.pos = ftell(fp);
			
			// Read FLC frame header
			fread(&frameh, sizeof(FRAMEHEADER), 1, fp);

/////////////////////////////////////////////
// Total check
/*
#ifdef DEBUG
			switch(frameh.Type) 
			{
			case 0xF100:
				TRACE("Frame# %d. PREFIX_TYPE - prefix chunk\n", nCurFrame);
				break;
				
			case 0xF1E0:
				TRACE("Frame# %d. SCRIPT_CHUNK - embedded 'Small' script\n", nCurFrame);
				break;
				
			case 0xF1FA:
				TRACE("Frame# %d. FRAME_TYPE - frame chunk\n", nCurFrame);
				break;
				
			case 0xF1FB:
				TRACE("Frame# %d. SEGMENT_TABLE - segment table chunk\n", nCurFrame);
				break;
				
			case 0xF1FC:
				TRACE("Frame# %d. HUFFMAN_TABLE - Huffman compression table chunk\n", nCurFrame);
				break;
		}
#endif
*/
/////////////////////////////////////////////
			if (frameh.Type != FRAME_TYPE)
			{
				// FLC's info data for only Autodesk Animator Pro
				// Skip this frame (and subtract total frames this)
				fseek(fp, frameh.Size - sizeof(frameh), SEEK_CUR);
				nTotalFrames--;
				continue;
			}
			
			size			= 0;
			bFrameByteRun	= FALSE;
			frame.size		= frameh.Size;
			frame.numch		= 0;
			pSurface		= NULL;
			
			for (nChunk = 0; nChunk < frameh.Count; nChunk++)
			{
				if (nChunk > 4)
				{
					ASSERT(FALSE);
					break;
				}
				
				chunk.pos = ftell(fp);
				
				// Read FLC chunk header
				fread(&chunkh, sizeof(CHUNKHEADER), 1, fp);
				
/////////////////////////////////////////////////
// Total check
/*
#ifdef DEBUG
			switch (chunkh.Type) 
			{
			case 3:
				TRACE("Frame# %d. CEL_DATA - registration and transparency\n", nCurFrame);
				break;
				
			case 4:
				TRACE("Frame# %d. COLOR_256 - 256-level colour palette\n", nCurFrame);
				break;
				
			case 7:
				TRACE("Frame# %d. DELTA_FLC (FLI_SS2) - delta image, word oriented RLE\n", nCurFrame);
				break;
				
			case 11:
				TRACE("Frame# %d. COLOR_64 - 64-level colour palette\n", nCurFrame);
				break;
				
			case 12:
				TRACE("Frame# %d. DELTA_FLI (FLI_LC) - delta image, byte oriented RLE\n", nCurFrame);
				break;
				
			case 13:
				TRACE("Frame# %d. BLACK - full black frame (rare)\n", nCurFrame);
				break;
				
			case 15:
				TRACE("Frame# %d. BYTE_RUN (FLI_BRUN) - full image, byte oriented RLE\n", nCurFrame);
				break;
				
			case 16:
				TRACE("Frame# %d. FLI_COPY - uncompressed image (rare)", nCurFrame);
				break;
				
			case 18:
				TRACE("Frame# %d. PSTAMP - postage stamp (icon of the first frame)\n", nCurFrame);
				break;
				
			case 25:
				TRACE("Frame# %d. DTA_BRUN - full image, pixel oriented RLE\n", nCurFrame);
				break;
				
			case 26:
				TRACE("Frame# %d. DTA_COPY - uncompressed image\n", nCurFrame);
				break;
				
			case 27:
				TRACE("Frame# %d. DTA_LC - delta image, pixel oriented RLE\n", nCurFrame);
				break;
				
			case 31:
				TRACE("Frame# %d. LABEL - frame label\n", nCurFrame);
				break;
				
			case 32:
				TRACE("Frame# %d. BMP_MASK - bitmap mask\n", nCurFrame);
				break;
				
			case 33:
				TRACE("Frame# %d. MLEV_MASK - multilevel mask\n", nCurFrame);
				break;
				
			case 34:
				TRACE("Frame# %d. SEGMENT - segment information\n", nCurFrame);
				break;
				
			case 35:
				TRACE("Frame# %d. KEY_IMAGE - key image, similar to BYTE_RUN / DTA_BRUN\n", nCurFrame);
				break;
				
			case 36:
				TRACE("Frame# %d. KEY_PAL - key palette, similar to COLOR_256\n", nCurFrame);
				break;
				
			case 37:
				TRACE("Frame# %d. REGION - region of frame differences\n", nCurFrame);
				break;
				
			case 38:
				TRACE("Frame# %d. WAVE - digitized audio\n", nCurFrame);
				break;
				
			case 39:
				TRACE("Frame# %d. USERSTRING - general purpose user data\n", nCurFrame);
				break;
				
			case 40:
				TRACE("Frame# %d. RGN_MASK - region mask\n", nCurFrame);
				break;
				
			case 41:
				TRACE("Frame# %d. LABELEX - extended frame label (includes symbolic name)\n", nCurFrame);
				break;
				
			case 42:
				TRACE("Frame# %d. SHIFT - scanline delta shifts (compression)\n", nCurFrame);
				break;
				
			case 43:
				TRACE("Frame# %d. PATHMAP - path map (segment transitions)\n", nCurFrame);
				break;
			}
#endif
*/
/////////////////////////////////////////////////
				switch (chunkh.Type)
				{
					// FLC Delta FLC
				case DELTA_FLC:
					{
						pSurface = new CSurface(m_pScreen);
						m_arSFrames.Add(pSurface);
						
						// TRACE("Frame# %d. DELTA_FLC Chunk.\n", nCurFrame);
						read = ReadDELTA_FLC(pBuffer, fp);
						fseek(fp, chunkh.Size - read, SEEK_CUR);
						
						chunk.type = ChDELTA_FLC;
						break;
					}
					
					// FLC Bitwize run-length compression
					// This chunk must be skipped
				case BYTE_RUN:
					{
						pSurface = new CSurface(m_pScreen);
						m_arSFrames.Add(pSurface);
						
						// TRACE("Frame# %d. BYTE_RUN Chunk.\n", nCurFrame);
						//fseek(fp, chunkh.Size-sizeof(CHUNKHEADER), SEEK_CUR);
						read = ReadBYTE_RUN(pBuffer, fp);
						
						// in case it's first frame of the current direction
						if (nCurDirectFrame == 0)
							bFrameByteRun = TRUE;
						
						chunk.type = ChBYTE_RUN;
						break;
					}
					
					// FLC Color chunk 256
				case COLOR_256:
					{
						// save position of the current color chunk
						// usually Civ3 FLC has just one such chunk,
						// but I have seen several exceptions, for example
						// "..\Art\units\Tactical Nuke\TacticalICBMAttackA.flc"
						m_arColorChunkPos.Add(ftell(fp));
						
						chunkh.Size = 778;
						if (bColorTable)
						{
							// TRACE("Frame# %d. COLOR_256 Chunk.\n", nCurFrame);
							read = ReadCOLOR_256(pBuffer, fp, m_rgbPalDef);
							
							// Copy default palette to the current palette
							ZeroMemory(&m_rgbPal, sizeof(m_rgbPal));
							memcpy(&m_rgbPal, &m_rgbPalDef, sizeof(m_rgbPalDef));

							// setup transparent colors - get the last color from palette
							m_colTransparent = RGB(m_rgbPalDef.pal[255].rgbBlue, m_rgbPalDef.pal[255].rgbGreen, m_rgbPalDef.pal[255].rgbRed);
						}
						else
						{
							// Skip color chunk
							// TRACE("Frame# %d. COLOR_256 Chunk - skipped.\n", nCurFrame);
							read = 778;
							fseek(fp, 772, SEEK_CUR);
						}
						
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
						
						chunk.type = ChCOLOR_256;
						break;
					}
					
					// FLC Copy chunk
				case FLI_COPY:
					{
						// add a new surface
						pSurface = new CSurface(m_pScreen);
						m_arSFrames.Add(pSurface);
						
						// TRACE("Frame# %d. FLI_COPY Chunk.\n", nCurFrame);
						read = ReadFLI_COPY(pBuffer, fp);
						fseek(fp, chunkh.Size - read, SEEK_CUR);
						
						chunk.type = ChFLI_COPY;
						break;
					}
					
					// FLC Black chunk
				case BLACK:
					{
						pSurface = new CSurface(m_pScreen);
						m_arSFrames.Add(pSurface);
						
						// TRACE("Frame# %d. BLACK Chunk.\n", nCurFrame);
						read = ReadBLACK(pBuffer, fp);
						
						chunk.type = ChBLACK;
						break;
					}

					default:
					// TRACE("Frame# %d. Default Chunk.\n", nCurFrame);
					fseek(fp, chunkh.Size - sizeof(CHUNKHEADER), SEEK_CUR);
					nTotalFrames--;
					chunk.type = ChDefault;
			}

			chunk.size = chunkh.Size;
			frame.chunks[frame.numch] = chunk;
			frame.numch++;
			//			frame.chunks.Add(chunk);
			
			size += chunkh.Size;
		}//for (nChunk = 0; nChunk < frameh.Chunks; nChunk++)
		
		// add a new surface
		if (pSurface)
		{
			if (pSurface->Create(nFrmWidth, nFrmHeight) != S_OK)
			{
				fclose(fp);
				SAFE_DELETE_MULTI(pBuffer);
				return FALSE;
			}
		}
		else
		{
			continue;
		}
		
		fseek(fp, frameh.Size - size - 16, SEEK_CUR);
		
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		
		// set transparent color key
		pSurface->GetDDS()->SetColorKey(DDCKEY_SRCBLT, &m_ddcTransparent);
		
		if (pSurface->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
		{
			fclose(fp);
			SAFE_DELETE_MULTI(pBuffer);
			return FALSE;
		}
		
		pSurface->flags.pixelData = new BYTE[m_nPixelsSize];
		memcpy(pSurface->flags.pixelData, pBuffer, 1 * m_nPixelsSize);
		
		sb		= (BYTE*)pBuffer;
		db		= (BYTE*)ddsd.lpSurface;
		line	= NULL;
		Ptr		= NULL;
		
		// fill the surface with pixels
		for (y = 0; y < nFrmHeight; y++)
		{
			line = db;
			
			for (x = 0; x < nFrmWidth; x++)
			{
				Ptr = &m_rgbPal.pal[*sb];
				
				line[0] = Ptr->rgbBlue;
				line[1] = Ptr->rgbGreen;
				line[2] = Ptr->rgbRed;
				line[3] = *sb;
				
				line += 4;
				sb++;
			}//for (x = 0; x < m_arSFrames[nCurFrame]->flags.width; x++)
			db += (ddsd.lPitch);
		}//for (y = 0; y < m_arSFrames[nCurFrame]->flags.height; y++)
		
		pSurface->GetDDS()->Unlock(NULL);
		
		if (nCurDirectFrame == m_civ3Header.anim_length)
			nCurDirectFrame = 0; // zeroing
		else
			nCurDirectFrame++;
		
		m_arFrames.Add(frame);
		frame.id++;
		
		// in case surface was created form the BYTE_RUN chunk 
		if (bFrameByteRun)
		{
			m_arSByteRun.Add(pSurface);
			m_arSFrames.RemoveAt(nCurFrame);
			continue;
		}
		
		nCurFrame++;
	}
	
	fclose(fp);
	SAFE_DELETE_MULTI(pBuffer);
	
	// in case the are no normal frames..
	if (m_arSFrames.GetSize() == 0)
	{
		//..and we have at least one full image frame
		if (m_arSByteRun.GetSize() > 0)
		{
			// we should copy these full image frames to
			// the frames array to show at least such "bad" picture
			for (int i = 0; i < m_arSByteRun.GetSize(); i++)
				m_arSFrames.Add(m_arSByteRun.GetAt(i));
			
			m_arSByteRun.RemoveAll();
			
			AfxMessageBox(uFormatString(IDS_NOT_STANDARD_FLC, lpszPathName));
		}
		else
			return FALSE;
	}
	else
	{
		// we don't need to store full image frames array
		// then clear it...
		for (int i = 0; i < m_arSByteRun.GetSize(); i++)
			SAFE_DELETE(m_arSByteRun[i]);
		
		m_arSByteRun.RemoveAll();
	}
	
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
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcView::LoadAnimation"), MB_ICONERROR);
		//AfxMessageBox(uFormatString(IDS_ERR_INVALID_FLC, lpszPathName), MB_ICONERROR);
		if (fp)
			fclose(fp);
		
		ClearSFrames();
		SAFE_DELETE_MULTI(pBuffer);
		return FALSE;
	}
	
	m_sPathName = lpszPathName;

	// for leaderheads
	if (m_flcHeader.speed == 0)
		m_flcHeader.speed = 10; 
	
	// for leader heads
	if (m_civ3Header.anim_length == 0)
	{
		m_civ3Header.anim_length	= m_arSFrames.GetSize();
		m_civ3Header.num_anims	= 1;
		
		m_civ3Header.anim_time	= m_arSFrames.GetSize() * m_flcHeader.speed;
		m_civ3Header.xs_orig	= WND_W;
		m_civ3Header.ys_orig	= WND_H;
		m_civ3Header.x_offset	= 0;//(WND_W - m_flcHeader.width) / 2;
		m_civ3Header.y_offset	= 0;//(WND_H - m_flcHeader.height) / 2;
		
		// in case leader heads we should not use background image
		// and background color must be black
		SAFE_DELETE(m_bgdSurface);
		m_bgdColor = RGB(0,0,0); // background must be black
		
		m_bLeaderHead = TRUE;
		
		// change window position with leader head sizes
		SetWindowPos(NULL, 0, 0, m_flcHeader.width + WND_ACX, m_flcHeader.height + WND_ACY,
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
	
	m_animLenth		= m_civ3Header.anim_length;
	m_uSpeed		= m_flcHeader.speed;
	m_speed			= (float)m_uSpeed;
	
	m_nTotalFrames	= m_arSFrames.GetSize();
	m_nDirCount		= (BYTE)m_civ3Header.num_anims;
	m_nFrmHeight	= (BYTE)m_flcHeader.height;
	m_nFrmWidth		= (BYTE)m_flcHeader.width;
	m_xOffset		= (BYTE)m_civ3Header.x_offset;
	m_yOffset		= (BYTE)m_civ3Header.y_offset;
	
	m_xPos			= m_bIgnoreOffset ? 0 : m_xOffset;
	m_yPos			= m_bIgnoreOffset ? 0 : m_yOffset;
/*
#ifdef _DEBUG
	CString sContent, sTmp, sType;
	int j = 0;

	for (int i = 0; i < m_arFrames.GetSize(); i++)
	{
		frame = m_arFrames.GetAt(i);
		sTmp.Format("\nFrame %d contains %d chunk(s). Pos: %d, Size: %d\n", frame.id, frame.numch, frame.pos, frame.size);
		sContent += sTmp;

		for (j = 0; j < frame.numch; j++)
		{
			switch (frame.chunks[j].type)
			{
			case ChDELTA_FLC:	sType = "DELTA_FLC";	break;
			case ChBYTE_RUN:	sType = "BYTE_RUN";		break;
			case ChCOLOR_256:	sType = "COLOR_256";	break;
			case ChBLACK:		sType = "BLACK";		break;
			case ChFLI_COPY:	sType = "FLI_COPY";		break;
			default:			sType = "unknown";
			}
		
			sTmp.Format("  Chunk %d is type %s. Pos: %d, Size: %d\n", j, sType, frame.chunks[j].pos, frame.chunks[j].size);
			sContent += sTmp;
		}
	}
	fp = fopen("D:\\test.txt", "wt");
	
	if (fp)
	{
		fwrite(sContent, sContent.GetLength(), 1, fp);
		fclose(fp);
	}
#endif
*/	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Write current FLC palette info.
//
UINT CFlcView::WriteCOLOR_256(FILE* fp)
{
	UINT	size;
	short	packets, i, j, idx;
	BYTE	skip, change;
	
	fread(&packets, sizeof(short), 1, fp);
	// CHUNKHEADER was already read
	size = sizeof(short) + sizeof(CHUNKHEADER);
	
	for (i = 0; i < packets; i++)
	{
		idx = 0;
		fread(&skip, 1, 1, fp);
		size += 1;
		// Skip requested colors
		idx += skip;
		
		fread(&change, 1, 1, fp);
		size += 1;

		// when you switch between reading and writing, there must be an
		// intervening fflush, fsetpos, fseek, or rewind operation.
		fseek(fp, 0, SEEK_CUR);
		
		// 0 is interpreted as all 256 colors
		if (change == 0)
		{
			for (idx = 0; idx < COLOR8; idx++/*, Ptr++*/)
			{
				fwrite(&m_rgbPal.pal[idx].rgbRed,	1, 1, fp);
				fwrite(&m_rgbPal.pal[idx].rgbGreen,	1, 1, fp);
				fwrite(&m_rgbPal.pal[idx].rgbBlue,	1, 1, fp);

				size += 3;
			}
			
			// when you switch between reading and writing, there must be an
			// intervening fflush, fsetpos, fseek, or rewind operation.
			fseek(fp, 0, SEEK_CUR);
		}
		else
		{
			// Read only requested colors
			for (j = 0; j < change; j++)
			{
				fwrite(&m_rgbPal.pal[idx].rgbRed,	1, 1, fp);
				fwrite(&m_rgbPal.pal[idx].rgbGreen,	1, 1, fp);
				fwrite(&m_rgbPal.pal[idx].rgbBlue,	1, 1, fp);

				size += 3;
				idx++;
			}
			
			// when you switch between reading and writing, there must be an
			// intervening fflush, fsetpos, fseek, or rewind operation.
			fseek(fp, 0, SEEK_CUR);
		}
	}//for (i = 0; i < packets; i++)

	return size;
}
/*
//////////////////////////////////////////////////////////////////////////
// Save FLC by the given path with current palette.
//
BOOL CFlcView::SavePalette(LPCTSTR lpszPathName)
{
	if (m_arColorChunkPos.GetSize() == 0)
		return FALSE;

	CString  sTempFile = uGetTempFileName("FLC");//"D:\\save test.flc";

	try
	{
		// Create copy of the FLC file
		if (!CopyFile(m_sPathName, sTempFile, FALSE))
			return FALSE;
		
		FILE *fp;
		fp = fopen(sTempFile, "r+b");
		
		if (!fp)
		{
			DeleteFile(sTempFile);
			return FALSE;
		}
		
		for (int i = 0; i < m_arColorChunkPos.GetSize(); i++)
		{
			// move to the current color chunk position
			if (fseek(fp, m_arColorChunkPos.GetAt(i), SEEK_SET) != 0)
			{
				fclose(fp);
				DeleteFile(sTempFile);
				return FALSE;
			}

			WriteCOLOR_256(fp);
		}
		
		fclose(fp); // now we should close the file
		
		if (!CopyFile(sTempFile, lpszPathName, FALSE))
		{
			DeleteFile(sTempFile); // we don't need more the temp file
			return FALSE;
		}
		
		DeleteFile(sTempFile); // we don't need more the temp file
	}
	catch (CMemoryException *e)
	{
//#ifdef _DEBUG
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
//#endif
		e->Delete();
		DeleteFile(sTempFile);
		return FALSE;
	}
	catch (CFileException *e)
	{
//#ifdef _DEBUG
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
//#endif
		e->Delete();
		DeleteFile(sTempFile);
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcView::SavePalette"), MB_ICONERROR);
		return FALSE;
	}

	m_sPathName = lpszPathName; // set a new name
	return TRUE;
}
*/
//////////////////////////////////////////////////////////////////////////
// Clear frames' surfaces and buffers.
//
void CFlcView::ClearSFrames()
{
	CAnimationView::ClearSFrames();

	for (int i = 0; i < m_arSByteRun.GetSize(); i++)
		SAFE_DELETE(m_arSByteRun[i]);
	
	m_arSByteRun.RemoveAll();

	m_arColorChunkPos.RemoveAll();
	m_arFrames.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// Read 256-level color palette info from file.
//
UINT CFlcView::ReadCOLOR_256(BYTE *pBuffer, FILE* fp, RGBQUADPAL& rgbPal)
{
	UINT	read;
	short	packets, i, j, idx;
	BYTE	skip, change;
	
	fread(&packets, sizeof(short), 1, fp);
	// CHUNKHEADER was already read
	read = sizeof(short) + sizeof(CHUNKHEADER);
	
	for (i = 0; i < packets; i++)
	{
		idx = 0;
		
		fread(&skip, 1, 1, fp);
		read += 1;
		// Skip requested colors
		idx += skip;
		
		fread(&change, 1, 1, fp);
		read += 1;
		
		// 0 is interpreted as all 256 colors
		if (change == 0)
		{
			for (idx = 0; idx < COLOR8; idx++/*, Ptr++*/)
			{
				fread(&rgbPal.pal[idx].rgbRed, 1, 1, fp);
				fread(&rgbPal.pal[idx].rgbGreen, 1, 1, fp);
				fread(&rgbPal.pal[idx].rgbBlue, 1, 1, fp);
				//Ptr->rgbReserved = 0;
				
				read += 3;
			}
		}
		else
		{
			// Read only requested colors
			for (j = 0; j < change; j++)
			{
				fread(&rgbPal.pal[idx].rgbRed, 1, 1, fp);
				fread(&rgbPal.pal[idx].rgbGreen, 1, 1, fp);
				fread(&rgbPal.pal[idx].rgbBlue, 1, 1, fp);
				read += 3;
				idx++;
			}
		}
	}//for (i = 0; i < packets; i++)
	
	// Compressed data must be zero-padded to end on a 16-bit boundary
	if ((read % 2) == 1)
	{
		fseek(fp, 1, SEEK_CUR);
		read++;
	}

	return read;
}

//////////////////////////////////////////////////////////////////////////
// Read word-oriented delta compression from file.
//
UINT CFlcView::ReadDELTA_FLC(BYTE *pBuffer, FILE* fp)
{
	UINT	read, pos = 0;
	short	lines, l, j, i;
	short	b, color;
	BYTE	skip;
	char	change;
	
	// Number of lines that should change
	fread(&lines, sizeof(short), 1, fp);
	read = sizeof(CHUNKHEADER) + sizeof(short);

	l = 0;
	while (lines > 0)
	{
		pos = l * m_flcHeader.width;
		
		fread(&b, sizeof(short), 1, fp);
		read += sizeof(short);

		// Number of packets following
		if ((b & 0xC000) == 0x0000) // if 14 bit == 0
		{
			b &= 0x3FFF;   // Number of packets in low 14 bits
			
			for (j = 0; j < b; j++)
			{
				// Skip unchanged pixels
				fread(&skip, 1, 1, fp);
				read += 1;
				pos += skip;
				
				// Pixels to change
				fread(&change, 1, 1, fp);
				read += 1;
				
				if (change  > 0)
				{
					for (i = 0; i < change; i++)
					{
						fread(&color, sizeof(short), 1, fp);
						read += sizeof(short);
						pBuffer[pos++] = (color & 0x00FF);
						pBuffer[pos++] = ((color >> 8) & 0x00FF);
					}
				}
				else
				{
					change = -change;
					fread(&color, sizeof(short), 1, fp);
					read += sizeof(short);
					
					for (i = 0; i < change; i++)
					{
						pBuffer[pos++] = (color & 0x00FF);
						pBuffer[pos++] = ((color >> 8) & 0x00FF);
					}
				}
			}
			
			lines--;
			l++;
		}
		// Number of lines that we should skip
		else if ((b & 0xC000) == 0xC000) 
			l -= b;
		// Color of last pixel in row
		else
			pBuffer[pos++] = b & 0x00FF;
	}
	
	// Compressed data must be zero-padded to end on a 32-bit boundary
	if ((read % 4) == 1)
	{
		fseek(fp, 2, SEEK_CUR);
		read++;
	}
	
	return read;
}

//////////////////////////////////////////////////////////////////////////
// Read "black" chunk - entire frame is color index 0 (full black frame).
//
UINT CFlcView::ReadBLACK(BYTE *pBuffer, FILE* fp)
{
	ASSERT(FALSE);
	memset(pBuffer, 0, m_nPixelsSize);
	
	return sizeof(CHUNKHEADER);
}

//////////////////////////////////////////////////////////////////////////
// Read full image (byte run length compression) from file.
//
UINT  CFlcView::ReadBYTE_RUN(BYTE *pBuffer, FILE* fp)
{
	UINT	read = sizeof(CHUNKHEADER);
	UINT	pos  = 0;
	BYTE	packets, color;
	short	k, j;
	short	line = 0;
	char	size;

	while (line++ < m_flcHeader.height)
	{
		fread(&packets, 1, 1, fp);
		read += 1;
		
		k = 0;
		while (k < m_flcHeader.width)
		{
			fread(&size, 1, 1, fp);
			read += 1;
			
#ifdef _DEBUG
			if (size == 0) // in principle, it's impossible...
			{
				ASSERT(FALSE);
			}
#endif

			// If the value of this byte is a positive number 
			// then the next byte is to be read and repeated 
			// "type" times.
			if (size > -1)
			{
				fread(&color, 1, 1, fp);
				read += 1;
				k += size;
				
				for (j = 0; j < size; j++)
					pBuffer[pos++] = color;
			}
			// If the value is negative then it is converted 
			// to its absolute value and the next "type" pixels 
			// are read literally from the encoded data.
			else
			{
				size	 = -size;
				read	+= size;
				k		+= size;
				
				for (j = 0; j < size; j++)
				{
					fread(&color, 1, 1, fp);
					pBuffer[pos++] = color;
				}
			}
		}//while (k < m_flcHeader.width)
	}//while (line++ < m_flcHeader.height)
	
	// Compressed data must be zero-padded to end on a 16-bit boundary
	//  if ((read % 2) == 1)
	//  {
	//	  fseek(fp, 1, SEEK_CUR);
	//	  read++;
	//  }
	
	return read;
}

//////////////////////////////////////////////////////////////////////////
// Read not compressed info from file.
//
UINT CFlcView::ReadFLI_COPY(BYTE *pBuffer, FILE* fp)
{
	UINT  read;
	
	read = sizeof(CHUNKHEADER);
	//printf("\n%d %d",read, size);
	//exit(1);
	
	fread(pBuffer, m_nPixelsSize, 1, fp);
	read += m_nPixelsSize;
	
	// Compressed data must be zero-padded to end on a 16-bit boundary
	//if ((read % 2) == 1) {
	//  fseek(f, 1, SEEK_CUR);
	//  read++;
	//}
	
	return read;
}

void CFlcView::OnDestroy() 
{
	CAnimationView::OnDestroy();
}

void CFlcView::OnTimer(UINT nIDEvent) 
{
	CAnimationView::OnTimer(nIDEvent);
}

void CFlcView::Refresh()
{
	if (!IsLoaded())
		return;

	Shutdown();
	SAFE_DELETE(m_bgdSurface);
	SAFE_DELETE(m_pScreen);

	OnInitialUpdate();
}

void CFlcView::OnOffset() 
{
	COffsetDlg dlg(this);
	if (dlg.DoModal() != IDOK)
		return;
	
	m_civ3Header.x_offset = m_xOffset = m_xPos;
	m_civ3Header.y_offset = m_yOffset = m_yPos;
}

void CFlcView::OnUpdateOffset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!m_bLeaderHead && !m_bIgnoreOffset));
}

//////////////////////////////////////////////////////////////////////////
// Save FLC by the given path.
// 1. Save changes to FlcHeader and FlicAnimHeader
// 4. Save current palette
//
BOOL CFlcView::SaveFLC(LPCTSTR lpszPathName)
{
	if (m_arColorChunkPos.GetSize() == 0)
		return FALSE;
	
	CString  sTempFile = uGetTempFileName(FLC_EXT);//"D:\\save test.flc";
	
	try
	{
		// Create copy of the FLC file
		if (!CopyFile(m_sPathName, sTempFile, FALSE))
			return FALSE;
		
		FILE *fp;
		fp = fopen(sTempFile, "r+b");
		
		if (!fp)
		{
			DeleteFile(sTempFile);
			return FALSE;
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Change 'Updater' info
//		m_flcHeader.updater = 0xCD;

		/////////////////////////////////////////////////
		// Change FlcHeader and FlicAnimHeader
		// Note: Only for unit animation
		if (!m_bLeaderHead && !m_bDefaultTimer)
		{
			m_flcHeader.speed		= m_uElapse; // Delay between frames
			m_civ3Header.anim_time	= m_animLenth * m_uElapse; // Frame rate of the animation.
		}

		// Write FLC header
		fwrite(&m_flcHeader, sizeof(FlcHeader), 1, fp);

		if (!m_bLeaderHead)
			fwrite(&m_civ3Header, sizeof(FlicAnimHeader), 1, fp);
		else
		{
			FlicAnimHeader civ3Header;
			ZeroMemory(&civ3Header, sizeof(FlicAnimHeader));
			fwrite(&civ3Header, sizeof(FlicAnimHeader), 1, fp);
		}
		
		fseek(fp, 0L, SEEK_SET); // move back to start
		for (int i = 0; i < m_arColorChunkPos.GetSize(); i++)
		{
			// move to the current color chunk position
			if (fseek(fp, m_arColorChunkPos.GetAt(i), SEEK_SET) != 0)
			{
				fclose(fp);
				DeleteFile(sTempFile);
				return FALSE;
			}
			
			WriteCOLOR_256(fp);
		}
		
		fclose(fp); // now we should close the file
		
		if (!CopyFile(sTempFile, lpszPathName, FALSE))
		{
			AfxMessageBox(uFormatString(IDS_ERR_COPY_FILE, sTempFile, lpszPathName), MB_ICONERROR);
			DeleteFile(sTempFile); // we don't need more the temp file
			return FALSE;
		}
		
		DeleteFile(sTempFile); // we don't need more the temp file
	}
	catch (CMemoryException *e)
	{
		//#ifdef _DEBUG
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		//#endif
		e->Delete();
		DeleteFile(sTempFile);
		return FALSE;
	}
	catch (CFileException *e)
	{
		//#ifdef _DEBUG
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		//#endif
		e->Delete();
		DeleteFile(sTempFile);
		return FALSE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CFlcView::SaveFLC"), MB_ICONERROR);
		return FALSE;
	}
	
	m_sPathName = lpszPathName; // set a new name
	return TRUE;
}

void CFlcView::OnUpdateShrinkFrmCount(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}
