#shader vertex
#version 330 core

layout(location = 0) in vec4 size;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texture_index;

out vec2 v_TexCoord;
out float u_Texture;

uniform mat4 u_MVP;
uniform mat4 u_transform;

void main()
{
   gl_Position = u_MVP * u_transform * size;
   v_TexCoord = texCoord;
   u_Texture = texture_index;
};




#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float u_Texture;

uniform sampler2D u_Textures[3];

void main()
{
	int index = int(u_Texture);
	color = texture(u_Textures[index], v_TexCoord);
}


