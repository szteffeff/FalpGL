#pragma once
#include <vector>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

struct Particle{
	/* Position 2, RGB 3, alpha 1, size 1, age 1*/
	/* Position - 2, RGB - 3, Opacity - 1, Size - 1*/
	float vertex_data[7];

	float momentum[2];
	float hue_momentum;
	float saturation_momentum;
	float lighness_momentum;


	Particle();
	~Particle();
	virtual float* tick();
};


class Particle_Renderer {
	Shader map_shader;
	VertexArray map_vertex_array;
	VertexBuffer map_vertex_buffer;
	IndexBuffer map_index_buffer;

	const size_t particle_size = sizeof(float) * 7;
	const size_t size;

	int current_index;

	std::vector<float> particle_vertex_data;
	std::vector<Particle> particles;
	std::vector<int> free_indexes;


public:
	Particle_Renderer(const size_t amount);

	void tick(float delta_time = 1);
	void add_particle(Particle particle);
};