//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// 
// FILE NAME:   FormViewX.cpp
// AUTHOR:      Tommy Svensson and Ari Greenberg (???)
// DESCRIPTION:	FormViewX implements "cool" form view.
//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include "stdafx.h"
#include "FormViewX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewX

//IMPLEMENT_DYNCREATE(CFormViewX, CFormView)
// here follows the IMPLEMENT_DYNCREATE macro substitute
CObject* PASCAL CFormViewX::CreateObject()
{
	return new CFormViewX(CFormViewX::IDD);
}
IMPLEMENT_RUNTIMECLASS(CFormViewX, CFormView, 0xFFFF, CFormViewX::CreateObject)


BEGIN_MESSAGE_MAP(CFormViewX, CFormView)
	//{{AFX_MSG_MAP(CFormViewX)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFormViewX::CFormViewX(UINT nIDTemplate)
: CFormView(nIDTemplate)
{
	//{{AFX_DATA_INIT(CFormViewX)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_xFormSize  = m_yFormSize = 0;
	m_bFirstSize = TRUE;
}

CFormViewX::~CFormViewX()
{
}

void CFormViewX::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormViewX)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CFormViewX message handlers

void CFormViewX::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		return;

	if ((!cx) && (!cy))
		return;

	if (!(m_xFormSize || m_yFormSize))
	{
		m_xFormSize = cx;
		m_yFormSize = cy;

		return;
	}

	if (m_bFirstSize)
	{
		for (int i = 0; i < m_CtrlInfo.GetSize(); i++)
		{
			GetDlgItem(m_CtrlInfo[i].m_id)->GetWindowRect(&m_CtrlInfo[i].m_pos);
			ScreenToClient(&m_CtrlInfo[i].m_pos);
		}
		m_bFirstSize = FALSE;
	}

	int dx = cx - m_xFormSize;
	int dy = cy - m_yFormSize;

	// Move and Size the controls using the information
	// we got in SetControlInfo()
	//
	CRect WndRect;
	CWnd *pWnd;
	CFormCtrlInfo cInfo;

	for (int i = 0; i < m_CtrlInfo.GetSize(); i++)
	{
		cInfo = m_CtrlInfo[i];
		pWnd = GetDlgItem(cInfo.m_id);

		if (!pWnd)
		{
			TRACE("Control ID - %d NOT FOUND!!\n", cInfo.m_id);
			continue;
		}

		if (!cInfo.m_Anchor)
			continue; // do nothing if anchored to top and or left

		WndRect = cInfo.m_pos;

		if (cInfo.m_Anchor & CF_RESIZE_HOR)
			WndRect.right += dx * cInfo.m_xDivWidth / cInfo.m_xDivider;

		if (cInfo.m_Anchor & CF_ANCHOR_RIGHT)
			WndRect.OffsetRect(dx * cInfo.m_xDivPos / cInfo.m_xDivider, 0);

		if (cInfo.m_Anchor & CF_RESIZE_VER)
			WndRect.bottom += dy * cInfo.m_yDivHeight / cInfo.m_yDivider;


		if (cInfo.m_Anchor & CF_ANCHOR_BOTTOM)
			WndRect.OffsetRect(0, dy * cInfo.m_yDivPos / cInfo.m_yDivider);

		pWnd->MoveWindow(&WndRect, TRUE);
	}
}

void CFormViewX::SetControlInfo(UINT CtrlId, WORD Anchor, WORD xDivider, WORD xDivPos, WORD yDivider, WORD yDivPos, WORD xDivWidth, WORD yDivHeight)
{
	if (Anchor == CF_ANCHOR_LEFT)
		return; // Do nothing

	CFormCtrlInfo cfci;
	cfci.m_id         = CtrlId;
	cfci.m_Anchor     = Anchor;
	cfci.m_xDivider   = xDivider;
	cfci.m_xDivPos    = xDivPos;
	cfci.m_yDivider   = yDivider;
	cfci.m_yDivPos    = yDivPos;
	cfci.m_xDivWidth  = xDivWidth;
	cfci.m_yDivHeight = yDivHeight;

	m_CtrlInfo.Add(cfci);
}

BOOL CFormViewX::GetControlPos(UINT CtrlId, CRect &pos)
{
	int id;
	if ((id = FindCtrl(CtrlId)) == -1)
		return FALSE;

	pos = m_CtrlInfo.GetAt(id).m_pos;

	return TRUE;
}

void CFormViewX::SetControlPos(UINT CtrlId, CRect &pos)
{
	int id;
	if ((id = FindCtrl(CtrlId)) == -1)
		return;

	CFormCtrlInfo cfci = m_CtrlInfo.GetAt(id);
	cfci.m_pos = pos;
	m_CtrlInfo.SetAt(id, cfci);
}

int CFormViewX::FindCtrl(UINT CtrlId)
{
	for (int i = 0; i < m_CtrlInfo.GetSize(); i++)
		if (m_CtrlInfo.GetAt(i).m_id == CtrlId)
			return i;

	return -1;
}

