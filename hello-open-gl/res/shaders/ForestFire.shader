#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D u_ImgOutput1;
layout(rgba32f, binding = 1) uniform image2D u_ImgOutput2;

uniform bool u_SwitchTexture;
uniform float u_FireProbability;
uniform float u_GrowthProbability;
uniform float u_Time;

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

// loading + saving cell
vec4 loadCell(ivec2 pos, ivec2 bounds) {
    // boundary condition: periodic
    if (int(pos.x) >= int(bounds.x)) {
        pos.x = 0;
    }
    else if (pos.x < 0) {
        pos.x = bounds.x - 1;
    }

    if (int(pos.y) >= int(bounds.y)) {
        pos.y = 0;
    }
    else if (pos.y < 0) {
        pos.y = bounds.y - 1;
    }

    return imageLoad(u_SwitchTexture ? u_ImgOutput1 : u_ImgOutput2, pos);
}

void saveCell(ivec2 pos, vec4 cell) {
    imageStore(u_SwitchTexture ? u_ImgOutput2 : u_ImgOutput1, pos, cell);
}

// util
float random(vec3 p3) {
    p3 = fract(p3 * .1031);
    p3 += dot(p3, p3.zyx + 31.32);
    return fract((p3.x + p3.y) * p3.z);
}

void main() {
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);
    ivec2 bounds = imageSize(u_ImgOutput1);

    vec4 currentCell = loadCell(position, bounds);

    if (isEmpty(currentCell)) {
        if (random(vec3(position.x, position.y, u_Time)) < u_GrowthProbability) {
            currentCell = tree();
        }
    }
    else if (isTree(currentCell)) {
        if (random(vec3(position.x, position.y, u_Time)) < u_FireProbability) {
            currentCell = fire();
        }
    }
    else if (isFire(currentCell)) {
        currentCell = empty();
    }

    saveCell(position, currentCell);
}
