#pragma once
#include "../renderer/RendererIncludes.h"
#include "../types/TileTypes.h"
#include "BatchRenderer.h" 
#include "Json.h"
#include "Tile.h"
#include <iostream>
#include <algorithm>


class Map {
private:
	Json_loader* loader;
	std::vector<Tile*> map_vector;
	std::vector<Tile*> map_vector_static;

	

	float PlayerLast_x, PlayerLast_y;
	int height, width;
	int resolution[2];
	float offset[2] = { 0.0f, 0.0f };
	float current_center[2] = { 0.0f, 0.0f };
	glm::mat4 transformation_matrix;
	glm::mat4 *projection_matrix;
	BatchRenderer renderer;
	Point tileposition(int index);

	void shift_up();
	void shift_down();
	void shift_left();
	void shift_right();

public:
	~Map();
	Map(glm::mat4* pm, Json_loader* l, int res_x, int res_y);

	void shift(float dx, float dy);
	void fill();
	void draw();
	void draw(glm::mat4 tm);

	glm::mat4* get_trans_matrix();
};
