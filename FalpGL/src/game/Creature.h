#pragma once
#include "Entity.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "SoundDevice.h"



class Creature {
public:
	static Json_loader* loader;
	float momentum[2], position[2];
	SoundDevice* creaturesound = SoundDevice::get();

public:
	Creature();
	virtual bool Player_Detection_simple_horizontal(float x, float* player_x);
	virtual bool Player_Detectoin_simple_vertical(float  y, float* player_y);
	virtual void walk(float direction, float magnitude);

	virtual void tick();
};





class Player : public Creature {
private:
	SoundSource creaturesound;
	Entity m_player;
	glm::mat4 player_transform_matrix = glm::mat4(1.0f);
	uint32_t walking = SoundBuffer::get()->addSoundEffect("files/SFX/Footsteps(better).wav");
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
	void walk_noise();
	void stop_walknoise();
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

public:
	Enemy_Ghost(VertexBuffer* vb);
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