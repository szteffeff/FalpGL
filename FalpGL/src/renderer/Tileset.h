#pragma once
#include "Framebuffer.h"
#include "../vendor/nlohmann_json/json.hpp"
#include "Texture.h"
#include <fstream>


struct Tile {
	float position[2];
	float texture_coord[2];
	float id;

	std::string filename;

	Tile(float in_id, std::string image, float origin[2], float tex_origin[2]);
};

struct Tile_Quad {
	float quad_data[16];

	Tile_Quad(Tile &tile);
	Tile_Quad(float qd[16]);
	Tile_Quad();

	void operator=(Tile& tile_in);

	void whole_texture();
};

class Tileset {
private:
	Shader shader;
	VertexArray vertex_array;
	VertexBuffer vertex_buffer;
	IndexBuffer index_buffer;

	glm::mat4 projection_matrix;

	Texture active_tile_texture;

	unsigned int framebuffer_id, texture_id, renderbuffer_id;
	unsigned int size = 2048, loaded_tile, max_tiles;

	nlohmann::json tileset_json;
	std::string tileset_filepath;
	std::string tile_image_filepath;

	float vertices[24] = {
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
	};

	std::vector<Tile> tileset_tiles;

	void stitch_tile(Tile_Quad quad);

public:
	Tileset(std::string Tileset_path, int texture_unit);

	void bind_texture(unsigned int unit);

	Tile& operator[](int index);
};