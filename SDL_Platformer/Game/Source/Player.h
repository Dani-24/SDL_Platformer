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

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() override;

	void OnCollision(PhysBody* c1, PhysBody* c2) override;

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void PlayerStartAnims();

	void GodMode();

	bool godMode;
	iPoint position;
	bool death;
	bool win;
	int HP;
	PhysBody* playerBody;

	bool canJump;

private:

	// The speed in which we move the player (pixels per frame)
	float speed;
	float lowSpeed;
	int velMax = 200;
	int velSlowMax = 50;
	int currentVel;

	bool jumping = false;

	bool Player_Dir;

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
		fallR, fallL,
		pushR, pushL,
		runR, runL,
		throwR, throwL,
		walkR, walkL,
		walkPunchR, walkPunchL;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	// Sound effects indices
	uint PlayerAttackFx = 0;
	uint AttackQuoteFx = 0;

};

#endif //!__PLAYER_H__
