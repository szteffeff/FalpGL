#pragma once

#include <string>
#include <fstream>
#include "../renderer/GraphicsHeaders.h"

class Json_loader {
private:
	std::string animation_filepath = "res/data/animations.json";
	std::string entity_filepath = "res/data/entities.json";
	std::string tile_filepath = "res/data/tiles.json";


public:
	Json_loader();

	json animations;
	json entities;
	json tiles;
};