#pragma once
#include "../renderer/Texture.h"
#include <vector>

#include "BatchRenderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Tile {
private:
	const float tile_size = 128;
	float quad_data[24];
	float texture_index = 1.0f;
	float t_top_right[2], t_bottom_left[2];
	unsigned int index;
	Texture texture;

	VertexBuffer& active_buffer;

public:
	float x, y;

public:
	void update_quad();

	Tile(float _x, float _y, VertexBuffer& vb);
	~Tile();

	void move(float new_x, float new_y);

	void move_texture(float top_right[2], float bottom_left[2]);

	void data();

	void to_buffer(VertexBuffer& buffer);

	void tick(VertexBuffer &vb);

	void index_change(float x);

	void active();
	void inactive();
};

class TileMap {
private:
	const int render_distance = 5;

	float position[2] = { 0, 0 };

	std::vector<Tile*> tiles;

	void fill(int x, int y);
	MapRenderer renderer;

	glm::mat4 transformation_matrix = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);

	glm::mat4 trans_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

public:
	TileMap();
	~TileMap();

	void tick(glm::mat4 proj_matrix);

	void tick(glm::mat4 proj_matrix, float delta_x, float delta_y);

	void shift(float deltaX, float deltaY);

	void expand(float height, float width);

	const float tile_size = 128;

	float total_position[2] = { 0.0f, 0.0f };
	
		 
};


/*
map needs to add and remove tiles to fit current resolution

tiles dont move, just transformation matrix:?
	pros:
		one multiplication
		might be faster

	cons:
		feels wrong
		




*/