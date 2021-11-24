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
	bg = app->tex->Load("Assets/maps/Background parts/_Complete_static_BG_(288 x 208).png");
	bgSky = app->tex->Load("Assets/maps/Background parts/5 - Sky_color.png");
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

	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	// Movimiento del titulo
	if (titleY < 50) {
		titleY += 1;
		titleMove = true;
	}
	else {
		titleMove = false;
	}

	// pasar de escena cuando se pare el titulo si pulsas enter
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
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	// --- Draw stuff ---

	// Background

	for (int i = 0; i < 4; i++) {
		if (scroller[i] <= -288) {
			scroller[i] = 288 * 3;
		}
		else {
			scroller[i] -= 0.5f;
		}
		app->render->DrawTexture(bg, scroller[i], 158);

		// Sky
		app->render->DrawTexture(bgSky, 288 * i, -50);
	}

	// BAck enemy
	if (enemyFlyX < -250) {
		enemyFlyX = 700;
	}
	else {
		enemyFlyX--;
	}
	enemyAngle++;
	app->render->DrawTexture(enemyFlying, enemyFlyX, 100, 0, 1, enemyAngle);

	// Title
	app->render->DrawTexture(titleText, 180, titleY);

	if (titleMove == false) {
		app->render->DrawTexture(pressEnter, 220, titleY + 150);
	}

	return ret;
}

// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Cleaning Title Scene");

	app->tex->UnLoad(bg);
	app->tex->UnLoad(bgSky);
	app->tex->UnLoad(titleText);
	app->tex->UnLoad(pressEnter);

	fxEnter = 0;

	return true;
}