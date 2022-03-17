#include "Tileset.h"
#include "renderer.h"

Tile_Quad::Tile_Quad()
	: quad_data()
{}

void Tile_Quad::operator=(Tile& tile_in)
{
	quad_data[0] = tile_in.position[0] * 32;  //X
	quad_data[1] = tile_in.position[1] * 32;  //Y

	quad_data[2] = tile_in.texture_coord[0];  //S
	quad_data[3] = tile_in.texture_coord[1];  //T


	quad_data[4] = tile_in.position[0] * 32 + 32;  //X
	quad_data[5] = tile_in.position[1] * 32;  //Y

	quad_data[6] = tile_in.texture_coord[0] + 0.015625f;  //S
	quad_data[7] = tile_in.texture_coord[1];  //T


	quad_data[8] = tile_in.position[0] * 32 + 32;  //X
	quad_data[9] = tile_in.position[1] * 32 + 32;  //Y

	quad_data[10] = tile_in.texture_coord[0] + 0.015625f;  //S
	quad_data[11] = tile_in.texture_coord[1] + 0.015625f;  //T


	quad_data[12] = tile_in.position[0] * 32;  //X
	quad_data[13] = tile_in.position[1] * 32 + 32;  //Y

	quad_data[14] = tile_in.texture_coord[0];  //S
	quad_data[15] = tile_in.texture_coord[1] + 0.015625f;  //T
}

Tile_Quad::Tile_Quad(float qd[16])
{
	memcpy(&quad_data, &qd, sizeof(quad_data));
}


Tileset::Tileset(std::string Tileset_path, int texture_unit)
	: shader("res/shaders/tile.shader"),
	vertex_array(),
	vertex_buffer(24 * sizeof(float)),
	max_tiles(size / 32)
{
	VertexBufferLayout vbl;
	vbl.Push<float>(2);
	vbl.Push<float>(2);

	vertex_array.AddBuffer(vertex_buffer, vbl);

	/* Generate framebuffer object */
	GLCall(glGenFramebuffers(1, &framebuffer_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));

	/* Generater texture */
	GLCall(glGenTextures(1, &texture_id));

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

	/* Attach texture to framebuffer*/
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0));

	/* Generate renderbuffer */
	GLCall(glGenRenderbuffers(1, &renderbuffer_id));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size, size));

	/* Attach renderbuffer to framebuffer*/
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_id));

	/* Check for errors */
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error: " << status << " | " << __FILE__ << ":" << __LINE__ << "\n";
	}

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

	if (tileset_json["tilecount"] > max_tiles)
	{
		/* crash lol */
	}

	for (auto tile = tileset_json["tiles"].begin(); tile != tileset_json["tiles"].end(); tile++)
	{
		tileset_tiles.push_back(Tile());
		/*
		push back tile
		setup to draw tile to buffer
			pick image
				load texture

			set coords
				array access

		draw
		*/
	}
}


void Tileset::stitch_tile(Tile data)
{

}
