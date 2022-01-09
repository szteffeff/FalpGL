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
	void tick();

	void walk(float direction, float magnitude);
	void set_animation(int id);

};