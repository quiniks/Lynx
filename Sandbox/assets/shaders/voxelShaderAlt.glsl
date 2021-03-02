#type vertex
#version 450 core

in vec3 a_Position;
in vec3 a_Color;
in vec3 a_Normal;
out vec3 v_Color;
out vec3 v_Normal;
out vec3 v_FragPosition;

uniform mat4 u_MVP;

void main()
{
	v_Color = a_Color;
	v_Normal = a_Normal;
	v_FragPosition = a_Position;
	gl_Position = u_MVP * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

in vec3 v_Color;
in vec3 v_Normal;
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
	vec3 result = (diffuseColor + u_AmbientColor) * v_Color;
	color = vec4(result, 1.0);
}