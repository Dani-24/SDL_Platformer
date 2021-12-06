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

Enemy::Enemy(App* application, bool start_enabled) : Module(application, start_enabled){

}

Enemy::~Enemy() {

}

bool Enemy::Start() {

	LOG("Start Enemy variables");

	// Assets
	enemySprite = app->tex->Load("Assets/textures/enemySprite.png");

	detectPlayerFx = app->audio->LoadFx("Assets/audio/fx/enemyDetectPlayer.wav");
	deathFx = app->audio->LoadFx("Assets/audio/fx/enemyDeath.wav");

	// Animations

	animDieL.PushBack({ 50, 150, 50, 50});
	animDieL.PushBack({ 0, 150, 50, 50 });
	animDieL.loop = false;
	animDieL.speed = 0.5f;

	animDieR.PushBack({ 100, 50, 50, 50 });
	animDieR.PushBack({ 150, 50, 50, 50 });
	animDieR.loop = false;
	animDieR.speed = 0.5f;

	animRunL.PushBack({ 50, 150, 50, 50 });
	animRunL.PushBack({ 0, 150, 50, 50 });
	animRunL.loop = true;
	animRunL.speed = 1.5f;

	animRunR.PushBack({ 150, 100, 50, 50 });
	animRunR.PushBack({ 100, 100, 50, 50 });
	animRunR.PushBack({ 50, 100, 50, 50 });
	animRunR.PushBack({ 150, 150, 50, 50 });
	animRunR.PushBack({ 100, 150, 50, 50 });
	animRunR.loop = true;
	animRunR.speed = 1.5f;

	return true;
}

void Enemy::AddEnemy(int x, int y) {

	LOG("Adding new enemy at X: %d and Y: %d", x, y);

	Enemies thisEnemy;

	thisEnemy.death = false;
	thisEnemy.sprite = enemySprite;
	thisEnemy.speed = 1.0f;
	thisEnemy.body = app->physics->CreateCircle(x, y, 50);
	thisEnemy.position.x = x;
	thisEnemy.position.y = y;
	thisEnemy.currentAnimation = &animRunL;

	// Add enemy (Crashea por algun motivo)
	//enemies->add(thisEnemy);

	//enemies->end->data.body->listener = this;
}

bool Enemy::PreUpdate() {

	return true;
}

bool Enemy::Update(float dt) {

	if (enemies != nullptr) {
		ListItem<Enemies>* c = enemies->start;
		while (c != NULL) {
			c->data.currentAnimation->Update();
			c = c->next;
		}
	}

	return true;
}

bool Enemy::PostUpdate() {

	if (enemies != nullptr) {
		ListItem<Enemies>* c = enemies->start;
		while (c != NULL) {
			SDL_Rect rect = c->data.currentAnimation->GetCurrentFrame();

			app->render->DrawTexture(c->data.sprite, c->data.position.x, c->data.position.y, &rect);
			c = c->next;
		}
	}
	return true;
}

bool Enemy::CleanUp() {

	if (enemies != nullptr) {
		ListItem<Enemies>* c = enemies->start;
		while (c != NULL) {
			app->physics->world->DestroyBody(c->data.body->body);
			c = c->next;
		}
		enemies->clear();
	}

	app->tex->UnLoad(enemySprite);

	animDieL.DeleteAnim();
	animDieR.DeleteAnim();
	animRunL.DeleteAnim();
	animRunR.DeleteAnim();

	return true;
}

void Enemy::DelEnemy(int enemyToDel) {
	LOG("Deleting enemy number %d", enemyToDel);

	enemies->del(enemies->At(enemyToDel));
}