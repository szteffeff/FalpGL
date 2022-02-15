#pragma once
#include "../renderer/RendererIncludes.h"
#include "../types/TileTypes.h"
#include "Json.h"
#include "Tile.h"
#include <iostream>
#include "Entity.h"
#include "Creature.h"

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
};

