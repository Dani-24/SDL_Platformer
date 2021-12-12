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

	SDL_Texture* sprite;
	PhysBody* body;

	Animation* currentAnimation = nullptr;

	Animation coinSpinAnim, coinFlashAnim;

	int type;
	int cont = 0;
	
	bool spawn;
	iPoint position;
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

	List<Items*> items;

	int itemNum;
	void AddItem(int x, int y, int type);
	uint coinFx = 0;
	uint willyFx = 0;
	uint foodFx = 0;

private:

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* ItemSprite = nullptr;
	SDL_Texture* Willycoin;

	Animation coinSpin, coinFlash, coinIdle, lollipop, candy, chocolate, can, iceCream1, iceCream2, cherry, banana, strawberry, apple, orange;
	
};

#endif // !__WILLCOIN_H__