// DocXML.cpp: implementation of the CDocXML class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocXML.h"
#include "MFCaids.h"

#include "Utilities.h"
#include "winerror.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDocXML::CDocXML()
{
	pIPagesNodeList	= NULL;
	m_pDoc = NULL;

	CoInitialize(NULL);

	CHECK_COM_RESULT(CoCreateInstance(
		MSXML::CLSID_DOMDocument, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		MSXML::IID_IXMLDOMDocument, 
		(void**)&m_pDoc));

	CHECK_COM_RESULT(m_pDoc->put_async(VARIANT_FALSE));

	CHECK_COM_RESULT(m_pDoc->QueryInterface(MSXML::IID_IXMLDOMNode, (LPVOID *)&m_pCurrentNode));
}

CDocXML::~CDocXML()
{
	SAFE_RELEASE(m_pDoc);
	SAFE_RELEASE(m_pCurrentNode);

	SAFE_RELEASE(pIPagesNodeList);

	POSITION pos = NodeLists.GetHeadPosition();

	while ( pos )
		NodeLists.GetNext(pos)->Release();
}

CDocXMLException::CDocXMLException(HRESULT hr)
{
	m_hResult = hr;
}

CDocXMLException::~CDocXMLException()
{
}

HRESULT CDocXMLException::GetHResultCode()
{
	return m_hResult;
}

BOOL CDocXML::Load(LPCTSTR fileName)
{
	VARIANT_BOOL vb;

	CHECK_COM_RESULT(m_pDoc->load(_variant_t(fileName), &vb));

	if (vb)
	{
		GoRoot();
	}

	return vb;
}

BOOL CDocXML::Save(LPCTSTR fileName)
{
	CHECK_COM_RESULT(m_pDoc->save(_variant_t(fileName)));
/*
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT hr;

	try
	{
		hr = m_pDoc->save(_variant_t(fileName));

		if (SUCCEEDED(hr))
			return TRUE;
		else if (hr == E_ACCESSDENIED)
			AfxMessageBox(uLoadString(IDS_ERR_ACCESSDENIED, fileName, E_ACCESSDENIED), MB_ICONERROR);
		else if (hr == E_INVALIDARG)
			AfxMessageBox(uLoadString(IDS_ERR_INVALIDARG, fileName, E_ACCESSDENIED), MB_ICONERROR);
		else if (hr == E_OUTOFMEMORY)
			AfxMessageBox(uLoadString(IDS_ERR_OUTOFMEMORY, fileName, E_ACCESSDENIED), MB_ICONERROR);
		else
			AfxMessageBox(uLoadString(IDS_ERR_SAVE), MB_ICONERROR);
	}
	catch (...)
	{
		AfxMessageBox(uLoadString(IDS_ERR_SAVE), MB_ICONERROR);
	}
*/
	return TRUE;
}

BOOL CDocXML::SaveXmlDoc(LPCTSTR fileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT hr;

	try
	{
		hr = m_pDoc->save(_variant_t(fileName));

		if (SUCCEEDED(hr))
			return TRUE;
		else if (hr == E_ACCESSDENIED)
			AfxMessageBox(uFormatString(IDS_ERR_ACCESSDENIED, fileName, E_ACCESSDENIED), MB_ICONERROR);
		else if (hr == E_INVALIDARG)
			AfxMessageBox(uFormatString(IDS_ERR_INVALIDARG, fileName, E_ACCESSDENIED), MB_ICONERROR);
		else if (hr == E_OUTOFMEMORY)
			AfxMessageBox(uFormatString(IDS_ERR_OUTOFMEMORY, fileName, E_ACCESSDENIED), MB_ICONERROR);
		else
			AfxMessageBox(uFormatString(IDS_ERR_SAVE, fileName), MB_ICONERROR);
	}
	catch (...)
	{
		AfxMessageBox(uLoadString(IDS_ERR_SAVE), MB_ICONERROR);
	}

	return FALSE;
}

MSXML::IXMLDOMDocument *CDocXML::GetDoc()
{
	return m_pDoc;
}

CString CDocXMLException::GetErrorDescription()
{
	CString errDesc;

	FormatMessage( 
		/*FORMAT_MESSAGE_ALLOCATE_BUFFER | */
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		m_hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		errDesc.GetBuffer(1024),
		1024,
		NULL 
	);
	errDesc.ReleaseBuffer();

	return errDesc;
}


void CDocXML::CreateDocHeader(LPCTSTR encoding)
{
	_bstr_t data;

	data = " version=\"1.0\" encoding=\"";
	data += encoding;
	data += "\"";

	MSXML::IXMLDOMProcessingInstruction *pProcInst = NULL;
	CHECK_COM_RESULT(m_pDoc->createProcessingInstruction(L"xml", data, &pProcInst));

	CHECK_COM_RESULT(m_pDoc->appendChild(pProcInst, NULL));

	SAFE_RELEASE(pProcInst);
}

void CDocXML::CreateElement(LPCTSTR name)
{
	MSXML::IXMLDOMElement *pElement;
	CHECK_COM_RESULT(m_pDoc->createElement(_bstr_t(name), &pElement));

	MSXML::IXMLDOMNode *pNode = NULL;
	CHECK_COM_RESULT(m_pCurrentNode->appendChild(pElement, &pNode));

	SAFE_RELEASE(pElement);
	
	SAFE_RELEASE(m_pCurrentNode);
	
	m_pCurrentNode = pNode;
}

void CDocXML::CreateTextNode(LPCTSTR name)
{
	MSXML::IXMLDOMText *pText;
	CString str(name);
	BSTR bstr = str.AllocSysString();
	CHECK_COM_RESULT(m_pDoc->createTextNode(bstr, &pText));
	SysFreeString(bstr);

	CHECK_COM_RESULT(m_pCurrentNode->appendChild(pText, NULL));
	
	SAFE_RELEASE(pText);
}

BOOL CDocXML::SetParentCurrent()
{
	MSXML::IXMLDOMNode *pNode;

	CHECK_COM_RESULT(m_pCurrentNode->get_parentNode(&pNode));

	if (pNode == NULL)
		return FALSE;

	SAFE_RELEASE(m_pCurrentNode);
	
	m_pCurrentNode = pNode;

	return TRUE;
}

void CDocXML::CreateAttribute(LPCTSTR name, LPCTSTR value)
{
	MSXML::IXMLDOMElement *pElement;
	if (SUCCEEDED(m_pCurrentNode->QueryInterface(MSXML::IID_IXMLDOMElement, (LPVOID *)&pElement)))
	{
		CHECK_COM_RESULT(pElement->setAttribute(_bstr_t(name), _variant_t(value)));

		SAFE_RELEASE(pElement);
	}
	else
		ASSERT(FALSE);
}

CString CDocXML::GetNodeName()
{
	BSTR bstr;

	CHECK_COM_RESULT(m_pCurrentNode->get_nodeName(&bstr));

	CString str(bstr); 
	SysFreeString(bstr);

	return str;
}

BOOL CDocXML::ChangeNode(LPCTSTR name)
{
	VARIANT_BOOL bHasChild;

	CHECK_COM_RESULT(m_pCurrentNode->hasChildNodes(&bHasChild));

	if (!bHasChild)
		return FALSE;

	MSXML::IXMLDOMNode *pNode;
	CHECK_COM_RESULT(m_pCurrentNode->get_firstChild(&pNode));

	MSXML::IXMLDOMNode *pNodeNext;
	do
	{
		BSTR bstr;
		pNode->get_nodeName(&bstr);

		CString str(bstr);
		SysFreeString(bstr);
		if (str.Compare(name) == 0)
		{
			SAFE_RELEASE(m_pCurrentNode);
			m_pCurrentNode = pNode;

			return TRUE;
		}

		CHECK_COM_RESULT(pNode->get_nextSibling(&pNodeNext));
		SAFE_RELEASE(pNode);
		pNode = pNodeNext;
	} while (pNode);

	return FALSE;
}

CString CDocXML::GetTextNodeValue()
{
//	_variant_t value;

	if (!ChangeNode("#text"))
		return "";

	MSXML::IXMLDOMText *pCData = NULL;
	CHECK_COM_RESULT(m_pCurrentNode->QueryInterface(MSXML::IID_IXMLDOMText, (LPVOID *)&pCData));
	BSTR bstr;
	pCData->get_data(&bstr);
	SAFE_RELEASE(pCData);
//	CHECK_COM_RESULT(m_pCurrentNode->get_nodeValue(&value));
	SetParentCurrent();

//	_bstr_t bstr = value;
//	BSTR str = bstr;
	CString str = bstr;
	SysFreeString(bstr);

	return str;
}

BOOL CDocXML::ChangeNode(LPCTSTR name, INT id)
{
	VARIANT_BOOL bHasChild;

	CHECK_COM_RESULT(m_pCurrentNode->hasChildNodes(&bHasChild));

	if (!bHasChild)
		return FALSE;

	MSXML::IXMLDOMNode *pNode;
	CHECK_COM_RESULT(m_pCurrentNode->get_firstChild(&pNode));

	MSXML::IXMLDOMNode *pNodeNext;
	do
	{
		BSTR bstr;
		pNode->get_nodeName(&bstr);

		CString str(bstr);
		SysFreeString(bstr);
		if (str.Compare(name) == 0)
		{
			MSXML::IXMLDOMNamedNodeMap *pNodeMap;

			CHECK_COM_RESULT(pNode->get_attributes(&pNodeMap));

			MSXML::IXMLDOMNode *pAttribute;
			CHECK_COM_RESULT(pNodeMap->getNamedItem(_bstr_t(xmlId), &pAttribute));

			SAFE_RELEASE(pNodeMap);
			variant_t vt;
//			BSTR bstr;
			CHECK_COM_RESULT(pAttribute->get_nodeValue(&vt));
			SAFE_RELEASE(pAttribute);
//			pAttribute->get_nodeName(&bstr);
//			CString att(bstr);
			if (vt == _variant_t(uInt2Str(id)))
			{
				SAFE_RELEASE(m_pCurrentNode);
				m_pCurrentNode = pNode;
				return TRUE;
			}
		}

		CHECK_COM_RESULT(pNode->get_nextSibling(&pNodeNext));
		SAFE_RELEASE(pNode);
		pNode = pNodeNext;
	} while (pNode);

	return FALSE;
}

CString CDocXML::GetAttribute(LPCTSTR att)
{
	MSXML::IXMLDOMNamedNodeMap *pNodeMap = NULL;
	
	CHECK_COM_RESULT(m_pCurrentNode->get_attributes(&pNodeMap));

	if (pNodeMap == NULL)
	{
		SAFE_RELEASE(pNodeMap);
		return "";
	}

	MSXML::IXMLDOMNode *pAttribute = NULL;
//	CHECK_COM_RESULT(pNodeMap->getNamedItem(_bstr_t(att), &pAttribute));
	if (FAILED(pNodeMap->getNamedItem(_bstr_t(att), &pAttribute)))
	{
		SAFE_RELEASE(pNodeMap);
		return "";
	}

	SAFE_RELEASE(pNodeMap);

	if (pAttribute == NULL)
	{
		SAFE_RELEASE(pAttribute);
		return "";
	}

	variant_t vt;
	CHECK_COM_RESULT(pAttribute->get_nodeValue(&vt));
	SAFE_RELEASE(pAttribute);
	
	_bstr_t bstr = vt;
	
	return (LPCTSTR)bstr;
}

void CDocXML::Attach(IUnknown *pUnk)
{
	CHECK_COM_RESULT(pUnk->QueryInterface(MSXML::IID_IXMLDOMDocument, (LPVOID *)&m_pDoc));

	GoRoot();
}

_bstr_t CDocXML::GetXML()
{
	BSTR bsTmp;

	m_pDoc->get_xml(&bsTmp);
	
	_bstr_t bsRet(bsTmp, FALSE);

	return bsRet;
}

BOOL CDocXML::SetPage(int nPage)
{
	long nPages = 0;

	if ( pIPagesNodeList )
		pIPagesNodeList->Release();
	
	m_pDoc->getElementsByTagName((_bstr_t)"Page", &pIPagesNodeList);
	
	pIPagesNodeList->get_length(&nPages);

	if ( nPages <= nPage )
		return FALSE;
	
	if ( m_pCurrentNode )
		m_pCurrentNode->Release();

	pIPagesNodeList->get_item(nPage, &m_pCurrentNode);

	return TRUE;
}

void CDocXML::GoUp(int nLevel)
{
	for (int i = 0; i < nLevel; i++)
		SetParentCurrent();
}

BOOL CDocXML::SetFirstNode(POSITION &pos, LPCSTR pNodeName)
{
	long					nItem		= 0;
	BOOL					bResult		= FALSE;
	MSXML::IXMLDOMNodeList	*pINodeList;
	
	m_pCurrentNode->selectNodes((_bstr_t)pNodeName, &pINodeList);
	
	pINodeList->get_length(&nItem);
	
	if ( nItem )
	{
		if ( m_pCurrentNode )
			m_pCurrentNode->Release();
		
		pINodeList->get_item(0, &m_pCurrentNode);
		
		bResult = TRUE;
	}
	NodeLists.AddHead(pINodeList);
	pos = NodeLists.GetHeadPosition();

	return bResult;
}

BOOL CDocXML::SetNode(POSITION posProc, int i)
{
	long nItem		= 0;
	BOOL bResult	= FALSE;
	
	NodeLists.GetAt(posProc)->get_length(&nItem);
	
	if ( nItem )
		if ( m_pCurrentNode )
			m_pCurrentNode->Release();

	if ( nItem > i )
	{
//		if ( m_pCurrentNode )
//			m_pCurrentNode->Release();
		
		NodeLists.GetAt(posProc)->get_item(i, &m_pCurrentNode);
		
		bResult = TRUE;
	}
	else if ( nItem )
		NodeLists.GetAt(posProc)->get_item(nItem - 1, &m_pCurrentNode);
	
	return bResult;
}

void CDocXML::CloseNodes(POSITION posNodeList)
{
	NodeLists.GetAt(posNodeList)->Release();
	NodeLists.RemoveAt(posNodeList);
}

void CDocXML::LoadXML(_bstr_t sXMLDoc)
{
	VARIANT_BOOL vb;
	
	CHECK_COM_RESULT(m_pDoc->loadXML(sXMLDoc, &vb));
	
	if (vb)
	{
		GoRoot();
	}
}

BOOL CDocXML::GoRoot()
{
	MSXML::IXMLDOMElement *pElement;
	CHECK_COM_RESULT(m_pDoc->get_documentElement(&pElement));
	if ( !pElement )
		return FALSE;

	SAFE_RELEASE(m_pCurrentNode);
	CHECK_COM_RESULT(pElement->QueryInterface(MSXML::IID_IXMLDOMNode, (LPVOID *)&m_pCurrentNode));
	pElement->Release();

	return TRUE;
}
