// FramePosition.cpp: implementation of the CFramePosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FramePosition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CFramePosition
//////////////////////////////////////////////////////////////////////

CFramePosition::CFramePosition()
{
}

CFramePosition::~CFramePosition()
{
}

// Save the size and position of the frame, so next time
// we'll start with this size
void
CFramePosition::SavePosition(CWnd* pWnd)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    pWnd->GetWindowPlacement(&wp);

    AfxGetApp()->WriteProfileInt(RegKey(), "CX", wp.rcNormalPosition.right - wp.rcNormalPosition.left);
    AfxGetApp()->WriteProfileInt(RegKey(), "CY", wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);

    AfxGetApp()->WriteProfileInt(RegKey(), "X", wp.rcNormalPosition.left);
    AfxGetApp()->WriteProfileInt(RegKey(), "Y", wp.rcNormalPosition.top);

    AfxGetApp()->WriteProfileInt(RegKey(), "Max", ((wp.showCmd & SW_SHOWMAXIMIZED) == SW_SHOWMAXIMIZED));
}

// Load the previous size and position of the frame from the Registry
void
CFramePosition::LoadPosition(CWnd* pWnd, BOOL bMax)
{
    ASSERT(pWnd);
    ASSERT(AfxGetApp());

    WINDOWPLACEMENT wp;
    wp.length                  = sizeof(wp);
    wp.flags                   = 0;
    wp.rcNormalPosition.left   = AfxGetApp()->GetProfileInt(RegKey(), "X", 0);
    wp.rcNormalPosition.top    = AfxGetApp()->GetProfileInt(RegKey(), "Y", 0);
    wp.rcNormalPosition.right  = wp.rcNormalPosition.left + AfxGetApp()->GetProfileInt(RegKey(), "CX", 0);
    wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + AfxGetApp()->GetProfileInt(RegKey(), "CY", 0);

    if (bMax)
        wp.showCmd = SW_MAXIMIZE;
    else
        wp.showCmd = (AfxGetApp()->GetProfileInt(RegKey(), "Max", FALSE) == 0) ? SW_SHOW : SW_MAXIMIZE;

    bMax = AfxGetApp()->GetProfileInt(RegKey(), "Max", FALSE);
    if ((wp.rcNormalPosition.left != 0) || (wp.rcNormalPosition.top != 0) || (wp.rcNormalPosition.right != 0) ||
        (wp.rcNormalPosition.bottom != 0))
        pWnd->SetWindowPlacement(&wp);
}

LPCTSTR
CFramePosition::RegKey()
{
    return "";
}

//////////////////////////////////////////////////////////////////////
// class CNoSizePosition
//////////////////////////////////////////////////////////////////////

CNoSizePosition::CNoSizePosition()
{
}

CNoSizePosition::~CNoSizePosition()
{
}

// Save the position of the frame, so next time
// we'll start with this point
void
CNoSizePosition::SavePosition(CWnd* pWnd)
{
    ASSERT(pWnd);
    ASSERT(AfxGetApp());

    CRect rc;
    pWnd->GetWindowRect(rc);
    AfxGetApp()->WriteProfileInt(RegKey(), "X", rc.left);
    AfxGetApp()->WriteProfileInt(RegKey(), "Y", rc.top);
}

// Load the previous position of the frame from the Registry
void
CNoSizePosition::LoadPosition(CWnd* pWnd)
{
    ASSERT(pWnd);
    ASSERT(AfxGetApp());

    int x = AfxGetApp()->GetProfileInt(RegKey(), "X", -1);
    int y = AfxGetApp()->GetProfileInt(RegKey(), "Y", -1);
    if (x > -1 && y > -1)
        ::SetWindowPos(pWnd->m_hWnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

LPCTSTR
CNoSizePosition::RegKey()
{
    return "";
}