//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include <Windows.h>

#define DLL_NAME L"BMDSwitcherAPI.dll"

typedef HRESULT(WINAPI *p)();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	
	HINSTANCE h = LoadLibrary(DLL_NAME);
	if (h != NULL)
	{
		p func = (p)GetProcAddress(h, "DllRegisterServer");
		HRESULT result = func();
		if (result == S_OK)
		{
			FreeLibrary(h);
			MessageBox(NULL, L"ATEM Dll Installation Successful!", L"Auxie", MB_OK);

			SHELLEXECUTEINFOW shex;
			ZeroMemory(&shex, sizeof(SHELLEXECUTEINFOW));

			shex.cbSize = sizeof(SHELLEXECUTEINFOW);
			shex.fMask = SEE_MASK_NOASYNC;
			shex.lpVerb = L"open";
			shex.lpFile = L"Auxie.exe";
			shex.nShow = SW_SHOWNORMAL;

			if (ShellExecuteExW(&shex))
				return TRUE;

			DWORD error = GetLastError();
			if (error == ERROR_FILE_NOT_FOUND)
				MessageBox(NULL, L"Cannot find Auxie.exe.\nMake sure file is in the same folder.", L"Auxie", MB_OK);

		}
	}
	else
	{
		MessageBox(NULL, L"Error loading DLL.", L"Auxie", MB_OK);
	}

	return 0;
}