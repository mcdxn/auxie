//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once

#define ALERT(msg) MessageBox(NULL,TEXT(msg),TEXT("Hiccup"),MB_OK)

#ifdef _DEBUG
#define DEBUGMSG(msg) OutputDebugString(msg)
#define DEBUGSPACE DEBUGMSG(L"\n")
#define DEBUGINDENT DEBUGMSG(L" ")
#endif

#define SAFEDELETEOBJECT(object) {if(object) delete object, object = nullptr;}
#define SAFECLOSEHANDLE(handle) {if(handle) CloseHandle(handle),handle = NULL;}

#define SOFTWARE_VERSION_NAME L"Auxie 0.a2 (ALPHA)"

#define FILENAME_SETTINGSXML L"settings.xml"

#define ATEM_PERFORM_SWITCH WM_USER+777
#define ATEM_INIT WM_USER+700
#define ATEM_CONNECT WM_USER+705
#define ATEM_DISCONNECT WM_USER+710
#define ATEM_REFRESH WM_USER+715


typedef enum ConnectionStatus
{
	NOTCONNECTED,
	CONNECTED,
	DISCONNECTED,
	COULDNOTCONNECT
}AUXIE_CONNECTION_STATUS;

typedef class _Refresh{
public:
	virtual void refresh()=0;
	virtual void refresh(LPVOID) { return; };
}REFRESH,*PREFRESH;