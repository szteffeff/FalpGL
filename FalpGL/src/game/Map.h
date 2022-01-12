#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h" 
#include "Json.h"


struct Tile : public Quad {
private:

public:
	Tile();
	Tile(VertexBuffer* vb, Json_loader* loader);
	Tile(VertexBuffer* vb, Json_loader* loader, std::string id);
};

struct Tilemap {
public:
	std::vector<Tile*> x;
	std::vector<Tile*> y;
};


class Map {
private:
	Json_loader* loader;
	std::vector<Tile*> map_vector;
	int height, width;
	glm::mat4 transformation_matrix;
	glm::mat4 *projection_matrix;
	BatchRenderer renderer;

public:
	Map(glm::mat4* pm, Json_loader *l);
	void fill();
	void draw();
	void draw(glm::mat4 tm);

	glm::mat4* get_trans_matrix();
};
