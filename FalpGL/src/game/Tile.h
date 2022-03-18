#pragma once
#include "../renderer/RendererIncludes.h"
#include "../types/TileTypes.h"
#include "Json.h"
#include <iostream>
#include <algorithm>






struct Old_Tile : public Quad {
private:
	bool solid;

public:
	std::string name;
	Old_Tile();
	Old_Tile(VertexBuffer* vb, Json_loader* loader);
	Old_Tile(VertexBuffer* vb, Json_loader* loader, std::string id);

	void change_type(Tile_id _id, Json_loader* loader);
	bool is_solid() const;
};