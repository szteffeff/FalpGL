#pragma once
#include "Entity.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "SoundDevice.h"
#include "SFX.h"
#include "NewMap.h"
#include <math.h>


class Creature {
public:
	static Json_loader* loader;
	float momentum[2], position[2];
	float* player_health;
	static float* curser_x;
	static float* curser_y;
	float health = 100;

	float* player_position_x = 0;
	float* player_position_y = 0;
	int* using_weapon;

	enum attacks {
		bow,
		dagger_light,
		dagger_heavy,
		axe_light,
		axe_heavy,
		spear_light,
		spear_heavy,
	};

public:
	Creature();
	virtual bool Player_Detection_simple_horizontal(float x, float* player_x);
	virtual bool Player_Detectoin_simple_vertical(float  y, float* player_y);
	virtual float Player_Detection_distance_Horizontal(float x, float* player_x);
	virtual float Player_Detection_distance_Vertical(float y, float* player_y);
	virtual float Player_Detetion_distance(float horizontal, float vertical);
	virtual void Player_Health(float* health);
	virtual void walk(float direction, float magnitude);

	virtual void attacked(float x, float y, float radius, int attack);
	virtual void get_weapon(int* weapon);

	virtual void Get_player_position(float* x, float* y);

	//weapon detection
	virtual float Arrow_Detection_horizontal(float x, float* arrow_x);
	virtual float Arrow_Detection_vertical(float y, float* arrow_y);

	virtual float Dagger_Detection_horizontal(float x, float* dagger_x);
	virtual float Dagger_Detection_vertical(float y, float* dagger_y);

	virtual float Axe_Detection_horizontal(float x, float* axe_x);
	virtual float Axe_Detection_vertical(float y, float* axe_y);

	virtual float Spear_Detection_horizontal(float x, float* spear_x);
	virtual float Spear_Detection_vertical(float y, float* spear_y);

	virtual void tick();
};

class Player : public Creature {
private:
	Entity m_player;
	Entity Player_bow;
	Entity Player_spear;
	Entity Player_dagger;
	Entity Player_Shatter_axe;
	Entity Player_arrow;

	glm::mat4 player_transform_matrix = glm::mat4(1.0f);
	SFX walking_sound;
	uint32_t walking = SoundBuffer::get()->addSoundEffect("files/SFX/Footsteps(better).wav");
	SFX hurt_sound;
	uint32_t hurt = SoundBuffer::get()->addSoundEffect("files/SFX/hurt_noise.wav");
	float offset[2] = { 0.0f, 0.0f };
	float momentum[2], position[2];
	float Health = 100;
	float Stamina = 100;
	int Potion = 4;
	float direction = 0;
	bool dodging = false;
	int magnitude;
	int dodge_frames = 60 * 2;
	bool RUN = false;
	int dodge_momentum = 5;

	int weapon_type;
	
	/// //////////////////////////////////////// Bow stuff
	
	bool shoot_bow = false;
	bool shwoop_shwoop = true;
	int dmg_control = 0;
	float dx = 0, dy = 0;
	float dx2 = 0, dy2 = 0;
	int arrow_lifespan = 60 * 4;
	int bow_lifespan = 60 * 1;
	bool arrow_going = false;

	/// //////////////////////////////////////// Shanky stuff
	
	// light attack stuff
	bool light_dagger = false;
	int frames_dagger_L = 0;
	int dagger_frame_L = 0;
	int dagger_start_L = 0;
	int dagger_end_L = 0;

	// heavy attack stuff
	bool heavy_dagger = false;
	int dagger_start_H = 0;
	int dagger_frame_H = 0;
	int dagger_mid_H = 0;
	int dagger_end_H = 0;
	float rad_dagger = 10;
	float time_dagger = 60 * 2;
	bool got_num_dagger = false;
	int speacial_move_dagger = rand() % 10;


	/// //////////////////////////////////////// Axy stuff
	
	bool attacking = false;

	// light attack stuff
	bool light_axe = false;
	int frames_axe = 0;
	int axe_frame = 0;
	int axe_start = 0;
	int axe_end = 0;

	//heavy attack stuff

	bool heavy_axe = false;
	int axe_start_H = 0;
	int axe_frame_H = 0;
	int axe_mid_H = 0;
	int axe_end_H = 0;
	float rad_axe = 10;
	float time_axe = 60 * 7.5;
	bool got_num_axe = false;
	int speacial_move_axe = rand() % 10;

	///////////////////////////////////////////// Speary stuff

	//light attack stuff
	bool light_spear = false;
	int frames_spear_L = 0;
	int spear_frame_L = 0;
	int spear_start_L = 0;
	int spear_end_L = 0;

	//heavy attack stuff
	bool heavy_spear = false;
	int frames_spear_H = 0;
	int spear_frame_H = 0;
	int spear_start_H = 0;
	int spear_end_H = 0;


	/// ////////////////////////////////////////

	enum class player_animations {
		IDLE = 0,
		UP,
		LEFT,
		DOWN,
		RIGHT
	};

	New_Map* active_map = nullptr;

public:
	void set_active_map(New_Map* map = nullptr);


	Player(VertexBuffer* vb);
	void walk(float direction, float magnitude);
	void sprint(float direction, float magnitude);
	void dodge(float direction, float magnitude);
	glm::mat4* get_trans_matrix();
	float position_x();
	float position_y();
	float* get_position_x();
	float* get_position_y();
	int get_weapon_type();
	float weapon_x();
	float weapon_y();
	bool am_attacking();
	void tick();
	float* GetHealth();
	float* GetStamina();
	int* GetPotion();
	void Take_Damage();
	void Take_Damage_tile();
	void Take_Heal();
	void Recover_Stamina();
	void Lose_Stamina();
	bool shwoop_doop();
	void Shoot_bow();
	void Dagger_light();
	void Dagger_Heavy();
	void Axe_Light();
	void Axe_Heavy();
	void Spear_Light();
	void Spear_Heavy();
	//float Change_Health(float Damage, float Heal)
};

class Red_Slime : public Creature {
private:
	Entity Red_slime;
	float momentum[2], position[2];
	const float Damage = 10;
	float* player_position_x;
	float* player_position_y;
	int frame;
	int bops;
	int bops_frame;
	int dmg_control = 0;

public:
	Red_Slime(VertexBuffer* vb);
	void Get_player_position(float* x, float* y);
	void tick() override;
};

class Enemy_Ghost : public Creature {
private:
	Entity Enemy_ghost;
	Entity Wizard_pink_bullet;

	float momentum[2], position[2];
	const float Health = 20;
	const float Damage = 10;
	float* player_position_x;
	float* player_position_y;
	int bullet_lifespan = 60 * 4;
	float dx = 0, dy = 0;
	int frames = 0;
	int frames_magic = 0;
	int teleporting = 0;
	int dmg_control = 0;
	SFX Ghost_move_sound;
	uint32_t Ghost_move = SoundBuffer::get()->addSoundEffect("files/SFX/ghost-moving-sound.wav");

public:
	Enemy_Ghost(VertexBuffer* vb);
	void Get_player_position(float* x, float* y);
	void tick();
};

class Garfield : public Creature {
private:
	Entity garfield;
	float* player_position_x;
	float* player_position_y;
public:
	Garfield(VertexBuffer* vb);
	void tick();
};

class Cow : public Creature {
private:
	Entity cow;
	float* player_position_x;
	float* player_position_y;
public:
	Cow(VertexBuffer* vb);
	void tick();
};

class Perry : public Creature {
private:
	Entity perry;
	float* player_position_x;
	float* player_position_y;
public:
	Perry(VertexBuffer* vb);
	void tick();
};

class Edgelord : public Creature {
private:
	Entity edgelord;
	float* player_position_x;
	float* player_position_y;
public:
	Edgelord(VertexBuffer* vb);
	void tick();
};

class Bush_Boi : public Creature {
private:
	Entity Bush_boi;
	float momentum[2], position[2];
	const float Health = 20;
	const float Damage = 10;
	float* player_position_x;
	float* player_position_y;
	int frame;
	int bops;
	int bops_frame;
	int dmg_control = 0;
	bool side_change = true;
	bool change_animation = true;

public:
	Bush_Boi(VertexBuffer* vb);
	void Get_player_position(float* x, float* y);
	void tick();
};

class Chompy_Slime : public Creature {
private:
	Entity Chompy_slime;
	float momentum[2], position[2];
	const float Health = 20;
	const float Damage = 10;
	float* player_position_x;
	float* player_position_y;
	int frame;
	int bops;
	int bops_frame;
	int dmg_control = 0;

public:
	Chompy_Slime(VertexBuffer* vb);
	void Get_player_position(float* x, float* y);
	void tick();
};

class Health_Bar : public Creature {
private:
	Entity frame;
	Entity red_bar;
	Entity green_bar;

	const float player_max_val = 100;
	const float x_offset = -450;
	const float y_offset = -450;

	float red_bar_max, green_bar_max;
	float red_bar_min, green_bar_min;
	float green_slope, red_slope;
	
public:
	enum class frame_animations {
			empty = 0,
			quarter,
			half,
			three_quarters,
			full
	};

	Health_Bar(VertexBuffer* buffer);

	void tick();
	void tick(float h, float s, frame_animations level);
	void set_health(float h);
	void set_stamina(float s);
	void set_level(frame_animations level);
};

class Sussy_Vase : public Creature {
private:
	Entity Sussy_vase;
	Entity Wizard_pink_bullet;

	float momentum[2], position[2];
	const float Health = 20;
	const float Damage = 10;
	float* player_position_x;
	float* player_position_y;
	int bullet_lifespan = 60 * 4;
	float dx = 0, dy = 0;
	int frames = 0;
	int frames_magic = 0;
	int dmg_control = 0;
	
public:
	Sussy_Vase(VertexBuffer* vb);
	void Get_player_position(float* x, float* y);
	void tick();
};

class Clair_Of_Cavern : public Creature {
private:
	Entity clair_of_cavern;
	float* player_position_x = 0;
	float* player_position_y = 0;
public:
	Clair_Of_Cavern(VertexBuffer* vb);
	void tick();
};

class Del_Ibra_of_Hillsby : public Creature {
private:
	Entity del_ibra_of_hillsby;
	float* player_position_x = 0;
	float* player_position_y = 0;
public:
	Del_Ibra_of_Hillsby(VertexBuffer* vb);
	void tick();
};

class Eloah_of_Minlet : public Creature {
private:
	Entity eloah_of_minlet;
	float* player_position_x = 0;
	float* player_position_y = 0;
public:
	Eloah_of_Minlet(VertexBuffer* vb);
	void tick();
};

class Felix_of_Festria : public Creature {
private:
	Entity felix_of_festria;
	float* player_position_x = 0;
	float* player_position_y = 0;
public:
	Felix_of_Festria(VertexBuffer* vb);
	void tick();
};

class Maban_of_Undermount: public Creature {
private:
	Entity maban_of_undermount;
	float* player_position_x = 0;
	float* player_position_y = 0;
public:
	Maban_of_Undermount(VertexBuffer* vb);
	void tick();
};