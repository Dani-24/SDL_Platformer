#include "Collision.h"

#include "App.h"

#include "Render.h"
#include "Input.h"
#include "SDL/include/SDL_Scancode.h"

Collision::Collision(App* application, bool start_enabled) : Module(application, start_enabled)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::FALL] = true;

	matrix[Collider::Type::GROUND][Collider::Type::GROUND] = false;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::FALL] = false;

	matrix[Collider::Type::FALL][Collider::Type::FALL] = false;
	matrix[Collider::Type::FALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::FALL][Collider::Type::GROUND] = false;
}

// Destructor
Collision::~Collision()
{

}

//bool Collision::PreUpdate()
//{
//	// Remove all colliders scheduled for deletion
//	for (uint i = 0; i < MAX_COLLIDERS; ++i)
//	{
//		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
//		{
//			delete colliders[i];
//			colliders[i] = nullptr;
//		}
//	}
//
//	Collider* c1;
//	Collider* c2;
//
//	for (uint i = 0; i < MAX_COLLIDERS; ++i)
//	{
//		// skip empty colliders
//		if (colliders[i] == nullptr)
//			continue;
//
//		c1 = colliders[i];
//		// avoid checking collisions already checked
//		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
//		{
//			// skip empty colliders
//			if (colliders[k] == nullptr)
//				continue;
//
//			c2 = colliders[k];
//
//			if (c1->Intersects(c2->rect))
//			{
//				if (matrix[c1->type][c2->type] && c1->listener)
//					c1->listener->OnCollision(c1, c2);
//
//				if (matrix[c2->type][c1->type] && c2->listener)
//					c2->listener->OnCollision(c2, c1);
//			}
//		}
//	}
//
//	return true;
//}

//bool Collision::PreUpdate() {
//	for (uint i = 0; i < MAX_COLLIDERS; i++) {
//		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true) {
//			delete colliders[i];
//			colliders[i] = nullptr;
//		}
//	}
//
//	Collider* c1;
//	Collider* c2;
//
//	for (uint i = 0; i < MAX_COLLIDERS; i++) {
//		if (colliders[i] == nullptr) {
//			continue;
//		}
//		c1 = colliders[i];
//		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {
//			if (colliders[k] == nullptr) {
//				continue;
//			}
//			c2 = colliders[k];
//
//			if (matrix[c1->type][c2->type] && c1->Intersects(c2->rect))
//			{
//				for (uint i = 0; i < MAX_LISTENERS; ++i)
//				{
//					if (c1->listeners[i] != nullptr)
//					{
//						c1->listeners[i]->OnCollision(c1, c2);
//					}
//				}
//				for (uint i = 0; i < MAX_LISTENERS; ++i)
//				{
//					if (c2->listeners[i] != nullptr) {
//						c2->listeners[i]->OnCollision(c2, c1);
//					}
//				}
//			}
//		}
//	}
//	return true;
//}

bool Collision::PreUpdate() {
	for (uint i = 0; i < MAX_COLLIDERS; i++) {
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true) {
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; i++) {
		if (colliders[i] == nullptr) {
			continue;
		}
		c1 = colliders[i];
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {
			if (colliders[k] == nullptr) {
				continue;
			}
			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->Intersects(c2->rect))
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
				{
					if (c1->listeners[i] != nullptr)
					{
						c1->listeners[i]->OnCollision(c1, c2);
					}
				}
				for (uint i = 0; i < MAX_LISTENERS; ++i)
				{
					if (c2->listeners[i] != nullptr) {
						c2->listeners[i]->OnCollision(c2, c1);
					}
				}
			}
		}
	}
	return true;
}

bool Collision::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (debug == true) {
			debug = false;
		}
		else {
			debug = true;
		}
	}

	return true;
}

bool Collision::PostUpdate()
{
	if (debug == true)
		DebugDraw();

	return true;
}

void Collision::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // white
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::PLAYER: // blue
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case Collider::Type::GROUND: // red
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case Collider::Type::FALL: // green
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool Collision::CleanUp()
{
	//LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collision::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}