#pragma once
#include "Entitiy.h"
#include "Map.h"

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
	void set_player(Sub_Entity* p);
	void set_matrix(glm::mat4* m);

	void recive(int key, int scancode, int action, int mods);
	void tick();

private:

	std::vector<key> keys;

	Sub_Entity* player;
	Map* map;
	glm::mat4* zoom_matrix;

	bool* running;
	const char alphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
};
