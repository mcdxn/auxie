//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include "Registry.h"
#include "Util.h"

#define AUXIE_REG_KEY L"Software\\Quorban\\Auxie"
#define AUXIE_REG_IP_BUFFERSIZE 30

class AuxieRegistry : private Registry
{
private:
	WCHAR ip_address_buffer[AUXIE_REG_IP_BUFFERSIZE] = {};

	LPARAM convertIP_to_LPARAM(LPCTSTR ip);
public:
	~AuxieRegistry();
	AuxieRegistry();

	void initIP();
	void updateIP(LPCWSTR ip);
	void saveIP(LPCWSTR ip);
	LPCWSTR getIP_String();
	LPARAM getIP_LPARAM();
};