// AnimationView.h : interface of the CAnimationView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ANIMATION_VIEW_CIV3FLCEDIT__INCLUDED_)
#define ANIMATION_VIEW_CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Surface.h"
#include "Screen.h"
#include "GraphicsLib.h"
#include "Definitions.h"

#include "ComboBoxCivColor.h"

#include "..\MFCaids\FormViewX.h"
#include "..\MFCaids\BtnST.h"

class CDlgBar;

typedef CArray<CSurface*, CSurface*> ARRAY_SURF;

class CAnimationView : public CView
{
    friend class CFlcImage;
    friend class CDlgBar;

    friend class CPanelAnimation;
    friend class CPanelDelay;
    friend class CPanelDirection;
    friend class CPanelExtra;
    friend class CPanelSound;
    friend class CPanelView;
    friend class COffsetDlg;

  protected: // create from serialization only
    CAnimationView();
    DECLARE_DYNCREATE(CAnimationView)

    // Attributes
  public:
    CDlgBar* GetDlgBar()
    {
        return m_pDlgBar;
    }

    // Operations
  public:
    CSize GetFrameSize();
    void UpdateControlsState();
    virtual void ClearSFrames();
    virtual BOOL LoadAnimation(LPCTSTR lpszPathName, BOOL bColorTable = TRUE);
    BOOL IsUseAlphaBlending()
    {
        return m_bUseAlphaBlending;
    }
    BOOL UseAlphaBlending(BOOL bUse = TRUE);
    BOOL ApplyPaletteChanges();
    COLORREF GetDefaultBgdColor();
    BOOL IsLeaderHead();
    BOOL ChangePalette(BYTE nColor, RGBQUADPAL* pPal = NULL);
    BOOL GetCivColorPalette(RGBQUADPAL* pPal, LPCTSTR lpszImgPath = NULL);
    BOOL SetCivColorPalette(LPCTSTR lpszImgPath = NULL);
    BOOL IsDefaultTimer();
    void SetTimerValue(UINT uElapse);
    void UseDefaultTimer(BOOL bUseDefault = TRUE);
    void UpdateBackground();
    void SetFramePos(EFramePos pos);
    void UpdateFrame(float t);                    // Called to update the civ3 unit
    void DrawFrame();                             // Called to draw the current frame
    BOOL LoadAnimationFile(LPCTSTR lpszPathName); // Called to load in a civ3 unit file
    BOOL ReloadWithPalette();
    EDirection SetDirection(EDirection direction);
    EDirection GetDirection()
    {
        return m_eDirection;
    }
    void Reset();

    BOOL SetBgdImg(EBdg typeBgd);
    BOOL SetBackgroundImage(LPCTSTR lpszImgPath); // set background image
    void ResetBackgroundColor();                  // set background color to default
    void SetBackgroundColor(COLORREF color);      // set background color

    void Stop();  // reset variables
    void Start(); // prepare to drawing animation

    BOOL Init();
    BOOL PlayAnimation();
    void Shutdown();

    BOOL IsLoaded()
    {
        return m_bLoaded;
    }
    BOOL IsActive()
    {
        return m_bActive;
    }
    BOOL IsPlaying()
    {
        return m_bPlaying;
    }
    void SetActive(BOOL bActive)
    {
        m_bActive = bActive;
        m_bPaused = FALSE;
    }

    BOOL IsPaused()
    {
        return m_bPaused;
    }
    void Pause(BOOL bPaused)
    {
        m_bPaused = bPaused;
    }

    void StartAnimationTimer();
    void KillAnimationTimer();

    CScreen* GetScreen()
    {
        return m_pScreen;
    }

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAnimationView)
  public:
    virtual void OnDraw(CDC* pDC); // overridden to draw this view
  protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CAnimationView();
    void StopRewind();
    void Rewind();
    void UpdateFrameSlider();
    void UpdateInfo();
    void ReleasedcaptureFramesSlider(WORD pos);
    void SelendokComboPalettes(int nOldPalette);
    virtual void ShowAnimInfo(){};
    virtual void Refresh(){};
    static BOOL GetWavFromAmb(CString sPath, CStringArray* pArWav, BOOL bSilent = TRUE);
    void SelectWavPath();
    void StopSoundFile();
    void PlaySoundFile();
    void ActivateDlgBar(BOOL bActivate);
    void SetPathWAV(LPCSTR sPath)
    {
        m_sPathWAV = sPath;
    }

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
    CDlgBar* m_pDlgBar;

    BOOL m_bPlayStatic; // play the same frame all time
    BOOL m_bPlaying;
    BOOL m_bActive;
    BOOL m_bPaused;
    BOOL m_bLoaded;
    BOOL m_bLeaderHead;
    BOOL m_bDefaultTimer;
    BOOL m_bShowStaticFrm;
    BOOL m_bIgnoreOffset;
    UINT m_uElapse;

    DWORD m_dwStartFrame;
    DWORD m_dwEndFrame;
    float m_frameTime;
    float m_speed; // animation speed

    RGBQUAD* m_alphaShadow;
    RGBQUAD* m_alphaSmoke;

    CSurface* m_bgdSurface;

    BYTE m_nDirCount;
    BYTE m_nFrmHeight;
    BYTE m_nFrmWidth;
    BYTE m_xPos;
    BYTE m_yPos;
    BYTE m_xOffset;
    BYTE m_yOffset;

    WORD m_nTotalFrames;

    UINT m_uSpeed;
    WORD m_animLenth;
    WORD m_firstFrame;  // first playing frame
    WORD m_lastFrame;   // last playing frame
    int m_curFrame;     // current frame
    WORD m_prevFrame;   // last played frame
    BYTE m_nCurPalette; // current palette 0 - default, 1-32 - civ. specifieed
    UINT m_nPixelsSize;

    EDirection m_eDirection;    // current direction
    EDirection m_eDirectionDef; // default direction
    COLORREF m_bgdColor;        // custom background color
    COLORREF m_bgdCustomColor;  // custom background color
    COLORREF m_bgdDefaultColor; // default background color
    EBdg m_eBackground;         // type of the background

    ARRAY_SURF m_arSFrames;
    ARRAY_SURF m_arSByteRun;

    CScreen* m_pScreen;

    DDCOLORKEY m_ddcTransparent;
    COLORREF m_colTransparent; // transparent color

    RGBQUADPAL m_rgbPalDef; // default palette
    RGBQUADPAL m_rgbPal;    // current palette

    CString m_sPathName;           // animation file path
    CDWordArray m_arColorChunkPos; // positions of the color chunks

    BOOL m_bUseAlphaBlending; // use or not alpha blending

    CStringArray m_arPathWAV;
    CString m_sPathWAV;

    CPanelAnimation* m_pPanelAnimation;
    CPanelDelay* m_pPanelDelay;
    CPanelDirection* m_pPanelDirection;
    CPanelExtra* m_pPanelExtra;
    CPanelSound* m_pPanelSound;
    CPanelView* m_pPanelView;

    // Generated message map functions
  public:
    void UpdateParams();
    //{{AFX_MSG(CAnimationView)
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnPlay();
    afx_msg void OnPause();
    afx_msg void OnStop();
    afx_msg void OnInfo();
    afx_msg void OnBgdDefault();
    afx_msg void OnBgdSelect();
    afx_msg void OnBgdSea();
    afx_msg void OnBgdLand();
    afx_msg void OnDirectionSW();
    afx_msg void OnDirectionW();
    afx_msg void OnDirectionNW();
    afx_msg void OnDirectionN();
    afx_msg void OnDirectionNE();
    afx_msg void OnDirectionE();
    afx_msg void OnDirectionSE();
    afx_msg void OnDirectionS();
    afx_msg void OnDirectionAll();
    afx_msg void OnCustomColor();
    afx_msg void OnFirst();
    afx_msg void OnPrevious();
    afx_msg void OnNext();
    afx_msg void OnLast();
    afx_msg void OnRadioTimerDef();
    afx_msg void OnRadioTimerCustom();
    afx_msg void OnColorTable();
    afx_msg void OnBack();
    afx_msg void OnForward();
    afx_msg void OnImportPalette();
    afx_msg void OnExportPalette();
    afx_msg void OnRefresh();
    afx_msg void OnUseAlphaBlending();
    afx_msg void OnCopyToClipboard();
    afx_msg void OnCopyToFile();
    afx_msg void OnUpdateFlcPlay(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcPause(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcStop(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcInfo(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBgdDefault(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBgdLand(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBgdSea(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBgdSelect(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcCustomColor(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcFirst(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcPrevious(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcNext(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcLast(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcColorTable(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcBack(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFlcForward(CCmdUI* pCmdUI);
    afx_msg void OnUpdateUseAlphaBlending(CCmdUI* pCmdUI);
    afx_msg void OnZoom();
    afx_msg void OnUpdateWindowCloseall(CCmdUI* pCmdUI);
    afx_msg void OnSaveFrameToFile();
    afx_msg void OnUpdateLoaded(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionAll(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionE(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionN(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionNE(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionNW(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionS(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionSE(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionSW(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirectionW(CCmdUI* pCmdUI);
    afx_msg void OnColorReplace();
    afx_msg void OnBgdGrid();
    afx_msg void OnUpdateBgdGrid(CCmdUI* pCmdUI);
    afx_msg void OnBgdImg();
    afx_msg void OnUpdateBgdImg(CCmdUI* pCmdUI);
    afx_msg void OnSelectImg();
    afx_msg void OnUpdateSelectImg(CCmdUI* pCmdUI);
    //}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ANIMATION_VIEW_CIV3FLCEDIT__INCLUDED_)
