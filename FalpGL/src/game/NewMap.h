#pragma once
#include "../renderer/BatchRenderer.h"
#include "../renderer/Point.h"

/*
*  objects depth varies in chunk, resets every chunk
*  when player(or other thing) goes up or down, snap depth to 0 or 1
*  no need to change depth for y
*  bad clipping at edge of chunk? yes.
* 
* 
*  change map object depth in vetex shader to y:
* 
*       1: things that need to occlude thing
*       2: map
*                      1    2
*                      \     |
*                       \    |
*    front of screen     \   |       back of screen
*                         \  |
*                          \ |
*                           \|
* 
*      y2 = lowest y coord for every quad
*/
class new_map {
private:
	BatchRenderer map_renderer;
	const int chunk_size = 64;

	bool test_collision_map[4][4] = {
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	};

public:
	void draw(glm::mat4 matrix);
	Point collision_line_desination(Point origin, Point desination, float collision_radius);
	Point collision_line_delta(Point origin, Point delta, float collision_radius);

};


class decoration {
	/*
	* for things that you can walk behind but don't interact with
	*/
};

class placeholder_active_object {
	/*
	* for things that you can walk behind and interact with
	*/
};