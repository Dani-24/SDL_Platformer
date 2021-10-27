#include "Player.h"
#include "App.h"
#include "Scene.h"
#include "Textures.h"
#include "Module.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"

Player::Player() : Module()
{

	//idle Right animation
	idleR.PushBack({ 0, 0, 32, 32 });
	idleR.loop = false;

	//Walk right -> animation
	walkR.PushBack({ 0,0,32,32 });
	walkR.PushBack({ 32,0,32,32 });
	walkR.PushBack({ 64,0,32,32 });
	walkR.PushBack({ 96,0,32,32 });
	walkR.PushBack({ 128,0,32,32 });
	walkR.PushBack({ 160,0,32,32 });
	walkR.loop = true;
	walkR.speed = 0.15f;

}

Player::~Player()
{

}

bool Player:: Start()
{
	bool ret = true;

	texture = app->tex->Load("Assets/textures/player.png");
	currentAnimation = &idleR;

	app->player->position.x = 0;
	app->player->position.y = 0;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	Player_Position = true;

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		if (currentAnimation != &walkR)
		{
			walkR.Reset();
			currentAnimation = &walkR;
			Player_Position = true;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_K) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		if (currentAnimation != &idleR)
		{
			idleR.Reset();
			currentAnimation = &idleR;
			Player_Position = true;
		}
	}

	return ret;
}

bool Player::PostUpdate() {
	bool ret = true;

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);//draw player
		
	return ret;
}

bool Player::CleanUp() {


	app->tex->UnLoad(texture);

	isEnabled = true;


	return true;
}