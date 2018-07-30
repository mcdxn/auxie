//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#pragma once

#include <Windows.h>
#include <iostream>
#include <list>

#include "Common.h"

typedef struct MY_FILE_NOTIFY_INFORMATION {
	DWORD NextEntryOffset;
	DWORD Action;
	DWORD FileNameLength;
	WCHAR FileName[256];
} MYFILE_NOTIFY_INFORMATION, *PMYFILE_NOTIFY_INFORMATION;

class FileMonitor 
{
private:

	static FileMonitor* monitor;

	static BOOL is_start;
	static std::list<HWND> refresh_list;
	static std::list<HWND>::iterator iterator;

    static HWND hwnd;

	static MYFILE_NOTIFY_INFORMATION myfni;
	static OVERLAPPED overlap;
	static HANDLE directory_handle;
	static HANDLE event_handle;
	static HANDLE wait_handle;

	static void startRefresh();
	static HANDLE getFirstNotificationHandle();
	static VOID CALLBACK callbackProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

	static void startReadDirectoryChanges();
	static void startWaitDirectoryChanges();

	~FileMonitor();
	FileMonitor();

public:

	FileMonitor* initMonitor();

	void destroyMonitor();
	void startMonitor();
	void stopMonitor();

	//void addToRefreshList(PREFRESH refresh_item);
	void addToRefreshList(HWND refresh_item);
};
