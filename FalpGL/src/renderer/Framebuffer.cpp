#include "Framebuffer.h"


Framebuffer::Framebuffer(int width, int height)
	: m_width(width), m_height(height), s("res/shaders/frame.shader"), va(), vb(24 * sizeof(float), &vertices), vbl()
{
	/* Generate framebuffer object */
	GLCall(glGenFramebuffers(1, &framebuffer_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));

	/* Generater texture */
	GLCall(glGenTextures(1, &texture_id));

	GLCall(glActiveTexture(GL_TEXTURE16));
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
	vbl.Push<float>(2);
	vbl.Push<float>(2);

	va.AddBuffer(vb, vbl);

	s.Bind();
	s.SetUniform1f("hue", 0.0f);
	s.SetUniform1f("sat", 100.0f);
	s.SetUniform1f("val", 100.0f);

	/* Unbind framebuffer */
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &framebuffer_id);
}

void Framebuffer::bind()
{
	GLCall(glActiveTexture(GL_TEXTURE16));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));
}

void Framebuffer::unbind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::set_saturation(float sat)
{
	s.Bind();
	s.SetUniform1f("sat", sat);
}

void Framebuffer::set_value(float val)
{
	s.Bind();
	s.SetUniform1f("val", val);
}

void Framebuffer::draw()
{
	va.Bind();
	s.Bind();
	s.SetUniform1i("screenTexture", 16);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}