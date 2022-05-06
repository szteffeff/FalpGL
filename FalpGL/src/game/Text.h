#pragma once
#include "../renderer/RendererIncludes.h"


struct Text {
	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;

	std::string text;


	std::vector<float> vertex_data;

	int ttl;
	int count;
	int size = 16;

	Text(std::string text, float x, float y, int time, Tileset& set);
};


class Text_Renderer {
	Shader text_shader;
	Tileset text_set;
	std::vector<Text*> texts;


public:
	Text_Renderer();

	void create_text(std::string text, float x, float y, int time);
	void draw_text(glm::mat4 matrix);
	void tick();
};