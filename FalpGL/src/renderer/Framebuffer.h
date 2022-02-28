#pragma once
#include "GraphicsHeaders.h"
#include "BatchRenderer.h"
#include <iostream>

class Framebuffer { 
	/* Use color attachments instead of 2 framebuffers - multiple textures + shaders
	*  bloom
	*/
private:
	float vertices[24] = {
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

protected:
	unsigned int framebuffer_id, texture_0_id, texture_1_id, renderbuffer_id;
	int m_width, m_height, screen_0_texture, screen_1_texture;


	Shader shader;
	VertexArray framebuffer_vertex_array;
	VertexBuffer framebuffer_vertex_buffer;
	VertexBufferLayout framebuffer_vertex_layout;

public:
	Framebuffer(int width, int height, std::string shader_filepath, int texture_unit = 16);
	~Framebuffer();
	void bind();
	void unbind();
	virtual void draw();

};

class HSL_Framebuffer : public Framebuffer {
public:
	HSL_Framebuffer(int width, int height, int texture_unit = 16);
	void set_saturation(float sat);
	void set_value(float sat);
	void set_hue(float hue);
};


class Chroma_Framebuffer : public Framebuffer {
public:
	Chroma_Framebuffer(int width, int height, int texture_unit = 16);
	void set_chroma(float chroma);
};
