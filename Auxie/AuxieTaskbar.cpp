//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "AuxieTaskbar.h"

#define TRAYICON_ICON_GUID_NAME L"iconguid"

AuxieTaskbar::AuxieTaskbar()
{
	this->hwnd = NULL;
}

AuxieTaskbar::AuxieTaskbar(HWND window_handle)
{
	this->hwnd = window_handle;
	this->InitTrayIcon(this->hwnd);
}

AuxieTaskbar::~AuxieTaskbar()
{
	this->DestroyTrayIcon();
}

void AuxieTaskbar::InitTrayIcon(HWND window_handle)
{
	this->is_win8 = IsWindows8OrGreater();

	if (!this->hwnd)
	{
		this->hwnd = window_handle;
		this->AddTrayIcon();
	}
	else
		this->AddTrayIcon();
}

void AuxieTaskbar::DestroyTrayIcon()
{
	this->RemoveTrayIcon();
}

void AuxieTaskbar::AddTrayIcon()
{
	Registry* reg = new Registry();
	NOTIFYICONDATA nid = {};

	reg->openCurrentUser(AUXIE_REG_KEY);

	if (!reg->isValueExists(TRAYICON_ICON_GUID_NAME))
	{
		HRESULT result = CoCreateGuid(&this->icon_guid);
		if (result == S_OK)
		{
			nid.guidItem = this->icon_guid;
			reg->setBinaryValue(TRAYICON_ICON_GUID_NAME, (LPVOID)&this->icon_guid, sizeof(GUID));
		}
	}
	else
	{
		reg->getBinaryValue(TRAYICON_ICON_GUID_NAME, (LPVOID)&this->icon_guid, sizeof(GUID));
	}


	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uVersion = NOTIFYICON_VERSION_4;
	nid.hWnd = this->hwnd;
	nid.uFlags = NIIF_USER | NIF_TIP | NIF_SHOWTIP| NIF_ICON | NIF_GUID | NIF_MESSAGE | NIF_STATE;
	nid.uID = ID_TRAYICON;
	nid.uCallbackMessage = WM_TRAYICON_MESSAGE;
	
	if (FAILED(StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), ConnectionStatusMessage[NOTCONNECTED]))) ALERT("TrayIcon: StringCchCopy");

	nid.guidItem = this->icon_guid;
	HMODULE module_handle = (HMODULE)GetWindowLong(this->hwnd, GWL_HINSTANCE);
	HICON icon = LoadIcon(module_handle, MAKEINTRESOURCE(IDI_ICON2));
	if (icon)
	nid.hIcon = icon;
	else ALERT("Can't load icon");

	START:
	if (!Shell_NotifyIcon(NIM_ADD, &nid))
	{
		GUID new_icon_guid;

		HRESULT result = CoCreateGuid(&new_icon_guid);
		if (result == S_OK)
		{
			nid.guidItem = this->icon_guid = new_icon_guid;
			reg->setBinaryValue(TRAYICON_ICON_GUID_NAME, (LPVOID)&this->icon_guid, sizeof(GUID));
			goto START;
		}
	}

	if (!Shell_NotifyIcon(NIM_SETVERSION, &nid))
		ALERT("TrayIcon: SetVersion");

	delete reg;
}

void AuxieTaskbar::RemoveTrayIcon()
{
	NOTIFYICONDATA nid = {};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = this->hwnd;
	nid.uFlags = NIF_GUID;
	nid.uID = ID_TRAYICON;
	nid.guidItem = this->icon_guid;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void AuxieTaskbar::UpdateTrayIconTooltip(LPCWCHAR message)
{
	NOTIFYICONDATA nid = {};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = this->hwnd;
	nid.uFlags = NIF_TIP|NIF_SHOWTIP|NIF_GUID;
	nid.uID = ID_TRAYICON;
	nid.guidItem = this->icon_guid;
	if(FAILED(StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), message))) ALERT("Update TrayIcon FAILED");
	Shell_NotifyIcon(NIM_MODIFY, &nid);

}

void AuxieTaskbar::MinimizeTrayIcon()
{
	if (!IsIconic(this->hwnd))
	{
		LONG new_window_style = WS_EX_TOOLWINDOW;
		
		if (this->is_win8 != TRUE)
			AnimateWindow(this->hwnd, 200, AW_HIDE|AW_CENTER); // Glitchy when run in Windows 8
		
		ShowWindow(this->hwnd, SW_MINIMIZE);
		ShowWindow(this->hwnd, SW_HIDE);
		SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, new_window_style);
	}
}

void AuxieTaskbar::MaximizeTrayIcon()
{
	if (IsIconic(this->hwnd))
	{
		SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, 0);
		ShowWindow(this->hwnd, SW_HIDE);
		ShowWindow(this->hwnd, SW_SHOWNORMAL);
		SetForegroundWindow(this->hwnd);
		//OpenIcon(this->hwnd);
	}
	else this->MinimizeTrayIcon();
}

