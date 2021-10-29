#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
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
	// L03: DONE: Load map
	app->map->Load("hello.tmx");
	app->audio->PlayMusic("Assets/audio/music/music_bg.ogg");



	// Easter Egg - Press 5 when playing :D
	loadEgg = false;
	EasterEgg();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 2;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 2;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 2;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 2;

	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {
		easterEgg = !easterEgg;
		EasterEgg();
	}

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->map->Draw();

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (easterEgg == true) {
		eggAnim.Update();
		SDL_Rect rect = eggAnim.GetCurrentFrame();
		app->render->DrawTexture(egg, 0, 0, &rect);
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Scene::EasterEgg() {
	if (loadEgg == false) {
		egg = app->tex->Load("Assets/textures/easteregg.png");
		int N = 0;
		for (int i = 0; i < 54; i++) {
			eggAnim.PushBack({ N,0,195,200 });
			N += 195;
		}
		eggAnim.speed = 0.25f;
		eggAnim.loop = true;

		loadEgg = true;
	}
	else if (eggMusic != true) {
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
		eggMusic = true;
	}
	else {
		app->audio->PlayMusic("Assets/audio/music/music_bg.ogg");
		eggMusic = false;
	}
}