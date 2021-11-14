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

quad::quad(VertexBuffer& vb, float h, float w, float size)
	: y(0.0f), y_offset(0.0f), buffer_index(0), active_buffer(vb), height(h), width(w)
{
	float hs = size / 2;

	quad_data[0] = -hs;
	quad_data[1] = -hs;
	quad_data[2] = 0.0f;

	quad_data[6] = hs;
	quad_data[7] = -hs;
	quad_data[8] = 0.0f;

	quad_data[12] = hs;
	quad_data[13] = hs;
	quad_data[14] = 0.0f;

	quad_data[18] = -hs;
	quad_data[19] = hs;
	quad_data[20] = 0.0f;


	quad_data[3] = 0.0f;
	quad_data[4] = 0.0f;

	quad_data[9] = 1.0f;
	quad_data[10] = 0.0f;

	quad_data[15] = 1.0f;
	quad_data[16] = 1.0f;

	quad_data[21] = 0.0f;
	quad_data[22] = 1.0f;


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

void quad::move(float delta_x, float delta_y, bool wrong_function_use_translate)
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

void quad::teleport(float new_x, float new_y)
{
	Point p = center();
	quad_data[0]  =  (new_x) + (quad_data[0] - p.x);
	quad_data[1]  =  (new_y) + (quad_data[1] - p.y);

	quad_data[6]  =  (new_x) + (quad_data[6] - p.x);
	quad_data[7]  =  (new_y) + (quad_data[7] - p.y);

	quad_data[12] = (new_x) + (quad_data[12] - p.x);
	quad_data[13] = (new_y) + (quad_data[13] - p.y);
											  
	quad_data[18] = (new_x) + (quad_data[18] - p.x);
	quad_data[19] = (new_y) + (quad_data[19] - p.y);

	update();
}

void quad::modify_height(float delta_y)
{
	quad_data[2] = delta_y;
	quad_data[8] = delta_y;
	quad_data[14] = delta_y;
	quad_data[20] = delta_y;

	//y_offset += delta_y;
}

void quad::update()
{
	active_buffer.modify_quad(quad_data, buffer_index);
}


void quad::rotate(float degrees, Point point, bool radians)
{
	if (!radians)
		degrees *= glm::pi<float>() / -180;
	else
		degrees *= -1;

	float x = quad_data[0];
	float y = quad_data[1];
	quad_data[0] = x * cos(degrees) - y * sin(degrees);
	quad_data[1] = x * sin(degrees) + y * cos(degrees);

	x = quad_data[6];
	y = quad_data[7];
	quad_data[6] = x * cos(degrees) - y * sin(degrees);
	quad_data[7] = x * sin(degrees) + y * cos(degrees);

	x = quad_data[12];
	y = quad_data[13];
	quad_data[12] = x * cos(degrees) - y * sin(degrees);
	quad_data[13] = x * sin(degrees) + y * cos(degrees);

	x = quad_data[18];
	y = quad_data[19];
	quad_data[18] = x * cos(degrees) - y * sin(degrees);
	quad_data[19] = x * sin(degrees) + y * cos(degrees);

	update();
}

void quad::scale(float ratio)
{
	quad_data[0] *= ratio;
	quad_data[1] *= ratio;

	quad_data[6] *= ratio;
	quad_data[7] *= ratio;

	quad_data[12] *= ratio;
	quad_data[13] *= ratio;

	quad_data[18] *= ratio;
	quad_data[19] *= ratio;

	update();
}

void quad::translate(float delta_x, float delta_y)
{
	quad_data[0] += delta_x;
	quad_data[1] += delta_y;

	quad_data[6] += delta_x;
	quad_data[7] += delta_y;

	quad_data[12] += delta_x;
	quad_data[13] += delta_y;

	quad_data[18] += delta_x;
	quad_data[19] += delta_y;

	update();
}

void quad::texture_index(float new_i)
{  //5 11 17 23
	quad_data[5] = new_i;
	quad_data[11] = new_i;
	quad_data[17] = new_i;
	quad_data[23] = new_i;

	update();
}

inline Point quad::center()
{
	return Point(((quad_data[0] + quad_data[6] + quad_data[12] + quad_data[18]) / 4), ((quad_data[1] + quad_data[7] + quad_data[13] + quad_data[19]) / 4));
}


void quad::replace_data(const void* data)
{
	memcpy(&quad_data, data, 24 * sizeof(float));
}


float quad::get_width() const
{
	return ((quad_data[12] + quad_data[0]) / 2);
}

float quad::get_height() const
{
	return ((quad_data[6] + quad_data[0]) / 2);;
}

float quad::get_x() const
{
	return (quad_data[0] + (get_width() / 2));
}

float quad::get_y() const
{
	return (quad_data[1] + (get_height() / 2));
}

void quad::set_texture_coords(tex_coord new_coords)
{
	quad_data[3] = new_coords.coords[0][0];
	quad_data[4] = new_coords.coords[0][1];

	quad_data[9] = new_coords.coords[1][0];
	quad_data[10] = new_coords.coords[1][1];

	quad_data[15] = new_coords.coords[2][0];
	quad_data[16] = new_coords.coords[2][1];

	quad_data[21] = new_coords.coords[3][0];
	quad_data[22] = new_coords.coords[3][1];
}
