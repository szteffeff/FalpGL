#include "Entitiy.h"

Entity::Entity()
{
}

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

	set_animation(active_animation);
	animations[active_animation].tick();
}

void Entity::tick()
{
	m_quad.translate(momentum[0], momentum[1]);
	momentum[0] *= 0.9;
	momentum[1] *= 0.9;

	if (sqrt(momentum[0] * momentum[0] + momentum[1] * momentum[1]) > 0.75)
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
	magnitude *= 3;
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += dx;
	momentum[1] += dy;
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

void Player::tick()
{
	offset[0] = 0.0f;
	offset[1] = 0.0f;

	position[0] += momentum[0];
	position[1] += momentum[1];

	m_quad.translate(momentum[0], momentum[1]);



	player_transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position[0], -position[1], 0.0f));


	if (sqrt(momentum[0] * momentum[0] + momentum[1] * momentum[1]) > 0.75)
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

	momentum[0] = 0;
	momentum[1] = 0;

	animations[active_animation].tick();

	//std::cout << "position: " << position[0] << ", " << position[1] << "\n";
}

float* Player::GetHealth()
{
	return &Health;
}

glm::mat4* Player::get_trans_matrix()
{
	return(&player_transform_matrix);
}

float Player::position_y()
{
	return position[1];
}

float Player::position_x()
{
	return position[0];
}