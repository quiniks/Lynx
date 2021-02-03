#type vertex
#version 450 core

in vec2 a_Position;
in vec2 a_Texel;
out vec2 v_Texel;
void main()
{
	v_Texel = a_Texel;
	gl_Position = vec4(a_Position, 0.0, 1.0);
}

#type fragment
#version 450 core

in vec2 v_Texel;
out vec4 color;

const float texelSep = 0.005;
const int radialCopies = 3;

uniform sampler2D u_TextureFramebuffer;
void main()
{
	vec4 sum = vec4(0.0);
	for (int x = -radialCopies; x <= radialCopies; x++)
		for (int y = -radialCopies; y <= radialCopies; y++)
			sum += texture(u_TextureFramebuffer, vec2(v_Texel.x + x * texelSep, v_Texel.y + y * texelSep)) / pow(radialCopies * 2 + 1.0f, 2.0f);
	color = sum;
}