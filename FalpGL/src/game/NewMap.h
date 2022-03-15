#pragma once
#include "../renderer/RendererIncludes.h"
#include <fstream>

/*
*  objects depth varies in chunk, resets every chunk
*  when player(or other thing) goes up or down, snap depth to 0 or 1
*  no need to change depth for y
*  bad clipping at edge of chunk? yes.
* 
* 
*  change map object depth in vetex shader to y:
* 
*       1: things that need to occlude thing
*       2: map
*                      1    2
*                      \     |
*                       \    |
*    front of screen     \   |       back of screen
*                         \  |
*                          \ |
*                           \|
* 
*      y2 = lowest y coord for every quad
*/


/*
* 330 tiles/min
* 
* 
*  3 * 3 loaded and rendering
*  5 * 5 loaded
*  
* 
*/


int idx(float x, float y);
Point index_to_coord_64(float idx);

struct Tile {
	/*
	* X  Y  S  T
	* X  Y  S  T
	* X  Y  S  T
	* X  Y  S  T
	* 
	* 0  1  2  3
	* 4  5  6  7
	* 8  9  10 11
	* 12 13 14 15
	*/
private:
	float quad_data[16];

public:
	Tile(float position_x, float position_y, float texture_x, float texture_y, bool solid);
	Tile(float position_x, float position_y, nlohmann::json tile_json);
	Tile();
};

class Chunk {
private:
	bool loaded;
	const int chunk_size = 64;
	float position[2];
	int id;
	std::vector<Tile> tiles;
	std::vector<std::vector<bool>> collision_map;

	/* json containing what tile should be at what position - scoped to specific chunk */
	nlohmann::json *chunk_json;

	/* json with texture coords for each tile */
	nlohmann::json *tile_json;
	
public:
	Chunk(float x, float y, nlohmann::json* c_json, nlohmann::json* t_json);
	Chunk();

	void load();
	void unload();

	bool collision_at(float x, float y);

	const void* vertex_data() const;
	const int get_id() const;
};

class New_Map {
private:
	Shader map_shader;
	VertexArray map_vertex_array;
	VertexBuffer map_vertex_buffer;
	IndexBuffer map_index_buffer;

	const std::string chunk_json_filename = "res/data/map.json";
	const std::string tile_json_filename = "res/data/tiles.json";

	nlohmann::json chunk_json;
	nlohmann::json tile_json;

	int samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	float texture_index = 0.0f;
	int loaded_chunks[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	std::vector<Chunk> chunks;

	void chunk_to_buffer(Chunk* c);
public:
	New_Map();

	void draw(glm::mat4 matrix);
	Point collision_line_desination(Point origin, Point desination, float collision_radius);
	Point collision_line_delta(Point origin, Point delta, float collision_radius);

	bool init();

};


class decoration {
	/*
	* for things that you can walk behind but don't interact with
	*/
};

class placeholder_active_object {
	/*
	* for things that you can walk behind and interact with
	*/
};