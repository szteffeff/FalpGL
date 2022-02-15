#include "Map.h"


Map::~Map()
{
	int i = 0;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			delete map_vector[i];
			i++;
		}
	}
}

Map::Map(glm::mat4* pm, Json_loader* l, int res_x, int res_y)
	: resolution{ res_x, res_y }, 
	width(((int)ceil(res_x / 32) % 2 == 1) ? (int)ceil(res_x / 32) + 3 : (int)ceil(res_x / 32) + 2),
	height(((int)ceil(res_y / 32) % 2 == 1) ? (int)ceil(res_y / 32) + 3 : (int)ceil(res_y / 32) + 2),
	renderer(BatchRenderer(width * height, "res/shaders/map.shader")), 
	projection_matrix(pm), loader(l)
{
	renderer.layout.Push<float>(3);
	renderer.layout.Push<float>(2);
	renderer.layout.Push<float>(1);
	renderer.add_layout(renderer.layout);


	std::cout << "Height/Width: " << height << ", " << width << "\n";

	map_vector.resize((unsigned int)height * (unsigned int)width);

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
			if (i != width * height / 2 + width / 2) {
				map_vector[i]->change_type(Tile_id::GRASS, loader);
			}
			else
			{
				map_vector[i]->change_type(Tile_id::COBBLESTONE_8, loader);
			}
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
	offset[0] = px - current_center[0] * 32;
	offset[1] = py - current_center[1] * 32;

	if ((px) < -16 + current_center[0] * 32) // left
	{
		std::cout << "player is on: " << map_vector[width * height / 2 + width / 2 - 1]->name << "\n";
		shift_left();
	}
	else if ((px) > 16 + current_center[0] * 32) // right
	{
		std::cout << "player is on: " << map_vector[width * height / 2 + width / 2 + 1]->name << "\n";
		shift_right();
	}

	if ((py) < -16 + current_center[1] * 32) // down
	{
		std::cout << "player is on: " << map_vector[width * height / 2 + width / 2 - width]->name << "\n";
		shift_down();
	}
	else if ((py) > 16 + current_center[1] * 32) // up
	{
		std::cout << "player is on: " << map_vector[width * height / 2 + width / 2 + width]->name << "\n";
		shift_up();
	}

	//std::cout << "offset: " << offset[0] << ", " << offset[1] << "\n";
}




void Map::shift_up()
{
	current_center[1] += 1;

	for (int i = 0; i < width; i++)
	{ /* Do tile things here */
		map_vector[i]->translate(0.0f, (float)height * 32.0f);
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
		map_vector[i]->translate(0.0f, (float)height * -32.0f);
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

		map_vector[left]->translate((float)(width * -32), 0.0f);
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

		map_vector[right]->translate((float)(width * 32), 0.0f);
		map_vector[right]->change_type(Tile_id((rand() % 20) + 1), loader);
	}
}


void Map::tile_at_position(float x, float y)
{
	x = (float)round((x + width * 32.0 * 0.5 + offset[0]) / 32);
	y = (float)round((y + height * 32.0 * 0.5 + offset[1]) / 32);

	int index = (int)(x + y * width);

	if (index < 0 || index > height * width) { return; }

	//std::cout << "arr: " << x << ", " << y << "\n";
	std::cout << "tile is: " << map_vector[index]->name << "\n";
}


void Map::draw()
{
	renderer.draw(*projection_matrix * transformation_matrix);
}

void Map::draw(glm::mat4 tm)
{
	renderer.draw(*projection_matrix * transformation_matrix * tm);
}

std::string Map::map_attack()
{
	std::string tile_attacking = map_vector[width * height / 2 + width / 2]->name;
	return tile_attacking;
}



glm::mat4* Map::get_trans_matrix()
{
	return &transformation_matrix;
}

