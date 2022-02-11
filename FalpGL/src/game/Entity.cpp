#include "Entity.h"

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

void Entity::tick()
{
	animations[active_animation].tick();
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


