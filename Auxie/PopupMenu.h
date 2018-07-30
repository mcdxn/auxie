//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include "Common.h"

#define POPUPMENU_ITEM_EXIT L"Exit"
#define POPUPMENU_ITEM_CLOSE L"Close"
#define POPUPMENU_ITEM_ABOUT L"About"
#define POPUPMENU_ITEM_CONNECT L"Connect"
#define POPUPMENU_ITEM_TOGGLEMONITOR L"Toggle Monitor"
#define POPUPMENU_ITEM_LIVEBUTTONS L"Live Buttons"
#define POPUPMENU_ITEM_ALWAYSONTOP L"Always OnTop"
#define POPUPMENU_ITEM_HIDEBORDER L"Hide Border"
#define POPUPMENU_ITEM_LOCK L"Lock"

typedef enum PopupMenuId
{
	POPUPMENU_ITEM_NULL = 0,
	POPUPMENU_ITEM_START_ID = 1909,
	POPUPMENU_ITEM_EXIT_ID = 1910,
	POPUPMENU_ITEM_CLOSE_ID,
	POPUPMENU_ITEM_ABOUT_ID,
	POPUPMENU_ITEM_CONNECT_ID,
	POPUPMENU_ITEM_TOGGLEMONITOR_ID,
	POPUPMENU_ITEM_LIVEBUTTONS_ID,
	POPUPMENU_ITEM_ALWAYSONTOP_ID,
	POPUPMENU_ITEM_HIDEBORDER_ID,
	POPUPMENU_ITEM_LOCK_ID,
	POPUPMENU_ITEM_END_ID
}POPUPMENUITEMID;

class PopupMenu {

private:
	HMENU hmenu;
	HWND hwnd;
	POPUPMENUITEMID current_checked_item;

public:
	~PopupMenu();
	PopupMenu();
	PopupMenu(HWND window_handle);
	void initPopupMenu(HWND window_handle);
	void showPopupMenu();
	void showPopupMenuRight();
	void showPopupMenuLeft();
	void addItemSeperator();
	void addItemString(POPUPMENUITEMID item_id, LPWSTR item_string);
	void addItemSubMenu(HMENU submenu_handle);

	BOOL isMarkedChecked(POPUPMENUITEMID item_id);
	void markCheckMenuItem(POPUPMENUITEMID item_id);
	void markUnCheckMenuItem(POPUPMENUITEMID item_id);
};