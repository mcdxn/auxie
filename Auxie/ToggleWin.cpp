//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "ToggleWin.h"


ToggleWin::~ToggleWin()
{
}

ToggleWin::ToggleWin()
{
	this->popup_menu = NULL;
}

int ToggleWin::window_count = 0;

ToggleWin::ToggleWin(HINSTANCE instance_handle)
{
	++ToggleWin::window_count;

	this->initWindow(instance_handle, (LPCWSTR)_bstr_t(TOGGLEWIN_CLASSNAME) + ToggleWin::window_count, TOGGLEWIN_TITLENAME, ToggleProc, TOGGLEWIN_WINDOWSTYLE, 0);

	this->group = new Group(4, L"BTN1", instance_handle, this->getWindowHandle(), 10, 10, 200, 50);
	this->group->setSize(300, 100);
	this->setWindowSize(320, 130);
	this->popup_menu = new PopupMenu(this->getWindowHandle());

	popup_menu->addItemString(POPUPMENU_ITEM_ALWAYSONTOP_ID, POPUPMENU_ITEM_ALWAYSONTOP);
	popup_menu->addItemString(POPUPMENU_ITEM_HIDEBORDER_ID, POPUPMENU_ITEM_HIDEBORDER);
	popup_menu->addItemSeperator();
	popup_menu->addItemString(POPUPMENU_ITEM_CLOSE_ID, POPUPMENU_ITEM_CLOSE);
}


	
