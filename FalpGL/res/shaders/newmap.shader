#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 0.0f, 0.0f);
	v_TexCoord = texCoord;
	v_Texture = texture_index;
};


#shader geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 v_TexCoord[];
out vec2 f_TexCoord;


void main()
{
	gl_Position = gl_in[0].gl_Position;
	f_TexCoord = v_TexCoord[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	f_TexCoord = v_TexCoord[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	f_TexCoord = v_TexCoord[2];
	EmitVertex();

	EndPrimitive();
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 f_TexCoord;

uniform sampler2D u_Textures[16];
uniform float u_Texture

void main()
{
	int index = int(f_Texture);
	color = texture(u_Textures[u_Texture], f_TexCoord);
}


