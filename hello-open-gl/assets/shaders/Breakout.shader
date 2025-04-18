#shader vertex
#version 430 core

layout(location = 0) in vec4 vertex; // <vec2 postion, vec2 texCoord>

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform bool u_Shake;
uniform bool u_Chaos;
uniform bool u_Confuse;
uniform float u_Time;

void main() {
	vec2 texture = vertex.zw;

	gl_Position = u_Projection * u_Model * vec4(vertex.xy, 0.0, 1.0);
	v_TexCoord = texture;

	if (u_Confuse) {
		v_TexCoord = vec2(1.0 - texture.x, 1.0 - texture.y);
	} else if (u_Chaos) {
		float strength = 0.3;
		v_TexCoord = vec2(texture.x + strength * sin(u_Time), texture.y + strength * cos(u_Time));
	}

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

const float offset = 1.0 / 300.0;
const vec2 offsets[9] = vec2[](
	vec2(-offset,  offset), // top-left
	vec2( 0.0f,    offset), // top-center
	vec2( offset,  offset), // top-right
	vec2(-offset,  0.0f),   // center-left
	vec2( 0.0f,    0.0f),   // center-center
	vec2( offset,  0.0f),   // center-right
	vec2(-offset, -offset), // bottom-left
	vec2( 0.0f,   -offset), // bottom-center
	vec2( offset, -offset)  // bottom-right    
);
const float edge[9] = float[](
	1,  1, 1,
	1, -8, 1,
	1,  1, 1
);
const float blur[9] = float[](
	1.0 / 16, 2.0 / 16, 1.0 / 16,
	2.0 / 16, 4.0 / 16, 2.0 / 16,
	1.0 / 16, 2.0 / 16, 1.0 / 16  
);

uniform sampler2D u_Sprite;
uniform bool u_Chaos;
uniform bool u_Confuse;
uniform bool u_Shake;

vec4 Kernel(float kernel[9]) {
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(u_Sprite, v_TexCoord.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        col += sampleTex[i] * kernel[i];
	}

	return vec4(col, 1.0);
}

vec4 Invert(vec4 color) {
	return vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, 1.0);
}

void main() {
	color = texture(u_Sprite, v_TexCoord);

	if (u_Confuse) {
		color = Invert(color);
	} else if (u_Chaos) {
		color = Kernel(edge);
	} else if (u_Shake) {
		color = Kernel(blur);
	}
}