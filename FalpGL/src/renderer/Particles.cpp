#include "Particles.h"


Particle::Particle()
{
}


Particle::~Particle()
{
}


float* Particle::tick()
{
	return nullptr;
}





Particle_Renderer::Particle_Renderer(const size_t amount)
	: size(amount)
{
	particles.resize(size);
	particle_vertex_data.resize(size * particle_size, 0.0f);
}


void Particle_Renderer::tick(float delta_time)
{
}


void Particle_Renderer::add_particle(Particle particle)
{
	if (free_indexes.empty())
	{

	}
	else
	{
		particles[current_index++] = particle;
		if (current_index >= size)
		{
			console_log("[WARN]: Particle renderer overflow");
			current_index = 0;
		}
	}
}
