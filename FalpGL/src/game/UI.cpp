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


void UserInterface::UI_Tick()
{
	Placeholder_Health(*Player_Health);
	Placeholder_Stamina(*Player_Stamina);
	Placeholder_Potion(*Player_Potion);
	health_Bar.tick();
}




