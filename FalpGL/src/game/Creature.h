#pragma once
#include "Entity.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "SoundDevice.h"
#include "SFX.h"


class Creature {
public:
	static Json_loader* loader;
	float momentum[2], position[2];

public:
	Creature();
	virtual bool Player_Detection_simple_horizontal(float x, float* player_x);
	virtual bool Player_Detectoin_simple_vertical(float  y, float* player_y);
	virtual void walk(float direction, float magnitude);

	virtual void tick();
};

class Player : public Creature {
private:
	Entity m_player;
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

	enum class player_animations {
		IDLE = 0,
		UP,
		LEFT,
		DOWN,
		RIGHT
	};


public:
	Player(VertexBuffer* vb);
	void walk(float direction, float magnitude);
	void sprint(float direction, float magnitude);
	glm::mat4* get_trans_matrix();
	float position_x();
	float position_y();
	float* get_position_x();
	float* get_position_y();
	void tick();
	float* GetHealth();
	float* GetStamina();
	int* GetPotion();
	void Take_Damage();
	void Take_Damage_tile();
	void Take_Heal();
	void Recover_Stamina();
	void Lose_Stamina();
	//float Change_Health(float Damage, float Heal)
};

class Red_Slime : public Creature {
private:
	Entity Red_slime;
	float momentum[2], position[2];
	const float Health = 20;
	const float Damage = 10;
	float* player_position_x;
	float* player_position_y;

public:
	Red_Slime(VertexBuffer* vb);
	void Get_player_position(float* x, float* y);
	void tick();
};

class Enemy_Ghost : public Creature {
private:
	Entity Enemy_ghost;
	float momentum[2], position[2];
	const float Health = 20;
	const float Damage = 10;
	float* player_position_x;
	float* player_position_y;
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

class Bush_Boi : public Creature {
private:
	Entity Bush_boi;
	float* player_position_x;
	float* player_position_y;
public:
	Bush_Boi(VertexBuffer* vb);
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