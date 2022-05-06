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

class UserInterface {
	float* Player_Health;
	float* Player_Stamina;
	int* Player_Potion;
	Health_Bar health_Bar;


public:


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

