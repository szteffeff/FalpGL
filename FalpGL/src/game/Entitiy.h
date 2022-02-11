#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h"
#include "Animation.h"
#include <iostream>
#include <math.h>


/* class player has player entity
* class health bar has 3 enities
*/

class Sub_Entity {
protected:
	Quad main_quad;
	std::vector<Animation> animations;
	Json_loader* loader; // remove, pass by copy to scoped json object
	int active_animation, animation_count, entity_id; // remove id
	float momentum[2], position[2]; // remove

public:
	std::string name; // remove
	Sub_Entity(VertexBuffer* a, Json_loader* load, unsigned int id);
	Sub_Entity(VertexBuffer* a, json load);
	Sub_Entity();
	Quad* get_quad(); //why

public:
	virtual void tick();

	void walk(float direction, float magnitude); // handled in full entity
	void set_animation(int id);

};


class Player : public Sub_Entity { /* rewrite player as full entity */
	using Sub_Entity::Sub_Entity;
private:
	glm::mat4 player_transform_matrix = glm::mat4(1.0f);

	float offset[2] = { 0.0f, 0.0f };
	
	enum class player_animations { // wrong
		IDLE = 0,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		HURT,
		X,
		Y,
		Z
	};


public:
	glm::mat4 *get_trans_matrix();
	float position_x();
	float position_y();
	void tick();
};

class Health_Bar {
private:
	Sub_Entity frame;
	Sub_Entity red_bar;
	Sub_Entity green_bar;

public:
	Health_Bar(BatchRenderer* for_active_buffer, Json_loader* l);

	void tick(float health_level, float stamina_level, int flask_state);
	
};
