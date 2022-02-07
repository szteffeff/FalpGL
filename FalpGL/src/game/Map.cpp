#include "Map.h"


Tile::Tile()
{}

Tile::Tile(VertexBuffer* vb, Json_loader * loader)
	: Quad(vb, 32, 32, 0.0f)
{
	tex_coord tx;
	
	tx.coords[0][0] = (loader->tiles["0"]["texture_coordinants"][0][0]) / 2048.0f; /* Denominator must be float or intiger division happens */
	tx.coords[0][1] = (loader->tiles["0"]["texture_coordinants"][0][1]) / 2048.0f;

	tx.coords[1][0] = (loader->tiles["0"]["texture_coordinants"][1][0]) / 2048.0f;
	tx.coords[1][1] = (loader->tiles["0"]["texture_coordinants"][1][1]) / 2048.0f;

	tx.coords[2][0] = (loader->tiles["0"]["texture_coordinants"][2][0]) / 2048.0f;
	tx.coords[2][1] = (loader->tiles["0"]["texture_coordinants"][2][1]) / 2048.0f;

	tx.coords[3][0] = (loader->tiles["0"]["texture_coordinants"][3][0]) / 2048.0f;
	tx.coords[3][1] = (loader->tiles["0"]["texture_coordinants"][3][1]) / 2048.0f;

	set_texture_coords(tx);
	set_z(-0.999);
}

Tile::Tile(VertexBuffer* vb, Json_loader* loader, std::string id)
	: Quad(vb, 32, 32, 0.0f)
{
	tex_coord tx;

	tx.coords[0][0] = (loader->tiles[id]["texture_coordinants"][0][0]) / 2048.0f; /* Denominator must be float or intiger division happens */
	tx.coords[0][1] = (loader->tiles[id]["texture_coordinants"][0][1]) / 2048.0f;

	tx.coords[1][0] = (loader->tiles[id]["texture_coordinants"][1][0]) / 2048.0f;
	tx.coords[1][1] = (loader->tiles[id]["texture_coordinants"][1][1]) / 2048.0f;

	tx.coords[2][0] = (loader->tiles[id]["texture_coordinants"][2][0]) / 2048.0f;
	tx.coords[2][1] = (loader->tiles[id]["texture_coordinants"][2][1]) / 2048.0f;

	tx.coords[3][0] = (loader->tiles[id]["texture_coordinants"][3][0]) / 2048.0f;
	tx.coords[3][1] = (loader->tiles[id]["texture_coordinants"][3][1]) / 2048.0f;

	set_texture_coords(tx);
	set_z(-0.9);
}


void Tile::change_type(tile_id id, Json_loader* loader)
{
	tex_coord tx;
	std::string _id = std::to_string(id);

	tx.coords[0][0] = (loader->tiles[_id]["texture_coordinants"][0][0]) / 2048.0f; /* Denominator must be float or intiger division happens */
	tx.coords[0][1] = (loader->tiles[_id]["texture_coordinants"][0][1]) / 2048.0f;

	tx.coords[1][0] = (loader->tiles[_id]["texture_coordinants"][1][0]) / 2048.0f;
	tx.coords[1][1] = (loader->tiles[_id]["texture_coordinants"][1][1]) / 2048.0f;

	tx.coords[2][0] = (loader->tiles[_id]["texture_coordinants"][2][0]) / 2048.0f;
	tx.coords[2][1] = (loader->tiles[_id]["texture_coordinants"][2][1]) / 2048.0f;

	tx.coords[3][0] = (loader->tiles[_id]["texture_coordinants"][3][0]) / 2048.0f;
	tx.coords[3][1] = (loader->tiles[_id]["texture_coordinants"][3][1]) / 2048.0f;

	set_texture_coords(tx);
}


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
	: resolution{ res_x, res_y }, width(ceil(res_x / 32) + 2), height(ceil(res_y / 32) + 2), renderer(BatchRenderer(width* height, "res/shaders/map.shader")), projection_matrix(pm), loader(l)
{
	renderer.layout.Push<float>(3);
	renderer.layout.Push<float>(2);
	renderer.layout.Push<float>(1);

	renderer.add_layout(renderer.layout);

	std::cout << "Height/Width: " << height << ", " << width << "\n";

	map_vector.resize((double)height * (double)width);

	mvec.resize(width);
	for (int i = 0; i < width; i++)
		mvec[i].resize(height);

	fill();
}

void Map::shift(float px, float py)
{
	bool print = false;

	if ((px) < -16 + current_center[0] * 32)
	{
		print = true;
		current_center[0] -= 1;
		for (int i = 0; i < height * width; i++) {
			map_vector[i]->translate(-32, 0);
		}
	}
	else if ((px) > 16 + current_center[0] * 32)
	{
		print = true;
		current_center[0] += 1;
		for (int i = 0; i < height * width; i++) {
			map_vector[i]->translate(32, 0);
		}
	}

	if ((py) < -16 + current_center[1] * 32)
	{
		print = true;
		current_center[1] -= 1;
		for (int i = 0; i < height * width; i++) {
			map_vector[i]->translate(0, -32);
		}
	}
	else if ((py) > 16 + current_center[1] * 32)
	{
		print = true;
		std::cout << "ayo u dere\n";
		current_center[1] += 1;
		for (int i = 0; i < height * width; i++) {
			map_vector[i]->translate(0, 32);
			if (i < width) {

			}
		};
	}

	if (print) { std::cout << "map center is: " << current_center[0] << ", " << current_center[1] << "\n"; }
}

void Map::fill()
{
	int i = 0;
	for (int x = 0.0f; x < width; x++)
	{
		for (int y = 0.0f; y < height; y++)
		{

			map_vector[i] = new Tile(&renderer.vertex_buffer, loader, "2");
			map_vector[i]->translate(x * 32.0f, y * 32.0f);
			map_vector[i]->texture_index(0);

			mvec[x][y] = map_vector[i];

			i++;
		}
	}

	transformation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-width / 2 * 32, -height / 2 *  32, 0.0f));
}

void Map::draw()
{
	renderer.draw(*projection_matrix, transformation_matrix);
}

void Map::draw(glm::mat4 tm)
{
	renderer.draw(*projection_matrix, transformation_matrix * tm);
}

glm::mat4* Map::get_trans_matrix()
{
	return &transformation_matrix;
}

