#pragma once
#include "../renderer/RendererIncludes.h"

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

struct Tile {
private:
	float quad_data[16];

public:
	Tile(float position_x, float position_y, float texture_x, float texture_y);
};

class Chunk {
private:
	bool loaded;
	const int chunk_size = 64;
	float position[2];
	std::vector<std::vector<Tile>> tiles;
	std::vector<std::vector<bool>> collision_map;

	nlohmann::json chunk_jsonn;
	nlohmann::json *tile_json;

public:
	Chunk(float x, float y);

	void load();
	void unload();

	bool collision_at(float x, float y);
};

class new_map {
private:
	BatchRenderer map_renderer;

	Shader map_shader;
	VertexArray map_vertex_array;
	VertexBuffer map_vertex_buffer;


public:
	void draw(glm::mat4 matrix);
	Point collision_line_desination(Point origin, Point desination, float collision_radius);
	Point collision_line_delta(Point origin, Point delta, float collision_radius);

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