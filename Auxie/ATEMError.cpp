//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#include "ATEMError.h"


void ATEMConnectionError(BMDSwitcherConnectToFailure error)
{
	switch (error)
	{
	case bmdSwitcherConnectToFailureNoResponse:
	{
		ALERT("bmdSwitcherConnectToFailureNoResponse");
	}break;
	case bmdSwitcherConnectToFailureIncompatibleFirmware:
	{
		ALERT("bmdSwitcherConnectToFailureIncompatibleFirmware");
	}break;
	case bmdSwitcherConnectToFailureCorruptData:
	{
		ALERT("bmdSwitcherConnectToFailureCorruptData");
	}break;
	case bmdSwitcherConnectToFailureStateSync:
	{
		ALERT("bmdSwitcherConnectToFailureStateSync");
	}break;
	case bmdSwitcherConnectToFailureStateSyncTimedOut:
	{
		ALERT("bmdSwitcherConnectToFailureStateSyncTimedOut");
	}break;
	default: break;
	}

}