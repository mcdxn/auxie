//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "AuxieRegistry.h"


#define AUXIE_REG_IP_VALUENAME L"ip"

#define AUXIE_REG_DEFAULT_VALUE_STRING ""
#define AUXIE_REG_DEFAULT_VALUE_INT 0

AuxieRegistry::AuxieRegistry()
{
	this->openCurrentUser(AUXIE_REG_KEY);
}

AuxieRegistry::~AuxieRegistry()
{
}

void AuxieRegistry::initIP()
{
	if (!this->isValueExists(AUXIE_REG_IP_VALUENAME))
	{
		this->setStringValue(AUXIE_REG_IP_VALUENAME, this->ip_address_buffer);
	}
	else
	{
		this->getStringValue(AUXIE_REG_IP_VALUENAME, this->ip_address_buffer, AUXIE_REG_IP_BUFFERSIZE);
	}
}

void AuxieRegistry::updateIP(LPCWSTR ip)
{
	Util_copyString(ip, lstrlen(ip), this->ip_address_buffer, AUXIE_REG_IP_BUFFERSIZE);
	this->saveIP(this->ip_address_buffer);
}

LPCWSTR AuxieRegistry::getIP_String()
{
	return this->ip_address_buffer;
}

LPARAM AuxieRegistry::getIP_LPARAM()
{
	return this->convertIP_to_LPARAM(this->ip_address_buffer);
}

void AuxieRegistry::saveIP(LPCWSTR ip)
{
	this->setStringValue(AUXIE_REG_IP_VALUENAME, ip);
}

LPARAM AuxieRegistry::convertIP_to_LPARAM(LPCTSTR ip)
{
	 CHAR field[4] = {};
	 BYTE ip_chunk[4] = {};
	 CHAR cstr[40] = {};
	 WCHAR filter = 0x2E;
	 int location[3] = {};
	 int field_index = 0;
	 int chunk_index = 0;
	 int  count = lstrlenW(ip);
	 int i = 0;
	 int j = 0;
	 int size = 1;

	for (i = 0; i < count; i++)
		wctomb_s(&size, &cstr[i], 1, ip[i]);

	for (i = 0; i < count; i++)
	{
		if (ip[i] == filter)
		{
			location[j] = i;
			j++;
		}
	}

	for (i = 0; i < location[0]; i++)
		field[i] = cstr[i];
	ip_chunk[0] = (BYTE)atoi(field);
	ZeroMemory(field, _countof(field));

	j = 0;
	for (i = location[0] + 1; i < location[1]; i++)
		field[j++] = cstr[i];
	ip_chunk[1] = (BYTE)atoi(field);
	ZeroMemory(field, _countof(field));

	j = 0;
	for (i = location[1] + 1; i < location[2]; i++)
		field[j++] = cstr[i];
	ip_chunk[2] = (BYTE)atoi(field);
	ZeroMemory(field, _countof(field));

	j = 0;
	for (i = location[2] + 1; i < count; i++)
		field[j++] = cstr[i];
	ip_chunk[3] = (BYTE)atoi(field);
	ZeroMemory(field, _countof(field));

	return MAKEIPADDRESS(ip_chunk[0], ip_chunk[1], ip_chunk[2], ip_chunk[3]);
}