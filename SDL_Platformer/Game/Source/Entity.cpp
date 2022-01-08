#include "Entity.h"
#include "App.h"
#include "Scene.h"
#include "Textures.h"
#include "Module.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Animation.h"
#include "Physics.h"
#include "Player.h"
#include "Map.h"

#include <time.h>  

Entity::Entity(App* application, bool start_enabled) : Module(application, start_enabled) {
	name.Create("entity");
}

Entity::~Entity() {

}

bool Entity::Start() {

	return true;
}
bool Entity::PreUpdate() {

	return true;
}
bool Entity::Update(float dt) {

	return true;
}
bool Entity::PostUpdate() {

	return true;
}
bool Entity::CleanUp(){

	return true;
}

// Juntar Willycoin y Enemy aquí siguiendo el handout/solution de entidades

// Cuando sea funcional borrar willycoin y enemy ???
// - App.cpp
// - App.h
// - Scene.cpp