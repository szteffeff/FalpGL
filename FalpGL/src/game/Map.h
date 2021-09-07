#pragma once


#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h" 


class Map {
private:
	std::vector<quad*> map_vector;
	unsigned int height, width;
	BatchRenderer renderer;

public:
	Map();
	void fill();
	void draw(glm::mat4 proj);
};