#include "Text.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "p2SString.h"
#include "j1Gui.h"


Text::Text(int x, int y, const char* _string, _TTF_Font* font) : Ui_element(x, y)
{
	if(font != nullptr)
		graphic = App->fonts->Print(_string, { 255,255,255,255 }, font);
	else
	{
		graphic = App->fonts->Print(_string);
	}
}


Text::~Text()
{

}

void Text::Draw()
{
	App->render->Blit(graphic, position.x, position.y);
}