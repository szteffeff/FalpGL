#pragma once
#include "../renderer/RendererIncludes.h"
#include "Decoration.h"
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
* TODO: use embedded tileset
*/

int idx(float x, float y);
int idx(int x, int y);
Point index_to_coord_64(int idx);

struct n_Tile {
private:
	float quad_data[16];

public:
	n_Tile(Prototype_Tile tile_in, float position[2]);
	n_Tile();
};

class Chunk {
private:
	bool loaded;
	size_t chunk_size[2];
	float position[2];

	std::vector<n_Tile> tiles;
	std::vector<int> chunk_data;
	
	Tileset& tileset;

public:
	Point get_position() const;

	Chunk(Tileset& set, std::vector<int>& data, float position_x, float position_y, int size_x, int size_y);


	void load();

	void unload();

	int tile_at(int x, int y);

	const void* vertex_data() const;

};

class New_Map {
private:
	Shader map_shader;
	VertexArray map_vertex_array;
	VertexBuffer map_vertex_buffer;
	IndexBuffer map_index_buffer;

	const std::string map_json_filename = "res/data/map3.json";

	nlohmann::json map_json;

	int samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	float texture_index = 4.0f;
	int loaded_chunks[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	std::vector<Chunk> chunks;

	std::vector<std::vector<Chunk*>> chunks_ordered;

	Tileset set;

	int map_height, map_width;
	int origin_x, origin_y;

	Decoration_Renderer dec_renderer;

private:
	void chunk_to_buffer(Chunk* c);
	Chunk* chunk_at_pixel(float x, float y);

public:
	New_Map();
	~New_Map();

	void draw(glm::mat4 matrix);
	void draw_extras(glm::mat4 matrix);

	Point collision_line_desination(Point origin, Point desination, float collision_radius);
	Point collision_line_delta(Point origin, Point delta, float collision_radius);

	bool init();
	void spawn_creatures();

	int tile_at(float x, float y);
	bool collision_at(float x, float y);
	bool collision_circle(float x, float y, float radius);

	bool collision_tile(float x, float y);

	void tick(float x = 0, float y = 0);
};