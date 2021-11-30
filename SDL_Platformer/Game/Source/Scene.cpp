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
#include "Physics.h"

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


	// Player position for Scene 1
	app->player->position.x = 32;
	app->player->position.y = 1090;
	// Camera at player
	app->render->camera.x = 0 - ((app->player->position.x * 2) - 50);
	app->render->camera.y = 0 - ((app->player->position.y * 2) - 720 / 2);

	// Enable modules q se usan

	app->physics->Enable();
	app->player->Enable();
	app->map->Enable();

	// Load map
	app->map->Load("mapa.tmx");
	app->map->Blocks();

	// Load Assets

	app->audio->PlayMusic("Assets/audio/music/music_bg.ogg");

	background = app->tex->Load("Assets/maps/Background.png");
	sky = app->tex->Load("Assets/maps/Background parts/5 - Sky_color.png");

	dieWindow = app->tex->Load("Assets/textures/dieTexture.png");

	for (int z = 0; z < 11; z++) {
		dieWindowAnim.PushBack({ z * 1280, 0, 1280, 720 });
	}
	dieWindowAnim.loop = false;
	dieWindowAnim.speed = 0.1f;

	int N = -886;
	for (int i = 0; i < 7; i++) {
		bgScrollX[i] = N;
		N += 886;
	}

	// Easter Egg - Press 5 when playing :D
	easterEgg = false;
	loadEgg = false;
	EasterEgg();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	// Lose condition
	if (app->player->position.y > 1280) {
		if (w == NULL && h == NULL) {
			w = -app->render->camera.x / 2 - 1280 / 4;
			h = -app->render->camera.y / 2 - 720 / 4;
		}

		app->player->death = true;
		dieWindowAnim.Update();
	}

	// Add win condition

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Request Load / Save when pressing F6/F5
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	// Camera Movement
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 20;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 20;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 20;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 20;

	// EasterEGG()
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {
		easterEgg = !easterEgg;
		EasterEgg();
	}

	// Back
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneTitle, 10);
	}

	// Death
	if (app->player->death == true) {
		if (cont > 150) {
			app->fade->StartFadeToBlack(this, (Module*)app->sceneEnding, 10);
		}
		else {
			cont++;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->StartFadeToBlack(this, (Module*)app->scene, 10);

	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	for (int i = 0; i < 7; i++) {
		if (bgScrollX[i] <= -(886 * 2)) {
			bgScrollX[i] = (886 * 3) - 1;
		}
		else {
			bgScrollX[i] -= 0.5f;
			app->render->DrawTexture(background, bgScrollX[i], 750);// 50
		}
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 5; j++) {
			app->render->DrawTexture(sky, i * 288, 750 - j * 208);
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

	// Die / Win textures

	SDL_Rect dieWRect = dieWindowAnim.GetCurrentFrame();
	app->render->DrawTexture(dieWindow, w, h, &dieWRect);

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	dieWindowAnim.Reset();

	app->tex->UnLoad(egg);
	app->tex->UnLoad(pandereta);
	app->tex->UnLoad(background);
	app->tex->UnLoad(sky);
	app->tex->UnLoad(dieWindow);

	eggAnim.DeleteAnim();
	panderetAnim.DeleteAnim();
	dieWindowAnim.DeleteAnim();

	app->map->Disable();
	app->player->Disable();
	app->physics->Disable();

	cont = w = h = 0;

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