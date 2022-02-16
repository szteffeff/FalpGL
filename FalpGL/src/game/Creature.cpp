#include "Creature.h"




Creature::Creature() {}
void Creature::tick() {}



/* Health_Bar */

Health_Bar::Health_Bar(VertexBuffer* buffer)
	: frame(buffer, loader->entities["HEALTH_BAR_FRAME"]),
	red_bar(buffer, loader->entities["HEALTH_BAR_RED"]),
	green_bar(buffer, loader->entities["HEALTH_BAR_GREEN"])
{
	/* move to very front of depth buffer */
	frame.set_z(0.998f);
	red_bar.set_z(0.999f);
	green_bar.set_z(0.999f);

	/* move bar to line up with frame */
	/* (42, -4), (30, -20) for double scale */
	/* (21, -2), (15, -10) for single scale */
	red_bar.translate(42, -4);
	green_bar.translate(30, -20);

	/* move everything to corner of screen */
	frame.translate(x_offset, y_offset);
	red_bar.translate(x_offset, y_offset);
	green_bar.translate(x_offset, y_offset);

	/* get and calculate values to map 0-100 to unknown bar size */
	red_bar_min = red_bar.get_vertex_pos(0).x;
	red_bar_max = red_bar.get_vertex_pos(1).x;
	red_slope = (red_bar_max - red_bar_min) / player_max_val;

	green_bar_min = green_bar.get_vertex_pos(0).x;
	green_bar_max = green_bar.get_vertex_pos(1).x;
	green_slope = (green_bar_max - green_bar_min) / player_max_val;


	/* set to defaults */
	set_health(100);
	set_stamina(100);
	set_level(frame_animations::full);
}

void Health_Bar::tick()
{
	frame.tick();
}

void Health_Bar::set_health(float h)
{
	if (h > 100) { h = 100; }
	if (h < 0) { h = 0; }
	red_bar.set_vertex_pos(red_bar_min + red_slope * h, red_bar.get_vertex_pos(1).y, 1);
	red_bar.set_vertex_pos(red_bar_min + red_slope * h, red_bar.get_vertex_pos(2).y, 2);
}

void Health_Bar::set_stamina(float s)
{
	if (s > 100) { s = 100; }
	if (s < 0) { s = 0; }
	green_bar.set_vertex_pos(green_bar_min + green_slope * s, green_bar.get_vertex_pos(1).y, 1);
	green_bar.set_vertex_pos(green_bar_min + green_slope * s, green_bar.get_vertex_pos(2).y, 2);
}

void Health_Bar::set_level(frame_animations level)
{
	frame.set_animation((int)level);
}

void Health_Bar::tick(float h, float s, frame_animations level)
{
	set_health(h);
	set_stamina(s);
	set_level(level);
	frame.tick();
}

/* Player */

Player::Player(VertexBuffer* vb)
	: m_player(vb, loader->entities["PLAYER"]), momentum(), position()
{}

void Player::walk(float direction, float magnitude)
{
	magnitude *= 3;
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += round(dx);
	momentum[1] += round(dy);
}

void Player::sprint(float direction, float magnitude)
{
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += round(1.5 * dx);
	momentum[1] += round(1.5 * dy);

	magnitude *= 3;

	Stamina -= 0.1;
}

void Player::tick()
{
	offset[0] = 0.0f;
	offset[1] = 0.0f;

	position[0] += momentum[0];
	position[1] += momentum[1];

	m_player.translate(momentum[0], momentum[1]);

	player_transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position[0], -position[1], 0.0f));


	if (sqrt(momentum[0] * momentum[0] + momentum[1] * momentum[1]) > 0.75)
	{
		float dir = (atan2(momentum[1], momentum[0]));
		if (dir < 0) { dir += 2.0f * 3.14159f; }
		unsigned int diri = (unsigned int)round(dir / 3.14159 * 2);
		float real_degrees = (dir * 180.0f) / 3.14159f;

		//std::cout << real_degrees << "\n";

		if (real_degrees < 75 || real_degrees > 285) { m_player.set_animation((int)player_animations::RIGHT); }
		else if (real_degrees > 80 && real_degrees < 100) { m_player.set_animation((int)player_animations::UP); }
		else if (real_degrees > 108 && real_degrees < 255) { m_player.set_animation((int)player_animations::LEFT); }
		else if (real_degrees > 260 && real_degrees < 280) { m_player.set_animation((int)player_animations::DOWN); }
	}
	else
	{
		m_player.set_animation(0);
	}



	momentum[0] = 0;
	momentum[1] = 0;

	m_player.tick();
}
float* Player::GetHealth()
{
	return &Health;
}

float* Player::GetStamina()
{
	return &Stamina;
}

int* Player::GetPotion()
{
	return &Potion;
}

void Player::Take_Damage()
{
	if (Health > 0) { Health -= 25; };
}

void Player::Take_Damage_tile()
{
	if (Health > 0) { Health -= 1; };
}

void Player::Take_Heal()
{
	if (Health < 100 && Potion != 0) {
		Health += 25;
		Potion -= 1;
	};
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

/* #### */