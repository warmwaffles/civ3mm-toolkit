/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	Utilities.cpp
	CREATED:	29.05.02
	VERSION:	1.7

	AUTHOR:		Dreyk

    DESCRIPTION: This file contain aids functions for
				 using from another applications.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Utilities.h"
#include "Resource.h"

#include <shfolder.h>
#include <shlobj.h>

// For using PathIsDirectory etc.
#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define PACKVERSION(major, minor) MAKELONG(minor, major)

// For BrowseForFolders:
#define DONT_DISMISS 1
static CString m_sFolder;
static CString m_sTmpPath;
static BOOL m_bCreateNewFolder;

/////////////////////////////////////////////////////////////////////////////
// Load string resources.
//
CString
uLoadString(UINT id)
{
    return CString((LPCTSTR)id);
}

/////////////////////////////////////////////////////////////////////////////
// Get formatted string from resources.
//
CString
uFormatString(UINT id, ...)
{
    CString s;
    va_list argList;

    va_start(argList, id);

    s.FormatV(CString((LPCTSTR)id), argList);

    va_end(argList);

    return s;
}

/////////////////////////////////////////////////////////////////////////////
// Load local string resources.
//
CString
uLocalLoadString(UINT id)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return CString((LPCTSTR)id);
}

/////////////////////////////////////////////////////////////////////////////
// Get formatted string from local resources.
//
CString
uLocalFormatString(UINT id, ...)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString s;
    va_list argList;

    va_start(argList, id);

    s.FormatV(CString((LPCTSTR)id), argList);

    va_end(argList);

    return s;
}

/////////////////////////////////////////////////////////////////////////////
// Load string resources and convert it like "Text1\nText2" to "Text1".
//
CString
uGetShort(UINT id)
{
    CString str;
    str.LoadString(id);
    int nIndex = str.ReverseFind(_T('\n'));

    if (nIndex != -1)
        str = str.Mid(nIndex + 1);
    return str;
}

/////////////////////////////////////////////////////////////////////////////
// Load local string resources and convert it like "Text1\nText2" to "Text1".
//
CString
uLocalGetShort(UINT id)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString str;
    str.LoadString(id);
    int nIndex = str.ReverseFind(_T('\n'));
    if (nIndex != -1)
        str = str.Mid(nIndex + 1);
    return str;
}

/////////////////////////////////////////////////////////////////////////////
// Convert int to CString.
//
CString
uInt2Str(INT num)
{
    CString res;
    res.Format("%d", num);

    return res;
}

/////////////////////////////////////////////////////////////////////////////
// Convert BOOL to CString.
//
CString
uBool2Str(BOOL bFlag)
{
    return (bFlag) ? "TRUE" : "FALSE";
}

/////////////////////////////////////////////////////////////////////////////
// Convert CString to BOOL.
//
BOOL
uStr2Bool(CString str)
{
    str.TrimLeft();
    str.TrimRight();
    if (str == "TRUE")
        return TRUE;

    return FALSE;
}

//////////////////////////////////////////////////////////////////
// Get the current Windows version
//
// Returning values:
// If it is Windows NT, Windows 2000 or Windows XP - return VER_PLATFORM_WIN32_NT (2)
// If it is Windows 95, Windows 98 or Windows ME   - return VER_PLATFORM_WIN32_WINDOWS (1)
// If it is Microsoft Win32s                       - return VER_PLATFORM_WIN32s (0)
// else - return -1
//
int
uGetWindowsVersion()
{
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;

    // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
    // If that fails, try using the OSVERSIONINFO structure.
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi))) {
        // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO*)&osvi))
            return -1;
    }

    switch (osvi.dwPlatformId) {
        // Windows NT, Windows 2000 or Windows XP
        case VER_PLATFORM_WIN32_NT:
            return VER_PLATFORM_WIN32_NT;
            break;

        // Windows 95, Windows 98 or Windows ME
        case VER_PLATFORM_WIN32_WINDOWS:
            return VER_PLATFORM_WIN32_WINDOWS;
            break;

        // Microsoft Win32s
        case VER_PLATFORM_WIN32s:
            return VER_PLATFORM_WIN32s;
            break;
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////////////
// Check is the current OS is Windows 2000 or later.
//
BOOL
uIsWin2000()
{
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;

    // Try calling GetVersionEx using the OSVERSIONINFOEX structure,
    // which is supported on Windows 2000.
    //
    // If that fails, try using the OSVERSIONINFO structure.
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi))) {
        // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO*)&osvi))
            return FALSE;
    }

    switch (osvi.dwPlatformId) {
        case VER_PLATFORM_WIN32_NT:

            if (osvi.dwMajorVersion >= 5)
                return TRUE;

            break;
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// The following function loads a specified DLL and attempts to call its
// DllGetVersion function. If successful, it uses a macro to pack the major
// and minor version numbers from the DLLVERSIONINFO structure into a DWORD
// that is returned to the calling application. If the DLL does not export
// DllGetVersion, the function returns zero.
//
// The following code example illustrates how you can use GetDllVersion to test if Comctl32.dll is version 4.71 or later.
// if (uGetDllVersion(TEXT("comctl32.dll")) >= PACKVERSION(4,71))
// {
//    //Proceed.
// }
// else
// {
//    //Use an alternate approach for older DLL versions.
// }
//
DWORD
uGetDllVersion(LPCTSTR lpszDllName)
{
    HINSTANCE hinstDll;
    DWORD dwVersion = 0;

    hinstDll = LoadLibrary(lpszDllName);

    if (hinstDll) {
        DLLGETVERSIONPROC pDllGetVersion;

        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

        //Because some DLLs might not implement this function, you
        //  must test for it explicitly. Depending on the particular
        // DLL, the lack of a DllGetVersion function can be a useful
        // indicator of the version.
        if (pDllGetVersion) {
            DLLVERSIONINFO dvi;
            HRESULT hr;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);

            hr = (*pDllGetVersion)(&dvi);

            if (SUCCEEDED(hr))
                dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
        }

        FreeLibrary(hinstDll);
    }
    return dwVersion;
}

/////////////////////////////////////////////////////////////////////////////
// Callback function for uBrowseFolder.
//
int CALLBACK
BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    switch (uMsg) {
        case BFFM_INITIALIZED: {
            if (lpData) // if there is a starting path - set the initial path
                ::SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);
        } break;

        case BFFM_SELCHANGED: {
            char pszPath[_MAX_PATH];

            // If the path is valid
            if (::SHGetPathFromIDList((const ITEMIDLIST*)lParam, pszPath)) {
                // If was selected some of the removable disk (floppy or CD)
                char pszTmp[MAX_PATH] = { 0 };
                strcpy(pszTmp, pszPath);
                pszTmp[3]    = 0;
                DWORD dwType = GetDriveType(pszTmp);

                if (dwType == DRIVE_REMOVABLE || dwType == DRIVE_CDROM) {
                    ::SendMessage(hwnd, BFFM_ENABLEOK, 0, 0);
                    return 0;
                }

                // Save the current path: it will be necessary
                // if user type a new path in the editbox
                m_sTmpPath = pszPath;

                // Set the path on the status text
                ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (long)pszPath);
            }
        } //case BFFM_SELCHANGED:
        break;

        case BFFM_VALIDATEFAILED: {
            CString sPath = (LPCTSTR)lParam;

            // Prepare the all path combine the current selected path,
            // which saved in the m_sTmpPath and new path, which was
            // added by user (sPath).
            // Example: m_sTmpPath == "C:\Temp", sPath == "Temp\New"
            // result is "C:\Temp\New".
            int nLen = sPath.Find('\\');

            // If was found slash.
            if (nLen != -1) {
                // If it is network path
                // For example, "\\Server\Folder".
                if (sPath[0] == '\\' && sPath[1] == '\\') {
                    // do something... if you want. ;-)
                } else if (sPath[1] != ':' && sPath[2] != '\\') {
                    // If it isn't network path and m_sTmpPath != "C:\"
                    // (or some different drive letter).
                    // For example, "C:\Temp".
                    if (!m_sTmpPath.IsEmpty()) {
                        if (m_sTmpPath.GetLength() > 3)
                            sPath = sPath.Mid(nLen, sPath.GetLength() - nLen);

                        CString strTmp = m_sTmpPath;
                        sPath.Insert(0, strTmp);
                    } else {
                        AfxMessageBox(uFormatString(IDS_INVALID_TARGET_PATH, sPath));
                        return DONT_DISMISS;
                    }
                } //else if (...
                else if (sPath[1] == ':' && sPath[2] == '\\') {
                } else
                    sPath = m_sTmpPath + '\\' + sPath;
            } //if (nLen != -1)
            else
                sPath = m_sTmpPath + '\\' + sPath;

            if ((sPath[0] != '\\' && sPath[1] != '\\') && PathGetDriveNumber(sPath) == -1) {
                AfxMessageBox(uFormatString(IDS_INVALID_TARGET_PATH, sPath));
                return DONT_DISMISS;
            }

            // If found a double slash in the target path - break
            nLen = sPath.Find("\\\\", 2);
            if (nLen != -1) {
                AfxMessageBox(uFormatString(IDS_INVALID_TARGET_PATH, sPath));
                return DONT_DISMISS;
            }

            // If last symbol is slash - remove it.
            // Example: "C:\Temp\" -> "C:\Temp"
            PathRemoveBackslash(sPath.GetBuffer(sPath.GetLength()));
            sPath.ReleaseBuffer();

            int iRes = AfxMessageBox(uFormatString(IDS_ASK_CREATE_DIRECTORY, sPath), MB_YESNOCANCEL | MB_ICONQUESTION);

            if (iRes == IDYES) {
                // If can not create a new folder
                if (!uCreateNewFolder(sPath)) {
                    AfxMessageBox(uFormatString(IDS_CANT_CREATE_DIRECTORY, sPath), MB_ICONERROR);
                    return DONT_DISMISS;
                } else {
                    m_sFolder = sPath;
                    m_sTmpPath.IsEmpty();
                    m_bCreateNewFolder = TRUE;
                }
            } else if (iRes == IDCANCEL)
                return DONT_DISMISS;
        } //case BFFM_VALIDATEFAILED:
        break;
    } //switch (uMsg)

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Select some target directory by the Browse for folders dialog box.
//
void
uBrowseFolder(HWND hParentWnd, CString* sFolder, CString sTitle, UINT ulFlags /*=0*/)
{
    CWaitCursor wait;
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString sInitFolder = *sFolder;
    *sFolder            = "";
    m_sFolder.Empty();
    m_bCreateNewFolder = FALSE;

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    BROWSEINFO BrInfo;           // Browse Info structure
    LPITEMIDLIST pidlRoot;       // PIDL for Root folder
    LPITEMIDLIST pidlSelFolder;  // PIDL for selected folder
    LPMALLOC pMalloc;            // IMalloc interface handler
    CHAR cDisplayName[MAX_PATH]; // Display Name of the selected folder
    strcpy(cDisplayName, "");

    SHGetMalloc(&pMalloc);                                                 // Get the IMalloc interface
    SHGetSpecialFolderLocation(NULL /*m_hWnd*/, CSIDL_DESKTOP, &pidlRoot); // Get the Root folder

    //	Fill the Browse Info structure
    BrInfo.hwndOwner      = hParentWnd;
    BrInfo.pidlRoot       = pidlRoot;
    BrInfo.pszDisplayName = cDisplayName;
    BrInfo.lpfn           = BrowseCallbackProc;
    BrInfo.lParam         = (long)((LPCTSTR)(sInitFolder)); // parse the initial folder value
    BrInfo.iImage         = NULL;

    //	bi.ulFlags      = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | 0x40 | BIF_EDITBOX | BIF_VALIDATE;

    // If Shell32.dll version >= 5,0 - we can use a new styles for BFF dialog
    if (uGetDllVersion(TEXT("Shell32.dll")) >= PACKVERSION(5, 0))
        BrInfo.ulFlags = 0x0040 /*64 == BIF_NEWDIALOGSTYLE*/ | BIF_EDITBOX | ulFlags;
    else {
        BrInfo.ulFlags = BIF_EDITBOX | BIF_VALIDATE | ulFlags;
        sTitle += CString((LPCSTR)IDS_OLD_STYLE_BROWSEINFO);
    }

    BrInfo.lpszTitle = sTitle;

    //	Call the Browse for folder dialog
    if ((pidlSelFolder = SHBrowseForFolder(&BrInfo)) != NULL) {
        if (SHGetPathFromIDList(pidlSelFolder, cDisplayName))
            *sFolder = cDisplayName;
        else {
            *sFolder = "";
            AfxMessageBox(CString((LPCSTR)IDS_WRONG_FOLDER), MB_ICONERROR);
        }

        // Changed from &SetFolder to
        pMalloc->Free(pidlSelFolder); // Free resources
    }

    if (m_bCreateNewFolder) {
        m_bCreateNewFolder = FALSE;
        *sFolder           = m_sFolder;
    }

    // Changed from &pidlRoot to
    pMalloc->Free(pidlRoot); // Free the PIDL resource
}

//////////////////////////////////////////////////////////////////////////////
// Create a new folder by the given path.
//
BOOL
uCreateNewFolder(CString sPath)
{
    if (PathIsDirectory(sPath)) // Selected directory already exist
        return TRUE;

    int i;
    CString sFolder;

    if (CreateDirectory(sPath, NULL)) // Try to simply create the directory
        return TRUE;

    for (i = 0; i < sPath.GetLength(); i++) // Find the 1st occurence of the '\\' character
    {
        if (sPath[i] == '\\')
            break;
        sFolder += sPath[i]; // Add new simbols to the Working path
    }

    while (i < sPath.GetLength()) // Repeat until the Path ends
    {
        sFolder += sPath[i];  // Add new simbols to the Working path
        if (sPath[i] == '\\') // If new 'folder devider' found,
        {
            if (!PathIsDirectory(sFolder))           // Check, if this folder already exists
                if (!CreateDirectory(sFolder, NULL)) // If not, try to create it
                    return FALSE;
        }
        i++;
    }

    if (!PathIsDirectory(sFolder))           // In a case, when the path is not ended with '/' character
        if (!CreateDirectory(sFolder, NULL)) // Try to create the last path's part
            return FALSE;                    // Return BAD status, if creating fails

    return TRUE; // Return OK status, if all action succeed
}

//////////////////////////////////////////////////////////////////////////////
// Check is the given directory already exist.
//
BOOL
uIsDirExist(LPCTSTR lpPath)
{
    if (PathIsDirectory(lpPath))
        return TRUE;

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// Check is the given file already exist.
//
BOOL
uIsFileExist(LPCTSTR lpPath)
{
    CFileStatus st;
    return CFile::GetStatus(lpPath, st);
}

//////////////////////////////////////////////////////////////////////////////
// Get name of the user currently logged onto the system, name of the
// local computer and IP address.
//
void
uGetUserNameAndHost(CString& sUserName, CString& sComputerName, CString& sIPaddress)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    DWORD MaxNameLength = MAX_COMPUTERNAME_LENGTH + 1;
    TCHAR lpszHostName[MAX_COMPUTERNAME_LENGTH + 1];
    PHOSTENT phostent;
    IN_ADDR inaddr;
    TCHAR lpszUserName[256];
    DWORD dwSize = 256;

    //////////////////////////////////
    // Initiates use of Ws2_32.dll by a process
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    //////////////////////////////////
    // Get the user name
    if (GetUserName(lpszUserName, &dwSize))
        sUserName = lpszUserName;

    //////////////////////////////////
    // Get computer name
    if (GetComputerName((LPTSTR)lpszHostName, (LPDWORD)&MaxNameLength)) {
        sComputerName = lpszHostName;

        // Get computer IP address
        if ((phostent = gethostbyname(lpszHostName)) != NULL) {
            memcpy(&inaddr, phostent->h_addr, 4);
            sIPaddress = inet_ntoa(inaddr);
        } else
            sIPaddress = uLoadString(IDS_LOCAL_HOST);
    }

    WSACleanup();
}

static LPCTSTR sDots = ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . \
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . \
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . \
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . \
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";

/////////////////////////////////////////////////////////////////////////////
// Add dots to the end of control's message string.
//
void
uAppendCtrlDots(CWnd* pWnd, UINT nCtrlID)
{
    CString sTemp;
    pWnd->GetDlgItem(nCtrlID)->GetWindowText(sTemp);
    sTemp += sDots;
    pWnd->GetDlgItem(nCtrlID)->SetWindowText(sTemp);
}

/////////////////////////////////////////////////////////////////////////////
// Make short name in case the name is too long.
//
void
uMakeShortString(CDC* pDC, CString& sText, int nColumnLen)
{
    LPCTSTR psz3Dots = _T("...");
    int nTextLen     = sText.GetLength();

    if (nTextLen > 0 && pDC->GetTextExtent(sText).cx > nColumnLen) {
        const int nAddLen = pDC->GetTextExtent(psz3Dots, 3).cx;
        int i;

        for (i = nTextLen - 1; i > 0; i--)
            if (pDC->GetTextExtent(sText, i).cx + nAddLen <= nColumnLen)
                break;

        if (i > 0) {
            sText.Delete(i, nTextLen - i);
            sText += psz3Dots;
        } else {
            // if one character and three dots are to wide, decrease the dot count
            int n1stLen = pDC->GetTextExtent(sText, 1).cx;
            int i;
            for (i = 2; i > 0; i--)
                if (n1stLen + pDC->GetTextExtent(psz3Dots, i).cx <= nColumnLen)
                    break;

            sText.Delete(1, nTextLen - 1);
            if (i > 0)
                sText += CString(psz3Dots, i);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Check a given string for incorrect chars: ':', '\', '/', '.', '|', '*'.
//
BOOL
uCheckIncorrectChars(CString& str, BOOL bReplaceBlanks /*= FALSE*/)
{
    int nLen = str.GetLength();

    for (int i = 0; i < nLen; i++) {
        switch (str[i]) {
            case ':':
            case '\\':
            case '/':
            case '.':
            case '|':
            case '*':
                return FALSE;
        }
    }

    // replace blanks to underscore
    if (bReplaceBlanks)
        str.Replace(' ', '_');

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Check a given string for correct line feed: replace all "\n" to "\r\n".
//
void
uCheckLineFeed(CString& str)
{
    str.Replace("\n", "\r\n");
}

//////////////////////////////////////////////////////////////////////////////////
// Get file's directory from a file path.
//
CString
uGetPathName(CString sPath)
{
    return sPath.Left(sPath.ReverseFind('\\') + 1);
}

//////////////////////////////////////////////////////////////////////////////////
// Get file's name from a file path.
//
CString
uGetFileName(CString sPath)
{
    int pos = sPath.ReverseFind('\\');
    if (pos == -1)
        return "";

    return sPath.Right(sPath.GetLength() - pos - 1);
}

//////////////////////////////////////////////////////////////////////////////////
// Get file's extension from a file path.
//
CString
uGetFileExt(CString sPath)
{
    int pos = sPath.ReverseFind('.');
    if (pos == -1)
        return "";

    pos = sPath.GetLength() - pos - 1;

    if (pos != 3)
        return "";

    return sPath.Right(pos);
}

///////////////////////////////////////////////////////////////////////
// Takes the CSIDL of a system folder and returns the path.
// Note: see "SHGetFolderPath" and "CSIDL Values" MSDN's topics for more information.
// To use this function you should include <shlobj.h> file.
//
CString
uSHGetFolderPath(int csidl)
{
    HMODULE hMod                         = NULL;
    PFNSHGETFOLDERPATHA pSHGetFolderPath = NULL;

    // Load SHFolder.dll only once
    hMod = LoadLibrary("SHFolder.dll");

    if (!hMod)
        return "";

    // Obtain a pointer to the SHGetFolderPathA function
    pSHGetFolderPath = (PFNSHGETFOLDERPATHA)GetProcAddress(hMod, "SHGetFolderPathA");
    if (!pSHGetFolderPath) {
        FreeLibrary(hMod);
        return "";
    }

    char buf[MAX_PATH] = { 0 };
    if (pSHGetFolderPath(NULL, csidl, NULL, 0, buf) != S_OK) {
        FreeLibrary(hMod);
        return "";
    }

    FreeLibrary(hMod);
    return buf;
}

///////////////////////////////////////////////////////////////////////
// Add number suffix to the end of name.
//
void
uGenerateNewName(CString& sName)
{
    int i = sName.GetLength();

    while (i-- > 0) {
        unsigned char ch = sName[i];
        if ((ch < '0') || (ch > '9'))
            break;
    }

    int curNum = atoi(sName.Mid(i + 1));
    sName.Format("%s%d", sName.Left(i + 1), curNum + 1);
}

///////////////////////////////////////////////////////////////////////
// Check name for incorrect chars and length (optionally).
//
BOOL
uCheckName(CString& sName, int nMaxLen /*=-1*/)
{
    CString sTmp;
    int nLen = sName.GetLength();

    if (nLen == 0)
        return FALSE;

    if (nMaxLen != -1 && nLen > nMaxLen)
        return FALSE;

    for (int i = 0; i < nLen; i++) {
        //		if (sName[i] == ' ')
        //			sTmp += '_'; // replace the blanks to underscore
        //		else
        if (sName[i] == ' ' || sName[i] == '_' || sName[i] == '\'' || sName[i] == '-' ||
            __iscsym(sName[i])) // __iscsym returns a non-zero value if c is a letter, underscore, or digit
                                //			isdigit(sName[i]) ||
                                //			uIsLetter(sName[i]))
        {
            sTmp += sName[i];
        } else
            return FALSE;
    }

    sName = sTmp;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////
// Check is a given char letter or not.
//
BOOL
uIsLetter(char c)
{
    return __iscsymf(c); // returns a non-zero value if c is a letter or an underscore
    //	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

///////////////////////////////////////////////////////////////////////
// Reads file in text mode and returns file content.
//
BOOL
uReadFile(LPCTSTR lpszFileName, CString& sContent, UINT nOpenFlags)
{
    char* pszBuf = NULL;
    int nLen;

    try {
        CFile fl(lpszFileName, nOpenFlags); // open file

        nLen = (int)fl.GetLength(); // get length of the file

        pszBuf = new char[nLen + 1];
        fl.Read(pszBuf, nLen); // retrive all data from file
        pszBuf[nLen] = 0;
        fl.Close();

        sContent = pszBuf;
        delete[] pszBuf;
        pszBuf = NULL;
    } catch (CFileException* e) {
        CString sMsg;
        TCHAR szCause[1024];

        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        e->GetErrorMessage(szCause, 1024);

        sMsg.Format(IDS_READ_FILE_ERR, lpszFileName, szCause);
        AfxMessageBox(sMsg, MB_ICONERROR);

        e->Delete();
        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////
// Write file content to the file.
//
BOOL
uWriteFile(LPCTSTR lpszFileName, CString sContent, UINT nOpenFlags)
{
    try {
        CFile fl(lpszFileName, nOpenFlags); // open file

        fl.SetLength(0);
        fl.Write(sContent, sContent.GetLength());
        fl.Close();
    } catch (CFileException* e) {
        CString sMsg;
        TCHAR szCause[1024];

        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        e->GetErrorMessage(szCause, 1024);

        sMsg.Format(IDS_WRITE_FILE_ERR, lpszFileName, szCause);
        AfxMessageBox(sMsg, MB_ICONERROR);

        e->Delete();
        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////
// Convert GUID to CString.
//
CString
uGuid2Str(GUID* pGuid)
{
    CString str;
    str.Format("%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X",
               pGuid->Data1,
               pGuid->Data2,
               pGuid->Data3,
               pGuid->Data4[0],
               pGuid->Data4[1],
               pGuid->Data4[2],
               pGuid->Data4[3],
               pGuid->Data4[4],
               pGuid->Data4[5],
               pGuid->Data4[6],
               pGuid->Data4[7]);

    return str;
}

///////////////////////////////////////////////////////////////////////
// Find string with any case.
//
int
uFindNoCase(CString sTxt, CString sSub, int nStart /* = 0*/)
{
    sTxt.MakeUpper();
    sSub.MakeUpper();

    // return -1 for not found, distance from beginning otherwise
    return sTxt.Find(sSub, nStart);
}

//////////////////////////////////////////////////////////////////////
//
//                  *** CFileX: WORK WITH FILES AND FOLDERS ***
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Create directories by the given path
//
void
CFileX::ProcessPath(CString sPath)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sPath, drive, dir, fname, ext);
    _makepath(path_buffer, drive, dir, "", "");

    CString _directory = path_buffer;
    CString _temp;
    for (int i = 0; i < _directory.GetLength(); i++) {
        char ch = _directory.GetAt(i);
        _temp += ch;

        if ((ch == '\\') || (ch == '/'))
            CreateDirectory(_temp, NULL);
    }
}

//////////////////////////////////////////////////////////////////////
// Check is file exist or no
//
BOOL
CFileX::FileExists(CString sFileName)
{
    return (GetFileAttributes(sFileName) != 0xffffffff);
}

//////////////////////////////////////////////////////////////////////
// Get size of the file
//
DWORD
CFileX::GetFileSize(CString sFileName)
{
    DWORD result = 0;
    try {
        result = (CFile(sFileName, CFile::shareDenyNone | CFile::modeRead)).GetLength();
    } catch (CFileException* pExc) {
        pExc->Delete();
        result = 0;
    } catch (...) {
        result = 0;
    }
    return result;
}

//////////////////////////////////////////////////////////////////////
// Remove the file
//
void
CFileX::Remove(CString sFileName)
{
    try {
        CFile::Remove(sFileName);
    } catch (CFileException* pExc) {
        pExc->m_strFileName = sFileName;
        throw pExc;
    }
}

//////////////////////////////////////////////////////////////////////
// Rename the file
//
void
CFileX::Rename(CString sOldFileName, CString sNewFileName)
{
    if (FileExists(sOldFileName)) {
        if (FileExists(sNewFileName)) {
            MakeNormal(sNewFileName);
            Remove(sNewFileName);
        }

        try {
            MakeNormal(sOldFileName);
            CFile::Rename(sOldFileName, sNewFileName);
        } catch (CFileException* pExc) {
            pExc->m_strFileName = sOldFileName;
            throw pExc;
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Reset all file attributes to normal
//
void
CFileX::MakeNormal(CString sFileName)
{
    if (FileExists(sFileName)) {
        CFileStatus status;
        CFile::GetStatus(sFileName, status);
        status.m_mtime     = 0;
        status.m_attribute = 0;
        CFile::SetStatus(sFileName, status);
    }
}

//////////////////////////////////////////////////////////////////////
// Create bakup of the file (file with .bak extension)
//
BOOL
CFileX::CreateBAKFile(CString sFileName)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sFileName, drive, dir, fname, ext);
    CString sName = fname;
    sName         = sName + ext;
    _makepath(path_buffer, drive, dir, sName, COleDateTime::GetCurrentTime().Format("%d.%m.%y_%H%M%S.bak")); //".bak");

    CString sBAKFileName = path_buffer;
    //	Rename(sFileName, sBAKFileName);
    return CopyFile(sFileName, sBAKFileName, FALSE);
}

//////////////////////////////////////////////////////////////////////
// Rename the file
//
CString
CFileX::ChangeFileName(CString sWholeFileName, CString sNewFileName)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sWholeFileName, drive, dir, fname, ext);
    _makepath(path_buffer, drive, dir, sNewFileName, ext);

    CString result = path_buffer;
    return result;
}

//////////////////////////////////////////////////////////////////////
// Change the file extension
//
CString
CFileX::ChangeFileExt(CString sWholeFileName, CString sNewFileExt)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sWholeFileName, drive, dir, fname, ext);
    _makepath(path_buffer, drive, dir, fname, sNewFileExt);

    CString result = path_buffer;
    return result;
}

//////////////////////////////////////////////////////////////////////
// Get folder path.
//
CString
CFileX::GetAsFolder(CString sName)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sName, drive, dir, fname, ext);

    CString sFolder = dir;
    sFolder += fname;
    sFolder += ext;
    _makepath(path_buffer, drive, sFolder, "", "");

    CString result = path_buffer;
    return result;
}

//////////////////////////////////////////////////////////////////////
// Creates a name for a temporary file
//
CString
CFileX::CreateTempFileName(CString sPath, CString sPrefix)
{
    CString result;

    char szTempName[MAX_PATH];
    GetTempFileName(sPath, sPrefix, 0, szTempName);

    result = szTempName;

    return result;
}

//////////////////////////////////////////////////////////////////////
// Get folder name
//
CString
CFileX::GetFolder(CString sName)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sName, drive, dir, fname, ext);
    _makepath(path_buffer, drive, dir, "", "");

    CString result = path_buffer;
    return result;
}

//////////////////////////////////////////////////////////////////////
// Get file name with extension
//
CString
CFileX::GetFileName(CString sName)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sName, drive, dir, fname, ext);
    _makepath(path_buffer, "", "", fname, ext);

    CString result = path_buffer;
    return result;
}

//////////////////////////////////////////////////////////////////////
// Get file name without extension
//
CString
CFileX::GetShortFileName(CString sName)
{
    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(sName, drive, dir, fname, ext);
    _makepath(path_buffer, "", "", fname, "");

    CString result = path_buffer;
    return result;
}

//////////////////////////////////////////////////////////////////////
// Check is a folder exist
//
BOOL
CFileX::FolderExists(CString sFolderName)
{
    return FileExists(GetAsFolder(sFolderName));
}

//////////////////////////////////////////////////////////////////////
// Create temporary folder
//
CString
CFileX::CreateTempFolder(CString sRoot)
{
    sRoot = GetAsFolder(sRoot);
    if (FolderExists(sRoot) == FALSE)
        ProcessPath(sRoot);

    CString sFolder;
    do {
        GUID guid;
        CoCreateGuid(&guid);
        sFolder = GetAsFolder(sRoot + uGuid2Str(&guid));
    } while (FolderExists(sFolder));

    ProcessPath(sFolder);
    return sFolder;
}

//////////////////////////////////////////////////////////////////////
// Remove all files
//
void
CFileX::RemoveFiles(CString sFileName)
{
    CFileFind finder;

    if (finder.FindFile(sFileName)) {
        BOOL bContinue = TRUE;
        BOOL bLast     = !finder.FindNextFile();

        while (bContinue) {
            if (finder.IsDots() == FALSE && finder.IsDirectory() == FALSE) {
                try {
                    CFile::Remove(finder.GetFilePath());
                } catch (CFileException* pExc) {
                    pExc->Delete();
                } catch (...) {
                }
            }

            bContinue = !bLast;
            if (bContinue)
                bLast = !finder.FindNextFile();
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Remove folder with all contents
//
void
CFileX::RemoveFolder(CString sFolderName)
{
    sFolderName = GetAsFolder(sFolderName);

    //////////////////////////////////////////////////////////////////
    //	remove contents
    CFileFind find;
    if (find.FindFile((sFolderName + "*.*"), 0)) {
        BOOL bLast     = !find.FindNextFile();
        BOOL bContinue = TRUE;
        while (bContinue) {
            if (find.IsDots() == FALSE) {
                if (find.IsDirectory()) {
                    // we have found the directory
                    RemoveFolder(GetAsFolder(find.GetFilePath()));
                    RemoveDirectory(find.GetFilePath());
                } else {
                    // we have found the file
                    try {
                        CFile::Remove(find.GetFilePath());
                    } catch (CFileException* pExc) {
                        pExc->Delete();
                    } catch (...) {
                    }
                }
            }

            bContinue = !bLast;
            if (bContinue)
                bLast = !find.FindNextFile();
        }
    }

    //////////////////////////////////////////////////////////////////
    //	remove the directory
    try {
        //	the directory have to be empty at this moment
        sFolderName.TrimRight("\\/");
        RemoveDirectory(sFolderName);
    } catch (CFileException* pExc) {
        pExc->Delete();
    } catch (...) {
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SPI_GETDROPSHADOW
#define SPI_GETDROPSHADOW 0x1024
#endif

void
ShowLastError()
{
    DWORD error = GetLastError();
    if (error) {
        LPVOID lpMsgBuf = NULL;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL,
                      error,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                      (LPTSTR)&lpMsgBuf,
                      0,
                      NULL);
        if (lpMsgBuf) {
            // Display the string.
            MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
            // Free the buffer.
            LocalFree(lpMsgBuf);
        } else {
            CString temp;
            temp.Format(_T("Error message 0x%lx not found"), error);
            // Display the string.
            MessageBox(NULL, temp, _T("Error"), MB_OK | MB_ICONINFORMATION);
        }
    }
}

WORD
NumBitmapColors(LPBITMAPINFOHEADER lpBitmap)
{
    if (lpBitmap->biClrUsed != 0)
        return (WORD)lpBitmap->biClrUsed;

    switch (lpBitmap->biBitCount) {
        case 1:
            return 2;
        case 4:
            return 16;
        case 8:
            return 256;
    }

    return 0;
}

int
NumScreenColors()
{
    static int nColors = 0;
    if (!nColors) {
        // DC of the desktop
        CClientDC myDC(NULL);
        nColors = myDC.GetDeviceCaps(NUMCOLORS);

        if (nColors == -1)
            nColors = 64000;
    }
    return nColors;
}

Win32Type
IsShellType()
{
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    DWORD winVer = GetVersion();
    if (winVer < 0x80000000) { /* NT */
        if (!GetVersionEx(&osvi))
            ShowLastError();

        if (osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 1L)
            return WinXP;
        else if (osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L)
            return Win2000;
        else if (osvi.dwMajorVersion == 4L)
            return WinNT4;
        else if (osvi.dwMajorVersion >= 5L) // for all future OS
            return WinXP;

        return WinNT3;
    } else if (LOBYTE(LOWORD(winVer)) < 4)
        return Win32s;

    if (!GetVersionEx(&osvi))
        ShowLastError();

    if (osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 10L)
        return Win98;
    else if (osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 90L)
        return WinME;

    return Win95;
}

const Win32Type g_Shell = IsShellType();

COLORREF
DarkenColorXP(COLORREF color)
{
    return RGB(MulDiv(GetRValue(color), 7, 10), MulDiv(GetGValue(color), 7, 10), MulDiv(GetBValue(color) + 55, 7, 10));
}

// Function splits a color into its RGB components and
// transforms the color using a scale 0..255
COLORREF
DarkenColor(long lScale, COLORREF lColor)
{
    long red   = MulDiv(GetRValue(lColor), (255 - lScale), 255);
    long green = MulDiv(GetGValue(lColor), (255 - lScale), 255);
    long blue  = MulDiv(GetBValue(lColor), (255 - lScale), 255);

    return RGB(red, green, blue);
}

COLORREF
MixedColor(COLORREF colorA, COLORREF colorB)
{
    // (86a + 14b) / 100
    int red   = MulDiv(86, GetRValue(colorA), 100) + MulDiv(14, GetRValue(colorB), 100);
    int green = MulDiv(86, GetGValue(colorA), 100) + MulDiv(14, GetGValue(colorB), 100);
    int blue  = MulDiv(86, GetBValue(colorA), 100) + MulDiv(14, GetBValue(colorB), 100);

    return RGB(red, green, blue);
}

COLORREF
MidColor(COLORREF colorA, COLORREF colorB)
{
    // (7a + 3b) / 10
    int red   = MulDiv(7, GetRValue(colorA), 10) + MulDiv(3, GetRValue(colorB), 10);
    int green = MulDiv(7, GetGValue(colorA), 10) + MulDiv(3, GetGValue(colorB), 10);
    int blue  = MulDiv(7, GetBValue(colorA), 10) + MulDiv(3, GetBValue(colorB), 10);

    return RGB(red, green, blue);
}

COLORREF
GrayColor(COLORREF crColor)
{
    int Gray = (((int)GetRValue(crColor)) + GetGValue(crColor) + GetBValue(crColor)) / 3;

    return RGB(Gray, Gray, Gray);
}

BOOL
IsLightColor(COLORREF crColor)
{
    return (((int)GetRValue(crColor)) + GetGValue(crColor) + GetBValue(crColor)) > (3 * 128);
}

// Function splits a color into its RGB components and
// transforms the color using a scale 0..255
COLORREF
LightenColor(long lScale, COLORREF lColor)
{
    long R = MulDiv(255 - GetRValue(lColor), lScale, 255) + GetRValue(lColor);
    long G = MulDiv(255 - GetGValue(lColor), lScale, 255) + GetGValue(lColor);
    long B = MulDiv(255 - GetBValue(lColor), lScale, 255) + GetBValue(lColor);

    return RGB(R, G, B);
}

COLORREF
BleachColor(int Add, COLORREF color)
{
    return RGB(min(GetRValue(color) + Add, 255), min(GetGValue(color) + Add, 255), min(GetBValue(color) + Add, 255));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************//

COLORREF
MapToSysColor(COLORREF color, BOOL bUseRGBQUAD)
{
    struct COLORMAP
    {
        // use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
        DWORD rgbqFrom;
        int iSysColorTo;
    };

    static const COLORMAP sysColorMap[] = {
        // mapping from color in DIB to system color
        { RGB_TO_RGBQUAD(0x00, 0x00, 0x00), COLOR_BTNTEXT },     // black
        { RGB_TO_RGBQUAD(0x80, 0x80, 0x80), COLOR_BTNSHADOW },   // dark grey
        { RGB_TO_RGBQUAD(0xC0, 0xC0, 0xC0), COLOR_BTNFACE },     // bright grey
        { RGB_TO_RGBQUAD(0xFF, 0xFF, 0xFF), COLOR_BTNHIGHLIGHT } // white
    };

    const int nMaps = 4;

    // look for matching RGBQUAD color in original
    for (int i = 0; i < nMaps; i++) {
        if (color == sysColorMap[i].rgbqFrom) {
            return bUseRGBQUAD ? CLR_TO_RGBQUAD(::GetSysColor(sysColorMap[i].iSysColorTo)) : ::GetSysColor(sysColorMap[i].iSysColorTo);
        }
    }

    return color;
}

HBITMAP
LoadSysColorBitmapSize(HINSTANCE hInst, HRSRC hRsrc, CSize sz)
{
    HGLOBAL hglb;

    if ((hglb = LoadResource(hInst, hRsrc)) == NULL)
        return NULL;

    LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)LockResource(hglb);
    if (lpBitmap == NULL)
        return NULL;

    UINT nSize          = lpBitmap->biSize;
    int nColorTableSize = 16;

    // less than 258 colors
    if (lpBitmap->biBitCount < 8) {
        // make copy of BITMAPINFOHEADER so we can modify the color table
        nSize = lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD);
    } else // 256 colors
      if (lpBitmap->biBitCount == 8) {
        nColorTableSize = 256;
        // make copy of BITMAPINFOHEADER so we can modify the color table
        nSize = lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD);
    }

    LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER)::malloc(nSize);

    if (lpBitmapInfo == NULL)
        return NULL;

    memcpy(lpBitmapInfo, lpBitmap, nSize);
    HBITMAP hbm = NULL;

    int nWidth  = (int)lpBitmapInfo->biWidth;
    int nHeight = (int)lpBitmapInfo->biHeight;

    // 16 or 256 colors
    if (lpBitmapInfo->biBitCount <= 8) {
        // color table is in RGBQUAD DIB format
        DWORD* pColorTable = (DWORD*)(((LPBYTE)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);

        for (int iColor         = 0; iColor < nColorTableSize; iColor++)
            pColorTable[iColor] = MapToSysColor(pColorTable[iColor], TRUE);

        HDC hDCScreen = ::GetDC(NULL);
        hbm           = ::CreateCompatibleBitmap(hDCScreen, sz.cx, sz.cy);

        if (hbm != NULL) {
            HDC hDCGlyphs  = ::CreateCompatibleDC(hDCScreen);
            HBITMAP hbmOld = (HBITMAP)::SelectObject(hDCGlyphs, hbm);

            LPBYTE lpBits;
            lpBits = (LPBYTE)(lpBitmap + 1);
            lpBits += (1 << (lpBitmapInfo->biBitCount)) * sizeof(RGBQUAD);

            StretchDIBits(
              hDCGlyphs, 0, 0, sz.cx, sz.cy, 0, 0, nWidth, nHeight, lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

            SelectObject(hDCGlyphs, hbmOld);
            ::DeleteDC(hDCGlyphs);
        }

        ::ReleaseDC(NULL, hDCScreen);
    } else // more than 256 colors
    {
        HDC hDCScreen = ::GetDC(NULL);
        hbm           = ::CreateCompatibleBitmap(hDCScreen, sz.cx, sz.cy);

        if (hbm != NULL) {
            HDC hDCGlyphs  = ::CreateCompatibleDC(hDCScreen);
            HBITMAP hbmOld = (HBITMAP)::SelectObject(hDCGlyphs, hbm);

            LPBYTE lpBits;
            lpBits = (LPBYTE)(lpBitmap + 1);

            StretchDIBits(
              hDCGlyphs, 0, 0, sz.cx, sz.cy, 0, 0, nWidth, nHeight, lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

            // Change a specific colors to system:
            for (int x = 0; x < sz.cx; x++) {
                for (int y = 0; y < sz.cy; y++) {
                    COLORREF clrOrig = ::GetPixel(hDCGlyphs, x, y);
                    COLORREF clrNew  = MapToSysColor(clrOrig, FALSE);

                    if (clrOrig != clrNew)
                        ::SetPixel(hDCGlyphs, x, y, clrNew);
                }
            }

            SelectObject(hDCGlyphs, hbmOld);
            ::DeleteDC(hDCGlyphs);
        }

        ::ReleaseDC(NULL, hDCScreen);
    }

    // free copy of bitmap info struct and resource itself
    ::free(lpBitmapInfo);
    ::FreeResource(hglb);

    return hbm;
}

// 'C:\Dir\file.ext' -> '..\file.ext'
BOOL
uMakeShortPath(CString& sPath)
{
    CString sTmp, sTmp1 = sPath;
    sTmp1.MakeReverse();

    int nLen = sTmp1.Find('\\', CFileX::GetFileName(sPath).GetLength() + 1);
    if (nLen != -1) {
        sTmp = sTmp1.Left(nLen);
        sTmp.MakeReverse();
        sTmp.Insert(0, "..\\");
        sPath = sTmp;
        return TRUE;
    }

    return FALSE;
}

// Generate temporary file target path
CString
uGetTempFileName(LPCSTR lpPrefixString /*= NULL*/)
{
    TCHAR sTemp[MAX_PATH + 1] = _T("\0");
    CString sPath;

    ::GetTempPath(MAX_PATH, sPath.GetBuffer(MAX_PATH));
    sPath.ReleaseBuffer();

    GetTempFileName(sPath, lpPrefixString == NULL ? "Tmp" : lpPrefixString, GetTickCount(), sTemp);
    return (CString)sTemp;
}

// Convert binary data to string
CString
uBin2Str(LPBYTE& lpData, DWORD nDataLen)
{
    CString sRes, sTmp;

    for (DWORD i = 0; i < nDataLen; i++) {
        sTmp.Format("%02X ", lpData[i]);
        sRes += sTmp;
    }

    sRes.TrimRight();

    return sRes;
}

// Convert string to binary data
DWORD
uStr2Bin(CString sSrc, LPBYTE& lpData)
{
    sSrc.TrimLeft();
    DWORD nDataLen = sSrc.GetLength();
    SAFE_DELETE_MULTI(lpData);
    lpData = new BYTE[nDataLen];

    DWORD i = 0;
    int x   = 0;
    while (!sSrc.IsEmpty()) {
        sscanf(sSrc.Left(3), "%X", &x);

        if (i < nDataLen)
            lpData[i] = x;

        i++;

        sSrc = sSrc.Right(sSrc.GetLength() - 3);
        //		sSrc.TrimLeft();
    }

    return i;
}

COLORREF
uColorFadeRed(COLORREF col, int nFadeValue)
{
    int r = col & 0x0000ff;
    int g = (col & 0x00ff00) >> 8;
    int b = (col & 0xff0000) >> 16;

    int nRem = 0;
    r += nFadeValue;
    if (r > 255) {
        nRem = r - 255;
        r    = 255;
    }

    if (g >= nFadeValue + (nRem >> 1))
        g -= nFadeValue + (nRem >> 1);
    else {
        nRem += nFadeValue + (nRem >> 1) - g;
        g = 0;
    }

    if (b >= nFadeValue + (nRem >> 1))
        b -= nFadeValue + (nRem >> 1);
    else {
        nRem += nFadeValue + (nRem >> 1) - b;
        b = 0;
    }

    r += nRem;
    if (r > 255)
        r = 255;

    return r | (g << 8) | (b << 16);
}

COLORREF
uColorFadeGreen(COLORREF col, int nFadeValue)
{
    int r = col & 0x0000ff;
    int g = (col & 0x00ff00) >> 8;
    int b = (col & 0xff0000) >> 16;

    int nRem = 0;
    g += nFadeValue;
    if (g > 255) {
        nRem = g - 255;
        g    = 255;
    }

    if (r >= nFadeValue + (nRem >> 1))
        r -= nFadeValue + (nRem >> 1);
    else {
        nRem += nFadeValue + (nRem >> 1) - r;
        r = 0;
    }

    if (b >= nFadeValue + (nRem >> 1))
        b -= nFadeValue + (nRem >> 1);
    else {
        nRem += nFadeValue + (nRem >> 1) - b;
        b = 0;
    }

    g += nRem;
    if (g > 255)
        g = 255;

    return r | (g << 8) | (b << 16);
}

COLORREF
uColorFadeBlue(COLORREF col, int nFadeValue)
{
    int r = col & 0x0000ff;
    int g = (col & 0x00ff00) >> 8;
    int b = (col & 0xff0000) >> 16;

    int nRem = 0;
    b += nFadeValue;
    if (b > 255) {
        nRem = b - 255;
        b    = 255;
    }

    if (g >= nFadeValue + (nRem >> 1))
        g -= nFadeValue + (nRem >> 1);
    else {
        nRem += nFadeValue + (nRem >> 1) - g;
        g = 0;
    }

    if (r >= nFadeValue + (nRem >> 1))
        r -= nFadeValue + (nRem >> 1);
    else {
        nRem += nFadeValue + (nRem >> 1) - r;
        r = 0;
    }

    b += nRem;
    if (b > 255)
        b = 255;

    return r | (g << 8) | (b << 16);
}

////////////////////////////////////////////////////////////////
// Check is file already opend or not.
//
BOOL
uIsOpend(LPCSTR lpFilePath)
{
    OFSTRUCT reOpenBuff;
    HFILE hfl = OpenFile(lpFilePath, &reOpenBuff, OF_READ | OF_SHARE_EXCLUSIVE);

    if (hfl == HFILE_ERROR) {
        if (GetLastError() == ERROR_SHARING_VIOLATION)
            return TRUE;
    } else
        CloseHandle((HANDLE)hfl);

    return FALSE;
}

////////////////////////////////////////////////////////////////
// Remove folder with all content.
//
DWORD
uDeleteFolder(CString sPath)
{
    DWORD dwErr = 0;
    CFileFind ff;

    // Dangerous situation: empty string means delete System32 folder :)
    if (sPath.IsEmpty())
        return 0;

    // Do not try to delete the directory if it doesn't exist.
    if (!::PathIsDirectory(sPath))
        return 0;

    // Do not delete relative paths (too dangerous)
    if (::PathIsRelative(sPath))
        return 0;

    if (ff.FindFile(sPath + "\\*.*") == 0)
        return GetLastError();

    BOOL bElse = TRUE;
    do {
        bElse = ff.FindNextFile();

        if (ff.IsDots())
            continue;

        if (ff.IsDirectory()) {
            if (dwErr == 0)
                dwErr = uDeleteFolder(ff.GetFilePath());
            else
                uDeleteFolder(ff.GetFilePath());
        } else {
            if (::DeleteFile(ff.GetFilePath()) == 0) {
                if (dwErr == 0)
                    dwErr = GetLastError();
            }
        }

    } while (bElse);

    ff.Close();

    if (dwErr == 0) {
        if (::RemoveDirectory(sPath) == 0)
            dwErr = GetLastError();
    }

    return dwErr;
}

//////////////////////////////////////////////////////////////////////////////
// Get file's version.
//
CString
uGetFileVersion(CString sPath)
{
    TCHAR szPath[MAX_PATH];
    memcpy(szPath, sPath, sPath.GetLength());
    szPath[sPath.GetLength()] = '\0';

    DWORD dwNullHandle = 0;
    DWORD dwLength     = ::GetFileVersionInfoSize(szPath, &dwNullHandle);
    if (dwLength <= 0)
        return "";

    BYTE* pProductInfo = new BYTE[dwLength];
    if (!::GetFileVersionInfo(szPath, NULL, dwLength, pProductInfo))
        return "";

    // Get file version
    LPVOID lpPtr;
    CString sQuery;
    UINT uiVerLength;

    typedef struct tagProductInformation
    {
        struct TRANSLATE
        {
            WORD languageId;
            WORD characterSet;
        } Translation;
    } PRODUCT_INFORMATION, *LPPRODUCT_INFORMATION;

    PRODUCT_INFORMATION ProductInfo;

    if (::VerQueryValue(pProductInfo, "\\VarFileInfo\\Translation", &lpPtr, &uiVerLength))
        ProductInfo.Translation = *(PRODUCT_INFORMATION::TRANSLATE*)lpPtr;

    sQuery.Format(
      _T ("\\StringFileInfo\\%04x%04x\\ProductVersion"), ProductInfo.Translation.languageId, ProductInfo.Translation.characterSet);
    ::VerQueryValue(pProductInfo, (LPTSTR)(LPCTSTR)sQuery, (LPVOID*)&lpPtr, &uiVerLength);
    CString sFileVersion = (LPCTSTR)lpPtr;
    delete[] pProductInfo;

    return sFileVersion;
}

//////////////////////////////////////////////////////////////////////////////
// Compare two file versions.
// -1 - error
// 0 - equal
// 1 - 1st > 2d
// 2 - 2d > 1st
//
int
uCompareVersions(CString sVer1, CString sVer2)
{
    if (sVer1.IsEmpty() || sVer2.IsEmpty())
        return -1;

    int nNum1 = 0, nNum2 = 0;
    int nLen1 = 0, nLen2 = 0;

    for (int i = 0; i < 4; i++) {
        nLen1 = sVer1.Find('.');
        if (nLen1 == -1)
            nNum1 = atoi(sVer1);
        else {
            nNum1 = atoi(sVer1.Left(nLen1));
            sVer1.Delete(0, nLen1 + 1);
        }

        nLen2 = sVer2.Find('.');
        if (nLen2 == -1)
            nNum2 = atoi(sVer2);
        else {
            nNum2 = atoi(sVer2.Left(nLen2));
            sVer2.Delete(0, nLen2 + 1);
        }

        if (nNum1 > nNum2)
            return 1;
        else if (nNum1 < nNum2)
            return 2;
    } //for (int i = 0, i < 4; i++)

    return 0;
}

BOOL
uCopyFile(CString sExistingFileName, CString sNewFileName)
{
    // in case name of source and destination files the same
    if (sExistingFileName.CompareNoCase(sNewFileName) == 0) {
        // in case destination file is already exist -
        // try to delete it
        //if (uIsFileExist(sNewFileName) && !DeleteFile(sNewFileName))
        return TRUE;
    }

    return CopyFile(sExistingFileName, sNewFileName, FALSE);
}