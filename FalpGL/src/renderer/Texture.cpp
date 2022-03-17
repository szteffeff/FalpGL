#include "Texture.h"

Texture::Texture(const std::string& filepath)
	: m_renderer_id(0), m_filepath(filepath), m_local_buffer(nullptr), m_height(0), m_width(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_local_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_renderer_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_local_buffer) {
		stbi_image_free(m_local_buffer);
	}
}

void Texture::reload(std::string& filepath)
{
	m_filepath = filepath;

	stbi_set_flip_vertically_on_load(1);
	m_local_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_renderer_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_local_buffer) {
		stbi_image_free(m_local_buffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_renderer_id));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
