#ifndef _GRAPHX_INCLUDED
#define _GRAPHX_INCLUDED

namespace GraphXnamespace {
#include "lt12\l_bitmap.h"
#include "lt12\l_error.h"
#include "lt12\ltlst.h"
#include "lt12\lttwn.h"
#include "lt12\ltscr.h"

/*	
//	#include "lt12\l_fox.h"
//	#include "lt12\l_ocrocx.h"
//	#include "lt12\l_ocx40.h"
//	#include "lt12\l_ocxerr.h"
#include "lt12\ldkrn.h"
#include "lt12\ltann.h"
#include "lt12\ltcap.h"
//	#include "lt12\ltcapocx.h"
#include "lt12\ltdic.h"
#include "lt12\ltdis.h"
#include "lt12\ltdlg.h"
//	#include "lt12\ltdlgocx.h"
#include "lt12\ltefx.h"
#include "lt12\lterr.h"
#include "lt12\ltfil.h"
#include "lt12\ltfpx.h"
#include "lt12\ltimg.h"
#include "lt12\ltisi.h"
//	#include "lt12\ltisiocx.h"
#include "lt12\ltkey.h"
#include "lt12\ltkrn.h"
#include "lt12\ltlck.h"
#include "lt12\ltlck_.h"
#include "lt12\ltlst.h"
#include "lt12\ltnet.h"
//	#include "lt12\ltnetocx.h"
#include "lt12\ltpck.h"
#include "lt12\ltscr.h"
//	#include "lt12\ltscpocx.h"
#include "lt12\ltsnd.h"
#include "lt12\ltsys.h"
#include "lt12\lttwn.h"
#include "lt12\lttyp.h"
#include "lt12\ltvid.h"
//	#include "lt12\ltvidocx.h"
#include "lt12\ltwnd.h"
#include "lt12\lvdlg.h"
#include "lt12\lvkrn.h"
	*/
}

// type definitions
#define T(x) typedef GraphXnamespace::x x;
T(L_INT)
T(L_UINT)
T(L_VOID)
T(pBITMAPHANDLE)
T(LPBINARYFLT)
T(L_BOOL)
T(L_UINT32)
T(FILEREADCALLBACK)
T(L_CHAR)
T(L_UCHAR)
T(L_INT32)
T(pFILEINFO)
T(pPCDINFO)
T(REDIRECTOPEN)
T(REDIRECTREAD)
T(REDIRECTWRITE)
T(REDIRECTSEEK)
T(REDIRECTCLOSE)
T(FILESAVECALLBACK)
T(LOADINFOCALLBACK)
T(STATUSCALLBACK)
T(LOADFILEOPTION)
T(pLOADFILEOPTION)
T(SAVEFILEOPTION)
T(pSAVEFILEOPTION)
T(COMPBUFFCALLBACK)
T(TWAINCALLBACK)
T(pTWAINPROPERTIES)
T(pVERSIONINFO)
T(LPSPATIALFLT)
T(ENUMSOURCECALLBACK)
T(TWAINPROPERTIES)
T(BITMAPHANDLE)
T(FILEINFO)
T(LILSELOPTION)
T(LILITEM)
T(LILITEMSEL)
T(HBITMAPLIST)
T(RGNXFORM)
T(pRGNXFORM)
T(FILEOPENPARM)
T(FILESAVEPARM)
T(LTCOMMDLGHELPCB)
T(FILESAVEFORMAT)
T(LPFILESAVEFORMAT)
T(COLORRESCALLBACK)
T(LILITEMOPTION)
T(LEADCAPTUREAREAOPTION)
T(LEADCAPTUREINFO)
T(AREATYPEENUM)
T(LEADCAPTUREOPTION)
T(pLEADCAPTUREINFO)
#undef T

enum
{
    TWAIN_ROT0,
    TWAIN_ROT90,
    TWAIN_ROT180,
    TWAIN_ROT270,
    TWAIN_PORTRAIT  = TWAIN_ROT0,
    TWAIN_LANDSCAPE = TWAIN_ROT270
};

#ifdef GRAPH_EXPORTS
#define GRAPH_API __declspec(dllexport)
#else
#define GRAPH_API __declspec(dllimport)
#endif

class GRAPH_API GRAPHX
{

  public:
    // Group for working with Image List Control
    static L_VOID UseImageListControl(L_VOID);
    static HWND
    CreateImageListControl(L_UINT32 dwStyle, L_INT x, L_INT y, L_INT nWidth, L_INT nHeight, HWND hWndParent, L_INT nID, COLORREF crBack);
    static L_INT ImgListSetSelOptions(HWND hWnd, LILSELOPTION* Opt);
    static L_INT ImgListGetSelOptions(HWND hWnd, LILSELOPTION* Opt);
    static L_INT ImgListClear(HWND hWnd);
    static L_INT ImgListGetItemCount(HWND hWnd);
    static L_INT ImgListInsert(HWND hWnd, LILITEM* pItem);
    static L_INT ImgListInsertAt(HWND hWnd, int nIndex, LILITEM* pItem);
    static L_INT ImgListGetItem(HWND hWnd, L_UINT nIndex, LILITEM* pItem);
    static L_INT ImgListSetItem(HWND hWnd, L_UINT nIndex, LILITEM* pItem);
    static L_INT ImgListGetItemOptions(HWND hWnd, LILITEMOPTION* pOpt);
    static L_INT ImgListSetItemOptions(HWND hWnd, LILITEMOPTION* pOpt);
    static L_INT ImgListRemove(HWND hWnd, int nItem);
    static L_INT ImgListGetSelItems(HWND hWnd, LILITEM* pItems);
    static L_INT ImgListGetSelCount(HWND hWnd);
    static L_INT ImgListEnsureVisible(HWND hWnd, int nItem);
    static L_INT ImgListGetPage(HWND hWnd);
    static L_INT ImgListGetPagesCount(HWND hWnd);
    static L_INT ImgListHitTest(HWND hWnd, POINT* pt);

    // Group for working with BitmapList
    static L_INT CreateBitmapList(HBITMAPLIST* phList);
    static L_INT DestroyBitmapList(HBITMAPLIST hList);
    static L_INT LoadBitmapList(LPSTR lpszFile,
                                HBITMAPLIST* phList,
                                L_INT nBitsTo,
                                L_INT nColorOrder,
                                pLOADFILEOPTION pLoadOptions,
                                pFILEINFO pFileInfo);
    static L_INT SaveBitmapList(LPSTR lpszFile,
                                HBITMAPLIST hList,
                                L_INT nFormat,
                                L_INT nBits                  = 0,
                                L_INT nQFactor               = 2,
                                pSAVEFILEOPTION pSaveOptions = NULL);
    static L_INT GetBitmapListCount(HBITMAPLIST hList, L_UINT L_FAR* puCount);
    static L_INT GetBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap);
    static L_INT SetBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap);
    static L_INT InsertBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap);
    static L_INT RemoveBitmapListItem(HBITMAPLIST hList, L_UINT uIndex, pBITMAPHANDLE pBitmap);
    static L_INT DeleteBitmapListItems(HBITMAPLIST hList, L_UINT uIndex, L_UINT uCount);

    // static L_INT GetExtFileOption(EXTFILEOPTION *pOption);
    static L_INT GetDefaultLoadFileOption(pLOADFILEOPTION pLoadOptions);
    static L_INT GetDefaultSaveFileOption(pSAVEFILEOPTION pSaveOptions);

    // on error throw a CGraphXException or returns the error code?
    static void SetOnErrorThrow(bool onErrorThrow);
    static bool GetOnErrorThrow();
    static L_INT CheckSuccess(L_INT result, const char* function);
    static L_VOID AccessBitmap(pBITMAPHANDLE pBitmap);
    static L_INT AddBitmapNoise(pBITMAPHANDLE pBitmap, L_UINT uRange, L_UINT uChannel);
    static L_INT AllocateBitmap(pBITMAPHANDLE pBitmap, L_INT nMemoryType);
    static L_INT AverageFilterBitmap(pBITMAPHANDLE pBitmap, L_UINT uDim);
    static L_INT BinaryFilterBitmap(pBITMAPHANDLE pBitmap, LPBINARYFLT pFilter);
    static L_INT ChangeBitmapContrast(pBITMAPHANDLE pBitmap, L_INT nChange);
    static L_INT ChangeBitmapHeight(pBITMAPHANDLE pBitmap, L_INT nHeight);
    static L_INT ChangeBitmapHue(pBITMAPHANDLE pBitmap, L_INT nAngle);
    static L_INT ChangeBitmapIntensity(pBITMAPHANDLE pBitmap, L_INT nChange);
    static L_INT ChangeBitmapSaturation(pBITMAPHANDLE pBitmap, L_INT nChange);
    static L_INT ChangeFromDDB(HDC hDC, pBITMAPHANDLE pBitmap, HBITMAP hBitmap, HPALETTE hPalette);

    static L_INT ChangeFromDIB(pBITMAPHANDLE pBitmap, HANDLE hDIB);
    static HBITMAP ChangeToDDB(HDC hDC, pBITMAPHANDLE pBitmap);
    static HANDLE ChangeToDIB(pBITMAPHANDLE pBitmap);
    static L_INT ClearBitmap(pBITMAPHANDLE pBitmap);
    static L_BOOL ClipboardReady(L_VOID);
    static L_INT ColorMergeBitmap(pBITMAPHANDLE pBitmap, pBITMAPHANDLE* ppBitmap, L_UINT32 uFlags);

    static L_INT ColorResBitmap(pBITMAPHANDLE pBitmapSrc,
                                pBITMAPHANDLE pBitmapDst,
                                L_INT nBitsPerPixel,
                                L_UINT32 uFlags,
                                LPRGBQUAD pPalette,
                                HPALETTE hPalette,
                                L_UINT uColors,
                                COLORRESCALLBACK pfnCallback,
                                L_VOID* pUserData);

    static L_INT ColorSeparateBitmap(pBITMAPHANDLE pBitmap, pBITMAPHANDLE* ppBitmap, L_UINT32 uFlags);

    static L_INT CombineBitmap(pBITMAPHANDLE pBitmapDst,
                               L_INT nXDst,
                               L_INT nYDst,
                               L_INT nWidth,
                               L_INT nHeight,
                               pBITMAPHANDLE pBitmapSrc,
                               L_INT nXSrc,
                               L_INT nYSrc,
                               L_UINT32 uFlags);

    static L_INT CompressBitmapWithStamp(pBITMAPHANDLE pBitmap,
                                         L_CHAR* pszFile,
                                         L_INT nFormat,
                                         L_INT nQFactor,
                                         L_INT nStampWidth,
                                         L_INT nStampHeight,
                                         L_INT nStampBits,
                                         L_INT nDitherFlag);

    static L_INT CompressBuffer(L_UCHAR* pBuffer);

    static L_INT CompressData(L_UCHAR* pInBuffer);

    static L_INT ConvertBuffer(L_UCHAR* pBuffer,
                               L_INT nWidth,
                               L_INT nBitsPerPixelSrc,
                               L_INT nBitsPerPixelDst,
                               L_INT nOrderSrc,
                               L_INT nOrderDst,
                               LPRGBQUAD pPaletteSrc,
                               LPRGBQUAD pPaletteDst);

    static L_INT ConvertColorSpace(L_UCHAR* pBufferSrc, L_UCHAR* pBufferDst, L_INT nWidth, L_INT nFormatSrc, L_INT nFormatDst);

    static L_INT ConvertFromDDB(HDC hDC, pBITMAPHANDLE pBitmap, HBITMAP hBitmap, HPALETTE hPalette);

    static L_INT ConvertFromDIB(pBITMAPHANDLE pBitmap, BITMAPINFO* pInfo, L_UCHAR* pBits);

    static HBITMAP ConvertToDDB(HDC hDC, pBITMAPHANDLE pBitmap);

    static HANDLE ConvertToDIB(pBITMAPHANDLE pBitmap);

    static L_INT CopyBitmap(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pBitmapSrc);

    static L_INT CopyBitmapData(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pBitmapSrc);

    static L_VOID CopyBitmapHandle(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pBitmapSrc);

    static L_INT CopyBitmapRect(pBITMAPHANDLE pBitmapDst,
                                pBITMAPHANDLE pBitmapSrc,
                                L_UINT uCol,
                                L_UINT uRow,
                                L_UINT uWidth,
                                L_UINT uHeight);

    static L_BOOL CopyFromClipboard(HWND hWnd, pBITMAPHANDLE pBitmap);

    static L_BOOL CopyToClipboard(HWND hWnd, pBITMAPHANDLE pBitmap);

    static L_INT CopyToClipboardExt(HWND hWnd, pBITMAPHANDLE pBitmap, L_UINT uFlags);

    static L_INT CreateBitmap(pBITMAPHANDLE pBitmap,
                              L_UINT uMemory,
                              L_UINT uWidth,
                              L_UINT uHeight,
                              L_UINT uBitsPerPixel,
                              L_UINT uOrder,
                              LPRGBQUAD pPalette,
                              L_UINT uViewPerspective,
                              L_UCHAR L_FAR* pData,
                              L_UINT32 dwSize);

    static HDC CreateGraphXDC(pBITMAPHANDLE pBitmap);

    static L_UINT* CreateUserMatchTable(LPRGBQUAD pPalette, L_UINT uColors);

    static HPALETTE CreatePaintPalette(HDC hDC, pBITMAPHANDLE pBitmap);

    static L_INT DecompressBitmapMemory(L_CHAR* pBuffer, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT32 nBufferSize);

    static L_INT DecompressData(L_UCHAR* pOutBuffer);

    static L_INT DecompressMemory(L_CHAR* pBuffer,
                                  pBITMAPHANDLE pBitmap,
                                  FILEREADCALLBACK pfnCallback,
                                  L_INT nBitsPerPixel,
                                  L_VOID* pUserData,
                                  L_INT nPadBytes,
                                  L_INT32 nBufferSize);

    static L_INT DefaultDithering(L_UINT uMethod);
    static L_BOOL DeleteGraphXDC(HDC hDC);
    static L_INT DeskewBitmap(pBITMAPHANDLE pBitmap, L_INT32 L_FAR* pnAngle);
    static L_INT DespeckleBitmap(pBITMAPHANDLE pBitmap);

    static L_INT DitherLine(pBITMAPHANDLE pBitmap, L_UCHAR* pBufferSrc, L_UCHAR* pBufferDst);

    static HPALETTE DupPalette(HPALETTE hPalette);
    static L_INT EmbossBitmap(pBITMAPHANDLE pBitmap, L_UINT uDirection, L_UINT uDepth);
    static L_INT EndCompressBuffer(L_VOID);
    static L_VOID EndCompressData(L_VOID);
    static L_VOID EndDecompressData(L_VOID);

    static L_INT FileConvert(const L_CHAR* pszFileSrc,
                             const L_CHAR* pszFileDst,
                             L_INT nType,
                             L_INT nWidth,
                             L_INT nHeight,
                             L_INT nBitsPerPixel,
                             L_INT nQFactor,
                             pLOADFILEOPTION pLoadOptions,
                             pSAVEFILEOPTION pSaveOptions,
                             pFILEINFO pFileInfo);

    static L_INT FileInfo(const L_CHAR* pszFile, pFILEINFO pFileInfo, L_UINT flags, pLOADFILEOPTION pLoadOptions);

    static L_INT FileInfo(int hFile, long nStart, long nLength, pFILEINFO pFileInfo, L_UINT flags);

    static L_INT FileInfoMemory(L_CHAR* pBuffer, pFILEINFO pFileInfo, L_INT32 nBufferSize, L_UINT flags, pLOADFILEOPTION pLoadOptions);

    static L_INT FillBitmap(pBITMAPHANDLE pBitmap, COLORREF crFill);

    static L_INT FlipBitmap(pBITMAPHANDLE pBitmap);
    static L_VOID FreeBitmap(pBITMAPHANDLE pBitmap);
    static L_INT FreeUserMatchTable(L_UINT* pTable);

    static L_INT GammaCorrectBitmap(pBITMAPHANDLE pBitmap, L_UINT uGamma);

    static L_INT GetBitmapColors(pBITMAPHANDLE pBitmap, L_INT nIndex, L_INT nCount, LPRGBQUAD pPalette);

    static L_INT GetBitmapHistogram(pBITMAPHANDLE pBitmap, L_UINT32* pHistogram, L_UINT uChannel);

    static L_INT32 GetBitmapRow(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_UINT32 uBytes);

    static L_INT32 GetBitmapRowCol(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_INT nCol, L_UINT32 uBytes);

    static L_INT GetComment(L_UINT uType, L_UCHAR* pComment, L_UINT uLength);

    static L_INT GetCompressFileStamp(L_CHAR* pszFile, pBITMAPHANDLE pBitmap);

    static L_UINT32 GetDisplayMode(L_VOID);

    static L_INT GetFixedPalette(LPRGBQUAD pPalette, L_INT nBitsPerPixel);

    static L_INT GetPCDResolution(L_CHAR* pszFile, pPCDINFO pPCDInfo);

    static COLORREF GetPixelColor(pBITMAPHANDLE pBitmap, L_INT nRow, L_INT nCol);

    static L_INT GrayScaleBitmap(pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel);
    static L_INT IsGrayScaleBitmap(pBITMAPHANDLE pBitmap);

    static L_INT HalfToneBitmap(pBITMAPHANDLE pBitmap, L_UINT uType);

    static L_INT HalfToneBitmapExt(pBITMAPHANDLE pBitmap, L_UINT uType, L_INT32 nAngle);

    static L_INT HistoContrastBitmap(pBITMAPHANDLE pBitmap, L_INT nChange);

    static L_INT HistoEqualizeBitmap(pBITMAPHANDLE pBitmap);

    static L_INT InitBitmap(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight, L_INT nBitsPerPixel);

    static L_INT IntensityDetectBitmap(pBITMAPHANDLE pBitmap, L_UINT uLow, L_UINT uHigh);

    static L_INT InvertBitmap(pBITMAPHANDLE pBitmap);

    static L_INT LoadBitmap(const L_CHAR* pszFile,
                            pBITMAPHANDLE pBitmap,
                            L_INT nBitsPerPixel,
                            L_INT nOrder,
                            pLOADFILEOPTION pLoadOptions,
                            pFILEINFO pFileInfo);

    static L_INT LoadBitmapMemory(L_CHAR* pBuffer,
                                  pBITMAPHANDLE pBitmap,
                                  L_INT nBitsPerPixel,
                                  L_INT nOrder,
                                  L_INT32 nBufferSize,
                                  pLOADFILEOPTION pLoadOptions,
                                  pFILEINFO pFileInfo);

    static L_INT LoadFile(L_CHAR* pszFile,
                          pBITMAPHANDLE pBitmap,
                          L_INT nBitsPerPixel,
                          L_INT nOrder,
                          L_UINT uFlags,
                          FILEREADCALLBACK pfnCallback,
                          L_VOID* pUserData,
                          pLOADFILEOPTION pLoadOptions,
                          pFILEINFO pFileInfo);

    static L_INT LoadFileOffset(L_INT fd,
                                L_INT32 nOffsetBegin,
                                L_INT32 nBytesToLoad,
                                pBITMAPHANDLE pBitmap,
                                L_INT nBitsPerPixel,
                                L_INT nOrder,
                                L_UINT uFlags,
                                FILEREADCALLBACK pfnCallback,
                                L_VOID* pUserData,
                                pLOADFILEOPTION pLoadOptions,
                                pFILEINFO pFileInfo);

    static L_INT LoadMemory(L_CHAR* pBuffer,
                            pBITMAPHANDLE pBitmap,
                            L_INT nBitsPerPixel,
                            L_INT nOrder,
                            L_UINT uFlags,
                            FILEREADCALLBACK pfnCallback,
                            L_VOID* pUserData,
                            L_INT32 nBufferSize,
                            pLOADFILEOPTION pLoadOptions,
                            pFILEINFO pFileInfo);

    static L_INT MedianFilterBitmap(pBITMAPHANDLE pBitmap, L_UINT uDim);

    static L_INT MosaicBitmap(pBITMAPHANDLE pBitmap, L_UINT uDim);

    static L_INT PaintDC(HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pSrc, LPRECT pClipSrc, LPRECT pDst, LPRECT pClipDst, L_UINT32 uROP3);

    static L_INT PaintDCBuffer(HDC hDC,
                               pBITMAPHANDLE pBitmap,
                               LPRECT pSrc,
                               LPRECT pClipSrc,
                               LPRECT pDst,
                               LPRECT pClipDst,
                               L_UINT32 uROP3,
                               L_UCHAR* pBuffer,
                               L_INT nRow,
                               L_INT nCount);

    static L_INT PaintDCEffect(HDC hDC,
                               pBITMAPHANDLE pBitmap,
                               LPRECT pSrc,
                               LPRECT pClipSrc,
                               LPRECT pDst,
                               LPRECT pClipDst,
                               L_UINT32 uROP3,
                               L_UINT uEffect);

    static L_INT PaintRgnDC(HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pSrc, LPRECT pClipSrc, LPRECT pDst, LPRECT pClipDst, L_UINT32 uROP3);

    L_UINT32 uROP3; // windows ROP code for display
    static L_INT PosterizeBitmap(pBITMAPHANDLE pBitmap, L_UINT uLevels);

    static HDC PrintBitmap(pBITMAPHANDLE pBitmap, L_INT nX, L_INT nY, L_INT nWidth, L_INT nHeight, L_BOOL fEndDoc);

    static HDC PrintBitmapExt(HDC hDC, pBITMAPHANDLE pBitmap, L_INT nX, L_INT nY, L_INT nWidth, L_INT nHeight, L_BOOL fEndDoc);

    static HDC PrintBitmapFast(HDC hDC, pBITMAPHANDLE pBitmap, L_INT nXPos, L_INT nYPos, L_INT nWidth, L_INT nHeight, L_BOOL fEndDoc);

    static L_INT PutBitmapColors(pBITMAPHANDLE pBitmap, L_INT nIndex, L_INT nCount, LPRGBQUAD pPalette);

    static L_INT32 PutBitmapRow(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_UINT32 uBytes);

    static L_INT32 PutBitmapRowCol(pBITMAPHANDLE pBitmap, L_UCHAR* pBuffer, L_INT nRow, L_INT nCol, L_UINT32 uBytes);

    static L_INT PutPixelColor(pBITMAPHANDLE pBitmap, L_INT nRow, L_INT nCol, COLORREF crColor);

    static L_INT ReadFileComment(L_CHAR* pszFile, L_UINT uType, L_UCHAR* pComment, L_UINT uLength);

    static L_VOID RedirectIO(REDIRECTOPEN pfnOpen,
                             REDIRECTREAD pfnRead,
                             REDIRECTWRITE pfnWrite,
                             REDIRECTSEEK pfnSeek,
                             REDIRECTCLOSE pfnClose,
                             L_VOID L_FAR* pUserData);

    static L_VOID ReleaseBitmap(pBITMAPHANDLE pBitmap);

    static L_INT RemapBitmapIntensity(pBITMAPHANDLE pBitmap, L_UCHAR* pLUT, L_UINT uChannel);

    static L_INT ResampleBitmap(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight);

    static L_INT Resize(L_UCHAR* pBuffer, L_INT nRow, L_INT nBitsPerPixel, L_INT* pXSize, L_INT* pYSize, L_VOID* pResizeData);

    static L_INT ResizeBitmap(pBITMAPHANDLE pBitmapSrc, pBITMAPHANDLE pBitmapDst);

    static L_INT ReverseBitmap(pBITMAPHANDLE pBitmap);

    static L_INT RotateBitmap(pBITMAPHANDLE pBitmap, L_INT nAngle, L_UINT uFlags, COLORREF crFill);

    //--static L_INT RotateBitmapFine(pBITMAPHANDLE pBitmap,
    /*                                        L_INT32 nAngle,
	L_BOOL fResize,
	COLORREF crFill);*/

    static L_INT SaveBitmap(const L_CHAR* pszFile,
                            pBITMAPHANDLE pBitmap,
                            L_INT nFormat,
                            L_INT nBitsPerPixel          = 0,
                            L_INT nQFactor               = 2,
                            pSAVEFILEOPTION pSaveOptions = NULL);

    static L_INT SaveBitmapMemory(HANDLE* hHandle,
                                  pBITMAPHANDLE pBitmap,
                                  L_INT nFormat,
                                  L_UINT32* uSize,
                                  L_INT nBitsPerPixel          = 0,
                                  L_INT nQFactor               = 2,
                                  pSAVEFILEOPTION pSaveOptions = NULL);

    static L_INT SaveFile(L_CHAR* pszFile,
                          pBITMAPHANDLE pBitmap,
                          L_INT nFormat,
                          L_INT nBitsPerPixel          = 0,
                          L_INT nQFactor               = 2,
                          L_UINT uFlags                = SAVEFILE_FIXEDPALETTE,
                          FILESAVECALLBACK pfnCallback = NULL,
                          L_VOID* pUserData            = NULL,
                          pSAVEFILEOPTION pSaveOptions = NULL);

    static L_INT SaveFileOffset(L_INT fd,
                                L_INT32 nOffsetBegin,
                                L_INT32* nSizeWritten,
                                pBITMAPHANDLE pBitmap,
                                L_INT nFormat,
                                L_INT nBitsPerPixel          = 0,
                                L_INT nQFactor               = 2,
                                L_UINT uFlags                = SAVEFILE_FIXEDPALETTE,
                                FILESAVECALLBACK pfnCallback = NULL,
                                L_VOID* pUserData            = NULL,
                                pSAVEFILEOPTION pSaveOptions = NULL);

    static L_INT ScreenCaptureBitmap(HDC hDC, pBITMAPHANDLE pBitmap, LPRECT pRect);

    static L_INT SetComment(L_UINT uType, L_UCHAR* pComment, L_UINT uLength);

    static L_UINT32 SetDisplayMode(L_UINT32 uFlagPos, L_UINT32 uFlagSet);

    static LOADINFOCALLBACK SetLoadInfoCallback(LOADINFOCALLBACK pfnCallback);
    static L_INT SetPCDResolution(L_INT nResolution);
    static STATUSCALLBACK SetStatusCallBack(STATUSCALLBACK pfnCallback, L_VOID L_FAR* pUserData);

    //--static L_INT SetExtFileOption(EXTFILEOPTION* pOption);

    static L_UINT* SetUserMatchTable(L_UINT* pTable);

    static L_INT SharpenBitmap(pBITMAPHANDLE pBitmap, L_INT nSharpness);

    static L_INT ShearBitmap(pBITMAPHANDLE pBitmapSrc, L_INT nAngle, L_BOOL fHorizontal, COLORREF crFill);

    static L_INT SizeBitmap(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight);

    static L_INT SizeBitmapExt(pBITMAPHANDLE pBitmap, L_INT nWidth, L_INT nHeight, L_UINT32 uFlags);

    static L_INT SpatialFilterBitmap(pBITMAPHANDLE pBitmap, LPSPATIALFLT pFilter);

    static L_INT StartCompressBuffer(pBITMAPHANDLE pBitmap,
                                     FILEREADCALLBACK pfnCallback,
                                     L_UINT32 uInputBytes,
                                     L_UINT uOutputBytes,
                                     L_CHAR* pOutputBuffer,
                                     L_INT nOutputType,
                                     L_INT nQFactor,
                                     L_VOID* pUserData);

    static L_INT StartCompressData(pBITMAPHANDLE pBitmap,
                                   COMPBUFFCALLBACK pfnCallback,
                                   L_UINT uInBufferSize,
                                   L_UINT uOutBufferSize,
                                   L_UCHAR* pOutBuffer,
                                   L_UINT uHeaderSize,
                                   L_UCHAR* pHeaderBuffer,
                                   L_VOID* pUserData);

    static L_INT StartDecompressData(pBITMAPHANDLE pBitmap,
                                     COMPBUFFCALLBACK pfnCallback,
                                     L_UINT uInBufferSize,
                                     L_UCHAR* pInBuffer,
                                     L_UINT uOutBufferSize,
                                     L_UINT* pHeaderSize,
                                     L_UCHAR* pHeaderBuffer,
                                     L_VOID* pUserData);

    static L_INT StartDithering(pBITMAPHANDLE pBitmap, LPRGBQUAD pPalette, L_UINT uColors);

    static L_INT StartResize(L_INT nOldWidth, L_INT nOldHeight, L_INT nNewWidth, L_INT nNewHeight, L_VOID** ppResizeData);

    static L_INT StopDithering(pBITMAPHANDLE pBitmap);
    static L_INT StopResize(L_VOID* pResizeData);
    static L_INT StretchBitmapIntensity(pBITMAPHANDLE pBitmap);
    static L_BOOL IsTwainAvailable(HWND hWnd);

    static L_INT TwainAcquire(HWND hWnd, pBITMAPHANDLE pBitmap);

    static L_INT TwainAcquireExt(HWND hWnd, pBITMAPHANDLE pBitmap, L_UINT uFlags, TWAINCALLBACK pfnCallback, L_VOID* pUserData);

    static L_INT TwainEnumSources(HWND hWnd, ENUMSOURCECALLBACK pFunction, L_VOID* pUserData);

    static L_INT TwainSelect(HWND hWnd);
    static L_INT TwainSetProps(HWND hWnd, pTWAINPROPERTIES pTwain, L_UINT32 uFlags);
    static L_INT TwainGetProps(pTWAINPROPERTIES pTwain);

    static L_INT UnderlayBitmap(pBITMAPHANDLE pBitmapDst, pBITMAPHANDLE pUnderlay, L_UINT uFlags);

    static L_VOID UnlockLZWSupport(L_CHAR* pKey);
    static L_INT VersionInfo(pVERSIONINFO pVersionInfo);

    static L_INT SetBitmapRgnHandle(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, HRGN hRgn, L_UINT uCombineMode);
    static L_INT SetBitmapRgnColor(pBITMAPHANDLE pBitmap, COLORREF crColor, L_UINT uCombineMode);
    static L_INT SetBitmapRgnEllipse(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, RECT L_FAR* pRect, L_UINT uCombineMode);
    static L_INT SetBitmapRgnPolygon(pBITMAPHANDLE pBitmap,
                                     pRGNXFORM pXForm,
                                     POINT L_FAR* pPoints,
                                     L_UINT uPoints,
                                     L_UINT uFillMode,
                                     L_UINT uCombineMode);
    static L_INT SetBitmapRgnRect(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, RECT L_FAR* pRect, L_UINT uCombineMode);
    static L_INT SetBitmapRgnRoundRect(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       RECT L_FAR* pRect,
                                       L_INT nWidthEllipse,
                                       L_INT nHeightEllipse,
                                       L_UINT uCombineMode);
    static L_INT SetBitmapRgnFromMask(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, pBITMAPHANDLE pMask, L_UINT uCombineMode);

    static L_BOOL BitmapHasRgn(pBITMAPHANDLE pBitmap);
    static L_INT GetBitmapRgnHandle(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, HRGN L_FAR* phRgn);
    static L_INT FreeBitmapRgn(pBITMAPHANDLE pBitmap);
    static L_INT OffsetBitmapRgn(pBITMAPHANDLE pBitmap, L_INT nRowOffset, L_INT nColOffset);
    static L_INT FrameBitmapRgn(HDC hDC, pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, L_UINT uType);
    static L_INT GetBitmapRgnArea(pBITMAPHANDLE pBitmap, L_UINT32 L_FAR* puArea);
    static L_INT GetBitmapRgnBounds(pBITMAPHANDLE pBitmap, pRGNXFORM pXForm, RECT L_FAR* pRect);
    static L_BOOL IsPtInBitmapRgn(pBITMAPHANDLE pBitmap, L_INT nRow, L_INT nCol);
    static L_INT CreateMaskFromBitmapRgn(pBITMAPHANDLE pBitmap, pBITMAPHANDLE pMask);

    static L_INT DlgFileOpen(HWND hWnd,
                             OPENFILENAME* pOpenFileName,
                             FILEOPENPARM L_FAR* pFileOpenParm,
                             L_UINT32 uFlags,
                             LTCOMMDLGHELPCB pfnCallback,
                             L_VOID L_FAR* pUserData);
    static L_INT DlgFileSave(HWND hWnd,
                             OPENFILENAME* pOpenFileName,
                             FILESAVEPARM L_FAR* pFileSaveParm,
                             L_UINT32 uFlags,
                             LPFILESAVEFORMAT pSaveFormats,
                             L_INT nSaveFormats,
                             LTCOMMDLGHELPCB pfnCallback,
                             L_VOID L_FAR* pUserData);

    static L_INT SetPaintIntensity(pBITMAPHANDLE pBitmap, L_INT nValue);
    static L_INT SetPaintContrast(pBITMAPHANDLE pBitmap, L_INT nValue);
    static L_INT GetPaintIntensity(pBITMAPHANDLE pBitmap);
    static L_INT GetPaintContrast(pBITMAPHANDLE pBitmap);

    static L_INT TrimBitmap(pBITMAPHANDLE pBitmap, L_INT nCol, L_INT nRow, L_UINT uWidth, L_UINT uHeight);
    static L_INT ChangeBitmapViewPerspective(pBITMAPHANDLE pDstBitmap, pBITMAPHANDLE pSrcBitmap, L_INT ViewPerspective);
    static BOOL SmartResize(BITMAPHANDLE* pBmpSrc, BITMAPHANDLE* pBmpDst, double dScale);

    static L_INT ConvertFile(LPCTSTR pszSrcFile,
                             LPCTSTR pszDestFile,
                             L_INT nType,
                             L_INT nPage,
                             L_INT nBitsPerPixel = 0,
                             L_INT nQFactor      = 2,
                             L_INT nPasses       = 0);
    static L_INT ConvertFileMemory(BYTE* pSrcBuf,
                                   int nBufSize,
                                   LPCTSTR pszDestFile,
                                   L_INT nType,
                                   L_INT nPage,
                                   L_INT nBitsPerPixel = 0,
                                   L_INT nQFactor      = 2,
                                   L_INT nPasses       = 0);
    static L_INT CreateMultipageTIFF(LPCTSTR* pszFilesList, int nSrcFiles, LPCTSTR szDestFile);
    static L_INT CreateMultipageTIFFMem(BYTE** pSrcBufs, int* pBufSizes, int nSrcFiles, LPCTSTR szDestFile);

    static L_INT CaptureArea(pBITMAPHANDLE pBitmap);
    static L_INT GetBitmapColorCount(pBITMAPHANDLE pBitmap, L_UINT32& nCount);
};

/////////////////////////////////////////////////////////////////////////////
// class CGraphXException

class GRAPH_API CGraphXException : public CException
{
    DECLARE_DYNAMIC(CGraphXException);

  public:
    CGraphXException(L_INT result, LPCTSTR function);

    // error message
    virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);

    // get private members
    L_INT GetLastError() const
    {
        return m_result;
    }
    LPCTSTR GetErrorFunction() const
    {
        return m_function;
    }

    // helper for error codes
    static CString GetErrorMessage(L_INT error);

  private:
    L_INT m_result;
    char m_function[1024];
};

struct GRAPH_API MULTIPAGEIMAGEHEADER
{
    struct BASE
    {
        char header[32]; // = "Multipage Image Format  "
        char version[8]; // = "   1.00a"
        int pagecount;
    } base;

    struct PAGE
    {
        long offset;
        unsigned long length;
    } page[256];

    MULTIPAGEIMAGEHEADER();
};

#undef GRAPH_API

#endif
