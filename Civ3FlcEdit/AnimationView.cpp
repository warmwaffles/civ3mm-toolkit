/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	AnimationView.cpp
	CREATED:	26.08.2004
	VERSION:	5.10

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CAnimationView class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "AnimationView.h"

#include "MainFrm.h"
#include "FlcDoc.h"
#include "ChildFrm.h"
#include "FlcImage.h"

#include "ColorTableDlg.h"
#include "FlcInfoDlg.h"
#include "FileFlcDlg.h"
#include "FileWavDlg.h"
#include "InfoDialog.h"
#include "ZoomingDlg.h"
#include "ColorReplaceDlg.h"
#include "SelectBgdImgDlg.h"

#include "..\MFCaids\ColourPickerXP.h"

#include <Mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern CChildFrm *FRAME_WND;
#define STATIC_MAX		MAX_FRM_COUNT+1

/////////////////////////////////////////////////////////////////////////////
// CAnimationView

IMPLEMENT_DYNCREATE(CAnimationView, CView)

BEGIN_MESSAGE_MAP(CAnimationView, CView)
	//{{AFX_MSG_MAP(CAnimationView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_PLAY, OnPlay)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(ID_INFO, OnInfo)
	ON_COMMAND(ID_BGD_DEFAULT, OnBgdDefault)
	ON_COMMAND(ID_BGD_SELECT, OnBgdSelect)
	ON_COMMAND(ID_BGD_SEA, OnBgdSea)
	ON_COMMAND(ID_BGD_LAND, OnBgdLand)
	ON_COMMAND(ID_DIRECTION_SW, OnDirectionSW)
	ON_COMMAND(ID_DIRECTION_W, OnDirectionW)
	ON_COMMAND(ID_DIRECTION_NW, OnDirectionNW)
	ON_COMMAND(ID_DIRECTION_N, OnDirectionN)
	ON_COMMAND(ID_DIRECTION_NE, OnDirectionNE)
	ON_COMMAND(ID_DIRECTION_E, OnDirectionE)
	ON_COMMAND(ID_DIRECTION_SE, OnDirectionSE)
	ON_COMMAND(ID_DIRECTION_S, OnDirectionS)
	ON_COMMAND(ID_DIRECTION_ALL, OnDirectionAll)
	ON_COMMAND(ID_CUSTOM_COLOR, OnCustomColor)
	ON_COMMAND(ID_FIRST, OnFirst)
	ON_COMMAND(ID_PREVIOUS, OnPrevious)
	ON_COMMAND(ID_NEXT, OnNext)
	ON_COMMAND(ID_LAST, OnLast)
	ON_COMMAND(ID_COLOR_TABLE, OnColorTable)
	ON_COMMAND(ID_BACK, OnBack)
	ON_COMMAND(ID_FORWARD, OnForward)
	ON_COMMAND(ID_IMPORT_PALETTE, OnImportPalette)
	ON_COMMAND(ID_EXPORT_PALETTE, OnExportPalette)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_USE_ALPHA_BLENDING, OnUseAlphaBlending)
	ON_COMMAND(ID_COPY_TO_CLIPBOARD, OnCopyToClipboard)
	ON_COMMAND(ID_COPY_TO_FILE, OnCopyToFile)
	ON_UPDATE_COMMAND_UI(ID_PLAY, OnUpdateFlcPlay)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdateFlcPause)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateFlcStop)
	ON_UPDATE_COMMAND_UI(ID_INFO, OnUpdateFlcInfo)
	ON_UPDATE_COMMAND_UI(ID_BGD_DEFAULT, OnUpdateBgdDefault)
	ON_UPDATE_COMMAND_UI(ID_BGD_LAND, OnUpdateBgdLand)
	ON_UPDATE_COMMAND_UI(ID_BGD_SEA, OnUpdateBgdSea)
	ON_UPDATE_COMMAND_UI(ID_BGD_SELECT, OnUpdateBgdSelect)
	ON_UPDATE_COMMAND_UI(ID_CUSTOM_COLOR, OnUpdateFlcCustomColor)
	ON_UPDATE_COMMAND_UI(ID_FIRST, OnUpdateFlcFirst)
	ON_UPDATE_COMMAND_UI(ID_PREVIOUS, OnUpdateFlcPrevious)
	ON_UPDATE_COMMAND_UI(ID_NEXT, OnUpdateFlcNext)
	ON_UPDATE_COMMAND_UI(ID_LAST, OnUpdateFlcLast)
	ON_UPDATE_COMMAND_UI(ID_COLOR_TABLE, OnUpdateFlcColorTable)
	ON_UPDATE_COMMAND_UI(ID_BACK, OnUpdateFlcBack)
	ON_UPDATE_COMMAND_UI(ID_FORWARD, OnUpdateFlcForward)
	ON_UPDATE_COMMAND_UI(ID_USE_ALPHA_BLENDING, OnUpdateUseAlphaBlending)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSEALL, OnUpdateWindowCloseall)
	ON_COMMAND(ID_SAVE_FRAME, OnSaveFrameToFile)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_PALETTE, OnUpdateLoaded)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_ALL, OnUpdateDirectionAll)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_E, OnUpdateDirectionE)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_N, OnUpdateDirectionN)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_NE, OnUpdateDirectionNE)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_NW, OnUpdateDirectionNW)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_S, OnUpdateDirectionS)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_SE, OnUpdateDirectionSE)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_SW, OnUpdateDirectionSW)
	ON_UPDATE_COMMAND_UI(ID_DIRECTION_W, OnUpdateDirectionW)
	ON_COMMAND(ID_COLOR_REPLACE, OnColorReplace)
	ON_COMMAND(ID_BGD_GRID, OnBgdGrid)
	ON_UPDATE_COMMAND_UI(ID_BGD_GRID, OnUpdateBgdGrid)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_PALETTE, OnUpdateLoaded)
	ON_UPDATE_COMMAND_UI(ID_REFRESH, OnUpdateLoaded)
	ON_UPDATE_COMMAND_UI(ID_COPY_TO_FILE, OnUpdateLoaded)
	ON_UPDATE_COMMAND_UI(ID_ZOOM, OnUpdateLoaded)
	ON_UPDATE_COMMAND_UI(ID_SAVE_FRAME, OnUpdateLoaded)
	ON_UPDATE_COMMAND_UI(ID_COPY_TO_CLIPBOARD, OnUpdateLoaded)
	ON_COMMAND(ID_BGD_IMG, OnBgdImg)
	ON_UPDATE_COMMAND_UI(ID_BGD_IMG, OnUpdateBgdImg)
	ON_COMMAND(ID_SELECT_IMG, OnSelectImg)
	ON_UPDATE_COMMAND_UI(ID_SELECT_IMG, OnUpdateSelectImg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationView construction/destruction

CAnimationView::CAnimationView()
{
	m_pDlgBar	= MAIN_FRAME->GetDlgBar();
	m_pScreen	= NULL;
	
	// set default transparent colors
	m_colTransparent					   = RGB_TRANSPARENT;
	m_ddcTransparent.dwColorSpaceLowValue  = RGB_TRANSPARENT;
	m_ddcTransparent.dwColorSpaceHighValue = RGB_TRANSPARENT;
	
	m_bUseAlphaBlending		= TRUE;
	
	m_bPlayStatic			= FALSE;
	m_bLoaded				= FALSE;
	m_bLeaderHead			= FALSE;
	m_bPaused				= FALSE;
	m_bPlaying				= FALSE;
	m_bActive				= FALSE;
	
	m_bDefaultTimer			= TRUE;
	m_uElapse				= 0;
	m_uSpeed				= 0;
	
	m_bgdSurface			= NULL;
	m_dwStartFrame			= 0;
	m_dwEndFrame			= 0;
	m_frameTime				= 0;
	
	m_nTotalFrames			= 0;
	m_nDirCount				= 0;
	m_nFrmHeight			= 0;
	m_nFrmWidth				= 0;
	m_xPos					= 0;
	m_yPos					= 0;
	m_xOffset				= 0;
	m_yOffset				= 0;
	
	m_animLenth				= 0;
	m_firstFrame			= 0;
	m_lastFrame				= 0;
	m_curFrame				= 0;
	m_prevFrame				= STATIC_MAX;
	m_nCurPalette			= 0;
	
	m_eDirection			= DirAll;
	m_eDirectionDef			= DirAll;
	m_eBackground			= BgdDefault;
	
	m_alphaShadow			= NULL;
	m_alphaSmoke			= NULL;
	
	m_bgdColor = m_bgdCustomColor = m_bgdDefaultColor = ::GetSysColor(COLOR_BTNFACE);

	m_pPanelAnimation	= NULL;
	m_pPanelDelay		= NULL;
	m_pPanelDirection	= NULL;
	m_pPanelExtra		= NULL;
	m_pPanelSound		= NULL;
	m_pPanelView		= NULL;
}

CAnimationView::~CAnimationView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAnimationView diagnostics

#ifdef _DEBUG
void CAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnimationView message handlers

void CAnimationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	Init();
}

/////////////////////////////////////////////////////////////////////////////
// CAnimationView message handlers
void CAnimationView::UpdateControlsState()
{
	m_bPlayStatic = FALSE;

	if (!m_bActive)
		return;

	m_pDlgBar->UpdateControlsState();
}

void CAnimationView::OnPlay() 
{
	StopRewind();
	Start();

	if (m_pPanelSound->m_bNonStopWAV)
		PlaySoundFile();

	UpdateControlsState();

	if (m_bActive)
	{
		m_pPanelAnimation->m_wndFrameSlider.ClearSel();
		m_pPanelAnimation->m_wndFrameSlider.SetRangeMin(0);
	}
	
	UpdateFrameSlider();
}

void CAnimationView::OnPause() 
{
	StopRewind();
	m_bPaused = !m_bPaused;

	if (m_bPaused)
		StopSoundFile();
	else if (m_pPanelSound->m_bNonStopWAV)
		PlaySoundFile();

	UpdateControlsState();
}

void CAnimationView::OnStop() 
{
	StopRewind();
	UseDefaultTimer();
	Stop();

	if (m_bActive)
	{
		m_pPanelDelay->m_nTimerValue = 0;
		m_pPanelDelay->UpdateData(FALSE);
		UpdateInfo();
		UpdateControlsState();
		
		m_pPanelAnimation->m_wndFrameSlider.ClearSel();
		m_pPanelAnimation->m_wndFrameSlider.SetRangeMin(0);
		UpdateFrameSlider();
	}
}

void CAnimationView::OnInfo() 
{
	if (!IsLoaded())
		return;

	ShowAnimInfo();
}

void CAnimationView::OnUpdateFlcPlay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnPlay.IsWindowEnabled());
}

void CAnimationView::OnUpdateFlcPause(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnPause.IsWindowEnabled());
	pCmdUI->SetCheck(IsPaused());
}

void CAnimationView::OnUpdateFlcStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnStop.IsWindowEnabled());
}

void CAnimationView::OnUpdateFlcInfo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelExtra->m_btnInfo.IsWindowEnabled());
}

void CAnimationView::OnDirectionSW() 
{
	SetDirection(DirSW);
}

void CAnimationView::OnDirectionW() 
{
	SetDirection(DirW);
}

void CAnimationView::OnDirectionNW() 
{
	SetDirection(DirNW);
}

void CAnimationView::OnDirectionN() 
{
	SetDirection(DirN);
}

void CAnimationView::OnDirectionNE() 
{
	SetDirection(DirNE);
}

void CAnimationView::OnDirectionE() 
{
	SetDirection(DirE);
}

void CAnimationView::OnDirectionSE() 
{
	SetDirection(DirSE);
}

void CAnimationView::OnDirectionS() 
{
	SetDirection(DirS);
}

void CAnimationView::OnDirectionAll() 
{
	SetDirection(DirAll);
}

void CAnimationView::OnBgdDefault() 
{
	ResetBackgroundColor();
}

void CAnimationView::OnUpdateBgdDefault(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
	pCmdUI->SetRadio(m_eBackground == BgdDefault);	
}

void CAnimationView::OnBgdSelect() 
{
	// Here should be added color peaker...
	CRect rDraw;
	CPoint pt;
	CToolBarX* toolbar = MAIN_FRAME->GetToolBar();
	if (toolbar->IsWindowVisible())
	{
		toolbar->GetToolBarCtrl().GetRect(ID_BGD_SELECT, &rDraw);
		toolbar->ClientToScreen(rDraw);
		
		if (rDraw.left > 0 && rDraw.bottom > 0)
		{
			pt.x = rDraw.left;
			pt.y = rDraw.bottom;
		}
		else
			GetCursorPos(&pt);
	}
	else
		GetCursorPos(&pt);
	
	new CColourPopupXP(pt,				// Point to display popup
		MAIN_FRAME->m_crBgd,			// Selected colour
		this,							// parent
		uLoadString(IDS_COLOR_DEFAULT),	// "Default" text area
		uLoadString(IDS_COLOR_CUSTOM),	// Custom Text
		ID_BGD_SELECT);					// Sender
}

LONG CAnimationView::OnSelEndOK(UINT lParam, LONG wParam)
{
	if (wParam == ID_BGD_SELECT)
	{
		COLORREF color = (COLORREF)lParam;
		if (color == CLR_DEFAULT)
		{
			color = GetDefaultBgdColor();
			MAIN_FRAME->m_crBgd = color;
			ResetBackgroundColor();
			SetBackgroundColor(MAIN_FRAME->m_crBgd);
		}
		else
		{
			MAIN_FRAME->m_crBgd = color;
			SetBackgroundColor(MAIN_FRAME->m_crBgd);
		}

		MAIN_FRAME->GetToolBar()->Invalidate();
	}
	
    return TRUE;
}

void CAnimationView::OnUpdateBgdSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
}

void CAnimationView::OnBgdSea() 
{
	if (!SetBgdImg(BgdSea))
		AfxMessageBox(IDS_ERR_SET_BGD_IMG, MB_ICONERROR);
}

void CAnimationView::OnUpdateBgdSea(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
	pCmdUI->SetRadio(m_eBackground == BgdSea);
}

void CAnimationView::OnBgdLand() 
{
	if (!SetBgdImg(BgdLand))
		AfxMessageBox(IDS_ERR_SET_BGD_IMG, MB_ICONERROR);
}

void CAnimationView::OnUpdateBgdLand(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
	pCmdUI->SetRadio(m_eBackground == BgdLand);
}

void CAnimationView::OnCustomColor() 
{
	SetBackgroundColor(m_bgdCustomColor);
}

void CAnimationView::OnUpdateFlcCustomColor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
	pCmdUI->SetRadio(m_eBackground == BgdCustom);
}

void CAnimationView::OnFirst() 
{
	StopRewind();
	SetFramePos(FrmFirst);
}

void CAnimationView::OnUpdateFlcFirst(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnFirst.IsWindowEnabled());
}

void CAnimationView::OnPrevious() 
{
	StopRewind();
	SetFramePos(FrmPrevious);
}

void CAnimationView::OnUpdateFlcPrevious(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnPrevious.IsWindowEnabled());
}

void CAnimationView::OnNext() 
{
	StopRewind();
	SetFramePos(FrmNext);
}

void CAnimationView::OnUpdateFlcNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnNext.IsWindowEnabled());
}

void CAnimationView::OnLast() 
{
	StopRewind();
	SetFramePos(FrmLast);
}

void CAnimationView::OnUpdateFlcLast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnLast.IsWindowEnabled());
}

void CAnimationView::UpdateInfo()
{
	if (!m_bActive)
		return;

	if (IsLoaded())
	{
		m_pPanelAnimation->m_sFirstFrame	= "1";
		m_pPanelAnimation->m_sCurFrame		= uInt2Str(m_curFrame - m_firstFrame + (m_eDirection == DirAll ? 0 : 1));
		m_pPanelAnimation->m_sTotalFrame	= uInt2Str(m_lastFrame - m_firstFrame + (m_eDirection == DirAll ? 0 : 1));
	}
	else
	{
		m_pPanelAnimation->m_sFirstFrame	= "0";
		m_pPanelAnimation->m_sCurFrame		= "0";
		m_pPanelAnimation->m_sTotalFrame	= "0";
	}

	m_pPanelAnimation->UpdateData(FALSE);
}

void CAnimationView::ReleasedcaptureFramesSlider(WORD pos) 
{
	m_curFrame		= m_firstFrame + pos;
	m_bPlayStatic	= FALSE;
	
	UpdateInfo();
}

void CAnimationView::UpdateFrameSlider()
{
	if (!m_bActive)
		return;

	m_pPanelAnimation->m_wndFrameSlider.SetRangeMax(m_lastFrame - m_firstFrame);
	m_pPanelAnimation->m_wndFrameSlider.SetPos(m_curFrame - m_firstFrame);
}

LRESULT CAnimationView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
		// Scroll message from the sliders
	case WM_HSCROLL:
        {
            // Get the current position
            if ((LOWORD(wParam) == TB_THUMBTRACK))
			{
				if (IsLoaded())
				{
					int	nPos = HIWORD(wParam);
					m_curFrame = m_firstFrame + nPos;
					UpdateInfo();
				}
			}
		}
		break;
	}
	return CView::WindowProc(message, wParam, lParam);
}

void CAnimationView::OnRadioTimerDef() 
{
//	UpdateData();
	UseDefaultTimer();
	UpdateControlsState();
}

void CAnimationView::OnRadioTimerCustom() 
{
//	UpdateData();
	UseDefaultTimer(FALSE);
	UpdateControlsState();
}

void CAnimationView::SelendokComboPalettes(int nOldPalette) 
{
	if (!ChangePalette(m_pPanelView->m_nPalette))
	{
		m_pPanelView->m_nPalette = nOldPalette;
		m_pPanelView->UpdateData(FALSE);
		AfxMessageBox(IDS_CANNOT_GET_PAL);
	}
	else
	{
		m_pPanelView->m_wndPalettes.SetItemColor(m_pPanelView->m_nPalette, RGB(
			m_rgbPal.pal[CIV_COLOR].rgbRed,
			m_rgbPal.pal[CIV_COLOR].rgbGreen,
			m_rgbPal.pal[CIV_COLOR].rgbBlue));
	}

	m_bPlayStatic = FALSE;
}

void CAnimationView::OnColorTable() 
{
	CColorTableDlg dlg(&m_rgbPal);
	if (dlg.DoModal() != IDOK)
		return;

	if (!ApplyPaletteChanges())
	{
		OnInitialUpdate();
		AfxMessageBox(uLoadString(IDS_CANNOT_CHANGE_PALETTE), MB_ICONERROR);
		return;
	}

	if (m_bActive)
	{
		m_pPanelView->m_nPalette = 0;
		m_pPanelView->UpdateData(FALSE);
		
		m_pPanelView->m_wndPalettes.SetItemColor(m_pPanelView->m_nPalette, RGB(
			m_rgbPal.pal[CIV_COLOR].rgbRed,
			m_rgbPal.pal[CIV_COLOR].rgbGreen, 
			m_rgbPal.pal[CIV_COLOR].rgbBlue));
	}

	m_bPlayStatic = FALSE;
}

void CAnimationView::OnUpdateFlcColorTable(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelExtra->m_btnColorTable.IsWindowEnabled());
}

void CAnimationView::Rewind()
{
	SetTimer(TIMER_REWIND_ID, TIMER_REWIND_DELAY, NULL);
}

void CAnimationView::StopRewind()
{
	KillTimer(TIMER_REWIND_ID);
	
	if (m_bActive)
	{
		m_pPanelAnimation->m_bBack		= FALSE;
		m_pPanelAnimation->m_bForward	= FALSE;
	}
}

void CAnimationView::OnBack() 
{
	if (!m_bActive)
		return;

	m_pPanelAnimation->m_bBack = !m_pPanelAnimation->m_bBack;
	m_pPanelAnimation->m_bForward = FALSE;

	if (m_pPanelAnimation->m_bBack)
		Rewind();
	else
		StopRewind();
}

void CAnimationView::OnForward() 
{
	if (!m_bActive)
		return;

	m_pPanelAnimation->m_bForward = !m_pPanelAnimation->m_bForward;
	m_pPanelAnimation->m_bBack = FALSE;

	if (m_pPanelAnimation->m_bForward)
		Rewind();
	else
		StopRewind();
}

void CAnimationView::OnUpdateFlcBack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnBack.IsWindowEnabled());
	pCmdUI->SetCheck(m_pPanelAnimation->m_bBack);
}

void CAnimationView::OnUpdateFlcForward(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pPanelAnimation->m_btnForward.IsWindowEnabled());
	pCmdUI->SetCheck(m_pPanelAnimation->m_bForward);
}

void CAnimationView::OnImportPalette() 
{
	if (!IsLoaded())
		return;

	if (!ImportPalette(m_rgbPal) ||
		!ApplyPaletteChanges())
	{
		OnInitialUpdate();
		return;
	}

	if (m_bActive)
	{
		m_pPanelView->m_nPalette = 0;
		m_pPanelView->UpdateData(FALSE);
		
		m_pPanelView->m_wndPalettes.SetItemColor(m_pPanelView->m_nPalette, RGB(
			m_rgbPal.pal[CIV_COLOR].rgbRed,
			m_rgbPal.pal[CIV_COLOR].rgbGreen, 
			m_rgbPal.pal[CIV_COLOR].rgbBlue));
	}

	AfxMessageBox(IDS_IMP_PAL_OK, MB_ICONINFORMATION);
}

void CAnimationView::OnExportPalette() 
{
	if (!IsLoaded())
		return;
	
	ExportPalette(m_rgbPal);
}

void CAnimationView::OnRefresh() 
{
	Refresh();
}

void CAnimationView::OnUseAlphaBlending() 
{
	if (!UseAlphaBlending(!IsUseAlphaBlending()))
		OnInitialUpdate();

	m_pPanelView->m_btnAlphaBlend.SetCheck(IsUseAlphaBlending());
	m_bPlayStatic = FALSE;
}

void CAnimationView::OnUpdateUseAlphaBlending(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(IsUseAlphaBlending());
	pCmdUI->Enable(IsLoaded() && !IsLeaderHead());
}

void CAnimationView::OnCopyToClipboard() 
{
	CFlcImage img;
	img.CopyToClipboard();
}

void CAnimationView::OnCopyToFile() 
{
	CFlcImage img;
	img.CopyToFile(this);
}

////////////////////////////////////////////////////////////////////

BOOL CAnimationView::Init()
{
	if (m_pDocument->GetPathName().IsEmpty())
		return FALSE;

	SetWindowPos(NULL, 0, 0, WND_CX, WND_CY,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	// Create new Screen
	m_pScreen = NULL;
	m_pScreen = new CScreen;

	CWinApp *pApp = AfxGetApp();
	m_bgdCustomColor	= pApp->GetProfileInt(REG_OPTIONS, REG_BGD_COLOR, ::GetSysColor(COLOR_BTNFACE));
	m_eDirectionDef		= (EDirection)pApp->GetProfileInt(REG_OPTIONS, REG_DIRECTION, DirAll);
	m_eBackground		= (EBdg)pApp->GetProfileInt(REG_OPTIONS, REG_BGD_TYPE, BgdDefault);
	m_bUseAlphaBlending = pApp->GetProfileInt(REG_OPTIONS, REG_ALPHA_BLEND, 1);
	m_bShowStaticFrm	= pApp->GetProfileInt(REG_OPTIONS, REG_ANIM_TYPE, 0);
	m_bIgnoreOffset		= pApp->GetProfileInt(REG_OPTIONS, REG_IGNORE_OFFSET, 0);
	
	// 'Attach' panels
	m_pPanelAnimation	= m_pDlgBar->m_pPanelAnimation;
	m_pPanelDelay		= m_pDlgBar->m_pPanelDelay;
	m_pPanelDirection	= m_pDlgBar->m_pPanelDirection;
	m_pPanelExtra		= m_pDlgBar->m_pPanelExtra;
	m_pPanelSound		= m_pDlgBar->m_pPanelSound;
	m_pPanelView		= m_pDlgBar->m_pPanelView;

	m_pPanelView->m_nPalette = 0; // select "default" color palette
	
	m_bShowStaticFrm = MAIN_APP->IsShowStaticFrm();

	if (!LoadAnimationFile(m_pDocument->GetPathName()))
		return FALSE;

	m_pPanelAnimation->m_wndFrameSlider.ClearSel();
	m_pPanelAnimation->m_wndFrameSlider.SetRangeMin(0);
	m_pPanelAnimation->UpdateData(FALSE);

	UpdateFrameSlider();
	m_pPanelView->m_wndPalettes.SetItemColor(0, RGB(
		m_rgbPal.pal[CIV_COLOR].rgbRed,
		m_rgbPal.pal[CIV_COLOR].rgbGreen, 
		m_rgbPal.pal[CIV_COLOR].rgbBlue));
	m_pPanelView->UpdateData(FALSE);
/*	
	m_pPanelDelay->m_wndTimerSlider.SetPos(m_uElapse);
	m_pPanelDelay->m_sDelay = uInt2Str(m_uElapse);
	m_pPanelDelay->m_sDefTimer = uFormatString(IDS_DEF_TIMER, m_uElapse);
	m_pPanelDelay->UpdateData(FALSE);
*/
	UpdateInfo();
	UpdateControlsState();
	return TRUE;
}

void CAnimationView::Shutdown()
{
	KillAnimationTimer();
	Stop();
	ClearSFrames();
	
	m_bActive		= FALSE;
	m_bLoaded		= FALSE;
	m_uElapse		= 0;
	m_animLenth		= 0;
	m_nCurPalette	= 0;
}

//////////////////////////////////////////////////////////////////////////
// Load FLC by the given path.
// In case bColorTable == FALSE color chunk will be skipped.
//
BOOL CAnimationView::LoadAnimation(LPCTSTR lpszPathName, BOOL bColorTable /*=TRUE*/)
{
	ClearSFrames();

	if (lpszPathName[0] == NULL)
		return FALSE;

	if (m_pScreen->Init(m_hWnd) != S_OK)
	{
		SAFE_DELETE(m_pScreen);
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Clear frames' surfaces and buffers.
//
void CAnimationView::ClearSFrames()
{
	for (int i = 0; i < m_arSFrames.GetSize(); i++)
		SAFE_DELETE(m_arSFrames[i]);

	m_arSFrames.RemoveAll();

	SAFE_DELETE_MULTI(m_alphaShadow);
	SAFE_DELETE_MULTI(m_alphaSmoke);

	if (m_pScreen && m_pScreen->IsWorking())
		m_pScreen->Shutdown();
}

BOOL CAnimationView::PlayAnimation()
{
	if (!IsLoaded())
		return FALSE;

	// show the same frame
	if (m_bPlayStatic)
	{
		m_pScreen->Flip(); // to don't redraw the whole frame
		return TRUE;
	}
	
	if (IsPlaying()) // if application has the focus, process
	{
		if (!IsPaused())
		{
			m_dwEndFrame	= GetTickCount();
			m_frameTime		= float(double(m_dwEndFrame - m_dwStartFrame) / double(1000));
			m_dwStartFrame	= GetTickCount();
			
			if ((m_frameTime * 60.0f) > 3.0f)  
				m_frameTime = ((1.0f / 60.0f) * 3.0f);
		}
		else
		{
			m_frameTime = 0;
		}
		
		UpdateFrame(m_frameTime * 1000.0f);

		if (m_bgdSurface)	// Draw background image
			m_bgdSurface->Draw(0, 0);
		else				// Fill background
			m_pScreen->Fill(m_bgdColor);

		DrawFrame();
		
		m_pScreen->Flip();
	}
	else // draw static picture
	{
		if (m_bgdSurface)	// Draw background image
			m_bgdSurface->Draw(0, 0);
		else				// Fill background
			m_pScreen->Fill(m_bgdColor);
		
		DrawFrame();
		
		m_pScreen->Flip();
	}

	return TRUE;
}

void CAnimationView::Start()
{
	m_bPlaying		= TRUE;
	m_bPaused		= FALSE;

	m_dwStartFrame	= GetTickCount();
	m_dwEndFrame	= 0;

	Reset();

	if (m_uElapse == 0)
		m_uElapse = (UINT)m_uSpeed;

	StartAnimationTimer();
}

void CAnimationView::Stop()
{
	if (!IsLoaded())
		return;

	Reset();

	m_bPlaying		= FALSE;
	m_bPaused		= FALSE;

	m_dwStartFrame	= 0;
	m_dwEndFrame	= 0;

	StopSoundFile();
}

void CAnimationView::SetBackgroundColor(COLORREF color)
{
	SAFE_DELETE(m_bgdSurface);
	m_bgdColor		 = RGB(GetBValue(color), GetGValue(color), GetRValue(color));
	m_bgdCustomColor = color;
	m_eBackground	 = BgdCustom;
	m_bPlayStatic	 = FALSE;

	CWinApp *pApp = AfxGetApp();
	pApp->WriteProfileInt(REG_OPTIONS, REG_BGD_TYPE, m_eBackground);
	pApp->WriteProfileInt(REG_OPTIONS, REG_BGD_COLOR, m_bgdCustomColor);
}

void CAnimationView::ResetBackgroundColor()
{
	SAFE_DELETE(m_bgdSurface);
	m_bgdColor		= RGB(GetBValue(m_bgdDefaultColor), GetGValue(m_bgdDefaultColor), GetRValue(m_bgdDefaultColor));//::GetSysColor(COLOR_BTNFACE);
	m_eBackground	= BgdDefault;
	m_bPlayStatic	= FALSE;

	AfxGetApp()->WriteProfileInt(REG_OPTIONS, REG_BGD_TYPE, m_eBackground);
}

BOOL CAnimationView::SetBackgroundImage(LPCTSTR lpszImgPath)
{
	SAFE_DELETE(m_bgdSurface);

	if (!uIsFileExist(lpszImgPath))
		return FALSE;

	CBgdImg bgdImg;
	if (!bgdImg.LoadBgdImgFile(lpszImgPath))
		return FALSE;
	
	m_bgdSurface = new CSurface(m_pScreen);
	m_bgdSurface->Create(WND_W, WND_H);

	if (bgdImg.CopyToSurface(m_bgdSurface) != S_OK)
	{
		SAFE_DELETE(m_bgdSurface);
		return FALSE;
	}
	
	return TRUE;
}

void CAnimationView::UpdateFrame(float t)
{
	if (m_bDefaultTimer)
	{
		m_speed -= t;
		
		if (m_speed <= 0.0f)
		{
			m_curFrame++;
			
			UpdateInfo();

			if (m_curFrame >= m_lastFrame)
				m_curFrame = m_firstFrame;
			
			m_speed = (float)m_uSpeed;
		}
	}
	else
	{
		m_curFrame++;
		
		UpdateInfo();

		if (m_curFrame >= m_lastFrame)
			m_curFrame = m_firstFrame;
	}
}

/*
The last 32 colors (last two rows) are also special - these are the 
Alpha Blending palette entries.  
Note that I called them palette entries, not colors.  
This is because the colors appearing in these positions are just place 
holders.  
Each palette position in this range specifies a certain level of 
Alpha Belending.  
The last position, which is normally magenta, is used to indicate 
full transparency.  
The other 15 colors in the last row form a transparency ramp, 
down to fully opaque black.  
These varying levels of transparency are used to create shadow effects.  
The 16 positions in the next-to-last line seem to form a ramp 
from transparent to fully opaque white, and are used to form smoke 
and haze effects.

shadow effects
16 - 240 - 255
0 - 100%
16 - transparent
  
smoke
16 - 224 - 239
0 - white RGB(255, 255, 255)
16 - transparent

  Values from Terrain.pcx (Mike Breitkreutz advice)
  254
  236
  218
  200
  182
  163
  145
  127
  109
  91
  72
  54
  36
  18
  0
*/
//static const BYTE arSmoke[16] = {0, 18, 36, 54, 72, 91, 109, 127, 145, 163, 182, 200, 218, 236, 254, 255};
//static const BYTE arSmoke[16] = {255,254,236,218,200,182,163,145,127,109,91,72,54,36,18,0};
void CAnimationView::DrawFrame()
{
	if (!m_bPlayStatic &&
		m_curFrame == m_prevFrame)
	{
		if (m_bPlaying)
		{
			if (m_bPaused)
				m_bPlayStatic = TRUE;
		}
		else
			m_bPlayStatic = TRUE;
	}

	if (m_curFrame >= m_nTotalFrames)
		m_curFrame = m_nTotalFrames-1;

	m_prevFrame = m_curFrame;

	// Leaderheads don't needed alpha blending!
	if (m_bLeaderHead || !m_bUseAlphaBlending)
	{
		m_pScreen->GetBack()->GetDDS()->BltFast(m_xPos, m_yPos, m_arSFrames[m_curFrame]->GetDDS(), NULL, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		return;
	}
	
	BYTE xFind, yFind;
	WORD pos = 0;

	DDSURFACEDESC	ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	if (m_pScreen->GetBack()->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
		return;

	BYTE* db		= (BYTE*)ddsd.lpSurface;
	BYTE* pixelData = m_arSFrames[m_curFrame]->flags.pixelData;
	BYTE* line		= NULL;
	BYTE curColor;
	
	db += (m_yPos * ddsd.lPitch);
	db += (m_xPos * 4);

	for (yFind = 0; yFind < m_nFrmHeight; yFind++)
	{
		line = db;

		for (xFind = 0; xFind < m_nFrmWidth; xFind++)
		{
			curColor = pixelData[pos];

			// shadow effects
			if (curColor > 239 && curColor < 255)
			{
				m_alphaShadow[pos].rgbBlue		= line[0];
				m_alphaShadow[pos].rgbGreen		= line[1];
				m_alphaShadow[pos].rgbRed		= line[2];
				m_alphaShadow[pos].rgbReserved	= 255 - curColor;
			}
			else
			{
				m_alphaShadow[pos].rgbBlue		= 0;
				m_alphaShadow[pos].rgbGreen		= 0;
				m_alphaShadow[pos].rgbRed		= 0;
//				m_alphaShadow[pos].rgbReserved	= 0; // isn't used
			}

			// smoke
			if (curColor > 223 && curColor < 240)
			{
				m_alphaSmoke[pos].rgbBlue		= line[0];
				m_alphaSmoke[pos].rgbGreen		= line[1];
				m_alphaSmoke[pos].rgbRed		= line[2];
				m_alphaSmoke[pos].rgbReserved	= curColor - 224;
			}
			else
			{
				m_alphaSmoke[pos].rgbBlue		= 0;
				m_alphaSmoke[pos].rgbGreen		= 0;
				m_alphaSmoke[pos].rgbRed		= 0;
//				m_alphaSmoke[pos].rgbReserved	= 0; // isn't used
			}

			line += 4;
			pos++;
		}
		db += (ddsd.lPitch);
	}

	m_pScreen->GetBack()->GetDDS()->Unlock(NULL);
	m_pScreen->GetBack()->GetDDS()->BltFast(m_xPos, m_yPos, m_arSFrames[m_curFrame]->GetDDS(), NULL, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (FAILED(m_pScreen->GetBack()->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))
		return;

	db = (BYTE*)ddsd.lpSurface;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   

	db += (m_yPos * ddsd.lPitch);
	db += (m_xPos * 4);

	pos		= 0;
	line	= NULL;
	int b	= 0;
	int g	= 0;
	int r	= 0;

	for (yFind = 0; yFind < m_nFrmHeight; yFind++)
	{
		line = db;
		for (xFind = 0; xFind < m_nFrmWidth; xFind++)
		{
			// shadow
			if (m_alphaShadow[pos].rgbBlue	> 0 ||
				m_alphaShadow[pos].rgbGreen	> 0 ||
				m_alphaShadow[pos].rgbRed	> 0)
			{
				b = m_alphaShadow[pos].rgbBlue - (m_alphaShadow[pos].rgbReserved * 12);
				if (b < 0)
					b = 0;
				
				g = m_alphaShadow[pos].rgbGreen - (m_alphaShadow[pos].rgbReserved * 12);
				if (g < 0)
					g = 0;
				
				r = m_alphaShadow[pos].rgbRed - (m_alphaShadow[pos].rgbReserved * 12);
				if (r < 0)
					r = 0;

				line[0] = b;
				line[1] = g;
				line[2] = r;
//				line[3] = 50;
			}

			// smoke
			if (m_alphaSmoke[pos].rgbBlue	> 0 ||
				m_alphaSmoke[pos].rgbGreen	> 0 ||
				m_alphaSmoke[pos].rgbRed	> 0)
			{
/*
				// a new variant
				b = (arSmoke[m_alphaSmoke[pos].rgbReserved] + m_alphaSmoke[pos].rgbBlue) / 2;
				if (b > 255)
					b = 255;
				
				g = (arSmoke[m_alphaSmoke[pos].rgbReserved] + m_alphaSmoke[pos].rgbGreen) / 2;
				if (g > 255)
					g = 255;
				
				r = (arSmoke[m_alphaSmoke[pos].rgbReserved] + m_alphaSmoke[pos].rgbRed) / 2;
				if (r > 255)
					r = 255;
*/
/**/
				// an old variant
				b = m_alphaSmoke[pos].rgbBlue + (m_alphaSmoke[pos].rgbReserved * 4);
				if (b > 255)
					b = 255;

				g = m_alphaSmoke[pos].rgbGreen + (m_alphaSmoke[pos].rgbReserved * 4);
				if (g > 255)
					g = 255;

				r = m_alphaSmoke[pos].rgbRed + (m_alphaSmoke[pos].rgbReserved * 4);
				if (r > 255)
					r = 255;

				line[0] = b;
				line[1] = g;
				line[2] = r;
//				line[3] = 50;
			}

			line += 4;
			pos++;
		}
		
		db += (ddsd.lPitch);
	}

	m_pScreen->GetBack()->GetDDS()->Unlock(NULL);
}

BOOL CAnimationView::LoadAnimationFile(LPCTSTR lpszPathName)
{
	if (lpszPathName[0] == NULL)
		return FALSE;

	KillAnimationTimer();
	Stop();

	CWaitCursor wait;

	if (!LoadAnimation(lpszPathName))
	{
		AfxMessageBox(uFormatString(IDS_ERR_INVALID_FLC, lpszPathName));
		ClearSFrames();
		return FALSE;
	}
	
	int nSize = sizeof(RGBQUAD) * m_nPixelsSize;
	m_alphaShadow = new RGBQUAD[nSize];
	ZeroMemory(m_alphaShadow, nSize);

	m_alphaSmoke = new RGBQUAD[nSize];
	ZeroMemory(m_alphaSmoke, nSize);
	
	SetDirection(m_eDirectionDef);
	Reset();
	
	m_bLoaded = TRUE;
	Start();

	if (m_bShowStaticFrm)
		m_bPaused = TRUE;
	else if (m_pPanelSound->m_bNonStopWAV)
		PlaySoundFile();

	return TRUE;
}

void CAnimationView::Reset()
{
	if (m_eDirection == DirAll)
	{
		m_firstFrame = 0;
		m_lastFrame  = m_animLenth * m_nDirCount;
		m_curFrame	 = m_firstFrame;
		m_prevFrame	 = STATIC_MAX;
	}
	else
	{
		m_firstFrame = (m_animLenth * (m_eDirection - 1));
		m_lastFrame	 = m_firstFrame + m_animLenth - 1;
		m_curFrame	 = m_firstFrame;
		m_prevFrame	 = STATIC_MAX;
	}

	m_speed = (float)m_uSpeed;
}

void CAnimationView::SetFramePos(EFramePos pos)
{
	if (!IsLoaded())
		return;

	switch (pos)
	{
	case FrmFirst:
		m_curFrame = m_firstFrame;
		break;

	case FrmPrevious:
		m_curFrame--;
		if (m_curFrame < m_firstFrame)
			m_curFrame = m_firstFrame;
		break;

	case FrmNext:
		m_curFrame++;
		if (m_curFrame > m_lastFrame)
			m_curFrame = m_lastFrame;
		break;

	case FrmLast:
		m_curFrame = m_lastFrame;
		break;

	default:
		ASSERT(FALSE);
	}
	
	m_prevFrame		= STATIC_MAX;
	m_bPlayStatic	= FALSE;

	UpdateInfo();
	if (!IsPlaying())
		UpdateFrameSlider();
}

EDirection CAnimationView::SetDirection(EDirection direction)
{
	if (!IsLoaded())
		return m_eDirection;

	if (m_nDirCount == DIRECTIONS_MAX)
		m_eDirection = direction;
	else
		m_eDirection = DirAll; // allow only all directions
	
	Reset();
	UpdateInfo();
	if (!IsPlaying())
		UpdateFrameSlider();

	UpdateControlsState();
	return m_eDirection;
}

void CAnimationView::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TIMER_PLAYER_ID:
		{
			if (!PlayAnimation())
				Stop();
			break;
		}
		
	case TIMER_REWIND_ID:
		{
			SetFramePos(m_pPanelAnimation->m_bBack ? FrmPrevious : FrmNext);
			
			if ((m_curFrame <= m_firstFrame) ||
				(m_curFrame >= m_lastFrame))
				StopRewind();
			break;
		}
	}

	CView::OnTimer(nIDEvent);
}

void CAnimationView::StartAnimationTimer()
{
	KillAnimationTimer();
	SetTimer(TIMER_PLAYER_ID, m_bDefaultTimer ? TIMER_PLAYER_DELAY : m_uElapse, NULL);
}

void CAnimationView::KillAnimationTimer()
{
	KillTimer(TIMER_PLAYER_ID);
}

void CAnimationView::OnDestroy() 
{
	StopRewind();
	StopSoundFile();

	CView::OnDestroy();

	Shutdown();
	
	SAFE_DELETE(m_bgdSurface);
	SAFE_DELETE(m_pScreen);

	CWinApp *pApp = AfxGetApp();
//	pApp->WriteProfileInt(REG_OPTIONS, REG_BGD_TYPE, m_eBackground);
//	pApp->WriteProfileInt(REG_OPTIONS, REG_BGD_COLOR, m_bgdCustomColor);
	pApp->WriteProfileInt(REG_OPTIONS, REG_DIRECTION, m_eDirection);
}

BOOL CAnimationView::SetBgdImg(EBdg typeBgd)
{
	UINT nId;
	CString sFileName, sImgPath;

	BOOL bRes = TRUE;
	BOOL bCustomImg = FALSE;

	switch (typeBgd)
	{
	case BgdLand:	nId = IDR_BGD_LAND;	sFileName = "Land";	break;
	case BgdSea:	nId = IDR_BGD_SEA;	sFileName = "Sea";	break;
	case BgdGrid:	nId = IDR_BGD_GRID;	sFileName = "Grid";	break;
	case BgdImg:	bCustomImg = TRUE;						break;
	default:
		return FALSE;
	}
	
	// try to get image from resources
	if (!bCustomImg)
	{
		HMODULE hInst = AfxGetApp()->m_hInstance;
		
		if (hInst == 0)
			hInst = AfxGetInstanceHandle();
		
		// Get current directory
		char buf[MAX_PATH];
		if (GetModuleFileName(hInst, buf, MAX_PATH) == 0)
			return FALSE;
		
		sImgPath = uGetPathName(buf) + sFileName + ".bmp";
		
		// In case file isn't exist - try to create it from resources
		if (!uIsFileExist(sImgPath))
		{
			DWORD dwSize		= 0;
			BOOL bCompressed	= FALSE;
			LPBYTE lpBuf		= NULL;
			
			// Create file from resources
			HRSRC resInfo = FindResource(hInst, MAKEINTRESOURCE(nId), "BGD"); // find the resource
			
			if (resInfo == NULL)                           // if can't find the resourse
				return FALSE;
			
			HGLOBAL hRes = LoadResource(hInst, resInfo);   // load the resource
			
			if (hRes == NULL) // if can't load the resourse
				return FALSE;
			
			// Retrive resource data
			dwSize = SizeofResource(hInst, resInfo); // get the resource size
			UINT FAR* lpnRes = (UINT FAR*)LockResource(hRes);
			lpBuf = new BYTE[dwSize + 1];
			memcpy(lpBuf, lpnRes, dwSize);
			lpBuf[dwSize] = 0;
			
			UnlockResource(hRes);                          
			FreeResource(hRes); // free the resource
								/*
								CString sImgPath;
								::GetTempPath(MAX_PATH, sImgPath.GetBuffer(MAX_PATH));
								sImgPath.ReleaseBuffer();
								sImgPath += sFileName + ".pcx";
			*/	
			// Create a file
			CFile fl;
			try
			{
				fl.Open(sImgPath, CFile::modeCreate | CFile::modeWrite | CFile::modeRead);
				
				// Set data to this file
				fl.Write(lpBuf, dwSize);
				fl.Close();
			}
			catch (...)
			{
				bRes = FALSE;
				AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CAnimationView::SetBgdImg"), MB_ICONERROR);
			}
			
			SAFE_DELETE_MULTI(lpBuf);
		}
	}//if (!bCustomImg)
	else
	{
		sImgPath = AfxGetApp()->GetProfileString(REG_OPTIONS, REG_BGD_IMG, "");
		if (!uIsFileExist(sImgPath))
			bRes = FALSE;
	}

	if (bRes)
		bRes = SetBackgroundImage(sImgPath);

	if (bRes)
	{
		m_eBackground = typeBgd;

//		switch (typeBgd)
//		{
//		case BgdLand:
//			m_eBackground = BgdLand;
//			break;
//		case BgdSea:
//			m_eBackground = BgdSea;
//			break;
//		case BgdGrid:
//			m_eBackground = BgdGrid;
//			break;
//		case BgdImg:
//			m_eBackground = BgdImg;
//		}

		AfxGetApp()->WriteProfileInt(REG_OPTIONS, REG_BGD_TYPE, m_eBackground);
	}
	
	m_bPlayStatic = FALSE;
	return bRes;
}

void CAnimationView::UpdateBackground()
{
	switch (m_eBackground)
	{
	case BgdCustom:
		SetBackgroundColor(m_bgdCustomColor);
		break;
		
	case BgdLand:
	case BgdSea:
	case BgdGrid:
	case BgdImg:
		SetBgdImg(m_eBackground);
		break;

	default:
		ResetBackgroundColor();
	}
}

void CAnimationView::UseDefaultTimer(BOOL bUseDefault)
{
	m_bDefaultTimer = bUseDefault;
	StartAnimationTimer();
}

void CAnimationView::SetTimerValue(UINT uElapse)
{
	m_uElapse = uElapse;
	StartAnimationTimer();
}

BOOL CAnimationView::IsDefaultTimer()
{
	return m_bDefaultTimer;
}

BOOL CAnimationView::SetCivColorPalette(LPCTSTR lpszImgPath /*=NULL*/)
{
	BYTE nColor = 0;
	if (lpszImgPath != NULL)
	{
		if (!uIsFileExist(lpszImgPath))
			return FALSE;

		CPcx pcxImg;
		
		// Load related file with specified civilization color palette
		if (!pcxImg.LoadBgdImgFile(lpszImgPath))
			return FALSE;
		
		// Copy default palette to the current palette
		//ZeroMemory(&m_rgbPal, RGBQUADPAL_SIZE);
		memcpy(&m_rgbPal, &m_rgbPalDef, RGBQUADPAL_SIZE);
		
		// Get first 64 colors (civilization related) from PCX and set them to the FLC color palette
		memcpy(m_rgbPal.pal, pcxImg.m_rgbPal.pal, sizeof(RGBQUAD) * 64);

		CString sColor(lpszImgPath);
		sColor = sColor.Mid(sColor.GetLength() - 6, 2);
		nColor = atoi(sColor) + 1;
	}
	else
	{
		// Copy default palette to the current palette
		//ZeroMemory(&m_rgbPal, RGBQUADPAL_SIZE);
		memcpy(&m_rgbPal, &m_rgbPalDef, RGBQUADPAL_SIZE);
	}

	if (!ReloadWithPalette())
	{
		Shutdown();
		return FALSE;
	}
	
	m_bPlayStatic = FALSE;
	m_bLoaded	  = TRUE;
	StartAnimationTimer();
	m_nCurPalette = nColor;

	return TRUE;
}

BOOL CAnimationView::IsLeaderHead()
{
	return m_bLeaderHead;
}

BOOL CAnimationView::ReloadWithPalette()
{
	if (!IsLoaded())
		return FALSE;
	
	KillAnimationTimer();
	m_bLoaded = FALSE;
	
	try
	{
		m_colTransparent = RGB(m_rgbPal.pal[255].rgbBlue, m_rgbPal.pal[255].rgbGreen, m_rgbPal.pal[255].rgbRed);
		
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
		
		DDSURFACEDESC	ddsd;
		BYTE* pixelData			= NULL;
		UINT pos				= 0;
		BYTE*			db		= NULL;
		BYTE*			line	= NULL;
		RGBQUAD*		pRgb	= NULL;
		BYTE x, y;
		BYTE idx = 0;
		
		for (int nCur = 0; nCur < m_arSFrames.GetSize(); nCur++)
		{
			pos = 0;
			pixelData = m_arSFrames[nCur]->flags.pixelData;
			
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			
			if (m_arSFrames[nCur]->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
			{
				return FALSE;
			}
			
			db		= (BYTE*)ddsd.lpSurface;
			line	= NULL;
			
			// set transparent color key
			
			m_arSFrames[nCur]->GetDDS()->SetColorKey(DDCKEY_SRCBLT, &m_ddcTransparent);
			
			// Get current frame's data
			for (y = 0; y < m_nFrmHeight; y++)
			{
				line = db;
				
				for (x = 0; x < m_nFrmWidth; x++)
				{
					idx = pixelData[pos];
					pRgb = &m_rgbPal.pal[idx];
					
					line[0] = pRgb->rgbBlue;
					line[1] = pRgb->rgbGreen;
					line[2] = pRgb->rgbRed;
					line[3] = 255;
					
					line += 4;
					pos++;
				}//for (x = 0; x < m_nFrmWidth; x++)
				db += (ddsd.lPitch);
			}//for (y = 0; y < m_nFrmHeight; y++)
			
			m_arSFrames[nCur]->GetDDS()->Unlock(NULL);
		}//for (i = 0; i < nDirCount && nCur < m_pFlc->m_arSFrames.GetSize(); i++)
	}
	catch (...)
	{
		AfxMessageBox(uFormatString(IDS_ERR_INVALID_FLC, m_sPathName), MB_ICONERROR);
		ClearSFrames();
		return FALSE;
	}
	
	StartAnimationTimer();
	m_bLoaded = TRUE;
	return TRUE;
}

COLORREF CAnimationView::GetDefaultBgdColor()
{
	return m_bgdDefaultColor;
}

BOOL CAnimationView::ApplyPaletteChanges()
{
	// Copy current palette to the default palette
	//ZeroMemory(&m_rgbPalDef, sizeof(m_rgbPalDef));
	memcpy(&m_rgbPalDef, &m_rgbPal, sizeof(m_rgbPal));

	m_colTransparent = RGB(m_rgbPalDef.pal[255].rgbBlue, m_rgbPalDef.pal[255].rgbGreen, m_rgbPalDef.pal[255].rgbRed);
	
	// Transparent color can be changed...
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
		
	if (!ReloadWithPalette())
	{
		Shutdown();
		return FALSE;
	}

	m_bLoaded = TRUE;
	StartAnimationTimer();

	return TRUE;
}

BOOL CAnimationView::UseAlphaBlending(BOOL bUse)
{
	if (m_bUseAlphaBlending == bUse)
		return TRUE;

	m_bUseAlphaBlending = bUse;

	if (m_bUseAlphaBlending)
	{
		m_ddcTransparent.dwColorSpaceLowValue  = m_colTransparent;
		m_ddcTransparent.dwColorSpaceHighValue = m_colTransparent;
	}
	else
	{
		m_ddcTransparent.dwColorSpaceLowValue  = RGB_ALPHA_ERROR;
		m_ddcTransparent.dwColorSpaceHighValue = RGB_ALPHA_ERROR;

		ResetBackgroundColor();
		// in case leader heads we should not use background image
		// and background color must be black
//		SAFE_DELETE(m_bgdSurface);
//		m_bgdColor	= m_bgdDefaultColor; // background must be default
	}

	DDSURFACEDESC	ddsd;
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	for (int i = 0; i < m_arSFrames.GetSize(); i++)
	{
		if (m_arSFrames[i]->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
			return FALSE;
		
		// set transparent color key
		m_arSFrames[i]->GetDDS()->SetColorKey(DDCKEY_SRCBLT, &m_ddcTransparent);
		
		m_arSFrames[i]->GetDDS()->Unlock(NULL);
	}
	
	AfxGetApp()->WriteProfileInt(REG_OPTIONS, REG_ALPHA_BLEND, m_bUseAlphaBlending);
	return TRUE;
}

BOOL CAnimationView::ChangePalette(BYTE nColor, RGBQUADPAL *pPal /*=NULL*/)
{
	ASSERT(nColor < 33);

	// Copy default palette to the current palette
	if (nColor == 0)
	{
		if (pPal)
			return GetCivColorPalette(pPal);

		return SetCivColorPalette();
	}

	nColor--;

	CString sImgPath, sFileName;
	sFileName.Format("ntp%d.pcx", nColor);
	if (nColor < 10)
		sFileName.Insert(3, "0");

	switch (MAIN_APP->GetPaletteType())
	{
	case PalGame:
		{
			if (!MAIN_APP->IsCivRegistered())
			{
				AfxMessageBox(IDS_ERR_PAL_GAME);
				return FALSE;
			}

			sImgPath = MAIN_APP->GetCivInstallPath();
			sImgPath += "\\Art\\Units\\Palettes\\" + sFileName;
			break;
		}

	case PalRes:
		{
			UINT nId = IDR_PCX_00 + nColor;
			
			HMODULE hInst = AfxGetApp()->m_hInstance;
			
			if (hInst == 0)
				hInst = AfxGetInstanceHandle();
			
			// Get current directory
			char buf[MAX_PATH];
			if (GetModuleFileName(hInst, buf, MAX_PATH) == 0)
			{
				AfxMessageBox(IDS_ERR_PAL_RES);
				return FALSE;
			}

			sImgPath = uGetPathName(buf) + "Palettes\\";
			sFileName.Insert(0, sImgPath);
			
			// In case the file isn't exist - make it from resources
			if (!uIsFileExist(sFileName))
			{
				// In case directory isn't exist - try to create it
				if (!uIsDirExist(sImgPath) && !uCreateNewFolder(sImgPath))
				{
					AfxMessageBox(IDS_ERR_PAL_RES);
					return FALSE;
				}

				BOOL bRes			= TRUE;
				DWORD dwSize		= 0;
				BOOL bCompressed	= FALSE;
				LPBYTE lpBuf		= NULL;
				
				// Create file from resources
				HRSRC resInfo = FindResource(hInst, MAKEINTRESOURCE(nId), "PCX"); // find the resource
				
				if (resInfo == NULL)                           // if can't find the resourse
				{
					AfxMessageBox(IDS_ERR_PAL_RES);
					return FALSE;
				}
				
				HGLOBAL hRes = LoadResource(hInst, resInfo);   // load the resource
				
				if (hRes == NULL) // if can't load the resourse
				{
					AfxMessageBox(IDS_ERR_PAL_RES);
					return FALSE;
				}
				
				// Retrive resource data
				dwSize = SizeofResource(hInst, resInfo); // get the resource size
				UINT FAR* lpnRes = (UINT FAR*)LockResource(hRes);
				lpBuf = new BYTE[dwSize + 1];
				memcpy(lpBuf, lpnRes, dwSize);
				lpBuf[dwSize] = 0;
				
				UnlockResource(hRes);                          
				FreeResource(hRes); // free the resource
				
				// Create a file
				CFile fl;
				try
				{
					fl.Open(sFileName, CFile::modeCreate | CFile::modeWrite | CFile::modeRead);
					
					// Set data to the file
					fl.Write(lpBuf, dwSize);
					fl.Close();
				}
				catch (...)
				{
					bRes = FALSE;
					AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CAnimationView::PaletteFromRes"), MB_ICONERROR);
				}
				
				SAFE_DELETE_MULTI(lpBuf);
				
				if (!bRes)
				{
					AfxMessageBox(IDS_ERR_PAL_RES);
					return FALSE;
				}
			}
			
			sImgPath = sFileName;
			break;
		}

	case PalDir:
		{
			sImgPath = MAIN_APP->GetPaletteDirectory();

			if (!uIsDirExist(sImgPath))
			{
				AfxMessageBox(IDS_ERR_PAL_DIR);
				return FALSE;
			}
			
			sImgPath += "\\" + sFileName;
			if (!uIsFileExist(sImgPath))
			{
				AfxMessageBox(IDS_ERR_PAL_DIR);
				return FALSE;
			}
			break;
		}
	}

	if (!uIsFileExist(sImgPath))
	{
		AfxMessageBox(uFormatString(IDS_ERR_PALETTE_FILE, sImgPath), MB_ICONERROR);
		return FALSE;
	}

	if (pPal)
		return GetCivColorPalette(pPal, sImgPath);

	return SetCivColorPalette(sImgPath);
}

BOOL CAnimationView::GetCivColorPalette(RGBQUADPAL *pPal, LPCTSTR lpszImgPath /*=NULL*/)
{
	if (lpszImgPath != NULL)
	{
		if (!uIsFileExist(lpszImgPath))
			return FALSE;

		CPcx pcxImg;
		
		// Load related file with specified civilization color palette
		if (!pcxImg.LoadBgdImgFile(lpszImgPath))
			return FALSE;
		
		// Copy pcx palette to the current palette
		//ZeroMemory(pPal, RGBQUADPAL_SIZE);
		memcpy(pPal, &pcxImg.m_rgbPal, RGBQUADPAL_SIZE);
	}
	else
	{
		// Copy default palette to the current palette
		//ZeroMemory(pPal, RGBQUADPAL_SIZE);
		memcpy(pPal, &m_rgbPalDef, RGBQUADPAL_SIZE);
	}

	return TRUE;
}

void CAnimationView::OnDraw(CDC* pDC)
{
//	CAnimationDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
}

CSize CAnimationView::GetFrameSize()
{
	return m_bLeaderHead ? CSize(m_nFrmWidth + WND_ACX, m_nFrmHeight + WND_ACY) : CSize(WND_CX, WND_CY);
}

//////////////////////////////////////////////////////////////
// Try to get list of all *.wav files from the given *.amb file.
//
BOOL CAnimationView::GetWavFromAmb(CString sPath, CStringArray* pArWav, BOOL bSilent /*= TRUE*/)
{
	pArWav->RemoveAll(); // clear array
	
	if (!uIsFileExist(sPath))
		return FALSE;
	
	if (uGetFileExt(sPath).CompareNoCase("amb") != 0)
		return FALSE;
	
	CString     sContent;
	char*       pszBuf = NULL;
	int         nStart;
	
	// Get file content
	try
	{
		CFile flRes(sPath, CFile::modeRead | CFile::shareDenyNone); // open file
		
		nStart = flRes.GetLength();       // get length of the file
		
		pszBuf = new char[nStart + 1];
		flRes.Read(pszBuf, nStart);       // retrive all data from file
		pszBuf[nStart] = 0;
		flRes.Close();
		
		for (int i = 0; i < nStart; i++)
		{
			if (isgraph(pszBuf[i]))
				sContent += pszBuf[i]; // set data to string
			else
				sContent += '*'; // set '*' instead of none printable character 
		}
		
		delete [] pszBuf;
		pszBuf = NULL;
	}
	catch (CFileException* e)
	{
		if (!bSilent)
		{
			TCHAR szCause[1024];
			e->GetErrorMessage(szCause, 1024);
			AfxMessageBox(szCause, MB_ICONERROR);
		}
		e->Delete();
		return FALSE;
	}
	// check file header - is it correct AMB file?
	if (sContent.Left(4).CompareNoCase("prgm") != 0)
	{
		//"'%s'\r\nThe file has not valid *.amb file format."
		if (!bSilent)
			AfxMessageBox(uFormatString(IDS_AMB_ERR3, sPath), MB_ICONERROR);
		return FALSE;
	}
	
	nStart	 = 0;
	int nEnd = sContent.Find(".wav");
	
	if (nEnd == -1) // we don't have any defined wav files
	{
		//"'%s'\r\nThe file has not defined *.wav files."
		if (!bSilent)
			AfxMessageBox(uFormatString(IDS_AMB_ERR4, sPath), MB_ICONERROR);
		return FALSE;
	}
	
	CString sCurPart;
	CString sWavPath = uGetPathName(sPath);

	while (nEnd != -1)
	{
		sCurPart = sContent.Mid(nStart, nEnd-nStart);
		nStart   = sCurPart.ReverseFind('*') + 1;
		pArWav->Add(sWavPath + sCurPart.Mid(nStart, nEnd - nStart) + ".wav");
		nStart   = nEnd + 4;
		nEnd     = sContent.Find(".wav", nStart);
	}
	
	return pArWav->GetSize() > 0;
}

void CAnimationView::SelectWavPath()
{
	CString sFilePath;
	CString sPath;
	
	CFileWavDlg dlg(TRUE, NULL, NULL,
		OFN_ENABLESIZING | OFN_ENABLETEMPLATE |
		OFN_EXPLORER | OFN_FILEMUSTEXIST |
		OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST,
		_T(uLoadString(IDS_FILTER_WAV)),
		this);
	
	dlg.m_ofn.hInstance = AfxGetInstanceHandle();
	dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_WAV);
	
	CString sTitle(uLoadString(IDS_TITLE_WAV));
	dlg.m_ofn.lpstrTitle = sTitle;
	
	CWinApp* pApp = AfxGetApp();
	dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(REG_WAV_DLG, REG_FILTER, 1);
	
	sPath = pApp->GetProfileString(REG_WAV_DLG, REG_LAST_OPENED, "");
	
	if (uIsFileExist(sPath))
		dlg.m_ofn.lpstrInitialDir = sPath;
	
	//	dlg.m_bOldPath = m_pPrto->IsWavOldPath(nAct);
	
	if (dlg.DoModal() != IDOK)
		return;
	
	sPath = dlg.GetPathName();
	CString sExt  = dlg.GetFileExt();
	
	pApp->WriteProfileString(REG_WAV_DLG, REG_LAST_OPENED, sPath);
	
	m_sPathWAV.Empty();
	m_arPathWAV.RemoveAll();
	
	m_pPanelSound->m_sPathWAV = m_sPathWAV = sPath;
	m_pPanelSound->UpdateData(FALSE);

	// Check the *.AMB file
	if (sExt.CompareNoCase("amb") == 0)
	{
		CString sWavList;
		
		if (GetWavFromAmb(sPath, &m_arPathWAV, FALSE))
		{
			for (int i = 0; i < m_arPathWAV.GetSize(); i++)
				sWavList += m_arPathWAV.GetAt(i) + "\r\n";
			
			InfoShowDialog(
				uFormatString(IDS_CHECK_AMB_RESULT, sPath, sWavList),
				uLoadString(IDS_CHECK_AMB_HEADER), 0, 
				uLoadString(IDS_CHECK_AMB_CAP));
		}
		else
			return;
	}
	else
		m_arPathWAV.Add(m_sPathWAV);

	UpdateControlsState();
}

void CAnimationView::PlaySoundFile()
{
	sndPlaySound(NULL, SND_SYNC);
	
	if (m_sPathWAV.IsEmpty())
		return;
	
	UINT uFlags = 0;
	int nCount = m_arPathWAV.GetSize();
	
	if (nCount == 0)
		return;
	else if (nCount == 1) // for WAV
	{
		uFlags = SND_ASYNC;
		
		if (m_pPanelSound->m_bNonStopWAV)
			uFlags = uFlags | SND_LOOP | SND_NOSTOP;
		
		sndPlaySound(m_arPathWAV.GetAt(0), uFlags);
	}
	else // for AMB
	{
		m_pPanelSound->m_btnNonStopWAV.EnableWindow(FALSE);
		m_pPanelSound->m_btnPlayWAV.EnableWindow(FALSE);
		m_pPanelSound->m_btnStopWAV.EnableWindow(FALSE);
		
		for (int i = 0; i < m_arPathWAV.GetSize(); i++)
		{
			sndPlaySound(m_arPathWAV.GetAt(i), uFlags);
			TRACE(m_arPathWAV.GetAt(i) + "\r\n");
		}
		
		m_pPanelSound->m_btnNonStopWAV.EnableWindow(TRUE);
		m_pPanelSound->m_btnPlayWAV.EnableWindow(TRUE);
		m_pPanelSound->m_btnStopWAV.EnableWindow(TRUE);
	}
}

void CAnimationView::StopSoundFile()
{
	sndPlaySound(NULL, SND_SYNC);
}

// Called from CChildFrm::OnMDIActivate
void CAnimationView::ActivateDlgBar(BOOL bActivate)
{
	m_bActive = bActivate;
	
	if (m_bActive)
	{
		m_pDlgBar->SetView(this);
		m_pPanelView->m_nPalette		= m_nCurPalette;
		m_pPanelDelay->m_nTimerValue	= !IsDefaultTimer();
		m_pPanelSound->m_sPathWAV	= m_sPathWAV;
		
		m_pPanelView->m_wndPalettes.SetItemColor(m_pPanelView->m_nPalette, RGB(
			m_rgbPal.pal[CIV_COLOR].rgbRed,
			m_rgbPal.pal[CIV_COLOR].rgbGreen, 
			m_rgbPal.pal[CIV_COLOR].rgbBlue));

		m_pPanelView->UpdateData(FALSE);
		m_pPanelDelay->UpdateData(FALSE);
		m_pPanelSound->UpdateData(FALSE);

		UpdateInfo();
		UpdateControlsState();
		
		MAIN_FRAME->m_crBgd = m_bgdCustomColor;
		MAIN_FRAME->GetToolBar()->Invalidate();
	}
	else
	{
		m_pDlgBar->SetView(NULL);
		StopSoundFile();
	}
}

void CAnimationView::OnZoom() 
{
	BYTE nWidth, nHeight;
	if (!IsLoaded())
		return;
	
	m_bPlayStatic = FALSE;
	if (!m_bLeaderHead)
	{
		nWidth	= WND_W;
		nHeight	= WND_H;
		
		if (m_bgdSurface)	// Draw background image
			m_bgdSurface->Draw(0, 0);
		else				// Fill background
			m_pScreen->Fill(m_bgdColor);
	}
	else
	{
		nWidth	= m_nFrmWidth;
		nHeight	= m_nFrmHeight;
	}
	
	DrawFrame();

	CZoomingDlg dlg(m_pScreen->GetBack(), nWidth, nHeight);
	dlg.DoModal();
}

void CAnimationView::OnUpdateWindowCloseall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CAnimationView::OnSaveFrameToFile() 
{
	CFlcImage img;
	img.SaveFrameToFile(this);
}

void CAnimationView::OnUpdateLoaded(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
}

void CAnimationView::OnUpdateDirectionAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded());
	pCmdUI->SetCheck(m_eDirection == DirAll);
}

void CAnimationView::OnUpdateDirectionE(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirE);
}

void CAnimationView::OnUpdateDirectionN(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirN);
}

void CAnimationView::OnUpdateDirectionNE(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirNE);
}

void CAnimationView::OnUpdateDirectionNW(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirNW);
}

void CAnimationView::OnUpdateDirectionS(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirS);
}

void CAnimationView::OnUpdateDirectionSE(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirSE);
}

void CAnimationView::OnUpdateDirectionSW(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirSW);
}

void CAnimationView::OnUpdateDirectionW(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && m_nDirCount == DIRECTIONS_MAX);
	pCmdUI->SetCheck(m_eDirection == DirW);
}

void CAnimationView::OnColorReplace() 
{
	CColorReplaceDlg dlg(&m_rgbPal);
	if (dlg.DoModal() != IDOK)
		return;
	
	if (!ApplyPaletteChanges())
	{
		OnInitialUpdate();
		AfxMessageBox(uLoadString(IDS_CANNOT_CHANGE_PALETTE), MB_ICONERROR);
		return;
	}
	
	if (m_bActive)
	{
		m_pPanelView->m_nPalette = 0;
		m_pPanelView->UpdateData(FALSE);
		
		m_pPanelView->m_wndPalettes.SetItemColor(m_pPanelView->m_nPalette, RGB(
			m_rgbPal.pal[CIV_COLOR].rgbRed,
			m_rgbPal.pal[CIV_COLOR].rgbGreen, 
			m_rgbPal.pal[CIV_COLOR].rgbBlue));
	}

	m_bPlayStatic = FALSE;
}

void CAnimationView::OnBgdGrid() 
{
	if (!SetBgdImg(BgdGrid))
		AfxMessageBox(IDS_ERR_SET_BGD_IMG, MB_ICONERROR);
}

void CAnimationView::OnUpdateBgdGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
	pCmdUI->SetRadio(m_eBackground == BgdGrid);
}

void CAnimationView::OnBgdImg() 
{
	if (!SetBgdImg(BgdImg))
		AfxMessageBox(IDS_ERR_SET_BGD_IMG, MB_ICONERROR);
}

void CAnimationView::OnUpdateBgdImg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
	pCmdUI->SetRadio(m_eBackground == BgdImg);
}

void CAnimationView::OnSelectImg() 
{
	CSelectBgdImgDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	if (uIsFileExist(dlg.m_sTargetPath))
		OnBgdImg();
}

void CAnimationView::OnUpdateSelectImg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsLoaded() && (!IsLeaderHead() && IsUseAlphaBlending()));
}

void CAnimationView::UpdateParams()
{
	CWinApp* pApp = AfxGetApp();
	m_bShowStaticFrm	= pApp->GetProfileInt(REG_OPTIONS, REG_ANIM_TYPE, 0);
	m_bIgnoreOffset		= pApp->GetProfileInt(REG_OPTIONS, REG_IGNORE_OFFSET, 0);

	m_xPos	= m_bIgnoreOffset ? 0 : m_xOffset;
	m_yPos	= m_bIgnoreOffset ? 0 : m_yOffset;
}
