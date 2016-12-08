// DocXML.h: interface for the CDocXML class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DOCXML_H__MFCAIDS__INCLUDED_)
#define DOCXML_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "msxml.dll" named_guids raw_interfaces_only
//#import "msxml.dll"
//using namespace MSXML;
//#import "msxml.dll" named_guids
//using namespace MSXML;

#include <afxtempl.h>

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

#define CHECK_COM_RESULT(hres)                                                                                                             \
    {                                                                                                                                      \
        HRESULT hr = hres;                                                                                                                 \
        if (FAILED(hr))                                                                                                                    \
            throw CDocXMLException(hr);                                                                                                    \
    }

class MFCAIDS_API CDocXMLException
{
  public:
    CString GetErrorDescription();
    HRESULT GetHResultCode();
    CDocXMLException(HRESULT hr);
    virtual ~CDocXMLException();

  protected:
    HRESULT m_hResult;
};

class MFCAIDS_API CDocXML
{
  public:
    BOOL GoRoot();
    void LoadXML(_bstr_t sXMLDoc);
    void CloseNodes(POSITION posNodeList);
    BOOL SetNode(POSITION posProc, int i);
    BOOL SetFirstNode(POSITION& pos, LPCSTR pNodeName);
    void GoUp(int nLevel);
    BOOL SetPage(int nPage);
    _bstr_t GetXML();
    void Attach(IUnknown* pUnk);
    CString GetAttribute(LPCTSTR att);
    BOOL ChangeNode(LPCTSTR name, INT id);
    CString GetTextNodeValue();
    BOOL ChangeNode(LPCTSTR name);
    CString GetNodeName();
    void CreateAttribute(LPCTSTR name, LPCTSTR value);
    BOOL SetParentCurrent();
    void CreateTextNode(LPCTSTR name);
    void CreateElement(LPCTSTR name);
    void CreateDocHeader(LPCTSTR encoding);
    BOOL Save(LPCTSTR fileName);
    BOOL SaveXmlDoc(LPCTSTR fileName);
    MSXML::IXMLDOMDocument* GetDoc();
    BOOL Load(LPCTSTR fileName);
    CDocXML();
    virtual ~CDocXML();

  protected:
    MSXML::IXMLDOMDocument* m_pDoc;
    MSXML::IXMLDOMNode* m_pCurrentNode;
    MSXML::IXMLDOMNode* m_pLevelNode;
    MSXML::IXMLDOMNodeList* pIPagesNodeList;
    CList<MSXML::IXMLDOMNodeList*, MSXML::IXMLDOMNodeList*> NodeLists;
};

static LPCTSTR xmlDoc              = "Document";
static LPCTSTR xmlDocInfo          = "Document_Info";
static LPCTSTR xmlVersion          = "Version";
static LPCTSTR xmlAuthor           = "Author";
static LPCTSTR xmlDescription      = "Description";
static LPCTSTR xmlCreationDate     = "Creation_Date";
static LPCTSTR xmlCreationTime     = "Creation_Time";
static LPCTSTR xmlModificationDate = "Modification_Date";
static LPCTSTR xmlModificationTime = "Modification_Time";
static LPCTSTR xmlSyntax           = "Syntax";
static LPCTSTR xmlBy               = "by";
static LPCTSTR xmlEncoding         = "UTF-8";
static LPCTSTR xmlId               = "ID";

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(DOCXML_H__MFCAIDS__INCLUDED_)
