//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//
#pragma once

#define MAX_BUTTONS 10
#define MAX_AUXIES 10
#define MAX_SOURCES 10
#define MAX_DESTINATIONS 1
#define MAX_BUTTON_NAME 10

#define MAX_ATEM_INPUT_NAME_LONG_STR sizeof(WCHAR)*12
#define MAX_ATEM_INPUT_NAME_SHORT_STR sizeof(WCHAR)*5

struct Input
{
	unsigned int key_onair : 1;
	unsigned int key_id : 1;
	unsigned int output_is_key : 1;
	unsigned int output_is_program : 1;
	unsigned int output_is_aux : 1;
	unsigned int toggle_next : 1;
	unsigned int toggle_position : 1;

	long long atem_input_id = 0;

	WCHAR atem_input_name_long[MAX_ATEM_INPUT_NAME_LONG_STR] = {};
	WCHAR atem_input_name_short[MAX_ATEM_INPUT_NAME_SHORT_STR] = {};
};

struct Auxie
{
	int id_button_owner = 0;
	int dest_count = 0;
	int src_count = 0;
	Input* destination = NULL;
	Input* source_array[MAX_SOURCES] = {};
	~Auxie() 
	{ 
		if (destination != NULL)
			delete destination, destination = nullptr; 
		
		for (int i = 0; i < MAX_SOURCES; i++)
			if (source_array[i] != NULL) 
				delete source_array[i], source_array[i] = nullptr;
			else break;
			
		//delete[] source_array;
	}
};

typedef struct Button
{
	int id = 0;
	WCHAR name[MAX_BUTTON_NAME] = {};
	int child_count = 0;
	Auxie* auxie_array[MAX_AUXIES] = {};
	Button* next = NULL;
	Button* prev = NULL;

	~Button()
	{ 
		
		for (int i = 0; i < MAX_AUXIES; i++)
			if (auxie_array[i] != NULL) 
				delete auxie_array[i], auxie_array[i] = nullptr;
			else break;
			
		
	//delete [] auxie_array;
		if(next) delete next, next = nullptr; 
	};

}BTNELEMENT, *PBTNELEMENT, *PBTNELEMENTLIST;

