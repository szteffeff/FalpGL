#include "Animation.h"

#include <fstream>
#include <iostream>



Animation::Animation(quad* quad)
	: m_quad(quad), current_frame(0), active(false), atlas(0), length(0), loop(true), last_time(std::chrono::high_resolution_clock::now())
{
	load(animation_id::ENTITY_PLAYER_MOVE_IDLE);

	if (length <= 0) {/*json loading messed up...*/}


}

bool Animation::tick()
{
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last_time).count() > times[current_frame] * 1000)
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

void Animation::load(animation_id id)
{
	std::ifstream file("res/data/animations.json");
	std::stringstream buf;
	buf << file.rdbuf();
	std::string file_string(buf.str());
	json j = json::parse(file_string); //File stream must be converted to string for some reason...

	j = j[std::to_string(id)];

	loop = j["loop"];
	atlas = j["atlas"];
	length = j["length"];

	tex_coords.resize(length);
	times.resize(length);

	for (int i = 0; i < length; i++)
	{
		times[i] = j["frame_times"][i];

		tex_coords[i].coords[0][0] = j["texture_coordinants"][i][0][0];
		tex_coords[i].coords[0][1] = j["texture_coordinants"][i][0][1];

		tex_coords[i].coords[1][0] = j["texture_coordinants"][i][1][0];
		tex_coords[i].coords[1][1] = j["texture_coordinants"][i][1][1];

		tex_coords[i].coords[2][0] = j["texture_coordinants"][i][2][0];
		tex_coords[i].coords[2][1] = j["texture_coordinants"][i][2][1];

		tex_coords[i].coords[3][0] = j["texture_coordinants"][i][3][0];
		tex_coords[i].coords[3][1] = j["texture_coordinants"][i][3][1];
	}

	m_quad->set_texture_coords(tex_coords[current_frame]);

}
