#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Module.h"

struct SDL_Texture;

class SceneTitle : public Module
{
public:

	SceneTitle(App* application, bool start_enabled = true);

	// Destructor
	virtual ~SceneTitle();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	// Textures 

	SDL_Texture* bg;
	SDL_Texture* bgSky;

	float scroller[4];

	SDL_Texture* titleText;
	float titleY; bool titleMove;

	SDL_Texture* pressEnter;

	SDL_Texture* enemyFlying;
	int enemyFlyX = 700, enemyAngle = 0; 

	uint fxEnter;

};

#endif // __SCENETITLE_H__