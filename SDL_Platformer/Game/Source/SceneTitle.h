#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Module.h"

struct SDL_Texture;

class SceneTitle : public Module
{
public:

	SceneTitle();

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
	SDL_Texture* img;
};

#endif // __SCENETITLE_H__