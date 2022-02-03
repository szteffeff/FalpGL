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
	set_height(-0.999);
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
	set_height(-0.9);
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

Map::Map(glm::mat4* pm, Json_loader* l)
	: height(24), width(32), renderer(BatchRenderer(width * height, "res/shaders/map.shader")), projection_matrix(pm), loader(l)
{
	renderer.layout.Push<float>(3);
	renderer.layout.Push<float>(2);
	renderer.layout.Push<float>(1);

	renderer.add_layout(renderer.layout);


	current_center[0] = width / 2.0f;
	current_center[1] = height / 2.0f;

	map_vector.resize((double)height * (double)width);

	mvec.resize(width);
	for (int i = 0; i < width; i++)
		mvec[i].resize(height);

	fill();
}

void Map::shift(float px, float py)
{
	float new_center[2] = { (px / 2.0f), (px / 2.0f) };

	if ((int)round(new_center[0]) % 32 > current_center[0])
	{
		//std::left_shift(mvec[0], 1)
		for (int i = 0; i < height; i++)
		{
			//mvec[width - 1][i] = load_new_tile_at((int)round(new_center[0]) % 32 + ((int)width % 32) / 2)
		}
	}
	else if ((int)round(new_center[0]) % 32 < current_center[0])
	{
		//std::left_shift(mvec[0], 1)
		std::shift_right(mvec.begin(), mvec.end(), 1);
		for (int i = 0; i < height; i++)
		{
			//mvec[width - 1][i] = load_new_tile_at((int)round(new_center[0]) % 32 + ((int)width % 32) / 2)
		}
	}

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

	transformation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-width * 32, -height * 32, 0.0f));
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

