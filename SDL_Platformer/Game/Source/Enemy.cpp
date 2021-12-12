#include "Enemy.h"
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

Enemy::Enemy(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("enemy");
}

Enemy::~Enemy() {

}

bool Enemy::Start() {

	LOG("Start Enemy variables");

	// Assets
	enemySprite = app->tex->Load("Assets/textures/enemySprite.png");
	enemyFlySprite = app->tex->Load("Assets/textures/enemyFlySprite.png");

	detectPlayerFx = app->audio->LoadFx("Assets/audio/fx/enemyDetectPlayer.wav");
	deathFx = app->audio->LoadFx("Assets/audio/fx/enemyDeath.wav");

	alertTexture = app->tex->Load("Assets/textures/alert.png");
	lostTexture = app->tex->Load("Assets/textures/stopChase.png");

	// Animations

	animIdleL.PushBack({ 150, 100, 50, 50 });
	animIdleL.loop = false;

	animIdleR.PushBack({ 0, 0, 50, 50 });
	animIdleR.loop = false;

	animDieL.PushBack({ 50, 150, 50, 50});
	animDieL.PushBack({ 0, 150, 50, 50 });
	animDieL.loop = false;
	animDieL.speed = 0.1f;

	animDieR.PushBack({ 100, 50, 50, 50 });
	animDieR.PushBack({ 150, 50, 50, 50 });
	animDieR.loop = false;
	animDieR.speed = 0.1f;

	animRunR.PushBack({ 0, 0, 50, 50 });
	animRunR.PushBack({ 50, 0, 50, 50 });
	animRunR.PushBack({ 100, 0, 50, 50 });
	animRunR.PushBack({ 0, 50, 50, 50 });
	animRunR.PushBack({ 50, 50, 50, 50 });
	animRunR.loop = true;
	animRunR.speed = 0.15f;

	animRunL.PushBack({ 150, 100, 50, 50 });
	animRunL.PushBack({ 100, 100, 50, 50 });
	animRunL.PushBack({ 50, 100, 50, 50 });
	animRunL.PushBack({ 150, 150, 50, 50 });
	animRunL.PushBack({ 100, 150, 50, 50 });
	animRunL.loop = true;
	animRunL.speed = 0.15f;

	return true;
}

bool Enemy::AddEnemy(int x, int y, int type) {

	LOG("Adding new enemy at X: %d and Y: %d", x, y);
	enemyNum += 1;
	Enemies* thisEnemy = new Enemies();

	thisEnemy->death = false;
	thisEnemy->position.x = x;
	thisEnemy->position.y = y;
	thisEnemy->speed = 0.05f;
	thisEnemy->dir = false;
	thisEnemy->playDetectFx = false;
	thisEnemy->body = app->physics->CreateChain(x, y, enemyChain, 16);
	thisEnemy->collider = app->physics->CreateRectangleSensor(x, y, 40, 40);
	thisEnemy->collider->type = "enemy";
	thisEnemy->body->body->SetType(b2_dynamicBody);
	thisEnemy->body->body->SetFixedRotation(true);

	thisEnemy->animDieL = animDieL;
	thisEnemy->animDieR = animDieR;
	thisEnemy->animRunL = animRunL;
	thisEnemy->animRunR = animRunR;

	thisEnemy->currentAnimation = &animIdleL;
	thisEnemy->type = type;

	if (type == 0) {
		thisEnemy->sprite = enemySprite;
	}
	else if (type == 1) {
		thisEnemy->sprite = enemyFlySprite;
	}
	else {
		LOG("Error at creating enemy. Type : %s don't exist", type);
		return false;
	}

	enemies.add(thisEnemy);
	return true;
}

bool Enemy::PreUpdate() {

	if (app->physics->debug == true) {
		// ENEMY DEBUG GENERATOR
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			// Get mouse position
			int x, y;
			app->input->GetMousePosition(x, y);

			// Transform mouse position into map position
			iPoint p = app->render->ScreenToWorld(x, y);
			p = app->map->WorldToMap(p.x, p.y);

			// Multiply per 16 as tiles widht and height are 16 pixels
			AddEnemy(p.x * 16, p.y * 16, 0);
		}
		if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			// Get mouse position
			int x, y;
			app->input->GetMousePosition(x, y);

			// Transform mouse position into map position
			iPoint p = app->render->ScreenToWorld(x, y);
			p = app->map->WorldToMap(p.x, p.y);

			// Multiply per 16 as tiles widht and height are 16 pixels
			AddEnemy(p.x * 16, p.y * 16, 1);
		}
	}
	return true;
}

bool Enemy::Update(float dt) {

	ListItem<Enemies*>* c = enemies.start;
	while (c != NULL) {
		if (c->data->death == false) {

			// ENEMIES ALIVE:

			// Move sensor to enemy body position
			c->data->collider->body->SetTransform(b2Vec2(c->data->body->body->GetPosition().x + PIXEL_TO_METERS(20), c->data->body->body->GetPosition().y + PIXEL_TO_METERS(20)), c->data->collider->body->GetAngle());

			// Detect player
			int chaseDistance = 200, limitVel = 100;

			if (app->player->position.x - c->data->position.x < chaseDistance && app->player->position.x - c->data->position.x > -chaseDistance && app->player->position.y - c->data->position.y < chaseDistance / 4 && app->player->position.y - c->data->position.y > -chaseDistance / 2) {
				// Play sfx
				if (c->data->playDetectFx != true) {
					app->audio->PlayFx(detectPlayerFx);
					c->data->playDetectFx = true;

					c->data->alert = true;
					c->data->lost = false;
				}
				// Chase player
				int vel = METERS_TO_PIXELS(c->data->body->body->GetLinearVelocity().x);	 // limit velocity

				if (-limitVel < vel && vel < limitVel) {
					if (app->player->position.x < c->data->position.x) {
						c->data->body->body->ApplyLinearImpulse(b2Vec2(-c->data->speed, 0), b2Vec2(0, 0), 1);
						if (c->data->currentAnimation != &c->data->animRunL) {
							c->data->currentAnimation = &c->data->animRunL;
						}
					}
					else {
						c->data->body->body->ApplyLinearImpulse(b2Vec2(c->data->speed, 0), b2Vec2(0, 0), 1);

						if (c->data->currentAnimation != &c->data->animRunR) {
							c->data->currentAnimation = &c->data->animRunR;
						}
					}
				}
			}
			else {
				c->data->playDetectFx = false;

				if (c->data->lost == false && c->data->currentAnimation == &c->data->animRunL || c->data->currentAnimation == &c->data->animRunR) {
					c->data->lost = true;
					c->data->alert = false;
				}

				if (c->data->currentAnimation == &c->data->animRunL) {
					c->data->currentAnimation = &animIdleL;
				}
				if (c->data->currentAnimation == &c->data->animRunR) {
					c->data->currentAnimation = &animIdleR;
				}
			}

			// Reset alert/lost effect
			if (c->data->alert == true || c->data->lost == true) {
				c->data->cont++;
			}

			if (c->data->cont > 100) {
				c->data->alert = false;
				c->data->lost = false;
				c->data->cont = false;
			}

			// Update animation and position
			c->data->currentAnimation->Update();
			c->data->body->GetPosition(c->data->position.x, c->data->position.y);
			c = c->next;
		}
		else {
			// ENEMIES DEAD

			LOG("Deleting enemy");
			enemyNum -= 1;
			app->physics->world->DestroyBody(c->data->body->body);
			app->physics->world->DestroyBody(c->data->collider->body);
			enemies.del(c);
			c = NULL;
			
			app->audio->PlayFx(deathFx);
		}
	}

	//// Update animations

	//animRunL.Update();
	//animRunR.Update();
	//animDieL.Update();
	//animDieR.Update();

	return true;
}

bool Enemy::PostUpdate() {

	if (app->player->death != true && app->player->win != true) {
		ListItem<Enemies*>* c = enemies.start;
		while (c != NULL) {
			SDL_Rect rect = c->data->currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(c->data->sprite, c->data->position.x - 5, c->data->position.y - 5, &rect);

			if (c->data->alert == true) {
				app->render->DrawTexture(alertTexture, c->data->position.x + 5, c->data->position.y - 25);
			}
			if (c->data->lost == true) {
				app->render->DrawTexture(lostTexture, c->data->position.x + 5, c->data->position.y - 25);
			}

			c = c->next;
		}
	}
	return true;
}

bool Enemy::CleanUp() {

	ListItem<Enemies*>* c = enemies.start;
	while (c != NULL) {
		if (c->data->body->body != NULL) {
			app->physics->world->DestroyBody(c->data->body->body);
			app->physics->world->DestroyBody(c->data->collider->body);
		}
		c = c->next;
	}
	enemies.clear();

	app->tex->UnLoad(enemySprite);
	app->tex->UnLoad(lostTexture);
	app->tex->UnLoad(alertTexture);
	app->tex->UnLoad(enemyFlySprite);

	animIdleL.DeleteAnim();
	animIdleR.DeleteAnim();
	animDieL.DeleteAnim();
	animDieR.DeleteAnim();
	animRunL.DeleteAnim();
	animRunR.DeleteAnim();

	return true;
}