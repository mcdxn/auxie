#include "MainCommon.h"

PBUTTON GetRootButton(PBUTTON button_list)
{
	while (button_list->prev != NULL)
		button_list = button_list->prev;

	return button_list;
}

int GetButtonCount(PBUTTON button_list)
{
	PBUTTON root = GetRootButton(button_list);
	int count = 0;

	while (root != NULL)
	{
		root = root->next;
		count++;
	}

	return count;
}