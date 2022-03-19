#include "Tileset.h"
#include "renderer.h"

Prototype_Tile::Prototype_Tile(float in_id, std::string image, float tex_origin[2], float atlas_size)
{
	id = in_id;

	texture_coord[0] = tex_origin[0];
	texture_coord[1] = tex_origin[1];

	texture_coord[2] = tex_origin[0] + (32.0f / atlas_size);
	texture_coord[3] = tex_origin[1];

	texture_coord[4] = tex_origin[0] + (32.0f / atlas_size);
	texture_coord[5] = tex_origin[1] + (32.0f / atlas_size);

	texture_coord[6] = tex_origin[0];
	texture_coord[7] = tex_origin[1] + (32.0f / atlas_size);

	filepath = std::string("files/") + image;
}

Prototype_Tile& Tileset::operator[](int index)
{
	return tileset_tiles[index];
}


Tileset::Tileset(std::string Tileset_path, int texture_unit)
	: shader("res/shaders/tile.shader"),
	vertex_array(),
	vertex_buffer(sizeof(float) * 16),
	size(0),
	index_buffer(1),
	tileset_filepath(Tileset_path)
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


	/* Calculate minimun texture size to fit all tiles in set - maximum 4096 pixels */
	int tilecount = tileset_json["tilecount"];
	for (int test_size_level = 5; test_size_level < 12; test_size_level++)
	{
		if (pow(pow(2, test_size_level), 2) / (32.0 * 32.0) > tilecount)
		{
			size = pow(2, test_size_level);
			break;
		}
	}

	/* Create framebuffer - same as framebuffer.cpp */
	GLCall(glGenFramebuffers(1, &gl_framebuffer_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_id));
	GLCall(glGenTextures(1, &gl_texture_id));
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
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
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
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

	/* Loop through all tiles in tileset file */
	for (auto tile = tileset_json["tiles"].begin(); tile != tileset_json["tiles"].end(); tile++)
	{
		/* Generate new tile protype and add it to the vector */
		Prototype_Tile new_tile((*tile)["id"], (*tile)["image"], tex_coords, size);
		tileset_tiles.push_back(new_tile);

		/* Advance to the next open space on the atlas */
		tex_coords[0] += (32.0f / size);
		if (tex_coords[0] >= 1.0f)
		{
			/* If the end of a row is hit, go back to the start of the next row up */
			tex_coords[0] = 0;
			tex_coords[1] += (32.0f / size);
		}

		/* Load texture image */
		Texture working_texture(new_tile.filepath);
		working_texture.Bind(0);

		/* Draw tile to atlas */
		stitch_tile(new_tile);
	}

	bind_texture(texture_unit);
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

	/* set vertex positions to texture coordinants on atlas */
	vertices[0] = (tile_to_stich.texture_coord[0] * 2.0f) - 1.0f;
	vertices[1] = (tile_to_stich.texture_coord[1] * 2.0f) - 1.0f;

	vertices[4] = ((tile_to_stich.texture_coord[0] + (32.0f / size)) * 2.0f) - 1.0f;
	vertices[5] = (tile_to_stich.texture_coord[1] * 2.0f) - 1.0f;

	vertices[8] = ((tile_to_stich.texture_coord[0] + (32.0f / size)) * 2.0f) - 1.0f;
	vertices[9] = ((tile_to_stich.texture_coord[1] + (32.0f / size)) * 2.0f) - 1.0f;

	vertices[12] = (tile_to_stich.texture_coord[0] * 2.0f) - 1.0f;
	vertices[13] = ((tile_to_stich.texture_coord[1] + (32.0f / size)) * 2.0f) - 1.0f;


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
