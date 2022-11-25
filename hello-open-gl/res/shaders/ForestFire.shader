#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D u_ImgOutput1;
layout(rgba32f, binding = 1) uniform image2D u_ImgOutput2;

uniform bool u_SwitchTexture;

uniform int u_BoundaryCondition;
uniform int u_NeighborhoodCondition;

uniform float u_FireProbability;
uniform float u_GrowthProbability;
uniform float u_Time;

uniform ivec2 u_MousePosition;

// neighborhood condition constants
const uint NEUMANN = 0;
const uint MOORE = 1;

// boundary condition constants
const uint ASSIGNED = 0;
const uint PERIODIC = 1;

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
    return vec4(0, 0.36, 0.45, 1);
}

vec4 fire() {
    return vec4(0.95, 0.4, 0.54, 2);
}

// boundary condition check
bool isAssigned(int condition) {
    return condition == ASSIGNED;
}

bool isPeriodic(int condition) {
    return condition == PERIODIC;
}

// neighborhood condition check
bool isVonNeumann(int condition) {
    return condition == NEUMANN;
}

bool isMoore(int condition) {
    return condition == MOORE;
}

// loading + saving cell
vec4 loadCell(ivec2 pos, ivec2 bounds) {
    // boundary conditions
    if (int(pos.x) >= int(bounds.x)) {
        pos.x = isPeriodic(u_BoundaryCondition) ? 0 : bounds.x - 1;
    }
    else if (pos.x < 0) {
        pos.x = isPeriodic(u_BoundaryCondition) ? bounds.x - 1 : 0;
    }

    if (int(pos.y) >= int(bounds.y)) {
        pos.y = isPeriodic(u_BoundaryCondition) ? 0 : bounds.y - 1;
    }
    else if (pos.y < 0) {
        pos.y = isPeriodic(u_BoundaryCondition) ? bounds.y - 1 : 0;
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
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 bounds = imageSize(u_ImgOutput1);

    vec4 currentCell = loadCell(pos, bounds);
    
    if (u_MousePosition.x != -1 && u_MousePosition.y != -1 && pos.x == u_MousePosition.x && pos.y == u_MousePosition.y) {
        currentCell = fire();
    }
    else if (isEmpty(currentCell)) {
        if (random(vec3(pos.x, pos.y, u_Time)) < u_GrowthProbability) {
            currentCell = tree();
        }
    }
    else if (isTree(currentCell)) {
        if (random(vec3(pos.x, pos.y, u_Time)) < u_FireProbability) {
            currentCell = fire();
        }
        else {
            // Check if a neighbour is on fire
            // If so the current cell will also catch fire
            bool isANeighborOnFire = false;

            // (Von Neumann Neighborhood)
            vec4 n1 = loadCell(ivec2(pos.x - 1, pos.y), bounds);
            vec4 n2 = loadCell(ivec2(pos.x, pos.y + 1), bounds);
            vec4 n3 = loadCell(ivec2(pos.x + 1, pos.y), bounds);
            vec4 n4 = loadCell(ivec2(pos.x, pos.y - 1), bounds);

            isANeighborOnFire = isFire(n1) || isFire(n2) || isFire(n3) || isFire(n4);

            // (Moore Neighborhood)
            if (!isANeighborOnFire && isMoore(u_NeighborhoodCondition)) {
                vec4 n5 = loadCell(ivec2(pos.x - 1, pos.y + 1), bounds);
                vec4 n6 = loadCell(ivec2(pos.x + 1, pos.y + 1), bounds);
                vec4 n7 = loadCell(ivec2(pos.x + 1, pos.y - 1), bounds);
                vec4 n8 = loadCell(ivec2(pos.x - 1, pos.y - 1), bounds);

                isANeighborOnFire = isFire(n5) || isFire(n6) || isFire(n7) || isFire(n8);
            }

            if (isANeighborOnFire) {
                currentCell = fire();
            }
        }

    }
    else if (isFire(currentCell)) {
        currentCell = empty();
    }


    saveCell(pos, currentCell);
}
