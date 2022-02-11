#pragma once
#include "../renderer/RendererIncludes.h"
#include "../types/TileTypes.h"
#include "BatchRenderer.h" 
#include "Json.h"
#include "Tile.h"
#include <iostream>
#include "Entitiy.h"

//health . ewapons, 
class UserInterface {
	float* Player_Health;
	float* Player_Stamina;
	Entity PlayerHealthbar;
	Entity PlayerStaminaBar;

public:
	UserInterface();
	void SetHealth(float* H);
	void SetStamina(float* S);
	void UI_Tick();
	void Placeholder_Health(float HP);
	void Placeholder_Stamina(float SP);

};

