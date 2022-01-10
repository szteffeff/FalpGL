#include "Entitiy.h"

Entity::Entity(VertexBuffer *a, Json_loader* load, unsigned int id)
	: loader(load), active_animation(0), momentum(), entity_id(id)
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
	m_quad.translate(momentum[0], momentum[1]);
	momentum[0] *= 0.9;
	momentum[1] *= 0.9;

	if (sqrt(momentum[0] * momentum[0] + momentum[1] * momentum[1]) > 0.125)
	{
		float dir = (atan2(momentum[1], momentum[0]));
		if (dir < 0) { dir += 2 * 3.14159; }
		unsigned int diri = (unsigned int)round(dir / 3.14159 * 2);
		set_animation((diri == 0) ? 4 : diri);
	}
	else
	{
		set_animation(0);
	}

	animations[active_animation].tick();
}

void Entity::walk(float direction, float magnitude)
{
	float dx = cos(direction * 3.14159 / 180) * magnitude;
	float dy = sin(direction * 3.14159 / 180) * magnitude;

	momentum[0] += (dx * 2 - momentum[0]) * 0.1;
	momentum[1] += (dy * 2 - momentum[1]) * 0.1;
}

void Entity::set_animation(int id)
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

void Player::walk(float direction, float magnitude)
{ /* This should all be in Player::tick() */
	if (m_quad.center().x + momentum[0] < -400.0f || m_quad.center().x + momentum[0] > 400.0f)
		player_screen_position[0] += momentum[0]
	else
		m_quad.translate(momentum[0], 0.0f)
		
		
	if (m_quad.center().y + momentum[1] < -400.0f || m_quad.center().y + momentum[1] > 400.0f)
		player_screen_position[1] += momentum[1]
	else
		m_quad.translate(0.0f, momentum[1])
		
		
	momentum[0] *= 0.9;
	momentum[1] *= 0.9;

	if (sqrt(momentum[0] * momentum[0] + momentum[1] * momentum[1]) > 0.125)
	{
		float dir = (atan2(momentum[1], momentum[0]));
		if (dir < 0) { dir += 2 * 3.14159; }
		unsigned int diri = (unsigned int)round(dir / 3.14159 * 2);
		set_animation((diri == 0) ? 4 : diri);
	}
	else
	{
		set_animation(0);
	}

	animations[active_animation].tick();
}

glm::mat4* Player::get_trans_matrix()
{
	return(&player_transform_matrix);
}

const float Player::screen_pos_x()
{
	return(position_on_screen[0]);	
}

const float Player::screen_pos_y()
{
	return(position_on_screen[1]);	
}
