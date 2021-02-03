#type vertex
#version 450 core

in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
}

#type geometry
#version 450 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

void main()
{
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}

#type fragment
#version 450 core

out vec4 color;

void main()
{
    color = vec4(0.9, 0.9, 0.6, 1.0);
}