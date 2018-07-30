//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <comutil.h>
#include "Common.h"
#include "Util.h"
#include "BMDSwitcherAPI_h.h"
#include "AuxieXMLElements.h"
#include "AuxieATEMInputNames.h"
#include "ATEMError.h"
#include "AuxieATEM.h"
#include "PopupMenu.h"
#include "PushButton.h"
#include "PushButtonList.h"
#include "Winwin.h"
#include "Group.h"


#define TOGGLEWIN_CLASSNAME L"TOGGLEMONITOR"
#define TOGGLEWIN_TITLENAME L"Toggle Monitor"
#define TOGGLEWIN_WINDOWSTYLE WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX

LRESULT CALLBACK ToggleProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class ToggleWin : public WinWin
{
private:
	static int window_count;
public:
	PopupMenu* popup_menu;
	Group* group;

	~ToggleWin();
	ToggleWin();
	ToggleWin(HINSTANCE instance_handle);

};