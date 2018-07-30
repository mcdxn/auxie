//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "XMLParser.h"

XMLParser::~XMLParser()
{}

XMLParser::XMLParser()
{
	nodeType = XmlNodeType_None;
	pwszFileName = NULL;
	pwszPrefix = NULL;
	pwszLocalName = NULL;
	pwszValue = NULL;
}

XMLParser::XMLParser(LPCWSTR filename)
{
	this->Initialize(filename);
}

void XMLParser::Initialize(LPCWSTR filename)
{
	HRESULT hr = S_OK;
	Sleep(100);
	if (FAILED(hr = SHCreateStreamOnFileEx(filename, STGM_READWRITE | STGM_SHARE_DENY_NONE, FILE_ATTRIBUTE_NORMAL, FALSE, NULL, &pFileStream)))
		ERROR_XMLParser("SHCreateStreamOnFile");

	if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&this->pReader, NULL)))
		ERROR_XMLParser("CreateXmlReader");

	if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
		ERROR_XMLParser("pReader->SetProperty");

	if (FAILED(hr = pReader->SetInput(this->pFileStream)))
		ERROR_XMLParser("pReader->SetInput");
}

void XMLParser::Reset()
{
	HRESULT hr = S_OK;
	LARGE_INTEGER i = {};

	hr = this->pFileStream->Seek(i, STREAM_SEEK_SET, NULL);
	if (FAILED(hr = pReader->SetInput(this->pFileStream)))
		ERROR_XMLParser("pReader->SetInput");
}


int XMLParser::CountElement(LPCWSTR element_name)
{
	INT count = 0;
	LPCWSTR element = NULL;
	HRESULT hr = S_OK;
	this->Reset();

	while (S_OK == (hr = pReader->Read(&this->nodeType)))
	{
		if (this->nodeType == XmlNodeType_Element)
		{
			pReader->GetLocalName(&element, NULL);
			if (lstrcmpiW(element, element_name) == 0)
				count += 1;
		}
	}

	return count;
}

int XMLParser::CountAttribute(LPCWSTR element_name, int index)
{
	INT count = 0;
	UINT count_attr = 0;
	LPCWSTR element = NULL;
	HRESULT hr = S_OK;
	this->Reset();
	if (this->GotoElement(element_name, index))
		goto done;
	else return NULL;

done: pReader->GetAttributeCount(&count_attr);
	return count_attr;
}

BOOL XMLParser::GotoElement(LPCWSTR element_name, int index)
{
	INT count = 0;
	LPCWSTR element = NULL;
	HRESULT hr = S_OK;

	int max_count = this->CountElement(element_name);
	if (index > max_count)
		return FALSE;

	this->Reset();

	while (S_OK == (hr = pReader->Read(&this->nodeType)))
	{
		if (this->nodeType == XmlNodeType_Element)
		{
			pReader->GetLocalName(&element, NULL);
			if (lstrcmpiW(element, element_name) == 0)
			{
				count += 1;
				if (count == index)
					goto done;
			}
		}
	}

done: return TRUE;
}

BOOL XMLParser::GotoChildElement(LPCWSTR element_name, int index, LPCWSTR child_name, int child_index)
{
	this->GotoElement(element_name, index);

	INT count = 0;
	LPCWSTR element = NULL;
	HRESULT hr = S_OK;

	while (S_OK == (hr = pReader->Read(&this->nodeType)))
	{
		if (this->nodeType == XmlNodeType_Element)
		{
			pReader->GetLocalName(&element, NULL);
			if (lstrcmpiW(element, child_name) == 0)
			{
				count += 1;
				if (count == child_index)
					goto done;
			}
		}
	}

done: return TRUE;
}
UINT XMLParser::ThenCountChildElements(LPCWSTR element_name, int index, LPCWSTR child_element_name)
{
	INT child_element_count = 0;
	UINT size = 0;
	LPCWSTR element = NULL;
	HRESULT hr = S_OK;

	this->GotoElement(element_name, index);

	while (S_OK == (hr = pReader->Read(&this->nodeType)))
	{
		switch (this->nodeType)
		{
		case XmlNodeType_Element:
		{
			pReader->GetLocalName(&element, &size);
			if (lstrcmpiW(element, child_element_name) == NULL)
				++child_element_count;
		}break;
		case XmlNodeType_EndElement: goto Finish;
		default: break;
		}
	}

Finish:	this->GotoElement(element_name, index);
	return child_element_count;
}

UINT XMLParser::ThenCountAttributes()
{
	UINT attribute_count = 0;
	pReader->GetAttributeCount(&attribute_count);
	return attribute_count;
}

UINT XMLParser::ThenCountAttributeOccurances(LPCWSTR attribute_name, BOOL multiple)
{
	int attribute_count = this->ThenCountAttributes();
	UINT count = 0;
	int length = lstrlenW(attribute_name);
	WCHAR string_part[10] = {};
	LPCWSTR attribute = NULL;

	if (attribute_count)
	{
		UINT index = 1;
		while (index < attribute_count + 1)
		{
			if (index == 1)
				pReader->MoveToFirstAttribute();
			else pReader->MoveToNextAttribute();

			pReader->GetLocalName(&attribute, NULL);

			if (!multiple)
			{
				if (lstrcmpiW(attribute, attribute_name) == 0) ++count;
			}
			else if (multiple)
			{
				for (int x = 0; x < length; x++)
					string_part[x] = attribute[x];

				if (lstrcmpiW(string_part, attribute_name) == 0) ++count;
			}

			index++;
		}
	}

	pReader->MoveToElement();
	return count;
}

LPCWSTR XMLParser::ThenGetAttributeValue(LPCWSTR attribute_name, BOOL multiple, int position)
{
	int result = 0;
	int attribute_count = this->ThenCountAttributes();
	UINT count = 0;
	int length = lstrlenW(attribute_name);
	WCHAR assembled_attribute[5] = {};

	if (multiple)
	{
		for (count = 0; count < length; count++)
			assembled_attribute[count] = attribute_name[count];
		WCHAR temp[2] = {};
		_itow_s(position, temp, 10);
		assembled_attribute[count] = temp[0];
	}

	if (attribute_count)
	{
		UINT index = 1;
		while (index < attribute_count + 1)
		{
			if (index == 1)
				pReader->MoveToFirstAttribute();
			else pReader->MoveToNextAttribute();

			pReader->GetLocalName(&this->pwszLocalName, NULL);

			if (!multiple)
				result = lstrcmpiW(this->pwszLocalName, attribute_name);
			else if (multiple)
				result = lstrcmpiW(this->pwszLocalName, assembled_attribute);

			if (result == NULL)
			{
				pReader->GetValue(&this->pwszValue, &count);
				goto Finish;
			}

			index++;
		}

	}

Finish:
	if (!multiple)
	{
		result = lstrcmpiW(this->pwszLocalName, attribute_name);
		if (result != NULL)
		{
			pReader->MoveToElement();
			return NULL;
		}
	}

	pReader->MoveToElement();
	return this->pwszValue;
}




