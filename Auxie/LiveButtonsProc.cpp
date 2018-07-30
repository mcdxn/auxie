//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//


#include "LiveButtonsWin.h"

LiveButtons* axLiveButtons = NULL;
BOOL Lock_LiveButtonsWin = FALSE;

LRESULT CALLBACK LiveButtonsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps = {};
	static HDC hdc = NULL;

	switch (uMsg)
	{
	case BUTT_MESSAGE:
	{
		if (wParam == DELBUTTON)
			axLiveButtons->push_buttons->delButton(lParam);

		return 0;
	}
	case WM_CLOSE:
	{
		if (axLiveButtons)
			axLiveButtons->hideWindow();

		return 0;
	}

	case WM_GETMINMAXINFO:
	{
		PMINMAXINFO pmmi = (PMINMAXINFO)lParam;
		pmmi->ptMinTrackSize.x = 10;
		pmmi->ptMinTrackSize.y = 10;

		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (!Lock_LiveButtonsWin)
		{
			if (wParam == MK_LBUTTON)
				SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}

		return 0;
	}

	case WM_RBUTTONUP:
	{
		axLiveButtons->popup_menu->showPopupMenuRight();
		return  0;
	}

	/*
	case WM_NCRBUTTONUP:
	{
		axLiveButtons->popup_menu->showPopupMenuRight();

		return  0;
	}

	case WM_NCHITTEST:
	{
		LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);

		if (hit == HTCLIENT)
			hit = HTCAPTION;

		return hit;
	}
	*/

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
			axLiveButtons->hideWindow();
		}break;

		case POPUPMENU_ITEM_LOCK_ID:
		{
			if (axLiveButtons->popup_menu->isMarkedChecked(POPUPMENU_ITEM_LOCK_ID))
			{
				Lock_LiveButtonsWin = FALSE;
				axLiveButtons->popup_menu->markUnCheckMenuItem(POPUPMENU_ITEM_LOCK_ID);
			}
			else
			{
				Lock_LiveButtonsWin = TRUE;
				axLiveButtons->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_LOCK_ID);
			}
		}break;

		case POPUPMENU_ITEM_ALWAYSONTOP_ID:
		{
			if (axLiveButtons->popup_menu->isMarkedChecked(POPUPMENU_ITEM_ALWAYSONTOP_ID))
			{
				axLiveButtons->disableAlwaysOnTop();
				axLiveButtons->popup_menu->markUnCheckMenuItem(POPUPMENU_ITEM_ALWAYSONTOP_ID);
			}
			else
			{
				axLiveButtons->enableAlwaysOnTop();
				axLiveButtons->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_ALWAYSONTOP_ID);
			}
		}break;
		case POPUPMENU_ITEM_HIDEBORDER_ID:
		{
			if (axLiveButtons->popup_menu->isMarkedChecked(POPUPMENU_ITEM_HIDEBORDER_ID))
			{
				axLiveButtons->showWindowBorder();
				axLiveButtons->popup_menu->markUnCheckMenuItem(POPUPMENU_ITEM_HIDEBORDER_ID);
			}
			else
			{
				axLiveButtons->hideWindowBorder();
				axLiveButtons->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_HIDEBORDER_ID);
			}
		}break;

		default:break;
		}
		
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
		{
			if (LOWORD(wParam) >= POPUPMENU_ITEM_START_ID && LOWORD(wParam) <= POPUPMENU_ITEM_END_ID)
				break;
			else
			{
				SendNotifyMessage(axLiveButtons->getMainWindowHandle(), ATEM_PERFORM_SWITCH, ATEM_SWITCH_FROM_LIVE_BUTTON, LOWORD(wParam));
			}



		}break;

		default:break;
		}
		
		return 0;
	}

	default:break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}