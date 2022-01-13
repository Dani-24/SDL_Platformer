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

	titleText = app->tex->Load("Assets/textures/title.png");
	titleY = -200;
	pressEnter = app->tex->Load("Assets/textures/title2.png");

	enemyFlying = app->tex->Load("Assets/textures/enemy.png");

	// Audio
	app->audio->PlayMusic("Assets/audio/music/music_title.ogg");

	fxEnter = app->audio->LoadFx("Assets/audio/fx/enter.wav");
	app->render->camera.x = 0; app->render->camera.y = 0;

	// GUI Buttons
	btn1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Test1", { (app->win->GetWidth() / 2) - 300, app->win->GetWidth() / 10, 160, 40 }, this);
	btn2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Test2", { (app->win->GetWidth() / 2) + 300, app->win->GetWidth() / 10, 160, 40 }, this);


	// ----------- Velocity ------------

	scrollVelocity = 1.0f;

	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	// Go to Scene 
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->audio->PlayFx(fxEnter);
		app->fade->StartFadeToBlack(this, (Module*)app->scene, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
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

	//Draw GUI
	app->guiManager->Draw();

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

	if (titleMove == false) {
		app->render->DrawTexture(pressEnter, 220, titleY + 150);
	}

	return ret;
}

bool SceneTitle::CleanUp()
{
	LOG("Cleaning Title Scene");

	app->tex->UnLoad(bg);
	app->tex->UnLoad(bgSky);
	app->tex->UnLoad(titleText);
	app->tex->UnLoad(pressEnter);

	fxEnter = scrollVelocity = 0;
	enemyFlyX = 700;

	return true;
}