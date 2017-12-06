#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "ClickManager.h"
#include "j1Collisions.h"
//This structure contains a pointer to a text and its offset. Its only use is to be passed to the icon or button constructor in order to have a text
//linked to it


j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	click_manager = new ClickManager();
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	background_file_name = conf.child("menu_background").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	menu_background = App->tex->Load(background_file_name.GetString());


	return true;
}

// Update of all ui_elements and click_manager will be executed here
bool j1Gui::PreUpdate()
{
	//Update all windows
	for (uint i = 0; i < windows.count(); ++i)
	{
		if (windows[i] != nullptr && windows[i]->active)
			windows[i]->Update();
	}
	//Update all icons (Maybe they should be able to blit from their own texture like texts)
	for (uint i = 0; i < icons.count(); ++i)
	{
		if (icons[i] != nullptr && icons[i]->active)
			icons[i]->Update();
	}
	//Update all buttons
	for (uint i = 0; i < buttons.count(); ++i)
	{
		if (buttons[i] != nullptr && buttons[i]->active)
			buttons[i]->Update();
	}
	//Update all texts
	for (uint i = 0; i < texts.count(); ++i)
	{
		if (texts[i] != nullptr && texts[i]->active)
			texts[i]->Update();
	}
	click_manager->Update();
	
	return true;
}

// Draw of all ui_elements and background will be executed here
bool j1Gui::PostUpdate()
{
		if(blit_background)
		App->render->Blit(menu_background, 0, 0,1,false);
	
		//Blit all windows
		for (uint i = 0; i < windows.count(); ++i)
		{
			if (windows[i] != nullptr && windows[i]->active)
				windows[i]->Draw(atlas);
		}
		//Blit all icons (Maybe they should be able to blit from their own texture like texts)
		for (uint i = 0; i < icons.count(); ++i)
		{
			if (icons[i] != nullptr && icons[i]->active)
				icons[i]->Draw(atlas);
		}
		//Blit all buttons
		for (uint i = 0; i < buttons.count(); ++i)
		{
			if (buttons[i] != nullptr && buttons[i]->active)
				buttons[i]->Draw(atlas);
		}
		//Blit all texts
		for (uint i = 0; i < texts.count(); ++i)
		{
			if (texts[i] != nullptr && texts[i]->active)
				texts[i]->Draw();
		}


	if(App->collision->debug)
	click_manager->DebugDraw();

	return true;
}

Icon* j1Gui::Add_icon(int x, int y)
{
	Icon* new_ui_element = nullptr;

	new_ui_element = new Icon(x, y);
	icons.add(new_ui_element);

	return new_ui_element;
}

Button* j1Gui::Add_button(int x, int y, j1Module* _listener, BUTTON_TYPE button_type)
{
	Button* new_ui_element = nullptr;

	new_ui_element = new Button(x, y,_listener,button_type);
	buttons.add(new_ui_element);

	return new_ui_element;
}

Text* j1Gui::Add_text(int x, int y, const char* text, _TTF_Font* font)
{
	Text* new_text = new Text(x, y, text, font);
	texts.add(new_text);

	return new_text;
}
Window* j1Gui::Add_window(int x, int y)
{
	Window* new_window = new Window(x, y);
	windows.add(new_window);

	return new_window;
}

//This method will iterate over all the colliders of the buttons in the "buttons" list, looking for the one that has the same collider that the one given to the 
//function. Then it will call its "OnClick" method
void j1Gui::OnMouseEvent_caller(Ui_collider* c1, UI_EVENT event)
{
	for (int i = 0; i < buttons.count(); i++)
	{
		if(buttons[i] != nullptr)
		{
			if (buttons[i]->collider == c1)
			{
				buttons[i]->listener->OnMouseEvent(event, buttons[i]);
				buttons[i]->OnMouseEvent(event);
			}
		}
	}
	for (int i = 0; i < windows.count(); i++)
	{
		if (windows[i] != nullptr)
		{
			if (windows[i]->collider == c1)
			{
				windows[i]->OnMouseEvent(event);
			}
		}
	}
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (int i = 0; i < icons.count(); i++)
	{
		delete icons[i];
		icons[i] = nullptr;
	}
	for (int i = 0; i < buttons.count(); i++)
	{
		delete buttons[i];
		buttons[i] = nullptr;
	}
	for (int i = 0; i < texts.count(); i++)
	{
		delete texts[i];
		texts[i] = nullptr;
	}
	for (int i = 0; i < windows.count(); i++)
	{
		delete windows[i];
		windows[i] = nullptr;
	}
	click_manager->Cleanup();//Free all ui_colliders
	delete click_manager;
	click_manager = nullptr;
	return true;
}

bool j1Gui::Erase_Ui_element(Ui_element* element)
{
	if (element == nullptr)
		return false;

	switch (element->type) {
		case ICON:
		{
			int index = icons.find((Icon*)element);

			if (index == -1)
				return false;

			p2List_item<Icon*>* node_to_delete = icons.At(index);

			for (int i = 0; i < icons[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(icons[index]->linked_elements[i]);
			}
			
			delete icons[index];
			icons.del(node_to_delete);
			return true;
		}
		case BUTTON:
		{
			int index = buttons.find((Button*)element);

			if (index == -1)
				return false;

			p2List_item<Button*>* node_to_delete = buttons.At(index);

			for (int i = 0; i < buttons[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(buttons[index]->linked_elements[i]);
			}


			click_manager->Erase_ui_collider(buttons[index]->collider);

			delete buttons[index];
			buttons.del(node_to_delete);
			return true;
		}
		case TEXT:
		{
			int index = texts.find((Text*)element);

			if (index == -1)
				return false;

			p2List_item<Text*>* node_to_delete = texts.At(index);

			for (int i = 0; i < texts[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(texts[index]->linked_elements[i]);
			}

			delete texts[index];
			texts.del(node_to_delete);
			return true;
		}
		case WINDOW:
		{
			int index = windows.find((Window*)element);

			if (index == -1)
				return false;

			p2List_item<Window*>* node_to_delete = windows.At(index);

			for (int i = 0; i < windows[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(windows[index]->linked_elements[i]);
			}

			click_manager->Erase_ui_collider(windows[index]->collider);

			delete windows[index];
			windows.del(node_to_delete);
			return true;
		}
	}

	return false;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

