#include "Input.h"
#include <iostream>

Input::Input()
{
	keys.resize(10);
}

Input::~Input()
{
}

void Input::set_pause(bool* pause)
{
	pause_menu = pause;
}

void Input::set_keepalive(bool* keepalive)
{
	running = keepalive;
}

void Input::set_player(Player* p)
{
	player = p;
}

void Input::set_matrix(glm::mat4* m)
{
	zoom_matrix = m;
}

void Input::set_height_width(float w, float h)
{
	window_height = h;
	window_width = w;
}

void Input::keys_recive(int key, int scancode, int action, int mods) /* release = 0, press = 1, repeat = 2 */
{
	if (action == 2) { return; }

	/* Triggers on press and release (twice) */
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

	case(GLFW_KEY_Q):
		*running = false;
		console_log("[INFO]: Stopping");
		break;

	case(GLFW_KEY_LEFT_SHIFT):
		keys[4].held = action;
		break;

	default:
		break;
	}

	if (action == 0) { return; }

	/* Triggers on press (once) */
	switch (key)
	{
	case(GLFW_KEY_0):
		*zoom_matrix = glm::scale(*zoom_matrix, glm::vec3(2.0f, 2.0f, 1.0f));
		break;

	case(GLFW_KEY_1):
		weapon = 1;
		break;

	case(GLFW_KEY_2):
		weapon = 2;
		break;

	case(GLFW_KEY_3):
		weapon = 3;
		break;

	case(GLFW_KEY_9):
		*zoom_matrix = glm::scale(*zoom_matrix, glm::vec3(0.5f, 0.5f, 1.0f));
		break;

	case(GLFW_KEY_ESCAPE):
		*pause_menu = !*pause_menu;
		std::cout << "Pausing\n";
		break;

	case(GLFW_KEY_O):
		player->Take_Damage();
		break;

	case(GLFW_KEY_P):
		player->Take_Heal();
		break;
	


	default:
		break;
	}

}

void Input::mouse_recive(int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{ 
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (weapon == 1 and player->shwoop_doop() == true) {
				player->Shoot_bow();
			}
			else if (weapon == 2) {
				player->Dagger_light();
			}
			else if (weapon == 3) {
				player->Axe_Light();
			}
			return;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (weapon == 1 and player->shwoop_doop() == true) {
				player->Shoot_bow();
			}
			else if (weapon == 2) {
				player->Dagger_Heavy();
			}
			else if (weapon == 3) {
				player->Axe_Heavy();
			}
			return;
		}
	}

	if (mouse_x > 0)
	{
		//*pause_menu = false;
	}

	if (mouse_x < 0)
	{
		//*pause_menu = true;
	}
}

void Input::tick(float mx, float my)
{
	mouse_x = mx;
	mouse_y = my;

	if (*pause_menu == true) {
		return;
	}

	if (keys[4].held) {
		if (keys[0].held) {
			player->sprint(90, 1);
		}

		if (keys[1].held) {
			player->sprint(180, 1);
		}

		if (keys[2].held) {
			player->sprint(270, 1);
		}

		if (keys[3].held) {
			player->sprint(0, 1);
		}
	}

	else {
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
	}
	
	if (!(keys[0].held || keys[1].held || keys[2].held || keys[3].held))
	{
	}
}