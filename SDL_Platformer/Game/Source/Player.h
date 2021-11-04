#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "App.h"
#include "Animation.h"
#include "List.h"
#include "Point.h"

struct SDL_Texture;
struct Collider;

#define Max_HP 10;

class Player : public Module
{
public:
	// Constructor
	Player(App* application, bool start_enabled);

	// Destructor
	~Player();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start();

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate();

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2);

	void GodMode();
	// Position of the player in the map

	bool CleanUp() override;

	iPoint position;
	bool death = false;



private:

	bool isEnabled = true;
	// The speed in which we move the player (pixels per frame)
	int speed = 3;
	int lowSpeed = 1;

	bool Player_Position = true;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;

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
		pushR, pushL,
		runR, runL,
		throwR, throwL,
		walkR, walkL,
		walkPunchR, walkPunchL;	

	// The player's collider
	Collider* collider = nullptr;
	Collider* colliderAttack = nullptr;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	// A countdown to when the player gets destroyed. After a while, the game exits
	uint destroyedCountdown = 120;

	// Sound effects indices
	uint PlayerAttackFx = 0;
	uint AttackQuoteFx = 0;


	bool god = false;

	int HP = Max_HP;
};

#endif //!__PLAYER_H__
