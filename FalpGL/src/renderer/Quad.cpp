#include "Quad.h"



Quad::Quad(VertexBuffer* vb, float h, float w, float size)
	: buffer_index(0), active_buffer(vb), height(h), width(w)
{

	quad_data[0] = -width / 2;
	quad_data[1] = -height / 2;
	quad_data[2] = 0.0f;

	quad_data[3] = 0.0f;
	quad_data[4] = 0.0f;

	quad_data[5] = 0.0f;


	quad_data[6] = width / 2;
	quad_data[7] = -height / 2;
	quad_data[8] = 0.0f;

	quad_data[9] = 1.0f;
	quad_data[10] = 0.0f;

	quad_data[11] = 0.0f;


	quad_data[12] = width / 2;
	quad_data[13] = height / 2;
	quad_data[14] = 0.0f;

	quad_data[15] = 1.0f;
	quad_data[16] = 1.0f;

	quad_data[17] = 0.0f;


	quad_data[18] = -width / 2;
	quad_data[19] = height / 2;
	quad_data[20] = 0.0f;

	quad_data[21] = 0.0f;
	quad_data[22] = 1.0f;

	quad_data[23] = 0.0f;

	buffer_index = active_buffer->add_quad(quad_data);
}

Point Quad::find_tile()
{
	float x_tile = quad_data[0] / 32.0f + 0.5f;
	float y_tile = quad_data[1] / 32.0f + 0.5f;
	return Point(x_tile, y_tile);
}

Quad::Quad()
	: active_buffer(nullptr), buffer_index(0)
{
	for (int i = 0; i < 24; i++)
	{
		quad_data[i] = 0.0f;
	}
}

void Quad::remove()
{
	for (int i = 0; i < 24; i++)
	{
		quad_data[i] = 0.0f;
	}
	update();
	active_buffer->delete_quad(buffer_index);
}

void Quad::set_z(float new_h)
{
	quad_data[2] = new_h;
	quad_data[8] = new_h;
	quad_data[14] = new_h;
	quad_data[20] = new_h;

	//y_offset += delta_y;
}

void Quad::update()
{
	active_buffer->modify_quad(quad_data, buffer_index);
}

void Quad::teleport(float new_x, float new_y)
{
	Point p = center();

	quad_data[0] = (new_x)+(quad_data[0] - p.x);
	quad_data[1] = (new_y)+(quad_data[1] - p.y);

	quad_data[6] = (new_x)+(quad_data[6] - p.x);
	quad_data[7] = (new_y)+(quad_data[7] - p.y);

	quad_data[12] = (new_x)+(quad_data[12] - p.x);
	quad_data[13] = (new_y)+(quad_data[13] - p.y);

	quad_data[18] = (new_x)+(quad_data[18] - p.x);
	quad_data[19] = (new_y)+(quad_data[19] - p.y);

	update();
}


void Quad::rotate(float degrees, Point point, bool radians)
{
	if (!radians)
		degrees *= glm::pi<float>() / -180;
	else
		degrees *= -1;

	Point current_center = center();

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

	teleport(current_center.x, current_center.y);

	update();
}

void Quad::scale(float ratio)
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

void Quad::translate(float delta_x, float delta_y)
{
	quad_data[0] += delta_x;
	quad_data[1] += delta_y;

	quad_data[6] += delta_x;
	quad_data[7] += delta_y;

	quad_data[12] += delta_x;
	quad_data[13] += delta_y;

	quad_data[18] += delta_x;
	quad_data[19] += delta_y;


	quad_data[0] = round(quad_data[0]);
	quad_data[1] = round(quad_data[1]);

	quad_data[6] = round(quad_data[6]);
	quad_data[7] = round(quad_data[7]);

	quad_data[12] = round(quad_data[12]);
	quad_data[13] = round(quad_data[13]);

	quad_data[18] = round(quad_data[18]);
	quad_data[19] = round(quad_data[19]);

	update();
}

Point Quad::center() const
{
	return Point(((quad_data[0] + quad_data[6] + quad_data[12] + quad_data[18]) / 4), ((quad_data[1] + quad_data[7] + quad_data[13] + quad_data[19]) / 4));
}


float Quad::get_width() const
{
	return ((quad_data[12] + quad_data[0]) / 2);
}

float Quad::get_height() const
{
	return ((quad_data[6] + quad_data[0]) / 2);;
}

float Quad::get_x() const
{
	return (quad_data[0] + (get_width() / 2));
}

float Quad::get_y() const
{
	return (quad_data[1] + (get_height() / 2));
}

void Quad::set_texture_coords(tex_coord new_coords)
{
	quad_data[3] = new_coords.coords[0][0];
	quad_data[4] = new_coords.coords[0][1];

	quad_data[9] = new_coords.coords[1][0];
	quad_data[10] = new_coords.coords[1][1];

	quad_data[15] = new_coords.coords[2][0];
	quad_data[16] = new_coords.coords[2][1];

	quad_data[21] = new_coords.coords[3][0];
	quad_data[22] = new_coords.coords[3][1];

	update();
}

void Quad::set_texture_index(float index)
{
	quad_data[5] = index;
	quad_data[11] = index;
	quad_data[17] = index;
	quad_data[23] = index;

	update();
}

void Quad::set_vertex_pos(float x, float y, int index)
{
	if (index < 0 || index > 3)
	{
		throw "Index out of range";
	}
	quad_data[6 * index] = round(x);
	quad_data[6 * index + 1] = round(y);

	update();
}

void Quad::set_vertex_pos(Point p, int index)
{
	if (index < 0 || index > 3)
	{
		throw "Index out of range";
	}
	quad_data[6 * index] = round(p.x);
	quad_data[6 * index + 1] = round(p.y);

	update();
}


Point Quad::get_vertex_pos(int index)
{
	if (index < 0 || index > 3)
	{
		throw "Index out of range";
	}

	return Point(quad_data[6 * index], quad_data[6 * index + 1]);
}


void Quad::reset()
{
	quad_data[0] = -width / 2;
	quad_data[1] = -height / 2;
	quad_data[2] = 0.0f;

	quad_data[6] = width / 2;
	quad_data[7] = -height / 2;
	quad_data[8] = 0.0f;

	quad_data[12] = width / 2;
	quad_data[13] = height / 2;
	quad_data[14] = 0.0f;

	quad_data[18] = -width / 2;
	quad_data[19] = height / 2;
	quad_data[20] = 0.0f;

	update();
}