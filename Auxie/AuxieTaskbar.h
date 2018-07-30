//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <VersionHelpers.h>
#include <WinUser.h>
#include <winerror.h>
#include <strsafe.h>

#include "resource.h"
#include "BMDSwitcherAPI_h.h"
#include "Common.h"
#include "Registry.h"
#include "AuxieXMLElements.h"
#include "AuxieATEM.h"
#include "AuxieRegistry.h"

#define ID_TRAYICON 2014

#define WM_TRAYICON_MESSAGE WM_USER+720 

class AuxieTaskbar {
	
private: 
	HWND hwnd;
	BOOL  is_win8;
	GUID icon_guid = {};
public:
	AuxieTaskbar();
	AuxieTaskbar(HWND window_handle);
	~AuxieTaskbar();

	void InitTrayIcon(HWND window_handle);
	void DestroyTrayIcon();
	void AddTrayIcon();
	void RemoveTrayIcon();
	void UpdateTrayIconTooltip(LPCWCHAR message);
	void MinimizeTrayIcon();
	void MaximizeTrayIcon();
};