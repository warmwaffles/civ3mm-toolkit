// FlcImage.h: interface for the CFlcImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FLC_IMAGE_H__DPDocPageINCLUDED_)
#define FLC_IMAGE_H__DPDocPageINCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FlcView.h"
#include "..\MFCaids\GraphXImage.h"

static const UINT m_constBits = BITS_8;

BOOL ImportPaletteFromFile(CString sPathName, RGBQUADPAL &palette);
BOOL ExportPaletteToFile(CString sPathName, RGBQUADPAL &palette, EPaletteFileType type, LPCTSTR sExtName = NULL);
void RemovePaletteFile(CString sPathName, EPaletteFileType type);
BOOL ExportPalette(RGBQUADPAL &palette);
BOOL ImportPalette(RGBQUADPAL &palette);
extern BOOL ReadC3F(CString sPath, C3FSTRUCT *c3f);
extern BOOL WriteC3F(CString sPath, C3FSTRUCT c3f);
extern BOOL WriteC3F(CString sPath, 
					 UINT nWidth, UINT nHeight, 
					 BYTE nFrmWidth, BYTE nFrmHeight, 
					 WORD nFrmCount, BYTE nDirCount,
					 WORD nDelay,
					 BYTE nXOffset, BYTE nYOffset,
					 BYTE nFileType, BYTE nPalType,
					 BOOL bLeaderHead);
extern BYTE GetPaletteIndex(RGBQUADPAL &rgbPal, COLORREF &color);

class CFlcImage : public CGraphXImage
{
	friend class CFlcView;
	friend class CC3fView;
	friend class CAnimationView;
	friend class CFileImgDlg;
	friend class CImportToC3F;

public:
	CFlcImage();
	CFlcImage(LPCTSTR sPath);
	CFlcImage(CFlcView* pFlc);
	CFlcImage(CFlcView* pFlc, LPCTSTR sPath);
	virtual ~CFlcImage();

public:
	BOOL SaveFrameToFile(CAnimationView* pAnim);
	BOOL SaveToFile(CAnimationView* pAnim = NULL);
	BOOL CopyToFile(CAnimationView* pAnim = NULL);
	BOOL CopyToClipboard();
	BOOL OpenStoryboard(CString sPath, C3FSTRUCT &c3f);
	BOOL NewStoryboard();					// create an empty storyboard with specified parameters
	BOOL FlcToStoryboard();					// convert FLC to storyboard
	void AttachFlcViewer(CFlcView* pFlc);
	BOOL CreateStoryboard(					// create an empty storyboard
		UINT nWidth,
		UINT nHeight,
		BYTE nFrmWidth,
		BYTE nFrmHeight,
		BYTE nDirect,
		WORD nFrmCount,
		BYTE nBitsPerPixel = BITS_8);

	static RGBQUAD m_rgbLast32Pal[32];

protected:
	void ReplaceLast32Colors(ELastPalType nLastPalType, LPCSTR sLastPalPath = NULL);
	BOOL CreateFile(LPCSTR sPath);			// create an empty file by the given name
	COLORREF GetColor(int nIndex)			// get color from palette
	{ 
		return RGB(
		m_rgbPal[nIndex].rgbRed,
		m_rgbPal[nIndex].rgbGreen,
		m_rgbPal[nIndex].rgbBlue
		);
	}

	CFlcView*	m_pFlc;
	RGBQUAD		m_rgbPal[COLOR8];			// Palette of the bitmap
};

#endif // !defined(FLC_IMAGE_H__DPDocPageINCLUDED_)
