#include "Entitiy.h"

Entity::Entity(VertexBuffer *a, Json_loader* load, unsigned int id)
	:	loader(load), active_animation(0), current_direction(0), entity_id(id)
{
	json j;
	j = loader->entities[std::to_string(id)];

	name = j["name"];

	animation_count = j["animation_count"];

	m_quad = Quad(a, j["size"][0], j["size"][1], 0.0f);

	animations.resize(animation_count);

	for (int i = 0; i < animation_count; i++)
	{
		animations[i] = Animation(&m_quad, loader, j["animations"][i]);
	}

	animations[active_animation].set();
}

void Entity::tick()
{
	animations[active_animation].tick();
}

void Entity::walk(float direction, float magnitude)
{
	float dx = cos(direction * 3.14159 / 180) * magnitude;
	float dy = sin(direction * 3.14159 / 180) * magnitude;

	m_quad.translate(dx, dy);
}

void Entity::set_animation(animation_id id)
{
	if (id == active_animation) { return; }
	animations[active_animation].unset();
	active_animation = id;
	animations[active_animation].set();
}

Quad* Entity::get_quad()
{
	return &m_quad;
}
