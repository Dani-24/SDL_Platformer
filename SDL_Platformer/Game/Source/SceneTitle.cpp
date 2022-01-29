#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneTitle.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "ModuleQFonts.h"
#include "Map.h"

SceneTitle::SceneTitle(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneTitle");
}

// Destructor
SceneTitle::~SceneTitle()
{}

// Called before render is available
bool SceneTitle::Awake()
{
	LOG("Loading Title Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneTitle::Start()
{
	// Textures

	bg = app->tex->Load("Assets/maps/BG/BG1.png");
	bgSky = app->tex->Load("Assets/maps/BG/Sky.png");
	scroller[0] = 0;
	scroller[1] = 288;
	scroller[2] = 288 * 2;
	scroller[3] = 288 * 3;

	settingsMenu = app->tex->Load("Assets/textures/settings.png");
	titleText = app->tex->Load("Assets/textures/title.png");
	titleY = -200;
	pressEnter = app->tex->Load("Assets/textures/titleButtons.png");

	enemyFlying = app->tex->Load("Assets/textures/enemy.png");

	// Audio
	app->audio->PlayMusic("Assets/audio/music/music_title.ogg");

	fxEnter = app->audio->LoadFx("Assets/audio/fx/enter.wav");
	app->render->camera.x = 0; app->render->camera.y = 0;

	app->font->Init();
	app->guiManager->Enable();
	// GUI Buttons
	btn1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "TEST", { 80, 275, 83, 51 }, this);
	btn2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "TEST", { 178, 275, 83, 51 }, this);
	btn3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "TEST", { 280, 275, 83, 51 }, this);
	btn4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "TEST", { 384, 275, 83, 51 }, this);
	btn5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "TEST", { 485, 275, 83, 51 }, this);

	// ----------- Velocity ------------

	scrollVelocity = 1.0f;

	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	// Go to Scene 
	if (settings == true && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		settings = false;
	}

	if (exit == true) {
		return false; // QUIT
	}

	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	// Applying dt directly bugged the scene so we change the velocity depending if the game runs at 60 or 30 fps (no more framerates are possible)
	if (dt < 30) {
		scrollVelocity = 1.0f;
	}
	else {
		scrollVelocity = 2.0f;
	}

	// Title movement
	if (titleY < 50) {
		titleY += scrollVelocity;
		titleMove = true;
	}
	else {
		titleMove = false;
	}

	// Background
	for (int i = 0; i < 4; i++) {
		if (scroller[i] < -286) {
			scroller[i] = 288 * 3;
		}
		else {
			scroller[i] -= scrollVelocity / 2;
		}
	}

	// Scrolling enemy
	if (enemyFlyX < -250) {
		enemyFlyX = 700;
	}
	else {
		enemyFlyX -= scrollVelocity;
	}
	enemyAngle += scrollVelocity;

	if (activeGui == true) {
		if (btn1->state == GuiControlState::DISABLED) {
			btn1->state = GuiControlState::NORMAL;
		}
		if (btn2->state == GuiControlState::DISABLED) {
			btn2->state = GuiControlState::UNABAILABLE;
		}
		if (btn3->state == GuiControlState::DISABLED) {
			btn3->state = GuiControlState::NORMAL;
		}
		if (btn4->state == GuiControlState::DISABLED) {
			btn4->state = GuiControlState::NORMAL;
		}
		if (btn5->state == GuiControlState::DISABLED) {
			btn5->state = GuiControlState::NORMAL;
		}
	}

	if (activeGui == false) {
		btn1->state = GuiControlState::DISABLED;
		btn2->state = GuiControlState::DISABLED;
		btn3->state = GuiControlState::DISABLED;
		btn4->state = GuiControlState::DISABLED;
		btn5->state = GuiControlState::DISABLED;
	}
	
	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	// --- Draw stuff ---

	// Background

	for (int i = 0; i < 4; i++) {
		// Sky
		app->render->DrawTexture(bgSky, 288 * i, -50);

		// BG
		app->render->DrawTexture(bg, scroller[i], 158);
	}

	// Scrolling enemy
	
	app->render->DrawTexture(enemyFlying, enemyFlyX, 100, 0, 1, enemyAngle);

	// Title
	app->render->DrawTexture(titleText, 180, titleY);

	//Title buttons
	if (titleMove == false) {
		app->render->DrawTexture(pressEnter, 80, 275);
	}

	//Settings
	if (settings == true) {
		app->render->DrawTexture(settingsMenu, 50, 15);
	}

	//Active Title Gui
	if (titleMove == false && settings == false) {
		activeGui = true;
	}
	else {
		activeGui = false;
	}

	//Draw GUI
		app->guiManager->Draw();
	
	return ret;
}

bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (control->id == 1) //Play
		{
			LOG("Click on button 1");
			app->audio->PlayFx(fxEnter);
			app->fade->StartFadeToBlack(this, (Module*)app->scene, 0);
		}

		if (control->id == 2) //Continue
		{
			LOG("Click on button 2");
		}
		
		if (control->id == 3) //Settings
		{
			LOG("Click on button 3");
			settings = true;
		}

		if (control->id == 4) //Credits
		{
			LOG("Click on button 4");
		}

		if (control->id == 5) //Exit
		{
			LOG("Click on button 5");
			exit = true; // QUIT
		}


	}
	//Other cases here

	default: break;
	}

	//control->state = GuiControlState::NORMAL;
	return true;
}

bool SceneTitle::CleanUp()
{
	LOG("Cleaning Title Scene");

	btn1->state = GuiControlState::DISABLED;
	btn2->state = GuiControlState::DISABLED;
	btn3->state = GuiControlState::DISABLED;
	btn4->state = GuiControlState::DISABLED;
	btn5->state = GuiControlState::DISABLED;
	app->tex->UnLoad(bg);
	app->tex->UnLoad(bgSky);
	app->tex->UnLoad(titleText);
	app->tex->UnLoad(pressEnter);
	app->font->UnloadFont();

	fxEnter = scrollVelocity = 0;
	enemyFlyX = 700;

	return true;
}