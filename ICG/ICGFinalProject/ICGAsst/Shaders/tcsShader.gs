#version 410 core
layout(vertices = 4) out;
uniform int tLevel;
in vec3 normalizedLightDir[];
in vec3 halfVector[];
in vec4 lightView_Position[];
in vec2 texCoord[];
in vec3 normalizedView[];
out vec3 normalizedView_tcs[];
in vec3 normalizedNormal[];
out vec3 normalizedNormal_tcs[];
out vec3 normalizedLightDir_tcs[];
out vec3 halfVector_tcs[];
//out vec4 lightView_Position_tcs[];
out vec2 texCoord_tcs[];
void main(void)
{
   gl_TessLevelOuter[0] = tLevel;
   gl_TessLevelOuter[1] = tLevel;
   gl_TessLevelOuter[2] = tLevel;
   gl_TessLevelOuter[3] = tLevel;
   gl_TessLevelInner[0] = tLevel;
   gl_TessLevelInner[1] = tLevel;
   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
   normalizedLightDir_tcs[gl_InvocationID] = normalizedLightDir[gl_InvocationID];
   halfVector_tcs[gl_InvocationID] = halfVector[gl_InvocationID];
   texCoord_tcs[gl_InvocationID] = texCoord[gl_InvocationID];
   normalizedNormal_tcs[gl_InvocationID] = normalizedNormal[gl_InvocationID];
   normalizedView_tcs[gl_InvocationID] = normalizedView[gl_InvocationID];
}
