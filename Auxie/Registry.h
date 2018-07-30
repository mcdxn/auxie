//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <winreg.h>

class Registry
{
protected:
	HKEY reg_key;

public:
	~Registry();
	Registry();
	Registry(HKEY key_handle, LPCWSTR subkey_name);

	int initRegistry(HKEY key_handle, LPCWSTR subkey_name);

	void closeRegistry();

	int openCurrentUser(LPCWSTR subkey_name);

	int isValueExists(LPCWSTR name);

	int setStringValue(LPCWSTR name, LPCWSTR string_value);
	int setIntValue(LPCWSTR name, int int_value);
	
	int setBinaryValue(LPCWSTR name, LPVOID value, int size_of_value);
	int getBinaryValue(LPCWSTR name, LPVOID value, int size_of_value);

	int getStringValue(LPCWSTR name, PWCHAR buffer, int buffer_size);
	int getIntValue(LPCWSTR name);
};