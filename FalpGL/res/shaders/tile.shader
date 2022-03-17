#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 f_TexCoord;
uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0f, 1.0f);
	f_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 f_TexCoord;

uniform sampler2D u_Textures[16];
uniform float u_Texture;

void main()
{
	color = texture(u_Textures[int(u_Texture)], f_TexCoord);
	color = vec4(1.0f);
}