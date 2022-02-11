#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "../renderer/Texture.h"
#include "BatchRenderer.h"
#include "../types/AnimationTypes.h"
#include "Json.h"

class Animation {
public:

	Animation(Quad* quad, std::string name);
	Animation();

	static Json_loader* loader;

	bool tick();
	void set();

private:
	Quad* m_quad;
	bool loop;
	float atlas, length, current_frame;
	std::string name;
	std::vector<float> times;
	std::vector<tex_coord> tex_coords;
	std::chrono::high_resolution_clock::time_point last_time;

private:
	void load(std::string name);
};
