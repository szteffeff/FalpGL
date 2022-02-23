#include "Framebuffer.h"


Framebuffer::Framebuffer(int width, int height, std::string shader_filepath, int texture_unit)
	: m_width(width), m_height(height), shader(shader_filepath), screen_texture(texture_unit),
	framebuffer_vertex_array(), framebuffer_vertex_buffer(24 * sizeof(float), &vertices), framebuffer_vertex_layout()
{
	/* Generate framebuffer object */
	GLCall(glGenFramebuffers(1, &framebuffer_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));

	/* Generater texture */
	GLCall(glGenTextures(1, &texture_id));

	GLCall(glActiveTexture(GL_TEXTURE0 + screen_texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	
	/* Attach texture to framebuffer*/
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0));

	/* Generate renderbuffer */
	GLCall(glGenRenderbuffers(1, &renderbuffer_id));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height));

	/* Attach renderbuffer to framebuffer*/
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_id));

	/* Check for errors */
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error: " << status << "\n";
	}

	/* Setup renderer */
	framebuffer_vertex_layout.Push<float>(2);
	framebuffer_vertex_layout.Push<float>(2);

	framebuffer_vertex_array.AddBuffer(framebuffer_vertex_buffer, framebuffer_vertex_layout);

	shader.Bind();

	/* Unbind framebuffer */
	unbind();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &framebuffer_id);
}

void Framebuffer::bind()
{
	GLCall(glActiveTexture(GL_TEXTURE0 + screen_texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));
}

void Framebuffer::unbind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}



void Framebuffer::draw()
{
	framebuffer_vertex_array.Bind();

	shader.Bind();
	shader.SetUniform1i("screenTexture", screen_texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



/* ##### HSL_Frambuffer ##### */

HSL_Framebuffer::HSL_Framebuffer(int width, int height, int texture_unit)
	: Framebuffer(width, height, "res/shaders/hsl.shader", texture_unit)
{
	shader.SetUniform1f("hue", 0.0f);
	shader.SetUniform1f("sat", 100.0f);
	shader.SetUniform1f("val", 100.0f);
}

void HSL_Framebuffer::set_saturation(float sat)
{
	shader.Bind();
	shader.SetUniform1f("sat", sat);
}

void HSL_Framebuffer::set_value(float val)
{
	shader.Bind();
	shader.SetUniform1f("val", val);
}

void HSL_Framebuffer::set_hue(float hue)
{
	shader.Bind();
	shader.SetUniform1f("hue", hue);
}

/* ##### Chroma_Frambuffer ##### */

Chroma_Framebuffer::Chroma_Framebuffer(int width, int height, int texture_unit)
	: Framebuffer(width, height, "res/shaders/chroma.shader", texture_unit)
{
	shader.Bind();
	shader.SetUniform1f("chroma", 1.0f);
	shader.SetUniform1f("resolution_x", width);
	shader.SetUniform1f("resolution_y", height);
}

void Chroma_Framebuffer::set_chroma(float chroma)
{
	shader.Bind();
	shader.SetUniform1f("chroma", chroma);
}