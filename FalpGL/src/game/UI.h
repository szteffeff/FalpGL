#pragma once
#include "../renderer/RendererIncludes.h"
#include "../types/TileTypes.h"
#include "BatchRenderer.h" 
#include "Json.h"
#include "Tile.h"
#include <iostream>
#include "Entity.h"
#include "Creature.h"

//health . ewapons, 
class UserInterface {
	float* Player_Health;
	Health_Bar hb;

public:
	UserInterface();
	void SetHealth(float* H);
};
