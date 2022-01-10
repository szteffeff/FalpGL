#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h"
#include "Animation.h"
#include <iostream>
#include <math.h>

class Entity {
private:
	Quad m_quad;
	std::vector<Animation> animations;
	Json_loader* loader;
	int active_animation, animation_count;
	float momentum[2];
	int entity_id;

public:
	std::string name;
	Entity(VertexBuffer* a, Json_loader* load, unsigned int id);
	Quad* get_quad();

public:
	virtual void tick();

	virtual void walk(float direction, float magnitude);
	void set_animation(int id);

};


class player : public Entity {
private:
 	glm::mat4 player_transform_matrix;
	float position_on_screen[2] = { 0, 0 };
	
public:
	glm::mat4 *get_trans_matrix();
	cosnt float screen_pos_x();
	const float screen_pos_y();
	void walk(float direction, float magnitude);
	void tick();
};
