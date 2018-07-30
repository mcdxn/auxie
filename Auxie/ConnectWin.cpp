//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "ConnectWin.h"

ConnectWin::~ConnectWin()
{
}

ConnectWin::ConnectWin()
{
	this->connect_window_win = NULL;
	this->button_win = NULL;
	this->ip_field_win = NULL;
	this->popup_menu = NULL;
}

ConnectWin::ConnectWin(HINSTANCE instance_handle)
{
	if (this->connect_window_win == NULL)
	{
		this->connect_window_win = new WinWin;
		this->connect_window_win->initWindowClass(instance_handle, CONNECTWIN_WINDOW_CLASSNAME, ConnectProc);
		this->connect_window_win->createWindow(FALSE, 0, 0, instance_handle, CONNECTWIN_WINDOW_CLASSNAME, SOFTWARE_VERSION_NAME, ConnectProc, CONNECTWIN_WINDOW_STYLE, 0, CONNECTWIN_WINDOW_WIDTH, CONNECTWIN_WINDOW_HEIGHT);
		this->connect_window_win->setBackgroundColor(255, 255, 255);
	}

	HWND parent_handle = this->connect_window_win->getWindowHandle();

	if (this->ip_field_win == NULL)
	{
		
		this->ip_field_win = new WinWin;		
		this->ip_field_win->attachCommonControl(ID_INPUTIPADRESS, WC_IPADDRESS, L"", instance_handle, parent_handle,
			INPUTIPADRESS_STYLE, INPUTIPADRESS_XPOS, INPUTIPADRESS_YPOS, INPUTIPADRESS_WIDTH, INPUTIPADRESS_HEIGHT);
		this->setIPAddressFieldContents();
	}

	if (this->button_win == NULL)
	{
		this->button_win = new WinWin;
		this->button_win->attachCommonControl(ID_INPUTBUTTONCONNECT, L"BUTTON", L"Connect", instance_handle, parent_handle,
			INPUTBUTTON_STYLE, INPUTBUTTON_XPOS, INPUTBUTTON_YPOS, INPUTBUTTON_WIDTH, INPUTBUTTON_HEIGHT);
	}

	if (this->popup_menu == NULL)
	{
		this->popup_menu = new PopupMenu(parent_handle);

		this->popup_menu->addItemString(POPUPMENU_ITEM_ABOUT_ID, POPUPMENU_ITEM_ABOUT);
		this->popup_menu->addItemSeperator();
		this->popup_menu->addItemString(POPUPMENU_ITEM_CONNECT_ID, POPUPMENU_ITEM_CONNECT);
		this->popup_menu->addItemString(POPUPMENU_ITEM_LIVEBUTTONS_ID, POPUPMENU_ITEM_LIVEBUTTONS);
		this->popup_menu->addItemString(POPUPMENU_ITEM_TOGGLEMONITOR_ID, POPUPMENU_ITEM_TOGGLEMONITOR);
		this->popup_menu->addItemSeperator();
		this->popup_menu->addItemString(POPUPMENU_ITEM_EXIT_ID, POPUPMENU_ITEM_EXIT);
	}
}

void ConnectWin::clearIPAddressField()
{
	// Clear address field
	SendMessage(this->ip_field_win->getWindowHandle(), IPM_CLEARADDRESS, 0, 0);
}

void ConnectWin::setIPAddressFieldContents()
{
	AuxieRegistry* axRegistry = new AuxieRegistry();
	axRegistry->initIP();

	SendMessage(this->ip_field_win->getWindowHandle(), IPM_SETADDRESS, 0, axRegistry->getIP_LPARAM());
	SAFEDELETEOBJECT(axRegistry);
}

HWND ConnectWin::getWindowHandle()
{
	return this->connect_window_win->getWindowHandle();
}

PopupMenu* ConnectWin::getPopupMenu() const
{
	return this->popup_menu;
}
WinWin* ConnectWin::getIPAddressField() const
{
	return this->ip_field_win;
}
WinWin* ConnectWin::getConnectButton() const
{
	return this->button_win;
}

COLORREF ConnectWin::getBackgroundColor()
{
	return this->connect_window_win->getBackgroundColor();
}

HBRUSH  ConnectWin::getBackgroundColorBrush()
{
	return this->connect_window_win->getBackgroundColorBrush();
}

_bstr_t* ConnectWin::getIPAddressFieldContents()
{
	register DWORD ip_address = 0;
	SendMessage(this->ip_field_win->getWindowHandle(), IPM_GETADDRESS, 0, (LPARAM)(DWORD)&ip_address);

	if (ip_address == 0)
		return nullptr;

	char buffer1[20] = {};
	char buffer2[20] = {};
	char buffer3[20] = {};
	char buffer4[20] = {};
	int buffer_size = sizeof(buffer1);

	_i64toa_s(FIRST_IPADDRESS(ip_address), buffer1, buffer_size, 10);
	_i64toa_s(SECOND_IPADDRESS(ip_address), buffer2, buffer_size, 10);
	_i64toa_s(THIRD_IPADDRESS(ip_address), buffer3, buffer_size, 10);
	_i64toa_s(FOURTH_IPADDRESS(ip_address), buffer4, buffer_size, 10);


	_bstr_t* ip_address_string = new _bstr_t(_bstr_t(buffer1) + TEXT(".") + _bstr_t(buffer2) + TEXT(".") + _bstr_t(buffer3) + TEXT(".") + _bstr_t(buffer4));

	return ip_address_string;
}

void ConnectWin::enableIPAddressField()
{
	this->ip_field_win->enableWindow();
}
void ConnectWin::disableIPAddressField()
{
	this->ip_field_win->disableWindow();
}

void ConnectWin::disableConnectButton()
{
	this->button_win->disableWindow();
}

void ConnectWin::enableConnectButton()
{
	this->button_win->enableWindow();
}

void ConnectWin::updateConnectionStatusDisplay()
{
	InvalidateRect(this->connect_window_win->getWindowHandle(), NULL, FALSE);
}
