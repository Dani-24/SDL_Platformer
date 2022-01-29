#include "Physics.h"
#include "Input.h"
#include "Render.h"
#include "Point.h"
#include "math.h"
#include "App.h"
#include "Window.h"
#include "Player.h"
#include "Enemy.h"
#include "Audio.h"
#include "Willycoin.h"
#include "Map.h"
#include "Scene.h"
#include "FadeToBlack.h"

Physics::Physics(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("physics");

	// Initialise all the internal class variables, at least to NULL pointer
	world = NULL;
	ground = NULL;
	mouse_joint = NULL;
	mouse_body = NULL;

	// Change this to show/hide colliders :D
	debug = false;
}

Physics::~Physics() {

}

bool Physics::Start() {
	LOG("Loading Physics");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	CreateScenarioGround();

	b2BodyDef bd;
	CreateScenarioGround();
	ground = world->CreateBody(&bd);

	return true;
}

bool Physics::PreUpdate() {

	if (pause != true) {
		
		// Physics framerate:
		dt = app->getDeltaTime();

		if (dt > 20) {
			world->Step(1.0f / 30.0f, 6, 2);
		}
		else {
			world->Step(1.0f / 60.0f, 6, 2);
		}

		for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
		{
			// For each contact detected by Box2D, see if the first one colliding is a sensor
			if (c->GetFixtureA()->IsSensor() && c->IsTouching())
			{
				// If so, we call the OnCollision listener function (only of the sensor), passing as inputs our custom PhysBody classes
				PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
				PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
				if (pb1 && pb2 && pb1->listener)
					pb1->listener->OnCollision(pb1, pb2);
			}
		}
	}

	// Reset count

	// Hit enemy
	if (playerDieCooldown > 0) {
		playerDieCooldown--;
	}
	else {
		playerDieCooldown = 0;
	}

	// Recover HP
	if (healthCont > 0) {
		healthCont--;
	}
	else {
		healthCont = 0;
	}

	return true;
}

bool Physics::PostUpdate() {

	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		debug = !debug;
		LOG("Physics debug enabled/disabled");
	}

	// If we are not in debug mode, do nothing else here and Keep playing
	if (!debug) {
		return true;
	}

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		// Solo dibujar colliders si estan cerca del player
		int Xdiff = METERS_TO_PIXELS(b->GetPosition().x) - app->player->position.x;
		int Ydiff = METERS_TO_PIXELS(b->GetPosition().y) - app->player->position.y;
		int spawnDist = 96;
		if (Xdiff <= spawnDist && Ydiff <= spawnDist && Xdiff >= -spawnDist && Ydiff >= -spawnDist) {
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					app->render->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 255);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 255);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 0);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 0);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
				}

			}
		}
	}
	return true;
}

bool Physics::CleanUp() {

	LOG("Destroying the amazing incredible physics world");

	delete world;

	pause = false;
	playerDieCooldown = fallCont = healthCont = 0;

	return true;
}

void Physics::CreateScenarioGround() {

	// Get coordinates of the screen center and radius
	int x, y, diameter;
	app->win->GetWindowSize(x, y);
	x = x * -2;
	y = y / 1.5f;
	diameter = x / 2;

	// Create a static body in the middle of the screen
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add this static body to the World
	b2Body* big_ball = world->CreateBody(&body);

	// Create a big circle shape
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	// Create a fixture and associate the circle to it
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add the ficture (plus shape) to the static body
	big_ball->CreateFixture(&fixture);
}

void Physics::BeginContact(b2Contact* contact)
{
	// Call the OnCollision listener function to bodies A and B, passing as inputs our custom PhysBody classes
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();
	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);
	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);

	// Jump Collision
	if (physA == app->player->playerBody && physB->type == "ground") {
		app->player->canJump = true;
	}

	if (physA == app->player->playerBody && physB->type == "brick") {
		if (physA->body->GetPosition().y <= physB->body->GetPosition().y) {
			app->player->canJump = true;
		}
	}

	// Die Collision
	if (app->player->godMode == false) {
		if (physA == app->player->playerBody && physB->type == "enemy") {
			if (playerDieCooldown <= 0) {
				app->audio->PlayFx(app->player->loseHPFx);
				app->player->HP -= 1;
				playerDieCooldown = 200;
			}
		}
		if (physA == app->player->playerBody && physB->type == "death") {

			//Reaparecer en el último punto de guardado, si no muere directamente
			if (app->player->saved == true) {
				app->LoadGameRequest();
				app->audio->PlayFx(app->player->fallFx);
			}
			else {
				app->player->death = true;
				app->fade->StartFadeToBlack(app->scene, app->scene, 120);
			}
		}
	}

	// Check enemy collision that kills them
	
	for (ListItem<Enemies*>* c = app->enemy->enemies.start; c != NULL; c = c->next) {
		if (physA == c->data->body && physB->type == "death") {
			LOG("Enemy fall from map");
			if (app->enemy->DeleteEnemy(c->data) == true) {
				physA == NULL;
				break;
			}
		}
		if (physA == c->data->body && physB == app->player->attackSensorLeft && app->player->attackL == true) {
			LOG("Enemy killed");
			if (app->enemy->DeleteEnemy(c->data) == true) {
				break;
			}
		}
		if (physA == c->data->body && physB == app->player->attackSensorRight && app->player->attackR == true) {
			LOG("Enemy killed");
			if (app->enemy->DeleteEnemy(c->data) == true) {
				break;
			}
		}
	}

	//Coin collision
	ListItem<Items*>* d = app->item->items.start;
	while (d != NULL) {
		switch (d->data->type)
		{
		case 1:
			if (physA == app->player->playerBody && physB == d->data->body) {
				app->audio->PlayFx(app->item->coinFx);
				d->data->spawn = false;
			}
			d = d->next;
			break;
		case 2:
			if (physA == app->player->playerBody && physB == d->data->body) {
				app->audio->PlayFx(app->item->willyFx);
				d->data->spawn = false;
			}
			d = d->next;
			break;
		case 3:
			if (physA == app->player->playerBody && physB == d->data->body) {
				app->audio->PlayFx(app->item->foodFx);
				if (healthCont <= 0) {
					app->player->HP += 1;
					healthCont = 120;
				}
				d->data->spawn = false;
			}
			d = d->next;
			break;
		}
	}

	// WIN 
	if (physA == app->player->playerBody && physB->type == "win") {
		app->player->win = true;
	}

	// Checkpoint collider
	if (physA == app->player->playerBody && physB == app->scene->checkPointSensor) {
		app->scene->checked = true;
	}
}

PhysBody* Physics::CreateChain(int x, int y, int* points, int size) {
	
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];
	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.CreateLoop(p, size / 2);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Clean-up temp array
	delete p;

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	// Return our PhysBody class
	return pbody;
}

PhysBody* Physics::CreateSolidChain(int x, int y, int* points, int size) {
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];
	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.CreateLoop(p, size / 2);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Clean-up temp array
	delete p;

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	// Return our PhysBody class
	return pbody;
}

PhysBody* Physics::CreateCircle(int x, int y, int radius)
{
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	// Return our PhysBody class
	return pbody;
}

PhysBody* Physics::CreateRectangle(int x, int y, int width, int height, SString type)
{
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	if (type != "default") {
		pbody->type = type;
	}

	// Return our PhysBody class
	return pbody;
}

PhysBody* Physics::CreateRectangleSensor(int x, int y, int width, int height)
{
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE (small "box" rectangle is ok; otherwise create whatever you need)
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true; // Set this fixture as SENSOR type

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	// Return our PhysBody class
	return pbody;
}

// PHYSBODY PART -------------------------------------------------------------------------------

PhysBody::PhysBody() : listener(NULL), body(NULL)
{
	// Initialize all internal class variables
	width = height = 0;
	body = NULL;
	listener = NULL;
}

PhysBody::~PhysBody()
{
	// Destroy the associated Box2D body
	body->GetWorld()->DestroyBody(body);
}

void PhysBody::GetPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx * fx) + (fy * fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}
