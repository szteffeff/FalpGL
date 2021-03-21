 #include "BatchRenderer.h"

BatchRenderer::BatchRenderer(int size, std::string shader_filepath)
	: index_buffer(size), shader(shader_filepath), vertex_buffer(size * 24 * 10 * sizeof(float)), textures("res/textures/Grass128.png")
{
}

void BatchRenderer::add_layout(VertexBufferLayout &_layout)
{
	vertex_array.Bind();
	vertex_buffer.Bind();

	vertex_array.AddBuffer(vertex_buffer, _layout);
}

void BatchRenderer::draw(glm::mat4 proj_matrix)
{
	vertex_array.Bind();
	shader.Bind();
	index_buffer.Bind();
	

	textures.Bind(1);

	shader.SetUniformMat4f("u_MVP", proj_matrix);
	shader.SetUniform1iv("u_Textures", 3, samplers);

	GLCall(glDrawElements(GL_TRIANGLES, index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr))
}

void BatchRenderer::remove_entity(int index)
{
}

void BatchRenderer::update()
{

}

void BatchRenderer::tick()
{
}

void MapRenderer::draw(glm::mat4& proj_matrix, glm::mat4& trans_matrix)
{
	vertex_array.Bind();
	shader.Bind();
	index_buffer.Bind();


	textures.Bind(1);

	shader.SetUniformMat4f("u_MVP", proj_matrix);
	shader.SetUniformMat4f("u_transform", trans_matrix);
	shader.SetUniform1iv("u_Textures", 3, samplers);

	GLCall(glDrawElements(GL_TRIANGLES, index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr))
}

quad::quad(VertexBuffer& vb, float h, float w)
	: y(0.0f), y_offset(0.0f), buffer_index(0), active_buffer(vb), height(h), width(w)
{
	quad_data[0] = 0.0f;
	quad_data[1] = 0.0f;
	quad_data[2] = 0.0f;

	quad_data[6] = 0.0f;
	quad_data[7] = 0.0f;
	quad_data[8] = 0.0f;

	quad_data[12] = 0.0f;
	quad_data[13] = 0.0f;
	quad_data[14] = 0.0f;

	quad_data[18] = 0.0f;
	quad_data[19] = 0.0f;
	quad_data[20] = 0.0f;


	quad_data[3] = 0.0f;
	quad_data[4] = 0.0f;

	quad_data[9] = 0.0f;
	quad_data[10] = 0.0f;

	quad_data[15] = 1.0f;
	quad_data[16] = 0.0f;

	quad_data[21] = 0.0f;
	quad_data[22] = 0.0f;


	quad_data[5] = 0.0f;
	quad_data[11] = 0.0f;
	quad_data[17] = 0.0f;
	quad_data[23] = 0.0f;

	position[0] = 0.0f;
	position[1] = 0.0f;
	buffer_index = active_buffer.add_quad(quad_data);
}

float *quad::data()
{
	return quad_data;
}

void quad::move(float delta_x, float delta_y)
{
	quad_data[0] += delta_x;
	quad_data[1] += delta_y;
	quad_data[2] += delta_y;

	quad_data[6] += delta_x;
	quad_data[7] += delta_y;
	quad_data[8] += delta_y;

	quad_data[12] += delta_x;
	quad_data[13] += delta_y;
	quad_data[14] += delta_y;

	quad_data[18] += delta_x;
	quad_data[19] += delta_y;
	quad_data[20] += delta_y;

	y += delta_y;

}

void quad::teleport(float x, float new_y)
{
	quad_data[0] = x;
	quad_data[1] = new_y;
	quad_data[2] = new_y;

	quad_data[6] = x;
	quad_data[7] = new_y;
	quad_data[8] = new_y;

	quad_data[12] = x;
	quad_data[13] = new_y;
	quad_data[14] = new_y;

	quad_data[18] = x;
	quad_data[19] = new_y;
	quad_data[20] = new_y;

	y_offset += new_y;
	y = new_y;
}

void quad::modify_height(float delta_y)
{
	quad_data[2] += delta_y;
	quad_data[8] += delta_y;
	quad_data[14] += delta_y;
	quad_data[20] += delta_y;

	y_offset += delta_y;
}

void quad::update()
{
	active_buffer.modify_quad(quad_data, buffer_index);
}


void quad::replace_data(const void* data)
{
	memcpy(&quad_data, data, 24 * sizeof(float));
}


float quad::get_width()
{
	return ((quad_data[12] + quad_data[0]) / 2);
}

float quad::get_height()
{
	return ((quad_data[6] + quad_data[0]) / 2);;
}

float quad::get_x()
{
	return (quad_data[0] + (get_width() / 2));
}

float quad::get_y()
{
	return (quad_data[1] + (get_height() / 2));
}
