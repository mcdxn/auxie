#include "MainCommon.h"

int GetIntValue(LPCWSTR value)
{
	int int_value = 0;
	if(value != NULL)
		int_value = _wtoi(value);
	
	return int_value;
}

LPCWSTR GetStringValue(LPCWSTR value)
{
	return value;
}

int CopyString(LPCWSTR src,int src_size, LPWSTR dest, int dest_size)
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

BOOL RunElevated(LPCWSTR file_name)
{
	SHELLEXECUTEINFOW shex;
	ZeroMemory(&shex, sizeof(SHELLEXECUTEINFOW));

	shex.cbSize = sizeof(SHELLEXECUTEINFOW);
	shex.hwnd = NULL; 
	shex.fMask = SEE_MASK_NOASYNC | SEE_MASK_FLAG_NO_UI;
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

BOOL Run(LPCWSTR file_name)
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