//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include "PushButton.h"

#define GROUP_STYLE WS_VISIBLE|WS_CHILD|BS_GROUPBOX

class Group : public PushButton
{
private:

public:
	~Group();
	Group(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height);

};