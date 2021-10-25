
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

    // L03: DONE 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)
	/*
    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;

    while (tileset != NULL) 
	{
        app->render->DrawTexture(tileset->data->texture,0,0);
        tileset = tileset->next;
    }
	*/
	
	// L04: TODO 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

	// L04: TODO 9: Complete the draw function (inside the loop from TODO 5)
	// Find which tile id is on x, y coordinates 
	// Find out that Tile’s Rect inside the tileset Image (
	// Find out where in the World(screen) we have to draw
	// DrawTexture()


}

// L04: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	//ret.x = //..;
	//ret.y = //..;

	return ret;
}

// L04: TODO 7:Implement the method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	bool ret = true; 

    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.clear();

	// L04: TODO 2: clean up all layer data
	// Remove all layers

	return true;

}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_document mapFile; 
    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

    if(ret == true)
    {
		ret = LoadMap(mapFile);
	}
	
    if (ret == true)
    {
        ret = LoadTileSets(mapFile.child("map"));
    }

	// L04: TODO 4: Iterate all layers and load each of them
	if (ret == true)
	{
		ret = LoadAllLayers(mapFile.child("map"));
	}

    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything


		// L04: TODO 4: LOG the info for each loaded layer
    }
	
	// Clean up the pugui tree
	if(mapFile) mapFile.reset();

    mapLoaded = ret;

    return ret;
}

// L03: DONE 3: Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE 3: Load map general properties
		mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
	}

	return ret;
}

// L03: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSets(pugi::xml_node mapFile) {

	bool ret = true;

	pugi::xml_node tileset;
	for (tileset = mapFile.child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();
		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);
		mapData.tilesets.add(set);
	}

	return ret;
}

// L03: TODO 4: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE 4: Load Tileset attributes
	set->name.Create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tilecount = tileset_node.attribute("tilecount").as_int();
	set->columns = tileset_node.attribute("columns").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	return ret;
}

// L03: DONE 4: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE 4: Load Tileset image
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		
		//SDL_Surface* surface = IMG_Load(tmp.GetString()); //Need include #include "SDL_image/include/SDL_image.h"
		//set->texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

		//NOTE: This line simplifies the loading
		set->texture = app->tex->Load(tmp.GetString());
	}

	return ret;
}

// L04: TODO 3: Implement the LoadLayer function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	//Load the attributes and assign to the layer variables
	
	//Initialize the tile array and reserve the memory 
	
	//Iterate over all the tiles in the xml and assign the values

	return ret;
}

// L04: TODO 4: Iterate all layers and load each of them
// This function is call from Mapp::Load()
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
	
	bool ret = true; 
	
	//Iterate ovel all layers in the XML and call the individual LoadLayer()

	return ret;
}
