//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "Winwin.h"


WinWin::~WinWin()
{
	if (this->hwnd)
		DestroyWindow(this->hwnd);
	if(this->color_background_brush)
		DeleteObject(this->color_background_brush);

}

WinWin::WinWin()
{
	hwnd = 0;
	style = 0;
	color_background_brush = NULL;
	is_parent_ready = FALSE;
	is_win8 = FALSE;
	is_window_border_hidden = FALSE;
	is_window_hidden = FALSE;
	ZeroMemory(&client_rect_size,sizeof(RECT));
	ZeroMemory(&window_rect_size, sizeof(RECT));
}

WinWin::WinWin(HINSTANCE instance_handle, LPCWSTR class_name, LPCWSTR title_name, WNDPROC window_procedure, DWORD window_style = 0, DWORD window_exstyle = 0)
{
	this->initWindow(instance_handle, class_name, title_name, window_procedure, window_style, window_exstyle);
}

void WinWin::initWindow(HINSTANCE instance_handle, LPCWSTR class_name, LPCWSTR title_name, WNDPROC window_procedure, DWORD window_style = 0, DWORD window_exstyle = 0)
{
	this->is_win8 = IsWindows8OrGreater();
	this->is_window_border_hidden = FALSE;
	this->style = window_style;

	this->initWindowClass(instance_handle, class_name, window_procedure);

	this->window_rect_size.right = WINWIN_DEFAULT_WIDTH;
	this->window_rect_size.bottom = WINWIN_DEFAULT_HEIGHT;

	this->client_rect_size = this->window_rect_size;

	AdjustWindowRectEx(&this->window_rect_size, this->style, FALSE, 0);

	this->createWindow(FALSE, NULL, NULL, instance_handle, class_name, title_name, window_procedure, window_style, window_exstyle, this->window_rect_size.right, this->window_rect_size.bottom);

	RECT crect;
	GetClientRect(this->hwnd, &crect);

	this->client_rect_size.right = crect.right;
	this->client_rect_size.bottom = crect.bottom;
}

void WinWin::initWindowClass(HINSTANCE instance_handle, LPCWSTR class_name, WNDPROC window_procedure)
{
	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = window_procedure;
	wcex.cbClsExtra = 20;
	wcex.cbWndExtra = 20;
	wcex.hInstance = instance_handle;
	wcex.hIcon = LoadIcon(instance_handle, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(32, 32, 32));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = class_name;

	if (!RegisterClassEx(&wcex))
		ALERT("Could Not Register Class");
}

void WinWin::createWindow(BOOL is_child, int child_id, HWND parent_window_handle, HINSTANCE instance_handle, LPCWSTR class_name, LPCWSTR title_name, WNDPROC window_procedure, DWORD window_style, DWORD window_exstyle,int width,int height)
{
	if (!is_child)
	{
		this->hwnd = CreateWindowEx(window_exstyle, class_name, title_name, window_style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, 0, instance_handle, NULL);
		if (this->hwnd == NULL)
			ALERT("CreateWindowEx");

		this->is_parent_ready = TRUE;
	}
	else if (is_child)
	{
		this->hwnd = CreateWindowEx(window_exstyle, class_name, title_name, window_style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, parent_window_handle, (HMENU)child_id, instance_handle, NULL);
		if (this->hwnd == NULL)
			ALERT("CreateWindowEx");
	}
}

void WinWin::attachCommonControl(UINT id, LPCWSTR control_class_name, LPCWSTR label, HINSTANCE instance_handle, HWND parent_window_handle, DWORD style, int x_pos, int y_pos, int width, int height)
{
	if (parent_window_handle != NULL)
		this->hwnd = CreateWindowEx(0, control_class_name, label, style, x_pos, y_pos, width, height, parent_window_handle, (HMENU)id, instance_handle, NULL);
	else
		ALERT("attachCommonControl");
}

HWND WinWin::getWindowHandle()
{
	return this->hwnd;
}

POINT WinWin::getWindowPositionInDesktop()
{
	int caption_size = GetSystemMetrics(SM_CYCAPTION);
	int border_size = GetSystemMetrics(SM_CXSIZEFRAME);

	WINDOWPLACEMENT wp = {};
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(this->hwnd, &wp);

	RECT r = {};
	GetWindowRect(this->hwnd, &r);

	POINT position = {};
	position.x = r.left - (border_size * 2);
	position.y = r.top - caption_size - (border_size * 2);

	return position;
}

void WinWin::showWindow()
{
	if (IsIconic(this->hwnd))
	{
		LONG new_window_style = GetWindowLong(this->hwnd, GWL_EXSTYLE);
		SetWindowLongPtr(this->hwnd, new_window_style, 0);
		//SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, 0);
		ShowWindow(this->hwnd, SW_HIDE);
		ShowWindow(this->hwnd, SW_RESTORE);
		SetForegroundWindow(this->hwnd);
	}
	else ShowWindow(this->hwnd, SW_SHOWNORMAL);
}

void WinWin::hideWindow()
{
	if (!IsIconic(this->hwnd))
	{
		LONG new_window_style = WS_EX_TOOLWINDOW; 

		if (this->is_win8 != TRUE)
			AnimateWindow(this->hwnd, 200, AW_HIDE | AW_CENTER); // Glitchy when run in Windows8

		ShowWindow(this->hwnd, SW_MINIMIZE);
		ShowWindow(this->hwnd, SW_HIDE);
		SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, new_window_style);
	} 
	else this->showWindow();
}

void WinWin::enableAlwaysOnTop()
{
	LONG new_window_style = GetWindowLong(this->hwnd, GWL_EXSTYLE);
	
	new_window_style |= WS_EX_TOPMOST;

	SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, new_window_style);
	SetWindowPos(this->hwnd, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE);
}

void WinWin::disableAlwaysOnTop()
{
	LONG new_window_style = GetWindowLong(this->hwnd, GWL_EXSTYLE);

	new_window_style ^= WS_EX_TOPMOST;

	SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, new_window_style);
	SetWindowPos(this->hwnd, HWND_BOTTOM, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE);
}

void WinWin::hideWindowBorder()
{
	if (this->is_window_border_hidden != TRUE)
	{
		this->is_window_border_hidden = TRUE;

		LONG new_window_style = 0;
		LONG new_window_ex_style = WS_EX_TOOLWINDOW;
		
		RECT crect;
		
		GetClientRect(this->hwnd, &crect);
		MapWindowPoints(this->hwnd, HWND_DESKTOP, (LPPOINT)&crect, 2);
		
		ShowWindow(this->hwnd, SW_HIDE);
		
		SetWindowLongPtr(this->hwnd, GWL_STYLE, new_window_style);
		SetWindowPos(this->hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE | SWP_FRAMECHANGED);

		SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, new_window_ex_style);
		SetWindowPos(this->hwnd, HWND_TOP, crect.left, crect.top, this->client_rect_size.right, this->client_rect_size.bottom, SWP_FRAMECHANGED);
		
		ShowWindow(this->hwnd, SW_SHOW);
	}
}

void WinWin::showWindowBorder()
{
	if (this->is_window_border_hidden == TRUE)
	{
		this->is_window_border_hidden = FALSE;

		LONG new_window_style = this->style;
		int caption_size = GetSystemMetrics(SM_CYCAPTION);
		int border_size = GetSystemMetrics(SM_CXSIZEFRAME);
	
		RECT crect;
		GetClientRect(this->hwnd, &crect);
		MapWindowPoints(this->hwnd, HWND_DESKTOP, (LPPOINT)&crect, 2);

		int x = crect.left - (border_size*2);
		int y = crect.top - caption_size - (border_size*2);

		SetWindowLongPtr(this->hwnd, GWL_STYLE, new_window_style);
		SetWindowPos(this->hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE |SWP_FRAMECHANGED);

		SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, 0);
		SetWindowPos(this->hwnd, HWND_TOP, x, y, this->window_rect_size.right, this->window_rect_size.bottom, SWP_FRAMECHANGED);
		
		ShowWindow(this->hwnd, SW_SHOW);
	}
}

void WinWin::setWindowSize(int width, int height)
{
	RECT zero = {0,0,0,0};
	this->window_rect_size = zero;
	this->client_rect_size = zero;

	this->window_rect_size.top =0;
	this->window_rect_size.left = 0;

	this->window_rect_size.right = width;
	this->window_rect_size.bottom = height;

	int caption_size = GetSystemMetrics(SM_CYCAPTION);
	int border_size = 2*GetSystemMetrics(SM_CXSIZEFRAME);

	this->window_rect_size.right += border_size;
	this->window_rect_size.bottom += caption_size + border_size;

	AdjustWindowRect(&this->window_rect_size, WS_CAPTION, FALSE);

	SetWindowPos(this->hwnd, HWND_TOP,0,0, this->window_rect_size.right, this->window_rect_size.bottom, SWP_NOMOVE | SWP_FRAMECHANGED);
	
	RECT crect;
	GetClientRect(this->hwnd, &crect);

	this->client_rect_size.right = crect.right;
	this->client_rect_size.bottom = crect.bottom;
}

void WinWin::setWindowPosition(int x, int y)
{
	SetWindowPos(this->hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
}

void  WinWin::setBackgroundColor(BYTE r, BYTE g, BYTE b)
{
	if (this->color_background_brush == NULL)
		this->color_background_brush = CreateSolidBrush(RGB(r, g, b));
	else
	{
		DeleteObject(this->color_background_brush);
		this->color_background_brush = CreateSolidBrush(RGB(r, g, b));
	}

	this->color_background = RGB(r, g, b);

	HRESULT result = SetClassLongPtr(this->hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)this->color_background_brush);
	RedrawWindow(this->hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOFRAME | RDW_ERASENOW);
}

COLORREF WinWin::getBackgroundColor()
{
	return this->color_background;
}

HBRUSH WinWin::getBackgroundColorBrush()
{
	return this->color_background_brush;
}

void WinWin::enableWindow()
{
	EnableWindow(this->hwnd, TRUE);
}
void WinWin::disableWindow()
{
	EnableWindow(this->hwnd, FALSE);

}