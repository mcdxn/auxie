//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <comutil.h>
#include <CommCtrl.h>
#include <winerror.h>
#include <WinUser.h>
#include <xmllite.h>
#include <ObjIdl.h>
#include <Shlwapi.h>
#include <atlcomcli.h>
#include <Strsafe.h>
#include "resource.h"
#include "Common.h"
#include "BMDSwitcherAPI_h.h"
#include "AuxieXMLElements.h"
#include "AuxieATEMInputNames.h"
#include "ATEMError.h"
#include "Util.h"
#include "Registry.h"
#include "Winwin.h"
#include "PopupMenu.h"
#include "PushButton.h"
#include "PushButtonList.h"
#include "AuxieATEM.h"
#include "AuxieMem.h"
#include "AuxieXMLParser.h"
#include "AuxieTaskbar.h"
#include "AuxieRegistry.h"
#include "Group.h"
#include "ToggleWin.h"
#include "LiveButtonsWin.h"
#include "Test.h"
#include "FileMonitor.h"

#define CONNECTWIN_WINDOW_CLASSNAME L"Auxie"
#define CONNECTWIN_WINDOW_WIDTH 250
#define CONNECTWIN_WINDOW_HEIGHT 150
#define CONNECTWIN_WINDOW_STYLE WS_VISIBLE|WS_SYSMENU | WS_CAPTION

#define CONNECTWIN_DEFAULT_ITEMSMARGIN 5

#define ID_INPUTIPADRESS 1100
#define INPUTIPADRESS_XPOS CONNECTWIN_DEFAULT_ITEMSMARGIN
#define INPUTIPADRESS_YPOS 45
#define INPUTIPADRESS_WIDTH 150
#define INPUTIPADRESS_HEIGHT 20
#define INPUTIPADRESS_STYLE WS_CHILD | WS_VISIBLE | WS_TABSTOP

#define ID_INPUTBUTTONCONNECT 1200
#define INPUTBUTTON_XPOS 150 + CONNECTWIN_DEFAULT_ITEMSMARGIN + 10
#define INPUTBUTTON_YPOS INPUTIPADRESS_YPOS
#define INPUTBUTTON_WIDTH 64
#define INPUTBUTTON_HEIGHT 20
#define INPUTBUTTON_STYLE WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT

LRESULT CALLBACK ConnectProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class ConnectWin
{
protected:

	WinWin* connect_window_win;
	WinWin* ip_field_win;
	WinWin* button_win;
	PopupMenu* popup_menu;
	ConnectWin();
public:
	~ConnectWin();
	ConnectWin(HINSTANCE instance_handle);

	void updateConnectionStatusDisplay();
	
	void clearIPAddressField();
	
	void setIPAddressFieldContents();
	_bstr_t* getIPAddressFieldContents();

	HWND getWindowHandle();
	PopupMenu* getPopupMenu() const;
	WinWin* getIPAddressField() const;
	WinWin* getConnectButton() const;
	COLORREF getBackgroundColor();
	HBRUSH getBackgroundColorBrush();

	void enableIPAddressField();
	void disableIPAddressField();

	void enableConnectButton();
	void disableConnectButton();

	void showConnectionStatusDisplay();
};