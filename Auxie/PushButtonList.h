//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include "Common.h"
#include "PushButton.h"

#define PUSHBUTTONLIST_MARGIN 10

class PushButtonList 
{
private:

	static int size;

	PushButton* button;
	PushButtonList* next;
	PushButtonList* prev;

	void adjustButtonPosition();
	void adjustButtonPositionVertical();
	void adjustButtonPositionHorizontal();

	PushButtonList* getNextEmpty();
	PushButtonList* getHead();
	PushButtonList* getTail();
	int getListCount();

	void compactButtonList(PushButtonList* button_list);

public:

	~PushButtonList();
	PushButtonList();
	PushButtonList(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height);

	void initButtons(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height);

	HBRUSH drawButtons(UINT button_id);


	void hideButtons();
	void showButtons();

	void addButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle);
	
	void delButton(UINT button_id);
	void delAllButtons();

	void disableButton(UINT button_id);


	void clearButtonsClickStateToUnClicked();
	void setButtonsClicked(UINT button_id);
	void setButtonsUnClicked(UINT button_id);
	void setButtonsStyleNormal();
	void setButtonsStyleAbnormal();
	void setButtonsSize(int width,int height);
	void setButtonsPosition(int x, int y);
	void setButtonsColor(BYTE r, BYTE g, BYTE b);
	void setButtonsTextColor(BYTE r, BYTE g, BYTE b);
	void setButtonsTextSize(int text_size);
	void setButtonsClickedColor(BYTE r, BYTE g, BYTE b);

	SIZE getButtonsTotalWindowArea();
};