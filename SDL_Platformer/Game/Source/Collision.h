#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#define MAX_COLLIDERS 1000

#include "Module.h"
#include "Collider.h"

class Collision : public Module
{
public:
	// Constructor
	// Fills all collision matrix data
	Collision(App* application, bool start_enabled = true);

	// Destructor
	~Collision();

	// Called at the beginning of the application loop
	// Removes all colliders pending to delete
	// Checks for new collisions and calls its listeners
	bool PreUpdate();

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	bool Update();

	// Called at the end of the application loop
	// Draw all colliders (if debug mode is enabled)
	bool PostUpdate();

	// Removes all existing colliders
	bool CleanUp();

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

	// Draws all existing colliders with some transparency
	void DebugDraw();

	// The collision matrix. Defines the interaction for two collider types
	// If set two false, collider 1 will ignore collider 2
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
	bool debug = false;

	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	

private:
	// All existing colliders in the scene
	
	// Simple debugging flag to draw all colliders
	
};

#endif // __COLLISIONS_H__
