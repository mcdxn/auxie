//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "Util.h"


int Util_getIntValue(LPCWSTR value)
{
	int int_value = 0;
	if(value != NULL)
		int_value = _wtoi(value);
	
	return int_value;
}

int Util_copyString(LPCWSTR src,int src_size, LPWSTR dest, int dest_size)
{
	if (src_size == 0 || dest_size == 0)
	{
		ALERT("CopyString: zero length string.");
		return 1;
	}

	if (src_size > dest_size)
	{
		ALERT("CopyString: buffer not big enough.");
		return 1;
	}
	else
	{
		for (int i = 0; i < src_size; i++)
			dest[i] = src[i];
	}

	return 0;
}

BOOL Util_runElevated(LPCWSTR file_name)
{
	SHELLEXECUTEINFOW shex;
	ZeroMemory(&shex, sizeof(SHELLEXECUTEINFOW));

	shex.cbSize = sizeof(SHELLEXECUTEINFOW);
	shex.hwnd = NULL; 
	shex.fMask =  SEE_MASK_FLAG_NO_UI;
	shex.lpVerb = L"runas";
	shex.lpFile = file_name;

	if (ShellExecuteExW(&shex))
		return TRUE;

	DWORD error = GetLastError();
	if (error == ERROR_FILE_NOT_FOUND)
		MessageBox(NULL, (_bstr_t(L"Cannot find ") + file_name + _bstr_t(L"\nMake sure file is in the same folder.")), L"Auxie", MB_OK);
	if (error == ERROR_CANCELLED)
		MessageBox(NULL, L"Could not complete operation.\nI need elevated privilidges to proceed.\nPlease try again.", L"Auxie", MB_OK);
	return FALSE;
}

BOOL Util_runNow(LPCWSTR file_name)
{
	SHELLEXECUTEINFOW shex;
	ZeroMemory(&shex, sizeof(SHELLEXECUTEINFOW));

	shex.cbSize = sizeof(SHELLEXECUTEINFOW);
	shex.fMask = SEE_MASK_NOASYNC;
	shex.lpVerb = L"open";
	shex.lpFile = file_name;
	 
	if (ShellExecuteExW(&shex))
		return TRUE;

	DWORD error = GetLastError();
	if (error == ERROR_FILE_NOT_FOUND)
		MessageBox(NULL, (_bstr_t(L"Cannot find ") + file_name + _bstr_t(L"\nMake sure file is in the same folder.")), L"Auxie", MB_OK);

	return FALSE;
}

UINT Util_extractNumberFromString(LPCTSTR in_string)
{
	if (in_string == NULL)
		return 0;

	TCHAR digits[10] = {};
	int current_digit = 0;
	UINT extracted_number = 0;

	for (int i = 0; in_string != '\0'; i++)
	{
		if (Util_isNumber(in_string[i]))
		{
			digits[current_digit++] = in_string[i];

			if (Util_isNumber(in_string[i + 1]))
				continue;
			else
				break;
		}
	}

	extracted_number = Util_convertStringToNumber(digits);

	return extracted_number;
}

BOOL Util_isAlphabet(const TCHAR in_char)
{	
	TCHAR alpha_capital = 65;
	TCHAR alpha_small = 97;
	
	for (int a = 0; a != 27; a++, alpha_capital++, alpha_small++)
	{
		if (in_char == alpha_capital || in_char == alpha_small)
			return TRUE;
	}

	return FALSE;
}

BOOL Util_isNumber(const TCHAR in_char)
{
	TCHAR current_number = 48;

	for (int n = 0; n != 10; n++, current_number++)
	{
		if (in_char == current_number)
			return TRUE;
	}

	return FALSE;
}

UINT Util_convertStringToNumber(LPCTSTR in_string)
{
	if (in_string == NULL || in_string[0] == 0x2D)
		return NULL;

	int dictionary[] = {  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
	int place_value[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
	int place_position = 0;

	TCHAR extracted_num = '0';
	UINT converted_num = 0;
	int digit = 0;

	for (int i = 0; Util_isNumber(in_string[i]); i++, place_position++);

	place_position -= 1;

	for (int i = 0; in_string[i] != '\0'; i++)
	{
		if (Util_isNumber(in_string[i]))
		{
			extracted_num = in_string[i];
			for (digit = 0; extracted_num != dictionary[digit]; digit++);
			converted_num += (dictionary[digit] ^ 0x30) * place_value[place_position - i];
		}
	}

	return converted_num;
}

INT Util_convertStringToNumberSigned(LPCTSTR in_string)
{
	if (in_string == NULL || in_string[0] != 0x2D)
		return NULL;

	int dictionary[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };	
	int place_value[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
	int place_position = 0;
	TCHAR extracted_num = '0';
	int converted_num = 0;
	int digit = 0;

	for (int i = 0; Util_isNumber(in_string[i+1]); i++, place_position++);

	place_position -= 1;

	for (int i = 0; in_string[i] != '\0'; i++)
	{
		if (Util_isNumber(in_string[i+1]))
		{
			extracted_num = in_string[i+1];
			for (digit = 0; extracted_num != dictionary[digit]; digit++);
			converted_num += (dictionary[digit] ^ 0x30) * place_value[place_position - i];
		}
	}
		
	return ((~converted_num) + 1);
}