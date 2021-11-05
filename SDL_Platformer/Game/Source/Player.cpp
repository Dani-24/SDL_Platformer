#include "Player.h"
#include "App.h"
#include "Scene.h"
#include "Textures.h"
#include "Module.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Animation.h"
#include "Collision.h"
#include "Collider.h"

Player::Player(App* application, bool start_enabled) : Module(application, start_enabled)
{

	//player
	playerR.PushBack({ 0, 0, 32, 32 });

	//atack ->
	punchR.PushBack({ 0,32,32,32});
	punchR.PushBack({ 32,32,32,32 });
	punchR.PushBack({ 64,32,32,32 });
	punchR.PushBack({ 96,32,32,32 });
	punchR.loop = true;
	punchR.speed = 0.1;

	//doble attack ->
	doublePunchR.PushBack({ 0,64,32,32 });
	doublePunchR.PushBack({ 32,64,32,32 });
	doublePunchR.PushBack({ 64,64,32,32 });
	doublePunchR.PushBack({ 96,64,32,32 });
	doublePunchR.PushBack({ 128,64,32,32 });
	doublePunchR.PushBack({ 160,64,32,32 });
	doublePunchR.loop = true;
	doublePunchR.speed = 0.1;

	//climb
	climb.PushBack({ 0,96,32,32 });
	climb.PushBack({ 32,96,32,32 });
	climb.PushBack({ 64,96,32,32 });
	climb.PushBack({ 96,96,32,32 });
	climb.loop = true;
	climb.speed = 0.3;

	//morir ->
	deathR.PushBack({ 0,128,32,32 });
	deathR.PushBack({ 32,128,32,32 });
	deathR.PushBack({ 64,128,32,32 });
	deathR.PushBack({ 96,128,32,32 });
	deathR.PushBack({ 128,128,32,32 });
	deathR.PushBack({ 160,128,32,32 });
	deathR.PushBack({ 192,128,32,32 });
	deathR.PushBack({ 224,128,32,32 });
	deathR.loop = true;
	deathR.speed = 0.3;

	//hit ->
	hitR.PushBack({ 0,160,32,32 });
	hitR.PushBack({ 32,160,32,32 });
	hitR.PushBack({ 64,160,32,32 });
	hitR.PushBack({ 96,160,32,32 });
	hitR.loop = true;
	hitR.speed = 0.3;

	//idle ->
	idleR.PushBack({ 0,192,32,32 });
	idleR.PushBack({ 32,192,32,32 });
	idleR.PushBack({ 64,192,32,32 });
	idleR.PushBack({ 96,192,32,32 });
	idleR.loop = true;
	idleR.speed = 0.1;

	//jump ->
	jumpR.PushBack({ 0,224,32,32 });
	jumpR.PushBack({ 32,224,32,32 });
	jumpR.PushBack({ 64,224,32,32 });
	jumpR.PushBack({ 96,224,32,32 });
	jumpR.PushBack({ 128,224,32,32 });
	jumpR.PushBack({ 160,224,32,32 });
	jumpR.PushBack({ 192,224,32,32 });
	jumpR.PushBack({ 224,224,32,32 });
	jumpR.loop = false;
	jumpR.speed = 0.2;

	//push ->
	pushR.PushBack({ 0,256,32,32 });
	pushR.PushBack({ 32,256,32,32 });
	pushR.PushBack({ 64,256,32,32 });
	pushR.PushBack({ 96,256,32,32 });
	pushR.PushBack({ 128,256,32,32 });
	pushR.PushBack({ 160,256,32,32 });
	pushR.loop = true;
	pushR.speed = 0.3;

	//run ->
	runR.PushBack({ 0,288,32,32 });
	runR.PushBack({ 32,288,32,32 });
	runR.PushBack({ 64,288,32,32 });
	runR.PushBack({ 96,288,32,32 });
	runR.PushBack({ 128,288,32,32 });
	runR.PushBack({ 160,288,32,32 });
	runR.loop = true;
	runR.speed = 0.2;

	//throw ->
	throwR.PushBack({ 0,320,32,32 });
	throwR.PushBack({ 32,320,32,32 });
	throwR.PushBack({ 64,320,32,32 });
	throwR.PushBack({ 96,320,32,32 });
	throwR.loop = true;
	throwR.speed = 0.3;

	//Walk right -> animation
	walkR.PushBack({ 0,352,32,32 });
	walkR.PushBack({ 32,352,32,32 });
	walkR.PushBack({ 64,352,32,32 });
	walkR.PushBack({ 96,352,32,32 });
	walkR.PushBack({ 128,352,32,32 });
	walkR.PushBack({ 160,352,32,32 });
	walkR.loop = true;
	walkR.speed = 0.1;

	//walk and punch ->
	walkPunchR.PushBack({ 0,384,32,32 });
	walkPunchR.PushBack({ 32,384,32,32 });
	walkPunchR.PushBack({ 64,384,32,32 });
	walkPunchR.PushBack({ 96,384,32,32 });
	walkPunchR.PushBack({ 128,384,32,32 });
	walkPunchR.PushBack({ 160,384,32,32 });
	walkPunchR.loop = true;
	walkPunchR.speed = 0.3;

	//player <-
	playerL.PushBack({ 224, 416, 32, 32 });

	//atack <-
	punchL.PushBack({ 224,448,32,32 });
	punchL.PushBack({ 192,448,32,32 });
	punchL.PushBack({ 160,448,32,32 });
	punchL.PushBack({ 128,448,32,32 });
	punchL.loop = true;
	punchL.speed = 0.1;

	//doble attack <-
	doublePunchL.PushBack({ 224,480,32,32 });
	doublePunchL.PushBack({ 192,480,32,32 });
	doublePunchL.PushBack({ 160,480,32,32 });
	doublePunchL.PushBack({ 128,480,32,32 });
	doublePunchL.PushBack({ 96,480,32,32 });
	doublePunchL.PushBack({ 64,480,32,32 });
	doublePunchL.loop = true;
	doublePunchL.speed = 0.1;

	//morir <-
	deathL.PushBack({ 224,544,32,32 });
	deathL.PushBack({ 192,544,32,32 });
	deathL.PushBack({ 160,544,32,32 });
	deathL.PushBack({ 128,544,32,32 });
	deathL.PushBack({ 96,544,32,32 });
	deathL.PushBack({ 64,544,32,32 });
	deathL.PushBack({ 32,544,32,32 });
	deathL.PushBack({ 0,544,32,32 });
	deathL.loop = true;
	deathL.speed = 0.3;

	//hit <-
	hitL.PushBack({ 224,576,32,32 });
	hitL.PushBack({ 192,576,32,32 });
	hitL.PushBack({ 160,576,32,32 });
	hitL.PushBack({ 128,576,32,32 });
	hitL.loop = true;
	hitL.speed = 0.3;

	//idle <-
	idleL.PushBack({ 224,608,32,32 });
	idleL.PushBack({ 192,608,32,32 });
	idleL.PushBack({ 160,608,32,32 });
	idleL.PushBack({ 128,608,32,32 });
	idleL.loop = true;
	idleL.speed = 0.1;

	//jump <-
	jumpL.PushBack({ 224,640,32,32 });
	jumpL.PushBack({ 192,640,32,32 });
	jumpL.PushBack({ 160,640,32,32 });
	jumpL.PushBack({ 128,640,32,32 });
	jumpL.PushBack({ 96,640,32,32 });
	jumpL.PushBack({ 64,640,32,32 });
	jumpL.PushBack({ 32,640,32,32 });
	jumpL.PushBack({ 0,640,32,32 });
	jumpL.loop = true;
	jumpL.speed = 0.3;

	//push <-
	pushL.PushBack({ 224,672,32,32 });
	pushL.PushBack({ 192,672,32,32 });
	pushL.PushBack({ 160,672,32,32 });
	pushL.PushBack({ 128,672,32,32 });
	pushL.PushBack({ 96,672,32,32 });
	pushL.PushBack({ 64,672,32,32 });
	pushL.loop = true;
	pushL.speed = 0.3;

	//run <-
	runL.PushBack({ 224,704,32,32 });
	runL.PushBack({ 192,704,32,32 });
	runL.PushBack({ 160,704,32,32 });
	runL.PushBack({ 128,704,32,32 });
	runL.PushBack({ 96,704,32,32 });
	runL.PushBack({ 64,704,32,32 });
	runL.loop = true;
	runL.speed = 0.2;

	//throw <-
	throwL.PushBack({ 224,736,32,32 });
	throwL.PushBack({ 192,736,32,32 });
	throwL.PushBack({ 160,736,32,32 });
	throwL.PushBack({ 128,736,32,32 });
	throwL.loop = true;
	throwL.speed = 0.3;

	//Walk <- animation
	walkL.PushBack({ 224,768,32,32 });
	walkL.PushBack({ 192,768,32,32 });
	walkL.PushBack({ 160,768,32,32 });
	walkL.PushBack({ 128,768,32,32 });
	walkL.PushBack({ 96,768,32,32 });
	walkL.PushBack({ 64,768,32,32 });
	walkL.loop = true;
	walkL.speed = 0.1;

	//walk and punch <-
	walkPunchL.PushBack({ 224,800,32,32 });
	walkPunchL.PushBack({ 192,800,32,32 });
	walkPunchL.PushBack({ 160,800,32,32 });
	walkPunchL.PushBack({ 128,800,32,32 });
	walkPunchL.PushBack({ 96,800,32,32 });
	walkPunchL.PushBack({ 64,800,32,32 });
	walkPunchL.loop = true;
	walkPunchL.speed = 0.3;
}

Player::~Player()
{

}

bool Player:: Start()
{
	bool ret = true;

	// Inicializar variables
	Player_Position = true;
	death = false;
	speed = 2;
	lowSpeed = 1;
	god = false;
	HP = Max_HP;
	destroyed = false;
	collider = nullptr;

	// Load assets
	texture = app->tex->Load("Assets/textures/player.png");
	currentAnimation = &idleR;

	app->player->position.x = 32;
	app->player->position.y = 432;

	collider = app->collision->AddCollider({ position.x, position.y+2, 30, 30 }, Collider::Type::PLAYER);


	app->render->camera.x = 0;
	app->render->camera.y = -432;

	Player_Position = true;

	return ret;
}


bool Player::Update(float dt)
{
	bool ret = true;

	collider->SetPos(position.x, position.y+2);

	//if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
	//	if (app->collision->debug == true) {
	//		app->collision->debug = false;
	//	}
	//	else {
	//		app->collision->debug = true;
	//	}
	//}

	//run ->
	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
	{
		//walk ->
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			position.x += lowSpeed;
			if (currentAnimation != &walkR)
			{
				walkR.Reset();
				currentAnimation = &walkR;
				Player_Position = true;
			}
		}
		else {
			position.x += speed;
			if (currentAnimation != &runR)
			{
				runR.Reset();
				currentAnimation = &runR;
				Player_Position = true;
			}
		}
	}

	//run <-
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
		//walk <-
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			position.x -= lowSpeed;
			if (currentAnimation != &walkL)
			{
				walkL.Reset();
				currentAnimation = &walkL;
				Player_Position = false;
			}
		}
		else {
			position.x -= speed;
			if (currentAnimation != &runL)
			{
				runL.Reset();
				currentAnimation = &runL;
				Player_Position = false;
			}
		}
	}

	//attack
	if ((app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN))
	{
		if (Player_Position == true) {
			if (currentAnimation != &punchR)
			{
				punchR.Reset();
				currentAnimation = &punchR;
				Player_Position = true;
			}
		}
		else {
			if (currentAnimation != &punchL)
			{
				punchL.Reset();
				currentAnimation = &punchL;
				Player_Position = false;
			}
		}
	}


	//idle
	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_O) == KEY_IDLE))
	{
		if (currentAnimation != &idleR && Player_Position == true)
		{
			idleR.Reset();
			currentAnimation = &idleR;
			Player_Position = true;
		}

		if (currentAnimation != &idleL && Player_Position == false)
		{
			idleL.Reset();
			currentAnimation = &idleL;
			Player_Position = false;
		}
	}

	

	currentAnimation->Update();

	return ret;
}

bool Player::PostUpdate() {
	bool ret = true;

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, 0,0,0);//draw player
		
	return ret;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{

}

bool Player::CleanUp() {
	// Clean player bullshit

	// Animations
	currentAnimation = nullptr;
	playerR.DeleteAnim(); 
	playerL.DeleteAnim();
	punchR.DeleteAnim(); 
	punchL.DeleteAnim();
	doublePunchR.DeleteAnim(); 
	doublePunchL.DeleteAnim();
	climb.DeleteAnim();
	deathR.DeleteAnim(); 
	deathL.DeleteAnim();
	hitR.DeleteAnim(); 
	hitL.DeleteAnim();
	idleR.DeleteAnim(); 
	idleL.DeleteAnim();
	jumpR.DeleteAnim(); 
	jumpL.DeleteAnim();
	pushR.DeleteAnim(); 
	pushL.DeleteAnim();
	runR.DeleteAnim(); 
	runL.DeleteAnim();
	throwR.DeleteAnim(); 
	throwL.DeleteAnim();
	walkR.DeleteAnim(); 
	walkL.DeleteAnim();
	walkPunchR.DeleteAnim(); 
	walkPunchL.DeleteAnim();

	// Textures
	app->tex->UnLoad(texture);

	return true;
}