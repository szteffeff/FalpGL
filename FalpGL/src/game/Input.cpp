#include "Input.h"
#include <iostream>

Input::Input()
{
	keys.resize(4);
}

Input::~Input()
{
}

void Input::set_keepalive(bool* keepalive)
{
	running = keepalive;
}

void Input::set_player(Entity* p)
{
	player = p;
}

void Input::set_map(Map* m)
{
	map = m;
}

void Input::recive(int key, int scancode, int action, int mods) // release = 0, press = 1, repeat = 2
{
	if (action == 2) { return; }

	switch (key)
	{
	case(GLFW_KEY_W):
		keys[0].held = action;
		break;

	case(GLFW_KEY_A):
		keys[1].held = action;
		break;

	case(GLFW_KEY_S):
		keys[2].held = action;
		break;

	case(GLFW_KEY_D):
		keys[3].held = action;
		break;

	case(GLFW_KEY_ESCAPE):
	case(GLFW_KEY_Q):
		*running = false;
		std::cout << "stopping\n";
		break;

	default:
		break;
	}
}

void Input::tick()
{
	if (keys[0].held) {
		player->walk(90, 1);
	}

	if (keys[1].held) {
		player->walk(180, 1);
	}

	if (keys[2].held) {
		player->walk(270, 1);
	}

	if (keys[3].held) {
		player->walk(0, 1);
	}

	if (!(keys[0].held || keys[1].held || keys[2].held || keys[3].held))
	{
	}
}