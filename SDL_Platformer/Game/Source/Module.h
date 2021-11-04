#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;

class Module
{
public:
	App* Application;
	Module(App* parent, bool startActive) : Application(parent), active(startActive)
	{

	}

	void Init()
	{
		active = true;
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

public:

	SString name;
	bool active = true;

};

#endif // __MODULE_H__