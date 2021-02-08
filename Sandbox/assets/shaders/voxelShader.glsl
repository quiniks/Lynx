#type vertex
#version 450 core

in vec3 a_Position;
in vec3 a_Color;
in int a_EnabledSides;
out vec3 v_Color;
out int v_EnabledSides;

void main()
{
	v_Color = a_Color;
	v_EnabledSides = a_EnabledSides;
	gl_Position = vec4(a_Position, 1.0);
}

#type geometry
#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

in vec3 v_Color[];
in int v_EnabledSides[];
out vec3 g_Color;

uniform float u_VoxelSize;
uniform mat4 u_MVP;
uniform vec3 u_CameraPosition;

void AddQuad(vec4 center, vec4 dy, vec4 dx) {
	g_Color = v_Color[0];
	gl_Position = center + (dx - dy);
	EmitVertex();

	g_Color = v_Color[0];
	gl_Position = center + (-dx - dy);
	EmitVertex();

	g_Color = v_Color[0];
	gl_Position = center + (dx + dy);
	EmitVertex();

	g_Color = v_Color[0];
	gl_Position = center + (-dx + dy);
	EmitVertex();

	EndPrimitive();
}

bool isVisible(float camera, float voxel) {
	if (camera > voxel) {
		return true;
	}
	return false;
}

void main()
{
	vec4 center2 = gl_in[0].gl_Position;
	//
	vec4 center = u_MVP * gl_in[0].gl_Position;
	vec4 dx = u_MVP[0] / 2.0 * u_VoxelSize;
	vec4 dy = u_MVP[1] / 2.0 * u_VoxelSize;
	vec4 dz = u_MVP[2] / 2.0 * u_VoxelSize;
	
	if ((v_EnabledSides[0] & 1) != 0 && isVisible(u_CameraPosition.x, center2.x))
		AddQuad(center + dx, dy, dz);
	
	if ((v_EnabledSides[0] & 2) != 0 && !isVisible(u_CameraPosition.x, center2.x))
		AddQuad(center - dx, dz, dy);

	if ((v_EnabledSides[0] & 4) != 0 && isVisible(u_CameraPosition.y, center2.y))
		AddQuad(center + dy, dz, dx);

	if ((v_EnabledSides[0] & 8) != 0 && !isVisible(u_CameraPosition.y, center2.y))
		AddQuad(center - dy, dx, dz);

	if ((v_EnabledSides[0] & 16) != 0 && isVisible(u_CameraPosition.z, center2.z))
		AddQuad(center + dz, dx, dy);

	if ((v_EnabledSides[0] & 32) != 0 && !isVisible(u_CameraPosition.z, center2.z))
		AddQuad(center - dz, dy, dx);
}

#type fragment
#version 450 core

in vec3 g_Color;
out vec4 color;

void main()
{
	color = vec4(g_Color, 1.0);
}