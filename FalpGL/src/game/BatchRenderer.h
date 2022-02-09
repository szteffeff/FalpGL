#pragma once

#include "../renderer/RendererIncludes.h"
//#include "OverMap.h"


struct tex_coord {
	float coords[4][2];
};

struct Quad /*needs to take height and width of initial quad for setup + locatation to generate quad_data. also texture stuff?*/
{
protected:
	float quad_data[24];

	/*
	float size[2];
	float position[2];
	float texture_coordinants[2];
	float zposition;
	int texture;
	*/

public:
	void operator=(const Quad&);

public:
	float height, width, y, y_offset, position[2];

	unsigned int buffer_index;
	VertexBuffer *active_buffer;

	Quad(VertexBuffer *vb, float height, float width, float size);
	Quad();

	float *data();

	void move(float delta_x, float delta_y, bool wrong_function_use_translate);
	void teleport(float new_x, float new_y);

	void set_z(float new_h);

	void update();

	void rotate(float degrees, Point point, bool radians = false);
	void scale(float ratio);
	void translate(float delta_x, float delta_y);
	void texture_index(float new_i);

	inline Point center();

	void replace_data(const void* data);

	float get_height() const;
	float get_width() const;

	float get_y() const;
	float get_x() const;

	///////////////

	void set_texture_coords(tex_coord new_coords);
	void set_texture_index(float index);
	Point find_tile();
};

/*
*	X Y Z TX TY TI
*	X Y Z TX TY TI
*	X Y Z TX TY TI
*	X Y Z TX TY TI
* 
*/


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
	void draw(glm::mat4 proj_matrix, glm::mat4 trans_matrix); //for map.shader
};