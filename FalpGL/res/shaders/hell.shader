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
uniform float chroma;
uniform float hue;
uniform float sat;
uniform float val;


vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float kernel[9] = float[]
(
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
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
    float colorr;
    float colorg;
    float colorb;

    for (int i = 0; i < 9; i++)
    {
        colorr += texture(screenTexture, vec2((v_TexCoord.x - -chroma / 2 / 1080.0f), v_TexCoord.y) + offsets[i]).r * kernel[i];
        colorg += texture(screenTexture, vec2((v_TexCoord.x - 0 / 1080.0f), v_TexCoord.y) + offsets[i]).g * kernel[i];
        colorb += texture(screenTexture, vec2((v_TexCoord.x - chroma / 2 / 1080.0f), v_TexCoord.y) + offsets[i]).b * kernel[i];
    }


    vec4 color = vec4(colorr, colorg, colorb, 1.0f);
    vec3 hsl = rgb2hsv(color.xyz);
    hsl.x += hue / 360.0f;
    hsl.y = hsl.y * (sat / 100.0f);
    hsl.z = hsl.z * (val / 100.0f);
    frag_color = vec4(hsv2rgb(hsl), 1.0);
}
