#include "Tileset.h"
#include "renderer.h"


Collision_Box::Collision_Box(float offset_x, float offset_y, float size_x, float size_y, float tile_size_y)
	: valid(true)
{
	/* Change y origin from top to bottom */
	offset[0] = offset_x;
	offset[1] = tile_size_y - (offset_y + size_y);

	size[0] = size_x;
	size[1] = size_y;

	/* Test if box is a quad not a line */
	if (size[0] == 0.0f || size[1] == 0.0f)
	{
		/* If it is a line, disable it */
		valid = false;
	}
}

bool Collision_Box::collides_at(float x, float y)
{
	if ((x >= offset[0] && x <= offset[0] + size[0]) && (y >= offset[1] && y <= offset[1] + size[1]))
	{	/* Point is inside box*/
		
		return true;
	}

	/* Point is outside box */
	return false;
}

bool Collision_Box::collision_circle(float x, float y, float radius)
{
	if (!valid) { return false; }

	/* Find closest point of rectangle to circle */
	float closest_x = std::clamp(x, offset[0], offset[0] + size[0]);
	float closest_y = std::clamp(y, offset[1], offset[1] + size[1]);

	if (abs(closest_x - x) < radius || abs(closest_y - y) < radius)
	{
		return true;
	}

	return false;
}

bool Prototype_Tile::has_collision()
{
	return (!collisions.empty());
}

 /* #### Tile ####*/

Prototype_Tile::Prototype_Tile()
{
}

Prototype_Tile::Prototype_Tile(float in_id, std::string image, float tex_origin[2], float atlas_size, std::vector<Collision_Box> boxes, bool nf, int size_x, int size_y)
	: collisions(boxes), nofade(nf)
{
	id = in_id;

	texture_coord[0] = tex_origin[0];
	texture_coord[1] = tex_origin[1];

	texture_coord[2] = tex_origin[0] + ((float)size_x / atlas_size);
	texture_coord[3] = tex_origin[1];

	texture_coord[4] = tex_origin[0] + ((float)size_x / atlas_size);
	texture_coord[5] = tex_origin[1] + ((float)size_y / atlas_size);

	texture_coord[6] = tex_origin[0];
	texture_coord[7] = tex_origin[1] + ((float)size_y / atlas_size);

	if (image.rfind("/") != std::string::npos) 
	{
		image = image.substr(image.rfind("/") + 1);
	}

	tile_name = image;

	filepath = std::string("files/tiles/") + image;
}

Prototype_Tile::Prototype_Tile(nlohmann::json tile_json, float tex_origin[2], float atlas_size)
	: nofade(false)
{
	id = tile_json["id"];

	int size_x = tile_json["imagewidth"];
	int size_y = tile_json["imageheight"];

	texture_coord[0] = tex_origin[0];
	texture_coord[1] = tex_origin[1];

	texture_coord[2] = tex_origin[0] + ((float)size_x / atlas_size);
	texture_coord[3] = tex_origin[1];

	texture_coord[4] = tex_origin[0] + ((float)size_x / atlas_size);
	texture_coord[5] = tex_origin[1] + ((float)size_y / atlas_size);

	texture_coord[6] = tex_origin[0];
	texture_coord[7] = tex_origin[1] + ((float)size_y / atlas_size);

	if (tile_json.contains("properties"))
	{
		for (auto prop : tile_json["properties"])
		{
			if (prop["name"] == "nofade")
			{
				nofade = prop["value"];
			}
		}
	}

	if (tile_json.contains("objectgroup"))
	{
		for (auto box : tile_json["objectgroup"]["objects"])
		{
			collisions.push_back(Collision_Box(box["x"], box["y"], box["width"], box["height"], tile_json["imageheight"]));
		}

	}

	std::string image = tile_json["image"];
	if (image.rfind("/") != std::string::npos)
	{
		image = image.substr(image.rfind("/") + 1);
	}

	filepath = std::string("files/tiles/") + image;

	tile_name = image;
}

bool Prototype_Tile::collides(float x, float y)
{
	for (auto box : collisions)
	{ /* Loop through all collision boxes*/
		if (box.collides_at(x, y))
		{ /* Return if any box hits*/
			return true;
		}
	}

	/* No collisions occured */
	return false;
}

bool Prototype_Tile::collision_circle(float x, float y, float radius)
{
	for (auto box : collisions)
	{ /* Loop through all collision boxes*/
		if (box.collision_circle(x, y, radius))
		{ /* Return if any box hits*/
			return true;
		}
	}

	/* No collisions occured */
	return false;
}

Prototype_Tile Prototype_Tile::transformed(unsigned int transforms)
{
	Prototype_Tile transformed_tile(*this);
	float original_coords[8];
	memcpy(&original_coords, &transformed_tile.texture_coord, sizeof(original_coords));

	/*
	* 
	* 0
	* 1
	* 2
	* 3
	* 
	*  Vertical: flip on horizontal axis
	*  Horizontal: flip on vertical axis
	*  Anti-Diagonal: swap point 1 and 3
	* 
	* 3-2
	* | |
	* 0-1
	* 
	*/

	if (transforms & transformations::flip_verticle)
	{
		transformed_tile.texture_coord[0] = original_coords[6];
		transformed_tile.texture_coord[1] = original_coords[7];

		transformed_tile.texture_coord[2] = original_coords[4];
		transformed_tile.texture_coord[3] = original_coords[5];

		transformed_tile.texture_coord[4] = original_coords[2];
		transformed_tile.texture_coord[5] = original_coords[3];

		transformed_tile.texture_coord[6] = original_coords[0];
		transformed_tile.texture_coord[7] = original_coords[1];
		memcpy(&original_coords, &transformed_tile.texture_coord, sizeof(original_coords));
	}

	if (transforms & transformations::flip_horizontal)
	{
		transformed_tile.texture_coord[0] = original_coords[2];
		transformed_tile.texture_coord[1] = original_coords[3];

		transformed_tile.texture_coord[2] = original_coords[0];
		transformed_tile.texture_coord[3] = original_coords[1];

		transformed_tile.texture_coord[4] = original_coords[6];
		transformed_tile.texture_coord[5] = original_coords[7];

		transformed_tile.texture_coord[6] = original_coords[4];
		transformed_tile.texture_coord[7] = original_coords[5];
		memcpy(&original_coords, &transformed_tile.texture_coord, sizeof(original_coords));
	}

	if (transforms & transformations::flip_anti_diagonal)
	{
		transformed_tile.texture_coord[0] = original_coords[0];
		transformed_tile.texture_coord[1] = original_coords[1];

		transformed_tile.texture_coord[2] = original_coords[6];
		transformed_tile.texture_coord[3] = original_coords[7];

		transformed_tile.texture_coord[4] = original_coords[4];
		transformed_tile.texture_coord[5] = original_coords[5];

		transformed_tile.texture_coord[6] = original_coords[2];
		transformed_tile.texture_coord[7] = original_coords[3];
	}

	return transformed_tile;
}


/* #### Tileset #### */

Prototype_Tile Tileset::operator[](int index)
{
	uint32_t id = index;
	uint32_t mask = 0b00001111111111111111111111111111;
	uint32_t horizontal = 0b10000000000000000000000000000000;
	uint32_t vertical = 0b01000000000000000000000000000000;
	uint32_t antidiagonal = 0b00100000000000000000000000000000;

	id &= mask;

	uint32_t transforms = 0;

	if (index & horizontal)
	{
		transforms |= Prototype_Tile::transformations::flip_horizontal;
	}

	if (index & vertical)
	{
		transforms |= Prototype_Tile::transformations::flip_verticle;
	}

	if (index & antidiagonal)
	{
		transforms |= Prototype_Tile::transformations::flip_anti_diagonal;
	}

	if (id_map.find(id - first_gid) == id_map.end())
	{
		std::cout << "help!\n";
	}

	return tileset_tiles[id_map.at(id - first_gid)].transformed(transforms);
}


Tileset::Tileset()
	: shader("res/shaders/tile.shader"),
	vertex_array(),
	vertex_buffer(sizeof(float) * 16),
	size(0),
	index_buffer(1),
	tileset_filepath(""),
	loaded(false),
	active_texture_unit(0)
{}


Tileset::Tileset(nlohmann::json set_json, int texture_unit)
	: shader("res/shaders/tile.shader"),
	vertex_array(),
	vertex_buffer(sizeof(float) * 16),
	size(0),
	index_buffer(1),
	tileset_filepath(""),
	loaded(true),
	active_texture_unit(texture_unit),
	tileset_json(set_json)
{
	create_atlas();
}

Tileset::Tileset(std::string Tileset_path, int texture_unit)
	: shader("res/shaders/tile.shader"),
	vertex_array(),
	vertex_buffer(sizeof(float) * 16),
	size(0),
	index_buffer(1),
	tileset_filepath(Tileset_path),
	loaded(true),
	active_texture_unit(texture_unit)
{

	/* Load and parse tilset json */
	try
	{
		std::ifstream file(tileset_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		tileset_json = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing map json!\n";
	}


	create_atlas();
}


void Tileset::init(nlohmann::json set_json, int texture_unit)
{
	if (loaded) return;

	tileset_json = set_json;
	active_texture_unit = texture_unit;
	create_atlas();
}

void Tileset::init(std::string Tileset_path, int texture_unit)
{
	if (loaded) return;

	/* Load and parse tilset json */
	try
	{
		std::ifstream file(tileset_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		tileset_json = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing map json!\n";
	}

	active_texture_unit = texture_unit;
	create_atlas();
}

void Tileset::create_atlas()
{
	/* Calculate minimun texture size to fit all tiles in set - maximum 4096 pixels */
	int tilecount = tileset_json["tilecount"];
	tilesize_x = tileset_json["tileheight"];
	tilesize_y = tileset_json["tilewidth"];

	for (int test_size_level = 5; test_size_level < 12; test_size_level++)
	{
		if (pow(pow(2, test_size_level), 2) / (tilesize_x * tilesize_y) > tilecount)
		{
			size = pow(2, test_size_level);
			break;
		}
	}


	/* Keep track of tileset's name */
	name = tileset_json["name"];

	/* Create framebuffer - same as framebuffer.cpp */
	GLCall(glGenFramebuffers(1, &gl_framebuffer_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_id));
	GLCall(glGenTextures(1, &gl_texture_id));
	GLCall(glActiveTexture(GL_TEXTURE0 + active_texture_unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, gl_texture_id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl_texture_id, 0));
	GLCall(glGenRenderbuffers(1, &gl_renderbuffer_id));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, gl_renderbuffer_id));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size, size));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gl_renderbuffer_id));
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error: " << status << " | " << __FILE__ << ":" << __LINE__ << "\n";
	}

	/* Bind and clear framebuffer */
	glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_id);
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	/* Setup openGL objects */
	VertexBufferLayout vbl;
	vbl.Push<float>(2);
	vbl.Push<float>(2);

	vertex_array.AddBuffer(vertex_buffer, vbl);
	shader.Bind();
	shader.SetUniform1i("u_Texture", 0);

	/* Keep track of where a tile is being drawn to */
	float tex_coords[2] = { 0, 0 };

	if (tileset_json.contains("firstgid"))
	{
		first_gid = tileset_json["firstgid"];
	}
	else
	{
		first_gid = 0;
	}

	int index = 0;

	/* Create dummy tile to for id 0 */
	{
		/* Generate new tile protype and add it to the vector */
		Prototype_Tile new_tile(0, "blank.png", tex_coords, size, std::vector<Collision_Box>(1, Collision_Box(0, 0, 0, 0)));
		tileset_tiles.push_back(new_tile);
		id_map[0] = index++;

		/* Advance to the next open space on the atlas */
		tex_coords[0] += ((float)tilesize_x / size);

		/* Load texture image */
		Texture working_texture(new_tile.filepath);
		working_texture.Bind(0);
	}


	/* Loop through all tiles in tileset file */
	for (auto tile = tileset_json["tiles"].begin(); tile != tileset_json["tiles"].end(); tile++)
	{
		/* Generate new tile protype */
		Prototype_Tile new_tile(*tile, tex_coords, size);
		
		tileset_tiles.push_back(new_tile);

		id_map[new_tile.id] = index++;

		/* Advance to the next open space on the atlas */
		tex_coords[0] += ((float)tilesize_x / (float)size);
		if (tex_coords[0] >= 1.0f)
		{
			/* If the end of a row is hit, go back to the start of the next row up */
			tex_coords[0] = 0;
			tex_coords[1] += ((float)tilesize_y / (float)size);
		}

		/* Load texture image */
		Texture working_texture(new_tile.filepath);
		working_texture.Bind(0);

		/* Draw tile to atlas */
		stitch_tile(new_tile);
	}

	/* Bind atlas */
	bind_texture(active_texture_unit);

	loaded = true;
}


void Tileset::stitch_tile(Prototype_Tile tile_to_stich)
{
	/* Turn off depth testing */
	glDisable(GL_DEPTH_TEST);

	/* Bind framebuffer texture */
	GLCall(glActiveTexture(GL_TEXTURE0 + 4));
	GLCall(glBindTexture(GL_TEXTURE_2D, gl_texture_id));

	/* Bind framebuffer */
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_id));
	glViewport(0, 0, size, size);

	/* Vertex data to draw tile */
	float vertices[16] = {
	 -1.0f, -1.0f, 0.0f, 0.0f,
	  1.0f, -1.0f, 1.0f, 0.0f,
	  1.0f,  1.0f, 1.0f, 1.0f,
	 -1.0f,  1.0f, 0.0f, 1.0f
	};

	/* Set vertex positions to texture coordinants on atlas */
	vertices[0] = (tile_to_stich.texture_coord[0] * 2.0f) - 1.0f;
	vertices[1] = (tile_to_stich.texture_coord[1] * 2.0f) - 1.0f;

	vertices[4] = (tile_to_stich.texture_coord[2] * 2.0f) - 1.0f;
	vertices[5] = (tile_to_stich.texture_coord[3] * 2.0f) - 1.0f;

	vertices[8] = (tile_to_stich.texture_coord[4] * 2.0f) - 1.0f;
	vertices[9] = (tile_to_stich.texture_coord[5] * 2.0f) - 1.0f;

	vertices[12] = (tile_to_stich.texture_coord[6] * 2.0f) - 1.0f;
	vertices[13] = (tile_to_stich.texture_coord[7] * 2.0f) - 1.0f;


	/* Write vertex data to buffer */
	vertex_buffer.buffer_data(0, sizeof(vertices), &vertices);

	/* Bind openGL objects */
	shader.Bind();
	shader.SetUniform1i("u_Texture", 0);
	vertex_array.Bind();
	index_buffer.Bind();

	/* Draw */
	glDrawElements(GL_TRIANGLES, index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr);

	/* Unbind framebuffer and turn on depth testing */
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glEnable(GL_DEPTH_TEST);
}

void Tileset::bind_texture(unsigned int unit)
{
	/* Bind atlas to selected texture unit */
	GLCall(glActiveTexture(GL_TEXTURE0 + unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, gl_texture_id));
}
