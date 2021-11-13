#pragma once
#include "OverMap.h"

class Input
{
public:
	Input();
	~Input();

	void set_keepalive(bool* keepalive);
	void set_tile(Tile* test);

	void recive(int key, int scancode, int action, int mods);

private:
	Tile* example;
	bool* running;
	const char alphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
};
