#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	gl_Position = u_Projection * u_View * position;
	//gl_Position = position;

	gl_PointSize = 64.0;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(1, 0, 0, 1);
};