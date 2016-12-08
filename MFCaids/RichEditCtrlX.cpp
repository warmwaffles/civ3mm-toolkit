/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 RichEditCtrlX.cpp
	CREATED:	 23.10.2003

	AUTHOR:		 Dreyk
	VERSION:     1.0

	DESCRIPTION: Modified rich edit control.

    Heavily based on the CAutoRichEditCtrl class by Jeremy Iverson (jeremy@dragonmount.net).

    -------------------------------------------------------------------------
	    Instructions on how to use CRichEditCtrlX in your application:
	-------------------------------------------------------------------------
	
    This article was contributed by Jeremy Iverson. 
    While MFC does provide a rich edit control, it does not provide a quick and easy 
	way to handle formatting and RTF codes. 
	CRichEditCtrlX is a derivative of CRichEditCtrl. 
	This new class adds several functions that allow you to add formatting quickly. 
	It also provides two functions to let you retrieve the formatted text (RTF) 
	in a CString, and to insert an RTF CString into the control. 

    To use CRichEditCtrlX in a dialog box, follow these instructions. 

    Insert a normal rich edit control in your dialog box. 
    Using ClassWizard, attach a control variable of type CRichEditCtrl to your control. 
    In your dialog's header, change CRichEditCtrl to CRichEditCtrlX. 
    In your application's InitInstance (CMyApp::InitInstance()), include a call to 
	AfxInitRichEdit() - this is needed for all rich edit controls. 
    Add interface elements that use CRichEditCtrlX's functions - see below. 
    Once you've inserted the control into your project, you can add toolbar buttons, 
	menu commands, or other elements to call the following functions: 

    GetRTF()                 - returns a CString holding the text of the control (including the RTF formatting code). 
    SetRTF(CString sRTF)     - parameter sRTF is an RTF formatted CString. Calling this will insert and format the string. If a CString is passed that does not have valid RTF code, the control will be cleared. 

    IsBold()                 - returns true if the current selection is bold, otherwise false. 
    IsItalic()               - returns true if the current selection is italic, else false. 
    IsUnderlined()           - returns true if the current selection is underlined, else false. 

    SetBold()                - sets the current selection to be bold (Note: this will actually act like pressing the Bold toolbar button in Wordpad - if the text is not bold, it will become bold; if the text is already bold, it will remove the bold formatting; if part of the selection is bold, the entire selection will become bold; if there is not anything selected, the format is changed to bold so when the user starts typing, it will be in bold). 
    SetItalic()              - sets the current selection to italic (see note in SetBold()). 
    SetUnderlined()          - sets the current selection to underlined (see note in SetBold()). 

    SetCenterAlign()         - sets the current paragraph to be centered. 
    SetLeftAlign()           - sets the current paragraph to be left justified. 
    SetRightAlign()          - sets the current paragraph to be right justified. 

    IsCenteredAlign()        - returns true if the current paragraph is centered, else false. 
    IsLeftAlign()            - returns true if the current paragraph is left justified, else false. 
    IsRightAlign()           - returns true if the current paragraph is right justified, else false. 

    SetBulleted()            - sets the bullet style (see note in SetBold()). 
    IsBulleted()             - returns true if the current selection is bulleted, else false. 

    SelectColor()            - displays a color selection dialog box and sets the selection to the chosen color (if OK is pressed). 

    SetFontName(CString sFontName) - sets the selected font to sFontName. 
    SetFontSize(int nPointSize)    - sets the selection to nPointSize. 

    GetSelectionFontName()   - returns a CString holding the selection's font name. 
    GetSelectionFontSize()   - returns a long holding the selection's font size. 

    GetSystemFonts(CStringArray &saFontList) - saFontList will hold an array of all true-type fonts available on the system. 

    GetCharFormat()          - returns the CHARFORMAT structure of the current selection. 
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "RichEditCtrlX.h"
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
// CRichEditCtrlX

CRichEditCtrlX::CRichEditCtrlX()
{
}

CRichEditCtrlX::~CRichEditCtrlX()
{
}

BEGIN_MESSAGE_MAP(CRichEditCtrlX, CRichEditCtrl)
//{{AFX_MSG_MAP(CRichEditCtrlX)
ON_WM_CONTEXTMENU()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlX message handlers

/////////////////////////////////////////////////////////////////////////////
// Return the RTF string of the text in the control.
//
CString
CRichEditCtrlX::GetRTF()
{
    CString sRTF;

    // Stream out here.
    EDITSTREAM es;
    es.dwError     = 0;
    es.pfnCallback = CBStreamOut;  // Set the callback
    es.dwCookie    = (DWORD)&sRTF; // so sRTF receives the string
    StreamOut(SF_RTF, es);         // Call CRichEditCtrl::StreamOut to get the string.

    return sRTF;
}

/////////////////////////////////////////////////////////////////////////////
// Put the RTF string sRTF into the rich edit control.
//
void
CRichEditCtrlX::SetRTF(CString sRTF)
{
    // Read the text in
    EDITSTREAM es;
    es.dwError     = 0;
    es.pfnCallback = CBStreamIn;
    es.dwCookie    = (DWORD)&sRTF;
    StreamIn(SF_RTF, es); // Do it.
}

/////////////////////////////////////////////////////////////////////////////
//	Callback function to stream an RTF string into the rich edit control.
//
DWORD CALLBACK
CRichEditCtrlX::CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
    // We insert the rich text here.

    //	This function taken from CodeGuru.com
    //	http://www.codeguru.com/richedit/rtf_string_streamin.shtml
    //	Zafir Anjum

    CString* pstr = (CString*)dwCookie;

    if (pstr->GetLength() < cb) {
        *pcb = pstr->GetLength();
        memcpy(pbBuff, (LPCSTR)*pstr, *pcb);
        pstr->Empty();
    } else {
        *pcb = cb;
        memcpy(pbBuff, (LPCSTR)*pstr, *pcb);
        *pstr = pstr->Right(pstr->GetLength() - cb);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//	Callback function to stream the RTF string out of the rich edit control.
//
DWORD CALLBACK
CRichEditCtrlX::CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
    // Address of our string var is in psEntry
    CString* psEntry = (CString*)dwCookie;

    CString tmpEntry;
    tmpEntry = (CString)pbBuff;

    // And write it!!!
    *psEntry += tmpEntry.Left(cb);

    return 0;
}

BOOL
CRichEditCtrlX::IsBold()
{
    CHARFORMAT cf = GetCharFormat();

    if (cf.dwEffects & CFM_BOLD)
        return TRUE;

    return FALSE;
}

BOOL
CRichEditCtrlX::IsItalic()
{
    CHARFORMAT cf = GetCharFormat();

    if (cf.dwEffects & CFM_ITALIC)
        return TRUE;

    return FALSE;
}

BOOL
CRichEditCtrlX::IsUnderlined()
{
    CHARFORMAT cf = GetCharFormat();

    if (cf.dwEffects & CFM_UNDERLINE)
        return TRUE;

    return FALSE;
}

BOOL
CRichEditCtrlX::IsLink()
{
    CHARFORMAT cf = GetCharFormat();

    if (cf.dwEffects & CFM_LINK)
        return TRUE;

    return FALSE;
}

CHARFORMAT
CRichEditCtrlX::GetCharFormat(DWORD dwMask)
{
    CHARFORMAT cf;
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = dwMask;

    GetSelectionCharFormat(cf);
    return cf;
}

void
CRichEditCtrlX::SetCharStyle(int MASK, int STYLE, int nStart, int nEnd)
{
    CHARFORMAT cf;
    cf.cbSize = sizeof(CHARFORMAT);
    //cf.dwMask = MASK;

    GetSelectionCharFormat(cf);

    if (cf.dwMask & MASK) // selection is all the same
        cf.dwEffects ^= STYLE;
    else
        cf.dwEffects |= STYLE;

    cf.dwMask = MASK;

    SetSelectionCharFormat(cf);
}

void
CRichEditCtrlX::SetBold()
{
    long start = 0, end = 0;
    GetSel(start, end); // Get the current selection

    SetCharStyle(CFM_BOLD, CFE_BOLD, start, end); // Make it bold
}

void
CRichEditCtrlX::SetItalic()
{
    long start = 0, end = 0;
    GetSel(start, end);

    SetCharStyle(CFM_ITALIC, CFE_ITALIC, start, end);
}

void
CRichEditCtrlX::SetUnderlined()
{
    long start = 0, end = 0;
    GetSel(start, end);

    SetCharStyle(CFM_UNDERLINE, CFE_UNDERLINE, start, end);
}

void
CRichEditCtrlX::SetLink()
{
    CHARFORMAT cf;
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_LINK;

    DWORD dwSelMask = GetSelectionCharFormat(cf);

    // If selection is all the same toggle PROTECTED style
    // turn it on otherwise over the whole selection
    if ((cf.dwMask & CFM_LINK) & (dwSelMask & CFM_LINK))
        cf.dwEffects ^= CFE_LINK;
    else
        cf.dwEffects |= CFE_LINK;

    cf.dwMask = CFM_LINK;
    SetSelectionCharFormat(cf);
}

void
CRichEditCtrlX::SetCenterAlign()
{
    PARAFORMAT paraFormat;
    paraFormat.cbSize     = sizeof(PARAFORMAT);
    paraFormat.dwMask     = PFM_ALIGNMENT;
    paraFormat.wAlignment = PFA_CENTER;

    SetParaFormat(paraFormat); // Set the paragraph.
}

void
CRichEditCtrlX::SetLeftAlign()
{
    PARAFORMAT paraFormat;
    paraFormat.cbSize     = sizeof(PARAFORMAT);
    paraFormat.dwMask     = PFM_ALIGNMENT;
    paraFormat.wAlignment = PFA_LEFT;

    SetParaFormat(paraFormat);
}

void
CRichEditCtrlX::SetRightAlign()
{
    PARAFORMAT paraFormat;
    paraFormat.cbSize     = sizeof(PARAFORMAT);
    paraFormat.dwMask     = PFM_ALIGNMENT;
    paraFormat.wAlignment = PFA_RIGHT;

    SetParaFormat(paraFormat);
}

BOOL
CRichEditCtrlX::IsCenteredAlign()
{
    PARAFORMAT pf = GetParagraphFormat();

    if (pf.wAlignment == PFA_CENTER)
        return TRUE;

    return FALSE;
}

BOOL
CRichEditCtrlX::IsLeftAlign()
{
    PARAFORMAT pf = GetParagraphFormat();

    if (pf.wAlignment == PFA_LEFT)
        return TRUE;

    return FALSE;
}

BOOL
CRichEditCtrlX::IsRightAlign()
{
    PARAFORMAT pf = GetParagraphFormat();

    if (pf.wAlignment == PFA_RIGHT)
        return TRUE;

    return FALSE;
}

PARAFORMAT
CRichEditCtrlX::GetParagraphFormat()
{
    PARAFORMAT pf;
    pf.cbSize = sizeof(PARAFORMAT);

    pf.dwMask = PFM_ALIGNMENT | PFM_NUMBERING;

    GetParaFormat(pf);

    return pf;
}

void
CRichEditCtrlX::SetBulleted()
{
    PARAFORMAT paraformat = GetParagraphFormat();

    if ((paraformat.dwMask & PFM_NUMBERING) && (paraformat.wNumbering == PFN_BULLET)) {
        paraformat.wNumbering    = 0;
        paraformat.dxOffset      = 0;
        paraformat.dxStartIndent = 0;
        paraformat.dwMask        = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
    } else {
        paraformat.wNumbering = PFN_BULLET;
        paraformat.dwMask     = PFM_NUMBERING;
        if (paraformat.dxOffset == 0) {
            paraformat.dxOffset = 4;
            paraformat.dwMask   = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
        }
    }

    SetParaFormat(paraformat);
}

BOOL
CRichEditCtrlX::IsBulleted()
{
    PARAFORMAT pf = GetParagraphFormat();

    if (pf.wNumbering == PFN_BULLET)
        return TRUE;

    return FALSE;
}

void
CRichEditCtrlX::SelectColor()
{
    CColorDialog dlg;

    CHARFORMAT cf = GetCharFormat();

    if (cf.dwEffects & CFE_AUTOCOLOR)
        cf.dwEffects -= CFE_AUTOCOLOR;

    // Get a color from the common color dialog.
    if (dlg.DoModal() == IDOK)
        cf.crTextColor = dlg.GetColor();

    cf.dwMask = CFM_COLOR;

    SetSelectionCharFormat(cf);
}

void
CRichEditCtrlX::SetFontName(CString sFontName)
{
    CHARFORMAT cf = GetCharFormat();

    // Set the font name.
    for (int i           = 0; i <= sFontName.GetLength() - 1; i++)
        cf.szFaceName[i] = sFontName[i];

    cf.dwMask = CFM_FACE;

    SetSelectionCharFormat(cf);
}

void
CRichEditCtrlX::SetFontSize(int nPointSize)
{
    CHARFORMAT cf = GetCharFormat();

    nPointSize *= 20; // convert from to twips
    cf.yHeight = nPointSize;
    cf.dwMask  = CFM_SIZE;

    SetSelectionCharFormat(cf);
}

void
CRichEditCtrlX::GetSystemFonts(CStringArray& saFontList)
{
    CDC* pDC = GetDC();

    EnumFonts(pDC->GetSafeHdc(), NULL, (FONTENUMPROC)CBEnumFonts, (LPARAM)&saFontList); //Enumerate
}

/////////////////////////////////////////////////////////////////////////////
// This function was written with the help of CCustComboBox, by Girish Bharadwaj.
// Available from Codeguru.
//
BOOL CALLBACK
CRichEditCtrlX::CBEnumFonts(LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)
{
    if (dwType == TRUETYPE_FONTTYPE)
        ((CStringArray*)lpData)->Add(lplf->lfFaceName);

    return TRUE;
}

CString
CRichEditCtrlX::GetSelectionFontName()
{
    CHARFORMAT cf = GetCharFormat();

    CString sName = cf.szFaceName;

    return sName;
}

long
CRichEditCtrlX::GetSelectionFontSize()
{
    CHARFORMAT cf = GetCharFormat();

    long nSize = cf.yHeight / 20;

    return nSize;
}

void
CRichEditCtrlX::OnContextMenu(CWnd* pWnd, CPoint point)
{
    UNREFERENCED_PARAMETER(pWnd);

    SetFocus();

    CNewMenu menu;
    menu.CreatePopupMenu();
    BOOL bReadOnly = GetStyle() & ES_READONLY;
    DWORD flags    = CanUndo() && !bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(0, MF_BYPOSITION | flags, EM_UNDO, MES_UNDO);

    menu.InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR);

    long nStartChar;
    long nEndChar;
    GetSel(nStartChar, nEndChar);

    flags = nStartChar == nEndChar ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_COPY, MES_COPY);

    flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_CUT, MES_CUT);
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_CLEAR, MES_DELETE);

    flags = IsClipboardFormatAvailable(CF_TEXT) && !bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_PASTE, MES_PASTE);

    menu.InsertMenu(6, MF_BYPOSITION | MF_SEPARATOR);

    int len = GetWindowTextLength();
    flags   = (!len || (nStartChar == 0 && nEndChar == len)) ? MF_GRAYED : 0;
    menu.InsertMenu(7, MF_BYPOSITION | flags, ME_SELECTALL, MES_SELECTALL);

    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        //		menu.SetMenuTitle(_T("Edit commands"), MFT_GRADIENT | MFT_SIDE_TITLE);
        menu.LoadToolBar(EditId, RGB_BACKGROUND);
    }

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}

BOOL
CRichEditCtrlX::OnCommand(WPARAM wParam, LPARAM lParam)
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
            return CRichEditCtrl::OnCommand(wParam, lParam);
    }
}
