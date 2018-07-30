//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <comutil.h>
#include "Common.h"

int  Util_getIntValue(LPCWSTR value);
int  Util_copyString(LPCWSTR src, int src_size, LPWSTR dest, int dest_size);
BOOL Util_runElevated(PCWSTR file_name);
BOOL Util_runNow(LPCWSTR file_name);
UINT Util_extractNumberFromString(LPCWSTR in_string);
UINT Util_convertStringToNumber(LPCTSTR in_string);
INT Util_convertStringToNumberSigned(LPCTSTR in_string);
BOOL Util_isAlphabet(const TCHAR in_wchar);
BOOL Util_isNumber(const TCHAR in_wchar);
