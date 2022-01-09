#include "OverMap.h"
#include "Terminal.h"

#include <math.h>

void Tile::update_quad()
{
	quad_data[0] = x - 64;
	quad_data[1] = y - 64;
	quad_data[2] = 0.0f;

	quad_data[6] = x + 64;
	quad_data[7] = y - 64;
	quad_data[8] = 0.0f;

	quad_data[12] = x + 64;
	quad_data[13] = y + 64;
	quad_data[14] = 0.0f;

	quad_data[18] = x - 64;
	quad_data[19] = y + 64;
	quad_data[20] = 0.0f;


	quad_data[3] = 0.0f;
	quad_data[4] = 0.0f;

	quad_data[9] = 1.0f;
	quad_data[10] = 0.0f;

	quad_data[15] = 1.0f;
	quad_data[16] = 1.0f;

	quad_data[21] = 0.0f;
	quad_data[22] = 1.0f;


	quad_data[5] = texture_index;
	quad_data[11] = texture_index;
	quad_data[17] = texture_index;
	quad_data[23] = texture_index;

	active_buffer.modify_quad(quad_data, index);
}

Tile::Tile(float _x, float _y, VertexBuffer& vb)
	: index(0), active_buffer(vb)
{
	x = _x * tile_size;
	y = _y * tile_size;

	texture_index = 1.0f;

	t_top_right[0] = 0.0f;
	t_top_right[1] = 0.0f;

	t_bottom_left[0] = 32.0f;
	t_bottom_left[0] = 32.0f;

	for (int i = 0; i < 20; i++) quad_data[i] = 0.0f;

	to_buffer(active_buffer);
	update_quad();
}

void Tile::move(float delta_x, float delta_y)
{
	x += delta_x;
	y += delta_y;

	quad_data[0] = x - 64;
	quad_data[1] = y - 64;

	quad_data[6] = x + 64;
	quad_data[7] = y - 64;

	quad_data[12] = x + 64;
	quad_data[13] = y + 64;

	quad_data[18] = x - 64;
	quad_data[19] = y + 64;

	active_buffer.modify_quad(quad_data, index);

}

void Tile::move_texture(float top_right[2], float bottom_left[2])
{
}

void Tile::data()
{
	quad_data[2] = 0.25f;
	quad_data[8] = 0.25f;
	quad_data[14] = 0.25f;
	quad_data[20] = 0.25f;
	active_buffer.modify_quad(quad_data, index);
}

void Tile::to_buffer(VertexBuffer& buffer)
{
	index = buffer.add_quad(quad_data);
}

void Tile::tick(VertexBuffer& vb)
{
	if (index != -1)
	{
		active_buffer.modify_quad(quad_data, index);
	}
}

void Tile::index_change(float x)
{
	texture_index = x;
}

void Tile::active()
{
	index = active_buffer.add_quad(quad_data);
}

void Tile::inactive()
{
	active_buffer.delete_quad(index);
	index = -1;
}

Tile::~Tile()
{
	active_buffer.delete_quad(index);
}



void TileMap::fill(int x, int y)
{
	float width = (x / 128 / 2.0f) + 3;
	float height = (y / 128 / 2.0f) + 3;
	for (float x = -width; x < width; x++)
	{
		for (float y = -height; y < height; y++)
		{
			Tile* tile = new Tile(x, y, renderer.vertex_buffer);
			tiles.push_back(tile);
		}
	}
}

TileMap::TileMap()  // tiles are 128 * 128, 16 * 10 for 1080
	: renderer(300, "res/shaders/map.shader")
{
	renderer.layout.Push<float>(3);
	renderer.layout.Push<float>(2);
	renderer.layout.Push<float>(1);

	renderer.add_layout(renderer.layout);

	fill(1920, 1080);
}

TileMap::~TileMap()
{
	for (auto& tile : tiles)
	{
		 delete tile;
	}
}

void TileMap::tick(glm::mat4 proj_matrix)
{
	for (auto& tile : tiles)
	{
		tile->tick(renderer.vertex_buffer);
	}


	renderer.draw(proj_matrix, trans_matrix);
}

void TileMap::tick(glm::mat4 proj_matrix, float delta_x, float delta_y)
{
	for (auto& tile : tiles)
	{
		tile->move(delta_x, delta_y);
		tile->tick(renderer.vertex_buffer);
	}

	renderer.draw(proj_matrix, trans_matrix);
}

void TileMap::shift(float deltaX, float deltaY)
{
	if (deltaX + position[0] > 128.0f)
	{
		//trans_mat[0] -= 128.0f
	}
	else if (deltaX + position[0] < -128.0f)
	{
		//trans_mat[0] += 128.0f
	}

	if (deltaX + position[0] > 128.0f)
	{
		//trans_mat[0] -= 128.0f
	}
	else if (deltaX + position[0] < -128.0f)
	{
		//trans_mat[0] += 128.0f
	}

	position[0] += deltaX;
	position[1] += deltaY;
}

void TileMap::expand(float height, float width)
{
	/*int new_x = ceil(width / 128.0f);
	int new_y = ceil(height / 128.0f);

	for (auto& tile : tiles)
	{
		if ((abs(tile->x) > new_x / 2.0f) && (abs(tile->y) > new_y / 2.0f))
		{
			tile->inactive();
		}
	}*/
}
