#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneEnding : public Module
{
public:

	SceneEnding(App* application, bool start_enabled = true);

	// Destructor
	virtual ~SceneEnding();

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

	SDL_Texture* bgT;
	SDL_Texture* bgTSky;

	float scroller[4];

	SDL_Texture* gameOverText;

	uint fxEnter;
};

#endif // __SCENEENDING_H__