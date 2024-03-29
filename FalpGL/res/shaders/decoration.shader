#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float opacity;

out vec2 v_TexCoord;
out float v_Opacity;

uniform mat4 u_MVP;

void main()
{
	gl_Position = vec4(u_MVP * position);
	v_TexCoord = texCoord;
	v_Opacity = opacity;
};


#shader geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 v_TexCoord[];
out vec2 f_TexCoord;

in float v_Opacity[];
out float f_Opacity;


void main()
{
	float d0 = gl_in[0].gl_Position.y;
	float d1 = gl_in[1].gl_Position.y;
	float d2 = gl_in[2].gl_Position.y;

	float depth = clamp(((d0 < d1 ? d0 : d1) < d2 ? (d0 < d1 ? d0 : d1) : d2), -1.0f, 1.0f);

	gl_Position = vec4(gl_in[0].gl_Position.xy, depth, gl_in[0].gl_Position.w);
	f_TexCoord = v_TexCoord[0];
	f_Opacity = v_Opacity[0];
	EmitVertex();

	gl_Position = vec4(gl_in[1].gl_Position.xy, depth, gl_in[1].gl_Position.w);
	f_TexCoord = v_TexCoord[1];
	f_Opacity = v_Opacity[1];
	EmitVertex();

	gl_Position = vec4(gl_in[2].gl_Position.xy, depth, gl_in[2].gl_Position.w);
	f_TexCoord = v_TexCoord[2];
	f_Opacity = v_Opacity[2];
	EmitVertex();

	EndPrimitive();
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 f_TexCoord;
in float f_Opacity;

uniform sampler2D u_Textures[16];
uniform float u_Texture;

void main()
{
	int index = int(u_Texture);
	color = texture(u_Textures[index], f_TexCoord);
	if (color.a == 0)
		discard;
	color.a = f_Opacity;
}


