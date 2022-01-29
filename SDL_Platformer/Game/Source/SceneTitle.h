#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Module.h"
#include "GuiButton.h"

struct SDL_Texture;

class SceneTitle : public Module
{
public:

	SceneTitle(App* application, bool start_enabled = true);

	// Destructor
	virtual ~SceneTitle();

	bool Awake();
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool CleanUp();

private:

	// Textures 

	SDL_Texture* bg;
	SDL_Texture* bgSky;

	// GUI Button
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;
	GuiButton* btn5;

	float scroller[4];

	SDL_Texture* titleText;
	float titleY; bool titleMove;

	SDL_Texture* pressEnter;

	SDL_Texture* settingsMenu;

	SDL_Texture* enemyFlying;
	int enemyFlyX = 700, enemyAngle = 0; 

	bool activeGui = false, settings = false;
	bool exit = false;

	uint fxEnter;

	float scrollVelocity;

};

#endif // __SCENETITLE_H__