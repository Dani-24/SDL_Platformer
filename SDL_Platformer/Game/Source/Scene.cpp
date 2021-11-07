#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"
#include "Collider.h"
#include "Collision.h"

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
	// Check if player is enabled:
	if (app->player->active != true) {
		app->player->Enable();
	}

	// Load map
	app->map->Load("hello.tmx");
	app->map->CreateCollisions();
	
	// Load Assets

	app->audio->PlayMusic("Assets/audio/music/music_bg.ogg");

	Background = app->tex->Load("Assets/maps/Background.png");
	bgScrollX[0] = 0; bgScrollX[1] = 886; bgScrollX[2] = 886 * 2; bgScrollX[3] = 886 * 3;
	bgScrollX[4] = 886 * 4; bgScrollX[5] = 886 * 5;

	// Easter Egg - Press 5 when playing :D
	easterEgg = false;
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
    // Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	// Camera Movement
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 20;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 20;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 20;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 20;

	// EasterEGG()
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {
		easterEgg = !easterEgg;
		EasterEgg();
	}

	// Debug
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (app->collision->debug == true) {
			app->collision->debug = false;
		}
		else {
			app->collision->debug = true;
		}
	}

	// Back
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 10);
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	for (int i = 0; i < 5; i++) {
		if (bgScrollX[i] < -886) {
			bgScrollX[i] = 886 *  4;
		}
		else {
			bgScrollX[i] -= 0.5f;
			app->render->DrawTexture(Background, bgScrollX[i], 0);
		}
	}

	if (easterEgg == true) {

		int vecesQSeRepiteEnX = 10;
		int vecesQSeRepiteEnY = 5;

		eggAnim.Update();
		SDL_Rect rect = eggAnim.GetCurrentFrame();

		panderetAnim.Update();
		SDL_Rect rect1 = panderetAnim.GetCurrentFrame();
		for (int i = 0; i < vecesQSeRepiteEnX; i++) {
			for (int j = 0; j < vecesQSeRepiteEnY; j++) {
				if (i % 2 == 0 && j % 2 == 0) {
					app->render->DrawTexture(pandereta, i * 200, j * 200, &rect1);
				}
				else {
					app->render->DrawTexture(egg, i * 200, j * 200, &rect);
				}
			}
		}
	}

	// Draw map
	app->map->Draw();

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width, app->map->mapData.height,
		app->map->mapData.tileWidth, app->map->mapData.tileHeight,
		app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->render->camera.x = 0; app->render->camera.y = 0;
	app->collision->debug = false;

	app->tex->UnLoad(img);
	app->tex->UnLoad(egg);
	app->tex->UnLoad(pandereta);
	app->tex->UnLoad(Background);
	
	eggAnim.DeleteAnim();
	panderetAnim.DeleteAnim();

	app->player->Disable();

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
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
		eggMusic = true;
	}
	else {
		app->audio->PlayMusic("Assets/audio/music/music_bg.ogg");
		eggMusic = false;
	}
}