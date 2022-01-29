#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

	GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiCheckBox();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	uint focusFx = 0;
	uint pressFx = 0;
	SDL_Texture* checkbox;
	SDL_Texture* checkedbox;
	SDL_Texture* focusbox;
	SDL_Texture* focusedbox;
	bool check = false;
	bool focusSound = false;
	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICHECKBOX_H__