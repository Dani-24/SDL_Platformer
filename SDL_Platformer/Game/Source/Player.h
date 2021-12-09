#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "App.h"
#include "Animation.h"
#include "List.h"
#include "Point.h"
#define MaxShoots 3

struct SDL_Texture;
struct Collider;

class Player : public Module
{
public:
	// Constructor
	Player(App* application, bool start_enabled);

	// Destructor
	~Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() override;

	void PlayerStartAnims();

	void GodMode();

public:
	bool godMode;
	iPoint position;
	int angle = 0, angleV = 1;
	iPoint initPos;
	iPoint mapLimit;
	bool death = false;
	bool win = false;
	int HP, max_HP = 3;
	PhysBody* playerBody;

	bool canJump;

	int velY;

private:

	// Chain for player sprite
	int playerChain[16]{
		0, 2,
		0, 24,
		2, 26,
		24, 26,
		26, 24,
		26, 2,
		24, 0,
		2, 0
	};

	// The speed in which we move the player (pixels per frame)
	float speed;
	float lowSpeed;
	int velMax = 150;
	int velSlowMax = 50;
	int currentVel;

	bool Player_Dir;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* playerSprite = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation playerR, playerL,
		punchR, punchL,
		doublePunchR, doublePunchL,
		climb,
		deathR, deathL,
		hitR, hitL,
		idleR, idleL,
		jumpR, jumpL,
		fallR, fallL,
		pushR, pushL,
		runR, runL,
		throwR, throwL,
		walkR, walkL,
		walkPunchR, walkPunchL;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	// Sound effects indices
	uint playerAttackFx = 0;
	uint killedFx = 0;
	uint deathFx = 0;

	uint winFx = 0;

	bool DieFxPlayed = false;
	bool WinFxPlayed = false;

};

#endif //!__PLAYER_H__
