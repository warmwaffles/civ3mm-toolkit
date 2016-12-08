#ifndef COMBO_BOX_CIV_COLOR_H__CIV3FLCEDIT__INCLUDED_
#define COMBO_BOX_CIV_COLOR_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_COLORS_NUMBER 33
/*
typedef struct 
{
	COLORREF crColour;
	CString  szName[32];
} CivColors;
*/
class CComboBoxCivColor : public CComboBox
{
    // Construction
  public:
    CComboBoxCivColor();

    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CComboBoxCivColor)
    //}}AFX_VIRTUAL

    // Implementation
  public:
    void Initialize();
    void SetItemColor(int nIndex, COLORREF rcColor);
    COLORREF GetItemColor(int nIndex);
    virtual ~CComboBoxCivColor();

    // Generated message map functions
  protected:
    // To hold the colours and their names

    //	static CivColors m_crColours[];
    COLORREF m_crColours[MAX_COLORS_NUMBER];

    //{{AFX_MSG(CComboBoxCivColor)
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // COMBO_BOX_CIV_COLOR_H__CIV3FLCEDIT__INCLUDED_
