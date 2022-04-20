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

float Creature::Player_Detection_distance_Horizontal(float x, float* player_x)
{
	bool direction = Player_Detection_simple_horizontal(x, player_x);
	float horizontal;

	if (direction == true) {
		horizontal = *player_x - x;
	}
	else {
		horizontal = x - *player_x;
	}
	return horizontal;
}

float Creature::Player_Detection_distance_Vertical(float y, float* player_y)
{
	bool direction = Player_Detection_simple_horizontal(y, player_y);
	float vertical;

	if (direction == true) {
		vertical = *player_y - y;
	}
	else {
		vertical = y - *player_y;
	}
	return vertical;
}

float Creature::Player_Detetion_distance(float horizontal, float vertical)
{
	float distance = sqrt(pow(horizontal, 2) + pow(vertical, 2));
	return distance;
}

void Creature::Player_Health(float* health)
{
	player_health = health;
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
	: m_player(vb, loader->entities["PLAYER"]), momentum(), position(), Player_bow(vb, loader->entities["Player_Bow"])
{
	Player_bow.teleport(200000, 2000000);
}

void Player::walk(float direction, float magnitude)
{
	magnitude *= 3;
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += round(dx);
	momentum[1] += round(dy);

	Recover_Stamina();
	//walk_noise();
	walking_sound.Play_sound(walking);
}

void Player::sprint(float direction, float magnitude)
{
	magnitude *= 3;
	float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
	float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

	momentum[0] += round(1.5 * dx);
	momentum[1] += round(1.5 * dy);

	//Lose_Stamina();
	
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
		if (active_map->collision_circle(position[0] + momentum[0], position[1] + momentum[1] - 47, 100))
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
		m_player.set_animation(0);
		Recover_Stamina();
		walking_sound.Stop_sound(walking);
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
	hurt_sound.Play_sound(hurt);
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

	if (tick_state.anim_state == animation_state::advanced_frame)
	{
		frame++;
		if (frame >= 3)
			frame = 0;
	}

	if (frame == 2)
	{

		static int frame;
		static int bops;
		static int bops_frame;
		entity_return tick_state = Red_slime.tick();

		bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
		bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);

		if (tick_state.anim_state == animation_state::advanced_frame)
		{
			frame++;
			if (frame >= 3)
				frame = 0;
		}

		if (frame == 2)
		{
			float magnitude = 2;
			if (bops < 3) {
				if (horizontal == true) { momentum[0] += magnitude; }
				else if (horizontal == false) { momentum[0] += -magnitude; }

				if (vertical == false) { momentum[1] += -magnitude; }
				else if (vertical == true) { momentum[1] += magnitude; }

			}
			else if (bops > 3) {
				magnitude = 2;
				if (horizontal == true) { momentum[0] += -magnitude; }
				else if (horizontal == false) { momentum[0] += magnitude; }

				if (vertical == false) { momentum[1] += magnitude; }
				else if (vertical == true) { momentum[1] += -magnitude; }
				bops_frame += 1;
			}
			if (bops_frame > 60 * 2) {
				bops_frame = 0;
				bops = 0;
			}

			position[0] += round(momentum[0]);
			position[1] += round(momentum[1]);

			Red_slime.translate(round(momentum[0]), round(momentum[1]));

			momentum[0] = 0;
			momentum[1] = 0;
		}

		if (abs(position[0] - *player_position_x) < 40 and abs(position[1] - *player_position_y) < 40) {
			*player_health -= 1;
			bops += 1;
		}
	}
}

Enemy_Ghost::Enemy_Ghost(VertexBuffer* vb)
	: Enemy_ghost(vb, loader->entities["Enemy_Ghost"]), momentum(), position(), Wizard_pink_bullet(vb, loader->entities["Wizard_Pink_bullet"])
{
	Enemy_ghost.set_animation(0);
	Wizard_pink_bullet.teleport(10000000, 10000000);
}

void Enemy_Ghost::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Enemy_Ghost::tick()
{
	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);
	entity_return tick_state = Enemy_ghost.tick();
	
	static int frames = 0;
	static int frames_magic = 0;
	static int teleporting = 0;

	if (tick_state.anim_state == animation_state::ended) {
		Enemy_ghost.set_animation(0);
	}

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 500 )

		if (frames++ == 60 * 6) {
			int random = 1 + (rand() % 2);

			if (random == 1 and teleporting > 0) {
				std::cout << "not moving" << std::endl;
			}
			else {
				Enemy_ghost.set_animation(1);
				teleporting += 1;
				
			}
			frames = 0;
		}

	if (teleporting > 1 and tick_state.anim_state == animation_state::ended) {
		Enemy_ghost.set_animation(0);
		teleporting = 0;
	}

	if (teleporting > 0) {
		if (tick_state.anim_state == animation_state::ended) {
			Enemy_ghost.set_animation(2);
			position[0] = *player_position_x + (rand() % 1000) - 500;
			position[1] = *player_position_y + (rand() % 1000) - 500;
			Ghost_move_sound.Play_sound(Ghost_move);
			std::cout << "moving" << std::endl;
			Enemy_ghost.teleport(position[0], position[1]);
			teleporting++;
		}
	}
	
	static float dx = 0, dy = 0;
	if (frames_magic++ == 60 * 4 and not teleporting) {
		Wizard_pink_bullet.teleport(position[0], position[1]);
		std::cout << "Yeet thy bullet" << std::endl;
		frames_magic = 0;
		float direction = atan2(*player_position_y - position[1], *player_position_x - position[0]) - atan2(position[1] - position[1], position[0] - position[0]);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		std::cout << direction << std::endl;
		dx = (float)(cos(direction)) * 3;
		dy = (float)(sin(direction)) * 3;
	}

	Wizard_pink_bullet.translate(dx, dy);

	if (abs(Wizard_pink_bullet.center().x - *player_position_x) < 20 and abs(Wizard_pink_bullet.center().y - *player_position_y) < 20) {
		*player_health -= 1;
		std::cout << "IVE BEEN HIT" << std::endl;
	}
}

Garfield::Garfield(VertexBuffer* vb)
	: garfield(vb, loader->entities["Garfield"])
{
	garfield.set_animation(0);
}
void Garfield::tick()
{
	garfield.tick();
	garfield.teleport(-100, -100);
}

Bush_Boi::Bush_Boi(VertexBuffer* vb)
	: Bush_boi(vb, loader->entities["Bush_Boi"])
{
	Bush_boi.set_animation(0);
	Bush_boi.teleport(100, 100);
}

void Bush_Boi::tick()
{
	Bush_boi.tick();
}

Chompy_Slime::Chompy_Slime(VertexBuffer* vb)
	: Chompy_slime(vb, loader->entities["Chompy_Slime"]), momentum(), position()
{
	Chompy_slime.set_animation(0);
}

void Chompy_Slime::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Chompy_Slime::tick()
{
	static int frame;
	static int bops;
	static int bops_frame;
	entity_return tick_state = Chompy_slime.tick();

	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);

	if (tick_state.anim_state == animation_state::advanced_frame)
	{
		frame++;
		if (frame >= 3)
			frame = 0;
	}

	if (frame == 2)
	{
		float magnitude = 2;
		if (bops < 3) {
			if (horizontal == true) { momentum[0] += magnitude; }
			else if (horizontal == false) { momentum[0] += -magnitude; }

			if (vertical == false) { momentum[1] += -magnitude; }
			else if (vertical == true) { momentum[1] += magnitude; }

		}
		else if (bops > 3) {
			magnitude = 2;
			if (horizontal == true) { momentum[0] += -magnitude; }
			else if (horizontal == false) { momentum[0] += magnitude; }

			if (vertical == false) { momentum[1] += magnitude; }
			else if (vertical == true) { momentum[1] += -magnitude; }
			bops_frame += 1;
		}
		if (bops_frame > 60 * 2) {
			bops_frame = 0;
			bops = 0;
		}
		
		position[0] += round(momentum[0]);
		position[1] += round(momentum[1]);

		Chompy_slime.translate(round(momentum[0]), round(momentum[1]));

		momentum[0] = 0;
		momentum[1] = 0;
	}

	if (abs(position[0] - *player_position_x) < 40 and abs(position[1] - *player_position_y) < 40) {
		*player_health -= 1;

		bops += 1;

		/*
		float dx = 0, dy = 0;

		float direction = atan2(*player_position_y, *player_position_x) - atan2(position[1], position[1]);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		std::cout << direction << std::endl;
		dx = (float)(cos(direction)) * 10;
		dy = (float)(sin(direction)) * 10;

		position[0] += -dx;
		position[1] += -dy;

		Chompy_slime.teleport(position[0], position[1]);
		frame = 0;
		*/
	}
}
