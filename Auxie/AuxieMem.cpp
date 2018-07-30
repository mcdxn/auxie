//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "AuxieMem.h"

AuxieMem::AuxieMem()
{
	this->MemHandle = NULL;
	this->Arg1Buffer = NULL;
	this->MemBufferPointer = NULL;
	Util_copyString(MEM_NAME,lstrlenW(MEM_NAME), this->MemName,MEM_NAME_SIZE);
	this->InitAuxieMem();
}

AuxieMem::~AuxieMem()
{
	this->DestroyAuxieMem();
}

void AuxieMem::InitAuxieMem()
{
	this->MemHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEM_BUFFERSIZE, this->MemName);
	
	if (!this->MemHandle)
		goto Error;
	
	goto Success;

Error: 
	ALERT("MemInit");

Success:		
	this->Arg1Buffer = (PWSTR)MapViewOfFile(this->MemHandle, FILE_MAP_ALL_ACCESS, 0, 0, MEM_BUFFERSIZE);
}

void AuxieMem::DestroyAuxieMem()
{
	if (this->MemHandle)
	{
		UnmapViewOfFile(this->Arg1Buffer);
		CloseHandle(this->MemHandle);
	}
}

int AuxieMem::GetValueAuxieMem()
{
	return (_wtoi(this->Arg1Buffer));
}