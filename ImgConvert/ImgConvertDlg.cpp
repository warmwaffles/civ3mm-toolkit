/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ImgConvertDlg.cpp
	CREATED:	12.01.2005
	AUTHOR:		Dreyk
	VERSION:	1,2

	DESCRIPTION: CImgConvertDlg dialog assigned to easily renaming and 
	conversion of image files.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

// Civ3 Modemaker Tool Kit

#include "stdafx.h"
#include "ImgConvert.h"
#include "ImgConvertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\MFCaids\FileDialogX.h"
#include "..\MFCaids\GraphXImage.h"

#define REG_SRC_PATH "Src Path"
#define REG_DST_PATH "Dst Path"
#define REG_USE_FILE_TYPE "File Type"
#define REG_USE_BITS_PER_PIXELS "Bits Per Pixel"
#define REG_USE_SIZE "Size"
#define REG_USE_SUFFIX "Suffix"
#define REG_USE_PROP_RESIZE "Prop Resize"
#define REG_USE_CIV_PAL "Civ Pal"
#define REG_USE_FRM "Frame Convert"

#define REG_LAST_FILE_TYPE "Last File Type"
#define REG_BITS_PER_PIXELS "Last File BpP"
#define REG_LAST_WIDTH "Last Width"
#define REG_LAST_HEYGTH "Last Height"
#define REG_LAST_LOCK "Lock"
#define REG_LAST_SUFFIX "Last Suffix"

#define REG_LAST_FRM_CX "Last Frm CX"
#define REG_LAST_FRM_CY "Last Frm CY"
#define REG_LAST_IMG_CX "Last Img CX"
#define REG_LAST_IMG_CY "Last Img CY"
#define REG_LAST_FRM_TYPE "Last Frm Type"
#define REG_LAST_FRM_PATH "Last Frm Path"

//static COLORREF m_colPink = RGB(255,0,255);
//static COLORREF m_colGreen = RGB(0,255,0);

#define RGB_PINK 0xFF00FF
#define RGB_GREEN 0x00FF00
#define COLOR8 256L

typedef struct _RGBQUADPAL
{
    RGBQUAD pal[COLOR8];
} RGBQUADPAL; // Total size: 1024 bytes

#define RGBQUADPAL_SIZE sizeof(RGBQUADPAL)

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogX
{
  public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum
    {
        IDD = IDD_ABOUTBOX
    };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg()
  : CDialogX(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void
CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogX)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgConvertDlg dialog

CImgConvertDlg::CImgConvertDlg(CWnd* pParent /*=NULL*/)
  : CDialogX(CImgConvertDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CImgConvertDlg)
    m_nHeight          = 32;
    m_nWidth           = 32;
    m_bLock            = TRUE;
    m_sTargetPath      = _T("");
    m_bUseBitsPerPixel = FALSE;
    m_bUseFileType     = FALSE;
    m_bUseSize         = FALSE;
    m_bUseSuffix       = FALSE;
    m_sSuffix          = _T("");
    m_bUsePropResize   = FALSE;
    m_bCivPalette      = FALSE;
    m_nFrmCX           = 0;
    m_nFrmCY           = 0;
    m_sFrmPath         = _T("");
    m_nImgCX           = 0;
    m_nImgCY           = 0;
    m_bUseFrm          = FALSE;
    m_nFrmConvertType  = 0;
    //}}AFX_DATA_INIT

    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon        = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bDrawGripper = FALSE;
}

void
CImgConvertDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CImgConvertDlg)
    DDX_Control(pDX, IDC_USE_SIZE, m_wndUseSize);
    DDX_Control(pDX, IDC_IMG_CX, m_wndImgCX);
    DDX_Control(pDX, IDC_IMG_CY, m_wndImgCY);
    DDX_Control(pDX, IDC_FRM_TARGET_PATH, m_wndFrmPath);
    DDX_Control(pDX, IDC_FRM_CX, m_wndFrmCX);
    DDX_Control(pDX, IDC_FRM_CY, m_wndFrmCY);
    DDX_Control(pDX, IDC_FRM_BROWSE_TARGET_PATH, m_btnFrmBrowse);
    DDX_Control(pDX, IDC_CIV_PAL, m_wndCivPalette);
    DDX_Control(pDX, IDC_USE_BPP, m_wndUseBitsPerPixel);
    DDX_Control(pDX, IDC_USE_PROP_RESIZE, m_wndUsePropResize);
    DDX_Control(pDX, IDC_SUFFIX, m_wndSuffix);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDC_TARGET_PATH, m_wndTargetPath);
    DDX_Control(pDX, IDC_BROWSE_TARGET_PATH, m_btnBrowseTargetPath);
    DDX_Control(pDX, IDC_LOCK, m_btnLock);
    DDX_Control(pDX, IDC_FRM_WIDTH_SPIN, m_wndWidthSpin);
    DDX_Control(pDX, IDC_FRM_WIDTH, m_wndWidth);
    DDX_Control(pDX, IDC_FRM_HEIGHT_SPIN, m_wndHeightSpin);
    DDX_Control(pDX, IDC_FRM_HEIGHT, m_wndHeight);
    DDX_Text(pDX, IDC_FRM_HEIGHT, m_nHeight);
    DDX_Text(pDX, IDC_FRM_WIDTH, m_nWidth);
    DDX_Check(pDX, IDC_LOCK, m_bLock);
    DDX_Control(pDX, IDC_BITS_PER_PIXEL, m_wndBitsPerPixel);
    DDX_Control(pDX, IDC_FILE_TYPE, m_wndFileType);
    DDX_Control(pDX, IDC_BROWSE_PATH, m_btnBrowse);
    DDX_Text(pDX, IDC_TARGET_PATH, m_sTargetPath);
    DDX_Check(pDX, IDC_USE_BPP, m_bUseBitsPerPixel);
    DDX_Check(pDX, IDC_USE_FILE_TYPE, m_bUseFileType);
    DDX_Check(pDX, IDC_USE_SIZE, m_bUseSize);
    DDX_Check(pDX, IDC_USE_SUFFIX, m_bUseSuffix);
    DDX_Text(pDX, IDC_SUFFIX, m_sSuffix);
    DDX_Check(pDX, IDC_USE_PROP_RESIZE, m_bUsePropResize);
    DDX_Check(pDX, IDC_CIV_PAL, m_bCivPalette);
    DDX_Text(pDX, IDC_FRM_CX, m_nFrmCX);
    DDX_Text(pDX, IDC_FRM_CY, m_nFrmCY);
    DDX_Text(pDX, IDC_FRM_TARGET_PATH, m_sFrmPath);
    DDX_Text(pDX, IDC_IMG_CX, m_nImgCX);
    DDX_Text(pDX, IDC_IMG_CY, m_nImgCY);
    DDX_Check(pDX, IDC_USE_FRM, m_bUseFrm);
    DDX_Radio(pDX, IDC_FRM_CUT, m_nFrmConvertType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImgConvertDlg, CDialogX)
//{{AFX_MSG_MAP(CImgConvertDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_LOCK, OnLock)
ON_EN_CHANGE(IDC_FRM_HEIGHT, OnChangeHeight)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_HEIGHT_SPIN, OnDeltaposHeightSpin)
ON_EN_CHANGE(IDC_FRM_WIDTH, OnChangeWidth)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_WIDTH_SPIN, OnDeltaposWidthSpin)
ON_BN_CLICKED(IDC_BROWSE_PATH, OnBrowsePath)
ON_CBN_SELENDOK(IDC_FILE_TYPE, OnSelendokFileType)
ON_BN_CLICKED(IDC_USE_SIZE, OnChanged)
ON_BN_CLICKED(IDC_BROWSE_TARGET_PATH, OnBrowseTargetPath)
ON_CBN_SELENDOK(IDC_BITS_PER_PIXEL, OnSelendokBitsPerPixel)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_CIV_PAL, OnCivPal)
ON_BN_CLICKED(IDC_FRM_BROWSE_TARGET_PATH, OnFrmBrowseTargetPath)
ON_BN_CLICKED(IDC_USE_FILE_TYPE, OnChanged)
ON_BN_CLICKED(IDC_USE_BPP, OnChanged)
ON_BN_CLICKED(IDC_USE_SUFFIX, OnChanged)
ON_BN_CLICKED(IDC_USE_PROP_RESIZE, OnChanged)
ON_BN_CLICKED(IDC_FRM_CUT, OnFrmTypeChanged)
ON_BN_CLICKED(IDC_FRM_PASTE, OnFrmTypeChanged)
ON_EN_CHANGE(IDC_FRM_CX, OnChangeFrmCx)
ON_EN_CHANGE(IDC_FRM_CY, OnChangeFrmCy)
ON_EN_CHANGE(IDC_IMG_CX, OnChangeImgCx)
ON_EN_CHANGE(IDC_IMG_CY, OnChangeImgCy)
ON_BN_CLICKED(IDC_USE_FRM, OnUseFrm)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgConvertDlg message handlers

BOOL
CImgConvertDlg::OnInitDialog()
{
    CDialogX::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    //////////////////////////////////////////////////////////////////////////
    LoadPosition(this);
    m_btnBrowse.SetIcon(IDI_BROWSE);
    m_btnBrowseTargetPath.SetIcon(IDI_BROWSE);
    m_btnFrmBrowse.SetIcon(IDI_BROWSE);
    m_btnOK.SetIcon(IDR_MAINFRAME);
    m_btnCancel.SetIcon(IDI_EXIT);

    CWinApp* pApp      = AfxGetApp();
    m_bUseFileType     = pApp->GetProfileInt(RegKey(), REG_USE_FILE_TYPE, 1);
    m_bUseBitsPerPixel = pApp->GetProfileInt(RegKey(), REG_USE_BITS_PER_PIXELS, 1);
    m_bUseSize         = pApp->GetProfileInt(RegKey(), REG_USE_SIZE, 1);
    m_sTargetPath      = pApp->GetProfileString(RegKey(), REG_DST_PATH, "");

    int nFileType      = pApp->GetProfileInt(RegKey(), REG_LAST_FILE_TYPE, 0);
    int nBitsPerPixels = pApp->GetProfileInt(RegKey(), REG_BITS_PER_PIXELS, 0);
    m_nWidth           = pApp->GetProfileInt(RegKey(), REG_LAST_WIDTH, 32);
    m_nHeight          = pApp->GetProfileInt(RegKey(), REG_LAST_HEYGTH, 32);
    m_bLock            = pApp->GetProfileInt(RegKey(), REG_LAST_LOCK, 1);

    m_bUseSuffix = pApp->GetProfileInt(RegKey(), REG_USE_SUFFIX, 0);
    m_sSuffix    = pApp->GetProfileString(RegKey(), REG_LAST_SUFFIX, "");

    m_bUsePropResize = pApp->GetProfileInt(RegKey(), REG_USE_PROP_RESIZE, 0);
    m_bCivPalette    = pApp->GetProfileInt(RegKey(), REG_USE_CIV_PAL, 0);

    m_bUseFrm         = pApp->GetProfileInt(RegKey(), REG_USE_FRM, 0);
    m_nFrmCX          = pApp->GetProfileInt(RegKey(), REG_LAST_FRM_CX, 0);
    m_nFrmCY          = pApp->GetProfileInt(RegKey(), REG_LAST_FRM_CY, 0);
    m_nImgCX          = pApp->GetProfileInt(RegKey(), REG_LAST_IMG_CX, 0);
    m_nImgCY          = pApp->GetProfileInt(RegKey(), REG_LAST_IMG_CY, 0);
    m_nFrmConvertType = pApp->GetProfileInt(RegKey(), REG_LAST_FRM_TYPE, 0);
    m_sFrmPath        = pApp->GetProfileString(RegKey(), REG_LAST_FRM_PATH, 0);

    m_wndFileType.AddString("PCX");
    m_wndFileType.AddString("BMP");
    m_wndFileType.AddString("JPG");
    m_wndFileType.SetCurSel(nFileType);

    m_wndBitsPerPixel.AddString(uLoadString(IDS_COLOR_256));
    m_wndBitsPerPixel.AddString(uLoadString(IDS_COLOR_TRUE));
    m_wndBitsPerPixel.SetCurSel(nBitsPerPixels);

    m_wndTargetPath.LimitText(MAX_PATH);
    m_wndWidth.LimitText(4);
    m_wndHeight.LimitText(4);
    m_wndSuffix.LimitText(32);
    m_wndFrmCX.LimitText(4);
    m_wndFrmCY.LimitText(4);
    m_wndImgCX.LimitText(4);
    m_wndImgCY.LimitText(4);

    m_wndWidthSpin.SetRange(1, 9999);
    m_wndHeightSpin.SetRange(1, 9999);

    m_nPropXtoY = (float)m_nWidth / (float)m_nHeight;
    m_nPropYtoX = (float)m_nHeight / (float)m_nWidth;

    m_btnLock.SetIcon(IDI_LOCK, IDI_UNLOCK);
    m_btnLock.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

    UpdateData(FALSE);
    UpdateControlsState();
    return TRUE;
}

void
CImgConvertDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialogX::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void
CImgConvertDlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialogX::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR
CImgConvertDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

void
CImgConvertDlg::OnLock()
{
    UpdateData();

    if (m_nPropXtoY > m_nPropYtoX)
        OnChangeHeight();
    else if (m_nPropYtoX > m_nPropXtoY)
        OnChangeWidth();
}

void
CImgConvertDlg::OnChangeHeight()
{
    if (m_wndHeight.GetSafeHwnd() && m_bLock) {
        UpdateData();

        CString res;
        res.Format("%.0f", (float)m_nHeight * m_nPropXtoY);
        m_nWidth = atoi(res);

        UpdateData(FALSE);
    }
}

void
CImgConvertDlg::OnDeltaposHeightSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeHeight();
    *pResult = 0;
}

void
CImgConvertDlg::OnChangeWidth()
{
    if (m_wndWidth.GetSafeHwnd() && m_bLock) {
        UpdateData();

        CString res;
        res.Format("%.0f", (float)m_nWidth * m_nPropYtoX);
        m_nHeight = atoi(res);

        UpdateData(FALSE);
    }
}

void
CImgConvertDlg::OnDeltaposWidthSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeWidth();
    *pResult = 0;
}

void
CImgConvertDlg::UpdateControlsState()
{
    UpdateData();

    m_wndFileType.EnableWindow(m_bUseFileType);

    if (m_bUseFileType && m_wndFileType.GetCurSel() == 2) // 2-JPG
    {
        m_wndUseBitsPerPixel.EnableWindow(FALSE);
        m_bUseBitsPerPixel = FALSE;

        m_wndCivPalette.EnableWindow(FALSE);
        m_bCivPalette = FALSE;

        UpdateData(FALSE);

        m_wndBitsPerPixel.EnableWindow(FALSE);
        m_wndBitsPerPixel.SetCurSel(1); // 24 bit
    } else {
        m_wndUseBitsPerPixel.EnableWindow(TRUE);
        m_wndBitsPerPixel.EnableWindow(m_bUseBitsPerPixel);
        m_wndCivPalette.EnableWindow(m_bUseBitsPerPixel && m_wndBitsPerPixel.GetCurSel() == 0);
    }

    m_wndFrmCX.EnableWindow(m_bUseFrm);
    m_wndFrmCY.EnableWindow(m_bUseFrm);
    m_wndImgCX.EnableWindow(m_bUseFrm);
    m_wndImgCY.EnableWindow(m_bUseFrm);

    GetDlgItem(IDC_FRM_CUT)->EnableWindow(m_bUseFrm);
    GetDlgItem(IDC_FRM_PASTE)->EnableWindow(m_bUseFrm);

    m_wndFrmPath.EnableWindow(m_bUseFrm && m_nFrmConvertType == 1);
    m_btnFrmBrowse.EnableWindow(m_bUseFrm && m_nFrmConvertType == 1);

    if (m_bUseFrm) {
        m_wndUseSize.SetCheck(0);
        m_wndUseSize.EnableWindow(FALSE);

        m_btnLock.EnableWindow(FALSE);
        m_wndWidthSpin.EnableWindow(FALSE);
        m_wndWidth.EnableWindow(FALSE);
        m_wndHeightSpin.EnableWindow(FALSE);
        m_wndHeight.EnableWindow(FALSE);
        m_wndUsePropResize.EnableWindow(FALSE);
    } else {
        m_wndUseSize.EnableWindow(TRUE);

        m_btnLock.EnableWindow(m_bUseSize);
        m_wndWidthSpin.EnableWindow(m_bUseSize);
        m_wndWidth.EnableWindow(m_bUseSize);
        m_wndHeightSpin.EnableWindow(m_bUseSize);
        m_wndHeight.EnableWindow(m_bUseSize);
        m_wndUsePropResize.EnableWindow(m_bUseSize);
    }

    m_wndSuffix.EnableWindow(m_bUseSuffix);

    m_btnOK.EnableWindow((m_bUseSize || m_bUseFileType) || (m_bUseBitsPerPixel || m_bUseSuffix) || m_bUseFrm);
}

void
CImgConvertDlg::OnSelendokFileType()
{
    UpdateControlsState();
}

void
CImgConvertDlg::OnSelendokBitsPerPixel()
{
    UpdateControlsState();
}

void
CImgConvertDlg::OnChanged()
{
    UpdateControlsState();
}

void
CImgConvertDlg::OnDestroy()
{
    SavePosition(this);

    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(RegKey(), REG_USE_FILE_TYPE, m_bUseFileType);
    pApp->WriteProfileInt(RegKey(), REG_USE_BITS_PER_PIXELS, m_bUseBitsPerPixel);
    pApp->WriteProfileInt(RegKey(), REG_USE_SIZE, m_bUseSize);
    pApp->WriteProfileString(RegKey(), REG_DST_PATH, m_sTargetPath);

    pApp->WriteProfileInt(RegKey(), REG_LAST_FILE_TYPE, m_wndFileType.GetCurSel());
    pApp->WriteProfileInt(RegKey(), REG_BITS_PER_PIXELS, m_wndBitsPerPixel.GetCurSel());
    pApp->WriteProfileInt(RegKey(), REG_LAST_WIDTH, m_nWidth);
    pApp->WriteProfileInt(RegKey(), REG_LAST_HEYGTH, m_nHeight);
    pApp->WriteProfileInt(RegKey(), REG_LAST_LOCK, m_bLock);

    pApp->WriteProfileInt(RegKey(), REG_USE_SUFFIX, m_bUseSuffix);
    pApp->WriteProfileString(RegKey(), REG_LAST_SUFFIX, m_sSuffix);

    pApp->WriteProfileInt(RegKey(), REG_USE_PROP_RESIZE, m_bUsePropResize);
    pApp->WriteProfileInt(RegKey(), REG_USE_CIV_PAL, m_bCivPalette);

    pApp->WriteProfileInt(RegKey(), REG_USE_FRM, m_bUseFrm);
    pApp->WriteProfileInt(RegKey(), REG_LAST_FRM_CX, m_nFrmCX);
    pApp->WriteProfileInt(RegKey(), REG_LAST_FRM_CY, m_nFrmCY);
    pApp->WriteProfileInt(RegKey(), REG_LAST_IMG_CX, m_nImgCX);
    pApp->WriteProfileInt(RegKey(), REG_LAST_IMG_CY, m_nImgCY);
    pApp->WriteProfileInt(RegKey(), REG_LAST_FRM_TYPE, m_nFrmConvertType);
    pApp->WriteProfileString(RegKey(), REG_LAST_FRM_PATH, m_sFrmPath);

    CDialogX::OnDestroy();
}

void
CImgConvertDlg::OnBrowseTargetPath()
{
    uBrowseFolder(this->m_hWnd, &m_sTargetPath, "Select Target Directory", BIF_DONTGOBELOWDOMAIN);
    UpdateData(FALSE);
}

void
CImgConvertDlg::OnBrowsePath()
{
    CFileDialogX dlg(TRUE,
                     NULL,
                     NULL,
                     OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT,
                     _T(uLoadString(IDS_FILTER_IMG)),
                     this);

    CString sTitle(uLoadString(IDS_TITLE_SELECT));
    dlg.m_ofn.lpstrTitle = sTitle;

    CWinApp* pApp = AfxGetApp();
    CString sPath = pApp->GetProfileString(RegKey(), REG_SRC_PATH, "");

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    // Modify OPENFILENAME members directly to point to bigBuff
    char bigBuff[MAX_PATH * 1000] = ""; // maximum common dialog buffer size
    dlg.m_ofn.lpstrFile           = bigBuff;
    dlg.m_ofn.nMaxFile            = sizeof(bigBuff);

    if (dlg.DoModal() != IDOK)
        return;

    POSITION pos = dlg.GetStartPosition();
    int nCount   = 0;

    m_arPaths.RemoveAll();
    while (pos != NULL) {
        m_arPaths.Add(dlg.GetNextPathName(pos));
        nCount++;
    }

    AfxMessageBox(uFormatString(IDS_SBROWSE_FILES_RESULT, nCount), MB_ICONINFORMATION);

    pApp->WriteProfileString(RegKey(), REG_SRC_PATH, dlg.GetPathName());
}

void
CImgConvertDlg::OnOK()
{
    UpdateData();

    if (m_sTargetPath.IsEmpty()) {
        AfxMessageBox(IDS_EMPTY_PATH);
        m_wndTargetPath.SetFocus();
        m_wndTargetPath.SetSel(0, -1);
        return;
    }

    if (!uIsDirExist(m_sTargetPath)) {
        if (AfxMessageBox(uFormatString(IDS_INVALID_TARGET_PATH, m_sTargetPath), MB_ICONQUESTION | MB_YESNO) != IDYES) {
            m_wndTargetPath.SetFocus();
            m_wndTargetPath.SetSel(0, -1);
            return;
        }

        if (!uCreateNewFolder(m_sTargetPath)) {
            AfxMessageBox(uFormatString(IDS_CANNOT_CREATE_DIR, m_sTargetPath));
            m_wndTargetPath.SetFocus();
            m_wndTargetPath.SetSel(0, -1);
            return;
        }
    }

    int nTotal = m_arPaths.GetSize();
    if (nTotal == 0) {
        AfxMessageBox(IDS_INVALID_SOURCE_FILES);
        m_btnBrowse.SetFocus();
        return;
    }

    if (m_bUseFrm && m_nFrmConvertType == 1) {
        if (m_sFrmPath.IsEmpty()) {
            AfxMessageBox(IDS_EMPTY_FRM_PATH);
            m_btnFrmBrowse.SetFocus();
            return;
        }

        if (!uIsFileExist(m_sFrmPath)) {
            AfxMessageBox(uFormatString(IDS_ERR_FRM_PATH, m_sFrmPath));
            m_btnFrmBrowse.SetFocus();
            return;
        }
    }

    int nBitsPerPixel = cm_256Colors;
    int nFileType     = FILE_PCX;
    int nSel          = m_wndBitsPerPixel.GetCurSel();

    if (nSel == 1)
        nBitsPerPixel = cm_TrueColor;

    nSel         = m_wndFileType.GetCurSel();
    CString sExt = ".pcx";

    switch (nSel) {
        case 1:
            nFileType = FILE_BMP;
            sExt      = ".bmp";
            break;
        case 2:
            nFileType = FILE_LEAD1JFIF;
            sExt      = ".jpg";
            break;
        default:
            nFileType = FILE_PCX;
            sExt      = ".pcx";
    }

    CGraphXImage img;
    CGraphXImage imgFrm;
    CString sPathSrc, sPathDst;
    CString sDir = m_sTargetPath;

    if (sDir[sDir.GetLength() - 1] != '\\')
        sDir += "\\";

    sDir = CFileX::GetFolder(sDir);

    if (AfxMessageBox(uFormatString(IDS_ASK_TO_CONVERT, sDir), MB_ICONQUESTION | MB_YESNO) != IDYES)
        return;

    CWaitCursor wait;
    BITMAPHANDLE hbmp;

    int nWidth      = 0;
    int nHeight     = 0;
    int nPercentage = 0;
    UINT nIndex     = 0;

    L_UINT32 dwColors = 0;

    CDC* pDC = NULL;
    HDC hDC  = NULL;

    CDC* pTempDC = NULL;
    HDC hTempDC  = NULL;

    RGBQUADPAL pPalS;
    RGBQUADPAL pPalD;

    try {
        //////////////////////////////////////////////////////////////////////////
        // Load Frame Template Image, if needed
        if (m_bUseFrm && m_nFrmConvertType == 1) {
            imgFrm.LoadFile(m_sFrmPath);
            if (!imgFrm.IsLoaded())
                return;
        }

        //////////////////////////////////////////////////////////////////////////
        // Move through all selected files
        for (int i = 0; i < nTotal; i++) {
            sPathSrc = m_arPaths.GetAt(i);
            if (!uIsFileExist(sPathSrc))
                continue;

            // Load Source File
            img.LoadFile(sPathSrc);
            if (!img.IsLoaded())
                continue;

            /*			//////////////////////////////////////////////////////////////////////////
			// To remove last '_large' or '_lg'
			CString sTmp = CFileX::GetShortFileName(sPathSrc);
			int nSize = sTmp.GetLength();
			if (nSize > 6)
			{
				 if (sTmp.Right(6).CompareNoCase("_large") == 0)
					 sTmp.Delete(nSize-6, 6);
				 else if (sTmp.Right(6).CompareNoCase("-large") == 0)
					 sTmp.Delete(nSize-6, 6);
				 else if (sTmp.Right(3).CompareNoCase("_lg") == 0)
					 sTmp.Delete(nSize-3, 3);
			}
			else if (nSize > 3)
			{
				if (sTmp.Right(3).CompareNoCase("_lg") == 0)
					sTmp.Delete(nSize-3, 3);
			}
			sPathDst = sDir + sTmp;
			//////////////////////////////////////////////////////////////////////////
*/
            sPathDst = sDir + CFileX::GetShortFileName(sPathSrc);

            // Add suffix, if needed
            if (m_bUseSuffix)
                sPathDst += m_sSuffix;

            // Add file extension
            if (m_bUseFileType)
                sPathDst += sExt;
            else
                sPathDst += "." + uGetFileExt(sPathSrc);

            //////////////////////////////////////////////////////////////////////////
            // Use Frame Conversion
            if (m_bUseFrm) {
                if (m_nFrmConvertType == 0) {
                    // Cut a part of image (for AoM technologies etc.)
                    GRAPHX::CopyBitmap(&hbmp, &img.m_hbmp);
                    GRAPHX::SizeBitmapExt(&img.m_hbmp, m_nImgCX, m_nImgCY, SIZE_BICUBIC);
                    GRAPHX::CopyBitmapRect(&img.m_hbmp, &hbmp, m_nFrmCX, m_nFrmCY, m_nImgCX, m_nImgCY);
                } else {
                    // Past an image to a frame image (for AoM technologies etc.)
                    imgFrm.LoadFile(m_sFrmPath);
                    if (!imgFrm.IsLoaded())
                        return;

                    GRAPHX::SizeBitmapExt(&img.m_hbmp, m_nImgCX, m_nImgCY, SIZE_BICUBIC);
                    GRAPHX::CopyBitmap(&hbmp, &img.m_hbmp);
                    GRAPHX::CopyBitmap(&img.m_hbmp, &imgFrm.m_hbmp);
                    GRAPHX::CombineBitmap(&img.m_hbmp, m_nFrmCX, m_nFrmCY, m_nImgCX, m_nImgCY, &hbmp, 0, 0, 0);
                }
            } //if (m_bUseFrm)
            //////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////
            // Resize image
            if (m_bUseSize) {
                // Proportional resizing
                if (m_bUsePropResize) {
                    nWidth      = 0;
                    nHeight     = 0;
                    nPercentage = 0;

                    GRAPHX::CopyBitmap(&hbmp, &img.m_hbmp);

                    if (hbmp.Width > hbmp.Height) {
                        nPercentage = (m_nWidth * 100 / hbmp.Width);
                        nWidth      = m_nWidth;
                        nHeight     = (hbmp.Height * nPercentage / 100);
                    } else {
                        nPercentage = (m_nHeight * 100 / hbmp.Height);
                        nHeight     = m_nHeight;
                        nWidth      = (hbmp.Width * nPercentage / 100);
                    }

                    GRAPHX::SizeBitmapExt(&hbmp, nWidth, nHeight, SIZE_BICUBIC);

                    GRAPHX::SizeBitmap(&img.m_hbmp, m_nWidth, m_nHeight);
                    GRAPHX::FillBitmap(&img.m_hbmp, RGB(255, 255, 255));

                    GRAPHX::CombineBitmap(&img.m_hbmp, (m_nWidth - nWidth) / 2, (m_nHeight - nHeight) / 2, nWidth, nHeight, &hbmp, 0, 0, 0);
                } else
                    GRAPHX::SizeBitmapExt(&img.m_hbmp, m_nWidth, m_nHeight, SIZE_BICUBIC);
            } //if (m_bUseSize)
            //////////////////////////////////////////////////////////////////////////

            /*		
			GRAPHX::SaveBitmap(sPathDst, &img.m_hbmp, 
				m_bUseFileType ? nFileType : img.m_fi.Format,
				m_bUseBitsPerPixel ? nBitsPerPixel : img.m_fi.BitsPerPixel);
*/
            //////////////////////////////////////////////////////////////////////////
            // Make Civilopedia compatable palette
            if ((m_bUseBitsPerPixel && m_bCivPalette) && nBitsPerPixel == cm_256Colors) {
                ZeroMemory(&pPalS, RGBQUADPAL_SIZE);
                ZeroMemory(&pPalD, RGBQUADPAL_SIZE);

                // Convert the current bitmap to 8 bits per pixel with an optimized palette
                if (img.m_hbmp.BitsPerPixel != cm_256Colors) {
                    GRAPHX::ColorResBitmap(&img.m_hbmp, &img.m_hbmp, cm_256Colors, CRF_OPTIMIZEDPALETTE, NULL, NULL, 0, NULL, NULL);
                }
                /*
				GRAPHX::SaveBitmap(sPathDst, &img.m_hbmp, 
					m_bUseFileType ? nFileType : img.m_fi.Format,
					m_bUseBitsPerPixel ? nBitsPerPixel : img.m_fi.BitsPerPixel);
*/
                GRAPHX::GetBitmapColorCount(&img.m_hbmp, dwColors);
                GRAPHX::GetBitmapColors(&img.m_hbmp, 0, dwColors, pPalS.pal);
                GRAPHX::CopyBitmap(&hbmp, &img.m_hbmp);

                if (dwColors < COLOR8 - 3) {
                    memcpy(pPalD.pal + 3, &pPalS, RGBQUADPAL_SIZE - sizeof(RGBQUAD) * 3);
                    dwColors += 3;
                } else if (dwColors < COLOR8 - 2) {
                    memcpy(pPalD.pal + 2, &pPalS, RGBQUADPAL_SIZE - sizeof(RGBQUAD) * 2);
                    dwColors += 2;
                } else if (dwColors < COLOR8) {
                    memcpy(pPalD.pal + 1, &pPalS, RGBQUADPAL_SIZE - sizeof(RGBQUAD));
                    dwColors++;
                } else
                    memcpy(&pPalD, &pPalS, RGBQUADPAL_SIZE);

                pPalD.pal[0].rgbRed   = 255;
                pPalD.pal[0].rgbGreen = 0;
                pPalD.pal[0].rgbBlue  = 255;

                pPalD.pal[1].rgbRed   = 0;
                pPalD.pal[1].rgbGreen = 255;
                pPalD.pal[1].rgbBlue  = 0;

                pPalD.pal[2].rgbRed   = 255;
                pPalD.pal[2].rgbGreen = 255;
                pPalD.pal[2].rgbBlue  = 255;

                if (img.m_fi.Format == FILE_PCX) {
                    ZeroMemory(&pPalS, RGBQUADPAL_SIZE);

                    // we must rearrange palette colors
                    for (nIndex                 = 0; nIndex < dwColors; nIndex++)
                        pPalS.pal[255 - nIndex] = pPalD.pal[nIndex];

                    GRAPHX::ColorResBitmap(&img.m_hbmp, &img.m_hbmp, cm_256Colors, CRF_USERPALETTE, pPalS.pal, NULL, 0, NULL, NULL);
                } else {
                    GRAPHX::ColorResBitmap(&img.m_hbmp, &img.m_hbmp, cm_256Colors, CRF_USERPALETTE, pPalD.pal, NULL, 0, NULL, NULL);
                }

                pDC = CDC::FromHandle(GRAPHX::CreateGraphXDC(&img.m_hbmp));
                hDC = pDC->m_hDC;

                pTempDC = CDC::FromHandle(GRAPHX::CreateGraphXDC(&hbmp));
                hTempDC = pTempDC->m_hDC;

                BitBlt(hDC, 0, 0, img.m_hbmp.Width, img.m_hbmp.Height, hTempDC, 0, 0, SRCCOPY);

                // Release DC
                GRAPHX::DeleteGraphXDC(*pDC);
                GRAPHX::DeleteGraphXDC(*pTempDC);
            } //if (m_bUseBitsPerPixel && nBitsPerPixel == cm_256Colors)
            //////////////////////////////////////////////////////////////////////////

            // Save result
            GRAPHX::SaveBitmap(sPathDst,
                               &img.m_hbmp,
                               m_bUseFileType ? nFileType : img.m_fi.Format,
                               m_bUseBitsPerPixel ? nBitsPerPixel : img.m_fi.BitsPerPixel);
        }
    } catch (CGraphXException* e) {
        AfxMessageBox(uFormatString(IDS_ERR_CONVERT, sPathSrc, e->GetErrorMessage(e->GetLastError())), MB_ICONERROR);
        e->Delete();
        return;
    } catch (CFileException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        return;
    } catch (CMemoryException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        return;
    } catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CImgConvertDlg::OnOK"), MB_ICONERROR);
        return;
    }

    AfxMessageBox(IDS_CONVERSION_SUCCESSFUL, MB_ICONINFORMATION);
    //	CDialogX::OnOK();
}

void
CImgConvertDlg::OnCivPal()
{
    UpdateControlsState();
}

void
CImgConvertDlg::OnFrmBrowseTargetPath()
{
    CFileDialogX dlg(
      TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T(uLoadString(IDS_FILTER_IMG)), this);

    CString sTitle(uLoadString(IDS_TITLE_FRM_SELECT));
    dlg.m_ofn.lpstrTitle = sTitle;

    CWinApp* pApp = AfxGetApp();
    CString sPath = pApp->GetProfileString(RegKey(), REG_SRC_PATH, "");

    if (uIsFileExist(m_sFrmPath))
        dlg.m_ofn.lpstrInitialDir = m_sFrmPath;

    if (dlg.DoModal() != IDOK)
        return;

    m_sFrmPath = dlg.GetPathName();
    UpdateData(FALSE);
}

void
CImgConvertDlg::OnFrmTypeChanged()
{
    UpdateControlsState();
}

void
CImgConvertDlg::OnUseFrm()
{
    UpdateControlsState();
}

void
CImgConvertDlg::OnChangeFrmCx()
{
    UpdateData();
}

void
CImgConvertDlg::OnChangeFrmCy()
{
    UpdateData();
}

void
CImgConvertDlg::OnChangeImgCx()
{
    UpdateData();
}

void
CImgConvertDlg::OnChangeImgCy()
{
    UpdateData();
}
