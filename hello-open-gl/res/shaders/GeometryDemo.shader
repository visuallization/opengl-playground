#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out VertexOutput {
	vec4 color;
} vertexOutput;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
   gl_Position = u_Projection * u_View * u_Model * position;
   vertexOutput.color = color;
};

#shader geometry
#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VertexOutput {
	vec4 color;
} geometryInput[];

out GeometryOutput {
	vec4 color;
} geometryOutput;

void build_house(vec4 position) {
	geometryOutput.color = geometryInput[0].color;

	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // bottom-left
	EmitVertex();

	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // bottom-right
	EmitVertex();

	gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // top-left
	EmitVertex();

	gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // top-right
	EmitVertex();

	gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // top
	geometryOutput.color = vec4(1.0, 1.0, 1.0, 1.0); // add snow on top ;)
	EmitVertex();

	EndPrimitive();
}

void main() {
	build_house(gl_in[0].gl_Position);
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in GeometryOutput {
	vec4 color;
} fragmentInput;

void main() {
	color = fragmentInput.color;
};