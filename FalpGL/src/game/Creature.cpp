#include "Creature.h"




Creature::Creature() {}

bool Creature::Player_Detection_simple_horizontal(float x, float* player_x)
{
	if (x < *player_x) { // if thing is left of player
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

void Creature::attacked(float x, float y, float radius, int attack, float* health, float* monster_x, float* monster_y)
{
	if (sqrt(pow(abs(*monster_x - x), 2) + pow(abs(*monster_y - y), 2)) < radius) {
		if (attack == 0) {*health -= 14;}
		else if (attack == 1) { *health -= 9; }
		else if (attack == 2) { *health -= 12; }
		else if (attack == 3) { *health -= 10; }
		else if (attack == 4) { *health -= 35; }
		else if (attack == 5) { *health -= 15; }
		else if (attack == 6) { *health -= 25; }
	}
}

void Creature::get_weapon(int* weapon)
{
	int* using_weapon = weapon;
}

void Creature::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}



float Creature::Arrow_Detection_horizontal(float x, float* arrow_x)
{
	bool direction = Player_Detection_simple_horizontal(x, arrow_x);
	float horizontal;

	if (direction == true) {horizontal = *arrow_x - x;}
	else {horizontal = x - *arrow_x;}
	return horizontal;
}

float Creature::Arrow_Detection_vertical(float y, float* arrow_y)
{
	bool direction = Player_Detection_simple_horizontal(y, arrow_y);
	float vertical;

	if (direction == true) {vertical = *arrow_y - y;}
	else {vertical = y - *arrow_y;}
	return vertical;
}

float Creature::Dagger_Detection_horizontal(float x, float* dagger_x)
{
	bool direction = Player_Detection_simple_horizontal(x, dagger_x);
	float horizontal;

	if (direction == true) { horizontal = *dagger_x - x; }
	else { horizontal = x - *dagger_x; }
	return horizontal;
}

float Creature::Dagger_Detection_vertical(float y, float* dagger_y)
{
	bool direction = Player_Detection_simple_horizontal(y, dagger_y);
	float vertical;

	if (direction == true) { vertical = *dagger_y - y; }
	else { vertical = y - *dagger_y; }
	return vertical;
}

float Creature::Axe_Detection_horizontal(float x, float* axe_x)
{
	bool direction = Player_Detection_simple_horizontal(x, axe_x);
	float horizontal;

	if (direction == true) { horizontal = *axe_x - x; }
	else { horizontal = x - *axe_x; }
	return horizontal;
}

float Creature::Axe_Detection_vertical(float y, float* axe_y)
{
	bool direction = Player_Detection_simple_horizontal(y, axe_y);
	float vertical;

	if (direction == true) { vertical = *axe_y - y; }
	else { vertical = y - *axe_y; }
	return vertical;
}

float Creature::Spear_Detection_horizontal(float x, float* spear_x)
{
	bool direction = Player_Detection_simple_horizontal(x, spear_x);
	float horizontal;

	if (direction == true) { horizontal = *spear_x - x; }
	else { horizontal = x - *spear_x; }
	return horizontal;
}

float Creature::Spear_Detection_vertical(float y, float* spear_y)
{
	bool direction = Player_Detection_simple_horizontal(y, spear_y);
	float vertical;

	if (direction == true) { vertical = *spear_y - y; }
	else { vertical = y - *spear_y; }
	return vertical;
}


void Creature::tick() {

}

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
	: m_player(vb, loader->entities["PLAYER"]), momentum(), position(), Player_bow(vb, loader->entities["Player_Bow"]), Player_arrow(vb, loader->entities["Player_Arrow"]), Player_spear(vb, loader->entities["Player_Spear"]), Player_dagger(vb, loader->entities["Player_Dagger"]), Player_Shatter_axe(vb, loader->entities["Player_Shatter_Axe"])
{
	Player_bow.teleport(10000, -10000);
	Player_arrow.teleport(20000, -10000);
	Player_dagger.teleport(30000, -10000);
	Player_spear.teleport(40000, -10000);
	Player_Shatter_axe.teleport(50000, -10000);

	m_player.teleport(1215, -1268);
	position[0] += 1215;
	position[1] += -1268;
	player_transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position[0], -position[1], 0.0f));
}

void Player::walk(float direction, float magnitude)
{
	if (attacking == false) {
		magnitude *= 3;
		float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
		float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

		momentum[0] += round(dx);
		momentum[1] += round(dy);

		Recover_Stamina();
		//walk_noise();
		//walking_sound.Play_sound(walking);
	}
}

void Player::sprint(float direction, float magnitude)
{
	if (attacking == false)
	{
		magnitude *= 3;
		float dx = (float)(cos(direction * 3.14159 / 180)) * magnitude;
		float dy = (float)(sin(direction * 3.14159 / 180)) * magnitude;

		momentum[0] += round(1.5 * dx);
		momentum[1] += round(1.5 * dy);

		//Lose_Stamina();
	}
}

void Player::dodge(float direction, float magnitude)
{
	if (Stamina > 10) {
		dodging = true;
		Stamina -= 10;
	}
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
		if (talk_frames == 0 and talk == true) {
			talk = false;
			talk_frames = 60 * 2;
		}
		talk_frames -= 1;

		if (active_map->collision_at(position[0] + momentum[0], position[1] + momentum[1] - 47))
		{} /* If collision, do nothing */
		else
		{
			if (dodging == true and dodge_momentum > 0) {
				position[0] += momentum[0] * dodge_momentum;
				position[1] += momentum[1] * dodge_momentum;
				m_player.translate(momentum[0] * dodge_momentum, momentum[1] * dodge_momentum);
				dodge_momentum -= 0.0625;
			}
			else if (dodging == true and dodge_momentum == 0) {
				dodge_momentum = 10;
				dodging = false;
			}
			else
			{
				position[0] += momentum[0];
				position[1] += momentum[1];
				m_player.translate(momentum[0], momentum[1]);
			}

			player_transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position[0], -position[1], 0.0f));

		}
	}
	else
	{
		position[0] += momentum[0];
		position[1] += momentum[1];

		if (dodging == true and dodge_frames > 0) {
			m_player.translate(momentum[0] * dodge_momentum, momentum[1] * dodge_momentum);
			dodge_frames -= 1;
			dodge_momentum -= 0.0625;
		}
		else if (dodging == true and dodge_frames == 0) {
			dodge_frames = 60 * 4;
			dodge_momentum = 15;
			dodging = false;
		}
		else
		{
			m_player.translate(momentum[0], momentum[1]);
		}
		

		player_transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position[0], -position[1], 0.0f));
	}

	if (sqrt(momentum[0] * momentum[0] + momentum[1] * momentum[1]) > 0.75)
	{
		float dir = (atan2(momentum[1], momentum[0]));
		if (dir < 0) { dir += 2.0f * 3.14159f; }
		float real_degrees = (dir * 180.0f) / 3.14159f;

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

	/// ///////////////////////////////////// Bow stuff

	if (shoot_bow == true and shwoop_shwoop == true and attacking == false and Stamina >= 25 and pewpew_unlock == true) { //creates the direction for bow and arrow to point
		Player_arrow.teleport(position[0], position[1]);
		dmg_control = 0;
		//std::cout << "Yeet thy arrow" << std::endl;
		float direction = atan2(*curser_y , *curser_x);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		Player_arrow.rotate(-direction + glm::pi<float>()*0.5, Player_arrow.center(), true);
		dx = (float)(cos(direction)) * 3;
		dy = (float)(sin(direction)) * 3;
		shoot_bow = false;
		shwoop_shwoop = false;
		arrow_going = true;

		Player_bow.rotate(-direction + glm::pi<float>() * 0.5, Player_arrow.center(), true);
		Player_bow.teleport(position[0] + dx*20, position[1] + dy*20);

		attacking = true;
		Stamina -= 25;
	}

	if (Stamina < 25 and shoot_bow == true) { shoot_bow = false; }

	if (arrow_going == true) { // keeps track of how long the bow and arrow are on the screeen as well as moves arrow
		bow_lifespan -= 1;
		arrow_lifespan -= 1;
		Player_arrow.translate(dx, dy);
	}

	if (arrow_lifespan == 0) { // when arrow runs out of time it resets arrow
		dx = 0;
		dy = 0;
		arrow_lifespan = 60 * 4;
		Player_arrow.reset();
		Player_arrow.teleport(100000, 100000);
		shwoop_shwoop = true;
		arrow_going = false;
	}
	
	if (bow_lifespan == 0) { // when bow runs out of time, resets bow
		bow_lifespan = 60 * 1;
		Player_bow.reset();
		Player_bow.teleport(100000, 100000);
		attacking = false;
	}
	
	/// //////////////////////////////////// Shifty Shank

	// dagger light attack
	if (light_dagger == true and not dagger_start_L and attacking == false and Stamina >= 10 and shanky_unlock == true) { // creates the direction of dagger and turns it
		attacking = true;
		Player_dagger.teleport(position[0], position[1]);
		direction = atan2(*curser_y, *curser_x);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		//std::cout << direction << std::endl;
		Player_dagger.rotate(-direction + glm::pi<float>() * 0.5, Player_dagger.center(), true);
		dx = (float)(cos(direction)) * 3;
		dy = (float)(sin(direction)) * 3;
		dagger_start_L = 1;

		Stamina -= 10;
		position[0] += dx / 3;
		position[1] += dy / 3;
		m_player.translate(dx/3, dy/3);
	}

	if (Stamina < 10 and light_dagger == true) { light_dagger = false; }

	if (dagger_start_L > 0 and dagger_frame_L < 25) { // moves dagger forward
		Player_dagger.translate(dx, dy);
		dagger_frame_L++;
	}
	else if (dagger_start_L > 0 and dagger_frame_L == 25 and dagger_end_L < 25) { // moves dagger back
		Player_dagger.translate(-dx, -dy);
		dagger_end_L++;
	}
	else if (dagger_start_L > 0 and dagger_frame_L == 25 and dagger_end_L == 25) { // teleports dagger away and resets the attack
		Player_dagger.reset();
		Player_dagger.teleport(100000, 100000);
		dagger_end_L = 0;
		dagger_frame_L = 0;
		dagger_start_L = 0;
		//frames_dagger_L = 0;
		light_dagger = false;
		attacking = false;
	}

	// dagger Heavy attack
	
	if (heavy_dagger == true and not dagger_start_H and attacking == false and Stamina >= 15 and shanky_unlock == true) {  // creates direction for dagger to move in
		attacking = true;
		speacial_move_dagger = rand() % 10;
		Player_dagger.teleport(position[0], position[1]);
		direction = atan2(*curser_y, *curser_x);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		//std::cout << direction << std::endl;
		direction += glm::pi<float>() / 2;
		Player_dagger.rotate(-direction + glm::pi<float>() * 0.5, Player_dagger.center(), true);
		dx = (float)(cos(direction)) * 3;
		dy = (float)(sin(direction)) * 3;
		dagger_start_H = 1;

		Stamina -= 15;
	}

	if (Stamina < 15 and heavy_dagger == true) { heavy_dagger = false; }

	if (dagger_start_H > 0 and dagger_frame_H < 25) { // moves dagger outward
		Player_dagger.translate(dx, dy);
		dagger_frame_H++;
	}
	else if (dagger_start_H > 0 and dagger_frame_H == 25 and got_num_dagger == false) { // saves the intial change of x and y so it can be used later
		got_num_dagger = true;
		dx2 = dx;
		dy2 = dy;
	}
	if (dagger_start_H > 0 and dagger_frame_H == 25 and dagger_mid_H < time_dagger and got_num_dagger == true) { // creates the swinging motion of the dagger
		Player_dagger.reset();
		dx = 0;
		dy = 0;
		direction -= (glm::pi<float>() / time_dagger) * pow(((dagger_mid_H < time_dagger / 2.0f ? dagger_mid_H * 2 : time_dagger - (dagger_mid_H - time_dagger / 2) * 2)) / time_dagger, 3.0f) * 4;
		if (speacial_move_dagger == 9) {
			Player_dagger.rotate((-direction + glm::pi<float>() * 0.5)* pow(((dagger_mid_H < time_dagger / 2.0f ? dagger_mid_H * 2 : time_dagger - (dagger_mid_H - time_dagger / 2) * 2) / time_dagger), 3.0f), Player_dagger.center(), true);
		}
		else { Player_dagger.rotate((-direction + glm::pi<float>() * 0.5), Player_dagger.center(), true); }
		float x = (cos(direction)/rad_dagger) * 1000;
		float y = (sin(direction)/rad_dagger) * 1000;
		Player_dagger.teleport(position[0] + x, position[1] + y);
		dagger_mid_H += 1;
	}
	if (dagger_start_H > 0 and dagger_frame_H == 25 and dagger_mid_H == time_dagger and dagger_end_H < 25){ // usues the saved values of direction from before to go backwards towards player
		Player_dagger.translate(dx2, dy2);
		dagger_end_H++;
	}
	else if (dagger_start_H > 0 and dagger_frame_H == 25 and dagger_mid_H == time_dagger and dagger_end_H == 25) { // resets the function so that player can swing again
		Player_dagger.reset();
		Player_dagger.teleport(100000, 100000);
		dagger_end_H = 0;
		dagger_mid_H = 0;
		dagger_frame_H = 0;
		dagger_start_H = 0;
		got_num_dagger = false;
		heavy_dagger = false;
		attacking = false;
	}

	/// ///////////////////////////// Axe sutff

	// axe light attack
	if (light_axe == true and not axe_start and attacking == false and Stamina >= 20 and huge_axe_unlock == true) { //makes direction for axe to point in
		attacking = true;
		Player_Shatter_axe.teleport(position[0], position[1]);
		direction = atan2(*curser_y, *curser_x);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		Player_Shatter_axe.rotate(-direction + glm::pi<float>() * 0.5, Player_Shatter_axe.center(), true);
		dx = (float)(cos(direction)) * 2;
		dy = (float)(sin(direction)) * 2;
		axe_start = 1;

		Stamina -= 20;
		position[0] += dx / 2;
		position[1] += dy / 2;
		m_player.translate(dx/2, dy/2);
	}

	if (Stamina < 20 and light_axe == true) { light_axe = false; }

	if (axe_start > 0 and axe_frame < 40) { // moves axe outward
		Player_Shatter_axe.translate(dx, dy);
		axe_frame++;
	}
	else if (axe_start > 0 and axe_frame == 40 and axe_end < 40) { //moves axe inward
		Player_Shatter_axe.translate(-dx, -dy);
		axe_end++;
	}
	else if (axe_start > 0 and axe_frame == 40 and axe_end == 40) { //teleports and resets axe so can be used again
		Player_Shatter_axe.reset();
		Player_Shatter_axe.teleport(100000, 100000);
		axe_end = 0;
		axe_frame = 0;
		axe_start = 0;
		frames_axe = 0;
		light_axe = false;
		attacking = false;
	}

	// heavy attack

	if (heavy_axe == true and not axe_start_H and attacking == false and Stamina >= 70 and huge_axe_unlock == true) {  // creates direction for axe to move in
		attacking = true;
		speacial_move_axe = rand() % 10;
		Player_Shatter_axe.teleport(position[0], position[1]);
		direction = atan2(*curser_y, *curser_x);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		//std::cout << direction << std::endl;
		direction += glm::pi<float>() / 2;
		Player_Shatter_axe.rotate(-direction + glm::pi<float>() * 0.5, Player_Shatter_axe.center(), true);
		dx = (float)(cos(direction)) * 3;
		dy = (float)(sin(direction)) * 3;
		axe_start_H = 1;

		Stamina -= 70;
	}

	if (Stamina < 70 and heavy_axe == true) { heavy_axe = false; }

	if (axe_start_H > 0 and axe_frame_H < 25) { // moves axe outward
		Player_Shatter_axe.translate(dx, dy);
		axe_frame_H++;
	}
	else if (axe_start_H > 0 and axe_frame_H == 25 and got_num_axe == false) { // saves the intial change of x and y so it can be used later
		got_num_axe = true;
		dx2 = dx;
		dy2 = dy;
	}
	if (axe_start_H > 0 and axe_frame_H == 25 and axe_mid_H < time_axe and got_num_axe == true) { // creates the swinging motion of the axe
		Player_Shatter_axe.reset();
		dx = 0;
		dy = 0;
		direction -= (glm::pi<float>() * 2 / time_axe) * pow(((axe_mid_H < time_axe / 2.0f ? axe_mid_H * 2 : time_axe - (axe_mid_H - time_axe / 2) * 2)) / time_axe, 3.0f) * 4;
		if (speacial_move_axe == 9) {
			Player_Shatter_axe.rotate((-direction + glm::pi<float>() * 0.5) * pow(((axe_mid_H < time_axe / 2.0f ? axe_mid_H * 2 : time_axe - (axe_mid_H - time_axe / 2) * 2) / time_axe), 3.0f), Player_Shatter_axe.center(), true);
		}
		else { Player_Shatter_axe.rotate((-direction + glm::pi<float>() * 0.5), Player_Shatter_axe.center(), true); }
		float x = (cos(direction) / rad_dagger) * 1000;
		float y = (sin(direction) / rad_dagger) * 1000;
		Player_Shatter_axe.teleport(position[0] + x, position[1] + y);
		axe_mid_H += 1;
	}
	if (axe_start_H > 0 and axe_frame_H == 25 and axe_mid_H == time_axe and axe_end_H < 25) { // usues the saved values of direction from before to go backwards towards player
		Player_Shatter_axe.translate(-dx2, -dy2);
		axe_end_H++;
	}
	else if (axe_start_H > 0 and axe_frame_H == 25 and axe_mid_H == time_axe and axe_end_H == 25) { // resets the function so that player can swing again
		Player_Shatter_axe.reset();
		Player_Shatter_axe.teleport(100000, 100000);
		axe_end_H = 0;
		axe_mid_H = 0;
		axe_frame_H = 0;
		axe_start_H = 0;
		got_num_axe = false;
		heavy_axe = false;
		attacking = false;
	}


	/// //////////////////////////////// spear stuff

	// spear light attack
	if (light_spear == true and not spear_start_L and attacking == false and Stamina >= 20 and bigger_shank_unlock == true) { // creates the direction of dagger and turns it
		attacking = true;
		Player_spear.teleport(position[0], position[1]);
		direction = atan2(*curser_y, *curser_x);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		//std::cout << direction << std::endl;
		Player_spear.rotate(-direction + glm::pi<float>() * 0.5, Player_spear.center(), true);
		dx = (float)(cos(direction)) * 2.5;
		dy = (float)(sin(direction)) * 2.5;
		spear_start_L = 1;

		Stamina -= 20;
		position[0] += dx / 2.5;
		position[1] += dy / 2.5;
		m_player.translate(dx/2.5, dy/2.5);
	}

	if (Stamina < 20 and light_spear == true ) { light_spear = false; }

	if (spear_start_L > 0 and spear_frame_L < 40) { // moves dagger forward
		Player_spear.translate(dx, dy);
		spear_frame_L++;
	}
	else if (spear_start_L > 0 and spear_frame_L == 40 and spear_end_L < 40) { // moves dagger back
		Player_spear.translate(-dx, -dy);
		spear_end_L++;
	}
	else if (spear_start_L > 0 and spear_frame_L == 40 and spear_end_L == 40) { // teleports dagger away and resets the attack
		Player_spear.reset();
		Player_spear.teleport(100000, 100000);
		spear_end_L = 0;
		spear_frame_L = 0;
		spear_start_L = 0;
		//frames_dagger_L = 0;
		light_spear = false;
		attacking = false;
	}

	// spear heavy (the same as light but farther)
	if (heavy_spear == true and not spear_start_H and attacking == false and Stamina >= 30 and bigger_shank_unlock == true) { // creates the direction of dagger and turns it
		attacking = true;
		Player_spear.teleport(position[0], position[1]);
		direction = atan2(*curser_y, *curser_x);
		if (direction < 0) { direction += 2.0f * 3.14159f; }
		//std::cout << direction << std::endl;
		Player_spear.rotate(-direction + glm::pi<float>() * 0.5, Player_spear.center(), true);
		dx = (float)(cos(direction)) * 3;
		dy = (float)(sin(direction)) * 3;
		spear_start_H = 1;

		Stamina -= 30;
		position[0] += dx/3;
		position[1] += dy/3;
		m_player.translate(dx / 3, dy / 3);
	}

	if (Stamina < 30 and heavy_spear == true) { heavy_spear = false; }

	if (spear_start_H > 0 and spear_frame_H < 80) { // moves dagger forward
		Player_spear.translate(dx, dy);
		spear_frame_H++;
	}
	else if (spear_start_H > 0 and spear_frame_H == 80 and spear_end_H < 80) { // moves dagger back
		Player_spear.translate(-dx, -dy);
		spear_end_H++;
	}
	else if (spear_start_H > 0 and spear_frame_H == 80 and spear_end_H == 80) { // teleports dagger away and resets the attack
		Player_spear.reset();
		Player_spear.teleport(100000, 100000);
		spear_end_H = 0;
		spear_frame_H = 0;
		spear_start_H = 0;
		//frames_dagger_L = 0;
		heavy_spear = false;
		attacking = false;
	}

	// if you die (at this point im just gonna teleport them for now)

	if (Health <= 0 and check_point_location == 0) {
		m_player.teleport(306, -405);
		position[0] = 306;
		position[1] = -405;
		Health = 100;
		Stamina = 100;
		Potion = 4;
	}
	else if (Health <= 0 and check_point_location == 1) {
		m_player.teleport(-2780, 5950);
		position[0] = -2780;
		position[1] = 5950;
		Health = 100;
		Stamina = 100;
		Potion = 4;
	}
	else if (Health <= 0 and check_point_location == 2) {
		m_player.teleport(-7111, 9407);
		position[0] = -7111;
		position[1] = 9407;
		Health = 100;
		Stamina = 100;
		Potion = 4;
	}
	else if (Health <= 0 and check_point_location == 3) {
		m_player.teleport(-10000, -7000);
		position[0] = -10000;
		position[1] = -7000;
		Health = 100;
		Stamina = 100;
		Potion = 4;
	}

	Player_spear.tick();
	Player_Shatter_axe.tick();
	Player_arrow.tick();
	Player_dagger.tick();
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

int Player::get_weapon_type()
{
	if (heavy_dagger == false and light_dagger == false and light_axe == false and heavy_axe == false and heavy_spear == false) { weapon_type = 0; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_type = 1; }
	else if (light_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_type = 2; }
	else if (heavy_dagger == false and shoot_bow == false and light_dagger == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_type = 3; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_spear == false) { weapon_type = 4; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and heavy_spear == false and heavy_axe == false) { weapon_type = 5; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_axe == false) { weapon_type = 6; }
	return weapon_type;
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

float Player::weapon_x()
{
	float weapon_x;
	if (heavy_dagger == false and light_dagger == false and light_axe == false and heavy_axe == false and heavy_spear == false) { weapon_x = Player_arrow.center().x; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_x = Player_dagger.center().x;; }
	else if (light_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_x = Player_dagger.center().x; }
	else if (heavy_dagger == false and shoot_bow == false and light_dagger == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_x = Player_Shatter_axe.center().x; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_spear == false) { weapon_x = Player_Shatter_axe.center().x; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and heavy_spear == false and heavy_axe == false) { weapon_x = Player_spear.center().x; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_axe == false) { weapon_x = Player_spear.center().x; }
	return weapon_x;
}

float Player::weapon_y()
{
	float weapon_y;
	if (heavy_dagger == false and light_dagger == false and light_axe == false and heavy_axe == false and heavy_spear == false) { weapon_y = Player_arrow.center().y; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_y = Player_dagger.center().y; }
	else if (light_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_y = Player_dagger.center().y; }
	else if (heavy_dagger == false and shoot_bow == false and light_dagger == false and heavy_axe == false and light_spear == false and heavy_spear == false) { weapon_y = Player_Shatter_axe.center().y; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_spear == false) { weapon_y = Player_Shatter_axe.center().y; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and heavy_spear == false and heavy_axe == false) { weapon_y = Player_spear.center().y; }
	else if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_axe == false) { weapon_y = Player_spear.center().y; }
	return weapon_y;
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

bool Player::am_attacking()
{
	return attacking;
}

void Player::Lose_Stamina()
{
	Stamina -= 0.1;
}

bool Player::shwoop_doop()
{
	return shwoop_shwoop;
}

void Player::Shoot_bow()
{
	if (heavy_dagger == false and light_dagger == false and light_axe == false and heavy_axe == false and heavy_spear == false) { shoot_bow = true; }
}

void Player::Dagger_light()
{
	if(heavy_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false){ light_dagger = true; }
}

void Player::Dagger_Heavy()
{
	if (light_dagger == false and shoot_bow == false and light_axe == false and heavy_axe == false and light_spear == false and heavy_spear == false) { heavy_dagger = true; }
}

void Player::Axe_Light()
{
	if (heavy_dagger == false and shoot_bow == false and light_dagger == false and heavy_axe == false and light_spear == false and heavy_spear == false) { light_axe = true; }
	//std::cout << "I WAS CLICKED BRO JUST SHOOT LIKE GOD UR A PRICK" << std::endl;
}

void Player::Axe_Heavy()
{
	if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_spear == false) { heavy_axe = true; }
}

void Player::Spear_Light()
{
	if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and heavy_spear == false and heavy_axe == false) { light_spear = true; }
}

void Player::Spear_Heavy()
{
	if (heavy_dagger == false and shoot_bow == false and light_axe == false and light_dagger == false and light_spear == false and heavy_axe == false) { heavy_spear = true; }
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

bool* Player::speaking()
{
	if (talk == false) {
		talk = true;
	}
	return &talk;
}

bool* Player::bow_unlock()
{
	return &pewpew_unlock;
}

bool* Player::shifty_shank_unlock()
{
	return &shanky_unlock;
}

bool* Player::big_axe_unlock()
{
	return &huge_axe_unlock;
}

bool* Player::bigger_shity_shank_unlock()
{
	return &bigger_shank_unlock;
}

int* Player::check_points()
{
	return &check_point_location;
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

float* Red_Slime::give_health()
{
	return &Health;
}

float* Red_Slime::give_x()
{
	return &position[0];
}

float* Red_Slime::give_y()
{
	return &position[1];
}

void Red_Slime::teleport(float x, float y)
{
	position[0] = x;
	position[1] = y;
	Red_slime.teleport(x, y);
}

void Red_Slime::tick()
{
	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);
	entity_return tick_state = Red_slime.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 500) {
		if (tick_state.anim_state == animation_state::advanced_frame)
		{
			frame++;
			if (frame > 3)
				frame = 0;
		}

		if (frame == 2)
		{
			if (frame == 2)
			{
				float magnitude = 2;
				if (bops < 3) {
					if (horizontal == true) { momentum[0] += magnitude; }
					else if (horizontal == false) { momentum[0] += -magnitude; }

					if (vertical == false) { momentum[1] += -magnitude; }
					else if (vertical == true) { momentum[1] += magnitude; }

					dmg_control = 0;
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
				if (dmg_control < 10) {
					*player_health -= 1;
					dmg_control++;
				}
				bops += 1;
			}
		}
	}
	if (Health <= 0) {
		position[0] = 100000;
		position[1] = 100000;
		Red_slime.teleport(100000, 100000);
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

float* Enemy_Ghost::give_health()
{
	return &Health;
}

float* Enemy_Ghost::give_x()
{
	return &position[0];
}

float* Enemy_Ghost::give_y()
{
	return &position[0];
}

void Enemy_Ghost::teleport(float x, float y)
{
	position[0] = x;
	position[1] = y;
	Enemy_ghost.teleport(x, y);
}

void Enemy_Ghost::tick()
{
	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);
	entity_return tick_state = Enemy_ghost.tick();
	entity_return tick_bullet = Wizard_pink_bullet.tick();
	bool pewpew = false;

	if (tick_state.anim_state == animation_state::ended) {
		Enemy_ghost.set_animation(0);
	}

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 500) {
		if (frames++ == 60 * 12) {
			int random = 1 + (rand() % 2);

			if (random == 1 and teleporting > 0) {
				//std::cout << "not moving" << std::endl;
				pewpew = true;
			}
			else {
				Enemy_ghost.set_animation(1);
				//Ghost_move_sound.Play_sound(Ghost_move);
				teleporting += 1;
				pewpew = false;

			}
			frames = 0;
		}

		if (teleporting > 1 and tick_state.anim_state == animation_state::ended) {
			Enemy_ghost.set_animation(0);
			//Ghost_move_sound.Stop_sound(Ghost_move);
			teleporting = 0;
		}

		if (teleporting > 0) {
			if (tick_state.anim_state == animation_state::ended) {
				Enemy_ghost.set_animation(2);
				position[0] = *player_position_x + (rand() % 1000) - 500;
				position[1] = *player_position_y + (rand() % 1000) - 500;
				//std::cout << "moving" << std::endl;
				Enemy_ghost.teleport(position[0], position[1]);
				teleporting++;
			}
		}

		if (frames_magic++ == 60 * 6 and pewpew == false) {
			bullet_lifespan = 60 * 4;
			Wizard_pink_bullet.set_animation(0);
			Wizard_pink_bullet.teleport(position[0], position[1]);
			dmg_control = 0;
			//std::cout << "Yeet thy bullet" << std::endl;
			frames_magic = 0;
			float direction = atan2(*player_position_y - position[1], *player_position_x - position[0]);
			if (direction < 0) { direction += 2.0f * 3.14159f; }
			//std::cout << direction << std::endl;
			dx = (float)(cos(direction)) * 3;
			dy = (float)(sin(direction)) * 3;
		}
	}
	bullet_lifespan -= 1;
	Wizard_pink_bullet.translate(dx, dy);

	if (bullet_lifespan == 0) {
		dx = 0;
		dy = 0;
		Wizard_pink_bullet.set_animation(1);
	}

	if (abs(Wizard_pink_bullet.center().x - *player_position_x) < 20 and abs(Wizard_pink_bullet.center().y - *player_position_y) < 20) {
		dx = 0;
		dy = 0;
		Wizard_pink_bullet.set_animation(1);
		if (dmg_control < 10) {
			*player_health -= 1;
			dmg_control++;
		}
		
		//std::cout << "IVE BEEN HIT" << std::endl;
	}
	if (tick_bullet.anim_state == animation_state::ended)
	{
		Wizard_pink_bullet.set_animation(0);
		Wizard_pink_bullet.teleport(100000, 10000);
	}

	if (Health <= 0) {
		position[0] = 100000;
		position[1] = 100000;
		Enemy_ghost.teleport(100000, 100000);
	}
}

Garfield::Garfield(VertexBuffer* vb)
	: garfield(vb, loader->entities["Garfield"]), momentum(), position()
{
	garfield.set_animation(0);
	garfield.teleport(306, -465);
	position[0] = 306;
	position[1] = -465;
}
void Garfield::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}
void Garfield::Get_talk(bool* spoken)
{
	talk = spoken;
}
void Garfield::Get_point(int* current_player_checkpoint)
{
	player_check_point = current_player_checkpoint;
}
void Garfield::tick()
{
	garfield.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		*player_check_point = checkpoint_num;
		text->create_text("Meow", position[0] + 40, position[1], 60 * 3);
	}
}

Bush_Boi::Bush_Boi(VertexBuffer* vb)
	: Bush_boi(vb, loader->entities["Bush_Boi"])
{
	Bush_boi.set_animation(0);
	Bush_boi.teleport(1000, 0);
	position[0] += 1000;
	position[1] += 0;
}

void Bush_Boi::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

float* Bush_Boi::give_health()
{
	return &Health;
}

float* Bush_Boi::give_x()
{
	return &position[0];
}

float* Bush_Boi::give_y()
{
	return &position[1];
}

void Bush_Boi::teleport(float x, float y)
{
	position[0] = x;
	position[1] = y;
	Bush_boi.teleport(x, y);
}

void Bush_Boi::tick()
{
	static int bush_frame;
	float magnitude = 2;
	entity_return tick_state = Bush_boi.tick();
	

	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);

	float dir = (atan2(*player_position_y - position[1], *player_position_x - position[0]) - atan2(position[1] - position[1], position[0] - position[0]));
	if (dir < 0) { dir += 2.0f * 3.14159f; }
	float real_degrees = (dir * 180.0f) / 3.14159f;

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100000) {
		
		if (bops == 0 and change_animation == true and side_change == true) { 
			Bush_boi.set_animation(4);
			side_change = false;
		}
		//else { Bush_boi.set_animation(0); }

		if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 200 ) {

			if (Bush_boi.get_animation() != 0 and change_animation == true) {
				Bush_boi.set_animation(0);
				change_animation = false;
				
				//Bush_boi.advance_frame();
			}
			if (tick_state.anim_state == animation_state::advanced_frame)
			{
				frame++;
				if (frame >= 3)
					frame = 0;
			}

			if (frame == 2)
			{

				if (bops < 3) {
					if (horizontal == true) { momentum[0] += magnitude; }
					else if (horizontal == false) { momentum[0] += -magnitude; }

					if (vertical == false) { momentum[1] += -magnitude; }
					else if (vertical == true) { momentum[1] += magnitude; }
					dmg_control = 0;
				}

				position[0] += round(momentum[0]);
				position[1] += round(momentum[1]);

				if (real_degrees > 108 && real_degrees < 255) { Bush_boi.set_animation(3); side_change = true; }
				else if (real_degrees > 260 && real_degrees < 280) { Bush_boi.set_animation(0); side_change = true; }
				else if (momentum[0] < 0) { Bush_boi.set_animation(2); side_change = true; }
				else if (momentum[0] > 0) {Bush_boi.set_animation(1); side_change = true; }

				Bush_boi.translate(round(momentum[0]), round(momentum[1]));

				momentum[0] = 0;
				momentum[1] = 0;
			}

			if (abs(position[0] - *player_position_x) < 40 and abs(position[1] - *player_position_y) < 40) {
				if (dmg_control < 5) {
					*player_health -= 1;
					dmg_control++;
				}
				bops += 1;
				change_animation = true;
			}
		}
		if (bops > 3) {
			magnitude = 3;
			if (horizontal == true) { momentum[0] += -magnitude; }
			else if (horizontal == false) { momentum[0] += magnitude; }

			if (vertical == false) { momentum[1] += magnitude; }
			else if (vertical == true) { momentum[1] += -magnitude; }
			bops_frame += 1;

			if (real_degrees < 75 || real_degrees > 285) { Bush_boi.set_animation(3); side_change = true; }
			else if (real_degrees > 80 && real_degrees < 100) { Bush_boi.set_animation(1); side_change = true; }
			else if (real_degrees > 108 && real_degrees < 255) { Bush_boi.set_animation(2); side_change = true; }
			else if (real_degrees > 260 && real_degrees < 280) { Bush_boi.set_animation(0); side_change = true; }

		}
		if (bops_frame > 60 * 2) {
			bops_frame = 0;
			bops = 0;
			Bush_boi.set_animation(4);
		}

		position[0] += round(momentum[0]);
		position[1] += round(momentum[1]);

		Bush_boi.translate(round(momentum[0]), round(momentum[1]));

		momentum[0] = 0;
		momentum[1] = 0;

	}
	if (Health <= 0) {
		position[0] = 100000;
		position[1] = 100000;
		Bush_boi.teleport(100000, 100000);
	}
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

float* Chompy_Slime::give_health()
{
	return &Health;
}

float* Chompy_Slime::give_x()
{
	return &position[0];
}

float* Chompy_Slime::give_y()
{
	return &position[1];
}

void Chompy_Slime::teleport(float x, float y)
{
	position[0] = x;
	position[1] = y;
	Chompy_slime.teleport(x, y);
}

void Chompy_Slime::tick()
{
	entity_return tick_state = Chompy_slime.tick();

	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 500) {
		
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
				dmg_control = 0;

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
			if (dmg_control < 15) {
				*player_health -= 2;
				dmg_control++;
			}

			bops += 1;
		}
	}
	if (Health <= 0) {
		position[0] = 100000;
		position[1] = 100000;
		Chompy_slime.teleport(100000, 100000);
	}
}

Sussy_Vase::Sussy_Vase(VertexBuffer* vb)
	: Sussy_vase(vb, loader->entities["Sussy_Vase"]), momentum(), position(), Wizard_pink_bullet(vb, loader->entities["Wizard_Pink_bullet"])
{
	Sussy_vase.set_animation(0);
	position[0] += 1500;
	position[1] += 100;
	Sussy_vase.teleport(1500, 100);
	Wizard_pink_bullet.teleport(10000000, 10000000);
	Wizard_pink_bullet.set_animation(0);
}


void Sussy_Vase::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

float* Sussy_Vase::give_health()
{
	return &Health;
}

float* Sussy_Vase::give_x()
{
	return &position[0];
}

float* Sussy_Vase::give_y()
{
	return &position[1];
}

void Sussy_Vase::teleport(float x, float y)
{
	position[0] = x;
	position[1] = y;
	Sussy_vase.teleport(x, y);
}

void Sussy_Vase::tick()
{
	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);
	entity_return tick_state = Sussy_vase.tick();
	entity_return tick_bullet = Wizard_pink_bullet.tick();
	bool pewpew = false;

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) >= 500) {
		Sussy_vase.set_animation(0);
	}

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 500) {
			
		Sussy_vase.set_animation(1);
			
		if (frames_magic++ == 60 * 6 and pewpew == false) {
			bullet_lifespan = 60 * 4;
			Wizard_pink_bullet.set_animation(0);
			Wizard_pink_bullet.teleport(position[0], position[1]);
			dmg_control = 0;
			//std::cout << "Yeet thy bullet" << std::endl;
			frames_magic = 0;
			float direction = atan2(*player_position_y - position[1], *player_position_x - position[0]) - atan2(position[1] - position[1], position[0] - position[0]);
			if (direction < 0) { direction += 2.0f * 3.14159f; }
			//std::cout << direction << std::endl;
			dx = (float)(cos(direction)) * 3;
			dy = (float)(sin(direction)) * 3;
		}
	}
	bullet_lifespan -= 1;
	Wizard_pink_bullet.translate(dx, dy);

	if (bullet_lifespan == 0) {
		dx = 0;
		dy = 0;
		Wizard_pink_bullet.set_animation(1);
	}

	if (abs(Wizard_pink_bullet.center().x - *player_position_x) < 20 and abs(Wizard_pink_bullet.center().y - *player_position_y) < 20) {
		dx = 0;
		dy = 0;
		Wizard_pink_bullet.set_animation(1);
		if (dmg_control < 10) {
			*player_health -= 1;
			dmg_control++;
		}

		//std::cout << "IVE BEEN HIT" << std::endl;
	}
	if (tick_bullet.anim_state == animation_state::ended)
	{
		Wizard_pink_bullet.set_animation(0);
		Wizard_pink_bullet.teleport(100000, 10000);
	}
	if (Health <= 0) {
		position[0] = 100000;
		position[1] = 100000;
		Sussy_vase.teleport(100000, 100000);
	}
}

Cow::Cow(VertexBuffer* vb)
	: cow(vb, loader->entities["Cow"]), momentum(), position()
{
	cow.set_animation(0);
	cow.teleport(-2780, 6006);
	position[0] = -2810;
	position[1] = -6006;
}

void Cow::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Cow::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Cow::Get_point(int* current_player_checkpoint)
{
	player_check_point = current_player_checkpoint;
}

void Cow::tick()
{
	cow.tick();

	float num = sqrt(pow(abs(*player_position_x - position[0]), 2) + pow(abs(*player_position_x - position[0]), 2));
	
	if ( num < 100 and *talk == true) {
		*talk = false;
		*player_check_point = checkpoint_num;
		text->create_text("Meow", position[0] + 40, position[1], 60 * 3);
	}
}

Perry::Perry(VertexBuffer* vb)
	: perry(vb, loader->entities["Perry"]), momentum(), position()
{
	perry.set_animation(0);
	perry.teleport(-7111, 9407);
	position[0] = -7111;
	position[1] = 9407;
}

void Perry::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Perry::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Perry::Get_point(int* current_player_checkpoint)
{
	player_check_point = current_player_checkpoint;
}

void Perry::tick()
{
	perry.tick();
	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		*player_check_point = checkpoint_num;
		text->create_text("Meow", position[0] + 40, position[1], 60 * 3);
	}

}

Edgelord::Edgelord(VertexBuffer* vb)
	: edgelord(vb, loader->entities["Edgelord"]), momentum(), position()
{
	edgelord.set_animation(0);
	edgelord.teleport(-10000, -70000);
	position[0] = -10000;
	position[1] = -70000;
}

void Edgelord::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Edgelord::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Edgelord::Get_point(int* current_player_checkpoint)
{
	player_check_point = current_player_checkpoint;
}

void Edgelord::tick()
{
	edgelord.tick();
	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		*player_check_point = checkpoint_num;
		//position[0] = *player_position_x + (rand() % 1000) - 500;
		//position[1] = *player_position_y + (rand() % 1000) - 500;
		//edgelord.teleport(position[0], position[1]);
	}
}

Clair_Of_Cavern::Clair_Of_Cavern(VertexBuffer* vb)
	: clair_of_cavern(vb, loader->entities["Clair_Of_Cavern"]), momentum(), position()
{
	//clair_of_cavern.set_animation(0);
	clair_of_cavern.teleport(1711, 2995);
	position[0] = 1711;
	position[1] = 2995;
	
}

void Clair_Of_Cavern::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Clair_Of_Cavern::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Clair_Of_Cavern::tick()
{
	clair_of_cavern.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		spoken += 1;

		if (spoken == 1) {
			text->create_text("You look friendly, can I ask a favour?", position[0] - 250, position[1] + 70, 60 * 6);
		}
		else if (spoken == 2) {
			text->create_text("I�m looking for my brother, Destus.\nHe looks like me, but he wears a different hat.\nHe went off on his own and I haven�t seen him since.", position[0] - 300, position[1] + 100, 60 * 12);
		}
		else if (spoken == 3) {
			text->create_text("I�m just worried about him,\nhe always gets in over his head.", position[0] - 200, position[1] + 80, 60 * 12);
		}
		else if (spoken == 4) {
			text->create_text("If you find him, please help him.", position[0] - 250, position[1] + 70, 60 * 12);
		}
		else if (spoken == 5) {
			text->create_text("I miss him.", position[0] - 80 , position[1] + 70, 60 * 12);
		}
		else if (spoken >= 6) {
			text->create_text("...", position[0] - 200, position[1] + 70, 60 * 12);
		}
	}
}

Destus_Of_Cavern::Destus_Of_Cavern(VertexBuffer* vb)
	: destus_of_cavern(vb, loader->entities["Destus_Of_Cavern"]), momentum(), position()
{
	destus_of_cavern.teleport(349, 9137);
	position[0] = 349;
	position[1] = 9137;

}

void Destus_Of_Cavern::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Destus_Of_Cavern::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Destus_Of_Cavern::tick()
{
	destus_of_cavern.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		spoken += 1;

		if (spoken == 1) {
			text->create_text("You... You there.\nI don�t have much time left,\nso listen. My name is Destus of Caven.\nI am an expert in hunting the\nabominable, but whatever is\nin there.. It made a joke of me.", position[0] + 25, position[1] + 150, 60 * 12);
		}
		else if (spoken == 2) {
			text->create_text("That creature,\nit�s worse than any of the\nothers...", position[0] + 25, position[1] + 100, 60 * 12);
		}
		else if (spoken == 3) {
			text->create_text("It�s powerful...\nIf you value your life, run", position[0] + 25, position[1] + 80, 60 * 12);
		}
		else if (spoken == 4) {
			text->create_text("... If you see my sister, Claire...\nTell her I�m sorry for running off.\nTell her to get out and that\nI�ll always watch over her.", position[0] + 25, position[1] + 150, 60 * 12);
		}
		else if (spoken == 5) {
			text->create_text("Good luck...", position[0] + 25, position[1] + 80, 60 * 12);
		}
		else if (spoken >= 6) {
			text->create_text("...", position[0] + 25, position[1] + 80, 60 * 12);
		}

	}
}

Del_Ibra_of_Hillsby::Del_Ibra_of_Hillsby(VertexBuffer* vb)
	: del_ibra_of_hillsby(vb, loader->entities["Del_Ibra_of_Hillsby"]), momentum(), position()
{
	//del_ibra_of_hillsby.set_animation(0);
	del_ibra_of_hillsby.teleport(-6869.5, 4256.5);
	position[0] = -6869.5;
	position[1] = 4256.5;
}

void Del_Ibra_of_Hillsby::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Del_Ibra_of_Hillsby::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Del_Ibra_of_Hillsby::Get_pewpew_lock(bool* lock)
{
	unlock = lock;
}


void Del_Ibra_of_Hillsby::tick()
{
	del_ibra_of_hillsby.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		spoken += 1;

		if (spoken == 1) {
			text->create_text("Hello little rabbit,\nI am Del Ibra of Hillsby.\nBy the red of your cloak,\nyou must be from Minlet. ", position[0] - 350, position[1] + 150, 60 * 12);
		}
		else if (spoken == 2) {
			text->create_text("If you�re here,\nMinlet must have fallen.\n I see the scourge has\nalready reached the south.", position[0] - 350, position[1] + 150, 60 * 12);
		}
		else if (spoken == 3) {
			text->create_text("There�s not much you can do \nwith those little weapons,\ntake this.", position[0] - 350 , position[1] + 150 , 60 * 12);
			*unlock = true;
		}
		else if (spoken == 4) {
			text->create_text("Don�t you worry about me,\nthat�s just a spare.\nNow go on, show them what\na rabbit can do.", position[0] - 350, position[1] + 150, 60 * 12);
		}
		else if (spoken == 5) {
			text->create_text("The dungeon entry must\nbe around here somewhere.", position[0] - 350, position[1] + 100, 60 * 12);
		}
		else if (spoken == 6) {
			text->create_text("I appreciate the company.\nAll alone,the snow can be\nquite melancholy and cold.\nIn the presence of others,\nit isn�t so bad.", position[0] - 350, position[1] + 150, 60 * 12);
		}
		else if (spoken >= 7) {
			text->create_text("I miss Hillsby.", position[0] - 250, position[1] + 70, 60 * 12);
		}

	}
}

Eloah_of_Minlet::Eloah_of_Minlet(VertexBuffer* vb)
	: eloah_of_minlet(vb, loader->entities["Eloah_of_Minlet"]), momentum(), position()
{
	//eloah_of_minlet.set_animation(0);
	eloah_of_minlet.teleport(213, -568);
	position[0] = 213;
	position[1] = -568;
}

void Eloah_of_Minlet::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Eloah_of_Minlet::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Eloah_of_Minlet::Get_shank_unlock(bool* lock)
{
	unlock = lock;
}

void Eloah_of_Minlet::tick()
{
	eloah_of_minlet.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		 spoken += 1;

		 if (spoken == 1) {
		    	text->create_text("You, rabbit. Come here!", position[0] - 160, position[1] + 60, 60 * 3);
		 }
		 else if (spoken == 2) {
			 text->create_text("You�re brave to have entered this place,\nmost wouldn�t dare. I�m grateful for the backup,\nbut you seem to have forgotten a weapon. Take this.", position[0] - 320, position[1] + 100, 60 * 12);
			 *unlock = true;
		 }
		 else if (spoken == 3) {
			 text->create_text("You should be able to protect yourself with that.\n You go on ahead, little one. I�ll rest here.", position[0] - 320, position[1] + 75, 60 * 12);
		 }
		 else if (spoken == 4) {
			 text->create_text("I appreciate the concern,\nbut you should get going.", position[0] - 200, position[1] + 70, 60 * 12);
		 }
		 else if (spoken == 5) {
			 text->create_text("I said go on, I need to rest. You carry on, \nfind the entrance to the dungeon proper.", position[0] - 320, position[1] + 70, 60 * 12);
		 }
		 else if (spoken == 6) {
			 text->create_text("Move on, little one!", position[0] - 160, position[1] + 60, 60 * 12);
		 }
		 else if (spoken == 7) {
			 text->create_text("Goodluck...", position[0] - 70, position[1] + 60, 60 * 12);
		 }
		 else if (spoken >= 8) {
			 text->create_text("...", position[0] - 160, position[1] + 60, 60 * 12);
		 }
	}
}

Felix_of_Festria::Felix_of_Festria(VertexBuffer* vb)
	: felix_of_festria(vb, loader->entities["Felix_of_Festria"]), momentum(), position()
{
	//felix_of_festria.set_animation(0);
	felix_of_festria.teleport(-4152.67, 3495.67);
	position[0] = -4152.67;
	position[1] = 3495.67;
}

void Felix_of_Festria::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Felix_of_Festria::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Felix_of_Festria::Get_big_axe_unlock(bool* lock)
{
	unlock = lock;
}

void Felix_of_Festria::tick()
{
	felix_of_festria.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		spoken += 1;

		if (spoken == 1) {
			text->create_text("...", position[0], position[1], 60 * 12);
		}
		else if (spoken == 2) {
			text->create_text("Oh, apologies.\nI was lost in thought.\nMy name is Felix,\nI come from Festria.", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 3) {
			text->create_text("You don�t talk much,\neh? No matter, you\nonly need to listen.", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 4) {
			text->create_text("It�s quite strange,\nI came here with many, and\nnow I�m the only Festrian\nleft,yet the monsters\ncontinue to grow in\n\number.", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 5) {
			text->create_text("I suspect the monsters\nare made from those who\ndie in this land.\nPerhaps that�s how the\ndungeon grows its\nmonsters, from the souls\nof the fallen.", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 6) {
			text->create_text("Ah, forget it.\nYou shouldn�t think too much\nof these things, just take\nthis and go thin their\nnumbers.", position[0] - 800, position[1], 60 * 12);
			*unlock = true;
		}
		else if (spoken == 7) {
			text->create_text("Get on with it,\nleave me to my\nthoughts.", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 8) {
			text->create_text("...", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 9) {
			text->create_text("...", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 10) {
			text->create_text("For someone who doesn�t talk,\nyou sure expect others to!\nShoo! Shoo!", position[0] - 800, position[1], 60 * 12);
		}
		else if (spoken == 11) {
			text->create_text("...", position[0] - 800, position[1], 60 * 12);
		}
	}
}

Maban_of_Undermount::Maban_of_Undermount(VertexBuffer* vb)
	: maban_of_undermount(vb, loader->entities["Maban_of_Undermount"]), momentum(), position()
{
	//maban_of_undermount.set_animation(0);
	maban_of_undermount.teleport(-1966, 8428);
	position[0] = -1966;
	position[1] = 8428;
}

void Maban_of_Undermount::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

void Maban_of_Undermount::Get_talk(bool* spoken)
{
	talk = spoken;
}

void Maban_of_Undermount::Get_Bigger_Shock(bool* lock)
{
	unlock = lock;
}

void Maban_of_Undermount::tick()
{
	maban_of_undermount.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 100 and *talk == true) {
		*talk = false;
		spoken += 1;

		if (spoken == 1) {
			text->create_text("How�d you find your\nway down here, rabbit?", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 2) {
			text->create_text("The silent type...\nI like that.", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 3) {
			text->create_text("You�re really growing on me, kid.\nTake this, as a token of\nmy respect.", position[0] + 32, position[1] + 70, 60 * 12);
			*unlock = true;
		}
		else if (spoken == 4) {
			text->create_text("Now go and hunt yourself\na biggun, there�s \nsomething powerful lurking\n down here and I\nwant its teeth.", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 5) {
			text->create_text("What, you have a problem\nwith teeth? Everyone�s got them,\n don�t look so concerned.", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 6) {
			text->create_text("... It�s not like I do anything\nweird with them!\nI just keep the teeth\n as a trophy, the bigger\nthe tooth, the bigger\nthe prey. Like a\nbadge of honour.", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 7) {
			text->create_text("Whatever, go on.\nYour silence is starting\nto freak me out.", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 8) {
			text->create_text("... I know you haven�t said\nanything but the way you\nkeep looking at me is really\nmessing with my head.", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 9) {
			text->create_text("Was it the teeth thing?\nIs that why you keep\ngiving me that look", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken == 10) {
			text->create_text("Okay fine,\ntwo can play the silent game.", position[0] + 32, position[1] + 70, 60 * 12);
		}
		else if (spoken >= 11) {
			text->create_text("...", position[0] + 32, position[1] + 70, 60 * 12);
		}
	}
}

Campfire::Campfire(VertexBuffer* vb)
	: campfire(vb, loader->entities["Campfire"])
{
}

void Campfire::tick()
{
	campfire.tick();
}

void Campfire::teleport(float x, float y)
{
	campfire.teleport(x, y);
}

Blue_Slime::Blue_Slime(VertexBuffer* vb)
	: blue_slime(vb, loader->entities["Blue_Slime"]), momentum(), position()
{
	blue_slime.set_animation(0);
}

void Blue_Slime::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

float* Blue_Slime::give_health()
{
	return &Health;
}

float* Blue_Slime::give_x()
{
	return &position[0];
}

float* Blue_Slime::give_y()
{
	return &position[1];
}

void Blue_Slime::teleport(float x, float y)
{
	position[0] = x;
	position[1] = y;
	blue_slime.teleport(x, y);
}

void Blue_Slime::tick()
{
	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);
	entity_return tick_state = blue_slime.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 500) {
		if (tick_state.anim_state == animation_state::advanced_frame)
		{
			frame++;
			if (frame > 3)
				frame = 0;
		}

		if (frame == 2)
		{
			if (frame == 2)
			{
				float magnitude = 2;
				if (bops < 3) {
					if (horizontal == true) { momentum[0] += magnitude; }
					else if (horizontal == false) { momentum[0] += -magnitude; }

					if (vertical == false) { momentum[1] += -magnitude; }
					else if (vertical == true) { momentum[1] += magnitude; }

					dmg_control = 0;
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

				blue_slime.translate(round(momentum[0]), round(momentum[1]));

				momentum[0] = 0;
				momentum[1] = 0;
			}

			if (abs(position[0] - *player_position_x) < 40 and abs(position[1] - *player_position_y) < 40) {
				if (dmg_control < 10) {
					*player_health -= 1;
					dmg_control++;
				}
				bops += 1;
			}
		}
	}
	if (Health <= 0) {
		position[0] = 100000;
		position[1] = 100000;
		blue_slime.teleport(100000, 100000);
	}
}

Ice_Slime::Ice_Slime(VertexBuffer* vb)
	: ice_slime(vb, loader->entities["Ice_Slime"]), momentum(), position()
{
	ice_slime.set_animation(0);
}

void Ice_Slime::Get_player_position(float* x, float* y)
{
	player_position_x = x;
	player_position_y = y;
}

float* Ice_Slime::give_health()
{
	return &Health;
}

float* Ice_Slime::give_x()
{
	return &position[0];
}

float* Ice_Slime::give_y()
{
	return &position[1];
}

void Ice_Slime::teleport(float x, float y)
{
	position[0] = x;
	position[1] = y;
	ice_slime.teleport(x, y);
}

void Ice_Slime::tick()
{
	bool horizontal = Player_Detection_simple_horizontal(position[0], player_position_x);
	bool vertical = Player_Detectoin_simple_vertical(position[1], player_position_y);
	entity_return tick_state = ice_slime.tick();

	if (Player_Detetion_distance(Player_Detection_distance_Horizontal(position[0], player_position_x), Player_Detection_distance_Vertical(position[1], player_position_y)) <= 500) {
		if (tick_state.anim_state == animation_state::advanced_frame)
		{
			frame++;
			if (frame > 3)
				frame = 0;
		}

		if (frame == 2)
		{
			if (frame == 2)
			{
				float magnitude = 2;
				if (bops < 3) {
					if (horizontal == true) { momentum[0] += magnitude; }
					else if (horizontal == false) { momentum[0] += -magnitude; }

					if (vertical == false) { momentum[1] += -magnitude; }
					else if (vertical == true) { momentum[1] += magnitude; }

					dmg_control = 0;
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

				ice_slime.translate(round(momentum[0]), round(momentum[1]));

				momentum[0] = 0;
				momentum[1] = 0;
			}

			if (abs(position[0] - *player_position_x) < 40 and abs(position[1] - *player_position_y) < 40) {
				if (dmg_control < 10) {
					*player_health -= 1;
					dmg_control++;
				}
				bops += 1;
			}
		}
	}
	if (Health <= 0) {
		position[0] = 100000;
		position[1] = 100000;
		ice_slime.teleport(100000, 100000);
	}
}
