//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//


#include "LiveButtonsWin.h"

LiveButtons::~LiveButtons()
{
}

LiveButtons::LiveButtons()
{
	this->popup_menu = NULL;
}

int LiveButtons::count = 0;

LiveButtons::LiveButtons(HINSTANCE instance_handle)
{
	LiveButtons::count += 1;
	this->initWindow(instance_handle,(_bstr_t(LIVEBUTTONS_CLASSNAME) + _bstr_t(LiveButtons::count)), LIVEBUTTONS_TITLENAME, LiveButtonsProc, LIVEBUTTONS_WINDOWSTYLE, 0);
	this->popup_menu = new PopupMenu(this->getWindowHandle());

	popup_menu->addItemString(POPUPMENU_ITEM_ALWAYSONTOP_ID, POPUPMENU_ITEM_ALWAYSONTOP);
	popup_menu->addItemString(POPUPMENU_ITEM_HIDEBORDER_ID, POPUPMENU_ITEM_HIDEBORDER);
	popup_menu->addItemString(POPUPMENU_ITEM_LOCK_ID, POPUPMENU_ITEM_LOCK);
	popup_menu->addItemSeperator();
	popup_menu->addItemString(POPUPMENU_ITEM_CLOSE_ID, POPUPMENU_ITEM_CLOSE);
}

void LiveButtons::setMainWindowHandle(HWND hwnd)
{
	this->main_window_handle = hwnd;
}

HWND LiveButtons::getMainWindowHandle()
{
	return this->main_window_handle;
}

void LiveButtons::refresh()
{

}

void LiveButtons::prepareLiveButtons(PBTNELEMENTLIST button_list)
{
	this->button_list = button_list;

	int buttons_count = 0;
	PBTNELEMENT head = NULL;
	PBTNELEMENT buttons = button_list;

	while (buttons != NULL)
	{
		if (buttons->prev == NULL)
		{
			head = buttons;
			break;
		}
		else
			buttons = buttons->prev;
	}

	buttons = head;
	do
	{
		++buttons_count;
		buttons = buttons->next;

	} while (buttons != NULL);

	HINSTANCE window_instance = (HINSTANCE) GetWindowLongPtr(this->getWindowHandle(), GWL_HINSTANCE);
	
	buttons = head;
	if (buttons->id == 0)
	{
		this->push_buttons = new PushButtonList(buttons->id, L":)", window_instance, this->getWindowHandle(), PUSHBUTTONLIST_MARGIN, PUSHBUTTONLIST_MARGIN, BUTT_DEFAULT_WIDTH, BUTT_DEFAULT_HEIGHT);
		this->push_buttons->disableButton(buttons->id);
		return;
	}

	this->push_buttons = new PushButtonList(buttons->id, buttons->name, window_instance, this->getWindowHandle(), PUSHBUTTONLIST_MARGIN, PUSHBUTTONLIST_MARGIN, BUTT_DEFAULT_WIDTH, BUTT_DEFAULT_HEIGHT);
	buttons = buttons->next;

	LPCWSTR label = NULL;

	for (int i = 1; i < buttons_count; i++)
	{
		WCHAR buffer[5] = {};
		_itow_s((i + 1), buffer, 10);
		
		if (lstrcmp(buttons->name,L"") != NULL)
		    label = buttons->name;
		else
			label = (LPCWSTR)(_bstr_t(L"BTN") + buffer);
	
		this->push_buttons->addButton(buttons->id, label, window_instance, this->getWindowHandle());
		buttons = buttons->next;

	}

}