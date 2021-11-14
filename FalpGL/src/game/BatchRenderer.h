#pragma once

#include "../renderer/RendererIncludes.h"
//#include "OverMap.h"


struct tex_coord {
	float coords[2][4];
};

struct quad /*needs to take height and width of initial quad for setup + locatation to generate quad_data. also texture stuff?*/
{
private:
	float quad_data[24];

	/*
	float size[2];
	float position[2];
	float texture_coordinants[2];
	float zposition;
	int texture;
	*/

public:
	float height, width, y, y_offset, position[2];

	unsigned int buffer_index;
	VertexBuffer& active_buffer;

	quad(VertexBuffer& vb, float height, float width, float size);

	float *data();

	void move(float delta_x, float delta_y, bool wrong_function_use_translate);
	void teleport(float new_x, float new_y);

	void modify_height(float delta_y);

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


	Texture textures;

	int samplers[3] = { 0, 1, 2 };

public:
	VertexBuffer vertex_buffer;

	BatchRenderer(int size, std::string shader_filepath);
	BatchRenderer() {}

	void add_layout(VertexBufferLayout &_layout);

	virtual void draw(glm::mat4 proj_matrix);

	void remove_entity(int index);

	void update();

	void tick();
};

class MapRenderer : public BatchRenderer {
public:
	void draw(glm::mat4 &proj_matrix, glm::mat4 &trans_matrix);

	using BatchRenderer::BatchRenderer;
};