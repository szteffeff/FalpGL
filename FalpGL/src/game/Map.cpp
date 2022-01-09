#include "Map.h"





Map::Map()
	: renderer(BatchRenderer(10000, "res/shaders/basic.shader"))
{
	renderer.layout.Push<float>(3);
	renderer.layout.Push<float>(2);
	renderer.layout.Push<float>(1);

	renderer.add_layout(renderer.layout);

	height = 100;
	width = height;

	map_vector.resize(10000);

	fill();
}

void Map::fill()
{

	float x = 0, y = 0;
	for (int i = 0; i < 10000; i++)
	{
		map_vector[i] = new Quad(&renderer.vertex_buffer, 32, 32, 32);
		map_vector[i]->translate((x - 50) * 32, (y - 50) * 32);
		map_vector[i]->texture_index(1);

		x++;
		if (x > 100)
		{
			x = 0;
			y++;
		}
	}
}

void Map::draw(glm::mat4 proj)
{
	renderer.draw(proj);
}
