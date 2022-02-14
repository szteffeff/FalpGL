#pragma once
#include "Entity.h"



class Creature {
public:
	static Json_loader* loader;
	Creature();
};




class Player : public Creature {
private:
	Entity m_player;
	glm::mat4 player_transform_matrix = glm::mat4(1.0f);
	float offset[2] = { 0.0f, 0.0f };
	float momentum[2], position[2];
	float Health;
	float Stamina;

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
	float* GetStamina();

};



class Health_Bar : public Creature {
private:
	Entity frame;
	Entity red_bar;
	Entity green_bar;

public:
	Health_Bar(VertexBuffer* buffer);

	void tick(float health_level, float stamina_level, int flask_state);
};