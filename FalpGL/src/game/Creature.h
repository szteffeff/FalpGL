#pragma once
#include "Entity.h"



class Creature {
public:
	static Json_loader* loader;

public:
	Creature();

	virtual void tick();
};





class Player : public Creature {
private:
	Entity m_player;
	glm::mat4 player_transform_matrix = glm::mat4(1.0f);
	float offset[2] = { 0.0f, 0.0f };
	float momentum[2], position[2];
	float Health;

	enum player_animations { // wrong
		IDLE = 0,
		UP,
		LEFT,
		DOWN,
		RIGHT
	};


public:
	Player(VertexBuffer* vb);
	void walk(float direction, float magnitude);
	glm::mat4* get_trans_matrix();
	float position_x();
	float position_y();
	void tick();
	float* GetHealth();


};



class Health_Bar : public Creature {
private:
	Entity frame;
	Entity red_bar;
	Entity green_bar;

	const float player_max_val = 100;
	const float x_offset = -450;
	const float y_offset = -500;

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
	void set_health(float h);
	void set_stamina(float s);
	void set_level(frame_animations level);
};