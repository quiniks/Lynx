#type vertex
#version 450 core

in vec3 a_Position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

uniform vec4 u_Tint;

layout(location = 0) out vec4 color;

void main()
{
    color = u_Tint;
}