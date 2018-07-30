//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "ToggleWin.h"

ToggleWin* axToggleWin = NULL;

LRESULT CALLBACK ToggleProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_GETMINMAXINFO:
	{
		PMINMAXINFO pmmi = (PMINMAXINFO)lParam;
		pmmi->ptMinTrackSize.x = 10;
		pmmi->ptMinTrackSize.y = 10;

		return 0;
	}

	case WM_CLOSE:
	{
		if (axToggleWin)
			axToggleWin->hideWindow();

		return 0;
	}
	case WM_NCRBUTTONUP:
	{
		axToggleWin->popup_menu->showPopupMenuRight();

		return  0;
	}
	case WM_NCLBUTTONUP:
	{
		return 0;
	}
	case WM_NCHITTEST:
	{
		LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);

		if (hit == HTCLIENT)
			hit = HTCAPTION;
		
		return hit;
	}

	case WM_SYSCOMMAND:
	{
		switch (wParam){

		case SC_MINIMIZE:
		{
			
		}break;

		default:break;
		}
	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case POPUPMENU_ITEM_CLOSE_ID:
		{
			axToggleWin->hideWindow();
		}break;
		case POPUPMENU_ITEM_ALWAYSONTOP_ID:
		{
			if (axToggleWin->popup_menu->isMarkedChecked(POPUPMENU_ITEM_ALWAYSONTOP_ID))
			{
				axToggleWin->disableAlwaysOnTop();
				axToggleWin->popup_menu->markUnCheckMenuItem(POPUPMENU_ITEM_ALWAYSONTOP_ID);
			}
			else
			{
				axToggleWin->enableAlwaysOnTop();
				axToggleWin->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_ALWAYSONTOP_ID);
			}
		}break;
		case POPUPMENU_ITEM_HIDEBORDER_ID:
		{
			if (axToggleWin->popup_menu->isMarkedChecked(POPUPMENU_ITEM_HIDEBORDER_ID))
			{
				axToggleWin->showWindowBorder();
				axToggleWin->popup_menu->markUnCheckMenuItem(POPUPMENU_ITEM_HIDEBORDER_ID);
			}
			else
			{
				axToggleWin->hideWindowBorder();
				axToggleWin->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_HIDEBORDER_ID);
			}
		}break;

		default:break;
		}
	}break;

	case WM_PAINT:
	{
		HFONT hFont = CreateFont(15, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, NULL);

		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, hFont);

		//TextOutW(hdc, DEFAULT_ITEMSMARGIN, 15, L"Toggle Monitor", _countof(L"Toggle Monitor"));
	
		EndPaint(hwnd, &ps);
		
	}break;
	default:break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}