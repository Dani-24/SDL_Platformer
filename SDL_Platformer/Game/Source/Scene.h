#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

#define Scroller 5

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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void ResetCamera();

	// EasterEgg
	void EasterEgg();

public:
	// Checkpoint
	bool checked = false;
	PhysBody* checkPointSensor;

private:

	// EasterEGG
	Animation eggAnim;
	Animation panderetAnim;
	SDL_Texture* egg;
	SDL_Texture* pandereta;

	// Die effect
	SDL_Texture* dieWindow;
	Animation dieWindowAnim;

	// Background
	SDL_Texture* forestTex;
	SDL_Texture* hillsTex;
	SDL_Texture* cloudsUpTex;
	SDL_Texture* cloudsBGTex;
	SDL_Texture* skyTex;

	float forestX[Scroller], hillsX[Scroller], cloudsDownX[Scroller], cloudsUpX[Scroller];
	int textureWidth = 864;
	int playerPosForScroll = 0;

	// Checkpoint

	SDL_Texture* checkpointTex;
	Animation checkpointAnim;
	iPoint checkPos;
	bool checkfxPlayed = false;
	uint checkPfx = 0;

	// Variables
	int cont = 0;
	int w, h;

	bool easterEgg;
	bool loadEgg, eggMusic;

	// Player Initial Position for Scene 1
	int initPosX = 32, initPosY = 1090;

	bool delSaveData = false, checkPointSave = false;

	// Pathfinding
	iPoint originPos;
	bool originSelected = false;
	uint pathFx;
};

#endif // __SCENE_H__