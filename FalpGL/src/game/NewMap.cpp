#include "NewMap.h"


inline int idx(float x, float y)
{
	return (x * 64 + y);
}

inline Point index_to_coord_64(int index)
{
	return { (float)(index % 64), (float)(index - (index % 64)) };
}


New_Map::New_Map()
	: 
	map_shader("res/shaders/newmap.shader"),
	map_vertex_array(),
	map_vertex_buffer(sizeof(n_Tile) * 64 * 64),
	map_index_buffer(64 * 64),
	set("res/data/tileset.json", 0)
{
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	map_vertex_array.AddBuffer(map_vertex_buffer, layout);

	map_shader.Bind();
	map_shader.SetUniform1iv("u_Textures", 16, samplers);
	map_shader.SetUniform1f("u_Texture", texture_index);
	map_shader.SetUniformMat4f("u_MVP", glm::mat4(1.0f));

	chunks.resize(1);
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
		std::cout << "error parsing map json!\n";
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
		std::cout << "error parsing tile json!\n";
		return false;
	}

	int index = 0;
	for (auto chunk : chunks)
	{
		chunk = Chunk(index_to_coord_64(index).x, index_to_coord_64(index).y, &chunk_json, &tile_json);
		chunk.load(set);

		map_vertex_buffer.buffer_data(0, sizeof(n_Tile) * 64 * 64, chunk.vertex_data());

		std::cout << "loaded chunk: " << index++ << "\n";
	}

	
}

void New_Map::draw(glm::mat4 matrix)
{

	map_vertex_array.Bind();
	map_index_buffer.Bind();
	map_shader.Bind();

	set.bind_texture(4.0f);

	map_shader.SetUniformMat4f("u_MVP", matrix);
	glDepthMask(false);
	GLCall(glDrawElements(GL_TRIANGLES, map_index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr));
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
	tiles.clear();

	id = -1;
}

void Chunk::load(Tileset& set)
{
	/* all json chunk names tbd*/

	for (int tile_x = 0; tile_x < chunk_size; tile_x++)
	{
		for (int tile_y = 0; tile_y < chunk_size; tile_y++)
		{
			int tile_index = (*chunk_json)["layers"][0]["chunks"][0]["data"][idx(63 - tile_y, tile_x)] - 1;

			float position[2] = { tile_x, tile_y };
			tiles[idx(tile_x, tile_y)] = n_Tile(set[tile_index], position);
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


n_Tile::n_Tile(Tile& tile, float position[2])
{
	quad_data[0] = position[0] * 32;  //X
	quad_data[1] = position[1] * 32;  //Y

	quad_data[2] = tile.texture_coord[0];  //S
	quad_data[3] = tile.texture_coord[1];  //T


	quad_data[4] = position[0] * 32 + 32;  //X
	quad_data[5] = position[1] * 32;  //Y

	quad_data[6] = tile.texture_coord[0] + 0.015625f;  //S
	quad_data[7] = tile.texture_coord[1];  //T


	quad_data[8] = position[0] * 32 + 32;  //X
	quad_data[9] = position[1] * 32 + 32;  //Y

	quad_data[10] = tile.texture_coord[0] + 0.015625f;  //S
	quad_data[11] = tile.texture_coord[1] + 0.015625f;  //T


	quad_data[12] = position[0] * 32;  //X
	quad_data[13] = position[1] * 32 + 32;  //Y

	quad_data[14] = tile.texture_coord[0];  //S
	quad_data[15] = tile.texture_coord[1] + 0.015625f;  //T
}

n_Tile::n_Tile(float position_x, float position_y, nlohmann::json tile_json)
{
	quad_data[0] = position_x * 32;  //X
	quad_data[1] = position_y * 32;  //Y

	quad_data[2] = (tile_json["texture_coordinants"][0][0]) / 2048.0f;  //S
	quad_data[3] = (tile_json["texture_coordinants"][0][1]) / 2048.0f;  //T


	quad_data[4] = position_x * 32 + 32;  //X
	quad_data[5] = position_y * 32;  //Y

	quad_data[6] = (tile_json["texture_coordinants"][1][0]) / 2048.0f;  //S
	quad_data[7] = (tile_json["texture_coordinants"][1][1]) / 2048.0f;  //T


	quad_data[8] = position_x * 32 + 32;  //X
	quad_data[9] = position_y * 32 + 32;  //Y

	quad_data[10] = (tile_json["texture_coordinants"][2][0]) / 2048.0f;  //S
	quad_data[11] = (tile_json["texture_coordinants"][2][1]) / 2048.0f;  //T


	quad_data[12] = position_x * 32;  //X
	quad_data[13] = position_y * 32 + 32;  //Y

	quad_data[14] = (tile_json["texture_coordinants"][3][0]) / 2048.0f;  //S
	quad_data[15] = (tile_json["texture_coordinants"][3][1]) / 2048.0f;  //T
}

n_Tile::n_Tile()
{

}