#include "Text.h"

Text::Text(std::string text, float x, float y, int time, Tileset& set)
	: count(0), ttl(time)
{

	float original_x = round(x);

	x = round(x);
	y = round(y);


	for (int i = 0; i < text.size(); i++)
	{
		float shift_y = 0;
		float shift_x = 0;

		if (text[i] == *("\n"))
		{
			y -= size + 4;
			x = original_x;
			continue;
		}

		if (text[i] == *(" "))
		{
			x += size;
			continue;
		}

		if (text[i] == *(","))
		{
			text[i] = 123;
			shift_y = -4;
			shift_x = -3;
		}

		if (text[i] == *("!"))
		{
			text[i] = 124;
			shift_x = -3;
		}

		if (text[i] == *("."))
		{
			text[i] = 125;
			shift_x = -3;
		}

		if (text[i] == *("?"))
		{
			text[i] = 126;
			shift_x = -3;
		}

		if (text[i] == *("’") || text[i] == *("'"))
		{
			text[i] = 127;
			shift_y = 4;
		}

		if (text[i] < 91 && text[i] > 64)
		{
			text[i] += 32;
		}

		Prototype_Tile letter = set[text[i] - 97];

		vertex_data.push_back(x + shift_x); // x
		vertex_data.push_back(y - size + shift_y); // y
		vertex_data.push_back(letter.texture_coord[0]); // u
		vertex_data.push_back(letter.texture_coord[1]); // v

		vertex_data.push_back(x + size + shift_x);
		vertex_data.push_back(y - size + shift_y);
		vertex_data.push_back(letter.texture_coord[2]);
		vertex_data.push_back(letter.texture_coord[3]);

		vertex_data.push_back(x + size + shift_x);
		vertex_data.push_back(y + shift_y);
		vertex_data.push_back(letter.texture_coord[4]);
		vertex_data.push_back(letter.texture_coord[5]);

		vertex_data.push_back(x + shift_x);
		vertex_data.push_back(y + shift_y);
		vertex_data.push_back(letter.texture_coord[6]);
		vertex_data.push_back(letter.texture_coord[7]);

		x += size;

		count++;
	}

	ib.init(count);
	vb.init(count * 16 * sizeof(float));

	VertexBufferLayout vbl;
	vbl.Push<float>(2);
	vbl.Push<float>(2);

	va.AddBuffer(vb, vbl);

	vb.buffer_data(0, vertex_data.size() * sizeof(float), vertex_data.data());

}

void Text_Renderer::create_text(std::string text, float x, float y, int time)
{
	texts.emplace_back(new Text(text, x, y, time, text_set));
}


void Text_Renderer::draw_text(glm::mat4 matrix)
{
	glDisable(GL_DEPTH_TEST);

	text_shader.Bind();
	text_shader.SetUniform1i("u_Texture", 12);
	text_shader.SetUniformMat4f("u_MVP", matrix);

	for (Text* t : texts)
	{
		t->va.Bind();
		t->ib.Bind();
		glDrawElements(GL_TRIANGLES, t->ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	glEnable(GL_DEPTH_TEST);
}

void Text_Renderer::tick()
{
	int index = 0;
	for (Text* t : texts)
	{
		if (t->ttl-- <= 0)
		{
			delete t;
			texts.erase(texts.begin() + index);
			index--;
		}
		index++;
	}
}

Text_Renderer::Text_Renderer()
	: text_shader("res/shaders/letter.shader"), text_set(std::string("res/data/letters.json"), 12)
{ }