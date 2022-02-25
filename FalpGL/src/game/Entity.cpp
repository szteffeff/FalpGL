#include "Entity.h"

entity_return::entity_return(animation_state s)
	: anim_state(s) {}

Entity::Entity(VertexBuffer* a, json load) /* takes loader already scoped to full entity */
	: active_animation(0)
{
	animation_count = load["animation_count"];

	name = load["name"];

	main_quad = Quad(a, load["size"][0], load["size"][1], 0.0f);

	animations.resize(animation_count);

	for (int i = 0; i < animation_count; i++)
	{
		animations[i] = Animation(&main_quad, load["animation_names"][i]);
	}

	set_animation(active_animation);
	animations[active_animation].tick();
}

Entity::Entity() 
	: active_animation(0), animation_count(0)
{}

Entity::~Entity()
{
	main_quad.~Quad();
}

entity_return Entity::tick()
{
	return entity_return(animations[active_animation].tick());
}



void Entity::set_animation(int id)
{
	if (id == active_animation) { return; }
	active_animation = id;
	animations[active_animation].set();
}

void Entity::translate(float x, float y)
{
	main_quad.translate(x, y);
}

void Entity::set_z(float z)
{
	main_quad.set_z(z);
}

void Entity::set_vertex_pos(float x, float y, int index)
{
	main_quad.set_vertex_pos(x, y, index);
}

void Entity::set_vertex_pos(Point p, int index)
{
	main_quad.set_vertex_pos(p, index);
}

Point Entity::get_vertex_pos(int index)
{
	return main_quad.get_vertex_pos(index);
}

Point Entity::center()
{
	return main_quad.center();
}