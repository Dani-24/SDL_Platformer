#include "Willycoin.h"
#include "App.h"
#include "Scene.h"
#include "Textures.h"
#include "Module.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Animation.h"
#include "Physics.h"
#include "Player.h"

Coin::Coin(App* application, bool start_enabled) : Module(application, start_enabled){
	name.Create("coin");
}

Coin::~Coin() {

}

bool Coin::Start() {

	LOG("Start Enemy variables");

	// Assets
	CoinTex = app->tex->Load("Assets/textures/willy_coin.png");

	coinFx = app->audio->LoadFx("Assets/audio/fx/coinSound.wav");

	return true;
}

void Coin::AddCoin(int x, int y) {

	LOG("Adding new COIN at X: %d and Y: %d", x, y);

	Coins* thisCoin = new Coins();

	thisCoin->spawn = true;
	thisCoin->body = app->physics->CreateRectangle(x, y, 16, 16);
	thisCoin->position.x = x;
	thisCoin->position.y = y;

	coins.add(thisCoin);
}

bool Coin::PreUpdate() {
	// COIN DEBUG GENERATOR

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		int x, y;
		
		app->input->GetMousePosition(x, y);
	
		if (app->player->position.x > 352 && app->player->position.x < 2880) { // Just Scene 1 limit
			x = x + app->player->position.x - 1280 / 4;
			y = y + app->player->position.y - 720 / 4;
		}
		else {
			x = x + app->player->position.x - 32;
			y = y + app->player->position.y - 720 / 4;
		}

		AddCoin(x, y);
	}

	return true;
}

bool Coin::Update(float dt) {
	
	ListItem<Coins*>* c = coins.start;
	while (c != NULL) {
		if (c->data->spawn == false) {
			LOG("Deleting Coin");
			app->physics->world->DestroyBody(c->data->body->body);
			coins.del(c);
			c = NULL;
		}
		else {
			c = c->next;
		}
	}	
	return true;
}

bool Coin::PostUpdate() {

	ListItem<Coins*>* c = coins.start;
	while (c != NULL) {
		app->render->DrawTexture(CoinTex, c->data->position.x - 8, c->data->position.y - 8);
		c = c->next;
	}
	return true;
}

bool Coin::CleanUp() {

	ListItem<Coins*>* c = coins.start;
	while (c != NULL) {
		app->physics->world->DestroyBody(c->data->body->body);
		c = c->next;
	}
	coins.clear();

	app->tex->UnLoad(CoinTex);
	return true;
}