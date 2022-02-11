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
	Entity PlayerHealthbar;

public:
	UserInterface();
	void SetHealth(float* H);
};
