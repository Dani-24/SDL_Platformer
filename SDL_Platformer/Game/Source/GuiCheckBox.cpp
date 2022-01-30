#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Map.h"
#include "ModuleQFonts.h"
#include "Textures.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	focusFx = app->audio->LoadFx("Assets/audio/fx/focusButton.wav");
	pressFx = app->audio->LoadFx("Assets/audio/fx/pressButton.wav");
	checkbox = app->tex->Load("Assets/textures/checkbox.png");
	checkedbox = app->tex->Load("Assets/textures/checkedbox.png");
	focusbox = app->tex->Load("Assets/textures/focusbox.png");
	focusedbox = app->tex->Load("Assets/textures/focusedbox.png");
	canClick = true;
	drawBasic = false;
}

GuiCheckBox::~GuiCheckBox()
{

}
//app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN
bool GuiCheckBox::Update(float dt)
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
				if (check == true) {
					check = false;
				}
				else {
					check = true;
				}
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

bool GuiCheckBox::Draw(Render* render)
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
		if(check == false){
			app->render->DrawTexture(checkbox, bounds.x, bounds.y);
		}
		else {
			app->render->DrawTexture(checkedbox, bounds.x, bounds.y);
		}
	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		if (check == false) {
			app->render->DrawTexture(focusbox, bounds.x, bounds.y);
		}
		else {
			app->render->DrawTexture(focusedbox, bounds.x, bounds.y);
		}
	} break;
	case GuiControlState::PRESSED:
	{
		if (check == false) {
			app->render->DrawTexture(checkbox, bounds.x, bounds.y);
		}
		else {
			app->render->DrawTexture(checkedbox, bounds.x, bounds.y);
		}
	} break;

	/******/

	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	return false;
}