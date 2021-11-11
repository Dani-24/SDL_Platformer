#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class PhysBody;

class Module
{
public:

	SString name;
	bool active = true;

public:

	App* Application;
	Module(App* parent, bool startEnabled) : Application(parent), active(startEnabled)
	{

	}

	void Init()
	{

	}

	// Called before render is available
	// L01: DONE 5: Sending config file to all modules
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnCollision(PhysBody* c1, PhysBody* c2)
	{

	}

	// L02: DONE 2: Create new virtual methods to Load / Save state
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&) const
	{
		return true;
	}

	virtual void Enable() {
		if (!active) {
			active = true;
			Start();
		}
	}

	virtual void Disable() {
		if (active) {
			active = false;
			CleanUp();
		}
	}

	inline bool isEnabled() {
		return active;
	}
};


#endif // __MODULE_H__