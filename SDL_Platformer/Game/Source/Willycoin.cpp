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
#include "Map.h"

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
	foodFx = app->audio->LoadFx("Assets/audio/fx/eatFx.wav");

	srand(time(NULL));
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
	coinSpin.speed = 0.08f;

	coinFlash.PushBack({ 16,112,16,16 });
	coinFlash.PushBack({ 32,112,16,16 });
	coinFlash.PushBack({ 48,112,16,16 });
	coinFlash.PushBack({ 64,112,16,16 });
	coinFlash.PushBack({ 80,112,16,16 });
	coinFlash.PushBack({ 96,112,16,16 });
	coinFlash.PushBack({ 16,112,16,16 });
	coinFlash.loop = false;
	coinFlash.speed = 0.1f;

	lollipop.PushBack({ 480,592,16,16 });
	candy.PushBack({ 496,592,16,16 });
	chocolate.PushBack({ 512,592,16,16 });
	can.PushBack({ 544,592,16,16 });
	iceCream1.PushBack({ 560,592,16,16 });
	iceCream2.PushBack({ 576,592,16,16 });
	cherry.PushBack({ 752,592,16,16 });
	orange.PushBack({ 768,592,16,16 });
	banana.PushBack({ 784,592,16,16 });
	strawberry.PushBack({ 800,592,16,16 });
	apple.PushBack({ 816,592,16,16 });

	return true;
}

void Item::AddItem(int x, int y, int type) {

	LOG("Adding new Item at X: %d and Y: %d", x, y);

	Items* thisItem = new Items();
	itemNum += 1;
	int anim = rand() % 11 + 1;
	switch (type)
	{
	case 1: //COIN
		thisItem->spawn = true;
		thisItem->type = 1;
		thisItem->sprite = ItemSprite;
		thisItem->body = app->physics->CreateRectangleSensor(x, y, 16, 16);
		thisItem->currentAnimation = &coinIdle;
		thisItem->position.x = x;
		thisItem->position.y = y;

		thisItem->coinFlashAnim = coinFlash;
		thisItem->coinSpinAnim = coinSpin;
		break;
	case 2: //WILLYCOIN
		thisItem->spawn = true;
		thisItem->type = 2;
		thisItem->body = app->physics->CreateRectangleSensor(x, y, 64, 64);
		thisItem->position.x = x;
		thisItem->position.y = y;
		break;
	case 3: //FOOD
		thisItem->spawn = true;
		thisItem->type = 3;
		thisItem->sprite = ItemSprite;
		thisItem->body = app->physics->CreateRectangleSensor(x, y, 16, 16);
		thisItem->position.x = x;
		thisItem->position.y = y;
		if (anim == 1) {
			thisItem->currentAnimation = &lollipop;
		}
		if (anim == 2) {
			thisItem->currentAnimation = &candy;
		}
		if (anim == 3) {
			thisItem->currentAnimation = &chocolate;
		}
		if (anim == 4) {
			thisItem->currentAnimation = &can;
		}
		if (anim == 5) {
			thisItem->currentAnimation = &iceCream1;
		}
		if (anim == 6) {
			thisItem->currentAnimation = &iceCream2;
		}
		if (anim == 7) {
			thisItem->currentAnimation = &cherry;
		}
		if (anim == 8) {
			thisItem->currentAnimation = &banana;
		}
		if (anim == 9) {
			thisItem->currentAnimation = &strawberry;
		}
		if (anim == 10) {
			thisItem->currentAnimation = &apple;
		}
		if (anim == 11) {
			thisItem->currentAnimation = &orange;
		}		
		break;
	}
	items.add(thisItem);

	thisItem = nullptr;
	delete thisItem;
}

bool Item::PreUpdate() {
	if (app->physics->debug == true) {
		// COIN DEBUG GENERATOR
		if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			// Item type
			int type = 1;

			// Get mouse position
			int x, y;
			app->input->GetMousePosition(x, y);

			// Transform mouse position into map position
			iPoint p = app->render->ScreenToWorld(x, y);
			p = app->map->WorldToMap(p.x, p.y);

			// Multiply per 16 as tiles widht and height are 16 pixels
			AddItem(p.x * 16, p.y * 16, type);
		}
		if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			// Item type
			int type = 2;

			// Get mouse position
			int x, y;
			app->input->GetMousePosition(x, y);

			// Transform mouse position into map position
			iPoint p = app->render->ScreenToWorld(x, y);
			p = app->map->WorldToMap(p.x, p.y);

			// Multiply per 16 as tiles widht and height are 16 pixels
			AddItem(p.x * 16, p.y * 16, type);
		}
		if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		{
			// Item type
			int type = 3;

			// Get mouse position
			int x, y;
			app->input->GetMousePosition(x, y);

			// Transform mouse position into map position
			iPoint p = app->render->ScreenToWorld(x, y);
			p = app->map->WorldToMap(p.x, p.y);

			// Multiply per 16 as tiles widht and height are 16 pixels
			AddItem(p.x * 16, p.y * 16, type);
		}
	}
	return true;
}

bool Item::Update(float dt) {
	
	ListItem<Items*>* c = items.start;
	int anim = rand() % 2 + 1;
	while (c != NULL) {
		// ======================================================
		//        Destroy Items when player collects them
		// ======================================================
		switch (c->data->type)
		{
		case 1:
			if (c->data->spawn == false) {
				LOG("Deleting Coin");
				itemNum -= 1;
				app->physics->world->DestroyBody(c->data->body->body);
				items.del(c);
				
				c = NULL;
			}
			else {
				if (c->data->cont >= 100) {
					if (anim == 1)
					{
						c->data->coinFlashAnim.Reset();
						c->data->currentAnimation = &c->data->coinFlashAnim;
					}
					if (anim == 2)
					{
						c->data->coinSpinAnim.Reset();
						c->data->currentAnimation = &c->data->coinSpinAnim;
					}
					c->data->cont = 0;
				}
				if (app->getDeltaTime() < 20) {
					c->data->currentAnimation->Update();
				}
				else {
					c->data->currentAnimation->Update();
					c->data->currentAnimation->Update();
				}
				c->data->cont += 1;
				c = c->next;

			}
			break;
		case 2:
			if (c->data->spawn == false) {
				LOG("Deleting Coin");
				itemNum -= 1;
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
				itemNum -= 1;
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
		// ======================================================
		//						Draw items
		// ======================================================
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
	items.~List();

	app->tex->UnLoad(ItemSprite);
	app->tex->UnLoad(Willycoin);

	coinFlash.DeleteAnim();
	coinIdle.DeleteAnim();
	coinSpin.DeleteAnim();
	lollipop.DeleteAnim(); 
	candy.DeleteAnim();
	chocolate.DeleteAnim();
	can.DeleteAnim();
	iceCream1.DeleteAnim();
	iceCream2.DeleteAnim();
	cherry.DeleteAnim();
	banana.DeleteAnim();
	strawberry.DeleteAnim();
	apple.DeleteAnim();
	orange.DeleteAnim();

	return true;
}