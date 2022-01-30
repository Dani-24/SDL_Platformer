#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneTitle.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"
#include "Physics.h"
#include "GuiButton.h"
#include "GuiManager.h"

// sus
#include "Enemy.h"
#include "Willycoin.h"

#include "ModuleQFonts.h"
#include "Pathfinder.h"

#include <sstream>
#include <string.h>
using namespace std;

Scene::Scene(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	LOG("Start Scene and load assets");
	
	coins = score = wCoins = 0;

	time = 400000;

	pause = false;

	fxEnter = app->audio->LoadFx("Assets/audio/fx/enter.wav");
	settingsMenu = app->tex->Load("Assets/textures/settings.png");

	//// Delete Save data to disable checkpoint tp if replay the game
	//delSaveData = true;
	//app->SaveGameRequest();

	// Player position for Scene 1
	app->player->initPos.x = initPosX;
	app->player->initPos.y = initPosY;

	// Camera at player
	ResetCamera();

	// Enable modules q se usan

	app->pathfinder->Enable();
	app->physics->Enable();
	app->player->Enable();
	app->map->Enable();
	// Enable is use fonts
	
	app->font->Init();
	app->font->LoadFont("Assets/textures/AmongUs-Regular.ttf");

	app->enemy->Enable();
	app->item->Enable();

	app->guiManager->Enable();

	// Load map
	app->map->Load("mapa.tmx");
	app->map->Blocks();

	// --- Load Assets ---

	// Hud
	hud_bg_texture = app->tex->Load("Assets/textures/sceneHUD.png");
	wCoin1tex = app->tex->Load("Assets/textures/wCoinHUD.png");
	wCoin2tex = app->tex->Load("Assets/textures/wCoinHUD2.png");
	pauseTexture = app->tex->Load("Assets/textures/pause.png");

	Volume = app->sceneTitle->Volume;
	Fx = app->sceneTitle->Fx;

	btn10 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Settings", { 98, 160, 136, 83 }, this);
	btn11 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Back To Title", { 247, 160, 136, 83 }, this);
	btn12 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Exit", { 397, 160, 136, 83 }, this);
	btn13 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "Resume", { 462, 74, 116, 35 }, this);

	btn14 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "TEST", { 332, 86, 43, 42 }, this);
	btn15 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "TEST", { 436, 86, 43, 42 }, this);
	btn16 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "TEST", { 332, 133, 43, 42 }, this);
	btn17 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "TEST", { 436, 133, 43, 42 }, this);
	btn18 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "TEST", { 50, 307, 87, 35 }, this);
	chk3 = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 3, "TEST", { 387, 189, 49, 42 }, this);
	chk4 = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 4, "TEST", { 387, 240, 49, 42 }, this);
	chk4->check = app->sceneTitle->chk2->check;

	timeControl.Start();

	// Background
	app->audio->PlayMusic("Assets/audio/music/music_bg.ogg");

	forestTex = app->tex->Load("Assets/maps/BG/FG.png");
	hillsTex = app->tex->Load("Assets/maps/BG/Hills.png");
	cloudsUpTex = app->tex->Load("Assets/maps/BG/CloudsUp.png");
	cloudsBGTex = app->tex->Load("Assets/maps/BG/CloudsDown.png");
	skyTex = app->tex->Load("Assets/maps/BG/SkyBig.png");

	for (int i = 0; i < Scroller; i++) {
		forestX[i] = hillsX[i] = cloudsDownX[i] = cloudsUpX[i] = i* textureWidth;
	}

	// Die effect
	dieWindow = app->tex->Load("Assets/textures/dieTexture.png");

	for (int z = 0; z < 11; z++) {
		dieWindowAnim.PushBack({ z * 1280, 0, 1280, 720 });
	}
	dieWindowAnim.loop = false;
	dieWindowAnim.speed = 0.2f;

	// Checkpoint

	checkPos.x = 2985;
	checkPos.y = 1073;

	checkPointSensor = app->physics->CreateRectangleSensor(checkPos.x, checkPos.y, 50, 64);

	checkpointTex = app->tex->Load("Assets/textures/checkpoint.png");
	checkPfx = app->audio->LoadFx("Assets/audio/fx/checkpoint.wav");

	for (int check = 0; check < 14; check++) {
		if (check < 7) {
			checkpointAnim.PushBack({ check * 50, 0, 50, 64 });
		}
		else {
			checkpointAnim.PushBack({ (check-7) * 50, 64, 50, 64});
		}
	}
	checkpointAnim.loop = false;
	checkpointAnim.speed = 0.5f;

	// Easter Egg - Press 5 when playing :D
	easterEgg = false;
	loadEgg = false;
	EasterEgg();

	// =============================================
	//				   PathFinding
	// =============================================

	// Check if map loaded, then create the walkability map
	if (app->map->Load("mapa.tmx") == true) {
		int w, h; uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data)) {
			app->pathfinder->SetMap(w, h, data);
		}
		RELEASE_ARRAY(data);
	}
	pathFx = app->audio->LoadFx("Assets/audio/fx/pathfx.wav");

	LOG("Spawn player at X = %d Y = %d", initPosX, initPosY);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	// ====================================
	//        Pathfinding testing
	// ====================================
	if (app->physics->debug == true) {
		int x, y;
		app->input->GetMousePosition(x, y);
		iPoint p = app->render->ScreenToWorld(x, y);
		p = app->map->WorldToMap(p.x, p.y);

		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (originSelected == true)
			{
				app->pathfinder->CreatePath(originPos, p);
				originSelected = false;

				app->audio->PlayFx(pathFx);
			}
			else
			{
				originPos = p;
				originSelected = true;
			}
		}
	}

	// ========================================
	// Request Load / Save when pressing F6/F5
	// ========================================
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN){
		app->LoadGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		delSaveData = false;
		app->SaveGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) {
		delSaveData = true;
		app->SaveGameRequest();
	}

	// ---------------- Camera (Follows player) ----------------------
	if (app->player->death != true && pause == false) {
		if (app->player->position.x > 352 && app->player->position.x < 2880) {
			app->render->camera.x = 0 - ((app->player->position.x * 2) - 1280 / 2);
		}
		if (app->player->position.y < 1114) {
			app->render->camera.y = 0 - ((app->player->position.y * 2) - 720 / 2);
		}
	}

	// ================== Win ====================
	if (app->player->win == true) {
		pause = true;
		app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 60);
	}

	// EasterEGG()
	if (app->input->GetKey(SDL_SCANCODE_7) == KEY_UP) {
		easterEgg = !easterEgg;
		EasterEgg();
	}

	//Settings
	if (settings == true && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		settings = false;
		activeGuiSettings = false;
	}

	// Back
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && settings == false) {
		if (app->scene->pause != true) {
			pause = true;
			LOG("Game Paused");
		}
		else {
			pause = false;
			pauseGui = false;
			LOG("Game UnPaused");
			timeControl.Start();
		}
	}

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (pause == false) {
		time -= timeControl.ReadSec();
	}
	else {
		timeControl.Stop();
	}

	if (time <= 0) {
		app->player->death = true;
	}

	// Death
	if (app->player->death == true) {

		// delete elements from scene:
		if (app->enemy->isEnabled()) {
			app->enemy->Disable();
		}

		// Death scene effect:
		if (cont > 150) {
			app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 10);
		}
		else {
			cont++;
			dieWindowAnim.Update();

			if (w == NULL && h == NULL) {
				w = -app->render->camera.x / 2 - 1280 / 4;
				h = -app->render->camera.y / 2 - 720 / 4;
			}
		}
	}

	// --------------- Go to 1st level --------------------
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->StartFadeToBlack(this, (Module*)app->scene, 10);
	}

	// --------------- Go to 2nd level -------------------- (there isn't 2nd lvl yet :P)
	/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneLevel2, 10);
	}*/

	// --------------- Start from the beginning of the current level -------------------- 
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->fade->StartFadeToBlack(this, (Module*)app->scene, 10);
	}

	// ======================================================
	// BackGround

	if (playerPosForScroll < METERS_TO_PIXELS(app->player->playerBody->body->GetPosition().x)) {
		for (int i = 0; i < Scroller; i++) {
			hillsX[i] -= 0.1f;
			cloudsDownX[i] -= 0.1f;
			forestX[i] -= 0.05f;
		}
	}
	else if (playerPosForScroll > METERS_TO_PIXELS(app->player->playerBody->body->GetPosition().x)) {
		for (int i = 0; i < Scroller; i++) {
			hillsX[i] += 0.1f;
			cloudsDownX[i] += 0.1f;
			forestX[i] += 0.05f;
		}
	}

	playerPosForScroll = METERS_TO_PIXELS(app->player->playerBody->body->GetPosition().x);

	for (int i = 0; i < Scroller; i++) {
		cloudsUpX[i] -= 0.5f;
		cloudsDownX[i] -= 0.3f;

		if (cloudsUpX[i] <= -textureWidth - 1) {
			cloudsUpX[i] = (Scroller - 1) * textureWidth;
		}
		if (forestX[i] <= -textureWidth - 1) {
			forestX[i] = (Scroller - 1) * textureWidth;
		}
		if (hillsX[i] <= -textureWidth - 1) {
			hillsX[i] = (Scroller - 1) * textureWidth;
		}
		if (cloudsDownX[i] <= -textureWidth - 1) {
			cloudsDownX[i] = (Scroller - 1) * textureWidth;
		}
	}

	if (pauseGui == true) {
		if (btn10->state == GuiControlState::DISABLED) {
			btn10->state = GuiControlState::NORMAL;
		}
		if (btn11->state == GuiControlState::DISABLED) {
			btn11->state = GuiControlState::NORMAL;
		}
		if (btn12->state == GuiControlState::DISABLED) {
			btn12->state = GuiControlState::NORMAL;
		}
		if (btn13->state == GuiControlState::DISABLED) {
			btn13->state = GuiControlState::NORMAL;
		}
	}

	if (pauseGui == false) {
		btn10->state = GuiControlState::DISABLED;
		btn11->state = GuiControlState::DISABLED;
		btn12->state = GuiControlState::DISABLED;
		btn13->state = GuiControlState::DISABLED;
	}

	if (activeGuiSettings == true) {
		if (btn14->state == GuiControlState::DISABLED) {
			btn14->state = GuiControlState::NORMAL;
		}
		if (btn14->state == GuiControlState::UNABAILABLE && Volume != 0)
		{
			btn14->state = GuiControlState::NORMAL;
		}
		if ((btn14->state == GuiControlState::NORMAL || btn14->state == GuiControlState::FOCUSED || btn14->state == GuiControlState::PRESSED) && Volume == 0)
		{
			btn14->state = GuiControlState::UNABAILABLE;
		}
		if (btn15->state == GuiControlState::DISABLED) {
			btn15->state = GuiControlState::NORMAL;
		}
		if (btn15->state == GuiControlState::UNABAILABLE && Volume != 128)
		{
			btn15->state = GuiControlState::NORMAL;
		}
		if ((btn15->state == GuiControlState::NORMAL || btn15->state == GuiControlState::FOCUSED || btn15->state == GuiControlState::PRESSED) && Volume == 128)
		{
			btn15->state = GuiControlState::UNABAILABLE;
		}
		if (btn16->state == GuiControlState::DISABLED) {
			btn16->state = GuiControlState::NORMAL;
		}
		if (btn16->state == GuiControlState::UNABAILABLE && Fx != 0)
		{
			btn16->state = GuiControlState::NORMAL;
		}
		if ((btn16->state == GuiControlState::NORMAL || btn16->state == GuiControlState::FOCUSED || btn16->state == GuiControlState::PRESSED) && Fx == 0)
		{
			btn16->state = GuiControlState::UNABAILABLE;
		}
		if (btn17->state == GuiControlState::DISABLED) {
			btn17->state = GuiControlState::NORMAL;
		}
		if (btn17->state == GuiControlState::UNABAILABLE && Fx != 128)
		{
			btn17->state = GuiControlState::NORMAL;
		}
		if ((btn17->state == GuiControlState::NORMAL || btn17->state == GuiControlState::FOCUSED || btn17->state == GuiControlState::PRESSED) && Fx == 128)
		{
			btn17->state = GuiControlState::UNABAILABLE;
		}
		if (btn18->state == GuiControlState::DISABLED) {
			btn18->state = GuiControlState::NORMAL;
		}
		if (chk3->state == GuiControlState::DISABLED) {
			chk3->state = GuiControlState::NORMAL;
		}
		
		if (chk4->state == GuiControlState::DISABLED) {
			chk4->state = GuiControlState::NORMAL;
		}
	}

	if (activeGuiSettings == false) {
		btn14->state = GuiControlState::DISABLED;
		btn15->state = GuiControlState::DISABLED;
		btn16->state = GuiControlState::DISABLED;
		btn17->state = GuiControlState::DISABLED;
		btn18->state = GuiControlState::DISABLED;
		chk3->state = GuiControlState::DISABLED;
		chk4->state = GuiControlState::DISABLED;
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	// Draw BackGround (divided each one bc they kbooom when scroller if we put them on the same for)
	for (int i = 0; i < Scroller; i++) {
		// Sky 
		app->render->DrawTexture(skyTex, i * textureWidth, 600);
		app->render->DrawTexture(skyTex, i * textureWidth, -24);
		app->render->DrawTexture(skyTex, i * textureWidth, -300);
	}

	for (int i = 0; i < Scroller; i++) {
		// Clouds Up
		app->render->DrawTexture(cloudsUpTex, cloudsUpX[i], 500);
		app->render->DrawTexture(cloudsUpTex, cloudsUpX[i], 100);
	}

	for (int i = 0; i < Scroller; i++) {
		// Clouds Down
		app->render->DrawTexture(cloudsBGTex, cloudsDownX[i], 625);
	}

	for (int i = 0; i < Scroller; i++) {
		// Ground
		app->render->DrawTexture(hillsTex, hillsX[i], 650);
	}

	for (int i = 0; i < Scroller; i++) {
		// Forest
		app->render->DrawTexture(forestTex, forestX[i], 675);
	}

	// Draw map
	app->map->Draw();

	// Draw EasterEGG
	if (easterEgg == true) {

		int vecesQSeRepiteEnX = 10;
		int vecesQSeRepiteEnY = 10;

		eggAnim.Update();
		SDL_Rect rectEgg = eggAnim.GetCurrentFrame();
		panderetAnim.Update();
		SDL_Rect rectPand = panderetAnim.GetCurrentFrame();

		for (int i = 0; i < vecesQSeRepiteEnX; i++) {
			for (int j = 0; j < vecesQSeRepiteEnY; j++) {
				if (i % 2 == 0 && j % 2 == 0) {
					app->render->DrawTexture(pandereta, i * 200, j * 200, &rectPand);
				}
				else {
					app->render->DrawTexture(egg, i * 200, j * 200, &rectEgg);
				}
			}
		}
	}

	// Checkpoints
	if (checked == true) {
		checkpointAnim.Update();

		// This will only be played one time:
		if (checkfxPlayed == false) {
			// Play fx
			app->audio->PlayFx(checkPfx);
			checkfxPlayed = true;

			// Save
			delSaveData = false;
			app->SaveGameRequest();
		}
	}

	SDL_Rect rectCheck = checkpointAnim.GetCurrentFrame();
	app->render->DrawTexture(checkpointTex, checkPos.x - 25, checkPos.y - 32, &rectCheck);

	// ========================================
	//             Pathfinding draw
	// ========================================

	if (app->physics->debug == true) {
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);
		iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x, mouseY - app->render->camera.y);

		app->input->GetMousePosition(mouseX, mouseY);
		iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
		p = app->map->WorldToMap(p.x, p.y);
		p = app->map->MapToWorld(p.x, p.y);

		app->render->DrawTexture(app->pathfinder->pathTexture, p.x, p.y);

		const DynArray<iPoint>* path = app->pathfinder->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(app->pathfinder->pathTexture, pos.x, pos.y);
		}

		iPoint originScreen = app->map->MapToWorld(originPos.x, originPos.y);
		app->render->DrawTexture(app->pathfinder->pathOriginTexture, originScreen.x, originScreen.y);
	}

	// =========================================
	//           Die / Win textures
	// =========================================
	if (app->player->death == true) {
		SDL_Rect dieWRect = dieWindowAnim.GetCurrentFrame();
		app->render->DrawTexture(dieWindow, w, h, &dieWRect);
	}

	// =========================================
	//					HUD
	// =========================================

	// ( *Lives at player.cpp )

	hudPos.x = -app->render->camera.x / 2;
	hudPos.y = -app->render->camera.y / 2;

	//bg
	app->render->DrawTexture(hud_bg_texture, hudPos.x, hudPos.y);

	app->font->drawText("LIVES", hudPos.x + 120, hudPos.y + 15, 255, 255, 255);
	
	// coins
	if (contHud >= 0) {
		contHud--;
		app->item->coinSpin.Update();
	}
	else {
		contHud = 100;
		app->item->coinSpin.Reset();
	}
	app->render->DrawTexture(app->item->ItemSprite, hudPos.x + 185, hudPos.y + 17, &app->item->coinSpin.GetCurrentFrame());
	
	// Coins
	sprintf_s(coinText, "%d", coins);	// Text to string

	app->font->drawText(coinText, hudPos.x + 210, hudPos.y + 15, 255, 255, 255);

	// time
	sprintf_s(timeText, "Time: %d", time/1000);

	app->font->drawText(timeText, hudPos.x + 235, hudPos.y + 15, 255, 255, 255);

	// Score
	sprintf_s(scoreText, "Score: %d", score);

	app->font->drawText(scoreText, hudPos.x + 325, hudPos.y + 15, 255, 255, 255);

	// Willycoins
	app->font->drawText("Willycoins: ", hudPos.x + 425, hudPos.y + 15, 255, 255, 255);

	switch (wCoins)
	{
	case 0:
		app->render->DrawTexture(wCoin2tex, hudPos.x + 525, hudPos.y + 11);
		app->render->DrawTexture(wCoin2tex, hudPos.x + 560, hudPos.y + 11);
		app->render->DrawTexture(wCoin2tex, hudPos.x + 595, hudPos.y + 11);
		break;
	case 1:
		app->render->DrawTexture(wCoin1tex, hudPos.x + 525, hudPos.y + 11);
		app->render->DrawTexture(wCoin2tex, hudPos.x + 560, hudPos.y + 11);
		app->render->DrawTexture(wCoin2tex, hudPos.x + 595, hudPos.y + 11);
		break;
	case 2:
		app->render->DrawTexture(wCoin1tex, hudPos.x + 525, hudPos.y + 11);
		app->render->DrawTexture(wCoin1tex, hudPos.x + 560, hudPos.y + 11);
		app->render->DrawTexture(wCoin2tex, hudPos.x + 595, hudPos.y + 11);
		break;
	case 3:
		app->render->DrawTexture(wCoin1tex, hudPos.x + 525, hudPos.y + 11);
		app->render->DrawTexture(wCoin1tex, hudPos.x + 560, hudPos.y + 11);
		app->render->DrawTexture(wCoin1tex, hudPos.x + 595, hudPos.y + 11);
		break;
	}

	if (pause == true && app->player->death == false) {
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		app->render->DrawTexture(pauseTexture, hudPos.x + 15, hudPos.y + 50);
		if (settings == false) {
			pauseGui = true;
		}
		else {
			pauseGui = false;
		}
	}

	//Settings
	if (settings == true) {
		app->render->DrawTexture(settingsMenu, 50, 15);
		activeGuiSettings = true;
	}

	//Draw GUI
	app->guiManager->Draw();
	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control){
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (control->id == 14) //- Volume
		{
			LOG("Click on button 6");
			Volume -= 10;
			if (Volume < 0) {
				Volume = 0;
			}
			app->audio->ChangeVolume(Volume);
		}
		if (control->id == 15) //+ Volume
		{
			LOG("Click on button 7");
			Volume += 10;
			if (Volume > 128) {
				Volume = 128;
			}
			app->audio->ChangeVolume(Volume);
		}
		if (control->id == 16) //- Sound
		{
			LOG("Click on button 8");
			Fx -= 10;
			if (Fx < 0) {
				Fx = 0;
			}
			app->audio->ChangeFXVolume(Fx);
		}
		if (control->id == 17) //+ Sound
		{
			LOG("Click on button 9");
			Fx += 10;
			if (Fx > 128) {
				Fx = 128;
			}
			app->audio->ChangeFXVolume(Fx);
		}
		if (control->id == 18) //Close Settings
		{
			LOG("Click on button Close Settings");
			settings = false;
			activeGuiSettings = false;
		}

		if (control->id == 10) //Settings
		{
			LOG("Click on button Settings");
			settings = true;
		}

		if (control->id == 11) //BackToTitle
		{
			LOG("Click on button Back to title");
			app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 0);
		}

		if (control->id == 12) //Exit
		{
			LOG("Click on button Exit");
			app->sceneTitle->exit = true; // QUIT
			app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 0);
		}

		if (control->id == 13) //Resume
		{
			LOG("Click on button Resume");
			pause = false;
			pauseGui = false;
			LOG("Game UnPaused");
			timeControl.Start();
		}
	}
	//Other cases here
	case GuiControlType::CHECKBOX:
	{
		//Checks the GUI element ID
		if (control->id == 3) //FullScreen
		{
			LOG("Click on check 1");
			if (chk3->check == true) {
				app->win->fullscreen_window = true;
				app->win->ToggleFullscreen();
			}
			else {
				app->win->fullscreen_window = false;
				app->win->ToggleFullscreen();
			}
		}

		if (control->id == 4) //Vsync
		{
			LOG("Click on check 2");
			if (chk4->check == true) {
				app->render->vSyncOn = true;
			}
			else {
				app->render->vSyncOn = false;
			}
		}
	}
	default: 
		break;
	}

	//control->state = GuiControlState::NORMAL;
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	btn10->state = GuiControlState::DISABLED;
	btn11->state = GuiControlState::DISABLED;
	btn12->state = GuiControlState::DISABLED;
	btn13->state = GuiControlState::DISABLED;
	//app->guiManager->Disable();

	app->font->UnloadFont();
	app->font->CleanUp();

	app->audio->PlayMusic(NULL);

	// EasterEGG
	app->tex->UnLoad(egg);
	app->tex->UnLoad(pandereta);

	eggAnim.DeleteAnim();
	panderetAnim.DeleteAnim();

	app->tex->UnLoad(hud_bg_texture);

	// BackGround
	app->tex->UnLoad(forestTex);
	app->tex->UnLoad(hillsTex);
	app->tex->UnLoad(cloudsBGTex);
	app->tex->UnLoad(cloudsUpTex);
	app->tex->UnLoad(skyTex);

	app->tex->UnLoad(checkpointTex);
	checkpointAnim.DeleteAnim();
	app->physics->world->DestroyBody(checkPointSensor->body);

	// Die effect
	dieWindowAnim.DeleteAnim();
	app->tex->UnLoad(dieWindow);

	// Disable modules
	app->map->Disable();
	app->player->Disable();

	if (app->enemy->isEnabled()) {
		app->enemy->Disable();
	}
	app->item->Disable();


	app->physics->Disable();
	app->pathfinder->Disable();

	// Reset Variables
	cont = w = h = playerPosForScroll = checkPfx = pathFx = NULL;
	loadEgg = delSaveData = checked = checkfxPlayed = false;
	for (int a = 0; a < Scroller; a++) {
		forestX[a] = hillsX[a] = cloudsDownX[a] = cloudsUpX[a] = NULL;
	}

	return true;
}

void Scene::ResetCamera() {
	app->render->camera.x = 0 - (app->player->position.x * 2);
	app->render->camera.y = 0 - ((app->player->position.y * 2) - 720 / 2);
}

bool Scene::LoadState(pugi::xml_node& data){
	// --------------- LOAD SAVE DATA -----------------
	LOG("Loading saved data");

	// Score & coins
	coins = data.child("coins").attribute("value").as_int();
	score = data.child("score").attribute("value").as_int();
	wCoins = data.child("willycoins").attribute("value").as_int();
	time = data.child("time").attribute("value").as_int();

	// ====================
	//	  reset player
	// ====================
	app->player->Disable();

	pugi::xml_node pNode = data.child("player");

	app->player->initPos.x = pNode.attribute("x").as_int();
	app->player->initPos.y = pNode.attribute("y").as_int();

	app->player->Enable();

	app->player->HP = pNode.attribute("hp").as_int();

	LOG("Loaded : X: %d, Y: %d, HP: %d", app->player->initPos.x, app->player->initPos.y, app->player->HP);
	// ====================

	// Reset Camera
	app->render->camera.x = 0 - (app->player->position.x * 2);
	if (app->player->position.x > 2880) {
		app->render->camera.x = 0 - ((app->player->position.x * 2) - 1280 / 2);
	}
	app->render->camera.y = 0 - ((app->player->position.y * 2) - (720 / 2) - 180);

	//items
	app->item->Disable();
	app->item->Enable();
	pugi::xml_node iNode = data.child("item");

	for (iNode; iNode.next_sibling("item"); iNode = iNode.next_sibling("item")) {
		int x, y, type;

		x = iNode.attribute("x").as_int();
		y = iNode.attribute("y").as_int();
		type = iNode.attribute("type").as_int();

		app->item->AddItem(x, y, type);
	}

	//for (int i = 0; i < 110; i++) { //Edit the number (110) in case of adding more items
	//	int x, y, type;
	//	x = iNode.attribute("x").as_int();
	//	y = iNode.attribute("y").as_int();
	//	type = iNode.attribute("type").as_int();
	//	app->item->AddItem(x, y, type);
	//	iNode = iNode.next_sibling("item");
	//}

	//enemy
	app->enemy->Disable();
	app->enemy->Enable();
	pugi::xml_node eNode = data.child("enemy");
	for(eNode; eNode.next_sibling("enemy"); eNode = eNode.next_sibling("enemy")){
		int x, y, type;

		x = eNode.attribute("x").as_int();
		y = eNode.attribute("y").as_int();
		type = eNode.attribute("type").as_int();

		app->enemy->AddEnemy(x, y, type);
	}

	return true;
}

bool Scene::SaveState(pugi::xml_node& data) const 
{
	pugi::xml_node pNode = data.append_child("player");
	ListItem<Items*>* d = app->item->items.start;
	ListItem<Enemies*>* c = app->enemy->enemies.start;

	if (delSaveData == false) {
		// ---------------- SAVE DATA -----------------------------
		LOG("Saving data");

		pNode.append_attribute("x") = app->player->position.x;
		pNode.append_attribute("y") = app->player->position.y;
		pNode.append_attribute("hp") = app->player->HP;

		data.append_child("coins").append_attribute("value") = coins;
		data.append_child("score").append_attribute("value") = score;
		data.append_child("willycoins").append_attribute("value") = wCoins;
		data.append_child("time").append_attribute("value") = time;
	}
	else if(delSaveData == true) {
		// ---------------- DELETE SAVE DATA ----------------------
		LOG("Deleting saved data");

		pNode.append_attribute("x") = initPosX;
		pNode.append_attribute("y") = initPosY;
		pNode.append_attribute("hp") = app->player->max_HP;

		data.append_child("coins").append_attribute("value") = 0;
		data.append_child("score").append_attribute("value") = 0;
		data.append_child("willycoins").append_attribute("value") = 0;
		data.append_child("time").append_attribute("value") = 400000;
	}
	
	// ---------------- ITEM SAVE DATA -----------------------------
	while (d != NULL) {
		pugi::xml_node iNode = data.append_child("item");
		iNode.append_attribute("type") = d->data->type;
		iNode.append_attribute("x") = d->data->position.x;
		iNode.append_attribute("y") = d->data->position.y;
		d = d->next;

	}
	// ---------------- ENEMY SAVE DATA -----------------------------
	while (c != NULL) {
		pugi::xml_node aNode = data.append_child("enemy");
		aNode.append_attribute("type") = c->data->type;
		aNode.append_attribute("x") = c->data->position.x;
		aNode.append_attribute("y") = c->data->position.y;
		c = c->next;
	}

	return true;
}

void Scene::EasterEgg() {
	if (loadEgg == false) {
		egg = app->tex->Load("Assets/textures/easteregg.png");
		pandereta = app->tex->Load("Assets/textures/pandereta.png");

		int N = 0;
		for (int i = 0; i < 54; i++) {
			eggAnim.PushBack({ N,0,195,200 });
			N += 195;
		}
		eggAnim.speed = 0.25f;
		eggAnim.loop = true;

		int M = 0;
		for (int i = 0; i < 16; i++) {
			panderetAnim.PushBack({ M,0,200,200 });
			M += 200;
		}
		panderetAnim.speed = 0.25f;
		panderetAnim.loop = true;

		loadEgg = true;
	}
	else if (eggMusic != true) {
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg", 10);
		eggMusic = true;
	}
	else {
		app->audio->PlayMusic("Assets/audio/music/music_bg.ogg", 10);
		eggMusic = false;
	}
}