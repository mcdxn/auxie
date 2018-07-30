//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "Common.h"
#include "PopupMenu.h"
#include "Util.h"

typedef enum _Butt_Message
{
	ADDBUTTON = 912,
	DELBUTTON,
	DISABLEBUTTON,
	ENABLEBUTTON
}BUTT_MSG, *PBUTT_MSG;


#define BUTT_CLASSNAME L"BUTTON"
#define BUTT_STYLE_NORMAL WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_FLAT
#define BUTT_STYLE_ABNORMAL WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_OWNERDRAW

#define BUTT_DEFAULT_LABEL L"BTTN"
#define BUTT_DEFAULT_WIDTH 50
#define BUTT_DEFAULT_HEIGHT 50

#define BUTT_MESSAGE WM_USER+4051

LRESULT CALLBACK PushButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

class PushButton
{
private:
	int id;
	HWND hwnd;

	BOOL is_clicked;

	LPCWSTR button_label;
	COLORREF color_background;
	COLORREF color_clicked;
	COLORREF text_color;

	HFONT font_handle;
	int font_size;

	HBRUSH brush_background;
	HBRUSH brush_clicked;

public:
	~PushButton();
	PushButton();
	PushButton(UINT button_id, HINSTANCE instance_handle, HWND window_handle);
	PushButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height);
	PushButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, DWORD style, int x_pos, int y_pos, int width, int height);

	void createButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height);
	void createButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, DWORD style, int x_pos, int y_pos, int width, int height);

	HBRUSH drawButton();

	void setButtonClicked();
	void setButtonUnClicked();
	BOOL isButtonClick();

	void setStyleNormal();
	void setStyleAbnormal();

	void setPosition(int x_pos, int y_pos);
	void setSize(int width, int height);
	void setLabel(LPCWSTR label);
	void setColor(BYTE r, BYTE g, BYTE b);
	void setColorClicked(BYTE r, BYTE g, BYTE b);
	void setTextColor(BYTE r, BYTE g, BYTE b);
	void setFontSize(int font_size);

	UINT getID();
	void setID(UINT id);

	LPCWSTR getButtonName();
	COLORREF getColor();
	COLORREF getColorClicked();
	HWND getHandleButton();
	HBRUSH getBrushBackground();
	HBRUSH getBrushClicked();
	SIZE getSize();
	POINT getPosition();
	POINT getPositionFront();
	POINT getPositionEnd();

	void hideButton();
	void showButton();
	void disableButton();
	void enableButton();

};