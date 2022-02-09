#include "Map.h"


Map::~Map()
{
	int i = 0;
	for (int x = 0.0f; x < width; x++)
	{
		for (int y = 0.0f; y < height; y++)
		{
			delete map_vector[i];
			i++;
		}
	}
}

Map::Map(glm::mat4* pm, Json_loader* l, int res_x, int res_y)
	: resolution{ res_x, res_y }, 
	width((int)ceil(res_x / 32) % 2 == 1 ? ceil(res_x / 32) + 3 : ceil(res_x / 32) + 2), 
	height((int)ceil(res_y / 32) % 2 == 1 ? ceil(res_y / 32) + 3 : ceil(res_y / 32) + 2),
	renderer(BatchRenderer(width * height, "res/shaders/map.shader")), 
	projection_matrix(pm), loader(l)
{
	renderer.layout.Push<float>(3);
	renderer.layout.Push<float>(2);
	renderer.layout.Push<float>(1);
	renderer.add_layout(renderer.layout);


	std::cout << "Height/Width: " << height << ", " << width << "\n";

	map_vector.resize((double)height * (double)width);

	fill();
}

void Map::fill()
{
	int i = 0;

	for (int y = -height / 2; y < height / 2; y++)
	{
		for (int x = -width / 2; x < width / 2; x++)
		{

			map_vector[i] = new Tile(&renderer.vertex_buffer, loader, "2");
			map_vector[i]->change_type(Tile_id((rand() % 20) + 1), loader);
			map_vector[i]->translate(x * 32.0f, y * 32.0f);
			map_vector[i]->set_texture_index(0);
			i++;
		}
	}

	map_vector_static = map_vector;

	transformation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
}




void Map::shift(float px, float py)
{
	if ((px) < -16 + current_center[0] * 32) // left
	{
		shift_left();
	}
	else if ((px) > 16 + current_center[0] * 32) // right
	{
		shift_right();
	}

	if ((py) < -16 + current_center[1] * 32) // down
	{
		shift_down();
	}
	else if ((py) > 16 + current_center[1] * 32) // up
	{
		shift_up();
	}
}




void Map::shift_up()
{
	current_center[1] += 1;

	for (int i = 0; i < width; i++)
	{ /* Do tile things here */
		map_vector[i]->translate(0, height * 32);
		map_vector[i]->change_type(Tile_id((rand() % 20) + 1), loader);
	}

	std::rotate(map_vector.begin(), map_vector.begin() + width, map_vector.end());
}

void Map::shift_down()
{
	current_center[1] -= 1;

	std::rotate(map_vector.begin(), map_vector.begin() + (height - 1) * width, map_vector.end());

	for (int i = 0; i < width; i++)
	{ /* Do tile things here */
		map_vector[i]->translate(0, height * -32);
		map_vector[i]->change_type(Tile_id((rand() % 20) + 1), loader);
	}
}

void Map::shift_left()
{
	std::vector<Tile*> buffer;
	current_center[0] -= 1;

	for (int left = 0; left < height * width; left += width)
	{
		int right = left + width - 1;
		buffer.push_back(map_vector[right]);
	}

	std::shift_right(map_vector.begin(), map_vector.end(), 1);

	for (int left = 0; left < height * width; left += width)
	{
		map_vector[left] = buffer[0];
		buffer.erase(buffer.begin());

		/* Do tile things here */

		map_vector[left]->translate(width * -32, 0);
		map_vector[left]->change_type(Tile_id((rand() % 20) + 1), loader);
	}
}

void Map::shift_right()
{
	std::vector<Tile*> buffer;
	current_center[0] += 1;

	for (int left = 0; left < height * width; left += width)
	{
		buffer.push_back(map_vector[left]);
	}

	std::shift_left(map_vector.begin(), map_vector.end(), 1);

	for (int left = 0; left < height * width; left += width)
	{ 
		int right = left + width - 1;
		map_vector[right] = buffer[0];
		buffer.erase(buffer.begin());

		/* Do tile things here */

		map_vector[right]->translate(width * 32, 0);
		map_vector[right]->change_type(Tile_id((rand() % 20) + 1), loader);
	}
}




void Map::draw()
{
	renderer.draw(*projection_matrix * transformation_matrix);
}

void Map::draw(glm::mat4 tm)
{
	renderer.draw(*projection_matrix * transformation_matrix * tm);
}



glm::mat4* Map::get_trans_matrix()
{
	return &transformation_matrix;
}

