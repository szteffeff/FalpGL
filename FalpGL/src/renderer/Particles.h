#pragma once
#include <vector>

struct Particle{
	/* Position 2, RGB 3, alpha 1, size 1, age 1*/
	float vertex_data[8];

	float momentum[2];
	float hue_momentum;
	float saturation_momentum;
	float lighness_momentum;


	Particle();
	~Particle();
	virtual float* tick();
};


class Particle_Renderer {
	const size_t particle_size = sizeof(float) * 6;
	const unsigned int particle_amount;
	std::vector<float> particle_vertex_data;


public:
	Particle_Renderer(const unsigned int amount);

	void tick(float delta_time = 1);
	void add_particle(Particle particle);
};