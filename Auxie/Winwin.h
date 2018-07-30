//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <WinUser.h>
#include <VersionHelpers.h>
#include "resource.h"
#include "Common.h"

#define WINWIN_DEFAULT_WIDTH 100
#define WINWIN_DEFAULT_HEIGHT 100

class WinWin {

private:
	HWND hwnd;
	DWORD style;
	COLORREF color_background;
	HBRUSH color_background_brush;

	BOOL is_parent_ready;
	BOOL is_win8;
	BOOL is_window_border_hidden;
	BOOL is_window_hidden;
	RECT client_rect_size;
	RECT window_rect_size;

public:
	~WinWin();
	WinWin();
	WinWin(HINSTANCE instance_handle, LPCWSTR class_name, LPCWSTR title_name, WNDPROC window_procedure, DWORD window_style, DWORD window_exstyle);

	void initWindowClass(HINSTANCE instance_handle, LPCWSTR class_name, WNDPROC window_procedure);
    void initWindow(HINSTANCE instance_handle, LPCWSTR class_name, LPCWSTR title_name, WNDPROC window_procedure, DWORD window_style, DWORD window_exstyle);
	void createWindow(BOOL is_child, int child_id, HWND parent_window_handle, HINSTANCE instance_handle, LPCWSTR class_name, LPCWSTR title_name, WNDPROC window_procedure, DWORD window_style, DWORD window_exstyle, int width, int height);

	void attachCommonControl(UINT button_id, LPCWSTR control_class_name, LPCWSTR label, HINSTANCE instance_handle, HWND parent_window_handle, DWORD style, int x_pos, int y_pos, int width, int height);

	void showWindow();
	void hideWindow();
	void enableAlwaysOnTop();
	void disableAlwaysOnTop();
	void hideWindowBorder();
	void showWindowBorder();

	void enableWindow();
	void disableWindow();

	void setWindowSize(int width, int height);
	void setWindowPosition(int x, int y);

	POINT getWindowPositionInDesktop();
	
	void setBackgroundColor(BYTE r, BYTE g, BYTE b);
	COLORREF getBackgroundColor();
	HBRUSH getBackgroundColorBrush();

	HWND getWindowHandle();
};