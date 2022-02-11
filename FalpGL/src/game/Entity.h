#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h"
#include "Animation.h"
#include <iostream>
#include <math.h>


/* class player has player entity
* class health bar has 3 enities
*/

class Entity {
protected:
	Quad main_quad;
	std::vector<Animation> animations;
	int active_animation, animation_count;
	std::string name;

public:
	Entity(VertexBuffer* a, json load);
	Entity();

public:
	virtual void tick();
	void set_animation(int id);

};


class Player : public Entity { 
	using Entity::Entity;
private:
	glm::mat4 player_transform_matrix = glm::mat4(1.0f);

	float offset[2] = { 0.0f, 0.0f };
	float momentum[2], position[2];
	float Health;
	
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
	void walk(float direction, float magnitude);
	glm::mat4 *get_trans_matrix();
	float position_x();
	float position_y();
	void tick();
	float* GetHealth();
};


