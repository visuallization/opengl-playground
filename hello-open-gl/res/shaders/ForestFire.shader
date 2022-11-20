#shader compute
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D u_ImgOutput1;
layout(rgba32f, binding = 1) uniform image2D u_ImgOutput2;

uniform bool u_SwitchTexture;

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 texColor = imageLoad(u_SwitchTexture ? u_ImgOutput1 : u_ImgOutput2, texelCoord);

    // the alpha channel (a) is used to classify the cell
    // 0 = empty, 1 = tree, 2 = fire
    if (u_SwitchTexture) {
        texColor.r = 0.1;
        texColor.g = 1;
        texColor.a = 1;
    }
    else {
        texColor.r = 1;
        texColor.g = 0.1;
        texColor.a = 2;
    }

    imageStore(u_SwitchTexture ? u_ImgOutput2 : u_ImgOutput1, texelCoord, texColor);
}
