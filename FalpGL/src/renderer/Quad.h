#pragma once
#include "BatchRenderer.h"

struct Quad
{
private:
	float quad_data[24];
	void update();

	unsigned int buffer_index;
	VertexBuffer* active_buffer;

protected:


public:
	Quad(VertexBuffer* vb, float height, float width, float size);
	Quad();

	void remove();

	void set_z(float new_h);
	void set_texture_coords(tex_coord new_coords);
	void set_texture_index(float index);
	void set_vertex_pos(float x, float y, int index);
	void set_vertex_pos(Point p, int index);
	void teleport(float new_x, float new_y);

	Point get_vertex_pos(int index);

	void rotate(float degrees, Point point, bool radians = false);
	void scale(float ratio);
	void translate(float delta_x, float delta_y);

	Point center() const;

	float get_height() const;
	float get_width() const;

	float get_y() const;
	float get_x() const;

	Point find_tile();
};