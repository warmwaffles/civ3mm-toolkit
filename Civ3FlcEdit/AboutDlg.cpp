/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	AboutDlg.cpp
	CREATED:	14.3.2003

	AUTHOR:		Dreyk
	MODIFIED:	04.06.2004

	DESCRIPTION: CAboutDlg dialog used for App About
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CY_SHADOW 4
#define CX_SHADOW 4
#define CY_INDENT 10
#define CX_INDENT 10

#define BLINK_TIMER 753

static int CX_CLIENT = 40; // new width of the icon
static int CY_CLIENT = 40; // new height of the icon

// BONA FAMA DIVITIS EST POTIOR // добрая слава лучше богатства (Публий Сир)
// FECI, QUOD POTUI, FACIANT MELIORA POTENTES // я сделал всё, что мог, пусть тот, кто может, сделает лучше.

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg()
  : CDialogX(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    m_sFileVersion = _T("");
    m_sCopyright   = _T("");
    //}}AFX_DATA_INIT

    SetControlInfo(IDC_STATIC_BLINK, CF_RESIZE_HOR);
    SetControlInfo(IDC_VIEW_ICON, CF_ANCHOR_RIGHT);

    SetControlInfo(IDC_FILE_VERSION, CF_RESIZE_HOR);
    SetControlInfo(IDC_COPYRIGHT, CF_RESIZE_HOR);

    SetControlInfo(IDC_STATIC_1, CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_2, CF_RESIZE_BOTH);
    SetControlInfo(IDC_EDIT_NOTE, CF_RESIZE_BOTH);

    SetControlInfo(IDC_STATIC_3, CF_ANCHOR_BOTTOM);
    SetControlInfo(IDC_STATIC_4, CF_ANCHOR_BOTTOM);
    SetControlInfo(IDC_STATIC_GO_URL, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR);
    SetControlInfo(IDC_STATIC_MAIL_TO, CF_ANCHOR_BOTTOM | CF_RESIZE_HOR);

    SetControlInfo(IDOK, CF_ANCHOR_BOTTOM | CF_ANCHOR_RIGHT);
    m_bThanksShowed = FALSE;
}

void
CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Control(pDX, IDC_ANIMATE_CTRL, m_wndAnimate);
    DDX_Control(pDX, IDC_EDIT_NOTE, m_wndNote);
    DDX_Control(pDX, IDC_STATIC_BLINK, m_wndBlink);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Text(pDX, IDC_FILE_VERSION, m_sFileVersion);
    DDX_Text(pDX, IDC_COPYRIGHT, m_sCopyright);
    DDX_Control(pDX, IDC_STATIC_GO_URL, m_HyperLink);
    DDX_Control(pDX, IDC_STATIC_MAIL_TO, m_MailTo);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogX)
//{{AFX_MSG_MAP(CAboutDlg)
ON_WM_PAINT()
ON_WM_TIMER()
ON_WM_DESTROY()
ON_WM_SIZE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL
CAboutDlg::OnInitDialog()
{
    CDialogX::OnInitDialog();

    LoadPosition(this);

    // Set icon to the dialog caption
    SetIcon(AfxGetApp()->LoadIcon(IDI_ABOUT), FALSE);

    m_btnOK.SetIcon(IDI_EXIT);

    m_wndNote.SetBackgroundColor(FALSE, ::GetSysColor(COLOR_BTNFACE));

    // Set RTF text from resources to the rich edit control
    ShowTextResource("LicenseRTF");

    /*	/////////////////////////////////////////////////////
	// Test - set link...
	m_wndNote.SetEventMask(ENM_LINK);// Enable linking

	// Set the selection to be the first occurrence of the 
	// string lpszmyString, if it is found.   
	FINDTEXTEX ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = -1;
	ft.lpstrText  = (LPSTR)"Civ3MM";

	if (m_wndNote.FindText(FR_MATCHCASE|FR_WHOLEWORD, &ft) != -1)
	{
		m_wndNote.SetSel(ft.chrgText);
		m_wndNote.SetLink();
	}
*/ //////////////////////////////////////////////////////

    /////////////////////////////////////////////
    // Set information about application
    HINSTANCE hDllInstance = AfxGetResourceHandle();
    HINSTANCE hAppInstance = AfxGetResourceHandle();

    DWORD dwLength, dwSize, dwNullHandle;
    TCHAR szFileName[MAX_PATH];
    LPVOID lpVersionPtr;
    UINT uiVerLength;

    dwSize = sizeof(szFileName) / sizeof(szFileName[0]);

    AfxSetResourceHandle(hAppInstance);
    dwLength = ::GetModuleFileName(AfxGetResourceHandle(), szFileName, dwSize);
    AfxSetResourceHandle(hDllInstance);

    if (dwLength <= 0)
        return FALSE;

    szFileName;
    dwLength = ::GetFileVersionInfoSize(szFileName, &dwNullHandle);
    if (dwLength <= 0)
        return FALSE;

    BYTE* pProductInfo = new BYTE[dwLength];
    if (!::GetFileVersionInfo(szFileName, NULL, dwLength, pProductInfo))
        return FALSE;

    if (!::VerQueryValue(pProductInfo, _T ("\\"), &lpVersionPtr, &uiVerLength))
        return FALSE;

    LPVOID lpPtr;
    CString sQuery;

    typedef struct tagProductInformation
    {
        //	CString sFilePath;
        //	CString	sCompany;
        //	CString	sCopyRight;
        //	CString	sDescription;
        //	CString	sFileVersion;
        //	CString	sInternalName;
        //	CString	sOriginalName;
        //	CString	sProductName;
        //	CString	sProductVersion;
        //	CString sRegisteredUserName;
        //	CString sRegisteredCompanyName;
        struct TRANSLATE
        {
            WORD languageId;
            WORD characterSet;
        } Translation;
    } PRODUCT_INFORMATION, *LPPRODUCT_INFORMATION;

    PRODUCT_INFORMATION ProductInfo;

    if (::VerQueryValue(pProductInfo, "\\VarFileInfo\\Translation", &lpPtr, &uiVerLength))
        ProductInfo.Translation = *(PRODUCT_INFORMATION::TRANSLATE*)lpPtr;

    // Get file version
    sQuery.Format(
      _T ("\\StringFileInfo\\%04x%04x\\ProductVersion"), ProductInfo.Translation.languageId, ProductInfo.Translation.characterSet);
    ::VerQueryValue(pProductInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr, &uiVerLength);
    m_sFileVersion = uFormatString(IDS_BUILD_NUMBER, (LPCTSTR)lpPtr);

    // Comments
    sQuery.Format(_T ("\\StringFileInfo\\%04x%04x\\Comments"), ProductInfo.Translation.languageId, ProductInfo.Translation.characterSet);
    ::VerQueryValue(pProductInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr, &uiVerLength);
    sQuery = (LPCTSTR)lpPtr;
    if (!sQuery.IsEmpty())
        m_sFileVersion += " (" + sQuery + ")";

    // Get copyright
    sQuery.Format(
      _T ("\\StringFileInfo\\%04x%04x\\LegalCopyRight"), ProductInfo.Translation.languageId, ProductInfo.Translation.characterSet);
    ::VerQueryValue(pProductInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr, &uiVerLength);
    m_sCopyright = (LPCTSTR)lpPtr;

    delete[] pProductInfo;

    // Set HyperLink for Home Page:
    m_HyperLink.SetURL(uLoadString(IDS_CIVFANATICS_FORUM_URL));
    m_HyperLink.SetUnderline(TRUE);
    m_HyperLink.m_bIsURL = TRUE;

    // Set HyperLink for Mail To:
    m_MailTo.SetURL(uLoadString(IDS_EMAIL_ADDRESS));
    m_MailTo.SetUnderline(TRUE);

    SetTimer(BLINK_TIMER, 50, 0);

#ifdef _DEBUG
    m_sFileVersion += " [DEBUG]";
#endif

    UpdateData(FALSE);

    if (m_wndAnimate.Open(IDR_AVI_CYBERLION))
        m_wndAnimate.Play(0, 0xFFFF, 1); // Play once

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// Drawing a multicoloured text.
//
void
CAboutDlg::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == BLINK_TIMER) {
        CString strTxt(uLoadString(IDS_ABOUT_DLG_BLINK));
        static int nColor = 255;
        static int nDelta = -10;

        if (nColor < 30)
            nDelta = 10;
        else if (nColor > 245)
            nDelta = -10;

        nColor += nDelta;

        CRect rc;
        HDC hdc;
        CDC dc;
        CDC dcMem;
        CBitmap bmp;

        m_wndBlink.GetClientRect(&rc);

        hdc = ::GetDC(m_wndBlink.m_hWnd);

        if (!hdc)
            return;

        dc.Attach(hdc);

        dcMem.CreateCompatibleDC(&dc);
        bmp.CreateCompatibleBitmap(&dc, rc.right, rc.bottom);

        // Select the bitmap into the in-memory DC
        CBitmap* pOldBitmap = dcMem.SelectObject(&bmp);

        dcMem.FillSolidRect(&rc, GetSysColor(COLOR_BTNFACE));
        dcMem.SetBkMode(TRANSPARENT);

        CFont font;
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));      // zero out structure
        lf.lfHeight = 12;                     // request a 12-pixel-height font
        strcpy(lf.lfFaceName, "Arial");       // request a face name "Arial"
        VERIFY(font.CreateFontIndirect(&lf)); // create the font

        dcMem.SetTextColor(GetSysColor(COLOR_3DSHADOW));
        CFont* def_font = dc.SelectObject(&font);

        rc.DeflateRect(1, 2);
        dcMem.DrawText(strTxt, -1, &rc, DT_CENTER);

        dcMem.SetTextColor(RGB(0, 0, nColor));
        rc.InflateRect(1, 2);
        dcMem.DrawText(strTxt, -1, &rc, DT_CENTER);

        dc.SelectObject(def_font);

        // Done with the font. Delete the font object.
        font.DeleteObject();

        dc.BitBlt(0, 0, rc.right, rc.bottom, &dcMem, 0, 0, SRCCOPY);
        dcMem.SelectObject(pOldBitmap);

        dcMem.DeleteDC();
        dc.Detach();
        ::ReleaseDC(m_wndBlink.m_hWnd, hdc);
        bmp.DeleteObject();
    }

    CDialogX::OnTimer(nIDEvent);
}

void
CAboutDlg::OnDestroy()
{
    SavePosition(this);
    KillTimer(BLINK_TIMER);
    CDialogX::OnDestroy();
}

void
CAboutDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    /*	CDC* pDC = GetDC();
	ASSERT(pDC != NULL);

	CRect rect;
	
	CDC dcMem;
	if (!dcMem.CreateCompatibleDC(pDC))
		return;
	
	HICON hicon;
	CBitmap bitmap;
	CBitmap* pBitmapOld;

	CPen pen;
	pen.CreateStockObject(BLACK_PEN);

	CBrush br;
	br.CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));

	CPen* pPenOld = pDC->SelectObject(&pen);
	
	// Draw icon into off-screen bitmap
	int cxIcon = ::GetSystemMetrics(SM_CXICON);
	int cyIcon = ::GetSystemMetrics(SM_CYICON);

	//////////////////////////////////////////////////////
	// Draw left icon

	// Load icon
	hicon = AfxGetApp()->LoadIcon(IDR_LOGO);
	if (hicon == NULL)
		return;

	if (!bitmap.CreateCompatibleBitmap(pDC, cxIcon, cyIcon))
		return;
	
	pBitmapOld = dcMem.SelectObject(&bitmap);
	if (pBitmapOld == NULL)
		return;

	// Blt the bits already on the window onto the off-screen bitmap
	dcMem.StretchBlt(0, 0, cxIcon, cyIcon, pDC,
		2, 2, CX_CLIENT-CX_SHADOW-4, CY_CLIENT-CY_SHADOW-4, SRCCOPY);

	// Draw the icon on the background
	dcMem.DrawIcon(0, 0, hicon);

	// Draw border around icon
	pDC->Rectangle(CX_INDENT, CY_INDENT, CX_INDENT+CX_CLIENT-CX_SHADOW, CY_INDENT+CY_CLIENT-CY_SHADOW);
	if (pPenOld)
		pDC->SelectObject(pPenOld);

	// Draw shadows around icon
	rect.SetRect(CX_INDENT+CX_CLIENT-CX_SHADOW, CY_INDENT+CY_SHADOW, CX_INDENT+CX_CLIENT, CY_INDENT+CY_CLIENT);
	pDC->FillRect(rect, &br);
	rect.SetRect(CX_INDENT+CX_SHADOW, CY_INDENT+CY_CLIENT-CY_SHADOW, CX_INDENT+CX_CLIENT, CY_INDENT+CY_CLIENT);
	pDC->FillRect(rect, &br);

	// Draw the icon contents
	pDC->StretchBlt(CX_INDENT+2, CY_INDENT+2, CX_CLIENT-CX_SHADOW-4, CY_CLIENT-CY_SHADOW-4,
		&dcMem, 0, 0, cxIcon, cyIcon, SRCCOPY);

	//////////////////////////////////////////////////////
	// Draw right icon
	GetClientRect(rect);
	int nCX = rect.Width() - (CX_CLIENT + CX_INDENT);// + CX_SHADOW);

	bitmap.DeleteObject();

	// Load icon
	hicon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (hicon == NULL)
		return;

	if (!bitmap.CreateCompatibleBitmap(pDC, cxIcon, cyIcon))
		return;
	
	pBitmapOld = dcMem.SelectObject(&bitmap);
	if (pBitmapOld == NULL)
		return;

	// Blt the bits already on the window onto the off-screen bitmap
	dcMem.StretchBlt(0, 0, cxIcon, cyIcon, pDC,
		2, 2, CX_CLIENT-CX_SHADOW-4, CY_CLIENT-CY_SHADOW-4, SRCCOPY);

	// Draw the icon on the background
	dcMem.DrawIcon(0, 0, hicon);

	// Draw border around icon
	pPenOld = pDC->SelectObject(&pen);
	pDC->Rectangle(nCX, CY_INDENT, nCX+CX_CLIENT-CX_SHADOW, CY_INDENT+CY_CLIENT-CY_SHADOW);
	pDC->SelectObject(pPenOld);

	// Draw shadows around icon
	rect.SetRect(nCX+CX_CLIENT-CX_SHADOW, CY_INDENT+CY_SHADOW, nCX+CX_CLIENT, CY_INDENT+CY_CLIENT);
	pDC->FillRect(rect, &br);
	rect.SetRect(nCX+CX_SHADOW, CY_INDENT+CY_CLIENT-CY_SHADOW, nCX+CX_CLIENT, CY_INDENT+CY_CLIENT);
	pDC->FillRect(rect, &br);

	// Draw the icon contents
	pDC->StretchBlt(nCX+2, CY_INDENT+2, CX_CLIENT-CX_SHADOW-4, CY_CLIENT-CY_SHADOW-4,
		&dcMem, 0, 0, cxIcon, cyIcon, SRCCOPY);
*/
}

void
CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogX::OnSize(nType, cx, cy);

    if (m_wndBlink.GetSafeHwnd())
        Invalidate();
}

BOOL
CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        // To show Eastern Egg
        if (!m_bThanksShowed) {
            switch (pMsg->wParam) {
                case 'T':
                    m_sThanks = 'T';
                    break;
                case 'H':
                    m_sThanks += 'H';
                    break;
                case 'A':
                    m_sThanks += 'A';
                    break;
                case 'N':
                    m_sThanks += 'N';
                    break;
                case 'K':
                    m_sThanks += 'K';
                    break;
                case 'S':
                    m_sThanks += 'S';
                    if (m_sThanks.Compare("THANKS") == 0) {
                        ShowTextResource("ThanksRTF");
                        m_bThanksShowed = TRUE;
                    }

                    m_sThanks.Empty();
                    break;

                default:
                    m_sThanks.Empty();
            }
        }

        //		// "Shift + T" - show Eastern Egg
        //		if (pMsg->wParam == 'T' && GetKeyState(VK_SHIFT))
        //		{
        //			ShowTextResource("ThanksRTF");
        //		}
    }

    return CDialogX::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////
// Set RTF text from resources to the rich edit control
//
void
CAboutDlg::ShowTextResource(LPCTSTR lpName)
{
    HINSTANCE hAppInstance = AfxGetResourceHandle();

    HRSRC hLicFile  = ::FindResource(hAppInstance, lpName, "TEXT_RESOURCE");
    HGLOBAL hRes    = ::LoadResource(hAppInstance, hLicFile);
    DWORD dwResSize = ::SizeofResource(hAppInstance, hLicFile);

    if (!hRes)
        return;

    UINT FAR* lpnRes = (UINT FAR*)::LockResource(hRes);
    BYTE* aBuf       = new BYTE[dwResSize + 1];
    memcpy(aBuf, lpnRes, dwResSize);
    aBuf[dwResSize] = 0;
    ::UnlockResource(hRes);

    CString sNote = aBuf;
    m_wndNote.SetRTF(sNote);

    delete[] aBuf;
}
