#include "Animation.h"

#include <fstream>
#include <iostream>


Animation::Animation(Quad* quad, Json_loader* j, unsigned int id)
	: m_quad(quad), current_frame(0), active(false), atlas(0), length(0), loop(true), loader(j) , last_time(std::chrono::high_resolution_clock::now())
{
	load(Animation_id(id));

	if (length <= 0) {/*json loading messed up...*/}


}

Animation::Animation()
	: m_quad(nullptr), loader(nullptr)
{}

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

void Animation::load(Animation_id id)
{
	json j;
	j = loader->animations[std::to_string((int)id)];

	loop = j["loop"];
	atlas = j["atlas"];
	length = j["length"];

	int frame = 0;

	tex_coords.resize(length);
	times.resize(length);

	for (int i = 0; i < length; i++)
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
}

void Animation::set()
{
	active = true;
	m_quad->set_texture_index(atlas);
	tick();
}

void Animation::unset()
{
	active = false;
}
