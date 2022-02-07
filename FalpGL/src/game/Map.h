#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h" 
#include "Json.h"
#include <iostream>
#include <algorithm>


struct Tile : public Quad {
private:

public:
	Tile();
	Tile(VertexBuffer* vb, Json_loader* loader);
	Tile(VertexBuffer* vb, Json_loader* loader, std::string id);
};






class Map {
private:
	Json_loader* loader;
	std::vector<Tile*> map_vector;

	std::vector<std::vector<Tile*>> mvec;

	int height, width;
	int resolution[2];
	float offset[2] = { 0.0f, 0.0f };
	float current_center[2] = { 0.0f, 0.0f };
	glm::mat4 transformation_matrix;
	glm::mat4 *projection_matrix;
	BatchRenderer renderer;

public:
	~Map();
	Map(glm::mat4* pm, Json_loader *l, int res_x, int res_y);

	void shift(float dx, float dy);
	void fill();
	void draw();
	void draw(glm::mat4 tm);

	glm::mat4* get_trans_matrix();
};
