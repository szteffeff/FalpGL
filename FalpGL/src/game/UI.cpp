#include "UI.h"

UserInterface::UserInterface()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UserInterface::SetHealth(float* H)
{
	Player_Health = H;
}

void UserInterface::SetStamina(float* S)
{
	Player_Stamina = S;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserInterface::Placeholder_Health(float HP)
{
	//function that sends float to healthbar
	//std::cout << "Player Health is " << HP << "\n";
}

void UserInterface::Placeholder_Stamina(float SP)
{
	//function that sends float to stamina bar
	//std::cout << "Player Stamina is " << SP << "\n";
}


void UserInterface::UI_Tick()
{
	Placeholder_Health(*Player_Health);
	Placeholder_Stamina(*Player_Stamina);
}




