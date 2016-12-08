#include "stdafx.h"
#include "GraphicsLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPcx::CPcx(void)
{
    m_pBitmapInfo = NULL;
    m_pStorage    = NULL;
    m_pData       = NULL;
}

CPcx::~CPcx(void)
{
    DestroyPcx();
}

BOOL
CPcx::LoadBgdImgFile(LPCTSTR lpszPathName)
{
    if (lpszPathName[0] == NULL)
        return FALSE;

    HANDLE fHandle = CreateFile(lpszPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (fHandle == INVALID_HANDLE_VALUE)
        return FALSE;

    ULONG bytesRead;
    long size = GetFileSize(fHandle, NULL);

    m_pData = new BYTE[size];

    if (!ReadFile(fHandle, m_pData, size, &bytesRead, NULL))
        return FALSE;

    CloseHandle(fHandle);
    fHandle = INVALID_HANDLE_VALUE;

    BYTE* lpTmp = m_pData;

    memcpy(&m_pcxHeader, lpTmp, sizeof(PCX_HEADER));
    lpTmp += sizeof(PCX_HEADER);

    // calc the pixel size of the image
    m_nWidth  = (m_pcxHeader.Xmax - m_pcxHeader.Xmin) + 1;
    m_nHeight = (m_pcxHeader.Ymax - m_pcxHeader.Ymin) + 1;

    int scanLineWidth = m_pcxHeader.NPlanes * m_pcxHeader.BytesPerLine;
    char* pScanLine   = new char[scanLineWidth + 1];

    m_pStorage = new BYTE[m_nWidth * m_nHeight * (8 >> 3)];
    int p      = 0;
    int w      = 0;
    BYTE* row  = NULL;

    for (int h = 0; h < m_nHeight; h++) {
        // decode each plane of RGB data, each having m_hdr.BytesPerLine
        for (p = 0; p < m_pcxHeader.NPlanes; p++) {
            if (DecodePcxLine(pScanLine + (p * m_pcxHeader.BytesPerLine), lpTmp, m_pcxHeader.BytesPerLine) != m_pcxHeader.BytesPerLine) {
                SAFE_DELETE_MULTI(pScanLine);
                return FALSE;
            }
        }

        // we need to deal with this on a row by row basis
        row = (BYTE*)(&m_pStorage[h * m_nWidth * m_pcxHeader.NPlanes]);

        for (w = 0; w < m_nWidth; w++)
            *row++ = pScanLine[w];
    } //for (int h = 0; h < m_nHeight; h++)

    SAFE_DELETE_MULTI(pScanLine);

    SetupBitmapInfo();

    RGBQUAD* Ptr = &m_pBitmapInfo->bmiColors[0];
    RGBQUAD buf;
    ZeroMemory(&buf, sizeof(buf));

    // READ 256 colour PALETTE
    lpTmp = &m_pData[size - 769];
    int c = (int)((CHAR)*lpTmp);
    lpTmp++;

    if (c == 12) {
        for (c = 0; c < COLOR8; c++, Ptr++) {
            memcpy(&buf, lpTmp, 3);
            lpTmp += 3;

            Ptr->rgbRed      = buf.rgbBlue;
            Ptr->rgbGreen    = buf.rgbGreen;
            Ptr->rgbBlue     = buf.rgbRed;
            Ptr->rgbReserved = 0;

            // Get palette color (RGB) and store it in array
            // convert BGR to RGB (stupid IBM!)
            m_rgbPal.pal[c].rgbBlue  = buf.rgbRed;
            m_rgbPal.pal[c].rgbGreen = buf.rgbGreen;
            m_rgbPal.pal[c].rgbRed   = buf.rgbBlue;
        }
    }

    return TRUE;
}

void
CPcx::DestroyPcx(void)
{
    SAFE_DELETE_MULTI(m_pData);
    SAFE_DELETE_MULTI(m_pStorage);
    SAFE_FREE(m_pBitmapInfo);
}

int
CPcx::DecodePcxLine(char* dest, BYTE*& src, int bytes)
{
    int n = 0;
    int i;
    char c;

    do {
        c = *src & 0xFF;
        src++;

        if ((c & 0xC0) == 0xC0) {
            i = c & 0x3F;
            c = *src;
            src++;

            while (i--)
                dest[n++] = c;
        } else
            dest[n++] = c;
    } while (n < bytes);

    return n;
}

void
CPcx::SetupBitmapInfo(void)
{
    //allocate 256 color rgbquads for maximum usage
    m_pBitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 256));

    if (m_pBitmapInfo == NULL)
        return;

    ZeroMemory(m_pBitmapInfo, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 256));

    m_pBitmapInfo->bmiHeader.biSize     = sizeof(m_pBitmapInfo->bmiHeader);
    m_pBitmapInfo->bmiHeader.biWidth    = m_nWidth;
    m_pBitmapInfo->bmiHeader.biHeight   = m_nHeight;
    m_pBitmapInfo->bmiHeader.biPlanes   = 1;
    m_pBitmapInfo->bmiHeader.biBitCount = BITS_8;
}

HRESULT
CPcx::CopyToSurface(CSurface* pSurface)
{
    DDSURFACEDESC ddsd;

    BYTE* sb;
    BYTE* db;
    int x, y;

    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

    if (FAILED(pSurface->GetDDS()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))
        return E_FAIL;

    sb = (BYTE*)m_pStorage;
    db = (BYTE*)ddsd.lpSurface;

    RGBQUAD* Ptr = NULL;

    for (y = 0; y < pSurface->flags.height; y++) {
        for (x = 0; x < pSurface->flags.width; x++) {
            Ptr = &m_pBitmapInfo->bmiColors[*sb];

            db[0] = Ptr->rgbBlue;
            db[1] = Ptr->rgbGreen;
            db[2] = Ptr->rgbRed;
            db[3] = 255;

            db += 4;
            sb += 1;
        }
    }

    pSurface->GetDDS()->Unlock(NULL);

    return S_OK;
}
