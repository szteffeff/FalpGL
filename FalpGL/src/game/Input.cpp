#include "Input.h"
#include <iostream>

Input::Input()
{

}

Input::~Input()
{
}

void Input::set_keepalive(bool* keepalive)
{
	running = keepalive;
}

void Input::set_tile(Tile* test)
{
	example = test;
}

void Input::recive(int key, int scancode, int action, int mods) // release = 0, press = 1, repeat = 2
{
	if (key >= 65 && key <= 90)
	{
		if (action == 1)
			std::cout << "key " << alphabet[key - 65] << " was pressed with code " << key << "\n";
		else if (action == 0)
			std::cout << "key " << alphabet[key - 65] << " was released\n";
	}

	if (key == 87 && action == 2)
		example->move(0, 1);

	if (key == 65 && action == 2)
		example->move(-1, 0);

	if (key == 83 && action == 2)
		example->move(0, -1);

	if (key == 68 && action == 2)
		example->move(1, 0);



	if (key == 256)
		*running = false;
}
