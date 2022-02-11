#pragma once
#include "Entity.h"



class Creature {
public:
	static Json_loader* loader;
	Creature();
};




//class Player : public Creature {};



class Health_Bar : public Creature {
private:
	Entity frame;
	Entity red_bar;
	Entity green_bar;

public:
	Health_Bar(VertexBuffer* buffer);

	void tick(float health_level, float stamina_level, int flask_state);
};