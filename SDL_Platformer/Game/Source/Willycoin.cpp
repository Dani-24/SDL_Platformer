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

#include <time.h>    

Item::Item(App* application, bool start_enabled) : Module(application, start_enabled){
	name.Create("coin");
}

Item::~Item() {

}

bool Item::Start() {

	LOG("Start Enemy variables");

	// Assets
	Willycoin = app->tex->Load("Assets/textures/willy_coin.png");
	ItemSprite = app->tex->Load("Assets/textures/coins_sprite.png");

	coinFx = app->audio->LoadFx("Assets/audio/fx/coinSound.wav");
	willyFx = app->audio->LoadFx("Assets/audio/fx/willy.wav");

	//Animations

	coinIdle.PushBack({ 16,16,16,16 });

	coinSpin.PushBack({ 16,16,16,16 });
	coinSpin.PushBack({ 32,16,16,16 });
	coinSpin.PushBack({ 48,16,16,16 });
	coinSpin.PushBack({ 64,16,16,16 });
	coinSpin.PushBack({ 80,16,16,16 });
	coinSpin.PushBack({ 96,16,16,16 });
	coinSpin.PushBack({ 16,16,16,16 });
	coinSpin.loop = false;
	coinSpin.speed = 0.05f;

	coinFlash.PushBack({ 16,112,16,16 });
	coinFlash.PushBack({ 32,112,16,16 });
	coinFlash.PushBack({ 48,112,16,16 });
	coinFlash.PushBack({ 64,112,16,16 });
	coinFlash.PushBack({ 80,112,16,16 });
	coinFlash.PushBack({ 96,112,16,16 });
	coinFlash.PushBack({ 16,112,16,16 });
	coinFlash.loop = false;
	coinFlash.speed = 0.05f;

	lollipop.PushBack({ 480,592,16,16 });
	candy.PushBack({ 496,592,16,16 });
	chocolate.PushBack({ 512,592,16,16 });
	can.PushBack({ 544,592,16,16 });
	iceCream1.PushBack({ 560,592,16,16 });
	iceCream2.PushBack({ 576,592,16,16 });
	cherry.PushBack({ 736,592,16,16 });
	orange.PushBack({ 752,592,16,16 });
	banana.PushBack({ 768,592,16,16 });
	strawberry.PushBack({ 784,592,16,16 });
	apple.PushBack({ 800,592,16,16 });



	return true;
}

void Item::AddItem(int x, int y, int type) {

	LOG("Adding new COIN at X: %d and Y: %d", x, y);

	Items* thisCoin = new Items();

	switch (type)
	{
	case 1:
		thisCoin->spawn = true;
		thisCoin->type = 1;
		thisCoin->sprite = ItemSprite;
		thisCoin->body = app->physics->CreateRectangleSensor(x, y, 16, 16);
		thisCoin->currentAnimation = &coinIdle;
		thisCoin->position.x = x;
		thisCoin->position.y = y;
		break;

	case 2:
		thisCoin->spawn = true;
		thisCoin->type = 2;
		thisCoin->body = app->physics->CreateRectangleSensor(x, y, 64, 64);
		thisCoin->position.x = x;
		thisCoin->position.y = y;
		break;
	case 3:
		thisCoin->spawn = true;
		thisCoin->type = 3;
		thisCoin->sprite = ItemSprite;
		thisCoin->body = app->physics->CreateRectangleSensor(x, y, 16, 16);
		thisCoin->currentAnimation = &lollipop;
		thisCoin->position.x = x;
		thisCoin->position.y = y;
		break;
	}


	items.add(thisCoin);
}

bool Item::PreUpdate() {
	// COIN DEBUG GENERATOR

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		int x, y;
		int	type = 1;
		
		app->input->GetMousePosition(x, y);
	
		if (app->player->position.x > 352 && app->player->position.x < 2880) { // Just Scene 1 limit
			x = x + app->player->position.x - 1280 / 4;
			y = y + app->player->position.y - 720 / 4;
		}
		else {
			x = x + app->player->position.x - 32;
			y = y + app->player->position.y - 720 / 4;
		}

		AddItem(x, y, type);
	}

	return true;
}

bool Item::Update(float dt) {
	
	ListItem<Items*>* c = items.start;
	srand(time(NULL));
	int anim = rand() % 2 + 1;
	while (c != NULL) {
		switch (c->data->type)
		{
		case 1:
			if (c->data->spawn == false) {
				LOG("Deleting Coin");
				app->physics->world->DestroyBody(c->data->body->body);
				items.del(c);
				c = NULL;
			}
			else {
				if (c->data->cont >= 100) {
					if (anim == 1)
					{
						coinFlash.Reset();
						c->data->currentAnimation = &coinFlash;
					}
					if (anim == 2)
					{
						coinSpin.Reset();
						c->data->currentAnimation = &coinSpin;
					}
					c->data->cont = 0;
				}
				c->data->currentAnimation->Update();
				c->data->cont += 1;
				c = c->next;

			}
			break;
		case 2:
			if (c->data->spawn == false) {
				LOG("Deleting Coin");
				app->physics->world->DestroyBody(c->data->body->body);
				items.del(c);
				c = NULL;
			}
			else {
				c = c->next;
			}
			break;
		case 3:
			if (c->data->spawn == false) {
				LOG("Deleting Item");
				app->physics->world->DestroyBody(c->data->body->body);
				items.del(c);
				c = NULL;
			}
			else {
				c = c->next;
			}
			break;
		}

	}	
	return true;
}

bool Item::PostUpdate() {

	if (app->player->death != true && app->player->win != true) {
		ListItem<Items*>* c = items.start;

		while (c != NULL) {
			switch (c->data->type)
			{
			case 1:
				SDL_Rect rect = c->data->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(c->data->sprite, c->data->position.x - 8, c->data->position.y - 8, &rect);
				c = c->next;
				break;
			case 2:
				app->render->DrawTexture(Willycoin, c->data->position.x - 32, c->data->position.y - 32);
				c = c->next;
				break;
			case 3:
				SDL_Rect rect2 = c->data->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(c->data->sprite, c->data->position.x - 8, c->data->position.y - 8, &rect2);
				c = c->next;
				break;
			}
		}
	}
	return true;
}

bool Item::CleanUp() {

	ListItem<Items*>* c = items.start;
	while (c != NULL) {
		app->physics->world->DestroyBody(c->data->body->body);
		c = c->next;
	}
	items.clear();

	app->tex->UnLoad(ItemSprite);
	app->tex->UnLoad(Willycoin);

	coinFlash.DeleteAnim();
	coinIdle.DeleteAnim();
	coinSpin.DeleteAnim();
	return true;
}