/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ExportFlcGen.cpp
	CREATED:	02.7.2004
	VERSION:	1.2

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ExportFlcGen.h"

#include "ExportFlcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportFlcGen dialog


CExportFlcGen::CExportFlcGen(CWnd* pParent /*=NULL*/)
	: CDialogX(CExportFlcGen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportFlcGen)
	m_sFrmCount = _T("");
	m_sDelay = _T("");
	m_sOutDir = _T("");
	m_sFrmHeight = _T("");
	m_sOutName = _T("");
	m_sPalette = _T("");
	m_sOutType = _T("");
	m_sFrmWidth = _T("");
	m_sOutPath = _T("");
	m_sFrmResize = _T("");
	m_sPalType = _T("");
	m_sXOffset = _T("");
	m_sYOffset = _T("");
	m_sLastColors = _T("");
	m_sResizeLeft = _T("");
	m_sResizeRight = _T("");
	m_sResizeTop = _T("");
	m_sResizeBottom = _T("");
	m_sColDepth = _T("");
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
	
	m_pParent = (CExportFlcDlg*) pParent;
	m_bDrawGripper = FALSE;
}

void CExportFlcGen::DoDataExchange(CDataExchange* pDX)
{
	CDialogX::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportFlcGen)
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
	DDX_Text(pDX, IDC_STATIC_RESIZE, m_sFrmResize);
	DDX_Text(pDX, IDC_STATIC_PAL, m_sPalType);
	DDX_Text(pDX, IDC_STATIC_OFFSET_LEFT, m_sXOffset);
	DDX_Text(pDX, IDC_STATIC_OFFSET_TOP, m_sYOffset);
	DDX_Text(pDX, IDC_STATIC_LAST_COLORS, m_sLastColors);
	DDX_Text(pDX, IDC_STATIC_RESIZE_LEFT, m_sResizeLeft);
	DDX_Text(pDX, IDC_STATIC_RESIZE_RIGHT, m_sResizeRight);
	DDX_Text(pDX, IDC_STATIC_RESIZE_TOP, m_sResizeTop);
	DDX_Text(pDX, IDC_STATIC_RESIZE_BOTTOM, m_sResizeBottom);
	DDX_Text(pDX, IDC_STATIC_COL_DEPTH, m_sColDepth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportFlcGen, CDialogX)
	//{{AFX_MSG_MAP(CExportFlcGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportFlcGen message handlers

BOOL CExportFlcGen::OnInitDialog() 
{
	CDialogX::OnInitDialog();
	
	ApplyChanges();
	return TRUE;
}

void CExportFlcGen::ApplyChanges()
{
	m_sOutPath		= m_pParent->m_sPath;
	m_sOutDir		= m_pParent->m_sDir;
	m_sOutName		= m_pParent->m_sName;
	m_sOutType		= uLoadString(m_pParent->m_nFileType == PCX_TYPE ? IDS_FILE_TYPE_PCX : IDS_FILE_TYPE_BMP);
	m_sPalType		= uLoadString(IDS_PAL_TYPE_ACT + m_pParent->m_nPalType);
	
	m_sFrmWidth		= uFormatString(IDS_INF_PIXELS, m_pParent->m_nFrmWidth);
	m_sFrmHeight	= uFormatString(IDS_INF_PIXELS, m_pParent->m_nFrmHeight);
	m_sXOffset		= uFormatString(IDS_INF_PIXELS, m_pParent->m_nXOffset);
	m_sYOffset		= uFormatString(IDS_INF_PIXELS, m_pParent->m_nYOffset);
	
	m_sFrmResize	= uLoadString(m_pParent->m_bFrmResize ? IDS_INF_YES : IDS_INF_NO);
	m_sFrmCount		= uInt2Str(m_pParent->m_nFrmCount);
	m_sDelay		= uFormatString(IDS_INF_MS, m_pParent->m_nDelay);
	m_sPalette		= m_pParent->m_sPalette;

	switch (m_pParent->m_nLastPalType)
	{
	case PalCurrent:	m_sLastColors = uLoadString(IDS_INF_PAL_CURRENT);	break;
	case PalHardcoded:	m_sLastColors = uLoadString(IDS_INF_PAL_HARDCODED);	break;
	case PalSelective:	m_sLastColors = uLoadString(IDS_INF_PAL_SELECTIVE);	break;
	case PalError:		m_sLastColors = uLoadString(IDS_INF_PAL_ERROR);		break;
	}

	m_sResizeLeft	= uFormatString(IDS_INF_PIXELS, m_pParent->m_nResizeLeft);
	m_sResizeRight	= uFormatString(IDS_INF_PIXELS, m_pParent->m_nResizeRight);
	m_sResizeTop	= uFormatString(IDS_INF_PIXELS, m_pParent->m_nResizeTop);
	m_sResizeBottom	= uFormatString(IDS_INF_PIXELS, m_pParent->m_nResizeBottom);
	m_sColDepth		= uFormatString(IDS_INF_COL_DEPTH, m_pParent->m_nBitsPerPixel == BitsPerPixel_8 ? BITS_8 : BITS_24);

	UpdateData(FALSE);
}
