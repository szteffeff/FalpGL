#include "NewMap.h"


inline int idx(float x, float y)
{
	return (x * 64 + y);
}

Point index_to_coord_64(int index)
{
	return { (float)(index % 64), (float)(index - (index % 64)) };
}


New_Map::New_Map()
	: 
	map_shader("res/shaders/newmap.shader"),
	map_vertex_array(),
	set(std::string("res/data/tileset.json"), 0)
{
	try
	{
		std::ifstream file(map_json_filename);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		map_json = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing map json!\n";
	}

	int chunksize[2], chunk_area;
	chunksize[0] = map_json["editorsettings"]["chunksize"]["width"];
	chunksize[1] = map_json["editorsettings"]["chunksize"]["height"];
	chunk_area = chunksize[0] * chunksize[1];

	std::vector<int> temp_chunk_data;
	temp_chunk_data.resize(chunk_area);

	for (auto json_chunk : map_json["layers"][0]["chunks"])
	{
		for (int i = 0; i < chunk_area; i++)
		{
			temp_chunk_data[i] = json_chunk["data"][i];
		}

		chunks.emplace_back(set, temp_chunk_data, json_chunk["x"], (json_chunk["y"] * -1) - 64, chunksize[0], chunksize[1]);
	}

	map_vertex_buffer.init(sizeof(n_Tile) * 64 * 64 * chunks.size());
	map_index_buffer.init(64 * 64 * chunks.size());


	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	map_vertex_array.AddBuffer(map_vertex_buffer, layout);

	map_shader.Bind();
	map_shader.SetUniform1iv("u_Textures", 16, samplers);
	map_shader.SetUniform1f("u_Texture", texture_index);
	map_shader.SetUniformMat4f("u_MVP", glm::mat4(1.0f));


	for (int i = 0; i < chunks.size(); i++)
	{
		map_vertex_buffer.buffer_data(sizeof(n_Tile) * 64 * 64 * i, sizeof(n_Tile) * 64 * 64, chunks[i].vertex_data());
	}

}

bool New_Map::init()
{
	return true;
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

/* Chunk */

Chunk::Chunk(Tileset& set, std::vector<int>& data, float position_x, float position_y, int size_x, int size_y)
	: tileset(set), chunk_data(data), loaded(false)
{
	position[0] = position_x;
	position[1] = position_y;

	chunk_size[0] = size_x;
	chunk_size[1] = size_y;

	load();
}

void Chunk::load()
{
	tiles.resize(chunk_size[0] * chunk_size[1]);

	for (int tile_x = 0; tile_x < chunk_size[0]; tile_x++)
	{
		for (int tile_y = 0; tile_y < chunk_size[1]; tile_y++)
		{
			/* Whack transformation for a whack coordinant system */
			int tile_index = chunk_data[idx(63 - tile_y, tile_x)];

			float tile_position[2] = { tile_x + position[0], tile_y + position[1] };
			tiles[idx(tile_x, tile_y)] = n_Tile(tileset[tile_index], tile_position);
		}
	}

	loaded = true;
}

void Chunk::unload()
{
	loaded = false;
	tiles.clear();
}

const void* Chunk::vertex_data() const
{
	return tiles.data();
}

n_Tile::n_Tile(Prototype_Tile& tile, float position[2])
{
	quad_data[0] = position[0] * 32;  //X
	quad_data[1] = position[1] * 32;  //Y

	quad_data[2] = tile.texture_coord[0];  //S
	quad_data[3] = tile.texture_coord[1];  //T


	quad_data[4] = position[0] * 32 + 32;  //X
	quad_data[5] = position[1] * 32;  //Y

	quad_data[6] = tile.texture_coord[2];  //S
	quad_data[7] = tile.texture_coord[3];  //T


	quad_data[8] = position[0] * 32 + 32;  //X
	quad_data[9] = position[1] * 32 + 32;  //Y

	quad_data[10] = tile.texture_coord[4];  //S
	quad_data[11] = tile.texture_coord[5];  //T


	quad_data[12] = position[0] * 32;  //X
	quad_data[13] = position[1] * 32 + 32;  //Y

	quad_data[14] = tile.texture_coord[6];  //S
	quad_data[15] = tile.texture_coord[7];  //T
}

n_Tile::n_Tile()
	: quad_data()
{}