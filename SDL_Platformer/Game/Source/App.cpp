#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "Scene.h"
#include "Player.h"
#include "Map.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Enemy.h"
#include "Willycoin.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

#include <sstream>
#include "SString.h"
using namespace std;

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	win = new Window(this);
	input = new Input(this);
	render = new Render(this);
	tex = new Textures(this);
	audio = new Audio(this);
	fade = new FadeToBlack(this);

	sceneLogo = new SceneLogo(this, true);
	sceneTitle = new SceneTitle(this, false);
	scene = new Scene(this, false);
	player = new Player(this, false);
	enemy = new Enemy(this, false);
	coin = new Coin(this, false);

	map = new Map(this, false);
	physics = new Physics(this, false);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);

	AddModule(fade);
	AddModule(sceneLogo);
	AddModule(sceneTitle);
	AddModule(scene);
	AddModule(map);

	AddModule(player);
	AddModule(enemy);
	AddModule(coin);

	AddModule(physics);

	// Render last to swap buffer
	AddModule(render);

	ptimer = new PerfTimer();
	frameDuration = new PerfTimer();
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
	
		// Read from config file your framerate cap
		maxFrameRate = configApp.child("frcap").attribute("value").as_int();

	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	startupTime.Start();
	lastSecFrameTime.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->isEnabled() == true) {
			ret = item->data->Start();
		}
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;
	
	// Calculate the dt: differential time since last frame
	dt = frameDuration->ReadMs();
	frameDuration->Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// Load / Save methods
	if (loadGameRequested == true) {
		LoadGame();
		loadGameRequested == false;
	}

	if (saveGameRequested == true) SaveGame();

	float secondsSinceStartup = startupTime.ReadSec();

	if (lastSecFrameTime.Read() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	// Window Title
	static char titleInfo[256];

	if (app->render->vSyncOn == true) {
		sprintf_s(titleInfo, 256, "FPS: %i || Av.FPS: %.2f || Last-frame MS: %.3f || Vsync: ON ", framesPerSecond, averageFps, dt);
	}
	else {
		sprintf_s(titleInfo, 256, "FPS: %i || Av.FPS: %.2f || Last-frame MS: %.3f || Vsync: OFF ", framesPerSecond, averageFps, dt);
	}

	strcat_s(titleInfo, " || ");
	strcat_s(titleInfo, title.GetString());

	float delay = float(maxFrameRate) - frameDuration->ReadMs();

	// Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	PerfTimer* delayt = new PerfTimer();
	delayt->Start();
	if (maxFrameRate > 0 && delay > 0) SDL_Delay(delay);

	app->win->SetTitle(titleInfo);

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		setFrameRateCap();
	}
}

void App::setFrameRateCap() {
	if (maxFrameRate != 34) {
		maxFrameRate = 34;		// 30 fps
	}
	else{
		maxFrameRate = 16;		// 60 fps
	}
}

float App::getFrameRate() {
	return framesPerSecond;
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	item = modules.start;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		if (item->data->isEnabled() == true) {
			ret = item->data->PreUpdate();
		}
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		// Send dt as an argument to all updates
		if (item->data->isEnabled() == true) {
			ret = item->data->Update(dt);
		}
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item; 
	item = modules.start;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		if (item->data->isEnabled() == true) {
			ret = item->data->PostUpdate();
		}
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		if (item->data->isEnabled() == true) {
			ret = item->data->CleanUp();
		}
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

// ---------------------------------------
// Create a method to actually load an xml file then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file save_game.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

// Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}

	ret = saveDoc->save_file("save_game.xml");

	saveGameRequested = false;

	return ret;
}