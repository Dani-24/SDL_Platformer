#ifndef __ENTITY_H__
#define __ENTITY_H__

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
//Everybody wants to be my entity
//Spare the sympathy
//Everybody wants to be my entity - y - y - y - y
//(Look out for yourself..!)
//My enemy - y - y - y - y
//(Look out for yourself..!)
//But I'm ready 
//...

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