#include "Map.h"


Tile::Tile()
{}

Tile::Tile(VertexBuffer* vb, Json_loader * loader)
	: Quad(vb, 32, 32, 0.0f)
{
	tex_coord tx;

	tx.coords[0][0] = loader->tiles["0"]["texture_coordinants"][0][0];
	tx.coords[0][1] = loader->tiles["0"]["texture_coordinants"][0][1];

	tx.coords[1][0] = loader->tiles["0"]["texture_coordinants"][1][0];
	tx.coords[1][1] = loader->tiles["0"]["texture_coordinants"][1][1];

	tx.coords[2][0] = loader->tiles["0"]["texture_coordinants"][2][0];
	tx.coords[2][1] = loader->tiles["0"]["texture_coordinants"][2][1];

	tx.coords[3][0] = loader->tiles["0"]["texture_coordinants"][3][0];
	tx.coords[3][1] = loader->tiles["0"]["texture_coordinants"][3][1];

	set_texture_coords(tx);
	set_height(-0.999);
}

Tile::Tile(VertexBuffer* vb, Json_loader* loader, std::string id)
	: Quad(vb, 32, 32, 0.0f)
{
	tex_coord tx;

	tx.coords[0][0] = loader->tiles[id]["texture_coordinants"][0][0];
	tx.coords[0][1] = loader->tiles[id]["texture_coordinants"][0][1];

	tx.coords[1][0] = loader->tiles[id]["texture_coordinants"][1][0];
	tx.coords[1][1] = loader->tiles[id]["texture_coordinants"][1][1];

	tx.coords[2][0] = loader->tiles[id]["texture_coordinants"][2][0];
	tx.coords[2][1] = loader->tiles[id]["texture_coordinants"][2][1];

	tx.coords[3][0] = loader->tiles[id]["texture_coordinants"][3][0];
	tx.coords[3][1] = loader->tiles[id]["texture_coordinants"][3][1];

	set_texture_coords(tx);
	set_height(-0.9);
}


Map::Map(glm::mat4* pm, Json_loader* l)
	: renderer(BatchRenderer(100000, "res/shaders/map.shader")), projection_matrix(pm), loader(l)
{
	renderer.layout.Push<float>(3);
	renderer.layout.Push<float>(2);
	renderer.layout.Push<float>(1);

	renderer.add_layout(renderer.layout);

	height = 100;
	width = height;

	map_vector.resize((double)height * (double)width);

	fill();
}

void Map::fill()
{
	int i = 0;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			map_vector[i] = new Tile(&renderer.vertex_buffer, loader);
			map_vector[i]->translate((x) * 32, (y) * 32);
			map_vector[i]->texture_index(0);

			i++;
		}
	}

	transformation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-width * 32 / 2, -height * 32 / 2, 0.0f));
}

void Map::draw()
{
	renderer.draw(*projection_matrix, transformation_matrix);
}

glm::mat4* Map::get_trans_matrix()
{
	return &transformation_matrix;
}

