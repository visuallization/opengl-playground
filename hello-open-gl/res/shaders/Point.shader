#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform float u_PointSize;

void main() {
	gl_Position = u_Projection * u_View * u_Model * position;
	gl_PointSize = u_PointSize;
	Color = color;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 Color;

void main() {
	// draw points
	float distanceFromCenter = length(2.0 * gl_PointCoord - 1.0);
	if (distanceFromCenter > 1.0) {
		discard;
	}

	color = Color;
};