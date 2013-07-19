/** @file Map.h */

#ifndef KQ_MAP_H
#define KQ_MAP_H

#include "Common.h"
#include "Transition.h"
#include "Rectangle.h"
#include "ExportableToLua.h"

class Map: public ExportableToLua
{
private:
	friend class MapLoader;		//Modifies the private fields of Map
	
	static MapLoader map_loader;	/**< The map file parser */
	
	//Map properties
	Game* game;					/**< the game this map was started in */
	std::string id;				/**< id of the map */
	
	int width8;					/**< map width in 8*8 squares (width8 = get_width() / 8) */
	int height8;				/**< map height in 8*8 squares (height8 = get_height() / 8) */
	
	std::string tileset_id;		/**< id of the current tileset */
	Tileset* tileset			/**< tileset of the map:  every tile of this map 
								 * is extracted from this tileset */
								 
	std::string music_id;		/**< id of the background music of the map:
								 * can be a valid music, Music::none or Music::unchanged */
								 
	std::string world;			/**< Name of the context where this map is. When changing context,
								 * the savegame starting position is set and crystal switches are reset. */
								 
	int floor;					/**< The floor where this map is (possibly NO_FLOOR). */

	Rectangle location			/**< location of the map in its context: the width and height fields
								 * indicate the map size in pixels, and the x and y field indicate the position:
								 * - in the outside world: location of the map's top-left corner
								 * relative to the whole world map
								 * - in the inside world: location of the map relative to the whole world map
								 * - in a dungeon: location of the map's top-left corner relative to the whole floor */
	
	//Screen
	
	Camera* camera;				/**< determines the visible area of the map */
	Surface* visible_surface;	/**< surface where the map is displayed - this surface is only the visible part
								 * of the map, so the coordinates on this surface are relative to the screen,
								 * not to the map */
	
	Rectangle clipping_rectangle;  /**< when drawing the map, indicates an area of the surface to be restricted to
									* (usually, the whole map is considered and this rectangle's values are all 0) */

	//Map state
	bool loaded;				  /**< true if the loading phase is finished */
	bool started;				  /**< true if this map is the current map */	
	std::string destination_name; /**< current destination point on the map,
									* or "_same" to keep the hero's coordinates,
									* or "_side0", "_side1", "_side2" or "_side3"
									* to place the hero on a side of the map,
									* or an empty string to use the one saved. */

	MapEntities* entities;		/**< the entities on the map */
	bool suspended;				/**< indicates whether the game is suspended */
	
	//Light
	int light;					/**< light level (0: dark, 1: full light) */
	Surface* dark_surfaces[4];	/**< dark foreground shown when there is no light */
	
	void set_suspended(bool suspended);
	void draw_background();
	void draw_foreground();
	
public:

	// creation and destruction
    Map(const std::string& id);
    ~Map();

    // map properties
    const std::string& get_id();
    Tileset& get_tileset();
    const std::string& get_tileset_id();
    void set_tileset(const std::string& tileset_id);
    const std::string& get_world();
    void set_world(const std::string& world);
    bool has_floor();
    int get_floor();
    void set_floor(int floor);
    const Rectangle& get_location();

    int get_width();
    int get_height();
    int get_width8();
    int get_height8();

    // camera
    Surface& get_visible_surface();
    const Rectangle& get_camera_position();
    void move_camera(int x, int y, int speed);
    void restore_camera();
    bool is_camera_fixed_on_hero();
    void set_clipping_rectangle(const Rectangle &clipping_rectangle = Rectangle());
    int get_light();
    void set_light(int light);

    // loading
    bool is_loaded();
    void load(Game &game);
    void unload();
    Game& get_game();
    LuaContext& get_lua_context();
    virtual const std::string& get_lua_type_name() const;

    void notify_opening_transition_finished();

    // entities
    MapEntities& get_entities();

    // presence of the hero
    bool is_started();
    void start();
    void leave();

    // current destination point
    void set_destination(const std::string &destination_name);
    const std::string& get_destination_name();
    Destination* get_destination();
    int get_destination_side();

    // collisions with obstacles (checked before a move)
    bool test_collision_with_border(int x, int y);
    bool test_collision_with_border(const Rectangle &collision_box);
    bool test_collision_with_tiles(Layer layer, int x, int y, MapEntity &entity_to_check);
    bool test_collision_with_entities(Layer layer, const Rectangle &collision_box, MapEntity &entity_to_check);
    bool test_collision_with_obstacles(Layer layer, const Rectangle &collision_box, MapEntity &entity_to_check);
    bool test_collision_with_obstacles(Layer layer, int x, int y, MapEntity &entity_to_check);
    Ground get_tile_ground(Layer layer, int x, int y);
    Ground get_tile_ground(Layer layer, const Rectangle &coordinates);
    bool has_empty_tiles(Layer layer, const Rectangle& collision_box);

    // collisions with detectors (checked after a move)
    void check_collision_with_detectors(MapEntity &entity);
    void check_collision_with_detectors(MapEntity &entity, Sprite &sprite);
    static Ground obstacle_to_ground(Obstacle obstacle);

    // main loop
    bool notify_input(InputEvent &event);
    void update();
    bool is_suspended();
    void check_suspended();
    void draw();
    void draw_sprite(Sprite &sprite, const Rectangle &xy);
    void draw_sprite(Sprite &sprite, int x, int y);

    static const int NO_FLOOR = -9999; /**< Represents a non-existent floor (nil in data files). */
};
#endif