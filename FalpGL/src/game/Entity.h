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
	void translate(float x, float y);

};

