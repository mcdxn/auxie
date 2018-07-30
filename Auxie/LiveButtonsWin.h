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
#include "AuxieXMLParser.h"
#include "AuxieATEMInputNames.h"
#include "ATEMError.h"
#include "AuxieATEM.h"
#include "PopupMenu.h"
#include "PushButton.h"
#include "PushButtonList.h"
#include "Winwin.h"

#define LIVEBUTTONS_CLASSNAME L"LIVEBUTTONS"
#define LIVEBUTTONS_TITLENAME L"Live Buttons"
#define LIVEBUTTONS_WINDOWSTYLE WS_CAPTION|WS_SYSMENU

LRESULT CALLBACK LiveButtonsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class LiveButtons : public WinWin, public _Refresh
{
	static int count;
	HWND main_window_handle;
	PBTNELEMENTLIST button_list;

public:
	PopupMenu* popup_menu;
	PushButtonList* push_buttons;

	~LiveButtons();
	LiveButtons();
	LiveButtons(HINSTANCE instance_handle);

	void setMainWindowHandle(HWND hwnd);
	HWND getMainWindowHandle();

	void prepareLiveButtons(PBTNELEMENTLIST button_list);

	void refresh();
};