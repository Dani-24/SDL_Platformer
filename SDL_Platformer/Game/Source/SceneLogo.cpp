#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

SceneLogo::SceneLogo(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo()
{}

// Called before render is available
bool SceneLogo::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLogo::Start()
{
	// Textures
	count = 0;
	logo = app->tex->Load("Assets/textures/logo.png");
	
	// Audio
	app->audio->PlayMusic("Assets/audio/music/music_logo.ogg");
	fxEnter = app->audio->LoadFx("Assets/audio/fx/enter.wav");

	return true;
}

// Called each loop iteration
bool SceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLogo::Update(float dt)
{
	if (count > 100) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 0);
	}
	else {
		count++;
		LOG("Count : %d", count);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP) {
		return false;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->audio->PlayFx(fxEnter);
		app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 0);
		LOG("aaaaa");
	}

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(logo, 0, 0);
	fxEnter = 0;

	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing best logo ever scene");

	app->tex->UnLoad(logo);

	return true;
}