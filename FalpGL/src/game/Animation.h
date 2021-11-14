#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "../renderer/Texture.h"
#include "BatchRenderer.h"


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
	std::chrono::high_resolution_clock::time_point last_time = std::chrono::high_resolution_clock::now();





};

/*
* In json/xml file
* 
* name/id
* frame times
* 
* list of texture coords
*	{{1, 0}, {...}, {...}, {...}}
* 
*/