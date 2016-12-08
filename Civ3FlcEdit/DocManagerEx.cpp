// DocManagerEx.cpp: implementation of the CDocManagerEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "DocManagerEx.h"

#include "DocTemplateFlc.h"
#include "DocTemplateC3f.h"
#include "MainFrm.h"
#include "FileFlcDlg.h"

#include "afxpriv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

enum ActionsId
{
    id_BLANK              = 0,
    id_DEFAULT            = 1,
    id_WALK               = 2,
    id_ATTACK1            = 3,
    id_ATTACK2            = 4,
    id_ATTACK3            = 5,
    id_DEATH              = 6,
    id_FORTIFY            = 7,
    id_FIDGET             = 8,
    id_VICTORY            = 9,
    id_BUILD              = 10,
    id_ROAD               = 11,
    id_MINE               = 12,
    id_IRRIGATE           = 13,
    id_FORTRESS           = 14,
    id_CAPTURE            = 15,
    id_JUNGLE             = 16,
    id_FOREST             = 17,
    id_PLANT              = 18,
    id_STOP_AT_LAST_FRAME = 19,
    id_ACTIONS_TOTAL      = 20
};

static const CString S_ACTION_INI[20] = { "BLANK",   "DEFAULT", "RUN",     "ATTACK1",  "ATTACK2",
                                          "ATTACK3", "DEATH",   "FORTIFY", "FIDGET",   "VICTORY",
                                          "BUILD",   "ROAD",    "MINE",    "IRRIGATE", "FORTRESS",
                                          "CAPTURE", "JUNGLE",  "FOREST",  "PLANT",    "STOP_AT_LAST_FRAME" };

static const CString S_ACTION_NAMES[20] = { "Blank",    "Default", "Run",     "AttackA", "AttackB", "AttackC",        "Death",
                                            "Fortyfy",  "Fidget",  "Victory", "Build",   "Road",    "Mine",           "Irrigate",
                                            "Fortress", "Capture", "Jungle",  "Forest",  "Plant",   "StopAtLastFrame" };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDocManagerEx::CDocManagerEx()
{
    m_bOpenAsC3f = FALSE;
}

CDocManagerEx::~CDocManagerEx()
{
}

void
CDocManagerEx::OnFileNew()
{
    if (CheckDocUnique()) {
        if (m_templateList.IsEmpty()) {
            TRACE0("Error: no document templates registered with CWinApp.\n");
            AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
            return;
        }

        CString sPath;
        CDocTemplate* pTemplate = NULL;

        // if this function was called at startup
        if (MAIN_APP->IsStartup() && MAIN_APP->IsLoadLastDoc()) {
            // get the tapget path of the last opend document
            sPath = AfxGetApp()->GetProfileString(REG_OPTIONS, REG_LAST_OPENED, "");

            if (uIsFileExist(sPath)) {
                if (uGetFileExt(sPath).CompareNoCase(FLC_EXT) == 0)
                    pTemplate = GetDocTemplateFlc();
                else
                    pTemplate = GetDocTemplateC3f();

                if (pTemplate)
                    pTemplate->OpenDocumentFile(sPath);

                return;
            }

            return;
        }

        pTemplate     = GetDocTemplateFlc();
        CFlcDoc* pDoc = (CFlcDoc*)pTemplate->OpenDocumentFile(NULL);
    }
}

BOOL
CDocManagerEx::CheckDocUnique()
{
    if (GetOpenDocumentCount() > 0) {
        if (!SaveAllModified())
            return FALSE;

        CloseAllDocuments(FALSE);
    }

    return TRUE;
}

CFlcDoc*
CDocManagerEx::GetFlcDoc()
{
    CDocTemplate* pDocTemplate = GetDocTemplateFlc();

    POSITION pos = pDocTemplate->GetFirstDocPosition();

    return (pos == NULL) ? NULL : (CFlcDoc*)pDocTemplate->GetNextDoc(pos);
}

CDocTemplate*
CDocManagerEx::GetDocTemplateFlc()
{
    POSITION pos = GetFirstDocTemplatePosition();
    if (pos == NULL) {
        TRACE("There is no doc templates registered..\n");
        ASSERT(FALSE);
    }
    CDocTemplate* pDocTemplate = GetNextDocTemplate(pos);

    while (pDocTemplate) {
        if (pDocTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplateFlc)))
            return pDocTemplate;

        pDocTemplate = GetNextDocTemplate(pos);
    }

    TRACE("The FLC document template is not found in the list of templates..\n");
    ASSERT(FALSE);

    return NULL;
}

CC3fDoc*
CDocManagerEx::GetC3fDoc()
{
    CDocTemplate* pDocTemplate = GetDocTemplateC3f();

    POSITION pos = pDocTemplate->GetFirstDocPosition();

    return (pos == NULL) ? NULL : (CC3fDoc*)pDocTemplate->GetNextDoc(pos);
}

CDocTemplate*
CDocManagerEx::GetDocTemplateC3f()
{
    POSITION pos = GetFirstDocTemplatePosition();
    if (pos == NULL) {
        TRACE("There is no doc templates registered..\n");
        ASSERT(FALSE);
    }
    CDocTemplate* pDocTemplate = GetNextDocTemplate(pos);

    while (pDocTemplate) {
        if (pDocTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplateC3f)))
            return pDocTemplate;

        pDocTemplate = GetNextDocTemplate(pos);
    }

    TRACE("The C3F document template is not found in the list of templates..\n");
    ASSERT(FALSE);

    return NULL;
}

void
CDocManagerEx::OnOpenC3f()
{
    /*	CFileDialog fd(TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		CString((LPCTSTR)IDS_SPOOL_FILTER));

	CString strTitle = uLoadString(IDS_OPEN_SPL_TITLE);
	fd.m_ofn.lpstrTitle = strTitle;

	CString REG_OPEN_SPL = "Dlg OpenSpl";
	CString REG_FILTER   = "Filter";

	// last used filter
	CWinApp* pApp = AfxGetApp();
	fd.m_ofn.nFilterIndex = AfxGetApp()->GetProfileInt(REG_OPEN_SPL, REG_FILTER, 1);

	if (fd.DoModal() != IDOK)
		return;

	pApp->WriteProfileInt(REG_OPEN_SPL, REG_FILTER, fd.m_ofn.nFilterIndex);
	
	m_bOpenAsC3f = TRUE;
	AfxGetApp()->OpenDocumentFile(fd.GetPathName());
	m_bOpenAsC3f = FALSE;
*/
}

void
CDocManagerEx::OnFileOpen()
{
    CFileFlcDlg dlg(TRUE,
                    NULL,
                    NULL,
                    OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_PATHMUSTEXIST,
                    _T(uLoadString(IDS_FILTER_FLCEDIT)),
                    AfxGetMainWnd());

    dlg.m_ofn.hInstance      = AfxGetInstanceHandle();
    dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_FLC);

    CString sTitle(uLoadString(IDS_TITLE_OPEN_FLCEDIT));
    dlg.m_ofn.lpstrTitle = sTitle;

    CCiv3FlcEditApp* pApp  = MAIN_APP;
    dlg.m_ofn.nFilterIndex = pApp->GetProfileInt(REG_OPTIONS, REG_FILTER_FLCEDIT, 1);
    CString sPath          = pApp->GetProfileString(REG_OPTIONS, REG_LAST_OPENED, "");

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    sPath = dlg.GetPathName();

    CDocTemplate* pDocTemplate = NULL;
    if (uGetFileExt(sPath).CompareNoCase(FLC_EXT) == 0) {
        pDocTemplate = GetDocTemplateFlc();
    } else {
        pDocTemplate = GetDocTemplateC3f();
    }

    if (pDocTemplate)
        pDocTemplate->OpenDocumentFile(sPath);

    pApp->WriteProfileString(REG_OPTIONS, REG_LAST_OPENED, sPath);
    pApp->WriteProfileInt(REG_OPTIONS, REG_FILTER_FLCEDIT, dlg.m_ofn.nFilterIndex);
}

BOOL
CDocManagerEx::OnFileSaveAs()
{
    MAIN_FRAME->SetFocus();
    CFlcDoc* pDoc = GetFlcDoc();

    if (!pDoc)
        return FALSE;

    CFileFlcDlg dlg(FALSE,
                    FLC_EXT,
                    pDoc->GetTitle(),
                    OFN_ENABLESIZING | OFN_ENABLETEMPLATE | OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_LONGNAMES |
                      OFN_PATHMUSTEXIST,
                    _T(uLoadString(IDS_FILTER_FLC)));

    dlg.m_ofn.hInstance      = AfxGetInstanceHandle();
    dlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_FILE_FLC);

    CString sTitle(uLoadString(IDS_TITLE_SAVE_AS));
    dlg.m_ofn.lpstrTitle = sTitle;

    CString sPath = pDoc->GetPathName();
    CWinApp* pApp = AfxGetApp();

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;
    else {
        sPath = pApp->GetProfileString(REG_OPTIONS, REG_LAST_OPENED, "");

        if (uIsFileExist(sPath))
            dlg.m_ofn.lpstrInitialDir = sPath;
    }

    if (dlg.DoModal() != IDOK)
        return FALSE;

    sPath = dlg.GetPathName();

    // .. save FLC
    //	if (!m_wndFlc.SavePalette(sPath))
    //		return FALSE;

    pDoc->DoSave(sPath);
    pApp->WriteProfileString(REG_OPTIONS, REG_LAST_OPENED, sPath);

    pDoc->SetModifiedFlag(FALSE);

    return TRUE;
}

AFX_STATIC BOOL AFXAPI
_AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
    if (lpszValueName == NULL) {
        if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ, lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS) {
            TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
            return FALSE;
        }
        return TRUE;
    } else {
        HKEY hKey;

        if (::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS) {
            LONG lResult =
              ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ, (CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

            if (::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
                return TRUE;
        }
        TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
        return FALSE;
    }
}

void
CDocManagerEx::RegisterShellFileTypes(BOOL bCompat)
{
    ASSERT(!m_templateList.IsEmpty()); // must have some doc templates

    CString strPathName, strTemp;

    AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

    /////////////////////////////////////////////////////
    // Register FLC file type
    CDocTemplate* pTemplate = GetDocTemplateFlc();

    CString strOpenCommandLine        = strPathName;
    CString strPrintCommandLine       = strPathName;
    CString strPrintToCommandLine     = strPathName;
    CString strDefaultIconCommandLine = strPathName;

    CString strFilterExt, strFileTypeId, strFileTypeName;
    LONG lSize   = 0;
    LONG lResult = 0;
    HKEY hKey    = NULL;

    if (pTemplate->GetDocString(strFileTypeId, CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty()) {
        // enough info to register it
        if (!pTemplate->GetDocString(strFileTypeName, CDocTemplate::regFileTypeName))
            strFileTypeName = strFileTypeId; // use id name

        ASSERT(strFileTypeId.Find(' ') == -1); // no spaces allowed

        // first register the type ID of our server
        if (!_AfxSetRegKey(strFileTypeId, strFileTypeName))
            return; // just skip it

        // path\shell\open\command = path filename
        strOpenCommandLine += _T(" \"%1\"");

        // path\shell\open\command = path filename
        strTemp.Format(_T("%s\\shell\\open\\%s"), (LPCTSTR)strFileTypeId, (LPCTSTR) _T("command"));
        if (!_AfxSetRegKey(strTemp, strOpenCommandLine))
            return; // just skip it

        pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
        if (!strFilterExt.IsEmpty()) {
            ASSERT(strFilterExt[0] == '.');

            lSize   = _MAX_PATH * 2;
            lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt, strTemp.GetBuffer(lSize), &lSize);
            strTemp.ReleaseBuffer();

            // no association for that suffix
            if (!_AfxSetRegKey(strFilterExt, strFileTypeId))
                return;
        }

        hKey    = NULL;
        lResult = RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.flc", &hKey);
        if (ERROR_SUCCESS == lResult) {
            RegDeleteValue(hKey, "Application");
            RegCloseKey(hKey);
        }
    }

    /////////////////////////////////////////////////////////
    // Set Default Icon

    // get current target path
    char buf[MAX_PATH];
    if (GetModuleFileName(MAIN_APP->m_hInstance, buf, MAX_PATH) == 0)
        return;

    // make default icon registry information
    CString sDefaultIcon;
    sDefaultIcon.Format("\"%s\",1", buf);

    // get identifier for the document type to be stored in the registration database maintained by Windows
    strFileTypeId += "\\DefaultIcon";
    LPCTSTR szKey = strFileTypeId;

    LPCTSTR szValue = "";
    LPCTSTR szData  = sDefaultIcon;

    DWORD dwDisposition;
    DWORD dwReserved = 0;
    HKEY hTempKey    = (HKEY)0;

    // length specifier is in bytes, and some TCHAR
    // are more than 1 byte each
    DWORD dwBufferLength = lstrlen(szData) * sizeof(TCHAR);

    // Open key of interest
    // Assume all access is okay and that all keys will be stored to file
    // Utilize the default security attributes
    if (ERROR_SUCCESS ==
        ::RegCreateKeyEx(
          HKEY_CLASSES_ROOT, szKey, dwReserved, (LPTSTR)0, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &hTempKey, &dwDisposition)) {

        // dwBufferLength must include size of terminating nul
        // character when using REG_SZ with RegSetValueEx function
        dwBufferLength += sizeof(TCHAR);

        ::RegSetValueEx(hTempKey, (LPTSTR)szValue, dwReserved, REG_SZ, (LPBYTE)szData, dwBufferLength);
    }

    // close opened key
    if (hTempKey)
        ::RegCloseKey(hTempKey);

    /////////////////////////////////////////////////////
    // Register C3F file type
    pTemplate = GetDocTemplateC3f();

    strOpenCommandLine        = strPathName;
    strPrintCommandLine       = strPathName;
    strPrintToCommandLine     = strPathName;
    strDefaultIconCommandLine = strPathName;

    if (pTemplate->GetDocString(strFileTypeId, CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty()) {
        // enough info to register it
        if (!pTemplate->GetDocString(strFileTypeName, CDocTemplate::regFileTypeName))
            strFileTypeName = strFileTypeId; // use id name

        ASSERT(strFileTypeId.Find(' ') == -1); // no spaces allowed

        // first register the type ID of our server
        if (!_AfxSetRegKey(strFileTypeId, strFileTypeName))
            return; // just skip it

        // path\shell\open\command = path filename
        strOpenCommandLine += _T(" \"%1\"");

        // path\shell\open\command = path filename
        strTemp.Format(_T("%s\\shell\\open\\%s"), (LPCTSTR)strFileTypeId, (LPCTSTR) _T("command"));
        if (!_AfxSetRegKey(strTemp, strOpenCommandLine))
            return; // just skip it

        pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
        if (!strFilterExt.IsEmpty()) {
            ASSERT(strFilterExt[0] == '.');

            lSize   = _MAX_PATH * 2;
            lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt, strTemp.GetBuffer(lSize), &lSize);
            strTemp.ReleaseBuffer();

            // no association for that suffix
            if (!_AfxSetRegKey(strFilterExt, strFileTypeId))
                return;
        }
        hKey    = NULL;
        lResult = RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.c3f", &hKey);
        if (ERROR_SUCCESS == lResult) {
            RegDeleteValue(hKey, "Application");
            RegCloseKey(hKey);
        }
    }

    /////////////////////////////////////////////////////////
    // Set Default Icon

    // get current target path

    // make default icon registry information
    sDefaultIcon.Format("\"%s\",2", buf);

    // get identifier for the document type to be stored in the registration database maintained by Windows
    strFileTypeId += "\\DefaultIcon";
    szKey = strFileTypeId;

    szValue  = "";
    szData   = sDefaultIcon;
    hTempKey = (HKEY)0;

    // length specifier is in bytes, and some TCHAR
    // are more than 1 byte each
    dwBufferLength = lstrlen(szData) * sizeof(TCHAR);

    // Open key of interest
    // Assume all access is okay and that all keys will be stored to file
    // Utilize the default security attributes
    if (ERROR_SUCCESS ==
        ::RegCreateKeyEx(
          HKEY_CLASSES_ROOT, szKey, dwReserved, (LPTSTR)0, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &hTempKey, &dwDisposition)) {
        // dwBufferLength must include size of terminating nul
        // character when using REG_SZ with RegSetValueEx function
        dwBufferLength += sizeof(TCHAR);

        ::RegSetValueEx(hTempKey, (LPTSTR)szValue, dwReserved, REG_SZ, (LPBYTE)szData, dwBufferLength);
    }

    // close opened key
    if (hTempKey)
        ::RegCloseKey(hTempKey);
}

void
CDocManagerEx::OpenFromINI()
{
    CFileDialogX dlg(
      TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T(uLoadString(IDS_FILTER_INI)), AfxGetMainWnd());

    CString sTitle(uLoadString(IDS_TITLE_INI));
    dlg.m_ofn.lpstrTitle = sTitle;

    CCiv3FlcEditApp* pApp = MAIN_APP;
    CString sPath         = pApp->GetProfileString(REG_OPTIONS, "Last INI", "");

    if (uIsFileExist(sPath))
        dlg.m_ofn.lpstrInitialDir = sPath;

    if (dlg.DoModal() != IDOK)
        return;

    sPath = dlg.GetPathName();
    pApp->WriteProfileString(REG_OPTIONS, "Last INI", sPath);

    ////////////////////////////////////////////////////////////
    // Get content of the unit's INI file
    CWaitCursor wait;

    CString sPrtoDir = sPath.Left(sPath.Find(dlg.GetFileName()));
    CString sINIContent, sTmp, sFile;
    int nStart, nEnd, nStartFlc, nEndFlc, i, j;

    if (!uReadFile(sPath, sINIContent))
        return;

    nStartFlc = sINIContent.Find("[Animations]");
    if (nStartFlc == -1)
        return;

    nEndFlc = sINIContent.Find("STOP_AT_LAST_FRAME=", nStartFlc);
    if (nEndFlc == -1)
        return;

    CStringArray arFlcPath;
    CStringArray* arSearch = MAIN_APP->GetSearchFolders();

    for (i = 0; i < id_ACTIONS_TOTAL; i++) {
        // Get FLC settings
        nStart = sINIContent.Find(S_ACTION_INI[i], nStartFlc);
        if (nStart != -1 && nStart < nEndFlc) {
            nStart += S_ACTION_INI[i].GetLength() + 1;
            nEnd = sINIContent.Find("\r\n", nStart);
            if (nEnd > nStart) {
                sTmp = sINIContent.Mid(nStart, nEnd - nStart);
                sTmp.TrimLeft();
                sTmp.TrimRight();
                if (!sTmp.IsEmpty()) {
                    if (sTmp[0] == '.') {
                        sTmp.Delete(0, 2);
                        for (j = 0; j < arSearch->GetSize(); j++) {
                            sFile = arSearch->GetAt(j) + "\\Art\\Units" + sTmp;
                            if (uIsFileExist(sFile)) {
                                arFlcPath.Add(sFile);
                                break;
                            }
                        }
                    } else {
                        sFile = sPrtoDir + sTmp;
                        if (uIsFileExist(sFile))
                            arFlcPath.Add(sFile);
                    }
                }
            }
        }
    } //for (int i = 0; i < id_ACTIONS_TOTAL; i++)
    ///////////////////////////////////////////////////////

    if (arFlcPath.GetSize() == 0) {
        AfxMessageBox(uFormatString(IDS_ERR_OPEN_INI, sPath));
        return;
    }

    CDocTemplate* pDocTemplate = GetDocTemplateFlc();

    if (!pDocTemplate)
        return;

    sTmp.Empty();
    for (i = 0; i < arFlcPath.GetSize(); i++)
        sTmp += "'" + arFlcPath.GetAt(i) + "'\r\n";

    if (AfxMessageBox(uFormatString(IDS_ASK_OPEN_INI, sPath, sTmp), MB_ICONQUESTION | MB_YESNO) != IDYES)
        return;

    for (i = 0; i < arFlcPath.GetSize(); i++)
        pDocTemplate->OpenDocumentFile(arFlcPath.GetAt(i));
}

void
CDocManagerEx::OpenFromFolder()
{
    CWinApp* pApp = AfxGetApp();
    CString sPath = pApp->GetProfileString(REG_OPTIONS, "Last Multi", "");

    uBrowseFolder(MAIN_FRAME->m_hWnd, &sPath, uLoadString(IDS_SELECT_ANIM_FOLDER), BIF_DONTGOBELOWDOMAIN);

    if (!uIsDirExist(sPath))
        return;

    pApp->WriteProfileString(REG_OPTIONS, "Last Multi", sPath);

    CString sTmp;
    CStringArray arFilePath;
    CFileFind finder;

    // build a string with wildcards
    CString sWildcard = sPath;
    sWildcard += "\\*.*";

    // start working for files
    BOOL bWorking = finder.FindFile(sWildcard);

    // while we found new spool-file(s) in the predefined folder
    while (bWorking) {
        // in case were found 20 files -
        // break to prevent any troubles with memory etc.
        if (arFilePath.GetSize() >= 20)
            break;

        bWorking = finder.FindNextFile();

        // skip . and .. files; otherwise, we'd recur infinitely!
        if (finder.IsDots())
            continue;

        // if it isn't a directory, do something...
        if (!finder.IsDirectory()) {
            sTmp = uGetFileExt(finder.GetFileName());
            if (sTmp.CompareNoCase(FLC_EXT) == 0 || sTmp.CompareNoCase(C3F_EXT) == 0) {
                arFilePath.Add(finder.GetFilePath());
            }
        }
    } //while (bWorking)

    finder.Close();

    //////////////////////////////////////////////////////////////////////////

    if (arFilePath.GetSize() == 0) {
        AfxMessageBox(uFormatString(IDS_ERR_OPEN_MULTI, sPath));
        return;
    }

    CDocTemplate* pDocTemplateFlc = GetDocTemplateFlc();
    CDocTemplate* pDocTemplateC3f = GetDocTemplateC3f();

    if (!pDocTemplateFlc || !pDocTemplateC3f)
        return;

    sTmp.Empty();
    for (int i = 0; i < arFilePath.GetSize(); i++)
        sTmp += "'" + arFilePath.GetAt(i) + "'\r\n";

    if (AfxMessageBox(uFormatString(IDS_ASK_OPEN_MULTI, sPath, sTmp), MB_ICONQUESTION | MB_YESNO) != IDYES)
        return;

    for (i = 0; i < arFilePath.GetSize(); i++) {
        sTmp = arFilePath.GetAt(i);

        if (uGetFileExt(sTmp).CompareNoCase(FLC_EXT) == 0)
            pDocTemplateFlc->OpenDocumentFile(sTmp);
        else
            pDocTemplateC3f->OpenDocumentFile(sTmp);
    }
}
