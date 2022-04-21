#pragma once
#include "../renderer/RendererIncludes.h"




class Decoration {
protected:
	float vertex_data[20];
	int buffer_index;
	int id;

public:
	Decoration(float x, float y, float size_x, float size_y, Prototype_Tile tile);

	void fade(float opacity = 1.0f);
	float* data();
};

class Static_Entity : public Decoration {
public:
	Static_Entity(float x, float y, float size_x, float size_y, Prototype_Tile tile);

	virtual void tick();
	virtual void interact();
	virtual void hit();
};



class Decoration_Renderer {
private:
	Shader dec_shader;
	VertexArray dec_vertex_array;
	VertexBuffer dec_vertex_buffer;
	IndexBuffer dec_index_buffer;

	Tileset decoration_set;

	int samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	float texture_index = 5.0f;

	std::vector<Decoration> decorations;
	std::vector<float> vertex_data;

public:
	Decoration_Renderer(nlohmann::json tileset_json, nlohmann::json decorations);
	Decoration_Renderer();

	void init(nlohmann::json tileset_json, nlohmann::json decorations);

	void add_decoration(Decoration d);

	void draw(glm::mat4 projection_matrix);

	void tick(float pos_x = 0, float pos_y = 0);
};