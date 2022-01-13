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

	bool CleanUp();

private:

	// Textures 

	SDL_Texture* bg;
	SDL_Texture* bgSky;

	// GUI Button
	GuiButton* btn1;
	GuiButton* btn2;

	float scroller[4];

	SDL_Texture* titleText;
	float titleY; bool titleMove;

	SDL_Texture* pressEnter;

	SDL_Texture* enemyFlying;
	int enemyFlyX = 700, enemyAngle = 0; 

	uint fxEnter;

	float scrollVelocity;

};

#endif // __SCENETITLE_H__