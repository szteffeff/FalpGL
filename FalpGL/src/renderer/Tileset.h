#pragma once
#include "Framebuffer.h"
#include "../vendor/nlohmann_json/json.hpp"
#include "Texture.h"
#include <fstream>
#include <math.h>



/* Bounding box and offset */
struct Collision_Box {
private:
	float offset[2];
	float size[2];

	bool valid;

public:
	Collision_Box(float offset_x, float offset_y, float size_x, float size_y);

	bool collides_at(float x, float y);
	bool collision_circle(float x, float y, float radius);
};



/* Contains data needed to construct specific tiles */
struct Prototype_Tile {
	int size_x, size_y;

	/* Texture coordinants of tile's texture on an atlas */
	float texture_coord[8];

	/* Unique id */
	float id;

	/* Filepath to texture's image file */
	std::string filepath;

	/* Vector of collision boxes*/
	std::vector<Collision_Box> collisions;

	bool collides(float x, float y);
	bool collision_circle(float x, float y, float radius);
	bool has_collision();

	Prototype_Tile(float in_id, std::string image, float tex_origin[2], float atlas_size, std::vector<Collision_Box> boxes, int size_x = 32, int size_y = 32);
	Prototype_Tile();
};



class Tileset {
protected:
	/* OpenGL objects */
	Shader shader;
	VertexArray vertex_array;
	VertexBuffer vertex_buffer;
	IndexBuffer index_buffer;

	/* OpenGL ids*/
	GLuint gl_framebuffer_id, gl_texture_id, gl_renderbuffer_id;

	/* Side length of atlas, Maximum number of tiles that can fit on atlas */
	unsigned int size, max_tiles;

	/* Tilset json object and filepath */
	nlohmann::json tileset_json;
	std::string tileset_filepath;

	/* Vector of constructed prototype tiles */
	std::vector<Prototype_Tile> tileset_tiles;


	/* Render tile to atlas */
	virtual void stitch_tile(Prototype_Tile tile_to_stich);

	bool loaded;
	unsigned int active_texture_unit;
	virtual void create_atlas();

	size_t first_gid;

public:
	Tileset(std::string Tileset_path, int texture_unit);
	Tileset(nlohmann::json set_json, int texture_unit);
	Tileset();

	virtual void init(nlohmann::json set_json, int texture_unit);
	virtual void init(std::string Tileset_path, int texture_unit);

	/* Bind framebuffer's texture to specific texture unit */
	void bind_texture(unsigned int unit);

	/* Array-like access to prototype tiles */
	Prototype_Tile& operator[](int index);
};
