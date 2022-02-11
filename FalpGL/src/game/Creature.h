#pragma once
#include "Entity.h"



class Creature {
public:
	Creature();
};




//class Player : public Creature {};



class Health_Bar : public Creature {
private:
	Entity frame;
	Entity red_bar;
	Entity green_bar;

public:
	Health_Bar(BatchRenderer* for_active_buffer, Json_loader* l);

	void tick(float health_level, float stamina_level, int flask_state);
};