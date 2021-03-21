#include "Animation.h"







Animation::Animation(std::string filepath, unsigned int frame_count, unsigned int speed)
	: filepath(filepath), frame_count(frame_count), speed(speed)
{
	texture = Texture(filepath);

	sheet_width = texture.GetWidth();
	sheet_height = texture.GetHeight();

	frame_width = sheet_width;
	frame_height = sheet_height / frame_count;
}

Animation::~Animation()
{
}

Animation::Animation()
	: filepath(0), frame_count(0), speed(0), frame_height(0), frame_width(0), sheet_width(0), sheet_height(0)
{
}

Texture& Animation::get_texture()
{
	return texture;
}
