//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "Group.h"

Group::~Group()
{

}

Group::Group(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height)
{
	this->createButton( button_id, label, instance_handle, window_handle, GROUP_STYLE,x_pos, y_pos, width,  height);
}