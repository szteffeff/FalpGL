#include "Tile.h"


Tile::Tile()
{}

Tile::Tile(VertexBuffer* vb, Json_loader* loader)
	: Quad(vb, 32, 32, 0.0f)
{
	tex_coord tx;

	tx.coords[0][0] = (loader->tiles["0"]["texture_coordinants"][0][0]) / 2048.0f; /* Denominator must be float or intiger division happens */
	tx.coords[0][1] = (loader->tiles["0"]["texture_coordinants"][0][1]) / 2048.0f;

	tx.coords[1][0] = (loader->tiles["0"]["texture_coordinants"][1][0]) / 2048.0f;
	tx.coords[1][1] = (loader->tiles["0"]["texture_coordinants"][1][1]) / 2048.0f;

	tx.coords[2][0] = (loader->tiles["0"]["texture_coordinants"][2][0]) / 2048.0f;
	tx.coords[2][1] = (loader->tiles["0"]["texture_coordinants"][2][1]) / 2048.0f;

	tx.coords[3][0] = (loader->tiles["0"]["texture_coordinants"][3][0]) / 2048.0f;
	tx.coords[3][1] = (loader->tiles["0"]["texture_coordinants"][3][1]) / 2048.0f;

	set_texture_coords(tx);
	set_z(-0.999);
}

Tile::Tile(VertexBuffer* vb, Json_loader* loader, std::string id)
	: Quad(vb, 32, 32, 0.0f)
{
	tex_coord tx;

	tx.coords[0][0] = (loader->tiles[id]["texture_coordinants"][0][0]) / 2048.0f; /* Denominator must be float or intiger division happens */
	tx.coords[0][1] = (loader->tiles[id]["texture_coordinants"][0][1]) / 2048.0f;

	tx.coords[1][0] = (loader->tiles[id]["texture_coordinants"][1][0]) / 2048.0f;
	tx.coords[1][1] = (loader->tiles[id]["texture_coordinants"][1][1]) / 2048.0f;

	tx.coords[2][0] = (loader->tiles[id]["texture_coordinants"][2][0]) / 2048.0f;
	tx.coords[2][1] = (loader->tiles[id]["texture_coordinants"][2][1]) / 2048.0f;

	tx.coords[3][0] = (loader->tiles[id]["texture_coordinants"][3][0]) / 2048.0f;
	tx.coords[3][1] = (loader->tiles[id]["texture_coordinants"][3][1]) / 2048.0f;

	set_texture_coords(tx);
	set_z(-0.9);
}


void Tile::change_type(Tile_id id, Json_loader* loader)
{
	tex_coord tx;
	std::string _id = std::to_string((int)id);

	tx.coords[0][0] = (loader->tiles[_id]["texture_coordinants"][0][0]) / 2048.0f; /* Denominator must be float or intiger division happens */
	tx.coords[0][1] = (loader->tiles[_id]["texture_coordinants"][0][1]) / 2048.0f;

	tx.coords[1][0] = (loader->tiles[_id]["texture_coordinants"][1][0]) / 2048.0f;
	tx.coords[1][1] = (loader->tiles[_id]["texture_coordinants"][1][1]) / 2048.0f;

	tx.coords[2][0] = (loader->tiles[_id]["texture_coordinants"][2][0]) / 2048.0f;
	tx.coords[2][1] = (loader->tiles[_id]["texture_coordinants"][2][1]) / 2048.0f;

	tx.coords[3][0] = (loader->tiles[_id]["texture_coordinants"][3][0]) / 2048.0f;
	tx.coords[3][1] = (loader->tiles[_id]["texture_coordinants"][3][1]) / 2048.0f;

	set_texture_coords(tx);
}