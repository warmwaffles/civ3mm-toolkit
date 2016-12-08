#include "stdafx.h"

#include "GraphXExp.h"
#include "GifCoder.h"

// wrapper for leadtools

static HANDLE s_redirect_filehandle;
static DWORD s_redirect_filestart;
static DWORD s_redirect_filelength;

L_INT
RedirectClose(L_INT /*hf*/, L_VOID L_FAR* /*pUserData*/)
{
    return TRUE;
}

L_UINT
RedirectWrite(L_INT /*FD*/, L_CHAR L_FAR* /*pBuf*/, L_INT uCount, L_VOID L_FAR* /*pUserData*/)
{
    // not implemented
    return (L_UINT)uCount;
}

L_INT
RedirectOpen(L_CHAR* /*pFileName*/, L_INT /*nMode*/, L_INT /*nShare*/, L_VOID L_FAR* /*pUserData*/)
{
    SetFilePointer(s_redirect_filehandle, s_redirect_filestart, 0, FILE_BEGIN);

    return 5;
}

L_INT32
RedirectSeek(L_INT /*fd*/, L_INT32 lnPos, L_INT nOrigin, L_VOID L_FAR* /*pUserData*/)
{
    switch (nOrigin) {
        case SEEK_SET:
            SetFilePointer(s_redirect_filehandle, lnPos + s_redirect_filestart, 0, FILE_BEGIN);
            break;
        case SEEK_CUR:
            SetFilePointer(s_redirect_filehandle, lnPos, 0, FILE_CURRENT);
            break;
        case SEEK_END:
            SetFilePointer(s_redirect_filehandle, s_redirect_filestart + s_redirect_filelength - lnPos, 0, FILE_BEGIN);
            break;
    }

    lnPos = (L_INT32)SetFilePointer(s_redirect_filehandle, 0, 0, FILE_CURRENT);

    if ((L_UINT32)lnPos < s_redirect_filestart)
        SetFilePointer(s_redirect_filehandle, s_redirect_filestart, 0, FILE_BEGIN);

    if ((L_UINT32)lnPos > s_redirect_filestart + s_redirect_filelength)
        SetFilePointer(s_redirect_filehandle, s_redirect_filestart + s_redirect_filelength, 0, FILE_BEGIN);

    return SetFilePointer(s_redirect_filehandle, 0, 0, FILE_CURRENT) - s_redirect_filestart;
}

L_UINT
RedirectRead(L_INT /*fd*/, L_CHAR* pBuf, L_INT nCount, L_VOID L_FAR* /*pUserData*/)
{
    L_UINT32 lnPos = SetFilePointer(s_redirect_filehandle, 0, 0, FILE_CURRENT);

    if (lnPos + nCount > s_redirect_filestart + s_redirect_filelength)
        nCount = s_redirect_filestart + s_redirect_filelength - lnPos;

    DWORD nRead;
    if (!ReadFile(s_redirect_filehandle, pBuf, nCount, &nRead, NULL))
        return 0;

    return (L_UINT)nRead;
}

HDC
GRAPHX::CreateGraphXDC(pBITMAPHANDLE pBitmap)
{
    HDC hdc = GraphXnamespace::L_CreateLeadDC(pBitmap);
    if (hdc == 0)
        THROW(new CGraphXException(FAILURE, _T("CreateGraphXDC")));
    return hdc;
}

L_BOOL
GRAPHX::DeleteGraphXDC(HDC hDC)
{
    if (!GraphXnamespace::L_DeleteLeadDC(hDC))
        THROW(new CGraphXException(FAILURE, _T("DeleteGraphXDC")));
    return TRUE;
}

L_VOID
GRAPHX::AccessBitmap(pBITMAPHANDLE pBitmap)
{
    GraphXnamespace::L_AccessBitmap(pBitmap);
}

L_INT
GRAPHX::AddBitmapNoise(pBITMAPHANDLE pBitmap, L_UINT uRange, L_UINT uChannel)
{

    L_INT res = GraphXnamespace::L_AddBitmapNoise(pBitmap, uRange, uChannel);
    return CheckSuccess(res, _T("AddBitmapNoise"));
}

L_INT
GRAPHX::AllocateBitmap(pBITMAPHANDLE pBitmap, L_INT nMemoryType)
{
    L_INT res = GraphXnamespace::L_AllocateBitmap(pBitmap, nMemoryType);
    return CheckSuccess(res, _T("AllocateBitmap"));
}

L_INT
GRAPHX::AverageFilterBitmap(pBITMAPHANDLE pBitmap, L_UINT uDim)
{
    L_INT res = GraphXnamespace::L_AverageFilterBitmap(pBitmap, uDim);
    return CheckSuccess(res, _T("AverageFilterBitmap"));
}

L_INT
GRAPHX::BinaryFilterBitmap(pBITMAPHANDLE pBitmap, LPBINARYFLT pFilter)
{
    L_INT res = GraphXnamespace::L_BinaryFilterBitmap(pBitmap, pFilter);
    return CheckSuccess(res, _T("BinaryFilterBitmap"));
}

L_INT
GRAPHX::ChangeBitmapContrast(pBITMAPHANDLE pBitmap, L_INT nChange)
{
    L_INT res = GraphXnamespace::L_ChangeBitmapContrast(pBitmap, nChange);
    return CheckSuccess(res, _T("ChangeBitmapContrast"));
}

L_INT
GRAPHX::ChangeBitmapHeight(pBITMAPHANDLE pBitmap, L_INT nHeight)
{
    L_INT res = GraphXnamespace::L_ChangeBitmapHeight(pBitmap, nHeight);
    return CheckSuccess(res, _T("ChangeBitmapHeight"));
}

L_INT
GRAPHX::ChangeBitmapHue(pBITMAPHANDLE pBitmap, L_INT nAngle)
{
    L_INT res = GraphXnamespace::L_ChangeBitmapHue(pBitmap, nAngle);
    return CheckSuccess(res, _T("ChangeBitmapHue"));
}

L_INT
GRAPHX::ChangeBitmapIntensity(pBITMAPHANDLE pBitmap, L_INT nChange)
{
    L_INT res = GraphXnamespace::L_ChangeBitmapIntensity(pBitmap, nChange);
    return CheckSuccess(res, _T("ChangeBitmapIntensity"));
}

L_INT
GRAPHX::ChangeBitmapSaturation(pBITMAPHANDLE pBitmap, L_INT nChange)
{
    L_INT res = GraphXnamespace::L_ChangeBitmapSaturation(pBitmap, nChange);
    return CheckSuccess(res, _T("ChangeBitmapSaturation"));
}

L_INT
GRAPHX::ChangeFromDDB(HDC hDC, pBITMAPHANDLE pBitmap, HBITMAP hBitmap, HPALETTE hPalette)
{
    L_INT res = GraphXnamespace::L_ChangeFromDDB(hDC, pBitmap, hBitmap, hPalette);
    return CheckSuccess(res, _T("ChangeFromDDB"));
}

L_INT
GRAPHX::ChangeFromDIB(pBITMAPHANDLE pBitmap, HANDLE hDIB)
{
    L_INT res = GraphXnamespace::L_ChangeFromDIB(pBitmap, hDIB);
    return CheckSuccess(res, _T("ChangeFromDIB"));
}

HANDLE
GRAPHX::ChangeToDIB(pBITMAPHANDLE pBitmap)
{
    HANDLE hndl = GraphXnamespace::L_ChangeToDIB(pBitmap);

    if (hndl == NULL) {
        throw new CGraphXException(FAILURE, _T("ChangeToDIB"));
    }

    return hndl;
}

L_INT
GRAPHX::ClearBitmap(pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_ClearBitmap(pBitmap);
    return CheckSuccess(res, _T("ClearBitmap"));
}

L_BOOL GRAPHX::ClipboardReady(L_VOID)
{
    return GraphXnamespace::L_ClipboardReady();
}

L_INT
GRAPHX::ColorMergeBitmap(pBITMAPHANDLE pBitmap, pBITMAPHANDLE* ppBitmap, L_UINT32 uFlags)
{
    L_INT res = GraphXnamespace::L_ColorMergeBitmap(pBitmap, ppBitmap, uFlags);
    return CheckSuccess(res, _T("ColorMergeBitmap"));
}

L_INT
GRAPHX::ColorResBitmap(pBITMAPHANDLE pBitmapSrc,
                       pBITMAPHANDLE pBitmapDst,
                       L_INT nBitsPerPixel,
                       L_UINT32 uFlags,
                       LPRGBQUAD pPalette,
                       HPALETTE hPalette,
                       L_UINT uColors,
                       COLORRESCALLBACK pfnCallback,
                       L_VOID* pUserData)
{
    L_INT res =
      GraphXnamespace::L_ColorResBitmap(pBitmapSrc, pBitmapDst, nBitsPerPixel, uFlags, pPalette, hPalette, uColors, pfnCallback, pUserData);
    return CheckSuccess(res, _T("ColorResBitmap"));
}

L_INT
GRAPHX::ColorSeparateBitmap(pBITMAPHANDLE pBitmap, pBITMAPHANDLE* ppBitmap, L_UINT32 uFlags)
{
    L_INT res = GraphXnamespace::L_ColorSeparateBitmap(pBitmap, ppBitmap, uFlags);
    return CheckSuccess(res, _T("ColorSeparateBitmap"));
}

L_INT
GRAPHX::CombineBitmap(pBITMAPHANDLE pBitmapDst,
                      L_INT nXDst,
                      L_INT nYDst,
                      L_INT nWidth,
                      L_INT nHeight,
                      pBITMAPHANDLE pBitmapSrc,
                      L_INT nXSrc,
                      L_INT nYSrc,
                      L_UINT32 uFlags)
{
    L_INT res = GraphXnamespace::L_CombineBitmap(pBitmapDst, nXDst, nYDst, nWidth, nHeight, pBitmapSrc, nXSrc, nYSrc, uFlags);
    return CheckSuccess(res, _T("CombineBitmap"));
}

L_INT GRAPHX::CompressBitmapWithStamp(pBITMAPHANDLE pBitmap,
                                      L_CHAR* pszFile,
                                      L_INT nFormat,
                                      L_INT nQFactor,
                                      L_INT nStampWidth,
                                      L_INT nStampHeight,
                                      L_INT nStampBits,
                                      L_INT nDitherFlag);

L_INT
GRAPHX::CompressBuffer(L_UCHAR* pBuffer)
{
    L_INT res = GraphXnamespace::L_CompressBuffer(pBuffer);
    return CheckSuccess(res, _T("CompressBuffer"));
}

L_INT
GRAPHX::ConvertBuffer(L_UCHAR* pBuffer,
                      L_INT nWidth,
                      L_INT nBitsPerPixelSrc,
                      L_INT nBitsPerPixelDst,
                      L_INT nOrderSrc,
                      L_INT nOrderDst,
                      LPRGBQUAD pPaletteSrc,
                      LPRGBQUAD pPaletteDst)
{
    L_INT res =
      GraphXnamespace::L_ConvertBuffer(pBuffer, nWidth, nBitsPerPixelSrc, nBitsPerPixelDst, nOrderSrc, nOrderDst, pPaletteSrc, pPaletteDst);
    return CheckSuccess(res, _T("ConvertBuffer"));
}

L_INT
GRAPHX::ConvertFromDDB(HDC hDC, pBITMAPHANDLE pBitmap, HBITMAP hBitmap, HPALETTE hPalette)
{
    L_INT res = GraphXnamespace::L_ConvertFromDDB(hDC, pBitmap, hBitmap, hPalette);
    return CheckSuccess(res, _T("ConvertFromDDB"));
}

HBITMAP
GRAPHX::ConvertToDDB(HDC hDC, pBITMAPHANDLE pBitmap)
{
    HBITMAP hBitmap = GraphXnamespace::L_ConvertToDDB(hDC, pBitmap);
    if (!hBitmap) {
        throw new CGraphXException(FAILURE, _T("ConvertToDDB"));
    }
    return hBitmap;
}

HANDLE
GRAPHX::ConvertToDIB(pBITMAPHANDLE pBitmap)
{
    HGLOBAL hSrcDIB = GraphXnamespace::L_ConvertToDIB(pBitmap);

    if (hSrcDIB == NULL) {
        throw new CGraphXException(FAILURE, _T("ConvertToDIB"));
    }

    return hSrcDIB;
}

L_INT
GRAPHX::CopyBitmap(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pBitmapSrc)
{
    L_INT res = GraphXnamespace::L_CopyBitmap(pBitmapDst, pBitmapSrc);
    return CheckSuccess(res, _T("CopyBitmap"));
}

L_INT
GRAPHX::CopyBitmapData(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pBitmapSrc)
{
    L_INT res = GraphXnamespace::L_CopyBitmapData(pBitmapDst, pBitmapSrc);
    return CheckSuccess(res, _T("CopyBitmapData"));
}

L_VOID
GRAPHX::CopyBitmapHandle(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pBitmapSrc)
{
    GraphXnamespace::L_CopyBitmapHandle(pBitmapDst, pBitmapSrc);
}

L_INT
GRAPHX::CopyBitmapRect(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pBitmapSrc, L_UINT uCol, L_UINT uRow, L_UINT uWidth, L_UINT uHeight)
{
    L_INT res = GraphXnamespace::L_CopyBitmapRect(pBitmapDst, pBitmapSrc, uCol, uRow, uWidth, uHeight);
    return CheckSuccess(res, _T("CopyBitmapRect"));
}

L_BOOL
GRAPHX::CopyFromClipboard(HWND hWnd, pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_CopyFromClipboard(hWnd, pBitmap);
    return CheckSuccess(res, _T("CopyFromClipboard"));
}

L_BOOL
GRAPHX::CopyToClipboard(HWND hWnd, pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_CopyToClipboard(hWnd, pBitmap);
    return CheckSuccess(res, _T("CopyToClipboard"));
}

L_INT
GRAPHX::CopyToClipboardExt(HWND hWnd, pBITMAPHANDLE pBitmap, L_UINT uFlags)
{
    L_INT res = GraphXnamespace::L_CopyToClipboardExt(hWnd, pBitmap, uFlags);
    return CheckSuccess(res, _T("CopyToClipboardExt"));
}

L_INT
GRAPHX::CreateBitmap(pBITMAPHANDLE pBitmap,
                     L_UINT uMemory,
                     L_UINT uWidth,
                     L_UINT uHeight,
                     L_UINT uBitsPerPixel,
                     L_UINT uOrder,
                     LPRGBQUAD pPalette,
                     L_UINT uViewPerspective,
                     L_UCHAR L_FAR* pData,
                     L_UINT32 dwSize)
{
    L_INT res =
      GraphXnamespace::L_CreateBitmap(pBitmap, uMemory, uWidth, uHeight, uBitsPerPixel, uOrder, pPalette, uViewPerspective, pData, dwSize);
    return CheckSuccess(res, _T("CreateBitmap"));
}

HPALETTE
GRAPHX::CreatePaintPalette(HDC hDC, pBITMAPHANDLE pBitmap)
{
    HPALETTE res = GraphXnamespace::L_CreatePaintPalette(hDC, pBitmap);
    return res;
}

L_INT
GRAPHX::DeskewBitmap(pBITMAPHANDLE pBitmap, L_INT32 L_FAR* pnAngle)
{
    return CheckSuccess(GraphXnamespace::L_DeskewBitmap(pBitmap, pnAngle), _T("DeskewBitmap"));
}

L_INT
GRAPHX::DespeckleBitmap(pBITMAPHANDLE pBitmap)
{
    return CheckSuccess(GraphXnamespace::L_DespeckleBitmap(pBitmap), _T("DespeckleBitmap"));
}

HPALETTE
GRAPHX::DupPalette(HPALETTE hPalette)
{
    return GraphXnamespace::L_DupPalette(hPalette);
}

L_INT
GRAPHX::FileConvert(const L_CHAR* pszFileSrc,
                    const L_CHAR* pszFileDst,
                    L_INT nType,
                    L_INT nWidth,
                    L_INT nHeight,
                    L_INT nBitsPerPixel,
                    L_INT nQFactor,
                    pLOADFILEOPTION pLoadOptions,
                    pSAVEFILEOPTION pSaveOptions,
                    pFILEINFO pFileInfo)
{
    L_INT res = GraphXnamespace::L_FileConvert(
      (LPSTR)pszFileSrc, (LPSTR)pszFileDst, nType, nWidth, nHeight, nBitsPerPixel, nQFactor, pLoadOptions, pSaveOptions, pFileInfo);
    return CheckSuccess(res, _T("FileConvert"));
}

L_INT
GRAPHX::FileInfo(const L_CHAR* pszFile, pFILEINFO pFileInfo, L_UINT flags, pLOADFILEOPTION pLoadOptions)
{
    L_INT res = GraphXnamespace::L_FileInfo((LPSTR)pszFile, pFileInfo, flags, pLoadOptions);
    return CheckSuccess(res, _T("FileInfo"));
}

L_INT
GRAPHX::FileInfo(int hFile, long nStart, long nLength, pFILEINFO pFileInfo, L_UINT flags)
{
    s_redirect_filehandle = (HANDLE)hFile;
    s_redirect_filestart  = nStart;
    s_redirect_filelength = nLength;
    GraphXnamespace::L_RedirectIO(RedirectOpen, RedirectRead, RedirectWrite, RedirectSeek, RedirectClose, NULL);

    L_INT res = GraphXnamespace::L_FileInfo("", pFileInfo, flags, NULL);

    GraphXnamespace::L_RedirectIO(NULL, NULL, NULL, NULL, NULL, NULL);

    return CheckSuccess(res, _T("FileInfo"));
}

L_INT
GRAPHX::FileInfoMemory(L_CHAR* pBuffer, pFILEINFO pFileInfo, L_INT32 nBufferSize, L_UINT flags, pLOADFILEOPTION pLoadOptions)
{
    L_INT res = GraphXnamespace::L_FileInfoMemory(pBuffer, pFileInfo, nBufferSize, flags, pLoadOptions);
    return CheckSuccess(res, _T("FileInfoMemory"));
}

L_INT
GRAPHX::FillBitmap(pBITMAPHANDLE pBitmap, COLORREF crFill)
{
    L_INT res = GraphXnamespace::L_FillBitmap(pBitmap, crFill);
    return CheckSuccess(res, _T("FillBitmap"));
}

L_VOID
GRAPHX::FreeBitmap(pBITMAPHANDLE pBitmap)
{
    GraphXnamespace::L_FreeBitmap(pBitmap);
}

L_INT
GRAPHX::GetBitmapColors(pBITMAPHANDLE pBitmap, L_INT nIndex, L_INT nCount, LPRGBQUAD pPalette)
{
    L_INT res = GraphXnamespace::L_GetBitmapColors(pBitmap, nIndex, nCount, pPalette);
    return CheckSuccess(res, _T("GetBitmapColors"));
}

L_INT32
GRAPHX::GetBitmapRowCol(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_INT nCol, L_UINT32 uBytes)
{
    L_INT32 res;
    res = GraphXnamespace::L_GetBitmapRowCol(pBitmap, pBuffer, nRow, nCol, uBytes);
    if (res >= 1)
        return res;
    return CheckSuccess(res, _T("GetBitmapRowCol"));
}

COLORREF
GRAPHX::GetPixelColor(pBITMAPHANDLE pBitmap, L_INT nRow, L_INT nCol)
{
    return GraphXnamespace::L_GetPixelColor(pBitmap, nRow, nCol);
}

L_INT
GRAPHX::GrayScaleBitmap(pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel)
{
    L_INT res = GraphXnamespace::L_GrayScaleBitmap(pBitmap, nBitsPerPixel);
    return CheckSuccess(res, _T("GrayScaleBitmap"));
}

L_INT
GRAPHX::IsGrayScaleBitmap(pBITMAPHANDLE pBitmap)
{
    return GraphXnamespace::L_IsGrayScaleBitmap(pBitmap);
}

L_INT
GRAPHX::HalfToneBitmap(pBITMAPHANDLE pBitmap, L_UINT uType)
{
    L_INT res = GraphXnamespace::L_HalfToneBitmap(pBitmap, uType);
    return CheckSuccess(res, _T("HalfToneBitmap"));
}

L_INT
GRAPHX::HalfToneBitmapExt(pBITMAPHANDLE pBitmap, L_UINT uType, L_INT32 nAngle)
{
    L_INT res = GraphXnamespace::L_HalfToneBitmapExt(pBitmap, uType, nAngle);
    return CheckSuccess(res, _T("HalfToneBitmapExt"));
}

L_INT
GRAPHX::InitBitmap(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight, L_INT nBitsPerPixel)
{
    L_INT res = GraphXnamespace::L_InitBitmap(pBitmap, nWidth, nHeight, nBitsPerPixel);
    return CheckSuccess(res, _T("InitBitmap"));
}

L_INT
GRAPHX::LoadBitmap(const L_CHAR* pszFile,
                   pBITMAPHANDLE pBitmap,
                   L_INT nBitsPerPixel,
                   L_INT nOrder,
                   pLOADFILEOPTION pLoadOptions,
                   pFILEINFO pFileInfo)
{
    // Read GIF-format files with our own decoder
    FILEINFO fi;
    L_INT res = GraphXnamespace::L_FileInfo((LPSTR)pszFile, &fi, 0, NULL);
    CheckSuccess(res, _T("LoadBitmap"));
    if (fi.Format == FILE_GIF) {
        CGifCoder coder;
        coder.LoadBitmap((LPSTR)pszFile, pBitmap, nBitsPerPixel, nOrder, pLoadOptions);
    } else
        res = GraphXnamespace::L_LoadBitmap((LPSTR)pszFile, pBitmap, nBitsPerPixel, nOrder, pLoadOptions, pFileInfo);

    return CheckSuccess(res, _T("LoadBitmap"));
}

L_INT
GRAPHX::LoadBitmapMemory(L_CHAR* pBuffer,
                         pBITMAPHANDLE pBitmap,
                         L_INT nBitsPerPixel,
                         L_INT nOrder,
                         L_INT32 nBufferSize,
                         pLOADFILEOPTION pLoadOptions,
                         pFILEINFO pFileInfo)
{
    // Read GIF-format files with our own decoder
    FILEINFO fi;
    L_INT res = GraphXnamespace::L_FileInfoMemory(pBuffer, &fi, nBufferSize, 0, NULL);
    CheckSuccess(res, _T("LoadBitmapMemory"));
    if (fi.Format == FILE_GIF) {
        CGifCoder coder;
        coder.LoadBitmapMemory(pBuffer, pBitmap, nBitsPerPixel, nOrder, nBufferSize, pLoadOptions, pFileInfo);
    } else
        res = GraphXnamespace::L_LoadBitmapMemory(pBuffer, pBitmap, nBitsPerPixel, nOrder, nBufferSize, pLoadOptions, pFileInfo);

    return CheckSuccess(res, _T("LoadBitmapMemory"));
}

L_INT
GRAPHX::LoadFile(L_CHAR* pszFile,
                 pBITMAPHANDLE pBitmap,
                 L_INT nBitsPerPixel,
                 L_INT nOrder,
                 L_UINT uFlags,
                 FILEREADCALLBACK pfnCallback,
                 L_VOID* pUserData,
                 pLOADFILEOPTION pLoadOptions,
                 pFILEINFO pFileInfo)
{
    // Read GIF-format files with our own decoder
    FILEINFO fi;
    L_INT res = GraphXnamespace::L_FileInfo((LPSTR)pszFile, &fi, 0, NULL);
    CheckSuccess(res, _T("LoadFile"));
    if (fi.Format == FILE_GIF) {
        CGifCoder coder;
        coder.LoadFile(pszFile, pBitmap, nBitsPerPixel, nOrder, uFlags, pfnCallback, pUserData, pLoadOptions);
    } else
        res = GraphXnamespace::L_LoadFile(pszFile, pBitmap, nBitsPerPixel, nOrder, uFlags, pfnCallback, pUserData, pLoadOptions, pFileInfo);
    return CheckSuccess(res, _T("LoadFile"));
}

L_INT
GRAPHX::LoadFileOffset(L_INT fd,
                       L_INT32 nOffsetBegin,
                       L_INT32 nBytesToLoad,
                       pBITMAPHANDLE pBitmap,
                       L_INT nBitsPerPixel,
                       L_INT nOrder,
                       L_UINT uFlags,
                       FILEREADCALLBACK pfnCallback,
                       L_VOID* pUserData,
                       pLOADFILEOPTION pLoadOptions,
                       pFILEINFO pFileInfo)
{
    L_INT res = GraphXnamespace::L_LoadFileOffset(
      fd, nOffsetBegin, nBytesToLoad, pBitmap, nBitsPerPixel, nOrder, uFlags, pfnCallback, pUserData, pLoadOptions, pFileInfo);
    return CheckSuccess(res, _T("LoadFileOffset"));
}

L_INT
GRAPHX::LoadMemory(L_CHAR* pBuffer,
                   pBITMAPHANDLE pBitmap,
                   L_INT nBitsPerPixel,
                   L_INT nOrder,
                   L_UINT uFlags,
                   FILEREADCALLBACK pfnCallback,
                   L_VOID* pUserData,
                   L_INT32 nBufferSize,
                   pLOADFILEOPTION pLoadOptions,
                   pFILEINFO pFileInfo)
{
    // Read GIF-format files with our own decoder
    FILEINFO fi;
    L_INT res = GraphXnamespace::L_FileInfoMemory(pBuffer, &fi, nBufferSize, 0, NULL);
    CheckSuccess(res, _T("LoadMemory"));
    if (fi.Format == FILE_GIF) {
        CGifCoder coder;
        coder.LoadMemory(pBuffer, pBitmap, nBitsPerPixel, nOrder, uFlags, pfnCallback, pUserData, nBufferSize, pLoadOptions, pFileInfo);
    } else
        res = GraphXnamespace::L_LoadMemory(
          pBuffer, pBitmap, nBitsPerPixel, nOrder, uFlags, pfnCallback, pUserData, nBufferSize, pLoadOptions, pFileInfo);
    return CheckSuccess(res, _T("LoadMemory"));
}

L_INT
GRAPHX::PaintDC(HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pSrc, LPRECT pClipSrc, LPRECT pDst, LPRECT pClipDst, L_UINT32 uROP3)
{
    // standard paint
    return CheckSuccess(GraphXnamespace::L_PaintDC(hDC, pBitmap, pSrc, pClipSrc, pDst, pClipDst, uROP3), _T("PaintDC"));
}

L_INT
GRAPHX::PaintDCEffect(HDC hDC,
                      pBITMAPHANDLE pBitmap,
                      LPRECT pSrc,
                      LPRECT pClipSrc,
                      LPRECT pDst,
                      LPRECT pClipDst,
                      L_UINT32 uROP3,
                      L_UINT uEffect)
{
    L_INT res = GraphXnamespace::L_PaintDCEffect(hDC, pBitmap, pSrc, pClipSrc, pDst, pClipDst, uROP3, uEffect);
    return CheckSuccess(res, _T("PaintDCEffect"));
}

L_INT
GRAPHX::PaintRgnDC(HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pSrc, LPRECT pClipSrc, LPRECT pDst, LPRECT pClipDst, L_UINT32 uROP3)
{
    L_INT res = GraphXnamespace::L_PaintRgnDC(hDC, pBitmap, pSrc, pClipSrc, pDst, pClipDst, uROP3);
    return CheckSuccess(res, _T("PaintRgnDC"));
}

HDC
GRAPHX::PrintBitmapExt(HDC hDC, pBITMAPHANDLE pBitmap, L_INT nX, L_INT nY, L_INT nWidth, L_INT nHeight, L_BOOL fEndDoc)
{
    HDC res = GraphXnamespace::L_PrintBitmapExt(hDC, pBitmap, nX, nY, nWidth, nHeight, fEndDoc);
    return res;
}

L_INT
GRAPHX::PutBitmapColors(pBITMAPHANDLE pBitmap, L_INT nIndex, L_INT nCount, LPRGBQUAD pPalette)
{
    L_INT res = GraphXnamespace::L_PutBitmapColors(pBitmap, nIndex, nCount, pPalette);
    return CheckSuccess(res, _T("PutBitmapColors"));
}

L_INT32 GRAPHX::PutBitmapRow(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_UINT32 uBytes);

L_INT32
GRAPHX::PutBitmapRowCol(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_INT nCol, L_UINT32 uBytes)
{
    L_INT32 res;
    res = GraphXnamespace::L_PutBitmapRowCol(pBitmap, pBuffer, nRow, nCol, uBytes);

    if (res >= 1)
        return res;
    return CheckSuccess(res, _T("PutBitmapRowCol"));
}

L_INT
GRAPHX::PutPixelColor(pBITMAPHANDLE pBitmap, L_INT nRow, L_INT nCol, COLORREF crColor)
{
    L_INT res = GraphXnamespace::L_PutPixelColor(pBitmap, nRow, nCol, crColor);
    return CheckSuccess(res, _T("PutPixelColor"));
}

L_VOID
GRAPHX::RedirectIO(REDIRECTOPEN pfnOpen,
                   REDIRECTREAD pfnRead,
                   REDIRECTWRITE pfnWrite,
                   REDIRECTSEEK pfnSeek,
                   REDIRECTCLOSE pfnClose,
                   L_VOID L_FAR* pUserData)
{
    GraphXnamespace::L_RedirectIO(pfnOpen, pfnRead, pfnWrite, pfnSeek, pfnClose, pUserData);
}

L_VOID
GRAPHX::ReleaseBitmap(pBITMAPHANDLE pBitmap)
{
    GraphXnamespace::L_ReleaseBitmap(pBitmap);
}

L_INT
GRAPHX::ResizeBitmap(pBITMAPHANDLE pBitmapSrc, pBITMAPHANDLE pBitmapDst)
{
    L_INT res = GraphXnamespace::L_ResizeBitmap(pBitmapSrc, pBitmapDst);
    return CheckSuccess(res, _T("ResizeBitmap"));
}

L_INT
GRAPHX::RotateBitmap(pBITMAPHANDLE pBitmap, L_INT nAngle, L_UINT uFlags, COLORREF crFill)
{
    L_INT res = GraphXnamespace::L_RotateBitmap(pBitmap, nAngle, uFlags, crFill);
    return CheckSuccess(res, _T("RotateBitmap"));
}

//--L_INT GRAPHX::RotateBitmapFine (pBITMAPHANDLE pBitmap,
/*                                   L_INT32 nAngle,
L_BOOL fResize,
COLORREF crFill)
{
L_INT res = GraphXnamespace::L_RotateBitmapFine(
pBitmap,
nAngle,
fResize,
crFill);
return CheckSuccess(res, _T("RotateBitmapFine"));
}*/

L_INT
GRAPHX::SaveBitmap(const L_CHAR* pszFile,
                   pBITMAPHANDLE pBitmap,
                   L_INT nFormat,
                   L_INT nBitsPerPixel,
                   L_INT nQFactor,
                   pSAVEFILEOPTION pSaveOptions)
{
    L_INT res = GraphXnamespace::L_SaveBitmap((L_CHAR*)pszFile, pBitmap, nFormat, nBitsPerPixel, nQFactor, pSaveOptions);
    return CheckSuccess(res, _T("SaveBitmap"));
}

L_INT
GRAPHX::SaveBitmapMemory(HANDLE* hHandle,
                         pBITMAPHANDLE pBitmap,
                         L_INT nFormat,
                         L_UINT32* uSize,
                         L_INT nBitsPerPixel,
                         L_INT nQFactor,
                         pSAVEFILEOPTION pSaveOptions)
{
    L_INT res = GraphXnamespace::L_SaveBitmapMemory(hHandle, pBitmap, nFormat, nBitsPerPixel, nQFactor, uSize, pSaveOptions);
    return CheckSuccess(res, _T("SaveBitmapMemory"));
}

L_INT
GRAPHX::SaveFile(L_CHAR* pszFile,
                 pBITMAPHANDLE pBitmap,
                 L_INT nFormat,
                 L_INT nBitsPerPixel,
                 L_INT nQFactor,
                 L_UINT uFlags,
                 FILESAVECALLBACK pfnCallback,
                 L_VOID* pUserData,
                 pSAVEFILEOPTION pSaveOptions)
{
    L_INT res =
      GraphXnamespace::L_SaveFile(pszFile, pBitmap, nFormat, nBitsPerPixel, nQFactor, uFlags, pfnCallback, pUserData, pSaveOptions);
    return CheckSuccess(res, _T("SaveFile"));
}

L_INT
GRAPHX::SaveFileOffset(L_INT fd,
                       L_INT32 nOffsetBegin,
                       L_INT32* nSizeWritten,
                       pBITMAPHANDLE pBitmap,
                       L_INT nFormat,
                       L_INT nBitsPerPixel,
                       L_INT nQFactor,
                       L_UINT uFlags,
                       FILESAVECALLBACK pfnCallback,
                       L_VOID* pUserData,
                       pSAVEFILEOPTION pSaveOptions)
{
    L_INT res = GraphXnamespace::L_SaveFileOffset(
      fd, nOffsetBegin, nSizeWritten, pBitmap, nFormat, nBitsPerPixel, nQFactor, uFlags, pfnCallback, pUserData, pSaveOptions);
    return CheckSuccess(res, _T("SaveFileOffset"));
}

L_UINT32
GRAPHX::SetDisplayMode(L_UINT32 uFlagPos, L_UINT32 uFlagSet)
{
    return GraphXnamespace::L_SetDisplayMode(uFlagPos, uFlagSet);
}

STATUSCALLBACK
GRAPHX::SetStatusCallBack(STATUSCALLBACK pfnCallback, L_VOID L_FAR* pUserData)
{
    return GraphXnamespace::L_SetStatusCallBack(pfnCallback, pUserData);
}

//--L_INT GRAPHX::SetExtFileOption (EXTFILEOPTION* pOption)
/*{
L_INT res = GraphXnamespace::L_SetExtFileOption(pOption);
return CheckSuccess(res, _T("SetExtFileOption"));
}
*/

L_INT
GRAPHX::SizeBitmap(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight)
{
    L_INT res = GraphXnamespace::L_SizeBitmap(pBitmap, nWidth, nHeight, SIZE_NORMAL);
    return CheckSuccess(res, _T("SizeBitmap"));
}

L_INT
GRAPHX::SizeBitmapExt(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight, L_UINT32 uFlags)
{
    L_INT res = GraphXnamespace::L_SizeBitmap(pBitmap, nWidth, nHeight, uFlags);
    return CheckSuccess(res, _T("SizeBitmapExt"));
}

L_BOOL
GRAPHX::IsTwainAvailable(HWND hWnd)
{
    return GraphXnamespace::L_IsTwainAvailable(hWnd);
}

L_INT
GRAPHX::TwainAcquire(HWND hWnd, pBITMAPHANDLE pBitmap)
{
    return CheckSuccess(GraphXnamespace::L_TwainAcquire(hWnd, pBitmap), _T("TwainAcquire"));
}

L_INT
GRAPHX::TwainAcquireExt(HWND hWnd, pBITMAPHANDLE pBitmap, L_UINT uFlags, TWAINCALLBACK pfnCallback, L_VOID* pUserData)
{
    return CheckSuccess(GraphXnamespace::L_TwainAcquireExt(hWnd, pBitmap, uFlags, pfnCallback, pUserData), _T("TwainAcquireExt"));
}

L_INT
GRAPHX::TwainEnumSources(HWND hWnd, ENUMSOURCECALLBACK pFunction, L_VOID* pUserData)
{
    return CheckSuccess(GraphXnamespace::L_TwainEnumSources(hWnd, pFunction, pUserData), _T("TwainEnumSources"));
}

L_INT
GRAPHX::TwainSelect(HWND hWnd)
{
    return CheckSuccess(GraphXnamespace::L_TwainSelect(hWnd), _T("TwainSelect"));
}

L_INT
GRAPHX::TwainSetProps(HWND hWnd, pTWAINPROPERTIES pTwain, L_UINT32 uFlags)
{
    L_INT res = CheckSuccess(GraphXnamespace::L_TwainSetProps(hWnd, pTwain, uFlags), _T("TwainSetProps"));

    TwainGetProps(pTwain);

    return res;
}

L_INT
GRAPHX::TwainGetProps(pTWAINPROPERTIES pTwain)
{
    memset(pTwain, '\0', sizeof *pTwain);

    return CheckSuccess(GraphXnamespace::L_TwainGetProps(pTwain), _T("TwainGetProps"));
}

// on error throw a CGraphXException or returns the error code?
static bool staticOnErrorThrow = true; // default is throw

void
GRAPHX::SetOnErrorThrow(bool onErrorThrow)
{
    staticOnErrorThrow = onErrorThrow;
}

bool
GRAPHX::GetOnErrorThrow()
{
    return staticOnErrorThrow;
}

L_INT
GRAPHX::CheckSuccess(L_INT res, const char* function)
{
    if (staticOnErrorThrow && (res < SUCCESS))
        throw new CGraphXException(res, function);
    else
        return res;
}

MULTIPAGEIMAGEHEADER::MULTIPAGEIMAGEHEADER()
{
    ZeroMemory(this, sizeof MULTIPAGEIMAGEHEADER);
    memcpy(this, _T("Multipage Image Format     1.0"), 40 * sizeof(TCHAR));
}

IMPLEMENT_DYNAMIC(CGraphXException, CException);

CGraphXException::CGraphXException(L_INT res, LPCTSTR function)
  : CException(TRUE)
{
    m_result = res;
    ZeroMemory(m_function, sizeof m_function);
    if (function)
        strncpy(m_function, function, sizeof m_function - 1);
}

CString
CGraphXException::GetErrorMessage(L_INT error)
{
    LPCTSTR msg = _T("Unknown Error.");

    switch (error) {
        case SUCCESS_ABORT:
            msg = _T("Function successful. You can quit now.");
            break;
        case SUCCESS:
            msg = _T("Function successful.");
            break;
        case FAILURE:
            msg = _T("Function is not successful.");
            break;
        case ERROR_NO_MEMORY:
            msg = _T("Error #1. Not enough memory available.");
            break;
        case ERROR_NO_BITMAP:
            msg = _T("Error #2. Invalid bitmap handle.");
            break;
        case ERROR_MEMORY_TOO_LOW:
            msg = _T("Error #3. Not enough memory available.");
            break;
        case ERROR_FILE_LSEEK:
            msg = _T("Error #4. Error seeking to position.");
            break;
        case ERROR_FILE_WRITE:
            msg = _T("Error #5. Error writing file.");
            break;
        case ERROR_FILE_GONE:
            msg = _T("Error #6. File not present - abort.");
            break;
        case ERROR_FILE_READ:
            msg = _T("Error #7. Error reading file.");
            break;
        case ERROR_INV_FILENAME:
            msg = _T("Error #8. Invalid filename specified.");
            break;
        case ERROR_FILE_FORMAT:
            msg = _T("Error #9. Invalid file format.");
            break;
        case ERROR_FILENOTFOUND:
            msg = _T("Error #10. File not found.");
            break;
        case ERROR_INV_RANGE:
            msg = _T("Error #11. Invalid width/height.");
            break;
        case ERROR_IMAGE_TYPE:
            msg = _T("Error #12. Image format recognized, but sub-type not supported.");
            break;
        case ERROR_INV_PARAMETER:
            msg = _T("Error #13. Invalid parmameter passed.");
            break;
        case ERROR_FILE_OPEN:
            msg = _T("Error #14. Not able to open file.");
            break;
        case ERROR_UNKNOWN_COMP:
            msg = _T("Error #15. Unknown compression format.");
            break;
        case ERROR_FEATURE_NOT_SUPPORTED:
            msg = _T("Error #16. Feature not supported.");
            break;
        case ERROR_NOT_256_COLOR:
            msg = _T("Error #17. VGA card only supports 256: colors (8 bit).");
            break;
        case ERROR_PRINTER:
            msg = _T("Error #18. Printer error.");
            break;
        case ERROR_CRC_CHECK:
            msg = _T("Error #19. Data CRC check error.");
            break;
        case ERROR_QFACTOR:
            msg = _T("Error #21. Invalid QFactor specified.");
            break;
        case ERROR_TARGAINSTALL:
            msg = _T("Error #22. TARGA not installed.");
            break;
        case ERROR_OUTPUTTYPE:
            msg = _T("Error #23. Invalid compression format.");
            break;
        case ERROR_XORIGIN:
            msg = _T("Error #24. X origin specified invalid.");
            break;
        case ERROR_YORIGIN:
            msg = _T("Error #25. Y origin specified invalid.");
            break;
        case ERROR_VIDEOTYPE:
            msg = _T("Error #26. Invalid video mode.");
            break;
        case ERROR_BITPERPIXEL:
            msg = _T("Error #27. Invalid bits/pixel.");
            break;
        case ERROR_WINDOWSIZE:
            msg = _T("Error #28. Invalid window size.");
            break;
        case ERROR_NORMAL_ABORT:
            msg = _T("Error #29. Escape key pressed.");
            break;
        case ERROR_NOT_INITIALIZED:
            msg = _T("Error #30. Internal error.");
            break;
        case ERROR_CU_BUSY:
            msg = _T("Error #31. Internal error.");
            break;
        case ERROR_INVALID_TABLE_TYPE:
            msg = _T("Error #32. Internal error.");
            break;
        case ERROR_UNEQUAL_TABLES:
            msg = _T("Error #33. Internal error.");
            break;
        case ERROR_INVALID_BUFFER:
            msg = _T("Error #34. Internal error.");
            break;
        case ERROR_MISSING_TILE_DATA:
            msg = _T("Error #35. Internal error.");
            break;
        case ERROR_INVALID_QVALUE:
            msg = _T("Error #36. Internal error.");
            break;
        case ERROR_INVALIDDATA:
            msg = _T("Error #37. Internal error.");
            break;
        case ERROR_INVALID_COMPRESSED_TYPE:
            msg = _T("Error #38. Internal error.");
            break;
        case ERROR_INVALID_COMPONENT_NUM:
            msg = _T("Error #39. Internal error.");
            break;
        case ERROR_INVALID_PIXEL_TYPE:
            msg = _T("Error #40. Internal error.");
            break;
        case ERROR_INVALID_PIXEL_SAMPLING:
            msg = _T("Error #41. Internal error.");
            break;
        case ERROR_INVALID_SOURCE_FILE:
            msg = _T("Error #42. Internal error.");
            break;
        case ERROR_INVALID_TARGET_FILE:
            msg = _T("Error #43. Internal error.");
            break;
        case ERROR_INVALID_IMAGE_DIMS:
            msg = _T("Error #44. Internal error.");
            break;
        case ERROR_INVALID_TILE_DIMS:
            msg = _T("Error #45. Internal error.");
            break;
        case ERROR_INVALID_PIX_BUFF_DIMS:
            msg = _T("Error #46. Internal error.");
            break;
        case ERROR_SEGMENT_OVERFLOW:
            msg = _T("Error #47. Internal error.");
            break;
        case ERROR_INVALID_SUBSAMPLING:
            msg = _T("Error #48. Internal error.");
            break;
        case ERROR_INVALID_Q_VIS_TABLE:
            msg = _T("Error #49. Internal error.");
            break;
        case ERROR_INVALID_DC_CODE_TABLE:
            msg = _T("Error #50. Internal error.");
            break;
        case ERROR_INVALID_AC_CODE_TABLE:
            msg = _T("Error #51. Internal error.");
            break;
        case ERROR_INSUFFICIENT_DATA:
            msg = _T("Error #52. Internal error.");
            break;
        case ERROR_MISSING_FUNC_POINTER:
            msg = _T("Error #53. Internal error.");
            break;
        case ERROR_TOO_MANY_DC_CODE_TABLES:
            msg = _T("Error #54. Internal error.");
            break;
        case ERROR_TOO_MANY_AC_CODE_TABLES:
            msg = _T("Error #55. Internal error.");
            break;
        case ERROR_INVALID_SUBIMAGE:
            msg = _T("Error #56. Internal error.");
            break;
        case ERROR_INVALID_ABORTION:
            msg = _T("Error #57. Internal error.");
            break;
        case ERROR_CU_NO_SUPPORT:
            msg = _T("Error #58. Internal error.");
            break;
        case ERROR_CU_FAILURE:
            msg = _T("Error #59. Internal error.");
            break;
        case ERROR_BAD_POINTER:
            msg = _T("Error #60. Internal error.");
            break;
        case ERROR_HEADER_DATA_FAILURE:
            msg = _T("Error #61. Internal error.");
            break;
        case ERROR_COMPRESSED_DATA_FAILURE:
            msg = _T("Error #62. Internal error.");
            break;
        case ERROR_FIXEDPAL_DATA:
            msg = _T("Error #63. Fixed palette data not found.");
            break;
        case ERROR_LOADFONT_DATA:
            msg = _T("Error #64. Fixed palette data not found.");
            break;
        case ERROR_NO_STAMP:
            msg = _T("Error #65. Stamp not found.");
            break;
        case ERROR_G3CODE_INVALID:
            msg = _T("Error #70. Invalid group 3 dat.");
            break;
        case ERROR_G3CODE_EOF:
            msg = _T("Error #71. Group3 code end of file.");
            break;
        case ERROR_G3CODE_EOL:
            msg = _T("Error #72. Group3 code end of code.");
            break;
        case ERROR_PREMATURE_EOF:
            msg = _T("Error #73. Premature EOF at scanline.");
            break;
        case ERROR_PREMATURE_EOL:
            msg = _T("Error #74. Premature end-of-line code.");
            break;
        case ERROR_UNCOMP_EOF:
            msg = _T("Error #75. Premature end-of-line code.");
            break;
        case ERROR_ACT_INCOMP:
            msg = _T("Error #76. Premature end-of-line code.");
            break;
        case ERROR_BAD_DECODE_STATE:
            msg = _T("Error #77. Premature end-of-line code.");
            break;
        case ERROR_VERSION_NUMBER:
            msg = _T("Error #78. Bad version number.");
            break;
        case ERROR_TWAIN_NODSM:
            msg = _T("Error #79. Source manager not found.");
            break;
        case ERROR_TWAIN_BUMMER:
            msg = _T("Error #80. Failure due to unknown causes.");
            break;
        case ERROR_TWAIN_LOWMEMORY:
            msg = _T("Error #81. Not enough memory to perform operation.");
            break;
        case ERROR_TWAIN_NODS:
            msg = _T("Error #82. No Data Source.");
            break;
        case ERROR_TWAIN_MAXCONNECTIONS:
            msg = _T("Error #83. DS is connected to max possible apps.");
            break;
        case ERROR_TWAIN_OPERATIONERROR:
            msg = _T("Error #84. DS or DSM reported error, app shouldn't.");
            break;
        case ERROR_TWAIN_BADCAP:
            msg = _T("Error #85. Unknown capability.");
            break;
        case ERROR_TWAIN_BADPROTOCOL:
            msg = _T("Error #86. Unrecognized MSG DG DAT combination.");
            break;
        case ERROR_TWAIN_BADVALUE:
            msg = _T("Error #87. Data parameter out of range.");
            break;
        case ERROR_TWAIN_SEQERROR:
            msg = _T("Error #88. DG DAT MSG out of expected sequence.");
            break;
        case ERROR_TWAIN_BADDEST:
            msg = _T("Error #89. Unknown destination App/Src in DSM_Entry.");
            break;
        case ERROR_TWAIN_CANCEL:
            msg = _T("Error #90. Operation was cancelled.");
            break;
        case ERROR_PANWINDOW_NOT_CREATED:
            msg = _T("Error #91. PanWindow was not created.");
            break;
        case ERROR_USER_ABORT:
            msg = _T("Error #100. User has aborted operation.");
            break;
        case ERROR_FPX_INVALID_FORMAT_ERROR:
            msg = _T("Error #101. FlashPix errors.");
            break;
        case ERROR_FPX_FILE_WRITE_ERROR:
            msg = _T("Error #102");
            break;
        case ERROR_FPX_FILE_READ_ERROR:
            msg = _T("Error #103");
            break;
        case ERROR_FPX_FILE_NOT_FOUND:
            msg = _T("Error #104");
            break;
        case ERROR_FPX_COLOR_CONVERSION_ERROR:
            msg = _T("Error #105");
            break;
        case ERROR_FPX_SEVER_INIT_ERROR:
            msg = _T("Error #106");
            break;
        case ERROR_FPX_LOW_MEMORY_ERROR:
            msg = _T("Error #107");
            break;
        case ERROR_FPX_IMAGE_TOO_BIG_ERROR:
            msg = _T("Error #108");
            break;
        case ERROR_FPX_INVALID_COMPRESSION_ERROR:
            msg = _T("Error #109");
            break;
        case ERROR_FPX_INVALID_RESOLUTION:
            msg = _T("Error #110");
            break;
        case ERROR_FPX_INVALID_FPX_HANDLE:
            msg = _T("Error #111");
            break;
        case ERROR_FPX_TOO_MANY_LINES:
            msg = _T("Error #112");
            break;
        case ERROR_FPX_BAD_COORDINATES:
            msg = _T("Error #113");
            break;
        case ERROR_FPX_FILE_SYSTEM_FULL:
            msg = _T("Error #114");
            break;
        case ERROR_FPX_MISSING_TABLE:
            msg = _T("Error #115");
            break;
        case ERROR_FPX_RETURN_PARAMETER_TOO_LARGE:
            msg = _T("Error #116");
            break;
        case ERROR_FPX_NOT_A_VIEW:
            msg = _T("Error #117");
            break;
        case ERROR_FPX_VIEW_IS_TRANFORMLESS:
            msg = _T("Error #118");
            break;
        case ERROR_FPX_ERROR:
            msg = _T("Error #119");
            break;
        case ERROR_FPX_UNIMPLEMENTED_FUNCTION:
            msg = _T("Error #120");
            break;
        case ERROR_FPX_INVALID_IMAGE_DESC:
            msg = _T("Error #121");
            break;
        case ERROR_FPX_INVALID_JPEG_TABLE:
            msg = _T("Error #122");
            break;
        case ERROR_FPX_ILLEGAL_JPEG_ID:
            msg = _T("Error #123");
            break;
        case ERROR_FPX_MEMORY_ALLOCATION_FAILED:
            msg = _T("Error #124");
            break;
        case ERROR_FPX_NO_MEMORY_MANAGEMENT:
            msg = _T("Error #125");
            break;
        case ERROR_FPX_OBJECT_CREATION_FAILED:
            msg = _T("Error #126");
            break;
        case ERROR_FPX_EXTENSION_FAILED:
            msg = _T("Error #127");
            break;
        case ERROR_FPX_FREE_NULL_PTR:
            msg = _T("Error #128");
            break;
        case ERROR_FPX_INVALID_TILE:
            msg = _T("Error #129");
            break;
        case ERROR_FPX_FILE_IN_USE:
            msg = _T("Error #130");
            break;
        case ERROR_FPX_FILE_CREATE_ERROR:
            msg = _T("Error #131");
            break;
        case ERROR_FPX_FILE_NOT_OPEN_ERROR:
            msg = _T("Error #132");
            break;
        case ERROR_FPX_USER_ABORT:
            msg = _T("Error #133");
            break;
        case ERROR_FPX_OLE_FILE_ERROR:
            msg = _T("Error #134");
            break;
        case ERROR_BAD_TAG:
            msg = _T("Error #140");
            break;
        case ERROR_INVALID_STAMP_SIZE:
            msg = _T("Error #141. Stamp size is too big or invalid bits/pixel, etc.");
            break;
        case ERROR_BAD_STAMP:
            msg = _T("Error #142. Stamp is present but data is corrupted.");
            break;
        //    case ERROR_EXPRESS_NOT_ENABLED:
        //        msg = _T("Error #. Document Express capability is required to use this function.");
        //        break;
        case ERROR_IMAGE_EMPTY:
            msg = _T("Error #144. Image is empty.");
            break;
        case ERROR_NO_CHANGE:
            msg = _T("Error #145. The image hasn't changed.");
            break;
        case ERROR_LZW_LOCKED:
            // msg = _T("Error #. LZW is locked.");
            msg = _T("Error #146. Graphical files compressed using LZW algorithm are not supported");
            break;
        case ERROR_FPXEXTENSIONS_LOCKED:
            msg = _T("Error #147. FPX Extensions is locked.");
            break;
        case ERROR_ANN_LOCKED:
            msg = _T("Error #148. The annotation object is locked.");
            break;
        case ERROR_EXCEPTION:
            msg = _T("Error #149. An unspecified exception occured - could be memory access violations.");
            break;
        case ERROR_DLG_CANCELED:
            msg = _T("Error #150. Common dialog was canceled by user.");
            break;
        case ERROR_DLG_FAILED:
            msg = _T("Error #151. An error occured during creation of the common dialog.");
            break;
        case ERROR_ISIS_NOCURSCANNER:
            msg = _T("Error #160. No ISIS Scanner selected.");
            break;
        case ERROR_ISIS_SCANDRIVER_NOT_LOADED:
            msg = _T("Error #161. ISIS Scanner driver failed to load.");
            break;
        case ERROR_ISIS_CANCEL:
            msg = _T("Error #162. ISIS operation canceled.");
            break;
        case ERROR_ISIS_BAD_TAG_OR_VALUE:
            msg = _T("Error #163. Bad ISIS Tag or Bad Tag Value.");
            break;
        case ERROR_ISIS_NOT_READY:
            msg = _T("Error #164. ISIS Scanner not ready.");
            break;
        case ERROR_ISIS_NO_PAGE:
            msg = _T("Error #165. No page in the ISIS Scanner.");
            break;
        case ERROR_ISIS_JAM:
            msg = _T("Error #166. ISIS Scanner Jam error.");
            break;
        case ERROR_ISIS_SCANNER_ERROR:
            msg = _T("Error #167. ISIS Scanner driver error.");
            break;
        case ERROR_ISIS_BUSY:
            msg = _T("Error #168. ISIS Scanner is busy.");
            break;
        case ERROR_ISIS_FILE_ERROR:
            msg = _T("Error #169. ISIS File driver error.");
            break;
        case ERROR_ISIS_NETWORK_ERROR:
            msg = _T("Error #170. ISIS Network error.");
            break;
        case ERROR_ISIS_NOT_INSTALLED:
            msg = _T("Error #171. ISIS is not installed.");
            break;
        case ERROR_DOC_NOT_INITIALIZED:
            msg = _T("Error #180");
            break;
        case ERROR_DOC_HANDLE:
            msg = _T("Error #181");
            break;
        case ERROR_DOC_EMPTY:
            msg = _T("Error #182");
            break;
        case ERROR_DOC_INVALID_FONT:
            msg = _T("Error #183");
            break;
        case ERROR_DOC_INVALID_PAGE:
            msg = _T("Error #184");
            break;
        case ERROR_DOC_INVALID_RULE:
            msg = _T("Error #185");
            break;
        case ERROR_DOC_INVALID_ZONE:
            msg = _T("Error #186");
            break;
        case ERROR_DOC_TYPE_ZONE:
            msg = _T("Error #187");
            break;
        case ERROR_DOC_INVALID_COLUMN:
            msg = _T("Error #188");
            break;
        case ERROR_DOC_INVALID_LINE:
            msg = _T("Error #189");
            break;
        case ERROR_DOC_INVALID_WORD:
            msg = _T("Error #190");
            break;
        case ERROR_OCR_LOCKED:
            msg = _T("Error #191. OCR is locked.");
            break;
        case ERROR_OCR_NOT_INITIALIZED:
            msg = _T("Error #192");
            break;
        case ERROR_OCR_MAX_REGIONS:
            msg = _T("Error #193");
            break;
        case ERROR_OCR_OPTION:
            msg = _T("Error #194");
            break;
        case ERROR_OCR_CONVERT_DIB:
            msg = _T("Error #195");
            break;
        case ERROR_OCR_CANCELED:
            msg = _T("Error #196");
            break;
        case ERROR_OCR_INVALID_OUTPUT:
            msg = _T("Error #197");
            break;
        case ERROR_OCR_BLOCKED:
            msg = _T("Error #198");
            break;
        case ERROR_OCR_RPCMEM:
            msg = _T("Error #199. These errors are used internally only by API code.");
            break;
        case ERROR_OCR_FATAL:
            msg = _T("Error #200. Fatal condition detected call technical support.");
            break;
        case ERROR_OCR_BADTAG:
            msg = _T("Error #201. Bad tag in tag value set.");
            break;
        case ERROR_OCR_BADVAL:
            msg = _T("Error #202. Value bad or out of range.");
            break;
        case ERROR_OCR_BADTYPE:
            msg = _T("Error #203. Wrong type used to make IO_TOKEN_T.");
            break;
        case ERROR_OCR_NOFILE:
            msg = _T("Error #204. Cannot access specified file.");
            break;
        case ERROR_OCR_BADTOK:
            msg = _T("Error #205. Invalid IO_TOKEN_T.");
            break;
        case ERROR_OCR_BADFMT:
            msg = _T("Error #206. Invalid input format.");
            break;
        case ERROR_OCR_BADMATCH:
            msg = _T("Error #207. Image incompatible with IO_TOKEN_T.");
            break;
        case ERROR_OCR_NOSUPPORT:
            msg = _T("Error #208. Param combination not supported or file format legal but not supported.");
            break;
        case ERROR_OCR_BADID:
            msg = _T("Error #209. Invalid index for language/lexicon.");
            break;
        case ERROR_OCR_NOLANG:
            msg = _T("Error #210. Language not loaded/installed.");
            break;
        case ERROR_OCR_LANGOVFL:
            msg = _T("Error #211. Too many langauge packs, remove one.");
            break;
        case ERROR_OCR_NOISRC:
            msg = _T("Error #212. No image source declared.");
            break;
        case ERROR_OCR_NOTIDL:
            msg = _T("Error #213. ICR must be idle to make this call.");
            break;
        case ERROR_OCR_NOVER:
            msg = _T("Error #214. ICR not blocked on verification.");
            break;
        case ERROR_OCR_NODRAW:
            msg = _T("Error #215. ICR not blocked on: progress draw.");
            break;
        case ERROR_OCR_MEMERR:
            msg = _T("Error #216. ICR process memory error.");
            break;
        case ERROR_OCR_BADRGN:
            msg = _T("Error #217. Region index error.");
            break;
        case ERROR_OCR_NOICR:
            msg = _T("Error #218. Server not found or down.");
            break;
        case ERROR_OCR_NOACTV:
            msg = _T("Error #219. There are no active io_tokens in API.");
            break;
        case ERROR_OCR_NOMORE:
            msg = _T("Error #220. Nothing (more) to read.");
            break;
        case ERROR_OCR_NOTWAIT:
            msg = _T("Error #221. ICR must be waiting to make this call.");
            break;
        case ERROR_OCR_LEXOVFL:
            msg = _T("Error #222. Too many lexicons, remove one.");
            break;
        case ERROR_OCR_PREPROC:
            msg = _T("Error #223. Bitmap preprocessing failed.");
            break;
        case ERROR_OCR_BADFILE:
            msg = _T("Error #224. Bad file or file content associated with the passed IO_TOKEN_T.");
            break;
        case ERROR_OCR_BADSCAN:
            msg = _T("Error #225. Scanner failure.");
            break;
        case ERROR_OCR_NOIMG:
            msg = _T("Error #226. Preprocessing, Recognition, or region retrieval with no image.");
            break;
        case ERROR_OCR_NOLICN:
            msg = _T("Error #227. Licensed version of API could not acquire a license from server.");
            break;
        case ERROR_OCR_NOLCSRV:
            msg = _T("Error #228. Licensed version of API could not find license server.");
            break;
        case ERROR_OCR_LMEMERR:
            msg = _T("Error #229. Local (API library) memory error.");
            break;
        case ERROR_OCR_RESCHNG:
            msg = _T("Error #230. Image resolution change within a document too many times.");
            break;
        case ERROR_OCR_BADPLGN:
            msg = _T("Error #231. Bad Polygon used to define region extents; probably beyond the dimensions of the source image or bad ")
                  _T("vertices.");
            break;
        case ERROR_OCR_NOSINK:
            msg = _T("Error #232. No sink token available for use with regions et. al.");
            break;
        case ERROR_OCR_NOSRC:
            msg = _T("Error #233. No source token available for use with user defined input et. al.");
            break;
        case ERROR_OCR_NOTOK:
            msg = _T("Error #234. Unable to specify token to unblock ICR - user managed I/O '_APP'.");
            break;
        case ERROR_OCR_IMBUFOVFL:
            msg = _T("Error #235. Image buffer full/overflow.");
            break;
        case ERROR_OCR_TMOUT:
            msg = _T("Error #236. Timeout error on IPC to server.");
            break;
        case ERROR_OCR_BADVRS:
            msg = _T("Error #237 . Server is for wrong version.");
            break;
        case ERROR_OCR_TAGNNW:
            msg = _T("Error #238. Tag cannot have value changed at this time.");
            break;
        case ERROR_OCR_SRVCAN:
            msg = _T("Error #239. Server cancelled processing on its initiative - page may be too complex for selected mode.");
            break;
        case ERROR_OCR_WRFAIL:
            msg = _T("Error #240. I/O write failed (no space).");
            break;
        case ERROR_OCR_SCNCAN:
            msg = _T("Error #241. Scan canceled by user at scanner (using button or Scanner UI).");
            break;
        case ERROR_OCR_RGOCCLD:
            msg = _T("Error #242. Region is totally occluded.");
            break;
        case ERROR_OCR_NOTORNT:
            msg = _T("Error #243. Could not orient text.");
            break;
        case ERROR_OCR_ACCDEN:
            msg = _T("Error #244. Access denied to this feature see your sales rep.");
            break;
        case ERROR_OCR_BADUOR:
            msg = _T("Error #245. Invalid UOR passed to the server.");
            break;
        case ERROR_RECORDING:
            msg = _T("Error #250. Wave device is currently recording.");
            break;
        case ERROR_COMPRESSOR:
            msg = _T("Error #251. Invalid compressor.");
            break;
        case ERROR_SOUND_DEVICE:
            msg = _T("Error #252. Problem with sound card.");
            break;
        case ERROR_DEVICE_INUSE:
            msg = _T("Error #253. The sound card is currently in use.");
            break;
        case ERROR_INV_TRACKTYPE:
            msg = _T("Error #254. Invalid track for operation.");
            break;
        case ERROR_NO_SOUNDCARD:
            msg = _T("Error #255. No sound card is presend.");
            break;
        case ERROR_NOT_RECORDING:
            msg = _T("Error #256. Not currently recording.");
            break;
        case ERROR_INV_MODE:
            msg = _T("Error #257. Invalid open mode for operation.");
            break;
        case ERROR_NO_VIDEO_MODULE:
            msg = _T("Error #258. No video module present on machine.");
            break;
        case ERROR_QUEUE_FULL:
            msg = _T("Error #259. The buffer queue is full.");
            break;
        case ERROR_HOST_RESOLVE:
            msg = _T("Error #270");
            break;
        case ERROR_CANT_INITIALIZE:
            msg = _T("Error #271");
            break;
        case ERROR_NO_CONNECTION:
            msg = _T("Error #272");
            break;
        case ERROR_HOST_NOT_FOUND:
            msg = _T("Error #273");
            break;
        case ERROR_NOT_SERVER:
            msg = _T("Error #274");
            break;
        case ERROR_NO_CONNECTIONS:
            msg = _T("Error #275");
            break;
        case ERROR_CONNECT_REFUSED:
            msg = _T("Error #276");
            break;
        case ERROR_IS_CONNECTED:
            msg = _T("Error #277. Computer is already connected.");
            break;
        case ERROR_NET_UNREACH:
            msg = _T("Error #278. Network is unreachable from host.");
            break;
        case ERROR_TIME_OUT:
            msg = _T("Error #279. Connect attempt time out without connecting.");
            break;
        case ERROR_NET_DOWN:
            msg = _T("Error #280. Network subsystem is down.");
            break;
        case ERROR_NO_BUFFERS:
            msg = _T("Error #281. No buffer space is available, too many connections.");
            break;
        case ERROR_NO_FILE_DESCR:
            msg = _T("Error #282. No more file handle available.");
            break;
        case ERROR_DATA_QUEUED:
            msg = _T("Error #283. Data has been queued to send later.");
            break;
        case ERROR_UNKNOWN:
            msg = _T("Error #284. Unkown network error has occured.");
            break;
        case ERROR_CONNECT_RESET:
            msg = _T("Error #285. Connection was reset - socket might be unusable.");
            break;
        case ERROR_TRANSFER_ABORTED:
            msg = _T("Error #286. Transfer was aborted.");
            break;
        case ERROR_DSHOW_FAILURE:
            msg = _T("Error #287. DirectShow isn't installed correctly.");
            break;
        case ERROR_REGISTRY_READ:
            msg = _T("Error #288. Error reading from the registry.");
            break;
        case ERROR_WAVE_FORMAT:
            msg = _T("Error #289. The wave format is not supported.");
            break;
        case ERROR_INSUFICIENT_BUFFER:
            msg = _T("Error #290. The buffer size is too small.");
            break;
        case ERROR_WAVE_CONVERT:
            msg = _T("Error #291. Error converting the wave format.");
            break;
        //    case ERROR_VIDEO_LOCKED:
        //        msg = _T("Error #292. Video capabilities not unlocked.");
        //        break;
        case ERROR_CAP_CONNECT:
            msg = _T("Error #293. Error connecting to capture device.");
            break;
        case ERROR_CAP_DISCONNECT:
            msg = _T("Error #294. Error disconnecting capture device.");
            break;
        case ERROR_DISK_ISFULL:
            msg = _T("Error #295. Not enough disk space.");
            break;
        case ERROR_CAP_OVERLAY:
            msg = _T("Error #296. Error setting capture overlay.");
            break;
        case ERROR_CAP_PREVIEW:
            msg = _T("Error #297. Error setting capture preview.");
            break;
        case ERROR_CAP_COPY:
            msg = _T("Error #298. Error copying to clipboard.");
            break;
        case ERROR_CAP_WINDOW:
            msg = _T("Error #299. Error with capture window.");
            break;
        case ERROR_CAP_ISCAPTURING:
            msg = _T("Error #300. Operation invalid during capture.");
            break;
        case ERROR_NO_STREAMS:
            msg = _T("Error #301. No streams to play.");
            break;
        case ERROR_CREATE_STREAM:
            msg = _T("Error #302. Error creating streams.");
            break;
        case ERROR_FRAME_DELETE:
            msg = _T("Error #303. Error deleting frames.");
            break;
        case ERROR_PAGE_NOT_FOUND:
            msg = _T("Error #310. Page not found.");
            break;
        case ERROR_DELETE_LAST_PAGE:
            msg = _T("Error #311. You cannot delete a page from a file containing only one page.");
            break;
        case ERROR_NO_HOTKEY:
            msg = _T("Error #312. There is no hotkey.");
            break;
        case ERROR_CANNOT_CREATE_HOTKEY_WINDOW:
            msg = _T("Error #313. Error creating the hotkey window.");
            break;
        case ERROR_MEDICAL_NOT_ENABLED:
            msg = _T("Error #314. Medical Express capability is required to use this function.");
            break;
        case ERROR_JBIG_NOT_ENABLED:
            msg = _T("Error #315. JBIG capability is required to use this function.");
            break;
        case ERROR_CAPTURE_STILL_IN_PROCESS:
            msg = _T("Error #320");
            break;
        case ERROR_INVALID_DELAY:
            msg = _T("Error #321. The user passed an invalid delay value.");
            break;
        case ERROR_INVALID_COUNT:
            msg = _T("Error #322. The user passed an invalid capture count.");
            break;
        case ERROR_INVALID_INTERVAL:
            msg = _T("Error #323. The user passed an invalid interval value.");
            break;
        //    case ERROR_HOTKEY_CONFILCTS_WITH_CANCELKEY:
        //        msg = _T("Error #324. The user specified the hotkey to be the same as the cancel key.");
        //        break;
        case ERROR_CAPTURE_INVALID_AREA_TYPE:
            msg = _T("Error #325. Wrong area type passed to L_CaptureArea().");
            break;
        case ERROR_CAPTURE_NO_OPTION_STRUCTURE:
            msg = _T("Error #326. Invalid options structure pointer.");
            break;
        case ERROR_CAPTURE_INVALID_FILL_PATTERN:
            msg = _T("Error #327. Invalid fill pattern.");
            break;
        case ERROR_CAPTURE_INVALID_LINE_STYLE:
            msg = _T("Error #328");
            break;
        case ERROR_CAPTURE_INVALID_INFOWND_POS:
            msg = _T("Error #329");
            break;
        case ERROR_CAPTURE_INVALID_INFOWND_SIZE:
            msg = _T("Error #330");
            break;
        case ERROR_CAPTURE_ZERO_AREA_SIZE:
            msg = _T("Error #331");
            break;
        case ERROR_CAPTURE_FILE_ACCESS_FAILED:
            msg = _T("Error #332. The specified filename couldn't be read.");
            break;
        case ERROR_CAPTURE_INVALID_32BIT_EXE_OR_DLL:
            msg = _T("Error #333. The specified filename is not a valid 32bit EXE/DLL.");
            break;
        case ERROR_CAPTURE_INVALID_RESOURCE_TYPE:
            msg = _T("Error #334. The specified resource type is not supported.");
            break;
        case ERROR_CAPTURE_INVALID_RESOURCE_INDEX:
            msg = _T("Error #335. The index resource is out of range.");
            break;
        case ERROR_CAPTURE_NO_ACTIVE_WINDOW:
            msg = _T("Error #336");
            break;
        case ERROR_CAPTURE_CANNOT_CAPTURE_WINDOW:
            msg = _T("Error #337");
            break;
        case ERROR_CAPTURE_STRING_ID_NOT_DEFINED:
            msg = _T("Error #338");
            break;
        case ERROR_CAPTURE_DELAY_LESS_THAN_ZERO:
            msg = _T("Error #339");
            break;
        case ERROR_CAPTURE_NO_MENU:
            msg = _T("Error #340");
            break;
    }

    return msg;
}

BOOL
CGraphXException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
    CString msg;
    char buff[2048];
    ZeroMemory(buff, sizeof buff);

    // cut to max len
    ZeroMemory(lpszError, nMaxError);
    strncpy(lpszError, buff, nMaxError - 1);

    // get msg
    msg = GetErrorMessage(m_result);

    // format message
    wsprintf(buff,
             _T("GRAPHX error\r\n")
             _T("function: %s\r\n")
             _T("code: %d\r\n")
             _T("description: %s"),
             m_function,
             (int)m_result,
             (LPCSTR)msg);

    // no help context
    if (pnHelpContext)
        *pnHelpContext = 0;

    // ok error message returned
    return true;
}

//--L_INT GRAPHX::GetExtFileOption(EXTFILEOPTION* pOption)
/*{
L_INT res = GraphXnamespace::L_GetExtFileOption(pOption);
return CheckSuccess(res, _T("L_GetExtFileOption"));

  }
*/

L_INT
GRAPHX::GetDefaultLoadFileOption(pLOADFILEOPTION pLoadOptions)
{
    L_INT res = GraphXnamespace::L_GetDefaultLoadFileOption(pLoadOptions);
    return CheckSuccess(res, _T("GetDefaultLoadFileOption"));
}

L_INT
GRAPHX::GetDefaultSaveFileOption(pSAVEFILEOPTION pSaveOptions)
{
    L_INT res = GraphXnamespace::L_GetDefaultSaveFileOption(pSaveOptions);
    return CheckSuccess(res, _T("GetDefaultSaveFileOption"));
}

L_INT
GRAPHX::CreateBitmapList(HBITMAPLIST* phList)
{
    L_INT res = GraphXnamespace::L_CreateBitmapList(phList);
    return CheckSuccess(res, _T("CreateBitmapList"));
}

L_INT
GRAPHX::DestroyBitmapList(HBITMAPLIST hList)
{
    L_INT res = GraphXnamespace::L_DestroyBitmapList(hList);
    return CheckSuccess(res, _T("DestroyBitmapList"));
}

L_INT
GRAPHX::LoadBitmapList(LPSTR lpszFile,
                       HBITMAPLIST* phList,
                       L_INT nBitsTo,
                       L_INT nColorOrder,
                       pLOADFILEOPTION pLoadOptions,
                       pFILEINFO pFileInfo)
{
    L_INT res = GraphXnamespace::L_LoadBitmapList(lpszFile, phList, nBitsTo, nColorOrder, pLoadOptions, pFileInfo);
    return CheckSuccess(res, _T("LoadBitmapList"));
}

L_INT
GRAPHX::SaveBitmapList(LPSTR lpszFile, HBITMAPLIST hList, L_INT nFormat, L_INT nBits, L_INT nQFactor, pSAVEFILEOPTION pSaveOptions)
{
    L_INT res = GraphXnamespace::L_SaveBitmapList(lpszFile, hList, nFormat, nBits, nQFactor, pSaveOptions);
    return CheckSuccess(res, _T("SaveBitmapList"));
}

L_INT
GRAPHX::GetBitmapListCount(HBITMAPLIST hList, L_UINT L_FAR* puCount)
{
    L_INT res = GraphXnamespace::L_GetBitmapListCount(hList, puCount);
    return CheckSuccess(res, _T("GetBitmapListCount"));
}

L_INT
GRAPHX::GetBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_GetBitmapListItem(hList, uIndex, pBitmap);
    return CheckSuccess(res, _T("GetBitmapListItem"));
}

L_INT
GRAPHX::SetBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_SetBitmapListItem(hList, uIndex, pBitmap);
    return CheckSuccess(res, _T("SetBitmapListItem"));
}

L_INT
GRAPHX::InsertBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_InsertBitmapListItem(hList, uIndex, pBitmap);
    return CheckSuccess(res, _T("InsertBitmapListItem"));
}

L_INT
GRAPHX::RemoveBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_RemoveBitmapListItem(hList, uIndex, pBitmap);
    return CheckSuccess(res, _T("RemoveBitmapListItem"));
}

L_INT
GRAPHX::DeleteBitmapListItems(HBITMAPLIST hList, L_UINT uIndex, L_UINT uCount)
{
    L_INT res = GraphXnamespace::L_DeleteBitmapListItems(hList, uIndex, uCount);
    return CheckSuccess(res, _T("DeleteBitmapListItems"));
}

//////////////////////////////////////////////////////////////////////////////////
//				Implementation part for ImageList control
//

L_VOID GRAPHX::UseImageListControl(L_VOID)
{
    GraphXnamespace::L_UseImageListControl();
}

HWND
GRAPHX::CreateImageListControl(L_UINT32 dwStyle, L_INT x, L_INT y, L_INT nWidth, L_INT nHeight, HWND hWndParent, L_INT nID, COLORREF crBack)
{
    return GraphXnamespace::L_CreateImageListControl(dwStyle, x, y, nWidth, nHeight, hWndParent, nID, crBack);
}

L_INT
GRAPHX::ImgListSetSelOptions(HWND hWnd, LILSELOPTION* Opt)
{
    L_INT res = SendMessage(hWnd, L_ILM_SETSELOPTIONS, 0, (LPARAM)Opt);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListSetSelOptions"));

    return res;
}

L_INT
GRAPHX::ImgListGetSelOptions(HWND hWnd, LILSELOPTION* Opt)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETSELOPTIONS, 0, (LPARAM)Opt);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetSelOptions"));

    return res;
}

L_INT
GRAPHX::ImgListClear(HWND hWnd)
{
    L_INT res = SendMessage(hWnd, L_ILM_CLEAR, 0, 0);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListClear"));

    return res;
}

L_INT
GRAPHX::ImgListGetItemCount(HWND hWnd)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETITEMCOUNT, 0, 0);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetItemCount"));

    return res;
}

L_INT
GRAPHX::ImgListInsert(HWND hWnd, LILITEM* pItem)
{
    L_INT res = SendMessage(hWnd, L_ILM_INSERT, 0, (LPARAM)pItem);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListInsert"));

    return res;
}

L_INT
GRAPHX::ImgListInsertAt(HWND hWnd, int nIndex, LILITEM* pItem)
{
    L_INT res = SendMessage(hWnd, L_ILM_INSERTAT, nIndex, (LPARAM)pItem);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListInsertAt"));

    return res;
}

L_INT
GRAPHX::ImgListGetItem(HWND hWnd, L_UINT nIndex, LILITEM* pItem)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETITEM, (WPARAM)nIndex, (LPARAM)pItem);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetItem"));

    return res;
}

L_INT
GRAPHX::ImgListSetItem(HWND hWnd, L_UINT nIndex, LILITEM* pItem)
{
    L_INT res = SendMessage(hWnd, L_ILM_SETITEM, (WPARAM)nIndex, (LPARAM)pItem);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListSetItem"));

    return res;
}

L_INT
GRAPHX::ImgListGetItemOptions(HWND hWnd, LILITEMOPTION* pOpt)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETITEMOPTIONS, (WPARAM)0, (LPARAM)pOpt);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetItemOptions"));

    return res;
}

L_INT
GRAPHX::ImgListSetItemOptions(HWND hWnd, LILITEMOPTION* pOpt)
{
    L_INT res = SendMessage(hWnd, L_ILM_SETITEMOPTIONS, (WPARAM)0, (LPARAM)pOpt);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListSetItemOptions"));

    return res;
}

L_INT
GRAPHX::ImgListRemove(HWND hWnd, int nItem)
{
    L_INT res = SendMessage(hWnd, L_ILM_REMOVE, (WPARAM)nItem, (LPARAM)0);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListRemove"));

    return res;
}

L_INT
GRAPHX::ImgListGetSelItems(HWND hWnd, LILITEM* pItems)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETSELITEMS, (WPARAM)0, (LPARAM)pItems);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetSelItems"));

    return res;
}

L_INT
GRAPHX::ImgListGetSelCount(HWND hWnd)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETSELCOUNT, (WPARAM)0, (LPARAM)0);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetSelCount"));

    return res;
}

L_INT
GRAPHX::ImgListEnsureVisible(HWND hWnd, int nItem)
{
    return SendMessage(hWnd, L_ILM_ENSUREVISIBLE, (WPARAM)nItem, (LPARAM)0);
}

L_INT
GRAPHX::ImgListGetPage(HWND hWnd)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETPAGE, (WPARAM)0, (LPARAM)0);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetPage"));

    return res;
}

L_INT
GRAPHX::ImgListGetPagesCount(HWND hWnd)
{
    L_INT res = SendMessage(hWnd, L_ILM_GETPAGECOUNT, (WPARAM)0, (LPARAM)0);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListGetPagesCount"));

    return res;
}

L_INT
GRAPHX::ImgListHitTest(HWND hWnd, POINT* pt)
{
    L_INT res = SendMessage(hWnd, L_ILM_HITTEST, (WPARAM)0, (LPARAM)pt);
    if (res < 0)
        return CheckSuccess(res, _T("ImgListHitTest"));

    return res;
}

L_INT
GRAPHX::SetBitmapRgnHandle(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, HRGN hRgn, L_UINT uCombineMode)
{
    L_INT res = GraphXnamespace::L_SetBitmapRgnHandle(pBitmap, pXForm, hRgn, uCombineMode);
    return CheckSuccess(res, _T("SetBitmapRgnHandle"));
}

L_INT
GRAPHX::SetBitmapRgnColor(pBITMAPHANDLE pBitmap, COLORREF crColor, L_UINT uCombineMode)
{
    L_INT res = GraphXnamespace::L_SetBitmapRgnColor(pBitmap, crColor, uCombineMode);
    return CheckSuccess(res, _T("SetBitmapRgnColor"));
}

L_INT
GRAPHX::SetBitmapRgnEllipse(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, RECT L_FAR* pRect, L_UINT uCombineMode)
{
    L_INT res = GraphXnamespace::L_SetBitmapRgnEllipse(pBitmap, pXForm, pRect, uCombineMode);
    return CheckSuccess(res, _T("SetBitmapRgnEllipse"));
}

L_INT
GRAPHX::SetBitmapRgnPolygon(pBITMAPHANDLE pBitmap,
                            pRGNXFORM pXForm,
                            POINT L_FAR* pPoints,
                            L_UINT uPoints,
                            L_UINT uFillMode,
                            L_UINT uCombineMode)
{
    L_INT res = GraphXnamespace::L_SetBitmapRgnPolygon(pBitmap, pXForm, pPoints, uPoints, uFillMode, uCombineMode);
    return CheckSuccess(res, _T("SetBitmapRgnPolygon"));
}

L_INT
GRAPHX::SetBitmapRgnRect(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, RECT L_FAR* pRect, L_UINT uCombineMode)
{
    L_INT res = GraphXnamespace::L_SetBitmapRgnRect(pBitmap, pXForm, pRect, uCombineMode);
    return CheckSuccess(res, _T("SetBitmapRgnRect"));
}

L_INT
GRAPHX::SetBitmapRgnRoundRect(pBITMAPHANDLE pBitmap,
                              pRGNXFORM pXForm,
                              RECT L_FAR* pRect,
                              L_INT nWidthEllipse,
                              L_INT nHeightEllipse,
                              L_UINT uCombineMode)
{
    L_INT res = GraphXnamespace::L_SetBitmapRgnRoundRect(pBitmap, pXForm, pRect, nWidthEllipse, nHeightEllipse, uCombineMode);
    return CheckSuccess(res, _T("SetBitmapRgnRoundRect"));
}

L_INT
GRAPHX::SetBitmapRgnFromMask(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, pBITMAPHANDLE pMask, L_UINT uCombineMode)
{
    L_INT res = GraphXnamespace::L_SetBitmapRgnFromMask(pBitmap, pXForm, pMask, uCombineMode);
    return CheckSuccess(res, _T("SetBitmapRgnFromMask"));
}

L_BOOL
GRAPHX::BitmapHasRgn(pBITMAPHANDLE pBitmap)
{
    return GraphXnamespace::L_BitmapHasRgn(pBitmap);
}

L_INT
GRAPHX::GetBitmapRgnHandle(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, HRGN L_FAR* phRgn)
{
    L_INT res = GraphXnamespace::L_GetBitmapRgnHandle(pBitmap, pXForm, phRgn);
    return CheckSuccess(res, _T("GetBitmapRgnHandle"));
}

L_INT
GRAPHX::FreeBitmapRgn(pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_FreeBitmapRgn(pBitmap);
    return CheckSuccess(res, _T("FreeBitmapRgn"));
}

L_INT
GRAPHX::OffsetBitmapRgn(pBITMAPHANDLE pBitmap, L_INT nRowOffset, L_INT nColOffset)
{
    L_INT res = GraphXnamespace::L_OffsetBitmapRgn(pBitmap, nRowOffset, nColOffset);
    return CheckSuccess(res, _T("OffsetBitmapRgn"));
}

L_INT
GRAPHX::FrameBitmapRgn(HDC hDC, pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, L_UINT uType)
{
    L_INT res = GraphXnamespace::L_FrameBitmapRgn(hDC, pBitmap, pXForm, uType);
    return CheckSuccess(res, _T("FrameBitmapRgn"));
}

L_INT
GRAPHX::GetBitmapRgnArea(pBITMAPHANDLE pBitmap, L_UINT32 L_FAR* puArea)
{
    L_INT res = GraphXnamespace::L_GetBitmapRgnArea(pBitmap, puArea);
    return CheckSuccess(res, _T("GetBitmapRgnArea"));
}

L_INT
GRAPHX::GetBitmapRgnBounds(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, RECT L_FAR* pRect)
{
    L_INT res = GraphXnamespace::L_GetBitmapRgnBounds(pBitmap, pXForm, pRect);
    return CheckSuccess(res, _T("GetBitmapRgnBounds"));
}

L_BOOL
GRAPHX::IsPtInBitmapRgn(pBITMAPHANDLE pBitmap, L_INT nRow, L_INT nCol)
{
    return GraphXnamespace::L_IsPtInBitmapRgn(pBitmap, nRow, nCol);
}

L_INT
GRAPHX::CreateMaskFromBitmapRgn(pBITMAPHANDLE pBitmap, pBITMAPHANDLE pMask)
{
    L_INT res = GraphXnamespace::L_CreateMaskFromBitmapRgn(pBitmap, pMask);
    return CheckSuccess(res, _T("CreateMaskFromBitmapRgn"));
}

L_INT
GRAPHX::DlgFileOpen(HWND hWnd,
                    OPENFILENAME* pOpenFileName,
                    FILEOPENPARM L_FAR* pFileOpenParm,
                    L_UINT32 uFlags,
                    LTCOMMDLGHELPCB pfnCallback,
                    L_VOID L_FAR* pUserData)
{
    L_INT res = GraphXnamespace::L_DlgFileOpen(hWnd, pOpenFileName, pFileOpenParm, uFlags, pfnCallback, pUserData);
    return CheckSuccess(res, _T("DlgFileOpen"));
}

L_INT
GRAPHX::DlgFileSave(HWND hWnd,
                    OPENFILENAME* pOpenFileName,
                    FILESAVEPARM L_FAR* pFileSaveParm,
                    L_UINT32 uFlags,
                    LPFILESAVEFORMAT pSaveFormats,
                    L_INT nSaveFormats,
                    LTCOMMDLGHELPCB pfnCallback,
                    L_VOID L_FAR* pUserData)
{
    L_INT res =
      GraphXnamespace::L_DlgFileSave(hWnd, pOpenFileName, pFileSaveParm, uFlags, pSaveFormats, nSaveFormats, pfnCallback, pUserData);
    return CheckSuccess(res, _T("DlgFileSave"));
}

L_INT
GRAPHX::SetPaintIntensity(pBITMAPHANDLE pBitmap, L_INT nValue)
{
    L_INT res = GraphXnamespace::L_SetPaintIntensity(pBitmap, nValue);
    return CheckSuccess(res, _T("SetPaintIntensity"));
}

L_INT
GRAPHX::SetPaintContrast(pBITMAPHANDLE pBitmap, L_INT nValue)
{
    L_INT res = GraphXnamespace::L_SetPaintContrast(pBitmap, nValue);
    return CheckSuccess(res, _T("SetPaintContrast"));
}

L_INT
GRAPHX::GetPaintIntensity(pBITMAPHANDLE pBitmap)
{
    return GraphXnamespace::L_GetPaintIntensity(pBitmap);
}

L_INT
GRAPHX::GetPaintContrast(pBITMAPHANDLE pBitmap)
{
    return GraphXnamespace::L_GetPaintContrast(pBitmap);
}

L_INT
GRAPHX::TrimBitmap(pBITMAPHANDLE pBitmap, L_INT nCol, L_INT nRow, L_UINT uWidth, L_UINT uHeight)
{
    L_INT res = GraphXnamespace::L_TrimBitmap(pBitmap, nCol, nRow, uWidth, uHeight);
    return CheckSuccess(res, _T("TrimBitmap"));
}

L_INT
GRAPHX::ChangeBitmapViewPerspective(pBITMAPHANDLE pDstBitmap, pBITMAPHANDLE pSrcBitmap, L_INT ViewPerspective)
{
    L_INT res = GraphXnamespace::L_ChangeBitmapViewPerspective(pDstBitmap, pSrcBitmap, ViewPerspective);
    return CheckSuccess(res, _T("ChangeBitmapViewPerspective"));
}

///////////////////////////////////////////////////////////////////////////////////////
// Description	    : Resizes 1-bit images (without loss of quality)
//
// Function name	: GRAPHX::SmartResize
// Return type		: BOOL					- TRUE, if resizable succeeded
//
// Argument         : BITMAPHANDLE* pBmpSrc	- Pointer to the source bitmap handle, that
//											  is needed to be resized
//
// Argument         : BITMAPHANDLE* pBmpDst	- Pointer to the resulting bitmap handle.
//											  This bitmap mustn't be allocated.
//
// Argument         : double dScale			- scale factor(0..1). New image size is
//											  OldSize*dScale.
//
BOOL
GRAPHX::SmartResize(BITMAPHANDLE* pBmpSrc, BITMAPHANDLE* pBmpDst, double dScale)
{
    ASSERT(pBmpSrc);
    ASSERT(pBmpDst);
    ASSERT(pBmpSrc->BitsPerPixel == 1);         // only 1-bit bitmaps supported
    ASSERT(pBmpSrc->Flags.Compressed == false); // and uncompressed
    ASSERT(dScale > 0 && dScale < 1);

    // initializing the grayscale palette
    RGBQUAD grayscalePal[256];
    RGBQUAD* pPal = grayscalePal;
    for (int i = 0; i < 256; i++) {
        pPal->rgbRed = pPal->rgbGreen = pPal->rgbBlue = (BYTE)i;
        pPal->rgbReserved                             = 0;
        pPal++;
    }

    // create new botmap with grayscale palette and new size.
    int w = int(pBmpSrc->Width * dScale);
    int h = int(pBmpSrc->Height * dScale);

    // not allow zero sizes (at least pixel)
    if (w == 0)
        w = 1;

    if (h == 0)
        h = 1;

    GRAPHX::CreateBitmap(pBmpDst,
                         TYPE_CONV,
                         w,
                         h, // new sizes
                         8,
                         0,
                         grayscalePal,
                         TOP_LEFT,
                         NULL,
                         0);

    if (pBmpDst->Width == 0 || pBmpDst->Height == 0)
        return false;

    // get the number of pixels in a source bitmap
    // to be represented by one pixel in a destination bitmap
    int total = int(1.0 / dScale);

    // get pointer to image data of the source and destination bitmaps
    BYTE* pSrcBits = pBmpSrc->Addr.Windows.pData;
    BYTE* pDstBits = pBmpDst->Addr.Windows.pData;

    // retrieve 2 first palette entries from source bitmap
    PALETTEENTRY PalEntries[2];
    GetPaletteEntries(pBmpSrc->hPalette, 0, 2, (PALETTEENTRY*)&PalEntries);

    // get additional parameters
    int iSrcWidthBytes = pBmpSrc->BytesPerLine;
    int iAlignBytes    = pBmpDst->BytesPerLine - pBmpDst->Width;

    // get the intensity for the color with indexes 0 and 1
    int nIntensity[2];
    nIntensity[0] = PalEntries[0].peRed + PalEntries[0].peGreen + PalEntries[0].peBlue;
    nIntensity[1] = PalEntries[1].peRed + PalEntries[1].peGreen + PalEntries[1].peBlue;
    ;

    // set flag indicating that the intensity of the  color[0] is more than of color[1]
    BOOL fZeroIsWhite = nIntensity[0] > nIntensity[1];

    //DWORD d1, d2;
    //d1= GetTickCount();

    int cy, cx;
    int whitePixCount; // the number of white pixels in the rectangle of the original bitmap
    // to be replaced by one pixel in the resulting bitmap

    int t1, t2;

    for (cy = 0; cy < pBmpDst->Height; cy++) // for every line of the resulting bitmap
    {
        t1 = int(cy / dScale);
        for (cx = 0; cx < pBmpDst->Width; cx++) // for every pixel of the line of the resulting bitmap
        {
            whitePixCount = 0;
            t2            = int(cx / dScale);

            /*			__asm
			{
			xor		ecx, ecx		// whiePixCount= 0
			xor		edi, edi		// i= 0;
			mov		esi, [pSrcBits]
			mov		ebx, [t1]		// ebx= t1
			
			  NextLine:
			  mov		eax, [iSrcWidthBytes]	// iSrcWidthBytes*
			  mul		ebx						// *(t1+i)
			  
				add		eax, [t2]
				add		eax, edi
				mov		edx, byte ptr [esi+eax]	// whitePixCount+= pSrcBits[iSrcWidthBytes*(t1+ i) + t2+ i];
				add		ecx, edx
				
				  
					inc		edi				// ++i
					inc		ebx				// ++t1
					
					  cmp		edi, [nScale]	// if (i< nScale)
					  jl		NextLine		//   goto NextLine;
					  
						mov		[whitePixCount], ecx
						}
			*/
            for (i = 0; i < total; ++i) // for every line of the current source bitmap rectangle
                whitePixCount += (pSrcBits[iSrcWidthBytes * (t1 + i) + ((t2 + i) >> 3)] >> (7 - (t2 + i) % 8)) & 1;

            // assign the appropriate grayscale value to the pixel of the resulting bitmap
            fZeroIsWhite ? * pDstBits = (BYTE)(255 * (total - whitePixCount) / total) : * pDstBits = (BYTE)(255 * whitePixCount / total);

            // go to the next pixel of the destination bitmap
            pDstBits++;
        }
        // omit the bytes used for alignment
        pDstBits += iAlignBytes;
    }

    //d2= GetTickCount();
    //CString s;
    //s.Format ("%d", d2- d1);
    //AfxMessageBox (s);

    // function succeed
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Description	    : Converts file in disk from one format to another
//
// Function name	: GRAPHX::ConvertFile
// Return type		: L_INT
// Argument         : LPCTSTR pszSrcFile
// Argument         : LPCTSTR pszDestFile
// Argument         : L_INT nType			- GRAPHX output file format
// Argument         : L_INT nPage			- page number to proceed. (-1) - all pages
// Argument         : L_INT nQFactor		- quality factor
// Argument         : L_INT nPasses			- quantity of passes (usually in JPEG). Pass 0 to save w/o passes
//
L_INT
GRAPHX::ConvertFile(LPCTSTR pszSrcFile,
                    LPCTSTR pszDestFile,
                    L_INT nType,
                    L_INT nPage,
                    L_INT nBitsPerPixel,
                    L_INT nQFactor /*=2*/,
                    L_INT nPasses /*=0*/)
{
    if (nPage != -1) {
        BITMAPHANDLE hBmp;

        // Load bitmap
        LOADFILEOPTION lfo;
        GetDefaultLoadFileOption(&lfo);
        lfo.PageNumber = nPage;
        LoadBitmap((LPTSTR)pszSrcFile, &hBmp, nBitsPerPixel, ORDER_BGR, &lfo, NULL);

        // Save bitmap
        try {
            SAVEFILEOPTION sfo;
            GetDefaultSaveFileOption(&sfo);
            sfo.Passes = nPasses;
            SaveBitmap((LPTSTR)pszDestFile, &hBmp, nType, 0, nQFactor, &sfo);
            FreeBitmap(&hBmp);
        } catch (CGraphXException*) {
            GraphXnamespace::L_FreeBitmap(&hBmp);
            throw;
        }
    } else {
        HBITMAPLIST hBmpList;

        // Load bitmap
        LoadBitmapList((LPTSTR)pszSrcFile, &hBmpList, nBitsPerPixel, ORDER_BGR, NULL, NULL);

        // Save bitmap
        try {
            SAVEFILEOPTION sfo;
            GetDefaultSaveFileOption(&sfo);
            sfo.Passes = nPasses;
            SaveBitmapList((LPTSTR)pszDestFile, hBmpList, nType, 0, nQFactor, &sfo);
            DestroyBitmapList(hBmpList);
        } catch (CGraphXException*) {
            GraphXnamespace::L_DestroyBitmapList(hBmpList);
            throw;
        }
    }

    return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////
// Description	    : Converts file in memory buffer from one format to another
//
// Function name	: GRAPHX::ConvertFileMemory
// Return type		: L_INT
// Argument         : BYTE* pSrcBuf			- pointer to memory buffer with source file
// Argument         : int nBufSize			- size in bytes of source buffer
// Argument         : LPCTSTR pszDestFile	- path of the destination file
// Argument         : L_INT nType			- GRAPHX output file format
// Argument         : L_INT nPage			- page number to proceed. (-1) - all pages
// Argument         : L_INT nQFactor		- quality factor
// Argument         : L_INT nPasses			- quantity of passes (usually in JPEG). Pass 0 to save w/o passes
//
L_INT
GRAPHX::ConvertFileMemory(BYTE* pSrcBuf,
                          int nBufSize,
                          LPCTSTR pszDestFile,
                          L_INT nType,
                          L_INT nPage,
                          L_INT nBitsPerPixel,
                          L_INT nQFactor,
                          L_INT nPasses)
{
    if (nPage == -1) {
        // get number of pages in file
        FILEINFO fi;
        FileInfoMemory((char*)pSrcBuf, &fi, nBufSize, FILEINFO_TOTALPAGES, NULL);
        int nPages = fi.TotalPages;
        fi.Flags   = FILEINFO_INFOVALID;

        BITMAPHANDLE hBmp;
        LOADFILEOPTION lfo;
        GetDefaultLoadFileOption(&lfo);

        HBITMAPLIST hBmpList;
        CreateBitmapList(&hBmpList);

        try {
            // load all pages and create bitmap list from them
            for (int i = 0; i < nPages; ++i) {
                lfo.PageNumber = i + 1;
                LoadBitmapMemory((char*)pSrcBuf, &hBmp, nBitsPerPixel, ORDER_BGR, nBufSize, &lfo, NULL);
                InsertBitmapListItem(hBmpList, (UINT)-1, &hBmp);
            }

            // save created bitmap list to the destination file
            SAVEFILEOPTION sfo;
            GetDefaultSaveFileOption(&sfo);
            sfo.Passes = nPasses;
            SaveBitmapList((char*)pszDestFile, hBmpList, nType, 0, nQFactor, &sfo);

            DestroyBitmapList(hBmpList);
        } catch (CGraphXException*) {
            GraphXnamespace::L_DestroyBitmapList(hBmpList);
            throw;
        }
    } else {
        // load and save only 1 passed page
        // loading...
        BITMAPHANDLE hBmp;
        LOADFILEOPTION lfo;
        GetDefaultLoadFileOption(&lfo);
        lfo.PageNumber = nPage;
        LoadBitmapMemory((char*)pSrcBuf, &hBmp, nBitsPerPixel, ORDER_BGR, nBufSize, &lfo, NULL);

        // saving...
        try {
            SAVEFILEOPTION sfo;
            GetDefaultSaveFileOption(&sfo);
            sfo.Passes = nPasses;
            SaveBitmap(pszDestFile, &hBmp, nType, 0, nQFactor, &sfo);
            FreeBitmap(&hBmp);
        } catch (CGraphXException*) {
            GraphXnamespace::L_FreeBitmap(&hBmp);
            throw;
        }
    }

    return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
// Description	    : Creates one multipage TIFF file from the passed files
//
// Function name	:
// Return type		: L_INT GRAPHX::CreateMultipageTIFF
// Argument         : LPCTSTR* pszFilesList		- string array with sorce file names
// Argument         : int nSrcFiles				- number of source files
// Argument         : LPCTSTR szDestFile		- name of destination file
//
L_INT
GRAPHX::CreateMultipageTIFF(LPCTSTR* pszFilesList, int nSrcFiles, LPCTSTR szDestFile)
{
    HBITMAPLIST hTmpList = NULL;
    try {
        int i, k, nPages;
        BITMAPHANDLE hBmp;
        SAVEFILEOPTION sfo;
        GetDefaultSaveFileOption(&sfo);
        sfo.PageNumber = 2; // save all pages to the end of file

        // load pages of every file and save them to the destination file
        for (i = 0; i < nSrcFiles; ++i) {
            LoadBitmapList((TCHAR*)pszFilesList[i], &hTmpList, 0, ORDER_BGR, NULL, NULL);
            GetBitmapListCount(hTmpList, (UINT*)&nPages);
            for (k = 0; k < nPages; ++k) {
                GetBitmapListItem(hTmpList, k, &hBmp);
                SaveBitmap(szDestFile, &hBmp, FILE_TIF, 0, 0, &sfo);
            }
            DestroyBitmapList(hTmpList);
        }
    } catch (CGraphXException*) {
        GraphXnamespace::L_DestroyBitmapList(hTmpList);
        throw;
    }

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description	    : Creates one multipage TIFF file from the passed files in memory
//
// Function name	:
// Return type		: L_INT GRAPHX::CreateMultipageTIFFMem
// Argument         : BYTE** pSrcBufs		- array with pointers to memory file
// Argument         : int* pBufSizes		- array with corresponding sizes of memory files
// Argument         : int nSrcFiles			- number of source files
// Argument         : LPCTSTR szDestFile	- name of destination file
//
L_INT
GRAPHX::CreateMultipageTIFFMem(BYTE** pSrcBufs, int* pBufSizes, int nSrcFiles, LPCTSTR szDestFile)
{
    BITMAPHANDLE hBmp;
    ZeroMemory(&hBmp, sizeof(hBmp));

    try {
        int i, k, nPages;
        SAVEFILEOPTION sfo;
        LOADFILEOPTION lfo;
        FILEINFO fi;
        GetDefaultLoadFileOption(&lfo);
        GetDefaultSaveFileOption(&sfo);
        sfo.PageNumber = 2; // save all pages to the end of file

        // load pages of every file and save them to the destination file
        for (i = 0; i < nSrcFiles; ++i) {
            FileInfoMemory((L_CHAR*)pSrcBufs[i], &fi, pBufSizes[i], FILEINFO_TOTALPAGES, NULL);
            nPages   = fi.TotalPages;
            fi.Flags = FILEINFO_INFOVALID;

            for (k = 0; k < nPages; ++k) {
                lfo.PageNumber = k + 1;
                LoadBitmapMemory((L_CHAR*)pSrcBufs[i], &hBmp, 0, ORDER_BGR, pBufSizes[i], &lfo, &fi);
                SaveBitmap(szDestFile, &hBmp, FILE_TIF, 0, 0, &sfo);
                FreeBitmap(&hBmp);
            }
        }
    } catch (CGraphXException*) {
        GraphXnamespace::L_FreeBitmap(&hBmp);
        throw;
    }

    return SUCCESS;
}

L_INT
GRAPHX::CaptureArea(pBITMAPHANDLE pBitmap)
{
    GraphXnamespace::L_StopCapture();

    L_INT res;
    LEADCAPTUREOPTION CaptureOption;
    LEADCAPTUREINFO CaptureInfo;
    LEADCAPTUREAREAOPTION CaptureAreaOption;

    // Get default area options.
    res = GraphXnamespace::L_GetDefaultAreaOption(&CaptureAreaOption);
    if (res != SUCCESS)
        return res;

    //	CaptureAreaOption.nInfoLeft			= CAPTURE_AREA_INFOWND_POS_RIGHT;
    //	CaptureAreaOption.nInfoTop			= CAPTURE_AREA_INFOWND_POS_TOP;
    //	CaptureAreaOption.nInfoWidth		= CAPTURE_AREA_INFOWND_SIZE_SMALL;
    //	CaptureAreaOption.nInfoHeight		= CAPTURE_AREA_INFOWND_SIZE_SMALL;
    CaptureAreaOption.crTextForeColor = GetSysColor(COLOR_INFOTEXT);
    CaptureAreaOption.crTextBackColor = GetSysColor(COLOR_INFOBK);
    CaptureAreaOption.uFlags =
      CAPTURE_AREA_SHOW_DRAWCURSOR | CAPTURE_AREA_ENABLE_KEYBOARD | CAPTURE_AREA_SHOW_CAPTURESIZE | CAPTURE_AREA_SHOW_OPAQUE_TEXT;

    // Get current capture options
    GraphXnamespace::L_GetCaptureOption(&CaptureOption);

    CaptureOption.nHotKey          = 0;
    CaptureOption.uHotKeyModifiers = 0;

    // Set current capture options
    res = GraphXnamespace::L_SetCaptureOption(&CaptureOption);
    if (res != SUCCESS)
        return res;

    return GraphXnamespace::L_CaptureArea(pBitmap, &CaptureAreaOption, &CaptureInfo, NULL, NULL);
}

L_INT
GRAPHX::GetBitmapColorCount(pBITMAPHANDLE pBitmap, L_UINT32& nCount)
{
    L_INT res = GraphXnamespace::L_GetBitmapColorCount(pBitmap, &nCount);
    return CheckSuccess(res, _T("GetBitmapColorCount"));
}

L_INT
GRAPHX::FlipBitmap(pBITMAPHANDLE pBitmap)
{
    L_INT res = GraphXnamespace::L_FlipBitmap(pBitmap);
    return CheckSuccess(res, _T("FlipBitmap"));
}

HBITMAP GRAPHX::ChangeToDDB(HDC hDC, pBITMAPHANDLE pBitmap);

L_INT GRAPHX::CompressData(L_UCHAR* pInBuffer);

L_INT GRAPHX::ConvertColorSpace(L_UCHAR* pBufferSrc, L_UCHAR* ppBufferDst, L_INT nWidth, L_INT nFormatSrc, L_INT nFormatDst);

L_INT GRAPHX::ConvertFromDIB(pBITMAPHANDLE pBitmap, BITMAPINFO* pInfo, L_UCHAR* pBits);

L_UINT* GRAPHX::CreateUserMatchTable(LPRGBQUAD pPalette, L_UINT uColors);

L_INT GRAPHX::DecompressBitmapMemory(L_CHAR* pBuffer, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT32 nBufferSize);

L_INT GRAPHX::DecompressData(L_UCHAR* pOutBuffer);

L_INT GRAPHX::DecompressMemory(L_CHAR* pBuffer,
                               pBITMAPHANDLE pBitmap,
                               FILEREADCALLBACK pfnCallback,
                               L_INT nBitsPerPixel,
                               L_VOID* pUserData,
                               L_INT nPadBytes,
                               L_INT32 nBufferSize);

L_INT GRAPHX::DefaultDithering(L_UINT uMethod);

L_INT GRAPHX::DitherLine(pBITMAPHANDLE pBitmap, L_UCHAR* pBufferSrc, L_UCHAR* pBufferDst);

L_INT GRAPHX::EmbossBitmap(pBITMAPHANDLE pBitmap, L_UINT uDirection, L_UINT uDepth);

L_INT GRAPHX::EndCompressBuffer(L_VOID);

L_VOID GRAPHX::EndCompressData(L_VOID);

L_VOID GRAPHX::EndDecompressData(L_VOID);

L_INT GRAPHX::FreeUserMatchTable(L_UINT* pTable);

L_INT GRAPHX::GammaCorrectBitmap(pBITMAPHANDLE pBitmap, L_UINT uGamma);

L_INT GRAPHX::GetBitmapHistogram(pBITMAPHANDLE pBitmap, L_UINT32* pHistogram, L_UINT uChannel);

L_INT32 GRAPHX::GetBitmapRow(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_UINT32 uBytes);

L_INT GRAPHX::GetComment(L_UINT uType, L_UCHAR* pComment, L_UINT uLength);

L_INT GRAPHX::GetCompressFileStamp(L_CHAR* pszFile, pBITMAPHANDLE pBitmap);

L_UINT32 GRAPHX::GetDisplayMode(L_VOID);

L_INT GRAPHX::GetFixedPalette(LPRGBQUAD pPalette, L_INT nBitsPerPixel);

L_INT GRAPHX::GetPCDResolution(L_CHAR* pszFile, pPCDINFO pPCDInfo);

L_INT GRAPHX::HistoContrastBitmap(pBITMAPHANDLE pBitmap, L_INT nChange);

L_INT GRAPHX::HistoEqualizeBitmap(pBITMAPHANDLE pBitmap);

L_INT GRAPHX::IntensityDetectBitmap(pBITMAPHANDLE pBitmap, L_UINT uLow, L_UINT uHigh);

L_INT GRAPHX::InvertBitmap(pBITMAPHANDLE pBitmap);

L_INT GRAPHX::MedianFilterBitmap(pBITMAPHANDLE pBitmap, L_UINT uDim);

L_INT GRAPHX::MosaicBitmap(pBITMAPHANDLE pBitmap, L_UINT uDim);

L_INT GRAPHX::PaintDCBuffer(HDC hDC,
                            pBITMAPHANDLE pBitmap,
                            LPRECT pSrc,
                            LPRECT pClipSrc,
                            LPRECT pDst,
                            LPRECT pClipDst,
                            L_UINT32 uROP3,
                            L_UCHAR* pBuffer,
                            L_INT nRow,
                            L_INT nCount);

L_INT GRAPHX::PosterizeBitmap(pBITMAPHANDLE pBitmap, L_UINT uLevels);

HDC GRAPHX::PrintBitmap(pBITMAPHANDLE pBitmap, L_INT nX, L_INT nY, L_INT nWidth, L_INT nHeight, L_BOOL fEndDoc);

HDC GRAPHX::PrintBitmapFast(HDC hDC, pBITMAPHANDLE pBitmap, L_INT nXPos, L_INT nYPos, L_INT nWidth, L_INT nHeight, L_BOOL fEndDoc);

L_INT GRAPHX::ReadFileComment(L_CHAR* pszFile, L_UINT uType, L_UCHAR* pComment, L_UINT uLength);

L_INT GRAPHX::RemapBitmapIntensity(pBITMAPHANDLE pBitmap, L_UCHAR* pLUT, L_UINT uChannel);

L_INT GRAPHX::ResampleBitmap(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight);

L_INT GRAPHX::Resize(L_UCHAR* pBuffer, L_INT nRow, L_INT nBitsPerPixel, L_INT* pXSize, L_INT* pYSize, L_VOID* pResizeData);

L_INT GRAPHX::ReverseBitmap(pBITMAPHANDLE pBitmap);

L_INT GRAPHX::ScreenCaptureBitmap(HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pRect);

L_INT GRAPHX::SetComment(L_UINT uType, L_UCHAR* pComment, L_UINT uLength);

LOADINFOCALLBACK GRAPHX::SetLoadInfoCallback(LOADINFOCALLBACK pfnCallback);

L_INT GRAPHX::SetPCDResolution(L_INT nResolution);

L_UINT* GRAPHX::SetUserMatchTable(L_UINT* pTable);

L_INT GRAPHX::SharpenBitmap(pBITMAPHANDLE pBitmap, L_INT nSharpness);

L_INT GRAPHX::ShearBitmap(pBITMAPHANDLE pBitmapSrc, L_INT nAngle, L_BOOL fHorizontal, COLORREF crFill);

L_INT GRAPHX::SpatialFilterBitmap(pBITMAPHANDLE pBitmap, LPSPATIALFLT pFilter);

L_INT GRAPHX::StartCompressBuffer(pBITMAPHANDLE pBitmap,
                                  FILEREADCALLBACK pfnCallback,
                                  L_UINT32 uInputBytes,
                                  L_UINT uOutputBytes,
                                  L_CHAR* pOutputBuffer,
                                  L_INT nOutputType,
                                  L_INT nQFactor,
                                  L_VOID* pUserData);

L_INT GRAPHX::StartCompressData(pBITMAPHANDLE pBitmap,
                                COMPBUFFCALLBACK pfnCallback,
                                L_UINT uInBufferSize,
                                L_UINT uOutBufferSize,
                                L_UCHAR* pOutBuffer,
                                L_UINT uHeaderSize,
                                L_UCHAR* pHeaderBuffer,
                                L_VOID* pUserData);

L_INT GRAPHX::StartDecompressData(pBITMAPHANDLE pBitmap,
                                  COMPBUFFCALLBACK pfnCallback,
                                  L_UINT uInBufferSize,
                                  L_UCHAR* pInBuffer,
                                  L_UINT uOutBufferSize,
                                  L_UINT* pHeaderSize,
                                  L_UCHAR* pHeaderBuffer,
                                  L_VOID* pUserData);

L_INT GRAPHX::StartDithering(pBITMAPHANDLE pBitmap, LPRGBQUAD pPalette, L_UINT uColors);

L_INT GRAPHX::StartResize(L_INT nOldWidth, L_INT nOldHeight, L_INT nNewWidth, L_INT nNewHeight, L_VOID** ppResizeData);

L_INT GRAPHX::StopDithering(pBITMAPHANDLE pBitmap);

L_INT GRAPHX::StopResize(L_VOID* pResizeData);

L_INT GRAPHX::StretchBitmapIntensity(pBITMAPHANDLE pBitmap);

L_INT GRAPHX::UnderlayBitmap(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pUnderlay, L_UINT uFlags);

L_VOID GRAPHX::UnlockLZWSupport(L_CHAR* pKey);

L_INT GRAPHX::VersionInfo(pVERSIONINFO pVersionInfo);
