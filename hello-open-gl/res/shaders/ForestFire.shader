#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput1;
layout(rgba32f, binding = 1) uniform image2D imgOutput2;

void main() {
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 texColor1 = imageLoad(imgOutput1, texelCoord);
    vec4 texColor2 = imageLoad(imgOutput2, texelCoord);

    texColor1.x = 0.1;
    texColor1.y = 1;

    texColor2.x = 1;
    texColor2.y = 0.1;

    imageStore(imgOutput1, texelCoord, texColor1);
    imageStore(imgOutput2, texelCoord, texColor2);
}
