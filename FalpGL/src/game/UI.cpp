#include "UI.h"


Text::Text(std::string text, float x, float y, Tileset& set)
{
	ttl = 5 * 60;

	x = round(x);
	y = round(y);

	int size = 0;

	for (int i = 0; i < text.size(); i++)
	{

		if (text[i] == *("\n"))
		{
			y -= 8;
			continue;
		}

		Prototype_Tile letter = set[1];

		vertex_data.push_back(x); // x
		vertex_data.push_back(y - 8); // y
		vertex_data.push_back(letter.texture_coord[0]); // u
		vertex_data.push_back(letter.texture_coord[1]); // v

		vertex_data.push_back(x + 8); 
		vertex_data.push_back(y - 8);
		vertex_data.push_back(letter.texture_coord[2]);
		vertex_data.push_back(letter.texture_coord[3]);
		
		vertex_data.push_back(x + 8);
		vertex_data.push_back(y);
		vertex_data.push_back(letter.texture_coord[4]);
		vertex_data.push_back(letter.texture_coord[5]);
		
		vertex_data.push_back(x);
		vertex_data.push_back(y);
		vertex_data.push_back(letter.texture_coord[6]);
		vertex_data.push_back(letter.texture_coord[7]);

		x += 8;

		size++;
	}

	ib.init(size);
	vb.init(size * 16 * sizeof(float));

	VertexBufferLayout vbl;
	vbl.Push<float>(2);
	vbl.Push<float>(2);

	vb.buffer_data(0, vertex_data.size() * sizeof(float), vertex_data.data());

	va.AddBuffer(vb, vbl);
}



void UserInterface::draw_text(glm::mat4 matrix)
{
	glDisable(GL_DEPTH_TEST);

	text_shader.Bind();
	for (Text t : texts)
	{
		t.va.Bind();
		t.ib.Bind();
		glDrawElements(GL_TRIANGLES, t.ib.GetCount(), GL_UNSIGNED_BYTE, nullptr);
	}
	glEnable(GL_DEPTH_TEST);
}


void UserInterface::create_text(std::string text, float x, float y)
{
	texts.emplace_back(text, x, y, text_set);
}


UserInterface::UserInterface(VertexBuffer* buffer)
	: health_Bar(buffer), text_shader("res/shaders/quad.shader"), text_set(std::string("res/data/letters.json"), 12)
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

	int index = 0;
	for (Text t : texts)
	{
		if (t.ttl-- <= 0)
		{
			texts.erase(texts.begin() + index);
		}
		index++;
	}
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
