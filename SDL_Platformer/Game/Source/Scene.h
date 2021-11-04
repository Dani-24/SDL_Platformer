#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(App* application, bool start_enabled = true);

	// Destructor
	virtual ~Scene();

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

	// EasterEgg

	void EasterEgg();

	Animation eggAnim;
	Animation panderetAnim;
	SDL_Texture* egg;
	SDL_Texture* pandereta;

	bool easterEgg;

	bool loadEgg, eggMusic;

private:
	SDL_Texture* img;
};

#endif // __SCENE_H__