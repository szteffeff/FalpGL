#include "Creature.h"




Creature::Creature() {}

bool Creature::Player_Detection_simple_horizontal(float x, float* player_x)
{
	if (x < *player_x){ // if thing is left of player
		return true;
	}
	else // if thing is to the left of player
	{
		return false;
	}
}

bool Creature::Player_Detectoin_simple_vertical(float y, float* player_y)
{
	if (y < *player_y) { // if thing is underneath player
		return true;
	}
	else // if thing is above player
	{
		return false;
	}

}

void Creature::walk(float direction, float magnitude)
{
	magnitude *= 3;
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += round(dx);
	momentum[1] += round(dy);
}



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

	Recover_Stamina();
	walk_noise();
}

void Player::walk_noise()
{
	ALint state = false;
	if (state != AL_PLAYING) {
		//alSourcei(walking, AL_LOOPING, AL_TRUE);
		creaturesound.Play(walking);
		console_log("Playing walking noise");
	}
}


void Player::sprint(float direction, float magnitude)
{
	magnitude *= 3;
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += round(1.5 * dx);
	momentum[1] += round(1.5 * dy);

	Lose_Stamina();
	
}

void Player::set_active_map(New_Map* map)
{
	active_map = map;
}

void Player::tick()
{
	offset[0] = 0.0f;
	offset[1] = 0.0f;

	if (active_map)
	{
		if (active_map->collision_at(position[0] + momentum[0], position[1] + momentum[1]))
		{} /* If collision, do nothing */
		else
		{
			position[0] += momentum[0];
			position[1] += momentum[1];

			m_player.translate(momentum[0], momentum[1]);

			player_transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position[0], -position[1], 0.0f));
		}
	}
	else
	{
		position[0] += momentum[0];
		position[1] += momentum[1];

		m_player.translate(momentum[0], momentum[1]);

		player_transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position[0], -position[1], 0.0f));
	}



	if (sqrt(momentum[0] * momentum[0] + momentum[1] * momentum[1]) > 0.75)
	{
		float dir = (atan2(momentum[1], momentum[0]));
		if (dir < 0) { dir += 2.0f * 3.14159f; }
		float real_degrees = (dir * 180.0f) / 3.14159f;

		//std::cout << real_degrees << "\n";

		if (real_degrees < 75 || real_degrees > 285) { m_player.set_animation((int)player_animations::RIGHT); }
		else if (real_degrees > 80 && real_degrees < 100) { m_player.set_animation((int)player_animations::UP); }
		else if (real_degrees > 108 && real_degrees < 255) { m_player.set_animation((int)player_animations::LEFT); }
		else if (real_degrees > 260 && real_degrees < 280) { m_player.set_animation((int)player_animations::DOWN); }
	}
	else
	{
		//stop_walknoise();
		m_player.set_animation(0);
		Recover_Stamina();
		creaturesound.Stop();
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

void Player::Recover_Stamina()
{
	Stamina += 0.05;
}

void Player::Lose_Stamina()
{
	Stamina -= 0.1;
}

glm::mat4* Player::get_trans_matrix()
{
	return(&player_transform_matrix);
}

float Player::position_y()
{
	return position[1];
}

float* Player::get_position_x()
{
	return &position[0];
}

float* Player::get_position_y()
{
	return &position[1];
}

float Player::position_x()
{
	return position[0];
}

/* #### */

Red_Slime::Red_Slime(VertexBuffer* vb)
	: Red_slime(vb, loader->entities["Red_Slime"]), momentum(), position()
{
	Red_slime.set_animation(0);
}

void Red_Slime::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Red_Slime::tick()
{
	static int frame;
	entity_return tick_state = Red_slime.tick();

	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);
	
	/*
	float distance_x = abs(*player_position_x - position[0]);
	float distance_y = abs(*player_position_y - position[1]);

	float direction = asinf(distance_x / distance_y);
	float dx = 0, dy = 0;
	float magnitude = 1;

	if (horizontal == true) { 
		float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	}
	else if (horizontal == false) {
		float dx = (float)(-cos(direction * 3.14159 / 180)) * magnitude;
	}

	if (vertical == false) {
		float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;
	}
	else if (vertical == true) { 
		float dy = (float)(-sin(direction * 3.14159 / 180)) * magnitude;
	}

	momentum[0] += round(dx);
	momentum[1] += round(dy);
	*/

	/* 
	float dir = (atan2(*player_position_x- position[0], *player_position_y - position[0]));
	if (dir < 0) { dir += 2.0f * 3.14159f; }
	float real_degrees = ((dir * 180.0f) / 3.14159f);

	float magnitude = 1;
	float dx = (float)(cos(real_degrees * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(real_degrees * 3.14159 / 180)) * magnitude;

	momentum[0] += round(dx);
	momentum[1] += round(dy);
	*/

	if (tick_state.anim_state == animation_state::advanced_frame)
	{
		frame++;
		if (frame >= 3)
			frame = 0;
	}

	if (frame == 2)
	{
		int magnitude = 2;
		if (horizontal == true) { momentum[0] += magnitude; }
		else if (horizontal == false) { momentum[0] += -magnitude; }

		if (vertical == false) { momentum[1] += -magnitude; }
		else if (vertical == true) { momentum[1] += magnitude; }

		position[0] += momentum[0];
		position[1] += momentum[1];

		Red_slime.translate(momentum[0], momentum[1]);

		momentum[0] = 0;
		momentum[1] = 0;
	}
}

Enemy_Ghost::Enemy_Ghost(VertexBuffer* vb)
	: Enemy_ghost(vb, loader->entities["Enemy_Ghost"]), momentum(), position()
{
	Enemy_ghost.set_animation(0);
}

void Enemy_Ghost::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Enemy_Ghost::tick()
{
	Enemy_ghost.tick();
	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);

	if (horizontal == true) { momentum[0] = 1; } // right
	else if (horizontal == false) { momentum[0] = -1; }  //left

	if (vertical == false) { momentum[1] = -1; }  // down
	else if (vertical == true) { momentum[1] = 1; } // up

	position[0] += momentum[0];
	position[1] += momentum[1];


	Enemy_ghost.translate(momentum[0], momentum[1]);

	momentum[0] = 0;
	momentum[1] = 0;
}
