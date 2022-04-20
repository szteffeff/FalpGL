#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "../types/AnimationTypes.h"
#include "../renderer/RendererIncludes.h"
#include "Json.h"


enum class animation_state {
	none = 0,
	advanced_frame,
	ended
 };

class Animation {
public:

	Animation(Quad* quad, std::string name);
	Animation();

	static Json_loader* loader;

	animation_state tick();
	void set();
	void advance_frame();

private:
	Quad* m_quad;
	bool loop;
	unsigned int atlas, current_frame, length;
	std::string name;
	std::vector<float> times;
	std::vector<tex_coord> tex_coords;
	std::chrono::high_resolution_clock::time_point last_time;

private:
	void load(std::string name);
};
