//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "Registry.h"

Registry::~Registry()
{
	if (this->reg_key)
		RegCloseKey(this->reg_key);
}

Registry::Registry()
{
	reg_key = NULL;
}

Registry::Registry(HKEY key_handle, LPCWSTR subkey_name)
{
	this->initRegistry(key_handle, subkey_name);
}

int Registry::initRegistry(HKEY key_handle, LPCWSTR subkey_name)
{
	DWORD disposition = NULL;
	LONG result = NULL;

	result = RegCreateKeyEx(key_handle,subkey_name,0,NULL,REG_OPTION_NON_VOLATILE,KEY_READ | KEY_WRITE,NULL,&this->reg_key,&disposition);
	if (result != ERROR_SUCCESS)
		return 1;

	return 0;
}

void Registry::closeRegistry()
{
	if (this->reg_key)
		RegCloseKey(this->reg_key);
}

int Registry::isValueExists(LPCWSTR name)
{
	LONG result = 0;

	result = RegQueryValueEx(this->reg_key, name, NULL, NULL, NULL, NULL);
	if (result == ERROR_FILE_NOT_FOUND)
			return 0;

	return 1;
}

int Registry::setStringValue(LPCWSTR name, LPCWSTR string_value)
{
	DWORD size = (lstrlen(string_value))*2;
	LONG result = 0;

	result = RegSetValueExW(this->reg_key, name, 0, REG_SZ, (BYTE*)string_value, size);
	if (result != ERROR_SUCCESS)
		return 1;

	return 0;
}

int Registry::getStringValue(LPCWSTR name, PWCHAR buffer, int buffer_size)
{
	DWORD size = buffer_size;
	LONG result = ERROR_SUCCESS;

	result = RegGetValue(this->reg_key,NULL,name,RRF_RT_REG_SZ,NULL,buffer, &size);
	if (result != ERROR_SUCCESS)
		return 1;

	return 0;
}

int Registry::setBinaryValue(LPCWSTR name, LPVOID value, int size_of_value)
{
	DWORD size = size_of_value;
	LONG result = 0;

	result = RegSetValueExW(this->reg_key, name, 0, REG_BINARY, (BYTE*)value, size);
	if (result != ERROR_SUCCESS)
		return 1;

	return 0;
}

int Registry::getBinaryValue(LPCWSTR name, LPVOID value, int size_of_value)
{
	DWORD size = size_of_value;
	LONG result = ERROR_SUCCESS;

	result = RegGetValue(this->reg_key, NULL, name, RRF_RT_REG_BINARY, NULL, value, &size);
	if (result != ERROR_SUCCESS)
		return 1;

	return 0;
}

int Registry::openCurrentUser(LPCWSTR subkey_name)
{
	if (this->reg_key)
		RegCloseKey(this->reg_key);

	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, subkey_name, 0, KEY_READ | KEY_WRITE, &this->reg_key);

	if (result != ERROR_SUCCESS)
	{
		result = this->initRegistry(HKEY_CURRENT_USER, subkey_name);
		if (result)
			return 1;
	}

	return 0;
}

