#shader vertex
#version 430 core

layout(location = 0) in vec4 vertex; // <vec2 postion, vec2 texCoord>

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main() {
	gl_Position = u_Projection * u_Model * vec4(vertex.xy, 0.0, 1.0);
	v_TexCoord = vertex.zw;
};


#shader fragment
#version 430 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D u_Sprite;
uniform vec3 u_SpriteColor;

void main() {
	color = vec4(u_SpriteColor, 1.0) * texture(u_Sprite, v_TexCoord);
};