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

void main()
{
    vec4 color = texture(screenTexture, v_TexCoord);
    vec3 hsl = rgb2hsv(color.xyz);
    hsl.x += hue / 360.0f;
    hsl.y = hsl.y * (sat  / 100.0f);
    hsl.z = hsl.z * (val / 100.0f);
    frag_color = vec4(hsv2rgb(hsl), 1.0);

    float x = abs((v_TexCoord.x - 0.5) * 2);
    float y = abs((v_TexCoord.y - 0.5) * 2);

    float scale = 1.5;
    float intensity = 0.5;

    frag_color.rgb *= clamp(vec3(1 - sqrt(x * x + y * y)) + scale - 1, 0, 1);
}
