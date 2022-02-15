#pragma once
#include "../renderer/RendererIncludes.h"
#include "Animation.h"
#include <iostream>
#include <math.h>



class Entity {
protected:
	Quad main_quad;
	std::vector<Animation> animations;
	int active_animation, animation_count;
	std::string name;

public:
	Entity(VertexBuffer* a, json load);
	Entity();
	~Entity();

public:
	virtual void tick();
	void set_animation(int id);
	void translate(float x, float y);
	void set_z(float z);
	Point center();
	void set_vertex_pos(float x, float y, int index);
	void set_vertex_pos(Point p, int index);
	Point get_vertex_pos(int index);

};

