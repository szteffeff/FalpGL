#pragma once

#include "../renderer/RendererIncludes.h"


struct tex_coord {
	float coords[4][2];
};

struct Quad
{
private:
	float quad_data[24];
	void update();

	unsigned int buffer_index;
	VertexBuffer* active_buffer;
protected:


public:
	Quad(VertexBuffer *vb, float height, float width, float size);
	Quad();

	void set_z(float new_h);
	void set_texture_coords(tex_coord new_coords);
	void set_texture_index(float index);
	void set_coords(float x0, float  y0, float x2, float y2);

	void rotate(float degrees, Point point, bool radians = false);
	void scale(float ratio);
	void translate(float delta_x, float delta_y);

	inline Point center() const;

	float get_height() const;
	float get_width() const;

	float get_y() const;
	float get_x() const;

	Point find_tile();
};


class BatchRenderer {
public:
	IndexBuffer index_buffer;
	VertexBufferLayout layout;
	
	VertexArray vertex_array;

	Shader shader;

	int samplers[3] = { 0, 1, 2 };

public:
	VertexBuffer vertex_buffer;

	BatchRenderer(int size, std::string shader_filepath);
	BatchRenderer() {}

	void add_layout(VertexBufferLayout &_layout);

	void draw(glm::mat4 proj_matrix);
};