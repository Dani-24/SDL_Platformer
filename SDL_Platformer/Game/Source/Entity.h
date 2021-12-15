#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "App.h"
#include "List.h"
#include "Animation.h"
#include "Point.h"

//Todo esto fue
//Cuando internet empezaba
//Cod por tos' lados
//Y algun otro que subía
//Algo de su casa
//Tutoriales yo busqué
//Pa' sacarme partidazas
//No veas este tio
//Como mata
//Juega tranquilo y pasivo
//Cuando de mapa le sale hardhat
//Aunque reventará
//Con todas las armas
//Viva willyrex
//Viva willyrex
//Tiene muchas moabs
//En el warfare 3
//Viva willyrex
//Viva willyrex
//Imagina si abre
//Los ojos pa' ver

//---------

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

struct Entities {
	SDL_Texture* sprite; //entity texture
	PhysBody* body, * collider; //The entity Collider
	float speed;

	//Animation
	Animation* currentAnimation = nullptr;
	Animation coinSpinAnim, coinFlashAnim, animRunL, animRunR, animDieL, animDieR;

	//TYPE: 1 = DEFAULT ENEMY, 2 = FLY ENEMY, 3 = COIN, 4 = WILLYCOIN, 5 = FOOD
	int type;

	int cont = 0;
	bool spawn, dir, playDetectFx; //if the item is on the map
	bool alert = false;
	bool lost = false;
	iPoint position; //The item position
	bool dieAnim = false;
};



class Item : public Module
{
public:
	// Constructor
	Item(App* application, bool start_enabled);

	// Destructor
	~Item();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() override;

	//All the items saved in a list
	//List<Items*> items;

	int itemNum;

	//Add an item to the Items List: POS X, POS Y, TYPE (1 = COIN, 2 = WILLYCOIN, 3 = FOOD)
	void AddItem(int x, int y, int type);
	
	//Items FX

	uint coinFx = 0;
	uint willyFx = 0;
	uint foodFx = 0;

private:

	// The ITEMS spritesheet loaded into an SDL_Texture
	SDL_Texture* ItemSprite = nullptr;
	SDL_Texture* Willycoin; // The WILLYCOIN texture loaded into an SDL_Texture

	//Items Animations
	Animation coinSpin, coinFlash, coinIdle, lollipop, candy, chocolate, can, iceCream1, iceCream2, cherry, banana, strawberry, apple, orange;
	
};

#endif // !__ENTITY_H__