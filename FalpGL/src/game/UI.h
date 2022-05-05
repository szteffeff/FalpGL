#pragma once
#include "../renderer/RendererIncludes.h"
#include "../types/TileTypes.h"
#include "Json.h"
#include <iostream>
#include "Entity.h"
#include "Creature.h"

enum class button_function {
	nothing = 0,
	resume,
	quit
};


class Button{
	/*
	* Just a entity and collision box for clicks
	* takes inputs from -1 to 1 so buttons are put in the right place on different resolution screens
	*/
private:
	Entity button_enitiy;
	Point bottom_left;
	Point top_right;

public:
	Button(VertexBuffer* buffer, Point _bottom_left, Point size, Point resolution, nlohmann::json loader);
	button_function pressed(float x, float y);
};


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

class UserInterface {
	float* Player_Health;
	float* Player_Stamina;
	int* Player_Potion;
	Health_Bar health_Bar;
	

	Shader text_shader;
	Tileset text_set;
	std::vector<Text*> texts;

public:
	void create_text(std::string text, float x, float y, int time);
	void draw_text(glm::mat4 matrix);


	UserInterface(VertexBuffer* buffer);
	void SetHealth(float* H);
	void SetStamina(float* S);
	void SetPotion(int* P);
	void UI_Tick();
	void Placeholder_Health(float HP);
	void Placeholder_Stamina(float SP);
	void Placeholder_Potion(int P);
	void Imma_Keep_It_100();
};

