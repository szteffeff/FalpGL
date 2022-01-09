#include "Player.h"
#include "../renderer/VertexBuffer.h"

Player::Player(VertexBuffer& vb)
	: x(0), y(0), quad_data(), idle(Texture("res/textures/boy_idle.png")), quad(&vb, 0.0f, 0.0f, 0)
{
	update_quad();
	quad.replace_data(&quad_data);
	quad.update();
}

void Player::move(int delta_x, int delta_y)
{
	x += delta_x;
	y += delta_y;
}

void Player::blit(VertexBuffer &vb)
{
	update_quad();
	vb.add_quad(quad_data);
}

void Player::TBind()
{
	idle.Bind();
}

void Player::update_quad()
{
	quad_data[0] = 24 / -2.0f * 6.0f; 
	quad_data[1] = 64 / -2.0f * 6.0f;
	quad_data[2] = 0.5f;

	quad_data[6] = 24 / 2.0f * 6.0f;
	quad_data[7] = 64 / -2.0f * 6.0f;
	quad_data[8] = 0.5f;

	quad_data[12] = 24 / 2.0f * 6.0f;
	quad_data[13] = 64 / 2.0f * 6.0f;
	quad_data[14] = 0.5f;

	quad_data[18] = 24 / -2.0f * 6.0f;
	quad_data[19] = 64 / 2.0f * 6.0f;
	quad_data[20] = 0.5f;


	quad_data[3] = 0.0f;
	quad_data[4] = 0.0f;

	quad_data[9] = 1.0f;
	quad_data[10] = 0.0f;

	quad_data[15] = 1.0f;
	quad_data[16] = 0.25f;

	quad_data[21] = 0.0f;
	quad_data[22] = 0.25f;


	quad_data[5] = 0.0f;
	quad_data[11] = 0.0f;
	quad_data[17] = 0.0f;
	quad_data[23] = 0.0f;
}

/* NEW
s  s  y  t  t  i
00 01 02 03 04 05
06 07 08 09 10 11
12 13 14 15 16 17
18 19 20 21 22 23
*/

