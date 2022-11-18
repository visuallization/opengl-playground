#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragmentPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
   gl_Position = u_Projection * u_View * u_Model * position;
   FragmentPosition = vec3(u_Model * position);
   Normal = normal;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec3 Normal;
in vec3 FragmentPosition;

uniform vec4 u_Color;
uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;
uniform float u_AmbientStrength;

void main() {
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(u_LightPosition - FragmentPosition);

	float diffuseImpact = max(dot(normal, lightDirection), 0.0);
	vec4 diffuse = diffuseImpact * u_LightColor;

	vec4 ambient = vec4(u_LightColor.xyz * u_AmbientStrength, 1.0);

	color = (ambient + diffuse) * u_Color;
};