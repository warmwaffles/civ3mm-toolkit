/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 EditX.cpp
	CREATED:	 19.5.2003

	AUTHOR:		 Dreyk
	VERSION:     1.3

	DESCRIPTION: Modified edit control.

    Based on the 
	CMenuEdit class of PJ Arends (pja@telus.net http://www3.telus.net/pja/) and
	CNewEdit class of Bruno Podetti (Podetti@gmx.net).
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "EditX.h"
#include "resource.h"

#include "MFCdefs.h"
#include "Utilities.h"
#include "NewMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MES_UNDO _T(uLocalGetShort(ID_EDIT_UNDO))
#define MES_CUT _T(uLocalGetShort(ID_EDIT_CUT))
#define MES_COPY _T(uLocalGetShort(ID_EDIT_COPY))
#define MES_PASTE _T(uLocalGetShort(ID_EDIT_PASTE))
#define MES_DELETE _T(uLocalGetShort(ID_EDIT_CLEAR))
#define MES_SELECTALL _T(uLocalGetShort(ID_EDIT_SELECT_ALL))
#define ME_SELECTALL WM_USER + 0x7000

static WORD EditId[] = { IDB_EDIT,     // resource ID
                         16,       16, // icons size
                         EM_UNDO,  WM_CUT, WM_COPY, WM_PASTE, WM_CLEAR, ME_SELECTALL, NULL };

/////////////////////////////////////////////////////////////////////////////
// CEditX

CEditX::CEditX()
{
}

CEditX::~CEditX()
{
}

BEGIN_MESSAGE_MAP(CEditX, CEdit)
//{{AFX_MSG_MAP(CEditX)
ON_WM_CONTEXTMENU()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void
CEditX::OnContextMenu(CWnd* pWnd, CPoint point)
{
    UNREFERENCED_PARAMETER(pWnd);

    SetFocus();

    CNewMenu menu;
    menu.CreatePopupMenu();
    BOOL bReadOnly = GetStyle() & ES_READONLY;
    DWORD flags    = CanUndo() && !bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(0, MF_BYPOSITION | flags, EM_UNDO, MES_UNDO);

    menu.InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR);

    DWORD sel = GetSel();
    flags     = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_COPY, MES_COPY);

    flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_CUT, MES_CUT);
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_CLEAR, MES_DELETE);

    flags = IsClipboardFormatAvailable(CF_TEXT) && !bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_PASTE, MES_PASTE);

    menu.InsertMenu(6, MF_BYPOSITION | MF_SEPARATOR);

    int len = GetWindowTextLength();
    flags   = (!len || (LOWORD(sel) == 0 && HIWORD(sel) == len)) ? MF_GRAYED : 0;
    menu.InsertMenu(7, MF_BYPOSITION | flags, ME_SELECTALL, MES_SELECTALL);

    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        //		menu.SetMenuTitle(_T("Edit commands"), MFT_GRADIENT | MFT_SIDE_TITLE);
        menu.LoadToolBar(EditId, RGB_BACKGROUND);
    }

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}

BOOL
CEditX::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam)) {
        case EM_UNDO:
        case WM_CUT:
        case WM_COPY:
        case WM_CLEAR:
        case WM_PASTE:
            return SendMessage(LOWORD(wParam));
        case ME_SELECTALL:
            return SendMessage(EM_SETSEL, 0, -1);
        default:
            return CEdit::OnCommand(wParam, lParam);
    }
}

BOOL
CEditX::PreTranslateMessage(MSG* pMsg)
{
    switch (pMsg->message) {
        case WM_KEYDOWN:
            if (pMsg->wParam == VK_RETURN) // click ENTER
            {
                DWORD dwStyle = GetStyle();
                if (!(dwStyle & ES_WANTRETURN)) {
                    DWORD dwSel = GetSel();

                    CWnd* pWnd = GetParent();
                    if (pWnd && pWnd->GetSafeHwnd())
                        pWnd->SetFocus();

                    if (GetSafeHwnd()) {
                        SetFocus();
                        SetSel(HIWORD(dwSel), LOWORD(dwSel));
                    }
                }
                break;
            } else if (GetKeyState(VK_CONTROL) < 0) {
                if (pMsg->wParam == 'A') {
                    SendMessage(EM_SETSEL, 0, -1);
                    return TRUE;
                }
                /*			
			switch (pMsg->wParam)
			{
			case 'Z':
				SendMessage(EM_UNDO);
				break;

			case 'X':
				SendMessage(WM_CUT);
				break;
				
			case 'C':
				SendMessage(WM_COPY);
				break;
				
			case 'P':
			case 'V':
				SendMessage(WM_PASTE);
				break;
				
			case 'A':
				SendMessage(EM_SETSEL, 0, -1);
				break;				
			}
*/
            }
    }
    return CEdit::PreTranslateMessage(pMsg);
}
