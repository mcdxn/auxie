//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//


#include "PopupMenu.h"


PopupMenu::~PopupMenu()
{
	if (this->hmenu)
		DestroyMenu(this->hmenu);
}
PopupMenu::PopupMenu()
{
	this->current_checked_item = POPUPMENU_ITEM_NULL;
	this->initPopupMenu(NULL);
}

PopupMenu::PopupMenu(HWND window_handle)
{
	this->initPopupMenu(window_handle);
}

void PopupMenu::initPopupMenu(HWND window_handle)
{
	if (window_handle)
	{
		if (this->hwnd == NULL)
			this->hwnd = window_handle;
	}
	else
	{
		this->hwnd = GetActiveWindow();
	}

	if (this->hmenu == NULL)
		this->hmenu = CreatePopupMenu();
}

void PopupMenu::showPopupMenu()
{
	POINT p;
	GetCursorPos(&p);
	SetForegroundWindow(this->hwnd);
	if (TrackPopupMenuEx(this->hmenu, TPM_LEFTBUTTON |
		TPM_CENTERALIGN | TPM_BOTTOMALIGN | TPM_RECURSE, p.x, p.y, this->hwnd, NULL) == 0)
		ALERT("TrackPopupMenu");
}

void PopupMenu::showPopupMenuLeft()
{
	POINT p;
	GetCursorPos(&p);
	SetForegroundWindow(this->hwnd);
	if (TrackPopupMenuEx(this->hmenu, TPM_LEFTBUTTON |
		TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_RECURSE, p.x, p.y, this->hwnd, NULL) == 0)
		ALERT("TrackPopupMenu");
}

void PopupMenu::showPopupMenuRight()
{
	POINT p;
	GetCursorPos(&p);
	SetForegroundWindow(this->hwnd);
	if (TrackPopupMenuEx(this->hmenu, TPM_LEFTBUTTON |
		TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RECURSE, p.x, p.y, this->hwnd, NULL) == 0)
		ALERT("TrackPopupMenu");
}

void PopupMenu::addItemString(POPUPMENUITEMID item_id, LPWSTR item_string)
{
	MENUITEMINFO item_info = {};
	item_info.cbSize = sizeof(MENUITEMINFO);
	item_info.fMask = MIIM_CHECKMARKS | MIIM_STATE | MIIM_FTYPE | MIIM_STRING| MIIM_ID;
	item_info.fType = MFT_STRING;
	item_info.fState = MFS_UNCHECKED|MFS_ENABLED;
	item_info.wID = item_id;
	item_info.hbmpChecked = NULL;
	item_info.hbmpUnchecked = NULL;
	item_info.dwTypeData = item_string;
	item_info.cch = lstrlen(item_string)+1;

	InsertMenuItem(this->hmenu, item_id, FALSE, &item_info);

	// ANOTHER WAY OF ADDING MENU ITEMS
	// if (AppendMenuW(this->hmenu, MF_STRING | MF_ENABLED, item_id, item_string) == 0)
	//	 ALERT("InsertMenuItem");

	DrawMenuBar(this->hwnd);
}

void PopupMenu::addItemSubMenu(HMENU submenu_handle)
{

	DrawMenuBar(this->hwnd);
}
void PopupMenu::addItemSeperator()
{
	if (AppendMenuW(this->hmenu, MF_SEPARATOR, NULL, NULL) == 0)
		ALERT("InsertMenuItem");

	DrawMenuBar(this->hwnd);
}

void PopupMenu::markCheckMenuItem(POPUPMENUITEMID item_id)
{
	MENUITEMINFO item_info = {};
	item_info.cbSize = sizeof(MENUITEMINFO);
	item_info.fMask = MIIM_STATE;
	item_info.fState = MFS_CHECKED;

	if (SetMenuItemInfo(this->hmenu, item_id, FALSE, &item_info) == 0)
	{
		DWORD error = GetLastError();
		ALERT("markCheckMenuItem");
	}
	else
		DrawMenuBar(this->hwnd);
}

void PopupMenu::markUnCheckMenuItem(POPUPMENUITEMID item_id)
{
	MENUITEMINFO item_info = {};
	item_info.cbSize = sizeof(MENUITEMINFO);
	item_info.fMask = MIIM_STATE;
	item_info.fState = MFS_UNCHECKED;

	if (SetMenuItemInfo(this->hmenu, item_id, FALSE, &item_info) == 0)
	{
		DWORD error = GetLastError();
		ALERT("markUnCheckMenuItem");
	}
	else
		DrawMenuBar(this->hwnd);
}

BOOL PopupMenu::isMarkedChecked(POPUPMENUITEMID item_id)
{
	MENUITEMINFO item_info = {};
	item_info.cbSize = sizeof(MENUITEMINFO);
	item_info.fMask = MIIM_STATE;

	GetMenuItemInfo(this->hmenu, item_id, FALSE, &item_info);

	if ((item_info.fState&MFS_CHECKED) == MFS_CHECKED)
		return TRUE;
	else
		return FALSE;
}