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
#include "Pathfinder.h"

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

	animDie.PushBack({ 100, 50, 50, 50 });
	animDie.PushBack({ 150, 50, 50, 50 });
	animDie.loop = false;
	animDie.speed = 0.05f;

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
	Enemies* thisEnemy = new Enemies();

	thisEnemy->position.x = x;
	thisEnemy->position.y = y;
	thisEnemy->speed = 0.01f;
	thisEnemy->dir = false;
	thisEnemy->playDetectFx = false;
	thisEnemy->body = app->physics->CreateChain(x, y, enemyChain, 16);
	thisEnemy->collider = app->physics->CreateRectangleSensor(x, y, 40, 40);
	thisEnemy->collider->type = "enemy";
	thisEnemy->body->body->SetType(b2_dynamicBody);
	thisEnemy->body->body->SetFixedRotation(true);

	thisEnemy->animRunL = animRunL;
	thisEnemy->animRunR = animRunR;
	thisEnemy->animDie = animDie;

	thisEnemy->currentAnimation = &animIdleL;
	thisEnemy->type = type;

	if (type == 0) {
		thisEnemy->sprite = enemySprite;
	}
	else if (type == 1) {
		thisEnemy->sprite = enemyFlySprite;

		// Allow fly
		thisEnemy->body->body->SetGravityScale(0);
	}
	else {
		LOG("Error at creating enemy. Type : %s don't exist", type);
		return false;
	}

	enemies.add(thisEnemy);

	thisEnemy = nullptr;
	delete thisEnemy;

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
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
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

	for (ListItem<Enemies*>* c = enemies.start; c != NULL; c = c->next) {
		if (c->data->dead == false) {
			// ENEMIES ALIVE:

			// Move sensor to enemy body position
			c->data->collider->body->SetTransform(b2Vec2(c->data->body->body->GetPosition().x + PIXEL_TO_METERS(20), c->data->body->body->GetPosition().y + PIXEL_TO_METERS(20)), c->data->collider->body->GetAngle());

			// Detect player
			int chaseDistance = 200, limitVel = 100;
			if (c->data->type == 0) {
				// ==================================================
				//					  Walking Enemy
				// ==================================================
				if (app->player->position.x - c->data->position.x < chaseDistance && app->player->position.x - c->data->position.x > -chaseDistance && app->player->position.y - c->data->position.y < chaseDistance / 4 && app->player->position.y - c->data->position.y > -chaseDistance / 2) {
					// Play sfx
					if (c->data->playDetectFx != true) {
						app->audio->PlayFx(detectPlayerFx);
						c->data->playDetectFx = true;
						c->data->alert = true;
						c->data->lost = false;
					}
					// ----------- Chase player --------------
					// Pathfinding
					iPoint origin = app->map->WorldToMap(c->data->position.x, c->data->position.y);
					iPoint destination = app->map->WorldToMap(app->player->position.x, app->player->position.y);

					int a = app->pathfinder->CreatePath(origin, destination);

					int vel = METERS_TO_PIXELS(c->data->body->body->GetLinearVelocity().x);	 // limit velocity

					if (-limitVel < vel && vel < limitVel) {
						if (app->player->position.x < c->data->position.x) {
							c->data->body->body->ApplyLinearImpulse(b2Vec2(-c->data->speed * dt, 0), b2Vec2(0, 0), 1);
							if (c->data->currentAnimation != &c->data->animRunL) {
								c->data->currentAnimation = &c->data->animRunL;
							}
						}
						else {
							c->data->body->body->ApplyLinearImpulse(b2Vec2(c->data->speed * dt, 0), b2Vec2(0, 0), 1);

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
			}
			// ==================================================
			//					  Flying Enemy
			// ==================================================
			else if (c->data->type == 1) {
				if (app->player->position.x - c->data->position.x < chaseDistance && app->player->position.x - c->data->position.x > -chaseDistance && app->player->position.y - c->data->position.y < chaseDistance && app->player->position.y - c->data->position.y > -chaseDistance) {
					// Play sfx
					if (c->data->playDetectFx != true) {
						app->audio->PlayFx(detectPlayerFx);
						c->data->playDetectFx = true;
						c->data->alert = true;
						c->data->lost = false;
					}

					// ----------- Chase player --------------

					// Pathfinding
					iPoint origin = app->map->WorldToMap(c->data->position.x, c->data->position.y);
					iPoint destination = app->map->WorldToMap(app->player->position.x, app->player->position.y);

					int a = app->pathfinder->CreatePath(origin, destination);

					// limit velocity
					iPoint vel;
					vel.x = METERS_TO_PIXELS(c->data->body->body->GetLinearVelocity().x);
					vel.y = METERS_TO_PIXELS(c->data->body->body->GetLinearVelocity().y);

					if (-limitVel < vel.x && vel.x < limitVel) {

						// X axis
						if (app->player->position.x < c->data->position.x) {
							c->data->body->body->ApplyLinearImpulse(b2Vec2(-c->data->speed * dt, 0), b2Vec2(0, 0), 1);
							if (c->data->currentAnimation != &c->data->animRunL) {
								c->data->currentAnimation = &c->data->animRunL;
							}
						}
						else if (app->player->position.x > c->data->position.x) {
							c->data->body->body->ApplyLinearImpulse(b2Vec2(c->data->speed * dt, 0), b2Vec2(0, 0), 1);
							if (c->data->currentAnimation != &c->data->animRunR) {
								c->data->currentAnimation = &c->data->animRunR;
							}
						}
					}
					if (-limitVel < vel.y && vel.y < limitVel) {
						// Y axis
						if (app->player->position.y < c->data->position.y) {
							c->data->body->body->ApplyLinearImpulse(b2Vec2(0, -c->data->speed * dt), b2Vec2(0, 0), 1);
						}
						else if (app->player->position.y > c->data->position.y) {
							c->data->body->body->ApplyLinearImpulse(b2Vec2(0, c->data->speed * dt), b2Vec2(0, 0), 1);
						}
					}
				}
				else {

					// Stop movement
					c->data->body->body->SetLinearVelocity(b2Vec2(0, 0));

					// Animations and interactions:
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
		}
		else {
			c->data->cont--;
			if (c->data->cont <= 0) {

				app->physics->world->DestroyBody(c->data->body->body);
				app->physics->world->DestroyBody(c->data->collider->body);

				enemies.del(c);
				break;
			}
			else {
				c->data->currentAnimation = &c->data->animDie;
				c->data->currentAnimation->Update();
			}
		}
	}

	return true;
}

bool Enemy::DeleteEnemy(Enemies* e) {

	// Iniciar borrado del enemigo
	
	ListItem<Enemies*>* c = enemies.start;

	bool cosaBorrada = false;

	while (c != NULL && cosaBorrada == false) {

		if (c->data == e) {
			
			app->audio->PlayFx(deathFx);

			c->data->dead = true;
			c->data->cont = 100;

			cosaBorrada = true;
			return true;
		}
		c = c->next;
	}
	return false;
}

bool Enemy::PostUpdate() {

	if (app->player->death != true && app->player->win != true) {
		ListItem<Enemies*>* c = enemies.start;

		while (c != NULL) {
			// ======================================================
			//                    Draw enemies
			// ======================================================
			SDL_Rect rect = c->data->currentAnimation->GetCurrentFrame();

			// Draw interactions
			if (c->data->alert == true) {
				app->render->DrawTexture(alertTexture, c->data->position.x + 5, c->data->position.y - 25);
			}
			if (c->data->lost == true) {
				app->render->DrawTexture(lostTexture, c->data->position.x + 5, c->data->position.y - 25);
			}

			app->render->DrawTexture(c->data->sprite, c->data->position.x - 5, c->data->position.y - 5, &rect);
			
			// ======================================================
			//                  Pathfinding Debug
			// ======================================================
			if (app->physics->debug == true) {

				const DynArray<iPoint>* path = app->pathfinder->GetLastPath();

				for (uint i = 0; i < path->Count(); ++i)
				{
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					app->render->DrawTexture(app->pathfinder->pathTexture, pos.x, pos.y);
				}

				iPoint originScreen = app->map->MapToWorld(c->data->position.x, c->data->position.y);
				app->render->DrawTexture(app->pathfinder->pathOriginTexture, originScreen.x, originScreen.y);
			}
						
			c = c->next;
		}
	}

	return true;
}

bool Enemy::CleanUp() {

	ListItem<Enemies*>* c = enemies.start;
	while (c != NULL) {
		app->physics->world->DestroyBody(c->data->body->body);
		app->physics->world->DestroyBody(c->data->collider->body);
		c = c->next;
	}
	enemies.clear();

	app->tex->UnLoad(enemySprite);
	app->tex->UnLoad(lostTexture);
	app->tex->UnLoad(alertTexture);
	app->tex->UnLoad(enemyFlySprite);

	animIdleL.DeleteAnim();
	animIdleR.DeleteAnim();
	animDie.DeleteAnim();
	animRunL.DeleteAnim();
	animRunR.DeleteAnim();

	return true;
}