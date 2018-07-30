//
//	
//	Created by Lemark S.
//	Copyright (c) 2015 Mr. McByte. All rights reserved.
//
//

#include "PushButtonList.h"

int PushButtonList::size = 0;

PushButtonList::~PushButtonList()
{
	if (this->button)
		delete this->button;
}

PushButtonList::PushButtonList()
{
	this->button = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
}

PushButtonList::PushButtonList(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height)
{
	++PushButtonList::size;

	this->initButtons(button_id, label, instance_handle, window_handle, x_pos, y_pos, width, height);
}

void PushButtonList::initButtons(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle, int x_pos, int y_pos, int width, int height)
{
	this->prev = nullptr;
	this->next = nullptr;
	this->button = new PushButton(button_id,label,instance_handle,window_handle,x_pos,y_pos,width,height);
}

HBRUSH PushButtonList::drawButtons(UINT button_id)
{
	PushButtonList* head = nullptr;
	head = this;

	while (head != nullptr)
	{
		if (head->button->getID() == button_id)
			return head->button->drawButton();
		else
			head = head->next;
	}
}

void  PushButtonList::setButtonsClicked(UINT button_id)
{
	this->clearButtonsClickStateToUnClicked();

	PushButtonList* head = nullptr;
	head = this;

	while (head != nullptr)
	{
		if (head->button->getID() == button_id)
		{
			head->button->setButtonClicked();
			return;
		}
		else
			head = head->next;
	}
}

void  PushButtonList::setButtonsUnClicked(UINT button_id)
{
	PushButtonList* head = nullptr;
	head = this;

	while (head != nullptr)
	{
		if (head->button->getID() == button_id)
		{
			head->button->setButtonUnClicked();
			return;
		}
		else
			head = head->next;
	}
}

void PushButtonList::clearButtonsClickStateToUnClicked()
{
	PushButtonList* head = nullptr;
	head = this;

	while (head != nullptr)
	{
		if (head->button->isButtonClick())
			head->button->setButtonUnClicked();
		else
			head = head->next;
	}
}

void PushButtonList::hideButtons()
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->hideButton();
			head = head->next;
		}
	}
}

void PushButtonList::showButtons()
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->showButton();
			head = head->next;
		}
	}
}

void PushButtonList::addButton(UINT button_id, LPCWSTR label, HINSTANCE instance_handle, HWND window_handle)
{
	if (this)
	{
		PushButtonList* empty = this->getNextEmpty();

		PushButtonList* insert = new PushButtonList(button_id, label, instance_handle, window_handle,
			empty->button->getPositionEnd().x + PUSHBUTTONLIST_MARGIN,
			empty->button->getPositionEnd().y,
			empty->button->getSize().cx,
			empty->button->getSize().cy);

		empty->next = insert;
		insert->prev = empty;
	}
}

PushButtonList* PushButtonList::getNextEmpty()
{
	PushButtonList* head = this;
		
	while (head != nullptr)
	{
		if (head->next == nullptr)
		{
			return head;
		}
		else
			head = head->next;
	}

}

PushButtonList* PushButtonList::getHead()
{
	return this;
}

PushButtonList* PushButtonList::getTail()
{
	PushButtonList* head = this;

	while (head)
	{
		head = head->next;
		if (head->next == nullptr)
			return head;
	}

}

void PushButtonList::disableButton(UINT button_id)
{
	if (this)
	{
		PushButtonList* node = this;

		while (node != nullptr)
		{
			if (node->button != nullptr)
			{
				if (node->button->getID() == button_id)
				{
					node->button->disableButton();
					return;
				}
				
			}

			if (node)
				node = node->next;
		}
	}
}

void PushButtonList::delButton(UINT button_id)
{
	if (this)
	{
		PushButtonList* node = this;
	
		while (node != nullptr)
		{
			if (node->button->getID() == button_id)
			{
				
				if (node->prev == nullptr)
				{
					// Current node is head node					
					delete node->button, node->button = nullptr;

					if (node->next)
						this->compactButtonList(node);

					return;
				}
				else if (node->next == nullptr)
				{
					// Current node is end node
					node->prev->next = nullptr;
					delete node;

					return;
				}

				delete node->button, node->button = nullptr;

				this->compactButtonList(node);

				return;
			}

			if (node)
				node = node->next;
		}
	}
}

void PushButtonList::delAllButtons()
{
	if (this)
	{
		PushButtonList* tail = nullptr;
		tail = this->getTail();
		
		while (tail != nullptr)
		{
			if (tail->next != nullptr)
			{
				delete tail->next;
				tail->next = nullptr;
			}

			tail = tail->prev;
		}

		delete this->button, this->button = nullptr;
	}
}

void PushButtonList::compactButtonList(PushButtonList* button_list)
{
	while (button_list != nullptr)
	{
		if (button_list->next)
		{
			button_list->button = button_list->next->button;
		}

		button_list = button_list->next;
	}

	PushButtonList* tail = this->getTail();
	tail->button = nullptr;
	if (tail->prev)
	{
		tail->prev->next = nullptr;
		delete tail;
	}
}

void PushButtonList::setButtonsStyleNormal()
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setStyleNormal();
			head = head->next;
		}
	}
}

void PushButtonList::setButtonsStyleAbnormal()
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setStyleAbnormal();
			head = head->next;
		}
	}
}

void PushButtonList::setButtonsSize(int width, int height)
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setSize(width,height);
			head = head->next;
		}
	}
}

void PushButtonList::setButtonsPosition(int x, int y)
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setPosition(x, y);
			head = head->next;
		}
	}
}

void PushButtonList::setButtonsColor(BYTE r, BYTE g, BYTE b)
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setColor(r, g, b);
			head = head->next;
		}
	}
}

void PushButtonList::setButtonsClickedColor(BYTE r, BYTE g, BYTE b)
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setColorClicked(r, g, b);
			head = head->next;
		}
	}
}

void PushButtonList::setButtonsTextColor(BYTE r, BYTE g, BYTE b)
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setTextColor(r, b, g);
			head = head->next;
		}
	}
}

void PushButtonList::setButtonsTextSize(int text_size)
{
	if (this)
	{
		PushButtonList* head = nullptr;
		head = this;

		while (head != nullptr)
		{
			head->button->setFontSize(text_size);
			head = head->next;
		}
	}
}

int PushButtonList::getListCount()
{
	int count = 0;

	PushButtonList* head = nullptr;
	head = this->getHead();

	while (head != nullptr)
	{
		++count;
		if (head->next == nullptr)
			return count;
		else
			head = head->next;
	}

}

SIZE PushButtonList::getButtonsTotalWindowArea()
{
	SIZE size = this->button->getSize();
	int buttons_count = this->getListCount();

	size.cx *= buttons_count;
	size.cx += (PUSHBUTTONLIST_MARGIN*buttons_count) + PUSHBUTTONLIST_MARGIN;
	size.cy += PUSHBUTTONLIST_MARGIN*2;

	return size;
}