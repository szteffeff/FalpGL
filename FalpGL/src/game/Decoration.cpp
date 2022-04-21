#include "Decoration.h"


Decoration::Decoration(float x, float y, float size_x, float size_y, Prototype_Tile tile)
	: buffer_index(-1), id(-1)
{
	vertex_data[0] = x;                        /* x */
	vertex_data[1] = y;                        /* y */
	vertex_data[2] = tile.texture_coord[0];    /* s */
	vertex_data[3] = tile.texture_coord[1];    /* t */
	vertex_data[4] = 1.0f;                     /* opacity */

	vertex_data[5] = x + size_x;
	vertex_data[6] = y;
	vertex_data[7] = tile.texture_coord[2];
	vertex_data[8] = tile.texture_coord[3];
	vertex_data[9] = 1.0f;

	vertex_data[10] = x + size_x;
	vertex_data[11] = y + size_y;
	vertex_data[12] = tile.texture_coord[4];
	vertex_data[13] = tile.texture_coord[5];
	vertex_data[14] = 1.0f;

	vertex_data[15] = x;
	vertex_data[16] = y + size_y;
	vertex_data[17] = tile.texture_coord[6];
	vertex_data[18] = tile.texture_coord[7];
	vertex_data[19] = 1.0f;
}

void Decoration::fade(float opacity)
{
}

float* Decoration::data()
{
	return &vertex_data[0];
}

Decoration_Renderer::Decoration_Renderer()
	: dec_shader("res/shaders/decoration.shader")
{
}

Decoration_Renderer::Decoration_Renderer(nlohmann::json tileset_json, nlohmann::json decorations)
	: decoration_set(tileset_json, 5), dec_shader("res/shaders/decoration.shader"), dec_vertex_array()
{
	int amount = decorations["objects"].size();
	dec_vertex_buffer.init(amount * 20);
	dec_index_buffer.init(amount);

	for (auto d : decorations["objects"])
	{
		add_decoration(Decoration(d["x"], d["y"], d["width"], d["height"], decoration_set[d["gid"]]));
	}

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	layout.Push<float>(1);

	dec_vertex_buffer.buffer_data(0, vertex_data.size() * sizeof(float), vertex_data.data());
	dec_vertex_array.AddBuffer(dec_vertex_buffer, layout);
}

void Decoration_Renderer::init(nlohmann::json tileset_json, nlohmann::json decorations)
{
	decoration_set.init(tileset_json, texture_index);


	int amount = decorations["objects"].size();
	dec_vertex_buffer.init(amount * 20 * sizeof(float));
	dec_index_buffer.init(amount);

	for (auto d : decorations["objects"])
	{
		add_decoration(Decoration(d["x"], (d["y"]) * -1.0f, d["width"], d["height"], decoration_set[d["gid"]]));
	}

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	layout.Push<float>(1);

	dec_vertex_buffer.buffer_data(0, vertex_data.size() * sizeof(float), vertex_data.data());
	dec_vertex_array.AddBuffer(dec_vertex_buffer, layout);
}


void Decoration_Renderer::add_decoration(Decoration d)
{
	decorations.push_back(d);
	for (int i = 0; i < 20; i++)
	{
		vertex_data.push_back(*(d.data() + i));
	}

}


void Decoration_Renderer::draw(glm::mat4 projection_matrix)
{
	/* Bind GL things*/
	dec_vertex_array.Bind();
	dec_index_buffer.Bind();
	dec_shader.Bind();

	decoration_set.bind_texture(texture_index);
	dec_shader.SetUniformMat4f("u_MVP", projection_matrix);
	dec_shader.SetUniform1iv("u_Textures", 16, samplers);
	dec_shader.SetUniform1f("u_Texture", texture_index);


	GLCall(glDrawElements(GL_TRIANGLES, dec_index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}