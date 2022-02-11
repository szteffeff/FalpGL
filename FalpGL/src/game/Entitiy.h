#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h"
#include "Animation.h"
#include <iostream>
#include <math.h>

class Entity {
protected:
	Quad m_quad;
	std::vector<Animation> animations;
	Json_loader* loader;
	int active_animation, animation_count, entity_id;
	float momentum[2], position[2];

public:
	std::string name;
	Entity();
	Entity(VertexBuffer* a, Json_loader* load, unsigned int id);
	Quad* get_quad();

public:

	virtual void tick();

	void walk(float direction, float magnitude);
	void set_animation(int id);

};


class Player : public Entity { 
	using Entity::Entity;
private:
	glm::mat4 player_transform_matrix = glm::mat4(1.0f);

	float offset[2] = { 0.0f, 0.0f };
	
	float Health = 100;
	float Stamina = 100;

public:
	glm::mat4 *get_trans_matrix();
	float position_x();
	float position_y();
	void tick();
	float* GetHealth();
	float* GetStamina();
};
