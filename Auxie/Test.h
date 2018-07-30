//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include "Winwin.h"
#include "PushButton.h"
#include "BMDSwitcherAPI_h.h"
#include "Common.h"
#include "AuxieXMLElements.h"
#include "AuxieATEMInputNames.h"
#include "ATEMError.h"
#include "Util.h"
#include "AuxieATEM.h"

#define TEST_BUTTON_ID 2314

LRESULT CALLBACK TestProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Test{

public:
	WinWin* test_win;

	~Test();
	Test();
	Test(HINSTANCE instance_handle);

};