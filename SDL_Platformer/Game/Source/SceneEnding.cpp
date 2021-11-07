#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneEnding.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"

SceneEnding::SceneEnding(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneEnding");
}

// Destructor
SceneEnding::~SceneEnding()
{}

// Called before render is available
bool SceneEnding::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	// Textures
	bgT = app->tex->Load("Assets/maps/Background parts/Background2.png");
	bgTSky = app->tex->Load("Assets/maps/Background parts/5 - Sky_color.png");
	scroller[0] = 0;
	scroller[1] = 288;
	scroller[2] = 288 * 2;
	scroller[3] = 288 * 3;

	// Audio
	app->audio->PlayMusic("Assets/audio/music/music_title.ogg");

	fxEnter = app->audio->LoadFx("Assets/audio/fx/enter.wav");
	app->render->camera.x = 0; app->render->camera.y = 0;

	return ret;
}

// Called before the first frame
bool SceneEnding::Start()
{
	
	return true;
}

// Called each loop iteration
bool SceneEnding::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneEnding::Update(float dt)
{
	// Back
	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 60);
	//}
	return true;
}

// Called each loop iteration
bool SceneEnding::PostUpdate()
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
		app->render->DrawTexture(bgT, scroller[i], 158);

		// Sky
		app->render->DrawTexture(bgTSky, 288 * i, -50);
	}

	// Game over text
	app->render->DrawTexture(gameOverText, 180, 200);


	return ret;
}

// Called before quitting
bool SceneEnding::CleanUp()
{
	LOG("Freeing scene");

	return true;
}