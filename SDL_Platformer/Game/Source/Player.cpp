#include "Player.h"
#include "App.h"
#include "Scene.h"
#include "Textures.h"
#include "Module.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Animation.h"
#include "Physics.h"

Player::Player(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("player");
}

Player::~Player()
{

}

void Player::PlayerStartAnims() {

	// player RIGHT
	playerR.PushBack({ 0, 0, 32, 32 });

	// player LEFT
	playerL.PushBack({ 224, 416, 32, 32 });
	
	// CLIMB
	climb.PushBack({ 0,96,32,32 });
	climb.PushBack({ 32,96,32,32 });
	climb.PushBack({ 64,96,32,32 });
	climb.PushBack({ 96,96,32,32 });
	climb.loop = true;
	climb.speed = 0.3;

	// DIE
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

	// idle RIGHT
	idleR.PushBack({ 0,192,32,32 });
	idleR.PushBack({ 32,192,32,32 });
	idleR.PushBack({ 64,192,32,32 });
	idleR.PushBack({ 96,192,32,32 });
	idleR.loop = true;
	idleR.speed = 0.1;

	//idle LEFT
	idleL.PushBack({ 224,608,32,32 });
	idleL.PushBack({ 192,608,32,32 });
	idleL.PushBack({ 160,608,32,32 });
	idleL.PushBack({ 128,608,32,32 });
	idleL.loop = true;
	idleL.speed = 0.1;


	fallR.PushBack({ 160,224,32,32 });
	fallR.PushBack({ 192,224,32,32 });
	fallR.PushBack({ 224,224,32,32 });
	fallR.loop = false;
	fallR.speed = 0.2;

	// fall LEFT
	fallL.PushBack({ 64,640,32,32 });
	fallL.PushBack({ 32,640,32,32 });
	fallL.PushBack({ 0,640,32,32 });
	fallL.loop = true;
	fallL.speed = 0.3;

	//push RIGHT
	pushR.PushBack({ 0,256,32,32 });
	pushR.PushBack({ 32,256,32,32 });
	pushR.PushBack({ 64,256,32,32 });
	pushR.PushBack({ 96,256,32,32 });
	pushR.PushBack({ 128,256,32,32 });
	pushR.PushBack({ 160,256,32,32 });
	pushR.loop = true;
	pushR.speed = 0.3;
	
	//push LEFT
	pushL.PushBack({ 224,672,32,32 });
	pushL.PushBack({ 192,672,32,32 });
	pushL.PushBack({ 160,672,32,32 });
	pushL.PushBack({ 128,672,32,32 });
	pushL.PushBack({ 96,672,32,32 });
	pushL.PushBack({ 64,672,32,32 });
	pushL.loop = true;
	pushL.speed = 0.3;

	//run RIGHT
	runR.PushBack({ 0,288,32,32 });
	runR.PushBack({ 32,288,32,32 });
	runR.PushBack({ 64,288,32,32 });
	runR.PushBack({ 96,288,32,32 });
	runR.PushBack({ 128,288,32,32 });
	runR.PushBack({ 160,288,32,32 });
	runR.loop = true;
	runR.speed = 0.2;
	
	//run LEFT
	runL.PushBack({ 224,704,32,32 });
	runL.PushBack({ 192,704,32,32 });
	runL.PushBack({ 160,704,32,32 });
	runL.PushBack({ 128,704,32,32 });
	runL.PushBack({ 96,704,32,32 });
	runL.PushBack({ 64,704,32,32 });
	runL.loop = true;
	runL.speed = 0.2;

	//throw RIGHT
	throwR.PushBack({ 0,320,32,32 });
	throwR.PushBack({ 32,320,32,32 });
	throwR.PushBack({ 64,320,32,32 });
	throwR.PushBack({ 96,320,32,32 });
	throwR.loop = true;
	throwR.speed = 0.3;

	//throw LEFT
	throwL.PushBack({ 224,736,32,32 });
	throwL.PushBack({ 192,736,32,32 });
	throwL.PushBack({ 160,736,32,32 });
	throwL.PushBack({ 128,736,32,32 });
	throwL.loop = true;
	throwL.speed = 0.3;

	//Walk right -> animation RIGHT
	walkR.PushBack({ 0,352,32,32 });
	walkR.PushBack({ 32,352,32,32 });
	walkR.PushBack({ 64,352,32,32 });
	walkR.PushBack({ 96,352,32,32 });
	walkR.PushBack({ 128,352,32,32 });
	walkR.PushBack({ 160,352,32,32 });
	walkR.loop = true;
	walkR.speed = 0.1;

	//Walk LEFT animation
	walkL.PushBack({ 224,768,32,32 });
	walkL.PushBack({ 192,768,32,32 });
	walkL.PushBack({ 160,768,32,32 });
	walkL.PushBack({ 128,768,32,32 });
	walkL.PushBack({ 96,768,32,32 });
	walkL.PushBack({ 64,768,32,32 });
	walkL.loop = true;
	walkL.speed = 0.1;

	//walk and punch RIGHT
	walkPunchR.PushBack({ 0,384,32,32 });
	walkPunchR.PushBack({ 32,384,32,32 });
	walkPunchR.PushBack({ 64,384,32,32 });
	walkPunchR.PushBack({ 96,384,32,32 });
	walkPunchR.PushBack({ 128,384,32,32 });
	walkPunchR.PushBack({ 160,384,32,32 });
	walkPunchR.loop = true;
	walkPunchR.speed = 0.3;

	//walk and punch LEFT
	walkPunchL.PushBack({ 224,800,32,32 });
	walkPunchL.PushBack({ 192,800,32,32 });
	walkPunchL.PushBack({ 160,800,32,32 });
	walkPunchL.PushBack({ 128,800,32,32 });
	walkPunchL.PushBack({ 96,800,32,32 });
	walkPunchL.PushBack({ 64,800,32,32 });
	walkPunchL.loop = true;
	walkPunchL.speed = 0.3;

	//attack RIGHT
	punchR.PushBack({ 0,32,32,32 });
	punchR.PushBack({ 32,32,32,32 });
	punchR.PushBack({ 64,32,32,32 });
	punchR.PushBack({ 96,32,32,32 });
	punchR.loop = false;
	punchR.speed = 0.1;

	//attack LEFT
	punchL.PushBack({ 224,448,32,32 });
	punchL.PushBack({ 192,448,32,32 });
	punchL.PushBack({ 160,448,32,32 });
	punchL.PushBack({ 128,448,32,32 });
	punchL.loop = false;
	punchL.speed = 0.1;

	//doble attack RIGHT
	doublePunchR.PushBack({ 0,64,32,32 });
	doublePunchR.PushBack({ 32,64,32,32 });
	doublePunchR.PushBack({ 64,64,32,32 });
	doublePunchR.PushBack({ 96,64,32,32 });
	doublePunchR.PushBack({ 128,64,32,32 });
	doublePunchR.PushBack({ 160,64,32,32 });
	doublePunchR.loop = true;
	doublePunchR.speed = 0.1;

	//doble attack LEFT
	doublePunchL.PushBack({ 224,480,32,32 });
	doublePunchL.PushBack({ 192,480,32,32 });
	doublePunchL.PushBack({ 160,480,32,32 });
	doublePunchL.PushBack({ 128,480,32,32 });
	doublePunchL.PushBack({ 96,480,32,32 });
	doublePunchL.PushBack({ 64,480,32,32 });
	doublePunchL.loop = true;
	doublePunchL.speed = 0.1;

	//morir LEFT
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

	// hit RIGHT ---
	hitR.PushBack({ 0,160,32,32 });
	hitR.PushBack({ 32,160,32,32 });
	hitR.PushBack({ 64,160,32,32 });
	hitR.PushBack({ 96,160,32,32 });
	hitR.loop = true;
	hitR.speed = 0.3;

	//hit LEFT
	hitL.PushBack({ 224,576,32,32 });
	hitL.PushBack({ 192,576,32,32 });
	hitL.PushBack({ 160,576,32,32 });
	hitL.PushBack({ 128,576,32,32 });
	hitL.loop = true;
	hitL.speed = 0.3;

	//jump RIGHT
	jumpR.PushBack({ 0,224,32,32 });
	jumpR.PushBack({ 32,224,32,32 });
	jumpR.PushBack({ 64,224,32,32 });
	jumpR.PushBack({ 96,224,32,32 });
	jumpR.PushBack({ 128,224,32,32 });
	jumpR.loop = false;
	jumpR.speed = 0.15f;

	//jump LEFT
	jumpL.PushBack({ 224,640,32,32 });
	jumpL.PushBack({ 192,640,32,32 });
	jumpL.PushBack({ 160,640,32,32 });
	jumpL.PushBack({ 128,640,32,32 });
	jumpL.PushBack({ 96,640,32,32 });
	jumpL.loop = true;
	jumpL.speed = 0.15f;

}

bool Player::Start()
{
	bool ret = true;

	// Inicializar variables
	speed = 0.05f;
	lowSpeed = 0.01f;
	HP = max_HP;

	// Load assets
	PlayerStartAnims();

	// Physics
	position.x = initPos.x;
	position.y = initPos.y;
		
	// Player physbody
	playerBody = app->physics->CreateChain(position.x, position.y, playerChain, 16);

	//attack collider
	attackSensorLeft = app->physics->CreateRectangleSensor(position.x, position.y, 16, 16);
	attackSensorRight = app->physics->CreateRectangleSensor(position.x, position.y, 16, 16);

	playerBody->body->SetType(b2_dynamicBody);
	playerBody->body->SetFixedRotation(true);

	attackSensorLeft->body->SetType(b2_dynamicBody);
	attackSensorLeft->body->SetFixedRotation(true);

	attackSensorRight->body->SetType(b2_dynamicBody);
	attackSensorRight->body->SetFixedRotation(true);

	//Joint the attack collider to the player body
	b2WeldJointDef LeftWeldJointDef;
	LeftWeldJointDef.bodyA = playerBody->body;
	LeftWeldJointDef.bodyB = attackSensorLeft->body;
	LeftWeldJointDef.localAnchorA.Set(-0.2, 0.3);
	LeftWeldJointDef.localAnchorB.Set(0, 0);

	b2WeldJoint* LeftWeldJoint = (b2WeldJoint*)app->physics->world->CreateJoint(&LeftWeldJointDef);

	b2WeldJointDef RightWeldJointDef;
	RightWeldJointDef.bodyA = playerBody->body;
	RightWeldJointDef.bodyB = attackSensorRight->body;
	RightWeldJointDef.localAnchorA.Set(0.7, 0.3);
	RightWeldJointDef.localAnchorB.Set(0, 0);

	b2WeldJoint* RightWeldJoint = (b2WeldJoint*)app->physics->world->CreateJoint(&RightWeldJointDef);

	// Texture & Animation
	playerSprite = app->tex->Load("Assets/textures/player.png");
	currentAnimation = &idleR;

	cooldownTex = app->tex->Load("Assets/textures/playerDieCooldown.png");

	livesSprite = app->tex->Load("Assets/textures/lives.png");
	liveOn.PushBack({0, 0, 30, 30});
	liveOff.PushBack({30, 0, 30, 30});
	liveOn.loop = liveOff.loop = false;

	// Audio
	deathFx = app->audio->LoadFx("Assets/audio/fx/death.wav");
	killedFx = app->audio->LoadFx("Assets/audio/fx/killedBySus.wav");
	playerAttackFx = app->audio->LoadFx("Assets/audio/fx/kick.wav");
	winFx = app->audio->LoadFx("Assets/audio/fx/winFx.wav");
	loseHPFx = app->audio->LoadFx("Assets/audio/fx/gnomeFx.wav");
	fallFx = app->audio->LoadFx("Assets/audio/fx/fallFx.wav");

	// HUD
	for (int i = 0; i < max_HP; i++) {
		livesHUD[i] = true;
	}
	hpCheck = HP;

	return ret;
}

bool Player::PreUpdate() {

	// DEBUG KEYS Player
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) {
		death = true;
	}

	// --------------------- HUD -------------------------

	// Player HP
	
	// Set maximun HP
	if (HP > max_HP) {
		HP = max_HP;
	}

	// Check if HP decreases
	if (hpCheck > HP) {
		//app->audio->PlayFx(loseHPFx);
		LOG("Player lose 1 HP, %d HP remaining", HP);
	}
	
	// If HP == 0 player dies
	if (HP <= 0) {
		death = true;
	}

	// Reset variable to check if changes on next frame
	hpCheck = HP;

	return true;
}

bool Player::Update(float dt)
{
	bool ret = true;

	// Disable jump if velocity at Y axis is != 0
	velY = playerBody->body->GetLinearVelocity().y;
	if ( velY != 0 ) {
		canJump = false;
	}

	if (death != true && win != true) {

		// --- Player movement ---
		if (godMode != true) {

			currentVel = METERS_TO_PIXELS(playerBody->body->GetLinearVelocity().x);

			if (currentVel < velMax && currentVel > -velMax)
			{
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)	// MOVE TO RIGHT
				{
					// --- walk ---
					if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
						if (currentVel < velSlowMax && currentVel > -velSlowMax) {
							playerBody->body->ApplyLinearImpulse(b2Vec2(speed * dt, 0), b2Vec2(0, 0), 1);

							if (currentAnimation != &walkR)
							{
								walkR.Reset();
								currentAnimation = &walkR;
								Player_Dir = true;
							}
						}
						else {
							playerBody->body->ApplyLinearImpulse(b2Vec2(-lowSpeed * dt, 0), b2Vec2(0, 0), 1);
						}
					}
					else {
						// --- Run ---	
						playerBody->body->ApplyLinearImpulse(b2Vec2(lowSpeed * dt, 0), b2Vec2(0, 0), 1);

						if (currentAnimation != &runR)
						{
							runR.Reset();
							currentAnimation = &runR;
							Player_Dir = true;
						}
					}
				}
				else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)	// MOVE TO LEFT
				{
					// --- he walk ---
					if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {

						if (currentVel < velSlowMax && currentVel > -velSlowMax) {
							playerBody->body->ApplyLinearImpulse(b2Vec2(-speed * dt, 0), b2Vec2(0, 0), 1);

							if (currentAnimation != &walkL)
							{
								walkL.Reset();
								currentAnimation = &walkL;
								Player_Dir = false;
							}
						}
						else {
							playerBody->body->ApplyLinearImpulse(b2Vec2(lowSpeed * dt, 0), b2Vec2(0, 0), 1);
						}
					} // --- He run ---
					else {

						playerBody->body->ApplyLinearImpulse(b2Vec2(-lowSpeed * dt, 0), b2Vec2(0, 0), 1);

						if (currentAnimation != &runL)
						{
							runL.Reset();
							currentAnimation = &runL;
							Player_Dir = false;
						}
					}
				}
				else {
					// ------------------------ FRICTION When stop moving --------------------------------
					if (currentVel > 3) {
						playerBody->body->ApplyLinearImpulse(b2Vec2(-speed * 0.5f * dt, 0), b2Vec2(0, 0), 1);
					}
					else if (currentVel < -16) {
						playerBody->body->ApplyLinearImpulse(b2Vec2(speed * 0.5f* dt, 0), b2Vec2(0, 0), 1);
					}
				}
			}
			// ------------------ JUMP -----------------
			if (canJump == true && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
			{
				if (dt < 20) {
					playerBody->body->ApplyForceToCenter(b2Vec2(0, -355), 1);
				}
				else {
					playerBody->body->ApplyForceToCenter(b2Vec2(0, -180), 1);
				}

				if (Player_Dir == true) {
					if (currentAnimation != &jumpR)
					{
						jumpR.Reset();
						currentAnimation = &jumpR;
					}
				}
				else {
					if (currentAnimation != &jumpL)
					{
						jumpL.Reset();
						currentAnimation = &jumpL;
					}
				}
			}
			else {
				// Allow move when jumping / falling
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && Player_Dir == true) {
					playerBody->body->ApplyLinearImpulse(b2Vec2(-speed * 0.07f * dt, 0), b2Vec2(0, 0), 1);
				}
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && Player_Dir == false) {
					playerBody->body->ApplyLinearImpulse(b2Vec2(speed * 0.07f * dt, 0), b2Vec2(0, 0), 1);
				}
			}

			// --- but most importantly, he Attack (with cooldown) ---
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && hitTimeL <= 0 && hitTimeR <= 0)
			{
				app->audio->PlayFx(playerAttackFx);
				if (Player_Dir == true) {
					if (currentAnimation != &punchR)
					{
						punchR.Reset();
						currentAnimation = &punchR;
						Player_Dir = true;

						hitTimeR = 20;
					}
				}
				else {
					if (currentAnimation != &punchL)
					{
						punchL.Reset();
						currentAnimation = &punchL;
						Player_Dir = false;

						hitTimeL = 20;
					}
				}
			}

			//LOG("R %d L %d", hitTimeR, hitTimeL);

			if (hitTimeR > 0 )
			{
				attackR = true;
				hitTimeR--;
			}
			else if (hitTimeL > 0){
				attackL = true;
				hitTimeL--;
			}
			else {
				attackR = false;
				attackL = false;
			}
		}
		else {
			GodMode();
		}
		// Get body position
		playerBody->GetPosition(position.x, position.y);
	}
	else if(death == true){
		app->physics->pause = true;
	
		// When you die, this makes the player go washingmachine 
		int w, h, width, height;
		w = -app->render->camera.x / 2 - 1280 / 4;
		h = -app->render->camera.y / 2 - 720 / 4;
		width = app->render->camera.w;
		height = app->render->camera.h;

		if (position.x > w + width / 2 -15) {
			position.x-= 5;
		}
		if (position.x < w + width / 2 -15) {
			position.x+= 5;
		}
		if (position.y > h + height / 2 -15) {
			position.y-= 5;
		}
		if (position.y < h + height / 2 -15) {
			position.y+= 5;
		}
		angle += angleV/2;
		angleV++;

		// Die sfx
		if (DieFxPlayed != true) {
			DieFxPlayed = true;
			app->audio->PlayFx(deathFx);
		}
	}
	else if (win == true) {
		
		// Win sfx
		if (WinFxPlayed != true) {
			WinFxPlayed = true;
			app->audio->PlayFx(winFx);
		}
	}
	return ret;
}

void Player::GodMode() {
	// --- GOD MODE Movement ---

	if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
	{
		playerBody->body->SetTransform(b2Vec2(playerBody->body->GetPosition().x + 0.4f, playerBody->body->GetPosition().y),playerBody->body->GetAngle());
	}
	if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
	{
		playerBody->body->SetTransform(b2Vec2(playerBody->body->GetPosition().x - 0.4f, playerBody->body->GetPosition().y), playerBody->body->GetAngle());
	}
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)) 
	{
		playerBody->body->SetTransform(b2Vec2(playerBody->body->GetPosition().x, playerBody->body->GetPosition().y + -0.4f), playerBody->body->GetAngle());
	}
	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) 
	{
		playerBody->body->SetTransform(b2Vec2(playerBody->body->GetPosition().x, playerBody->body->GetPosition().y + 0.4f), playerBody->body->GetAngle());
	}
}

bool Player::PostUpdate() {
	bool ret = true;

	// --- idle amimation ---
	if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE
		&& currentAnimation != &punchL && currentAnimation != &punchR && currentAnimation != &jumpL && currentAnimation != &jumpR))
	{
		if (currentAnimation != &idleR && Player_Dir == true)
		{
			idleR.Reset();
			currentAnimation = &idleR;
			Player_Dir = true;
		}

		if (currentAnimation != &idleL && Player_Dir == false)
		{
			idleL.Reset();
			currentAnimation = &idleL;
			Player_Dir = false;
		}
	}

	// Reset punch animation:
	if (currentAnimation == &punchL && currentAnimation->GetCurrentFrameINT() == 3) {
		idleL.Reset();
		currentAnimation = &idleL;
	}
	else {
		punchL.Update();
	}
	if (currentAnimation == &punchR && currentAnimation->GetCurrentFrameINT() == 3) {
		idleR.Reset();
		currentAnimation = &idleR;
	}
	else {
		punchR.Update();
	}

	// Jump anim:
	if (currentAnimation == &jumpL && currentAnimation->GetCurrentFrameINT() == 4) {
		runL.Reset();
		currentAnimation = &runL;
	}
	else {
		jumpL.Update();
	}
	if (currentAnimation == &jumpR && currentAnimation->GetCurrentFrameINT() == 4) {
		runR.Reset();
		currentAnimation = &runR;
	}
	else {
		jumpR.Update();
	}

	// Update Animation each frame
	if (app->getDeltaTime() > 20) {
		currentAnimation->Update();
		currentAnimation->Update();
	}
	else {
		currentAnimation->Update();
	}

	// --- Draw Player ---	
	app->render->DrawTexture(playerSprite, position.x-3, position.y-5, &currentAnimation->GetCurrentFrame(), 1.0f, angle, 34, 34); // -3 and -5 are for hitbox adjustments

	// --------------------- DRAW LIVES HUD --------------------------------------

	// Lives:

	int Xdist = 10;
	int Ydist = 10;

	switch (HP)
	{
	case 0:
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOff.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 35 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOff.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 70 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOff.GetCurrentFrame());
		break;
	case 1:
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOn.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 35 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOff.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 70 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOff.GetCurrentFrame());
		break;
	case 2:
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOn.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 35 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOn.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 70 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOff.GetCurrentFrame());
		break;
	case 3:
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOn.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 35 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOn.GetCurrentFrame());
		app->render->DrawTexture(livesSprite, (-app->render->camera.x / 2) + 70 + Xdist, (-app->render->camera.y / 2) + Ydist, &liveOn.GetCurrentFrame());
		break;
	}

	if (app->physics->playerDieCooldown != 0 && death == false && win == false) {
		app->render->DrawTexture(cooldownTex, position.x, position.y);
	}

	return ret;
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
	liveOff.DeleteAnim();
	liveOn.DeleteAnim();

	// Textures
	app->tex->UnLoad(playerSprite);
	app->tex->UnLoad(livesSprite);
	app->tex->UnLoad(cooldownTex);

	// Physbody
	app->physics->world->DestroyBody(playerBody->body);
	app->physics->world->DestroyBody(attackSensorLeft->body);
	app->physics->world->DestroyBody(attackSensorRight->body);

	// Reset variables
	currentVel = velY = position.x = position.y = initPos.x = initPos.y = angle = angleV = killedFx = deathFx = playerAttackFx = winFx = loseHPFx = fallFx = NULL;
	godMode = death = win = destroyed = DieFxPlayed = WinFxPlayed = false;

	return true;
}