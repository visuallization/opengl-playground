#shader vertex
#version 430 core

layout(location = 0) in vec4 vertex; // <vec2 postion, vec2 texCoord>

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main() {
	gl_Position = u_Projection * u_Model * vec4(vertex.xy, 0.0, 1.0);
};

#shader fragment
#version 430 core

out vec4 color;

uniform vec4 u_Color;

void main() {
	color = u_Color;
};