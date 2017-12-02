#pragma once
#include "Ui_element.h"

//This will also be used to make windows
struct Linked_text;
class Icon : public Ui_element
{
public:
	Icon(int x, int y); //It should recive a pointer to the texture and animations should be set by the creator of the UI element
	~Icon();

	//Detect mouse events on ui element internally
	void OnMouseEvent(UI_EVENT event);
private:
	Animation Idle;
};

