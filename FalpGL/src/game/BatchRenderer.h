#pragma once

#include "../renderer/RendererIncludes.h"
//#include "OverMap.h"


struct quad /*needs to take height and width of initial quad for setup + locatation to genorate quad_data. also texture stuff?*/
{
private:
	float quad_data[24];
public:
	float height, width, y, y_offset, position[2];

	unsigned int buffer_index;
	
	VertexBuffer& active_buffer;

	quad(VertexBuffer& vb, float height, float width);

	float *data();

	void move(float delta_x, float delta_y);
	void teleport(float x, float new_y);

	void modify_height(float delta_y);

	void update();

	void replace_data(const void* data);

	float get_height();
	float get_width();

	float get_y();
	float get_x();

};

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