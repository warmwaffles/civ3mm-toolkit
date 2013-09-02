// GifCoder.h: interface for the CGifCoder class.
//

#if !defined(GRAPH_X_GIFCODER_H__INCLUDED_)
#define GRAPH_X_GIFCODER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphXExp.h"
#include "LT12\ltcap.h"	// Added by ClassView


typedef unsigned short USHORT;		// 16 bits unsigned integer
#define MAX_CODES   4095



const int CodeMask[13]= {
	0,
	0x0001, 0x0003,
	0x0007, 0x000F,
	0x001F, 0x003F,
	0x007F, 0x00FF,
	0x01FF, 0x03FF,
	0x07FF, 0x0FFF
};

class CGifCoder : public CObject  
{
	struct rgb { byte b,g,r; };
	struct TColTable			/*   Table of colores  */
	{
		SHORT colres;			/* color resolution */
		SHORT sogct;			/* size of global color table */
		rgb paleta[256];		/* palette  */
	};

	struct TGif
	{
		int x, y;				// x,y coordinates of topleft corner
		int cx, cy;				// width & height
		int delay;				// delay in 1/100 seconds
		int TrIndex;			// index of transparent color
		int nRes;				// color resolution
		int RemoveMeth;			// Removing method for image
		byte* PicBuf;			// picture buffer
		COLORREF* ColTable;		// color table (palette)
	};

public:
	L_INT LoadMemory(L_CHAR L_FAR *pBuffer, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT nOrder, L_UINT uFlags, FILEREADCALLBACK pfnCallback, L_VOID L_FAR *pUserData, L_INT32 nBufferSize, pLOADFILEOPTION pLoadOptions, pFILEINFO pFileInfo);
	L_INT LoadBitmapMemory(L_CHAR L_FAR *pBuffer, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT nOrder, L_INT32 nBufferSize, pLOADFILEOPTION pLoadOptions, pFILEINFO pFileInfo);
	L_INT LoadFile(L_CHAR* pszFile, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT nOrder, L_UINT uFlags, FILEREADCALLBACK pfnCallback, L_VOID* pUserData, pLOADFILEOPTION pLoadOptions);
	L_INT LoadBitmap(L_CHAR L_FAR* pszFile, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT nOrder, pLOADFILEOPTION pLoadOptions);
	CGifCoder();
	virtual ~CGifCoder();

// gifdecoder
protected:
	void GetTotalSize (int& cx, int& cy);
	int OpenPicture(LPCTSTR fname);
	int OpenPictureMemory(BYTE* buf, DWORD len);
	short GetNextCode();
	short Decoder(SHORT linewidth, INT&  bad_code_count);
	int ReadGif ();
	int OutLine (byte* pixels, int LineLen);

protected:
	void CreateLTBitmap(pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel);
	FILEINFO m_FileInfo;
	pBITMAPHANDLE m_pBitmap;
	LPVOID m_pUserData;
	FILEREADCALLBACK m_pfnCallback;
	LOADFILEOPTION* m_pLFO;
	int m_nFrames;
	int m_FileLength;
	inline byte GetByte ();
	TGif images[256];
	byte* m_PicBuf;
	int m_nRowsPassed;							// Number of decoded rows
	int m_iypos;								// For interlaced gif
	int m_ipass;
	int m_istep;
	struct TImage{				             /* Image Descriptor */
		USHORT l;
		USHORT t;
		USHORT w;
		USHORT h;
		byte   pf;
	} image;
	struct Tdscgif {                 /* Logic Screen Descriptor  */
		char header[6];        /* Firma and version */
		USHORT scrwidth;
		USHORT scrheight;
		byte pflds;
		byte bcindx;
		byte pxasrat;
	} dscgif;

	COLORREF m_BgColor;					// background color
	BOOL m_IsInterlaced;
	int m_BufPos;
	byte* m_Buf;
	short curr_size;                     /* The current code size */
	short navail_bytes;              /* # bytes left in block */
	short nbits_left;                /* # bits left in current byte */
};


#endif // !defined(GRAPH_X_GIFCODER_H__INCLUDED_)
