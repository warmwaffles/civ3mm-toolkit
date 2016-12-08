// GraphXImage.h: interface for the CGraphXImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(IMAGE_H__MFCAIDS__INCLUDED_)
#define IMAGE_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\GraphX\GraphXExp.h"

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

class MFCAIDS_API CGraphXImage
{
  public:
    BOOL ConvertTo(UINT nFileType, LPCTSTR sNewExt = NULL);
    BOOL ConvertToPCX();
    BOOL ConvertToBMP();
    void LoadFileSafely(LPCTSTR sPath);
    BOOL SaveChangesToFile();
    BOOL ResizeImage(UINT cx, UINT cy);
    BOOL SetColorMode(UINT nBitsPerPixel);
    FILEINFO* GetImgFileInfo();
    CString GetFileName();
    void GetFrame(CDC* pDC, CRect& rc);
    BOOL IsLoaded();
    void Render(CDC* pDC, CRect& rcDst, UINT nMode = PaintOriginal);
    void DrawClipRect(CDC* pDC, CRect rcDst, CRect rcClip, UINT nMode = PaintOriginal);
    void CloseFile();
    void LoadFile(LPCTSTR sPath);
    CGraphXImage();
    CGraphXImage(LPCTSTR sPath);
    virtual ~CGraphXImage();

    enum
    {
        PaintOriginal    = 0, // draw with original picture size
        PaintProportion  = 1, // draw proportional
        PaintProportion2 = 2  // draw original, if it's possible; else - draw proportional
    };

    CString m_sPath;
    FILEINFO m_fi;
    BITMAPHANDLE m_hbmp;
};

#endif // !defined(IMAGE_H__MFCAIDS__INCLUDED_)
