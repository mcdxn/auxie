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
#include "AuxieATEMInputNames.h"
#include "AuxieXMLElements.h"
#include "XMLParser.h"

#define ELEMENT_BUTTON L"button"
#define ELEMENT_AUXIE L"auxie"
#define AT_ID L"id"
#define AT_NAME L"name"
#define AT_DEST L"d"
#define AT_SRC L"t"


class AuxieXMLParser : private XMLParser {
private:

	static PBTNELEMENT button;

	BOOL isKey(LPCWSTR name);
	void initButtons();
	AuxieXMLParser();

public:

	~AuxieXMLParser();
	AuxieXMLParser(LPCWSTR filename);
	
	int getCountButtons();
	static void destroyButtons();
	static PBTNELEMENTLIST getButtons();
};