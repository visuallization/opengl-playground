#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
   gl_Position = u_Projection * u_View * u_Model * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform vec4 u_LightColor;
uniform float u_AmbientStrength;

void main() {
	vec4 ambient = vec4(u_LightColor.xyz * u_AmbientStrength, 1.0);
	color = u_Color * ambient;
};