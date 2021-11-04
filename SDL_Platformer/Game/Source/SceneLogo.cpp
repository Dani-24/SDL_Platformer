#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"

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

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing scene");

	return true;
}