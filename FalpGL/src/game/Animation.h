#pragma once

#include <string>

#include "../renderer/Texture.h"

class Animation {
public:

	Animation(std::string filepath, unsigned int frame_count, unsigned int speed);
	Animation();

	~Animation();

	Texture& get_texture();

	Texture texture;

	unsigned int frame_count, speed, sheet_width, sheet_height, frame_width, frame_height;

private:
	
	std::string filepath;

	

};