//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once
#include <Windows.h>
#include <WinBase.h>
#include "Util.h"
#include "Common.h"

#define MEM_BUFFERSIZE sizeof(WCHAR)*10
#define MEM_NAME L"LEMARK"
#define MEM_NAME_SIZE sizeof(WCHAR)*10

class AuxieMem {

private:
	HANDLE MemHandle;
	PWSTR Arg1Buffer;
	LPCTSTR MemBufferPointer;
	WCHAR  MemName[MEM_NAME_SIZE] = {};

public:
	AuxieMem();
	~AuxieMem();

	void InitAuxieMem();
	void DestroyAuxieMem();
	int GetValueAuxieMem();
};

class AuxieMemWriter {
private:

public:

	AuxieMemWriter();
	~AuxieMemWriter();


};

