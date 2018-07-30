//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#include "AuxieATEM.h"

AuxieATEM::AuxieATEM()
{
	this->status = NOTCONNECTED;
	this->switcherDiscovery = NULL;
	this->switchOne = NULL;
	this->mixEffectBlock = NULL;
}

AuxieATEM::AuxieATEM(BSTR* connect_to_ip_address)
{
	this->Initialize(connect_to_ip_address);
}

AuxieATEM::~AuxieATEM()
{
	
	CoUninitialize();

	if (this->buttons[0])
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
			delete this->buttons[i];
	}

	if (this->switchOne)
		this->switchOne->Release();

	if (this->mixEffectBlock)
		this->mixEffectBlock->Release();

	if (this->switcherDiscovery)
		this->switcherDiscovery->Release();
}


void AuxieATEM::destroy_atem_inputs()
{
	for (int i = 0; atem_inputs[i] != NULL; i++)
	{
		if (atem_inputs[i]) delete atem_inputs[i];
	}
}

void AuxieATEM::destroy_inputAuxes()
{

	for (int i = 0; i < ATEM_MAX_AUXES;i++)
		if (inputAuxes[i].aux != NULL) 
			inputAuxes[i].aux->Release(), inputAuxes[i].aux = nullptr, inputAuxes[i].id = NULL;
		else break;

}

void AuxieATEM::destroy_inputKeys()
{

	for (int i = 0; i < ATEM_MAX_KEYS; i++)
		if (inputKeys[i].key != NULL)
			inputKeys[i].key->Release(), inputKeys[i].key = nullptr;
		else break;
	
		
}

void AuxieATEM::destroy_inputDSKs()
{

	for (int i = 0; i < ATEM_MAX_DSK; i++)
		if (inputDSKs[i].dsk != NULL) 
			inputDSKs[i].dsk->Release(), inputDSKs[i].dsk = nullptr;
		else break;

}

void AuxieATEM::refresh()
{
	return;
}

void AuxieATEM::refresh(LPVOID button_list)
{
	PBTNELEMENTLIST btn_list = (PBTNELEMENTLIST)button_list;

	this->destroy_inputAuxes();
	this->destroy_inputDSKs();
	this->destroy_inputKeys();

	this->InitializeButtons(btn_list);
	this->InitializeAuxes();
	this->InitializeDSK();
	this->InitializeKeys();

}

void AuxieATEM::InstallDLL()
{
	int selection = MessageBox(NULL,
		L"Looks like the ATEM DLL is not installed.\nI need this file to operate the ATEM Switcher.\nDo you want me to install it?",
		L"Auxie", MB_YESNOCANCEL | MB_ICONQUESTION);

	switch (selection)
	{
	case IDYES:
	{
		BOOL result = Util_runElevated(L"AuxieDLLInstall.exe");
		PostQuitMessage(0);
	}break;
	case IDCANCEL:
	case IDNO:
	default:PostQuitMessage(0);
	}
}

int AuxieATEM::Initialize()
{
	Util_copyString(ATEM_SWITCHER_NAME_DEFAULT, _countof(ATEM_SWITCHER_NAME_DEFAULT), this->switcher_name, ATEM_SWITCHER_NAME_SIZE);

	HRESULT result;

	result = CoInitialize(NULL);

	if (this->switcherDiscovery == NULL)
		result = CoCreateInstance(CLSID_CBMDSwitcherDiscovery, NULL, CLSCTX_ALL, IID_IBMDSwitcherDiscovery, (void**)&this->switcherDiscovery);
	
	if (result == REGDB_E_CLASSNOTREG)
		return 1;

	if (result != S_OK)
		ALERT("switcherDiscovery");

	return 0;
}

void AuxieATEM::Initialize(BSTR* connect_to_ip_address)
{
	this->Initialize();
	this->Connect(connect_to_ip_address);
}

void AuxieATEM::Connect(BSTR* connect_to_ip_address)
{	

	if (this->switcherDiscovery != NULL)
	{
		HRESULT result;
		BMDSwitcherConnectToFailure reasonForFailure;
		result = this->switcherDiscovery->ConnectTo(*connect_to_ip_address, &this->switchOne, &reasonForFailure);
		
		ATEMConnectionError(reasonForFailure);

		if (result == S_OK)
		{
			BSTR switch_name = NULL;
			this->switchOne->GetProductName(&switch_name);
			Util_copyString(switch_name, lstrlenW(switch_name), this->switcher_name, ATEM_SWITCHER_NAME_SIZE);

			IBMDSwitcherMixEffectBlockIterator *mixEffectBlockIterator = NULL;

			result = this->switchOne->CreateIterator(IID_IBMDSwitcherMixEffectBlockIterator, (void**)&mixEffectBlockIterator);

			if (result != S_OK)
				ALERT("mixEffectBlockIterator");

			if (mixEffectBlockIterator)
				while (result = mixEffectBlockIterator->Next(&this->mixEffectBlock) != S_OK);

			this->status = CONNECTED;
		}
		else
		{
			this->status = COULDNOTCONNECT;
		}
	}
	else ALERT("Initialize switcherDiscovery First.");
}


void AuxieATEM::InitializeAuxes()
{
	 long long input_id = 0;
	 LPCWSTR aux_name = NULL;
	 IBMDSwitcherInputAux* inputAux = NULL;
	 IBMDSwitcherInput* input = NULL;

	for (int i = 0; this->atem_inputs[i] != NULL; i++)
	{
		aux_name = this->atem_inputs[i]->atem_input_name_long;
		
		if (lstrcmpiW(aux_name, INPUT_NAME_SHORT_AUX1) == 0 || lstrcmpiW(aux_name, INPUT_NAME_LONG_AUX1) == 0)
		{
			input = this->GetInput(this->atem_inputs[i]->atem_input_id);
			this->inputAuxes[0].id = this->atem_inputs[i]->atem_input_id;
			this->inputAuxes[0].aux = this->GetInputAux(input);
		}
		else if (lstrcmpiW(aux_name, INPUT_NAME_SHORT_AUX2) == 0 || lstrcmpiW(aux_name, INPUT_NAME_LONG_AUX2) == 0)
		{
			input = this->GetInput(this->atem_inputs[i]->atem_input_id);
			this->inputAuxes[1].id = this->atem_inputs[i]->atem_input_id;
			this->inputAuxes[1].aux = this->GetInputAux(input);
		}
		else if (lstrcmpiW(aux_name, INPUT_NAME_SHORT_AUX3) == 0 || lstrcmpiW(aux_name, INPUT_NAME_LONG_AUX3) == 0)
		{
			input = this->GetInput(this->atem_inputs[i]->atem_input_id);
			this->inputAuxes[2].id = this->atem_inputs[i]->atem_input_id;
			this->inputAuxes[2].aux = this->GetInputAux(input);
		}
	}

}

void  AuxieATEM::InitializeKeys()
{
	HRESULT result = S_OK;
	IBMDSwitcherKeyIterator* key_iterator = nullptr;
	IBMDSwitcherKey* available_key = nullptr;

	result = this->mixEffectBlock->CreateIterator(IID_IBMDSwitcherKeyIterator, (void**)&key_iterator);

	if (result != S_OK)
		ALERT("IBMDSwitcherKeyIterator");
	
	int count = 0;
	while (S_OK == key_iterator->Next(&available_key))
	{
		if (available_key)
		{
			if (count < ATEM_MAX_KEYS)
				this->inputKeys[count].key = available_key;
			else
				ALERT("ATEM_MAX_KEYS reached");
		}

		count++;
	}

}

void  AuxieATEM::InitializeDSK()
{
	HRESULT result = S_OK;
	IBMDSwitcherDownstreamKeyIterator* key_iterator = nullptr;
	IBMDSwitcherDownstreamKey* available_key = nullptr;

	result = this->switchOne->CreateIterator(IID_IBMDSwitcherDownstreamKeyIterator, (void**)&key_iterator);

	if (result != S_OK)
		ALERT("IBMDSwitcherDownstreamKeyIterator");

	int count = 0;
	while (S_OK == key_iterator->Next(&available_key))
	{
		if (available_key)
		{
			if (count < ATEM_MAX_DSK)
				this->inputDSKs[count].dsk = available_key;
			else
				ALERT("ATEM_MAX_DSK reached");
		}

		count++;
	}

}

void AuxieATEM::InitializeButtons(PBTNELEMENTLIST button_list)
{
	 PBTNELEMENT root_button = this->GetButtonRoot(button_list);
	 int button_count = this->GetButtonCount(root_button);
	 int each_input = 0;
	 int each_button = 0;
	 int each_auxie = 0;
	 BMDSwitcherInputId input_id;
	 LPCWSTR name = NULL;
	 LPCWSTR name_short_src = NULL;
	 LPWSTR name_short_des = NULL;

	 for (each_button = 0; each_button < button_count; each_button++)
	{
		for (each_auxie = 0; each_auxie < root_button->child_count; each_auxie++)
		{
			
			for (each_input = 0; each_input < root_button->auxie_array[each_auxie]->dest_count; each_input++)
			{
				if (root_button->auxie_array[each_auxie]->destination->output_is_key == TRUE)
					continue;

				name = root_button->auxie_array[each_auxie]->destination->atem_input_name_long;
				
				input_id = this->GetInputID(name);
				
				root_button->auxie_array[each_auxie]->destination->atem_input_id = input_id;
				
				name_short_src = this->GetInputShortName(input_id);
				name_short_des = root_button->auxie_array[each_auxie]->destination->atem_input_name_short;
				Util_copyString(name_short_src, lstrlenW(name_short_src), name_short_des, MAX_ATEM_INPUT_NAME_SHORT_STR);

				
					if (lstrcmpiW(name, INPUT_NAME_SHORT_PROGRAM) == 0 || lstrcmpiW(name, INPUT_NAME_LONG_PROGRAM) == 0)
						root_button->auxie_array[each_auxie]->destination->output_is_program = TRUE;
					else
					if (lstrcmpiW(name, INPUT_NAME_SHORT_AUX1) == 0 || lstrcmpiW(name, INPUT_NAME_LONG_AUX1) == 0)
						root_button->auxie_array[each_auxie]->destination->output_is_aux = TRUE;
					else
					if (lstrcmpiW(name, INPUT_NAME_SHORT_AUX2) == 0 || lstrcmpiW(name, INPUT_NAME_LONG_AUX2) == 0)
						root_button->auxie_array[each_auxie]->destination->output_is_aux = TRUE;
					else
					if (lstrcmpiW(name, INPUT_NAME_SHORT_AUX3) == 0 || lstrcmpiW(name, INPUT_NAME_LONG_AUX3) == 0)
						root_button->auxie_array[each_auxie]->destination->output_is_aux = TRUE;

				name = NULL;
				name_short_src = NULL;
				name_short_des = NULL;
				input_id = NULL;
			}

			for (each_input = 0; each_input < root_button->auxie_array[each_auxie]->src_count; each_input++)
			{
				if (root_button->auxie_array[each_auxie]->source_array[each_input]->output_is_key == TRUE)
					continue;

				name = root_button->auxie_array[each_auxie]->source_array[each_input]->atem_input_name_long;
				
				input_id = this->GetInputID(name);
				
				root_button->auxie_array[each_auxie]->source_array[each_input]->atem_input_id = input_id;
				
				name_short_src = this->GetInputShortName(input_id);
				name_short_des = root_button->auxie_array[each_auxie]->source_array[each_input]->atem_input_name_short;
				Util_copyString(name_short_src, lstrlenW(name_short_src), name_short_des, MAX_ATEM_INPUT_NAME_SHORT_STR);
				
				name = NULL;
				name_short_src = NULL;
				name_short_des = NULL;
				input_id = NULL;
			}
		}

		this->buttons[each_button] = root_button;
		root_button = root_button->next;
	}


}

void AuxieATEM::InitializeInputs()
{
	 HRESULT result;
	 IBMDSwitcherInput* input = NULL;
	 IBMDSwitcherInputIterator* inputIterator = NULL;
	 PWCHAR dest = NULL;
	 DWORD input_index = 0;
	 const wchar_t* inputNameLong = NULL;
	 const wchar_t* inputNameShort = NULL;
	
	result = switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&inputIterator);
	if (result != S_OK)
		ALERT("inputIterator");

	while (S_OK == inputIterator->Next(&input))
	{
		BMDSwitcherInputId id;
		BSTR name_long;
		BSTR name_short;

		this->atem_inputs[input_index] = new Input;
		result = input->GetInputId(&id);
		this->atem_inputs[input_index]->atem_input_id = id;
		
		result = input->GetString(bmdSwitcherInputPropertyIdLongName, &name_long);
		inputNameLong = _bstr_t(name_long);
		dest = this->atem_inputs[input_index]->atem_input_name_long;
		Util_copyString(inputNameLong, lstrlenW(inputNameLong), dest, MAX_ATEM_INPUT_NAME_LONG_STR);

		result = input->GetString(bmdSwitcherInputPropertyIdShortName, &name_short);
		inputNameShort = _bstr_t(name_short);
		dest = this->atem_inputs[input_index]->atem_input_name_short;
		Util_copyString(inputNameShort, lstrlenW(inputNameShort), dest, MAX_ATEM_INPUT_NAME_SHORT_STR);

		inputNameLong = NULL;
		inputNameShort = NULL;
		input->Release();

		input_index++;
	}

	input_index = 0;
}

ConnectionStatus AuxieATEM::GetConnectionStatus()
{
	return this->status;
}

LPCWSTR AuxieATEM::GetSwitcherName()
{
	return this->switcher_name;
}

LPCWSTR AuxieATEM::GetInputShortName(BMDSwitcherInputId input_id)
{
	LPCWSTR short_name = NULL;
	BMDSwitcherInputId id = 0;

	for (int i = 0; this->atem_inputs[i] != NULL; i++)
	{
		id = this->atem_inputs[i]->atem_input_id;
		if (input_id == id)
		{
			short_name = this->atem_inputs[i]->atem_input_name_short;
			return short_name;
		}
	}
}

LPCWSTR AuxieATEM::GetInputLongName(BMDSwitcherInputId input_id)
{
	LPCWSTR long_name = NULL;
	BMDSwitcherInputId id = 0;

	for (int i = 0; this->atem_inputs[i] != NULL; i++)
	{
		id = this->atem_inputs[i]->atem_input_id;
		if (input_id == id)
		{
			long_name = this->atem_inputs[i]->atem_input_name_long;
			return long_name;
		}
	}
}

BMDSwitcherInputId AuxieATEM::GetInputID(LPCWSTR input_name)
{
	BMDSwitcherInputId input_id = 0;
	LPCWSTR atem_input_name = NULL;

	for (int i = 0; this->atem_inputs[i] != NULL; i++)
	{
		atem_input_name = this->atem_inputs[i]->atem_input_name_long;
		if (lstrcmpiW(atem_input_name, input_name) == NULL)
		{
			input_id = this->atem_inputs[i]->atem_input_id;
			return input_id;
		}
	}

	return input_id;
}

IBMDSwitcherInput* AuxieATEM::GetInput(BMDSwitcherInputId input_id)
{
	IBMDSwitcherInput* input = NULL;
	IBMDSwitcherInputIterator* iterator = NULL;
	BMDSwitcherInputId id = 0;

	HRESULT result;

	result = this->switchOne->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&iterator);

	while (S_OK == iterator->Next(&input))
	{
		result = input->GetInputId(&id);

		if (id == input_id)
			goto done;

		input->Release();
	}

done:
	{
		if (iterator)
			iterator->Release();

		return input;
	}
}

IBMDSwitcherInputAux* AuxieATEM::GetInputAux(IBMDSwitcherInput* aux_input)
{
	IBMDSwitcherInputAux* aux = NULL;
	HRESULT result = S_OK;
	result = aux_input->QueryInterface(IID_IBMDSwitcherInputAux, (void**)&aux);

	return aux;
}

PBTNELEMENTLIST AuxieATEM::GetButtonRoot(PBTNELEMENTLIST button_list)
{
	while (button_list->prev != NULL)
		button_list = button_list->prev;

	return button_list;
}

int AuxieATEM::GetButtonCount(PBTNELEMENTLIST button_list)
{
	PBTNELEMENT root = this->GetButtonRoot(button_list);
	int count = 0;

	while (root != NULL)
	{
		root = root->next;
		count++;
	}

	return count;
}

void AuxieATEM::PerformSwitch(int button_id)
{
	 long long dest_id = 0;
	 long long src_id = 0;
	 int src_current = 0;
	 int src_next = 0;
	 int src_count = 0;

	 PBTNELEMENT current_button = this->buttons[button_id - 1];

	int child_count = current_button->child_count;
	if (child_count > MAX_BUTTONS)
		child_count = MAX_BUTTONS;

	for (int current_index = 0; current_index < child_count; current_index++)
	{
		src_count = current_button->auxie_array[current_index]->src_count;

		for (src_current = 0; src_current < src_count; src_current++)
		{
			if (current_button->auxie_array[current_index]->source_array[src_current]->toggle_next == TRUE)
			{
				src_next = (src_current+1);

				if (src_next < src_count)
				{
					current_button->auxie_array[current_index]->source_array[src_next]->toggle_next = TRUE;
					current_button->auxie_array[current_index]->source_array[src_current]->toggle_next = FALSE;
				}
				else if (src_next == src_count)
				{

					current_button->auxie_array[current_index]->source_array[src_current]->toggle_next = FALSE;
					current_button->auxie_array[current_index]->source_array[0]->toggle_next = TRUE;
				}

				if (current_button->auxie_array[current_index]->destination->output_is_key)
				{
					src_id = current_button->auxie_array[current_index]->source_array[src_current]->key_id;
					this->PerformToggleKey(src_id, TRUE, current_button->auxie_array[current_index]->source_array[src_current]->key_onair);
					return;
				}
				else
				{
					src_id = current_button->auxie_array[current_index]->source_array[src_current]->atem_input_id;
					goto SWITCHNORMAL;
				}
			}
		}

	SWITCHNORMAL:
		if (current_button->auxie_array[current_index]->destination->output_is_program)
		{
			this->PerformSwitchToProgram(src_id);
			return;
		}
		else if (current_button->auxie_array[current_index]->destination->output_is_aux)
		{
			dest_id = current_button->auxie_array[current_index]->destination->atem_input_id;
			this->PerformSwitchToAux(src_id, dest_id);
			return;
		}

	}
}

void AuxieATEM::PerformSwitchToProgram(BMDSwitcherInputId source_id)
{
	this->mixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, source_id);
}

void AuxieATEM::PerformSwitchToAux(BMDSwitcherInputId source_id, BMDSwitcherInputId destination_id)
{
	if (this->inputAuxes[0].id == destination_id)
		this->inputAuxes[0].aux->SetInputSource(source_id);

	else if (this->inputAuxes[1].id == destination_id)
		this->inputAuxes[1].aux->SetInputSource(source_id);

	else if (this->inputAuxes[2].id == destination_id)
		this->inputAuxes[2].aux->SetInputSource(source_id);
}

void AuxieATEM::PerformToggleKey(int key_index, BOOL override, BOOL on)
{
	HRESULT result = S_OK;
	BOOL is_onair = FALSE;

	if (key_index < 0)
		key_index = 0;

	if (key_index > ATEM_MAX_KEYS)
		key_index = ATEM_MAX_KEYS;

	if (override)
	{
		result = this->inputKeys[key_index - 1].key->SetOnAir(on);
		return;
	}


	if (key_index <= ATEM_MAX_KEYS)
	{
		result = this->inputKeys[key_index - 1].key->GetOnAir(&is_onair);

		if (is_onair)
		{
			result = this->inputKeys[key_index-1].key->SetOnAir(0);
		}
		else
		{
			result = this->inputKeys[key_index - 1].key->SetOnAir(1);
		}
	}
}

void AuxieATEM::PerformToggleDSK(int key_index, BOOL override, BOOL on)
{
	HRESULT result = S_OK;
	BOOL is_onair = FALSE;

	if (key_index < 0)
		key_index = 0;

	if (key_index > ATEM_MAX_DSK)
		key_index = ATEM_MAX_DSK;

	if (override)
	{
		result = this->inputDSKs[key_index - 1].dsk->SetOnAir(on);
		return;
	}

	if (key_index <= ATEM_MAX_DSK)
	{
		result = this->inputDSKs[key_index - 1].dsk->GetOnAir(&is_onair);

		if (is_onair)
		{
			result = this->inputDSKs[key_index - 1].dsk->SetOnAir(FALSE);
		}
		else
		{
			result = this->inputDSKs[key_index - 1].dsk->SetOnAir(TRUE);
		}
	}
}