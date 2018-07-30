//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#pragma once

#include <Windows.h>
#include <winerror.h>
#include <WinUser.h>
#include <xmllite.h>

#include <Shlwapi.h>
#include <atlcomcli.h>

#include "Util.h"

#define ERROR_XMLParser(msg) MessageBox(NULL,TEXT(msg),TEXT("XMLParser Error"),MB_OK)

class XMLParser {
protected:
	CComPtr<IStream> pFileStream;
	CComPtr<IXmlReader> pReader;

	XmlNodeType nodeType;
	const WCHAR* pwszFileName = NULL;
	const WCHAR* pwszPrefix = NULL;
	const WCHAR* pwszLocalName = NULL;
	const WCHAR* pwszValue = NULL;

	void Reset();
	BOOL GotoElement(LPCWSTR element_name, int index);
	BOOL GotoChildElement(LPCWSTR element_name, int index, LPCWSTR child_name, int child_index);

	UINT ThenCountChildElements(LPCWSTR element_name, int index, LPCWSTR child_element_name);
	UINT ThenCountAttributes();
	UINT ThenCountAttributeOccurances(LPCWSTR attribute_name, BOOL multiple);
	LPCWSTR ThenGetAttributeValue(LPCWSTR attribute_name, BOOL multiple, int position);

	int CountElement(LPCWSTR element_name);
	int CountAttribute(LPCWSTR element_name, int index);

public:
	XMLParser();
	XMLParser(LPCWSTR filename);
	~XMLParser();

	void Initialize(LPCWSTR filename);
	void Destroy();

	/* FOR DEBUG
	void Test();
	*/

	

};