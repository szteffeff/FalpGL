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
	set()
{
	/* Load and parse map json */
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

	/* Check if mpa has embedded tileset */
	if (map_json["tilesets"][0].contains("source"))
	{
		/* If not, load from given file */
		std::string tileset_filepath = map_json["tilesets"][0]["source"];

		if (tileset_filepath.rfind("/") != std::string::npos)
		{
			tileset_filepath = tileset_filepath.substr(tileset_filepath.rfind("/") + 1);
		}

		tileset_filepath = std::string("res/data/") + tileset_filepath;

		set.init(tileset_filepath, 0);
	}
	else
	{
		/* If so, pass tileset json to create tileset */
		console_log("[INFO]: Using embedded tileset");
		nlohmann::json tileset_json = map_json["tilesets"][0];
		set.init(tileset_json, 0);
	}

	/* Get chunksize data from json. For now must be 64*64 */
	int chunksize[2], chunk_area;
	chunksize[0] = map_json["editorsettings"]["chunksize"]["width"];
	chunksize[1] = map_json["editorsettings"]["chunksize"]["height"];
	chunk_area = chunksize[0] * chunksize[1];

	/* Vector to hold list of tile IDs */
	std::vector<int> temp_chunk_data;
	temp_chunk_data.resize(chunk_area);

	/* Loop through all chunks in map */
	for (auto json_chunk : map_json["layers"][0]["chunks"])
	{
		static int index = 0;

		/* Loop through all 4096 tile IDs. I couldn't find a better way to do this. Whatever */
		for (int i = 0; i < chunk_area; i++)
		{
			temp_chunk_data[i] = json_chunk["data"][i];
			//temp_chunk_data[i] = index;
		}

		/* Construct new chunk in vector of chunks. Use emplace_back so constructed chunk is not copied, but this probably doesn't matter */
		chunks.emplace_back(set, temp_chunk_data, json_chunk["x"], (json_chunk["y"] * -1) - 64, chunksize[0], chunksize[1]);

		index++;
	}

	/* Initalize OpenGL buffers. This can't be done in initalizer list because the size of the map is not known */
	/* In the future fixed sized buffers should be loading with only visible chunks */
	/* Or each chunk has its own buffers and a seperate draw call is made for each (max ~4 draw calls for 64 * 64 chunk) */
	map_vertex_buffer.init(sizeof(n_Tile) * 64 * 64 * chunks.size());
	map_index_buffer.init(64 * 64 * chunks.size());

	/* Create and add layout*/
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	map_vertex_array.AddBuffer(map_vertex_buffer, layout);

	/* Set shader uniforms to default */
	map_shader.Bind();
	map_shader.SetUniform1iv("u_Textures", 16, samplers);
	map_shader.SetUniform1f("u_Texture", texture_index);
	map_shader.SetUniformMat4f("u_MVP", glm::mat4(1.0f));

	/* Write every chunk's vertex data to vertex buffer */
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
	/* Bind GL things*/
	map_vertex_array.Bind();
	map_index_buffer.Bind();
	map_shader.Bind();

	/* Set uniforms. 4 is temporary. Matrix is projection matrix to put verticies in pixel space */
	set.bind_texture(4.0f);
	map_shader.SetUniformMat4f("u_MVP", matrix);

	/* Draw without depth test to be sure that all fragments are visible */
	glDepthMask(false);
	GLCall(glDrawElements(GL_TRIANGLES, map_index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr));
	glDepthMask(true);
}


int New_Map::tile_at(float x, float y)
{
	return 0;
}

int New_Map::tile_at(glm::vec2 position)
{
	
	return 0;
}


Point New_Map::collision_line_desination(Point origin, Point desination, float collision_radius)
{
	/* not written yet */
	return Point(0, 0);
}

Point New_Map::collision_line_delta(Point origin, Point delta, float collision_radius)
{
	/* not written yet */
	return Point(0, 0);
}

void New_Map::chunk_to_buffer(Chunk* c)
{

}

/* Chunk */

Chunk::Chunk(Tileset& set, std::vector<int>& data, float position_x, float position_y, int size_x, int size_y)
	: tileset(set), chunk_data(data), loaded(false)
{
	/* Set variables */
	position[0] = position_x;
	position[1] = position_y;

	chunk_size[0] = size_x;
	chunk_size[1] = size_y;

	/* load chunk */
	load();
}

void Chunk::load()
{
	/* Resize tile vector to correct size */
	tiles.resize(chunk_size[0] * chunk_size[1]);

	/* Loop through all coordinants in chunk */
	for (int tile_x = 0; tile_x < chunk_size[0]; tile_x++)
	{
		for (int tile_y = 0; tile_y < chunk_size[1]; tile_y++)
		{
			/* Tiled exports with an inverted y axis and different rotation to what we want, correct for that */
			int tile_index = chunk_data[idx(63 - tile_y, tile_x)];

			/* Find tile's absolute position and create it */
			float tile_position[2] = { tile_x + position[0], tile_y + position[1] };
			tiles[idx(tile_x, tile_y)] = n_Tile(tileset[tile_index], tile_position);
		}
	}

	/* Chunk is now loaded */
	loaded = true;
}

void Chunk::unload()
{
	loaded = false;
	tiles.clear();
}

const void* Chunk::vertex_data() const
{
	/* Return pointer to array of tiles for writing to vertex buffer */
	return tiles.data();
}

n_Tile::n_Tile(Prototype_Tile& tile, float position[2])
{
	/* Math for texture coordinants is done completely for prototype tiles so it doesn't have to be done many times here */
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