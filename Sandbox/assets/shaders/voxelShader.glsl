#type vertex
#version 450 core

in vec3 a_Position;
in vec3 a_Color;
in int a_ActiveSides;
out vec3 v_Color;
out int v_ActiveSides;

void main()
{
	v_Color = a_Color;
	v_ActiveSides = a_ActiveSides;
	gl_Position = vec4(a_Position, 1.0);
}

#type geometry
#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

in vec3 v_Color[];
in int v_ActiveSides[];
out vec3 g_Color;
out vec4 g_Normal;
out vec3 g_FragPosition;

uniform float u_VoxelSize;
uniform mat4 u_MVP;
uniform vec3 u_CameraPosition;

void AddQuad(vec4 offset, vec4 dy, vec4 dx) {
	g_Color = v_Color[0];
	g_FragPosition = vec3(offset + (dx - dy));
	gl_Position = u_MVP * (offset + (dx - dy));
	EmitVertex();

	g_Color = v_Color[0];
	g_FragPosition = vec3(offset + (-dx - dy));
	gl_Position = u_MVP * (offset + (-dx - dy));
	EmitVertex();

	g_Color = v_Color[0];
	g_FragPosition = vec3(offset + (dx + dy));
	gl_Position = u_MVP * (offset + (dx + dy));
	EmitVertex();

	g_Color = v_Color[0];
	g_FragPosition = vec3(offset + (-dx + dy));
	gl_Position = u_MVP * (offset + (-dx + dy));
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
	//vec4 center = u_MVP * gl_in[0].gl_Position;
	vec4 dx = vec4(1.0, 0.0, 0.0, 0.0) / 2.0 * u_VoxelSize;
	vec4 dy = vec4(0.0, 1.0, 0.0, 0.0) / 2.0 * u_VoxelSize;
	vec4 dz = vec4(0.0, 0.0, 1.0, 0.0) / 2.0 * u_VoxelSize;

	
	if ((v_ActiveSides[0] & 2) != 0 && isVisible(u_CameraPosition.x, center2.x)) {
		g_Normal = dx;
		AddQuad(center2 + dx, dy, dz);
	}
	
	if ((v_ActiveSides[0] & 1) != 0 && !isVisible(u_CameraPosition.x, center2.x)) {
		g_Normal = -dx;
		AddQuad(center2 - dx, dz, dy);
	}

	if ((v_ActiveSides[0] & 8) != 0 && isVisible(u_CameraPosition.y, center2.y)) {
		g_Normal = dy;
		AddQuad(center2 + dy, dz, dx);
	}

	if ((v_ActiveSides[0] & 4) != 0 && !isVisible(u_CameraPosition.y, center2.y)) {
		g_Normal = -dy;
		AddQuad(center2 - dy, dx, dz);
	}

	if ((v_ActiveSides[0] & 32) != 0 && isVisible(u_CameraPosition.z, center2.z)) {
		g_Normal = dz;
		AddQuad(center2 + dz, dx, dy);
	}

	if ((v_ActiveSides[0] & 16) != 0 && !isVisible(u_CameraPosition.z, center2.z)) {
		g_Normal = -dz;
		AddQuad(center2 - dz, dy, dx);
	}
}

#type fragment
#version 450 core

in vec3 g_Color;
in vec4 g_Normal;
in vec3 g_FragPosition;
out vec4 color;

uniform vec3 u_LightPosition;
uniform vec3 u_DiffuseColor;
uniform vec3 u_AmbientColor;

void main()
{
	vec3 normal = vec3(normalize(g_Normal));
	vec3 lightDir = vec3(normalize(u_LightPosition - g_FragPosition));
	float diffuse = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * u_DiffuseColor;
	vec3 result = (diffuseColor + u_AmbientColor) * g_Color;
	color = vec4(result, 1.0);
}