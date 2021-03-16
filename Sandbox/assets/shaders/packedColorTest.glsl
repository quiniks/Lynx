#type vertex
#version 450 core

in vec3 a_Position;
in vec3 a_Color;
out vec3 v_Color;

uniform mat4 u_MVP;

void main()
{
    v_Color = a_Color;
    gl_Position = u_MVP * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

in vec3 v_Color;
out vec4 color;

void main()
{
    color = vec4(v_Color, 1.0f);
}