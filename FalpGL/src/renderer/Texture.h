#pragma once

#include "renderer.h"


class Texture
{
private:
	unsigned int m_renderer_id;
	std::string m_filepath;
	unsigned char* m_local_buffer;
	int m_width, m_height, m_BPP;

public:
	Texture(const std::string& filepath);
	Texture() {}
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind();

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
};
