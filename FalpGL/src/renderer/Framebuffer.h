#pragma once
#include "GraphicsHeaders.h"
#include "BatchRenderer.h"
#include <iostream>

class Framebuffer {
private:
	unsigned int framebuffer_id, texture_id, renderbuffer_id;
	int m_width, m_height;

	float vertices[24] = {
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	Shader s;
	VertexArray va;
	VertexBuffer vb;
	VertexBufferLayout vbl;

public:
	Framebuffer(int width, int height);
	~Framebuffer();
	void bind();
	void unbind();
	void draw();
	void set_saturation(float sat);
	void set_value(float sat);
};