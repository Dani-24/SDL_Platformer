#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Map.h"
#include "ModuleQFonts.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	focusFx = app->audio->LoadFx("Assets/audio/fx/focusButton.wav");
	pressFx = app->audio->LoadFx("Assets/audio/fx/pressButton.wav");
	app->font->LoadFont("Assets/textures/AmongUs-Regular.ttf");
	canClick = true;
	drawBasic = false;
}

GuiButton::~GuiButton()
{

}
//app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN
bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED && state != GuiControlState::UNABAILABLE)
	{
		// L14: TODO 3: Update the state of the GUiButton according to the mouse position
		// Get mouse position
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;
			if (focusSound == false) {
				app->audio->PlayFx(focusFx);
				focusSound = true;
			}
			
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				state = GuiControlState::PRESSED;
				app->audio->PlayFx(pressFx);
				NotifyObserver();
			}

			// If mouse button pressed -> Generate event!
			/*if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				NotifyObserver();
			}*/
		}
		else { 
			state = GuiControlState::NORMAL; 
			focusSound = false;
		}
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{

	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED: 
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
	} break; 

	case GuiControlState::UNABAILABLE:
	{
		render->DrawRectangle(bounds, 0, 0, 0, 80);
	} break;

	case GuiControlState::NORMAL:
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
		//app->font->drawText(text, bounds.x, bounds.y, 0, 0, 0);

	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		render->DrawRectangle(bounds, 255, 255, 255, 80);
		//app->font->drawText(text, bounds.x, bounds.y, 0, 0, 0);
	} break;
	case GuiControlState::PRESSED:
	{
		render->DrawRectangle(bounds, 146, 76, 36, 255);
		//app->font->drawText(text, bounds.x, bounds.y, 0, 0, 0);
	} break;

	/******/

	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	return false;
}