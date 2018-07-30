//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

typedef enum ConnectionStatus
{
	NotConnected,
	Connected,
	Disconnected,
	CouldNotConnect
}AUXIE_CONNECTION_STATUS;

#define MAX_RECONNECT_TRY 2
static ConnectionStatus CONNECTION_STATUS;
static WCHAR ConnectionStatusMessage[][50] = { 
	{ L"STATUS: Not Connected" }, 
	{ L"STATUS: Connected" }, 
	{ L"STATUS: Disconnected" }, 
	{ L"STATUS: Could Not Connect" },
};
