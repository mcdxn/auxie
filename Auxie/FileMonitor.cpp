//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "FileMonitor.h"

FileMonitor::~FileMonitor()
{
	if (FileMonitor::directory_handle);
		CloseHandle(FileMonitor::directory_handle);
}

FileMonitor::FileMonitor()
{
}

BOOL FileMonitor::is_start = FALSE;
FileMonitor* FileMonitor::monitor = nullptr;

std::list<HWND>::iterator FileMonitor::iterator;
std::list<HWND> FileMonitor::refresh_list;

HWND FileMonitor::hwnd = NULL;

MYFILE_NOTIFY_INFORMATION FileMonitor::myfni = {};
OVERLAPPED FileMonitor::overlap = {};

HANDLE FileMonitor::directory_handle = NULL;
HANDLE FileMonitor::event_handle = NULL;
HANDLE FileMonitor::wait_handle = NULL;

FileMonitor* FileMonitor::initMonitor()
{
	FileMonitor::monitor = new FileMonitor;
	return FileMonitor::monitor;
}

void FileMonitor::destroyMonitor()
{
	FileMonitor::stopMonitor();

	if (FileMonitor::monitor)
	{
		delete FileMonitor::monitor;
		FileMonitor::monitor = NULL;
	}
}

 void FileMonitor::startMonitor()
{
	FileMonitor::is_start = TRUE;
	FileMonitor::event_handle = CreateEvent(NULL, FALSE, FALSE, L"FILEEVENT");
	FileMonitor::overlap.hEvent = FileMonitor::event_handle;

	FileMonitor::startReadDirectoryChanges();
	FileMonitor::startWaitDirectoryChanges();
}

void FileMonitor::stopMonitor()
{
	FileMonitor::is_start = FALSE;
}

HANDLE FileMonitor::getFirstNotificationHandle()
{
	HANDLE h = NULL;
	TCHAR filepath[MAX_PATH] = {};
	if (GetCurrentDirectory(sizeof(filepath)*MAX_PATH, filepath) == 0)
		ALERT("Changes: GetCurrentDirectory");
	h = FindFirstChangeNotification(filepath, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);
	return h;
}

void  FileMonitor::addToRefreshList(HWND refresh_item)
{
	FileMonitor::refresh_list.push_back(refresh_item);
}

void FileMonitor::startRefresh()
{

	for (FileMonitor::iterator = FileMonitor::refresh_list.begin(); FileMonitor::iterator != FileMonitor::refresh_list.end(); FileMonitor::iterator++)
	{
		HWND pr = *FileMonitor::iterator;
	}

}

void FileMonitor::startReadDirectoryChanges()
{
	TCHAR filepath[MAX_PATH] = {};

	HRESULT result = GetCurrentDirectory(sizeof(TCHAR)*MAX_PATH, filepath);
	
	if (result == 0)
		ALERT("FileMonitor::startMonitor - GetCurrentDirectory");

	if (FileMonitor::directory_handle != NULL)
	{
		CloseHandle(FileMonitor::directory_handle);
	
		FileMonitor::directory_handle = NULL;
	}

	FileMonitor::directory_handle = CreateFile(filepath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
	
	if (FileMonitor::directory_handle == INVALID_HANDLE_VALUE)
		ALERT("FileMonitor::startMonitor - Invalid Directory Handle");

	result = ReadDirectoryChangesW(FileMonitor::directory_handle, &FileMonitor::myfni, sizeof(MYFILE_NOTIFY_INFORMATION), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE, 0, &FileMonitor::overlap, NULL);
	result = GetLastError();
	result = HRESULT_FROM_WIN32(result);
	
	if (result != S_OK)
		ALERT("FileMonitor::startMonitor - ReadDirectoryChanges");
}

void FileMonitor::startWaitDirectoryChanges()
{
	if (FileMonitor::wait_handle)
		UnregisterWaitEx(FileMonitor::wait_handle, NULL);

	RegisterWaitForSingleObject(&FileMonitor::wait_handle, FileMonitor::event_handle, FileMonitor::callbackProc, NULL, INFINITE, WT_EXECUTEINWAITTHREAD | WT_EXECUTEONLYONCE);
}

VOID CALLBACK FileMonitor::callbackProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	UNREFERENCED_PARAMETER(TimerOrWaitFired);
	UNREFERENCED_PARAMETER(lpParameter);

	if (FileMonitor::is_start)
	{
		int result = 0;

		PCTSTR filename = FileMonitor::myfni.FileName;

		result = lstrcmpW(filename, FILENAME_SETTINGSXML);

		ZeroMemory(&FileMonitor::myfni, sizeof(MYFILE_NOTIFY_INFORMATION));

		if (result == 0)
		{
			for (FileMonitor::iterator = FileMonitor::refresh_list.begin(); FileMonitor::iterator != FileMonitor::refresh_list.end(); FileMonitor::iterator++)
			{
				HWND refresh_window = *FileMonitor::iterator;
				PostMessage(refresh_window, ATEM_REFRESH, 0, 0);
			}

		}

		FileMonitor::startReadDirectoryChanges();
		FileMonitor::startWaitDirectoryChanges();
	}
}