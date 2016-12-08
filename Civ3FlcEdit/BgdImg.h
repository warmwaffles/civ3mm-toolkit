#if !defined(BGD_IMG_H__CIV3FLC__INCLUDED)
#define BGD_IMG_H__CIV3FLC__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Definitions.h"

#include "..\MFCaids\GraphXImage.h"

class CSurface;

class CBgdImg : public CGraphXImage
{
  public:
    CBgdImg();
    CBgdImg(LPCTSTR sPath);
    virtual ~CBgdImg();

    virtual BOOL LoadBgdImgFile(LPCTSTR sPath);
    HRESULT CopyToSurface(CSurface* pSurface);

  protected:
    UINT m_nWidth;
    UINT m_nHeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(BGD_IMG_H__CIV3FLC__INCLUDED)
