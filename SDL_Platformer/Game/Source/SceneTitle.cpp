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
	// De momento no hay nada asi q pasa a la scene
	app->fade->StartFadeToBlack(this, (Module*)app->scene, 0);

	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Cleaning Title Scene");

	return true;
}