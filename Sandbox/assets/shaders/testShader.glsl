#type vertex
#version 450 core

in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}

#type fragment
#version 450 core

uniform vec3 triangleColor;

out vec4 color;

void main()
{
    color = vec4(triangleColor, 1.0);
}