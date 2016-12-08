/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 CivTextWrapper.cpp
	CREATED:	 17.10.2003

	AUTHOR:		 Dreyk
	MODIFIED:	 

	DESCRIPTION: Wrapper for work with Civilopedia.txt.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "resource.h"
#include "CivTextWrapper.h"

#include "Utilities.h"

#ifndef S_BREAK
#define S_BREAK "\r\n"
#define N_BREAK 2
#endif

#ifndef S_DBREAK
#define S_DBREAK "\r\n\r\n"
#define N_DBREAK 4
#endif

//////////////////////////////////////////////////////////////////////
// Convert plain text to RTF
CString
Txt2Rtf(LPCTSTR sTxt)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString sTmp(sTxt);
    CString sLine;
    int nStart = 0;
    int nEnd   = 0;

    while ((nEnd = sTmp.Find(S_BREAK, nStart)) != -1) {
        sLine  = sTmp.Mid(nStart, nEnd - nStart);
        nStart = nEnd + N_BREAK;
    }

    nEnd = sTmp.GetLength();
    if (nStart < nEnd) {
        sLine = sTmp.Mid(nStart, nEnd - nStart);
    }

    return uFormatString(IDS_RTF_SEGMENT, uLoadString(IDS_RTF_MAIN) + " " + uLoadString(IDS_RTF_COLOR_TABLE) + sTxt);
}

void
MakeCivFormat(CString& sTxt)
{
    // we don't need blanks and breaks at the end...
    sTxt.TrimRight();

    // nothing to check ;)
    if (sTxt.IsEmpty())
        return;

    // check first '^'
    if (sTxt[0] != '^')
        sTxt.Insert(0, '^');

    // check for "\r\n" without "^"
    int nLen;
    int nStart = 0;
    while ((nStart = sTxt.Find(S_BREAK, nStart)) != -1) {
        nLen = sTxt.GetLength();
        if ((nStart + N_BREAK < nLen) && (sTxt[nStart + N_BREAK] != '^'))
            sTxt.Insert(nStart + N_BREAK, '^');

        nStart += N_BREAK;
    }

    // check last '^'
    nLen = sTxt.GetLength();
    if (nLen > 1 && sTxt.Right(N_BREAK) == S_BREAK)
        sTxt.Insert(nLen, '^');

    // check for "^" without "\r\n"
    nStart = 1; // do not check the first char
    while ((nStart = sTxt.Find('^', nStart)) != -1) {

        if (sTxt.Mid(nStart - N_BREAK, N_BREAK) != S_BREAK) {
            sTxt.Insert(nStart, S_BREAK);
            nStart += N_BREAK + 1;
        } else
            nStart += 1;
    }
}