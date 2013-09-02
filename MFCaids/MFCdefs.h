/////////////////////////////////////////////////////////////////////////////
// MFCdefs.h: MFC definitions
//

#ifndef __MFC_DEFINITIONS_H__INCLUDED_
#define __MFC_DEFINITIONS_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// Copied from the latest Platform SDK available.
// This should help if you don't have SDK installed on your PC.

/////////////////////////////////////////////////////////////////////////////
// COMCTL32 v5.80 specific:
#ifndef RBBS_USECHEVRON
#define RBBS_USECHEVRON     0x00000200
#define RBHT_CHEVRON        0x0008
#define RB_PUSHCHEVRON      (WM_USER + 43)
#define RBN_CHEVRONPUSHED   (RBN_FIRST - 10)

typedef struct tagNMREBARCHEVRON
{
    NMHDR hdr;
    UINT uBand;
    UINT wID;
    LPARAM lParam;
    RECT rc;
    LPARAM lParamNM;
} NMREBARCHEVRON, *LPNMREBARCHEVRON;
#endif  // !RBBS_USECHEVRON

#ifndef BTNS_WHOLEDROPDOWN
#define BTNS_WHOLEDROPDOWN      0x0080
#endif  // !BTNS_WHOLEDROPDOWN

#ifndef TBN_INITCUSTOMIZE
#define TBN_INITCUSTOMIZE       (TBN_FIRST - 23)
#define TBNRF_HIDEHELP          0x00000001
#endif  // !TBN_INITCUSTOMIZE

#ifndef TBN_SAVE
#define TBN_SAVE                (TBN_FIRST - 22)
#define TBN_RESTORE             (TBN_FIRST - 21)

typedef struct tagNMTBSAVE
{
    NMHDR hdr;
    DWORD* pData;
    DWORD* pCurrent;
    UINT cbData;
    int iItem;
    int cButtons;
    TBBUTTON tbButton;
} NMTBSAVE, *LPNMTBSAVE;

typedef struct tagNMTBRESTORE
{
    NMHDR hdr;
    DWORD* pData;
    DWORD* pCurrent;
    UINT cbData;
    int iItem;
    int cButtons;
    int cbBytesPerRecord;
    TBBUTTON tbButton;
} NMTBRESTORE, *LPNMTBRESTORE;
#endif  // !TBN_SAVE


/////////////////////////////////////////////////////////////////////////////
// COMCTL32 v5.81 specific:

#ifndef TBSTYLE_EX_MIXEDBUTTONS
#define TBSTYLE_EX_MIXEDBUTTONS         0x00000008
#define TBSTYLE_EX_HIDECLIPPEDBUTTONS   0x00000010
#endif  // !TBSTYLE_EX_MIXEDBUTTONS

#ifndef BTNS_SHOWTEXT
#define BTNS_SHOWTEXT       0x0040
#endif  // !BTNS_SHOWTEXT

#ifndef I_IMAGENONE
#define I_IMAGENONE         (-2)
#endif  // !I_IMAGENONE

#ifndef IS_INTRESOURCE
#define IS_INTRESOURCE(hh) (HIWORD(hh) == NULL)
#endif

/////////////////////////////////////////////////////////////////////////////
// Default background color for all toolbars and image lists
static COLORREF RGB_BACKGROUND = RGB(255, 0, 255);

#define SCALE_DARK       30  // clolor scale for dark elements
#define SCALE_LIGHT      100 // clolor scale for light elements 

/////////////////////////////////////////////////////////////////////////////
// For using by CFormViewX and CDialogX classes
#define CF_ANCHOR_LEFT		0x0000
#define	CF_ANCHOR_TOP		0x0000
#define	CF_ANCHOR_RIGHT		0x0001
#define	CF_ANCHOR_BOTTOM	0x0002
#define	CF_RESIZE_HOR		0x0004
#define	CF_RESIZE_VER		0x0008
#define	CF_RESIZE_BOTH		0x000c

class CFormCtrlInfo
{
public:
	DWORD m_id;
	CRect m_pos;
	WORD  m_Anchor;
	WORD  m_xDivider;
	WORD  m_xDivPos;
	WORD  m_yDivider;
	WORD  m_yDivPos;
	WORD  m_xDivWidth;
	WORD  m_yDivHeight;
};

/////////////////////////////////////////////////////////////////////////////
#endif  // !__MFC_DEFINITIONS_H__INCLUDED_
