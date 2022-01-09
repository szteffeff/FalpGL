#pragma once
#include "OverMap.h"
#include "Entitiy.h"


struct key {
	char m_key = 0;
	bool held = false;
};


class Input
{
public:
	Input();
	~Input();

	void set_keepalive(bool* keepalive);
	void set_player(Entity* p);

	void recive(int key, int scancode, int action, int mods);
	void tick();

private:

	std::vector<key> keys;

	Entity* player;
	bool* running;
	const char alphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
};
