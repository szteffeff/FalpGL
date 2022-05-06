#include "UI.h"


UserInterface::UserInterface(VertexBuffer* buffer)
	: health_Bar(buffer)
{
}


void UserInterface::SetHealth(float* H)
{
	Player_Health = H;
	//std::cout << "Player Health is " << Player_Health << "\n";
}

void UserInterface::SetStamina(float* S)
{
	Player_Stamina = S;
	//std::cout << "Player Stamina is " << Player_Stamina << "\n";
}

void UserInterface::SetPotion(int* P)
{
	Player_Potion = P;
}

void UserInterface::Placeholder_Health(float HP)
{
	//function that sends float to healthbar
	health_Bar.set_health(HP);
	//std::cout << "Player Health is " << HP << "\n";
}

void UserInterface::Placeholder_Stamina(float SP)
{
	//function that sends float to stamina bar
	health_Bar.set_stamina(SP);
	//std::cout << "Player Stamina is " << SP << "\n";
}

void UserInterface::Placeholder_Potion(int P)
{
	//function that thing to stamina
	health_Bar.set_level(Health_Bar::frame_animations(P));

}

void UserInterface::Imma_Keep_It_100()
{
	if (*Player_Health > 100) {
		float bonus_health = *Player_Health - 100;
		*Player_Health -= bonus_health;
	}
	if (*Player_Stamina > 100) {
		float bonus_stamina = *Player_Stamina - 100;
		*Player_Stamina -= bonus_stamina;
	}
}

void UserInterface::UI_Tick()
{
	Placeholder_Health(*Player_Health);
	Placeholder_Stamina(*Player_Stamina);
	Placeholder_Potion(*Player_Potion);
	Imma_Keep_It_100();
	health_Bar.tick();
}

Button::Button(VertexBuffer* buffer, Point _bottom_left, Point size, Point resolution, nlohmann::json loader)
	: button_enitiy(buffer, loader), top_right(0, 0), bottom_left(0, 0)
{
	//

}

button_function Button::pressed(float x, float y)
{
	return button_function();
}
