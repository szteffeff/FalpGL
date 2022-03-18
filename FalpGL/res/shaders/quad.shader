#shader vertex
#version 330 core

layout(location = 0) in vec2 size;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(size.xy, 0.0f, 1.0f);
	v_TexCoord = texCoord;
};




#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
	color = vec4(color.rgb, 0.5f);
}


