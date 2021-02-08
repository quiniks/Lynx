#type vertex
#version 450 core

in vec2 a_Position;
in int a_Test;
out int v_Test;

void main()
{
	v_Test = a_Test;
	gl_Position = vec4(a_Position, 0.0, 1.0);
}

#type geometry
#version 450 core

layout(points) in;
in int v_Test[1];
layout(line_strip, max_vertices = 2) out;

void main()
{
	if (v_Test[0] >= 3) {
		gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
		EmitVertex();
	
		gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
		EmitVertex();

		EndPrimitive();
	}
}

#type fragment
#version 450 core

out vec4 color;

void main()
{
	color = vec4(0.9, 0.9, 0.6, 1.0);
}