#include "Animation.h"

#include <fstream>
#include <iostream>


Animation::Animation(Quad* quad, std::string name)
	: m_quad(quad), current_frame(0), atlas(0), length(0), loop(true), last_time(std::chrono::high_resolution_clock::now())
{
	load(name);
}

Animation::Animation()
	: m_quad(nullptr)
{
	loop = true;
	atlas = 0;
	length = 0;
	current_frame = 0;
}

bool Animation::tick()
{
	if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()
		- last_time).count() > times[current_frame]))
	{
		if (current_frame == length - 1)
		{
			if (loop)
				current_frame = 0;
			else
				return false;
		}
		else
		{
			current_frame++;
		}

		m_quad->set_texture_coords(tex_coords[current_frame]);

		last_time = std::chrono::high_resolution_clock::now();

	}

	return true;
}

void Animation::load(std::string name)
{
	json j;
	j = loader->animations[name];

	loop = j["loop"];
	atlas = j["atlas"];
	length = j["length"];

	int frame = 0;

	tex_coords.resize(length);
	times.resize(length);

	for (unsigned int i = 0; i < length; i++)
	{
		frame = j["frame_order"][i];

		times[i] = j["frame_times"][i];

		tex_coords[i].coords[0][0] = (j["texture_coordinants"][frame][0][0]) / 2048.0f; /* Denominator must be float or intiger division happens */
		tex_coords[i].coords[0][1] = (j["texture_coordinants"][frame][0][1]) / 2048.0f;

		tex_coords[i].coords[1][0] = (j["texture_coordinants"][frame][1][0]) / 2048.0f;
		tex_coords[i].coords[1][1] = (j["texture_coordinants"][frame][1][1]) / 2048.0f;

		tex_coords[i].coords[2][0] = (j["texture_coordinants"][frame][2][0]) / 2048.0f;
		tex_coords[i].coords[2][1] = (j["texture_coordinants"][frame][2][1]) / 2048.0f;

		tex_coords[i].coords[3][0] = (j["texture_coordinants"][frame][3][0]) / 2048.0f;
		tex_coords[i].coords[3][1] = (j["texture_coordinants"][frame][3][1]) / 2048.0f;
	}

	m_quad->set_texture_coords(tex_coords[current_frame]);
	m_quad->set_texture_index((float)atlas);
}

void Animation::set()
{
	m_quad->set_texture_index((float)atlas);
	m_quad->set_texture_coords(tex_coords[current_frame]);
	tick();
}
