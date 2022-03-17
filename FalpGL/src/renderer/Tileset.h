#pragma once
#include "Framebuffer.h"
#include "../vendor/nlohmann_json/json.hpp"
#include <fstream>


struct Tile {
	float position[2];
	float texture_coord[2];
	float id;
};

struct Tile_Quad {
	float quad_data[16];

	Tile_Quad(float pos_x, float pos_y, float tex_x, float tex_y);
	Tile_Quad(float qd[16]);
	Tile_Quad();

	void operator=(Tile& tile_in);
};

class Tileset {
private:
	Shader shader;
	VertexArray vertex_array;
	VertexBuffer vertex_buffer;

	unsigned int framebuffer_id, texture_id, renderbuffer_id;
	unsigned int size = 2048, loaded_tile, max_tiles;

	nlohmann::json tileset_json;
	std::string tileset_filepath;

	float vertices[24] = {
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
	};

	std::vector<Tile> tileset_tiles;

	void stitch_tile(Tile data);

public:
	Tileset(std::string Tileset_path, int texture_unit);
};