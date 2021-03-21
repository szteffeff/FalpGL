#shader vertex
#version 330 core

layout(location = 0) in vec4 size;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texture_index;

out vec2 v_TexCoord;
out float u_Texture;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * size;
   v_TexCoord = texCoord;
   u_Texture = texture_index;
};




#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float u_Texture;

uniform sampler2D u_Textures[2];

void main()
{
	int index = int(u_Texture);
	color = vec4(0.5, 0.5, 1.0, 1.0);
}


