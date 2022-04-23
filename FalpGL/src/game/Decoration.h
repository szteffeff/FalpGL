#pragma once
#include "../renderer/RendererIncludes.h"
#include <algorithm>




class Decoration {
protected:
	float vertex_data[20];
	int buffer_index;

	bool nofade;

public:
	const float x, y;
	const float center_x, center_y;
	const float size_x, size_y;
	const int id;


	Decoration(float x, float y, float size_x, float size_y, Prototype_Tile tile);
	Decoration(nlohmann::json object, Tileset& set);

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
	std::vector<Decoration> close_to_player;
	std::vector<float> vertex_data;

public:
	Decoration_Renderer(nlohmann::json tileset_json, nlohmann::json decorations);
	Decoration_Renderer();

	bool collision(float x, float y);

	void init(nlohmann::json tileset_json, nlohmann::json decorations);

	void add_decoration(Decoration d);

	void draw(glm::mat4 projection_matrix);

	void tick(float pos_x = 0, float pos_y = 0);
};