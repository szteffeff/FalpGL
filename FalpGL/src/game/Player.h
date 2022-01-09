#pragma once

#include <vector>

#include "Animation.h"
#include "BatchRenderer.h"



class Player 
{
public:
	Player(VertexBuffer& vb);

	void move(int delta_x, int delta_y);

	void blit(VertexBuffer& vb);

	Texture idle;

	void TBind();

	Quad quad;

private:

	int x, y;

	float quad_data[24];

	void update_quad();
};