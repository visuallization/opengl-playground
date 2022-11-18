#shader vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, TexCoords);
}