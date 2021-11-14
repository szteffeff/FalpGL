#include "Animation.h"



Animation::Animation(quad* quad)
	: m_quad(quad), current_frame(0), active(false)
{
	tex_coords.resize(length);
	times.resize(length);
	//load coords/times from file
}

bool Animation::tick()
{
	if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last_time).count() > times[current_frame])
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


	}

	return true;
}
