//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#pragma once
#include <Windows.h>
#include <comutil.h>

#include "BMDSwitcherAPI_h.h"
#include "Common.h"
#include "AuxieXMLElements.h"
#include "AuxieATEMInputNames.h"
#include "ATEMError.h"
#include "Util.h"


#define ATEM_SWITCH_FROM_DEAD_BUTTON 1
#define ATEM_SWITCH_FROM_LIVE_BUTTON 2

#define ATEM_SWITCHER_NAME_DEFAULT L""
#define ATEM_SWITCHER_NAME_SIZE sizeof(WCHAR)*20
#define ATEM_MAX_INPUTS 30
#define ATEM_MAX_AUXES 3 
#define ATEM_MAX_KEYS 4
#define ATEM_MAX_DSK 4

#define ATEM_MAX_RECONNECT_TRY 0
static ConnectionStatus CONNECTION_STATUS;
static WCHAR ConnectionStatusMessage[][50] = {
	{ L"STATUS: Not Connected" },
	{ L"STATUS: Connected" },
	{ L"STATUS: Disconnected" },
	{ L"STATUS: Could Not Connect" },
};

typedef struct _AUX_INPUT {
	BMDSwitcherInputId id = NULL;
	IBMDSwitcherInputAux* aux = NULL;
}AUX_INPUT, *PAUX_INPUT;

typedef struct _KEY_INPUT
{
	IBMDSwitcherKey* key = NULL;
}KEY_INPUT, *PKEY_INPUT;

typedef struct _DSK_INPUT
{
	IBMDSwitcherDownstreamKey* dsk = NULL;
}DSK_INPUT,*PDSK_INPUT;


class AuxieATEM : public _Refresh
{
private:
	IBMDSwitcherDiscovery* switcherDiscovery;
	IBMDSwitcher* switchOne;
	IBMDSwitcherMixEffectBlock *mixEffectBlock;

	WCHAR switcher_name[ATEM_SWITCHER_NAME_SIZE] = {};
	
	Input* atem_inputs[ATEM_MAX_INPUTS] = {};

	AUX_INPUT inputAuxes[ATEM_MAX_AUXES] = {};
	KEY_INPUT inputKeys[ATEM_MAX_KEYS] = {};
	DSK_INPUT inputDSKs[ATEM_MAX_DSK] = {};

	PBTNELEMENT buttons[MAX_BUTTONS] = {};
	
	ConnectionStatus status;

	void destroy_atem_inputs();
	void destroy_inputAuxes();
	void destroy_inputKeys();
	void destroy_inputDSKs();

	void PerformSwitchToAux(BMDSwitcherInputId source_id, BMDSwitcherInputId destination_id);
	void PerformSwitchToProgram(BMDSwitcherInputId source_id);
	void PerformToggleKey(int key_index=0, BOOL override=FALSE, BOOL on=FALSE);
	void PerformToggleDSK(int key_index=0, BOOL override=FALSE, BOOL on=FALSE);

public:

	AuxieATEM();
	AuxieATEM(BSTR* connect_to_ip_address);
	~AuxieATEM();

	int Initialize();
	void Initialize(BSTR* connect_to_ip_address);

	void InitializeInputs();
	void InitializeAuxes();
	void InitializeKeys();

	void  InitializeDSK();

	void Connect(BSTR* connect_to_ip_address);
	
	void InstallDLL();

	void refresh();
	void refresh(LPVOID button_list);
	
	void InitializeButtons(PBTNELEMENTLIST button_list);
	PBTNELEMENTLIST GetButtonRoot(PBTNELEMENTLIST button_list);
	int GetButtonCount(PBTNELEMENTLIST button_list);

	LPCWSTR GetSwitcherName();
	LPCWSTR GetInputShortName(BMDSwitcherInputId input_id);
	LPCWSTR GetInputLongName(BMDSwitcherInputId input_id);
	BMDSwitcherInputId GetInputID(LPCWSTR input_name);
	IBMDSwitcherInput* GetInput(BMDSwitcherInputId input_id);
	IBMDSwitcherInputAux* GetInputAux(IBMDSwitcherInput* aux_input);
	ConnectionStatus GetConnectionStatus();

	void PerformSwitch(int button_id);
	
	
};