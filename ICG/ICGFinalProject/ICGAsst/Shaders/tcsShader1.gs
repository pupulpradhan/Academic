#version 410 core
layout(vertices = 4) out;
uniform int tLevel;
void main(void)
{
    gl_TessLevelOuter[0] = tLevel;
    gl_TessLevelOuter[1] = tLevel;
    gl_TessLevelOuter[2] = tLevel;
    gl_TessLevelOuter[3] = tLevel;
    gl_TessLevelInner[0] = tLevel;
    gl_TessLevelInner[1] = tLevel;
   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

