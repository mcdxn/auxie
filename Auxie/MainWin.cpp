//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include <Windows.h>
#include <comutil.h>
#include "Common.h"
#include "BMDSwitcherAPI_h.h"
#include "AuxieXMLElements.h"
#include "AuxieATEMInputNames.h"
#include "ATEMError.h"
#include "Util.h"
#include "AuxieATEM.h"
#include "AuxieMem.h"
#include "Winwin.h"
#include "PopupMenu.h"
#include "ConnectWin.h"

ConnectWin* axConnectWindow = NULL;
FileMonitor*  axFileMonitor = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	int cmdline_size = 0;
	LPWSTR* cmdline_options = CommandLineToArgvW(pCmdLine, &cmdline_size);

	HANDLE mutex_handle = CreateMutex(NULL, TRUE, L"Auxie");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND hwnd = FindWindow(CONNECTWIN_WINDOW_CLASSNAME, NULL);
	
		if (cmdline_size == 1)
		{
			HANDLE Arg1 = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MEM_NAME);
			PWSTR Arg1Buffer = (PWSTR)MapViewOfFile(Arg1, FILE_MAP_ALL_ACCESS, 0, 0, MEM_BUFFERSIZE);
			
			if (Arg1Buffer)
			CopyMemory((PVOID)Arg1Buffer, cmdline_options[0], MEM_BUFFERSIZE);
		
			UnmapViewOfFile(Arg1Buffer);
			
			if (Arg1)
			CloseHandle(Arg1);
			
			PostMessage(hwnd, ATEM_PERFORM_SWITCH, ATEM_SWITCH_FROM_DEAD_BUTTON, 0);
		}
		else ALERT("Only need one command.");

		
		return 0;
	}
	else
	{
		axConnectWindow = new ConnectWin(hInstance);

		if (axFileMonitor == NULL)
		{
			axFileMonitor = axFileMonitor->initMonitor();
			axFileMonitor->addToRefreshList(axConnectWindow->getWindowHandle());
			axFileMonitor->startMonitor();
		}
	}


	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}






