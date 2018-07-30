//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "AuxieXMLParser.h"

PBTNELEMENT AuxieXMLParser::button = nullptr;

AuxieXMLParser::~AuxieXMLParser()
{}

AuxieXMLParser::AuxieXMLParser()
{}

AuxieXMLParser::AuxieXMLParser(LPCWSTR filename)
{
	this->Initialize(filename);
	AuxieXMLParser::initButtons();
}

int AuxieXMLParser::getCountButtons()
{
	return this->CountElement(ELEMENT_BUTTON);
}

void  AuxieXMLParser::initButtons()
{
	//
	// THIS IS WHERE THE MADNESS BEGINS: Part 1 :))
	//
	// This procedure can be broken up into a few different pieces each processing different levels
	//

	AuxieXMLParser::destroyButtons();

	AuxieXMLParser::button = new Button;
	PBTNELEMENT root = button;
	int button_element_count = this->CountElement(ELEMENT_BUTTON);
	int attribute_count = 0;
	int button_id = 0;
	int auxie_id = 0;

	// LEVEL 1: Process Button Elements
	for (int i = 0; i < button_element_count; i++)
	{
		this->GotoElement(ELEMENT_BUTTON, ++button_id);

		button->id = Util_getIntValue(this->ThenGetAttributeValue(AT_ID, FALSE, 0));

		LPCWSTR button_name = this->ThenGetAttributeValue(AT_NAME, FALSE, 0);

		if (button_name != NULL)
			Util_copyString(button_name, lstrlen(button_name), button->name, MAX_BUTTON_NAME);

		button->child_count = this->ThenCountChildElements(ELEMENT_BUTTON, button->id, ELEMENT_AUXIE);

		if (button->child_count > MAX_AUXIES)
			button->child_count = MAX_AUXIES;

		UINT child_attribute_count = 0;
		UINT child_src_count = 0;
		UINT child_dest_count = 0;

		BOOL is_dest_keyer = FALSE;

		// LEVEL 2: Process Auxie Elements
		for (int x = 0; x < button->child_count; x++)
		{
			button->auxie_array[x] = new Auxie;
			button->auxie_array[x]->id_button_owner = button->id;

			this->GotoChildElement(ELEMENT_BUTTON, button->id, ELEMENT_AUXIE, x + 1);
			child_attribute_count = this->ThenCountAttributes();

			child_dest_count = this->ThenCountAttributeOccurances(AT_DEST, FALSE);
			if (child_dest_count > MAX_DESTINATIONS) child_dest_count = MAX_DESTINATIONS;

			child_src_count = this->ThenCountAttributeOccurances(AT_SRC, TRUE);
			if (child_src_count > MAX_SOURCES) child_src_count = MAX_SOURCES;

			// LEVEL 3: Process Input Elements
			button->auxie_array[x]->dest_count = child_dest_count;
			button->auxie_array[x]->src_count = child_src_count;
			button->auxie_array[x]->destination = new Input;

			// Process Destination Input Element
			for (int y = 0; y < child_dest_count; y++)
			{
				button->auxie_array[x]->destination->toggle_position = 0;

				LPWSTR name = button->auxie_array[x]->destination->atem_input_name_long;
				LPCWSTR value = this->ThenGetAttributeValue(AT_DEST, FALSE, 0);

				// Check if destination is KEYER - if NOT - process normal.
				if (is_dest_keyer = isKey(value))
				{
					// Process Key
					Util_copyString(value, lstrlenW(value), name, MAX_ATEM_INPUT_NAME_LONG_STR);
					name = button->auxie_array[x]->destination->atem_input_name_short;
					Util_copyString(value, lstrlenW(value), name, MAX_ATEM_INPUT_NAME_SHORT_STR);

					button->auxie_array[x]->destination->output_is_key = TRUE;
					button->auxie_array[x]->destination->key_id = Util_extractNumberFromString(value);
					button->auxie_array[x]->destination->key_onair = FALSE;
				}
				else
				{
					// Process Normal
					Util_copyString(value, lstrlenW(value), name, MAX_ATEM_INPUT_NAME_LONG_STR);
				}
			}

			int toggle_position_count = 0;

			// Process Source Input Elements
			for (int y = 0; y < child_src_count; y++)
			{
				button->auxie_array[x]->source_array[y] = new Input;
				button->auxie_array[x]->source_array[y]->toggle_position = ++toggle_position_count;

				if (button->auxie_array[x]->source_array[y]->toggle_position == TRUE)
					button->auxie_array[x]->source_array[y]->toggle_next = TRUE;
				else
					button->auxie_array[x]->source_array[y]->toggle_next = FALSE;

				LPWSTR name = button->auxie_array[x]->source_array[y]->atem_input_name_long;
				LPCWSTR value = this->ThenGetAttributeValue(AT_SRC, TRUE, toggle_position_count);

				// IF DESTINATION KEYER - PROCESS KEY SOURCES- ELSE PROCEED NORMAL PROCESSING
				if (is_dest_keyer)
				{
					int is_on = Util_extractNumberFromString(value);

					button->auxie_array[x]->source_array[y]->output_is_key = button->auxie_array[x]->destination->output_is_key;

					button->auxie_array[x]->source_array[y]->key_id = button->auxie_array[x]->destination->key_id;

					if (is_on == 0)
						button->auxie_array[x]->source_array[y]->key_onair = FALSE;
					else if (is_on == 1)
						button->auxie_array[x]->source_array[y]->key_onair = TRUE;

				}
				else
				{
					Util_copyString(value, lstrlenW(value), name, MAX_ATEM_INPUT_NAME_LONG_STR);
				}
			}
		}

		auxie_id = 0;

		if ((i + 1) < button_element_count) // Stops from creating extra Button
		{
			button->next = new Button;
			button->next->prev = button;
			button = button->next;
		}
	}

}

void AuxieXMLParser::destroyButtons()
{
	if (AuxieXMLParser::button != NULL)
		delete  AuxieXMLParser::button;
}

PBTNELEMENTLIST AuxieXMLParser::getButtons()
{
	return AuxieXMLParser::button;
}

BOOL AuxieXMLParser::isKey(LPCWSTR name)
{
    LPCWSTR INPUT_NAME_KEYS[] =
	{
		INPUT_NAME_LONG_KEY1,
		INPUT_NAME_SHORT_KEY1,
		INPUT_NAME_LONG_KEY2,
		INPUT_NAME_SHORT_KEY2,
		INPUT_NAME_LONG_KEY3,
		INPUT_NAME_SHORT_KEY3,
		INPUT_NAME_LONG_KEY4,
		'\0'
	};

	for (int i = 0; INPUT_NAME_KEYS[i] != NULL; i++)
	{
		if (lstrcmpiW(name, INPUT_NAME_KEYS[i]) == 0)
			return TRUE;
	}
	
	return FALSE;
}
