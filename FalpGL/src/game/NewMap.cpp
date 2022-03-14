#include "NewMap.h"


inline int idx(float x, float y)
{
	return (x * 64 + y);
}

inline Point index_to_coord_64(int index)
{
	return { index % 64, index - (index % 64) };
}


New_Map::New_Map()
	: 
	map_shader("res/shader/newmap.shader"),
	map_vertex_array(),
	map_vertex_buffer(sizeof(Tile) * 64 * 64 * 9),
	map_index_buffer(64 * 64 * 9)
{
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	map_vertex_array.AddBuffer(map_vertex_buffer, layout);

	map_shader.SetUniform1iv("u_Textures", 16, samplers);
	map_shader.SetUniform1f("f_Texture", texture_index);
	map_shader.SetUniformMat4f("u_MVP", glm::mat4(1.0f));

	chunks.resize(64 * 64);
}

bool New_Map::init()
{
	try
	{
		std::ifstream file(chunk_json_filename);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		chunk_json = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing animation json!\n";
		return false;
	}

	try
	{
		std::ifstream file(tile_json_filename);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		tile_json = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing animation json!\n";
		return false;
	}

	int index = 0;
	for (auto chunk : chunks)
	{
		chunk = Chunk(index_to_coord_64(index).x, index_to_coord_64(index).y, &chunk_json, &tile_json);
	}
}

void New_Map::draw(glm::mat4 matrix)
{
	map_vertex_array.Bind();
	map_index_buffer.Bind();
	map_shader.Bind();

	glDepthMask(false);
	glDrawElements(GL_TRIANGLES, map_index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	glDepthMask(true);
}

Point New_Map::collision_line_desination(Point origin, Point desination, float collision_radius)
{
	return Point(0, 0);
}

Point New_Map::collision_line_delta(Point origin, Point delta, float collision_radius)
{
	return Point(0, 0);
}

void New_Map::chunk_to_buffer(Chunk* c)
{
	int free_index = -1;

	for (int i = 0; i < 9; i++)
	{
		if (loaded_chunks[i] == 0)
		{
			free_index = i;
			loaded_chunks[i] = c->get_id();
			break;
		}
	}

	map_vertex_buffer.buffer_data(free_index * 64 * 64 * sizeof(Tile), 64 * 64 * sizeof(Tile), c->vertex_data());
}





Chunk::Chunk(float x, float y, nlohmann::json* c_json, nlohmann::json* t_json)
	: loaded(false), position(), chunk_json(c_json), tile_json(t_json)
{
	tiles.resize(64 * 64);

	position[0] = x;
	position[1] = y;

	id = (int)(0.5 * (x + y) * (x + y + 1) + y);
}

Chunk::Chunk()
	: loaded(false), position(), chunk_json(), tile_json()
{
	tiles.resize(64 * 64);

	id = -1;
}

void Chunk::load()
{
	/* all json chunk names tbd*/

	for (int tile_x = 0; tile_x < chunk_size; tile_x++)
	{
		for (int tile_y = 0; tile_y < chunk_size; tile_y++)
		{
			tiles[idx(tile_x, tile_y)] = Tile(tile_x, tile_y, tile_json[chunk_json[std::to_string(idx(tile_x, tile_y))]]);
		}
	}

	loaded = true;
}

void Chunk::unload()
{
	loaded = false;
	tiles.clear();
}

bool Chunk::collision_at(float x, float y)
{
	return false;
}

const void* Chunk::vertex_data() const
{
	return tiles.data();
}

const int Chunk::get_id() const
{
	return id;
}


Tile::Tile(float position_x, float position_y, float texture_x, float texture_y, bool solid)
{
}

Tile::Tile(float position_x, float position_y, nlohmann::json tile_json)
{
	quad_data[0];  //X
	quad_data[0];  //Y

	quad_data[0];  //S
	quad_data[0];  //T


	quad_data[0];  //X
	quad_data[0];  //Y

	quad_data[0];  //S
	quad_data[0];  //T


	quad_data[0];  //X
	quad_data[0];  //Y

	quad_data[0];  //S
	quad_data[0];  //T


	quad_data[0];  //X
	quad_data[0];  //Y

	quad_data[0];  //S
	quad_data[0];  //T
}