#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D u_ImgOutput1;
layout(rgba32f, binding = 1) uniform image2D u_ImgOutput2;

uniform bool u_SwitchTexture;
uniform float u_FireProbability;
uniform float u_GrowthProbability;

// the alpha channel (a) is used to classify the cell
// 0 = empty, 1 = tree, 2 = fire
bool isEmpty(vec4 cell) {
    return cell.a == 0;
}

bool isTree(vec4 cell) {
    return cell.a == 1;
}

bool isFire(vec4 cell) {
    return cell.a == 2;
}

// the alpha channel (a) is used to set the cell
// 0 = empty, 1 = tree, 2 = fire
vec4 empty() {
    return vec4(0, 0, 0, 0);
}

vec4 tree() {
    return vec4(0, 1, 0, 1);
}

vec4 fire() {
    return vec4(1, 0, 0, 2);
}

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 currentCell = imageLoad(u_SwitchTexture ? u_ImgOutput1 : u_ImgOutput2, texelCoord);

    if (isEmpty(currentCell)) {
        currentCell = tree();
    }
    else {
        currentCell = fire();
    }

    imageStore(u_SwitchTexture ? u_ImgOutput2 : u_ImgOutput1, texelCoord, currentCell);
}
