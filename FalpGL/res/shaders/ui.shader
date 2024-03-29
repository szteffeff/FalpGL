#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texture_index;

out vec2 v_TexCoord;
out float v_Texture;

uniform mat4 u_MVP;

void main()
{
	gl_Position = vec4(u_MVP * position);
	v_TexCoord = texCoord;
	v_Texture = texture_index;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_Texture;

uniform sampler2D u_Textures[16];

void main()
{
	int index = int(v_Texture);
	color = texture(u_Textures[index], v_TexCoord);
}


