#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(App* application, bool start_enabled = true);

	virtual ~Scene();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	/*bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;*/

	// EasterEgg
	void EasterEgg();

private:
	Animation eggAnim;
	Animation panderetAnim;
	SDL_Texture* egg;
	SDL_Texture* pandereta;
	SDL_Texture* background;
	SDL_Texture* sky;

	SDL_Texture* dieWindow;
	Animation dieWindowAnim;

	int cont = 0;
	int w, h;

	float bgScrollX[6];
	bool easterEgg;
	bool loadEgg, eggMusic;
};

#endif // __SCENE_H__