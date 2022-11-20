#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;

void main() {
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 texColor = imageLoad(imgOutput, ivec2(0, 0));

    if (texColor.x == 0) {
        value.x = 1;
    }
    else {
        value.x = 0.1;
    }

    if (texColor.y == 0) {
        value.y = 1;
    }
    else {
        value.y = 1;
    }

    imageStore(imgOutput, texelCoord, value);
}
