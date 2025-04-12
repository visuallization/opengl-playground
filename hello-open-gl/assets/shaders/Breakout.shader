#shader vertex
#version 430 core

layout(location = 0) in vec4 vertex; // <vec2 postion, vec2 texCoord>

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform bool u_Shake;
uniform float u_Time;

void main() {
	gl_Position = u_Projection * u_Model * vec4(vertex.xy, 0.0, 1.0);
	v_TexCoord = vertex.zw;

	if (u_Shake) {
		float strength = 0.01;
		gl_Position.x += cos(u_Time * 10.0) * strength;
		gl_Position.y += cos(u_Time * 15.0) * strength;
	}
};


#shader fragment
#version 430 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D u_Sprite;

void main() {
	color = texture(u_Sprite, v_TexCoord);
}