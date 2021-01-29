#type vertex
#version 450 core

in vec3 a_Position;
in vec2 a_Texel;
out vec2 v_Texel;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_Texel = a_Texel;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

in vec2 v_Texel;

uniform vec4 u_Tint;
uniform sampler2D u_Texture;
uniform sampler2D u_Texture2;

layout(location = 0) out vec4 color;

void main()
{
    vec4 textureColor = mix(texture(u_Texture, v_Texel), texture(u_Texture2, v_Texel), 0.5);
    color = textureColor * u_Tint;
}