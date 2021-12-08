#ifndef __WILLCOIN_H__
#define __WILLCOIN_H__

#include "Module.h"
#include "App.h"
#include "List.h"
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

struct Coins {

	SDL_Texture* sprite;
	PhysBody* body;

	bool live;
	iPoint position;
};

class Coin : public Module
{
public:
	// Constructor
	Coin(App* application, bool start_enabled);

	// Destructor
	~Coin();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() override;

	List<Coins*> coins;

	void AddCoin(int x, int y);
	uint coinFx = 0;

private:

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* CoinTex;
	// Sound effects indices
	
};

#endif // !__WILLCOIN_H__