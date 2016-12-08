/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FlcInfoExt.cpp
	CREATED:	09.7.2004
	VERSION:	1.0

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FlcInfoExt.h"

#include "FlcInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COL_TYPE 0
#define COL_NOTE 1
#define COL_POS 2
#define COL_SIZE 3

enum EImgList
{
    imgFrame = 0,
    imgDelta = 1,
    imgRun   = 2,
    imgColor = 3,
    imgBlack = 4,
    imgCopy  = 5,
    imgErr   = 6
};

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoExt dialog

CFlcInfoExt::CFlcInfoExt(CWnd* pParent /*=NULL*/)
  : CDialogX(CFlcInfoExt::IDD, pParent)
{
    //{{AFX_DATA_INIT(CFlcInfoExt)
    //}}AFX_DATA_INIT

    SetControlInfo(IDC_LIST, CF_RESIZE_BOTH);

    m_pParent      = (CFlcInfoDlg*)pParent;
    m_bDrawGripper = FALSE;
}

void
CFlcInfoExt::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFlcInfoExt)
    DDX_Control(pDX, IDC_LIST, m_Table);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlcInfoExt, CDialogX)
//{{AFX_MSG_MAP(CFlcInfoExt)
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlcInfoExt message handlers

BOOL
CFlcInfoExt::OnInitDialog()
{
    CDialogX::OnInitDialog();

    CBitmap bmp;
    m_imgState.Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 0);
    bmp.LoadBitmap(IDB_FLC_INFO);
    m_imgState.Add(&bmp, RGB_BACKGROUND);

    m_Table.SetImageList(&m_imgState, LVSIL_SMALL);

    m_Table.ColorSortColumn();
    m_Table.KeepLabelLeft();
    m_Table.SetRowHeight(18);

    m_Table.InsertColumn(COL_TYPE, uLoadString(IDS_COL_TYPE), LVCFMT_LEFT, 150);

    m_Table.InsertColumn(COL_NOTE, uLoadString(IDS_COL_NOTE), LVCFMT_LEFT, 100);

    m_Table.InsertColumn(COL_POS, uLoadString(IDS_COL_POS), LVCFMT_LEFT, 100);

    m_Table.InsertColumn(COL_SIZE, uLoadString(IDS_COL_SIZE), LVCFMT_LEFT, 100);

    m_Table.LoadColumns(4, 150, 100, 100, 100);

    FRAME frm;
    int idx = 0;
    int j;
    EImgList nImg;

    CString sType;
    for (int i = 0; i < m_pParent->m_pFlc->m_arFrames.GetSize(); i++) {
        frm = m_pParent->m_pFlc->m_arFrames.GetAt(i);

        m_Table.InsertItem(idx, uFormatString(IDS_TYPE_FRAME, i));
        m_Table.SetItemText(idx, COL_NOTE, uFormatString(IDS_NOTE_FRAME, frm.numch));
        m_Table.SetItemText(idx, COL_POS, uInt2Str(frm.pos));
        m_Table.SetItemText(idx, COL_SIZE, uInt2Str(frm.size));
        m_Table.SetItemImage(idx, imgFrame);

        idx++;
        /*enum EImgList
		{
			imgFrame	= 0,
				imgDelta	= 1,
				imgRun		= 2,
				imgColor	= 3,
				imgBlack	= 4,
				imgCopy		= 5,
				imgErr		= 6
		};
		*/
        for (j = 0; j < frm.numch; j++) {
            m_Table.InsertItem(idx, uFormatString(IDS_TYPE_CHUNK, j));

            switch (frm.chunks[j].type) {
                case ChDELTA_FLC:
                    sType = "DELTA_FLC";
                    nImg  = imgDelta;
                    break;
                case ChBYTE_RUN:
                    sType = "BYTE_RUN";
                    nImg  = imgRun;
                    break;
                case ChCOLOR_256:
                    sType = "COLOR_256";
                    nImg  = imgColor;
                    break;
                case ChBLACK:
                    sType = "BLACK";
                    nImg  = imgBlack;
                    break;
                case ChFLI_COPY:
                    sType = "FLI_COPY";
                    nImg  = imgCopy;
                    break;
                default:
                    sType = "unknown";
                    nImg  = imgErr;
            }

            m_Table.SetItemText(idx, COL_NOTE, sType);
            m_Table.SetItemText(idx, COL_POS, uInt2Str(frm.chunks[j].pos));
            m_Table.SetItemText(idx, COL_SIZE, uInt2Str(frm.chunks[j].size));
            m_Table.SetItemImage(idx, nImg);

            idx++;
        }
    }

    return TRUE;
}

void
CFlcInfoExt::OnDestroy()
{
    m_Table.SaveColumns();
    CDialogX::OnDestroy();
}
