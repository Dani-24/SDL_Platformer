#ifndef __WILLCOIN_H__
#define __WILLCOIN_H__

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

struct SDL_Texture;

struct Items {
	SDL_Texture* sprite; //Item texture
	PhysBody* body; //The item Collider

	//Animation
	Animation* currentAnimation = nullptr;
	Animation coinSpinAnim, coinFlashAnim;

	//TYPE: 1 = COIN, 2 = WILLYCOIN, 3 = FOOD
	int type;
	int cont = 0;
	
	bool spawn; //if the item is on the map
	iPoint position; //The item position
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
	List<Items*> items;

	//Add an item to the Items List: POS X, POS Y, TYPE (1 = COIN, 2 = WILLYCOIN, 3 = FOOD)
	void AddItem(int x, int y, int type);
	
	//Items FX

	uint coinFx = 0;
	uint willyFx = 0;
	uint foodFx = 0;

	// The ITEMS spritesheet loaded into an SDL_Texture
	SDL_Texture* ItemSprite = nullptr;
	SDL_Texture* Willycoin; // The WILLYCOIN texture loaded into an SDL_Texture

	//Items Animations
	Animation coinSpin, coinFlash, coinIdle, lollipop, candy, chocolate, can, iceCream1, iceCream2, cherry, banana, strawberry, apple, orange;
};

#endif // !__WILLCOIN_H__