#pragma once
#include "../renderer/RendererIncludes.h"
#include "../types/TileTypes.h"
#include "Json.h"
#include <iostream>
#include <algorithm>






struct Tile : public Quad {
private:
	bool solid;

public:
	std::string name;
	Tile();
	Tile(VertexBuffer* vb, Json_loader* loader);
	Tile(VertexBuffer* vb, Json_loader* loader, std::string id);

	void change_type(Tile_id _id, Json_loader* loader);
	bool is_solid() const;
};