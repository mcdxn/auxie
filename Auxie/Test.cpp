//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "Test.h"

Test::~Test()
{
	if (this->test_win)
		delete this->test_win;
}

Test::Test()
{
	this->test_win = nullptr;
}

Test::Test(HINSTANCE instance_handle)
{
	this->test_win = new WinWin;
	this->test_win->initWindowClass(instance_handle, L"TEST BED", TestProc);
	this->test_win->createWindow(FALSE, 0, NULL, instance_handle, L"TEST BED", L"TEST BED", TestProc, WS_CAPTION|WS_VISIBLE, 0, 80, 80);
	this->test_win->setWindowSize(80, 80);

	PushButton* button = new PushButton(TEST_BUTTON_ID, instance_handle, this->test_win->getWindowHandle());
	button->setPosition(10, 10);
	button->setLabel(L"KEY");
}