//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "PushButton.h"

PushButton::~PushButton()
{
	RemoveWindowSubclass(this->hwnd, PushButtonProc, this->id);

	if (this->brush_background)
		DeleteBrush(this->brush_background);

	if (this->brush_clicked)
		DeleteBrush(this->brush_clicked);

	if (this->hwnd)
		DestroyWindow(this->hwnd);

	this->brush_clicked = NULL;
	this->brush_background = NULL;
	this->hwnd = NULL;
}

PushButton::PushButton()
{
	this->hwnd = NULL;
}

PushButton::PushButton(UINT button_id, HINSTANCE instance_handle, HWND window_handle)
{
	this->createButton(button_id, BUTT_DEFAULT_LABEL, instance_handle, window_handle, 0, 0, BUTT_DEFAULT_WIDTH, BUTT_DEFAULT_WIDTH);

}

PushButton::PushButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height)
{
	this->createButton(button_id, label, instance_handle, window_handle, BUTT_STYLE_NORMAL, x_pos, y_pos, width, height);
}

PushButton::PushButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, DWORD style, int x_pos, int y_pos, int width, int height)
{
	this->createButton(button_id, label, instance_handle, window_handle, style, x_pos, y_pos, width, height);
}

void PushButton::createButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height)
{ 
	this->createButton(button_id, label, instance_handle, window_handle, BUTT_STYLE_NORMAL,  x_pos,  y_pos,  width,  height);
}

void PushButton::createButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, DWORD style, int x_pos, int y_pos, int width, int height)
{
	this->is_clicked = FALSE;
	this->button_label = label;
	this->hwnd = CreateWindowEx(0, BUTT_CLASSNAME, label, style, x_pos, y_pos, width, height, window_handle, (HMENU)button_id, instance_handle, NULL);
	this->id = button_id;

	SetWindowSubclass(this->hwnd,PushButtonProc,this->id,(DWORD) this);
}

HBRUSH PushButton::drawButton()
{
	RECT crect;
	HDC hdc = GetDC(this->hwnd);

	GetClientRect(this->hwnd, &crect);
	SelectObject(hdc,this->font_handle);
	SetBkColor(hdc, this->color_background);
	SetTextColor(hdc, this->text_color);
	
	DrawText(hdc, this->button_label, lstrlen(this->button_label), &crect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	ReleaseDC(this->hwnd, hdc);

	if (this->is_clicked)
	{
		this->is_clicked = FALSE;
		return this->brush_clicked;
	}
	else
		return this->brush_background;
}

BOOL PushButton::isButtonClick()
{
	return this->is_clicked;
}

void PushButton::setButtonClicked()
{
	this->is_clicked = TRUE;
	InvalidateRect(this->hwnd, NULL, TRUE);
}

void PushButton::setButtonUnClicked()
{
	this->is_clicked = FALSE;
	InvalidateRect(this->hwnd, NULL, TRUE);
}

void PushButton::setStyleNormal()
{
	SetWindowLongPtr(this->hwnd, GWL_STYLE, BUTT_STYLE_NORMAL);
}

void PushButton::setStyleAbnormal()
{
	SetWindowLongPtr(this->hwnd, GWL_STYLE, BUTT_STYLE_ABNORMAL);
}

void PushButton::setFontSize(int font_size)
{
	this->font_size = font_size;
	this->font_handle = CreateFont(font_size, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, NULL);
	
}

void PushButton::setPosition(int x_pos, int y_pos)
{
	SetWindowPos(this->hwnd, NULL, x_pos, y_pos, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
}

void PushButton::setSize(int width, int height)
{
	SetWindowPos(this->hwnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
}

void PushButton::setLabel(LPCWSTR label)
{
	this->button_label = label;
	Button_SetText(this->hwnd, label);
}

void PushButton::setColor(BYTE r, BYTE g, BYTE b)
{
	this->color_background = RGB(r, g, b);
	this->brush_background = CreateSolidBrush(this->color_background);
}

void PushButton::setColorClicked(BYTE r, BYTE g, BYTE b)
{
	this->color_clicked = RGB(r, g, b);
	this->brush_clicked = CreateSolidBrush(this->color_clicked);
}

void PushButton::setTextColor(BYTE r, BYTE g, BYTE b)
{
	this->text_color = RGB(r, g, b);
}

COLORREF PushButton::getColor()
{
	return this->color_background;
}

COLORREF PushButton::getColorClicked()
{
	return this->color_clicked;
}

HBRUSH PushButton::getBrushBackground()
{
	return this->brush_background;
}

HBRUSH PushButton::getBrushClicked()
{
	return this->brush_clicked;
}

HWND PushButton::getHandleButton()
{
	return this->hwnd;
}

UINT PushButton::getID()
{
	return this->id;
}

void PushButton::setID(UINT id)
{
	this->id = id;
	SetMenu(this->hwnd, (HMENU)id);
}

LPCWSTR PushButton::getButtonName()
{
	return this->button_label;
}

SIZE PushButton::getSize()
{
	RECT rect = {0,0,0,0};
	SIZE size = {0,0};

	GetClientRect(this->hwnd, &rect);

	size.cx = rect.right;
	size.cy = rect.bottom;

	return size;
}

POINT PushButton::getPosition()
{
	return this->getPositionFront();
}

POINT PushButton::getPositionFront()
{
	POINT position;
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(this->hwnd, &wp);

	position.x = wp.rcNormalPosition.left;
	position.y = wp.rcNormalPosition.top;

	return position;
}

POINT PushButton::getPositionEnd()
{

	POINT position;
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(this->hwnd, &wp);

	position.x = wp.rcNormalPosition.right;
	position.y = wp.rcNormalPosition.top;

	return position;
}

void PushButton::hideButton()
{
	ShowWindow(this->hwnd, SW_HIDE);
}

void PushButton::showButton()
{
	ShowWindow(this->hwnd, SW_SHOW);
}

void PushButton::disableButton()
{
	Button_Enable(this->hwnd, FALSE);
}

void PushButton::enableButton()
{
	Button_Enable(this->hwnd, TRUE);
}