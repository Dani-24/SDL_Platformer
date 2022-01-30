#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"

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

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	// EasterEgg
	void EasterEgg();

public:

	bool pause;

	bool continueButton = false;

	// Checkpoint
	bool checked = false;
	PhysBody* checkPointSensor;

	// Public GUI info
	int coins, score, wCoins;
	int time;

private:

	uint fxEnter;
	// HUD
	SDL_Texture* hud_bg_texture, *wCoin1tex, *wCoin2tex, *pauseTexture;
	iPoint hudPos;
	int contHud = 100;
	char coinText[100], scoreText[100], timeText[100];
	Timer timeControl;

	SDL_Texture* livesSprite;
	Animation liveOn, liveOff;

	// PAUSE GUI
	GuiButton* btn10;
	GuiButton* btn11;
	GuiButton* btn12;
	GuiButton* btn13;

	// Settings GUI

	GuiButton* btn14;
	GuiButton* btn15;
	GuiButton* btn16;
	GuiButton* btn17;
	GuiButton* btn18;
	GuiCheckBox* chk3;
	GuiCheckBox* chk4;

	bool settings = false;
	bool activeGuiSettings = false;
	bool pauseGui = false;
	bool exit = false;

	int Volume, Fx;

	SDL_Texture* settingsMenu;

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

	bool delSaveData = false;

	// Pathfinding
	iPoint originPos;
	bool originSelected = false;
	uint pathFx;

	uint loadFx, deleteSaveFx;
};

#endif // __SCENE_H__