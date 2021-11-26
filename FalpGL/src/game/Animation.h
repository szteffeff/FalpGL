#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "../renderer/Texture.h"
#include "BatchRenderer.h"
#include "AnimationTypes.h"


class Animation {
public:
	Animation(quad* quad);

	bool tick();

private:
	quad* m_quad;
	bool loop, active;
	int atlas, length, current_frame;
	std::vector<float> times;
	std::vector<tex_coord> tex_coords;
	std::chrono::high_resolution_clock::time_point last_time;

private:
	void load(animation_id id);



};

/*
* In json file
* 
* name/id
* frame times
* 
* list of texture coords
*	{{1, 0}, {...}, {...}, {...}}
* 
*/