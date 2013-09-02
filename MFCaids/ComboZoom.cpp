// ComboZoom.cpp : implementation file
//

#include "stdafx.h"
#include "ComboZoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboZoom

CComboZoom::CComboZoom()
{
	m_pWnd = NULL;
}

CComboZoom::~CComboZoom()
{
}

BEGIN_MESSAGE_MAP(CComboZoom, CComboBox)
	//{{AFX_MSG_MAP(CComboZoom)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboZoom message handlers

int CComboZoom::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitComboZoom();
	return 0;
}

BOOL CComboZoom::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((int)pMsg->wParam == VK_RETURN)
		{
			// if Enter key was pressed

			// set the message that the current selection in 
			// the combo box was changed
			if (m_pWnd)
			{
				m_pWnd->SendMessage (WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)this->m_hWnd);
				m_pWnd->SetFocus();
			}
		}
		else if ((int)pMsg->wParam == VK_ESCAPE)
		{
			if (m_pWnd)
				m_pWnd->SetFocus();
		}
		else if (pMsg->wParam== VK_DELETE)
		{
			SendMessage (WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return true;
		}
		else if (pMsg->wParam== VK_BACK)
		{
			PostMessage (WM_CHAR, pMsg->wParam, pMsg->lParam);
			return true;
		}
	}
	else if (pMsg->message == WM_CHAR)
	{
		if ((((UINT)pMsg->wParam < '0') || ((UINT)pMsg->wParam > '9'))
			&& ((UINT)pMsg->wParam != '\b') && ((UINT)pMsg->wParam != '%'))
		{
			return TRUE;
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CComboZoom::ZoomIn()
{
	CString strText;
	GetWindowText(strText);

	int nRatio = atoi(strText);

	for (int i = 0; i < GetCount(); i++)
	{
		GetLBText(i, strText);
		int nZoomPoint = atoi(strText);
		if (nRatio < nZoomPoint)
		{
			int test = GetDlgCtrlID();
			SetCurSel(i);
			if (m_pWnd)
				m_pWnd->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)m_hWnd);
			break;
		}
	}
}

void CComboZoom::ZoomOut()
{
	CString strText;
	GetWindowText(strText);

	int nRatio = atoi(strText);

	for (int i = GetCount() - 1; i >= 0; i--)
	{
		GetLBText(i, strText);
		int nZoomPoint = atoi(strText);
		if (nRatio > nZoomPoint)
		{
			SetCurSel(i);
			if (m_pWnd)
				m_pWnd->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)m_hWnd);
			break;
		}
	}
}

void CComboZoom::SetParentWnd(CWnd *pWnd)
{
	m_pWnd = pWnd;
}

void CComboZoom::InitComboZoom(int nZoomFactor /*= 0*/)
{
	ResetContent();

	m_pWnd = GetParent();
	LOGFONT logFont;
	m_pWnd->GetFont()->GetLogFont(&logFont);
	m_font.CreateFontIndirect(&logFont);
	
	SetFont(&m_font);
	
	LimitText(5);
	AddString(_T("10%"));
	AddString(_T("25%"));
	AddString(_T("50%"));
	AddString(_T("75%"));
	AddString(_T("100%"));
	AddString(_T("150%"));
	AddString(_T("200%"));
	AddString(_T("300%"));
	AddString(_T("500%"));
	AddString(_T("750%"));
	AddString(_T("1000%"));

	if (nZoomFactor != 0)
	{
		CString strValue;
		strValue.Format("%d%%", nZoomFactor);
		SetWindowText(strValue);
	}
	else
		SelectString(-1, _T("100%"));
}
