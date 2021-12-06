#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "App.h"
#include "Animation.h"
#include "List.h"
#include "Point.h"

//(Look out for yourself..!)
//I wake up to the sounds of the silence that allows
//For my mind to run around, with my ear up to the ground
//I'm searching to behold the stories that are told
//When my back is to the world that was smiling when I turned
//
//Tell you you're the greatest
//But once you turn, they hate us
//
//Oh, the misery
//Everybody wants to be my enemy
//Spare the sympathy
//Everybody wants to be my enemy - y - y - y - y
//(Look out for yourself..!)
//My enemy - y - y - y - y
//(Look out for yourself..!)
//But I'm ready

struct SDL_Texture;

struct Enemies {

	SDL_Texture* sprite;
	PhysBody* body;

	iPoint position;

	bool death, dir, playDetectFx;
	int speed;

	Animation* currentAnimation = nullptr;
};

class Enemy : public Module
{
public:
	// Constructor
	Enemy(App* application, bool start_enabled);

	// Destructor
	~Enemy();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() override;

	List<Enemies*> enemies;

	void AddEnemy(int x, int y);

private:

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* enemySprite = nullptr;

	// Animations
	Animation animRunL, animRunR, animDieL, animDieR;

	// Sound effects indices
	uint detectPlayerFx = 0;
	uint deathFx = 0;

	// Chain collider
	int enemyChain[16]{ // Enemy is 40x40
		0, 2,
		0, 38,
		2, 40,
		38, 40,
		40, 38,
		40, 2,
		38, 0,
		2, 0
	};

};

#endif // !__ENEMY_H__