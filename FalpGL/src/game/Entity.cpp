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

void Player::walk(float direction, float magnitude)
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
	active_animation = id;
	animations[active_animation].set();
}

void Player::tick()
{
	offset[0] = 0.0f;
	offset[1] = 0.0f;

	position[0] += momentum[0];
	position[1] += momentum[1];

	main_quad.translate(momentum[0], momentum[1]);

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


