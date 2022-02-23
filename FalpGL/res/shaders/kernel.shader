#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(position, 1.0, 1.0);
	v_TexCoord = texCoord;
};




#shader fragment
#version 330 core

layout(location = 0) out vec4 frag_color;

in vec2 v_TexCoord;

uniform sampler2D screenTexture;
uniform float resolution_x;
uniform float resolution_y;

float kernel[9] = float[]
(
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
    );


float offset_x = 1.0f / resolution_x;
float offset_y = 1.0f / resolution_y;


vec2 offsets[9] = vec2[]
(
    vec2(-offset_x, offset_y), vec2(0.0f, offset_y), vec2(offset_x, offset_y),
    vec2(-offset_x, 0.0f), vec2(0.0f, 0.0f), vec2(offset_x, 0.0f),
    vec2(-offset_x, -offset_y), vec2(0.0f, -offset_y), vec2(offset_x, -offset_y)
    );


void main()
{
    vec3 color;

    for (int i = 0; i < 9; i++)
    {
        color += texture(screenTexture, v_TexCoord. + offsets[i]) * kernel[i];
    }

    frag_color = vec4(color, 1.0);
}
