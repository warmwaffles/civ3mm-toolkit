/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	NewFlcGen.cpp
	CREATED:	06.7.2004
	VERSION:	1.2

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "NewFlcGen.h"

#include "NewFlcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFlcGen dialog

CNewFlcGen::CNewFlcGen(CWnd* pParent /*=NULL*/)
  : CDialogX(CNewFlcGen::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNewFlcGen)
    m_sFrmCount   = _T("");
    m_sDelay      = _T("");
    m_sOutDir     = _T("");
    m_sFrmHeight  = _T("");
    m_sOutName    = _T("");
    m_sPalette    = _T("");
    m_sOutType    = _T("");
    m_sFrmWidth   = _T("");
    m_sOutPath    = _T("");
    m_sDirCount   = _T("");
    m_sPalType    = _T("");
    m_sAnimType   = _T("");
    m_sXOffset    = _T("");
    m_sYOffset    = _T("");
    m_sLastColors = _T("");
    m_sColDepth   = _T("");
    //}}AFX_DATA_INIT

    SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_2, CF_RESIZE_BOTH);

    SetControlInfo(IDC_TARGET_PATH, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_DIR, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_NAME, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_TYPE, CF_RESIZE_HOR);

    SetControlInfo(IDC_STATIC_HEIGHT, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_WIDTH, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_COUNT, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_PALETTE, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_DELAY, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_LAST_COLORS, CF_RESIZE_HOR);

    m_pParent      = (CNewFlcDlg*)pParent;
    m_bDrawGripper = FALSE;
}

void
CNewFlcGen::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewFlcGen)
    DDX_Control(pDX, IDC_TARGET_PATH, m_wndOutPath);
    DDX_Text(pDX, IDC_STATIC_COUNT, m_sFrmCount);
    DDX_Text(pDX, IDC_STATIC_DELAY, m_sDelay);
    DDX_Text(pDX, IDC_STATIC_DIR, m_sOutDir);
    DDX_Text(pDX, IDC_STATIC_HEIGHT, m_sFrmHeight);
    DDX_Text(pDX, IDC_STATIC_NAME, m_sOutName);
    DDX_Text(pDX, IDC_STATIC_PALETTE, m_sPalette);
    DDX_Text(pDX, IDC_STATIC_TYPE, m_sOutType);
    DDX_Text(pDX, IDC_STATIC_WIDTH, m_sFrmWidth);
    DDX_Text(pDX, IDC_TARGET_PATH, m_sOutPath);
    DDX_Text(pDX, IDC_STATIC_DIR_COUNT, m_sDirCount);
    DDX_Text(pDX, IDC_STATIC_PAL, m_sPalType);
    DDX_Text(pDX, IDC_STATIC_ANIMATION, m_sAnimType);
    DDX_Text(pDX, IDC_STATIC_OFFSET_LEFT, m_sXOffset);
    DDX_Text(pDX, IDC_STATIC_OFFSET_TOP, m_sYOffset);
    DDX_Text(pDX, IDC_STATIC_LAST_COLORS, m_sLastColors);
    DDX_Text(pDX, IDC_STATIC_COL_DEPTH, m_sColDepth);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewFlcGen, CDialogX)
//{{AFX_MSG_MAP(CNewFlcGen)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFlcGen message handlers

BOOL
CNewFlcGen::OnInitDialog()
{
    CDialogX::OnInitDialog();

    ApplyChanges();
    return TRUE;
}

void
CNewFlcGen::ApplyChanges()
{
    m_sOutPath  = m_pParent->m_sPath;
    m_sOutDir   = m_pParent->m_sDir;
    m_sOutName  = m_pParent->m_sName;
    m_sOutType  = uLoadString(m_pParent->m_nFileType == PCX_TYPE ? IDS_FILE_TYPE_PCX : IDS_FILE_TYPE_BMP);
    m_sPalType  = uLoadString(IDS_PAL_TYPE_ACT + m_pParent->m_nPalType);
    m_sAnimType = uLoadString(m_pParent->m_bLeaderHead ? IDS_INF_LEADERHEAD : IDS_INF_UNIT);

    m_sFrmWidth  = uFormatString(IDS_INF_PIXELS, m_pParent->m_nFrmWidth);
    m_sFrmHeight = uFormatString(IDS_INF_PIXELS, m_pParent->m_nFrmHeight);
    m_sXOffset   = uFormatString(IDS_INF_PIXELS, m_pParent->m_nXOffset);
    m_sYOffset   = uFormatString(IDS_INF_PIXELS, m_pParent->m_nYOffset);

    m_sDirCount = uInt2Str(m_pParent->m_nDirCount);
    m_sFrmCount = uInt2Str(m_pParent->m_nFrmCount);
    m_sDelay    = uFormatString(IDS_INF_MS, m_pParent->m_nDelay);
    m_sPalette  = m_pParent->m_sPalette;
    m_sColDepth = uFormatString(IDS_INF_COL_DEPTH, m_pParent->m_nBitsPerPixel == BitsPerPixel_8 ? BITS_8 : BITS_24);

    switch (m_pParent->m_nLastPalType) {
        case PalCurrent:
            m_sLastColors = uLoadString(IDS_INF_PAL_CURRENT);
            break;
        case PalHardcoded:
            m_sLastColors = uLoadString(IDS_INF_PAL_HARDCODED);
            break;
        case PalSelective:
            m_sLastColors = uLoadString(IDS_INF_PAL_SELECTIVE);
            break;
        case PalError:
            m_sLastColors = uLoadString(IDS_INF_PAL_ERROR);
            break;
    }

    UpdateData(FALSE);
}
