//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#include "ConnectWin.h"


Test* for_testing = nullptr;

AuxieATEM* axAtem = NULL;
AuxieXMLParser* axParser = NULL;
AuxieTaskbar* axTaskbar = NULL;
AuxieMem* axMem = NULL;

extern FileMonitor*  axFileMonitor;
extern ConnectWin* axConnectWindow;
extern ToggleWin* axToggleWin;
extern LiveButtons* axLiveButtons;

namespace ASYNC
{
	VOID CALLBACK ConnectAsyncWaitCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
	DWORD WINAPI ConnectAsyncThreadCallback(LPVOID lpParameter);

	void initConnectAsync();
	void startConnectAsync();
	void resetConnectAsync();

	HANDLE ConnectAsyncEventHandle = NULL;
	HANDLE ConnectAsyncThreadHandle = NULL;
	HANDLE ConnectAsyncWaitHandle = NULL;

	void initConnectAsync()
	{
		ConnectAsyncThreadHandle = CreateThread(NULL, 0, ConnectAsyncThreadCallback, NULL, CREATE_SUSPENDED, NULL);
	
		ConnectAsyncEventHandle = CreateEvent(NULL, TRUE, FALSE, L"CONNECTEVENT");
		 
		RegisterWaitForSingleObject(&ConnectAsyncWaitHandle, ConnectAsyncEventHandle, ConnectAsyncWaitCallback, NULL, INFINITE, WT_EXECUTEINWAITTHREAD | WT_EXECUTEONLYONCE);
	}

	void destroyConnectAsync()
	{
		UnregisterWait(ConnectAsyncWaitHandle);

		SAFECLOSEHANDLE(ConnectAsyncEventHandle);
		SAFECLOSEHANDLE(ConnectAsyncThreadHandle);
	}

	void resetConnectAsync()
	{
		destroyConnectAsync();
		initConnectAsync();
	}

	void startConnectAsync()
	{
		ResumeThread(ConnectAsyncThreadHandle);
	}

	VOID CALLBACK ConnectAsyncWaitCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		ResetEvent(ConnectAsyncEventHandle);
		
		if (!IsWindowEnabled(axConnectWindow->getConnectButton()->getWindowHandle()))
		{
			axConnectWindow->enableConnectButton();
			axConnectWindow->enableIPAddressField();
		}

		resetConnectAsync();
	}

	DWORD WINAPI ConnectAsyncThreadCallback(LPVOID lpParameter)
	{
		HWND hwnd = ::axConnectWindow->getWindowHandle();

		_bstr_t* ip_address_entered = ::axConnectWindow->getIPAddressFieldContents();

		if (ip_address_entered == nullptr)
		{
			::axConnectWindow->enableIPAddressField();
			::axConnectWindow->enableConnectButton();

			return 0;
		}

		::axAtem->Connect(&ip_address_entered->GetBSTR());

		if (::axAtem->GetConnectionStatus() == CONNECTED)
		{
			::axAtem->InitializeInputs();
			::axAtem->InitializeAuxes();
			::axAtem->InitializeKeys();
			::axAtem->InitializeDSK();

			if (AuxieXMLParser::getButtons())
				::axAtem->InitializeButtons(AuxieXMLParser::getButtons());
			else
			{
				if (::axParser == NULL)
					::axParser = new AuxieXMLParser(FILENAME_SETTINGSXML);
				SAFEDELETEOBJECT(::axParser);
				::axAtem->InitializeButtons(AuxieXMLParser::getButtons());
			}

			::axTaskbar->UpdateTrayIconTooltip(ConnectionStatusMessage[::axAtem->GetConnectionStatus()]);

			ip_address_entered = ::axConnectWindow->getIPAddressFieldContents();

			AuxieRegistry* axRegistry = new AuxieRegistry();
			axRegistry->initIP();
			axRegistry->updateIP((LPCWSTR)*ip_address_entered);
			SAFEDELETEOBJECT(axRegistry);

			::axConnectWindow->updateConnectionStatusDisplay();
			::axConnectWindow->clearIPAddressField();
			::axConnectWindow->disableConnectButton();
			::axConnectWindow->enableIPAddressField();

			PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);

			return 0;
		}
		else
		{
			SetEvent(ConnectAsyncEventHandle);
		}

		InvalidateRect(hwnd, NULL, FALSE);
		UpdateWindow(hwnd);

		return 0;
	}

}


LRESULT CALLBACK ConnectProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case ATEM_REFRESH:
	{
		axParser = new AuxieXMLParser(FILENAME_SETTINGSXML);	
		SAFEDELETEOBJECT(axParser);		
	
		if (axAtem->GetConnectionStatus() == CONNECTED)
			axAtem->refresh(AuxieXMLParser::getButtons());

		// TEMPORARY REFRESH CODE FOR - LIVEBUTTONS;
		if (axLiveButtons)
		{
			POINT position = axLiveButtons->getWindowPositionInDesktop();

			delete axLiveButtons;
			axLiveButtons = NULL;

			axLiveButtons = new LiveButtons((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE));
			axLiveButtons->setMainWindowHandle(hwnd);
			axLiveButtons->prepareLiveButtons(AuxieXMLParser::getButtons());

			SIZE area = axLiveButtons->push_buttons->getButtonsTotalWindowArea();

			axLiveButtons->setWindowPosition(position.x, position.y);
			axLiveButtons->setWindowSize(area.cx, area.cy);

			axLiveButtons->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_HIDEBORDER_ID);
			axLiveButtons->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_ALWAYSONTOP_ID);

			axLiveButtons->enableAlwaysOnTop();
			axLiveButtons->hideWindowBorder();
			axLiveButtons->showWindow();
		}

		return 0;
	}
	case ATEM_CONNECT:
	{	
		axConnectWindow->disableConnectButton();
		axConnectWindow->disableIPAddressField();
		SetForegroundWindow(hwnd);
		
		ASYNC::startConnectAsync();

		return 0; 
	}

	case ATEM_DISCONNECT:
	{return 0; }

	case ATEM_PERFORM_SWITCH:
	{
		if (axAtem->GetConnectionStatus() == CONNECTED)
		{
			switch (wParam)
			{
			case ATEM_SWITCH_FROM_DEAD_BUTTON:
			{
				axAtem->PerformSwitch(axMem->GetValueAuxieMem());	
			}break;

			case ATEM_SWITCH_FROM_LIVE_BUTTON:
			{
				axAtem->PerformSwitch(lParam);
			}break;
			default:break;

			}
		}
		else ALERT("Must be connected to a switcher");

		return 0;
	}

	case WM_GETMINMAXINFO:
	{
		PMINMAXINFO pmmi = (PMINMAXINFO)lParam;
		pmmi->ptMinTrackSize.x = 10;
		pmmi->ptMinTrackSize.y = 10;

		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (wParam == MK_LBUTTON)
			SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);

		return 0;
	}

	case WM_DESTROY:
	{
		axParser->destroyButtons();
		axFileMonitor->destroyMonitor();

		ASYNC::destroyConnectAsync();

		SAFEDELETEOBJECT(axToggleWin);
		SAFEDELETEOBJECT(axTaskbar);
		SAFEDELETEOBJECT(axAtem);
		SAFEDELETEOBJECT(axConnectWindow);
		SAFEDELETEOBJECT(axMem);

		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		return 0;
	}

	case WM_QUIT:
	{
		DestroyWindow(hwnd);
		return wParam;
	}

	case WM_CREATE:
	{	
		//if (for_testing == nullptr)
		//	for_testing = new Test((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE));

		ASYNC::initConnectAsync();
	
		if (axParser == NULL)
		{
			axParser = new AuxieXMLParser(FILENAME_SETTINGSXML);
			SAFEDELETEOBJECT(axParser);
			axParser = NULL;
		}

		if (axAtem == NULL)
		{
			axAtem = new AuxieATEM();

			if (axAtem->Initialize())
			{
				axAtem->InstallDLL();
				return 0;
			}
		}

		if(axTaskbar == NULL) 
			axTaskbar = new AuxieTaskbar(hwnd);

		if(axMem == NULL) 
			axMem = new AuxieMem();

		if (axToggleWin == NULL)
		{
			axToggleWin = new ToggleWin((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE));
			axToggleWin->hideWindow();
		}

		if (axLiveButtons == NULL)
		{
			axLiveButtons = new LiveButtons((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE));
			axLiveButtons->setMainWindowHandle(hwnd);
			axLiveButtons->prepareLiveButtons(AuxieXMLParser::getButtons());
			
			SIZE area = axLiveButtons->push_buttons->getButtonsTotalWindowArea();
			axLiveButtons->setWindowSize(area.cx, area.cy);

			axLiveButtons->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_HIDEBORDER_ID);
			axLiveButtons->popup_menu->markCheckMenuItem(POPUPMENU_ITEM_ALWAYSONTOP_ID);
			
			axLiveButtons->enableAlwaysOnTop();
			axLiveButtons->hideWindowBorder();
			axLiveButtons->showWindow();
		}

		PostMessage(hwnd, ATEM_CONNECT, 0, 0);

	}break;

	

	case WM_TRAYICON_MESSAGE:
	{
		// PURPOSE: Handles trayicon events.
		// TO-DO:
		// 
		if (HIWORD(lParam) == ID_TRAYICON)
		{

			if (LOWORD(lParam) == WM_LBUTTONDOWN)
			{
				axTaskbar->MaximizeTrayIcon();

				return 0;
			}

			// PURPOSE: Shows option when user right click's trayicon.
			// TO-DO:
			// Show Pop-up window with Exit, About,Settings options, Presets, etc.
			if (LOWORD(lParam) == WM_RBUTTONDOWN)
			{

				return 0;
			}

			if (LOWORD(lParam) == WM_RBUTTONUP)
			{
				axConnectWindow->getPopupMenu()->showPopupMenu();

				return 0;
			}

			if (LOWORD(lParam) == NIN_POPUPCLOSE)
			{}

			if (LOWORD(lParam) == NIN_POPUPOPEN)
			{}
		}
		
	}break;

	
	case WM_PAINT:
	{
		static PAINTSTRUCT ps = {};
		static HDC hdc = NULL;
		static HFONT hfont = CreateFont(15, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, NULL);

		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, hfont);
		SetBkColor(hdc,axConnectWindow->getBackgroundColor());
		
		TextOutW(hdc, CONNECTWIN_DEFAULT_ITEMSMARGIN, 15, ConnectionStatusMessage[axAtem->GetConnectionStatus()], _countof(ConnectionStatusMessage[axAtem->GetConnectionStatus()]));
		TextOutW(hdc, CONNECTWIN_DEFAULT_ITEMSMARGIN, 80, axAtem->GetSwitcherName(),ATEM_SWITCHER_NAME_SIZE);
		
		EndPaint(hwnd, &ps);
		
	}break;
	
	case WM_SYSCOMMAND:
	{
		switch (wParam){

		case SC_MINIMIZE:
		{
			axTaskbar->MinimizeTrayIcon();
		}break;

		default:break;
		}
	}break;

	case  WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
		{
			
			if (LOWORD(wParam) == ID_INPUTBUTTONCONNECT)
			{
				
				SendMessage(hwnd, ATEM_CONNECT, 0, 0);
			}

			
		}break;
		

		// Input IP Address Messages
		case EN_SETFOCUS:
		{
			//if (LOWORD(wParam) == ID_INPUTIPADRESS)
			//if (SWITCHER_CONNECTED)
			//if (SendMessage(hwndInputIPAddress, IPM_ISBLANK, 0, 0))EnableWindow(hwndInputButton, true); 
		
			
		}break;
		
		default:break;
		} // END OF SWITCH

		switch (LOWORD(wParam))
		{
		
		case POPUPMENU_ITEM_ABOUT_ID:
		{
			HINSTANCE hinstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
			HICON icon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
			MSGBOXPARAMS msgbox;
			ZeroMemory(&msgbox, sizeof(MSGBOXPARAMS));

			msgbox.cbSize = sizeof(MSGBOXPARAMS);
			msgbox.hwndOwner = hwnd;
			msgbox.hInstance = hinstance;
			
			WCHAR buffer[256] = {};
			LPCWSTR formatted = 
			L"\n%c 2015 Q - Mr. McByte\nAll rights reserved.\nMark 12:30\nmr.mcbyte@live.com";
			
			StringCbPrintf(buffer, _countof(buffer), formatted, 169);

			_bstr_t versioning(SOFTWARE_VERSION_NAME);
			versioning += _bstr_t(buffer);

			msgbox.lpszText = (LPCWSTR)versioning;
			msgbox.lpszCaption = TEXT("About Auxie");
			msgbox.dwStyle = MB_OK |MB_USERICON|MB_TOPMOST|MB_DEFBUTTON1;
			msgbox.lpszIcon = MAKEINTRESOURCE(IDI_ICON2);
			msgbox.dwLanguageId = LANG_ENGLISH;

			MessageBoxIndirect(&msgbox);


		}break;

		case POPUPMENU_ITEM_EXIT_ID:
		{
			DestroyWindow(hwnd);
		}break;

		case POPUPMENU_ITEM_LIVEBUTTONS_ID:
		{
			axLiveButtons->showWindow();
		}break;
		case POPUPMENU_ITEM_TOGGLEMONITOR_ID:
		{
			axToggleWin->showWindow();
		}break;

		case POPUPMENU_ITEM_CONNECT_ID:
		{
			axTaskbar->MaximizeTrayIcon();
		}break;
	
		default:break;
		} // END OF SWITCH
		
	}break; // END OF WM_COMMAND.

	case WM_NOTIFY:{
		
		// Checks which ip field was modified
		LPNMIPADDRESS ip_address = (LPNMIPADDRESS)lParam;
		if (ip_address->hdr.code == IPN_FIELDCHANGED)
			if (ip_address->hdr.idFrom == ID_INPUTIPADRESS)
				if (ip_address->iField == 2)
				{
					DWORD ip_address = 0;
					SendMessage(axConnectWindow->getIPAddressField()->getWindowHandle(), IPM_GETADDRESS, 0, (LPARAM)(DWORD)&ip_address);
					if (SECOND_IPADDRESS(ip_address) != 0 || THIRD_IPADDRESS(ip_address) != 0)
						axConnectWindow->enableIPAddressField();
				}
		
	}break; // END OF WM_NOTIFY.
	
	default: break;

	} // END OF MAIN MESSAGE SWITCH.

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

