//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#include "Test.h"

extern AuxieATEM* axAtem;

LRESULT CALLBACK TestProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_GETMINMAXINFO:
	{
		PMINMAXINFO pmmi = (PMINMAXINFO)lParam;
		pmmi->ptMinTrackSize.x = 10;
		pmmi->ptMinTrackSize.y = 10;

		return 0;
	}

	case  WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
		{
			if (axAtem->GetConnectionStatus() == CONNECTED)
			{
			}
		}

		default:break;
		}

		return 0;
	}
	default:break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}