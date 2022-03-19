#include "Tileset.h"
#include "renderer.h"

Tile::Tile(float in_id, std::string image, float origin[2], float tex_origin[2])
{
	id = in_id;

	position[0] = origin[0];
	position[1] = origin[1];

	texture_coord[0] = tex_origin[0];
	texture_coord[1] = tex_origin[1];

	filename = std::string("files/") + image;
}

Tile_Quad::Tile_Quad()
	: quad_data()
{}

Tile_Quad::Tile_Quad(Tile& tile)
{
	quad_data[0] = tile.position[0] * 32;  //X
	quad_data[1] = tile.position[1] * 32;  //Y

	quad_data[2] = tile.texture_coord[0];  //S
	quad_data[3] = tile.texture_coord[1];  //T


	quad_data[4] = tile.position[0] * 32 + 32;  //X
	quad_data[5] = tile.position[1] * 32;  //Y

	quad_data[6] = tile.texture_coord[0] + 0.015625f;  //S
	quad_data[7] = tile.texture_coord[1];  //T


	quad_data[8] = tile.position[0] * 32 + 32;  //X
	quad_data[9] = tile.position[1] * 32 + 32;  //Y

	quad_data[10] = tile.texture_coord[0] + 0.015625f;  //S
	quad_data[11] = tile.texture_coord[1] + 0.015625f;  //T


	quad_data[12] = tile.position[0] * 32;  //X
	quad_data[13] = tile.position[1] * 32 + 32;  //Y

	quad_data[14] = tile.texture_coord[0];  //S
	quad_data[15] = tile.texture_coord[1] + 0.015625f;  //T
}

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

void Tile_Quad::whole_texture()
{
	quad_data[2] = 0;
	quad_data[3] = 0;

	quad_data[6] = 1;
	quad_data[7] = 0;

	quad_data[10] = 1;
	quad_data[11] = 1;

	quad_data[14] = 0;
	quad_data[15] = 1;
}

Tile& Tileset::operator[](int index)
{
	return tileset_tiles[index];
}

Tileset::Tileset(std::string Tileset_path, int texture_unit)
	: shader("res/shaders/tile.shader"),
	vertex_array(),
	vertex_buffer(sizeof(Tile_Quad)),
	max_tiles((size / 32) * (size / 32)),
	index_buffer(1),
	tileset_filepath(Tileset_path)
{
	projection_matrix = glm::ortho(
		round(-0.5f * size),
		round(0.5f * size),
		round(-0.5f * size),
		round(0.5f * size),
		-1.0f, 1.0f);


	VertexBufferLayout vbl;
	vbl.Push<float>(2);
	vbl.Push<float>(2);

	vertex_array.AddBuffer(vertex_buffer, vbl);
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", projection_matrix);
	shader.SetUniform1i("u_Texture", 0);

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


	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


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


	float position[2] = { 0, 0 };
	float tex_coords[2] = { 0, 0 };

	for (auto tile = tileset_json["tiles"].begin(); tile != tileset_json["tiles"].end(); tile++)
	{
		Tile gen_tile((*tile)["id"], (*tile)["image"], position, tex_coords);
		tileset_tiles.push_back(gen_tile);

		Tile_Quad quad = gen_tile;

		position[0] += 32;
		if (position[0] > size)
		{
			position[0] = 0;
			position[1] += 32;
		}

		tex_coords[0] += 0.015625;
		if (tex_coords[0] > 1.0f)
		{
			tex_coords[0] = 0;
			tex_coords[1] += 0.015625;
		}

		//active_tile_texture.reload(tileset_tiles[index].filename);
		Texture working_texture(gen_tile.filename);
		working_texture.Bind(6);

		stitch_tile(quad);
	}

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));

	
}


void Tileset::stitch_tile(Tile_Quad quad)
{
	glDisable(GL_DEPTH_TEST);
	GLCall(glActiveTexture(GL_TEXTURE0 + 4));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));
	glViewport(0, 0, size, size);


	vertices[0] = (quad.quad_data[2] * 2) - 1.0f;
	vertices[1] = (quad.quad_data[3] * 2) - 1.0f;

	vertices[4] = (quad.quad_data[6] * 2) - 1.0f;
	vertices[5] = (quad.quad_data[7] * 2) - 1.0f;

	vertices[8] = (quad.quad_data[10] * 2) - 1.0f;
	vertices[9] = (quad.quad_data[11] * 2) - 1.0f;

	vertices[12] = (quad.quad_data[14] * 2) - 1.0f;
	vertices[13] = (quad.quad_data[15] * 2) - 1.0f;



	vertex_buffer.buffer_data(0, sizeof(Tile_Quad), &vertices);

	shader.Bind();
	shader.SetUniformMat4f("u_MVP", glm::mat4(1.0f));
	shader.SetUniform1i("u_Texture", 6);
	vertex_array.Bind();
	index_buffer.Bind();

	glDrawElements(GL_TRIANGLES, index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glEnable(GL_DEPTH_TEST);
}

void Tileset::bind_texture(unsigned int unit)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
}
