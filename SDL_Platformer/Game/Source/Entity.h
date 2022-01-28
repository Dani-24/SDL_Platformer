#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "App.h"
#include "Animation.h"
#include "List.h"
#include "Point.h"

//Oh, the misery
//Everybody wants to be my entity
//Spare the sympathy
//Everybody wants to be my entity - y - y - y - y
//(Look out for yourself..!)
//My enemy - y - y - y - y
//(Look out for yourself..!)
//But I'm ready 
//...

enum class EntityType
{
	PLAYER,
	ENEMY_WALK,
	ENEMY_FLY,
	ITEM_COIN,
	ITEM_HEAL,
	ITEM_WILLYCOIN
};

class Entity : public Module {
public:
	Entity(App* application, bool start_enabled);
	~Entity();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() override;

private:

};

#endif // !__ENTITY_H__