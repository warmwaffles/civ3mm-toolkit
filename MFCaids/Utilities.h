#if !defined(UTILITIES_H__MFCAIDS__INCLUDED_)
#define UTILITIES_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

#define SAFE_FREE(ptr) if (ptr != NULL) { free(ptr); ptr = NULL; }
#define SAFE_DELETE_MULTI(ptr) if (ptr) { delete[] ptr; ptr = NULL; }
#define SAFE_DELETE(ptr) if (ptr) { delete ptr; ptr = NULL; }
#define SAFE_RELEASE(ptr) if (ptr) {(ptr)->Release(); ptr = NULL;}

//////////////////////////////////////////////////////////////////////
CString MFCAIDS_API uLoadString(UINT id); // Load string resources
CString MFCAIDS_API uFormatString(UINT id, ...); // Get formatted string from resources
CString MFCAIDS_API uGetShort(UINT id); // Load string resources and convert it like "A\nB" to "A"
CString MFCAIDS_API uLocalLoadString(UINT id); // Load local string resources
CString MFCAIDS_API uLocalFormatString(UINT id, ...); // Get formatted string from local resources
CString MFCAIDS_API uLocalGetShort(UINT id); // Load local string resources and convert it like "A\nB" to "A"
CString MFCAIDS_API uInt2Str(INT num); // Convert int to CString 
int     MFCAIDS_API uGetWindowsVersion(); // Get the current Windows version
BOOL    MFCAIDS_API uIsWin2000(); // Is the current OS is Windows 2000 or later
DWORD   MFCAIDS_API uGetDllVersion(LPCTSTR lpszDllName); // get version of dll
BOOL    MFCAIDS_API uCreateNewFolder(CString sPath); // Create a new folder by the given path
BOOL    MFCAIDS_API uIsDirExist(LPCTSTR lpPath); // Check is a directory exists or not
BOOL    MFCAIDS_API uIsFileExist(LPCTSTR lpPath); // Check is a file exists or not
int     CALLBACK    BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
void    MFCAIDS_API uBrowseFolder(HWND hParentWnd, CString *sFolder, CString sTitle, UINT ulFlags = 0);
void    MFCAIDS_API uGetUserNameAndHost(CString& sUserName, CString& sComputerName, CString& sIPaddress);
CString MFCAIDS_API uBool2Str(BOOL bFlag); // Convert BOOL to CString
BOOL    MFCAIDS_API uStr2Bool(CString str); // Convert CString to BOOL
void    MFCAIDS_API uAppendCtrlDots(CWnd *pWnd, UINT nCtrlID); // Add dots to the end of control's message string
void    MFCAIDS_API uMakeShortString(CDC *pDC, CString &sText, int nColumnLen); // Make short name in case the name is too long
BOOL    MFCAIDS_API uCheckIncorrectChars(CString &str, BOOL bReplaceBlanks = FALSE); // Check a given string for incorrect chars
void    MFCAIDS_API uCheckLineFeed(CString &str); // Check a given string for correct line feed: replace all "\n" to "\r\n"
CString MFCAIDS_API uGetFileName(CString sPath); // Get file's name from a file path
CString MFCAIDS_API uGetFileExt(CString sPath); // Get file's extension from a file path
CString MFCAIDS_API uGetPathName(CString sPath); // Get file's directory from a file path
CString MFCAIDS_API uSHGetFolderPath(int csidl); // Takes the CSIDL of a system folder and returns the path
BOOL    MFCAIDS_API uCheckName(CString &sName, int nMaxLen = -1); // Check name for incorrect chars
void    MFCAIDS_API uGenerateNewName(CString &sName); // Add number suffix to the end of name
BOOL    MFCAIDS_API uIsLetter(char c); // Check is a given char letter or not
BOOL    MFCAIDS_API uReadFile(LPCTSTR lpszFileName, CString &sContent, UINT nOpenFlags = CFile::modeRead | CFile::shareDenyNone); // Reads file in text mode and returns file content
BOOL    MFCAIDS_API uWriteFile(LPCTSTR lpszTxt, CString sContent, UINT nOpenFlags = CFile::modeCreate | CFile::modeReadWrite); // Write file content to the file
CString MFCAIDS_API uGuid2Str(GUID *pGuid); // Convert GUID to CString
int     MFCAIDS_API uFindNoCase(CString sTxt, CString sSub, int nStart = 0); // return -1 for not found, distance from beginning otherwise
BOOL    MFCAIDS_API uMakeShortPath(CString &sPath); // 'C:\Dir\SubDir\file.ext' -> '..\SubDir\file.ext'
CString MFCAIDS_API uGetTempFileName(LPCSTR lpPrefixString = NULL);
CString MFCAIDS_API uBin2Str(LPBYTE &lpData, DWORD nDataLen); // Convert binary data to string
DWORD   MFCAIDS_API uStr2Bin(CString sSrc, LPBYTE &lpData); // Convert string to binary data
BOOL	MFCAIDS_API uIsOpend(LPCSTR lpFilePath); // check is file already opend or not
DWORD	MFCAIDS_API uDeleteFolder(CString sPath); // remove folder with all content 
CString MFCAIDS_API uGetFileVersion(CString sPath); // Get DLL's version
int		MFCAIDS_API uCompareVersions(CString sVer1, CString sVer2); // compare two file versions
COLORREF MFCAIDS_API uColorFadeRed(COLORREF col, int nFadeValue);
COLORREF MFCAIDS_API uColorFadeGreen(COLORREF col, int nFadeValue);
COLORREF MFCAIDS_API uColorFadeBlue(COLORREF col, int nFadeValue);
BOOL	MFCAIDS_API uCopyFile(CString sExistingFileName, CString sNewFileName);

namespace CFileX // work with files and folders
{
CString MFCAIDS_API GetAsFolder(CString sName); // Get folder path
CString MFCAIDS_API GetFolder(CString sName); // Get folder name
CString MFCAIDS_API GetFileName(CString sName);	// Get file name with extension
CString MFCAIDS_API GetShortFileName(CString sName); // Get file name without extension
void    MFCAIDS_API ProcessPath(CString sPath); // Create directories by the given path
BOOL    MFCAIDS_API FileExists(CString sFileName); // Check is file exist or no
DWORD	MFCAIDS_API GetFileSize(CString sFileName); // Get size of the file
void	MFCAIDS_API Remove(CString sFileName); // Remove the file
void	MFCAIDS_API Rename(CString sOldFileName, CString sNewFileName); // Rename the file
void	MFCAIDS_API MakeNormal(CString sFileName); // Reset all file attributes to normal
BOOL	MFCAIDS_API CreateBAKFile(CString sFileName); // Create bakup of the file (file with .bak extension)
CString	MFCAIDS_API CreateTempFileName(CString sPath, CString sPrefix); // Creates a name for a temporary file
CString	MFCAIDS_API ChangeFileName(CString sWholeFileName, CString sNewFileName); // Rename the file
CString	MFCAIDS_API ChangeFileExt(CString sWholeFileName, CString sNewFileExt); // Change the file extension
BOOL	MFCAIDS_API FolderExists(CString sFolderName); // Check is a folder exist
CString	MFCAIDS_API CreateTempFolder(CString sRoot); // Create temporary folder
void	MFCAIDS_API RemoveFiles( CString sFileName ); // Remove all files
void	MFCAIDS_API RemoveFolder( CString sFolderName); // Remove folder with all content
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	DIBs use RGBQUAD format:
//		0xbb 0xgg 0xrr 0x00
//
//	Reasonably efficient code to convert a COLORREF into an
//	RGBQUAD is byte-order-dependent, so we need different
//	code depending on the byte order we're targeting.
#define RGB_TO_RGBQUAD(r,g,b)   (RGB(b,g,r))
#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

// Helper datatypes
class CToolBarData
{
public:
  WORD wVersion;
  WORD wWidth;
  WORD wHeight;
  WORD wItemCount;
  //WORD aItems[wItemCount]
  WORD* items()
    { return (WORD*)(this + 1); }
};

// Constans for detecting OS-Type
enum MFCAIDS_API Win32Type
{
  Win32s, 
  WinNT3, 
  Win95, 
  Win98, 
  WinME, 
  WinNT4, 
  Win2000, 
  WinXP
};

extern const MFCAIDS_API Win32Type g_Shell;

void MFCAIDS_API ShowLastError();
WORD MFCAIDS_API NumBitmapColors(LPBITMAPINFOHEADER lpBitmap);
int MFCAIDS_API NumScreenColors();
Win32Type MFCAIDS_API IsShellType();
COLORREF MFCAIDS_API DarkenColorXP(COLORREF color);
COLORREF MFCAIDS_API DarkenColor(long lScale, COLORREF lColor);
COLORREF MFCAIDS_API MixedColor(COLORREF colorA, COLORREF colorB);
COLORREF MFCAIDS_API MidColor(COLORREF colorA, COLORREF colorB);
COLORREF MFCAIDS_API GrayColor(COLORREF crColor);
COLORREF MFCAIDS_API LightenColor(long lScale, COLORREF lColor);
COLORREF MFCAIDS_API BleachColor(int Add, COLORREF color);
BOOL MFCAIDS_API IsLightColor(COLORREF crColor);
COLORREF MFCAIDS_API MapToSysColor(COLORREF color, BOOL bUseRGBQUAD);
HBITMAP MFCAIDS_API LoadSysColorBitmapSize(HINSTANCE hInst, HRSRC hRsrc, CSize sz);

//////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
#endif // !defined(UTILITIES_H__MFCAIDS__INCLUDED_)