#include "Entitiy.h"

Sub_Entity::Sub_Entity(VertexBuffer *a, Json_loader* load, unsigned int id)
	: loader(load), active_animation(0), momentum(), entity_id(id)
{
	json j;
	j = loader->entities[std::to_string(id)];

	name = j["name"];

	animation_count = j["animation_count"];

	main_quad = Quad(a, j["size"][0], j["size"][1], 0.0f);

	animations.resize(animation_count);

	for (int i = 0; i < animation_count; i++)
	{
		animations[i] = Animation(&main_quad, loader, j["animations"][i]);
	}

	set_animation(active_animation);
	animations[active_animation].tick();
}

Sub_Entity::Sub_Entity(VertexBuffer* a, json load) /* takes loader already scoped to full entity */
	: loader(), active_animation(0), momentum()
{
	json j;
	j = load;

	name = "";

	animation_count = j["animation_count"];

	main_quad = Quad(a, j["size"][0], j["size"][1], 0.0f);

	animations.resize(animation_count);

	for (int i = 0; i < animation_count; i++)
	{
		animations[i] = Animation(&main_quad, loader, j["animations"][i]);
	}

	set_animation(active_animation);
	animations[active_animation].tick();
}

Sub_Entity::Sub_Entity() {}

void Sub_Entity::tick()
{
	main_quad.translate(momentum[0], momentum[1]);
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

void Sub_Entity::walk(float direction, float magnitude)
{
	magnitude *= 3;
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += dx;
	momentum[1] += dy;
}

void Sub_Entity::set_animation(int id)
{
	if (id == active_animation) { return; }
	animations[active_animation].unset();
	active_animation = id;
	animations[active_animation].set();
}

Quad* Sub_Entity::get_quad()
{
	return &main_quad;
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

Health_Bar::Health_Bar(BatchRenderer* for_active_buffer, Json_loader* l)
	: frame(&for_active_buffer->vertex_buffer, l->entities["1"]["flask"]),
	red_bar(&for_active_buffer->vertex_buffer, l->entities["1"]["red_bar"]),
	green_bar(&for_active_buffer->vertex_buffer, l->entities["1"]["green_bar"])
{}
