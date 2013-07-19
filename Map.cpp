/** @file Map.cpp */

#include "Map.h"
#include "MapLoader.h"
#include "Game.h"
#include "Savegame.h"
#include "DialogBox.h"
#include "Sprite.h"
#include "Camera.h"
#include "LuaContext.h"
#include "FileTools.h"
#include "Surface.h"
#include "VideoManager.h"
#include "Music.h"

MapLoader Map::map_loader;

/**
* \brief Creates a map.
* \param id id of the map, used to determine the description file
* and the script file of the map
*/
Map::Map(const std::string& id):
	game(NULL),
	id(id),
	tileset(NULL),
	floor(NO_FLOOR),
	loaded(false),
	started(false),
	destination_name(""),
	entities(NULL),
	suspended(false),
	light(1) 
{
}

/**
* \brief Destructor.
*/
Map::~Map() 
{
  if(is_started())
  {
     std::cerr << "Deleting a map that is still running. Call Map::leave() before.\n";
  }
  if (is_loaded()) 
  {
    unload();
  }
}

/**
* \brief Returns the id of the map.
* \return the map id
*/
const std::string& Map::get_id() 
{
  return id;
}

/**
* \brief Returns the tileset associated to this map.
* \return the tileset
*/
Tileset& Map::get_tileset() 
{
  return *tileset;
}

/**
* \brief Returns the id of the tileset associated to this map.
* \return the id of the tileset
*/
const std::string& Map::get_tileset_id() 
{
  // note that if set_tileset() has been called, tileset_id != tileset->get_id()
  return tileset_id;
}










