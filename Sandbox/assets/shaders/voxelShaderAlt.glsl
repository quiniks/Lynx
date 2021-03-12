#type vertex
#version 450 core

in vec3 a_Position;
in vec3 a_Color;
in int a_Side;
in int a_AO;
out vec3 v_Color;
out vec3 v_Normal;
out vec3 v_AO;
out vec3 v_FragPosition;

uniform mat4 u_MVP;

void main()
{
	if (a_AO == 0)
		v_AO = vec3(1.0, 1.0, 1.0);
	else if (a_AO == 1)
		v_AO = vec3(0.9, 0.9, 0.9);
	else if (a_AO == 2)
		v_AO = vec3(0.7, 0.7, 0.7);
	else if (a_AO == 5)
		v_AO = vec3(0.5, 0.5, 0.5);

	if (a_Side == 2)
		v_Normal = vec3(1.0, 0.0, 0.0);
	else if (a_Side == 1)
		v_Normal = vec3(-1.0, 0.0, 0.0);
	else if (a_Side == 8)
		v_Normal = vec3(0.0, 1.0, 0.0);
	else if (a_Side == 4)
		v_Normal = vec3(0.0, -1.0, 0.0);
	else if (a_Side == 32)
		v_Normal = vec3(0.0, 0.0, 1.0);
	else if (a_Side == 16)
		v_Normal = vec3(0.0, 0.0, -1.0);

	v_Color = a_Color;
	v_FragPosition = a_Position;
	gl_Position = u_MVP * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

in vec3 v_Color;
in vec3 v_Normal;
in vec3 v_AO;
in vec3 v_FragPosition;
out vec4 color;

uniform vec3 u_LightPosition;
uniform vec3 u_DiffuseColor;
uniform vec3 u_AmbientColor;

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = vec3(normalize(u_LightPosition - v_FragPosition));
	float diffuse = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * u_DiffuseColor;
	vec3 result = (diffuseColor + u_AmbientColor) * v_AO * v_Color;
	color = vec4(result, 1.0);
}