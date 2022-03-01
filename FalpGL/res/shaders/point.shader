#shader vertex
#version 330 core
layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}


#shader geometry
#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 4) out;

void build_house(vec4 position)
{
    gl_Position = position + vec4(-0.1, -0.1, 0.0, 0.0);    
    EmitVertex();
    gl_Position = position + vec4(0.1, -0.1, 0.0, 0.0);    
    EmitVertex();
    gl_Position = position + vec4(-0.1, 0.1, 0.0, 0.0);    
    EmitVertex();
    gl_Position = position + vec4(0.1, 0.1, 0.0, 0.0);    
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_house(gl_in[0].gl_Position);
}

#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.5, 0.0, 1.0, 1.0);
}