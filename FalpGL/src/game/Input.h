#pragma once
#include "Entity.h"
#include "Creature.h"

struct key {
	char m_key = 0;
	bool held = false;
};


class Input
{
public:
	Input();
	~Input();

	void set_pause(bool* pause);
	void set_keepalive(bool* keepalive);
	void set_player(Player* p);
	void set_matrix(glm::mat4* m);
	void set_height_width(float w, float h);

	void keys_recive(int key, int scancode, int action, int mods);
	void mouse_recive(int button, int action, int mods);

	void tick(float mx, float my);

private:

	std::vector<key> keys;

	float mouse_x, mouse_y;
	float window_width, window_height;

	Player* player;
	glm::mat4* zoom_matrix;

	bool* running;
	bool* pause_menu;
	const char alphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
};
