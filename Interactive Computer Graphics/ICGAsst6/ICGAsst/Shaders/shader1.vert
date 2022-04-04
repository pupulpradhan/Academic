#version 330 core
layout(location = 0) in vec3 pos;
//layout(location = 1) in vec3 normal;
//layout(location = 2) in vec2 txc;
//uniform mat4 mvp;
uniform mat4 inverse;
//uniform mat3 normalMatrix;
uniform vec3 center;
//out vec3 normalizedNormal;
//out vec3 normalizedLightDir;
//out vec3 halfVector;
out vec3 dir;
void main()
{
    vec4 pos_world = inverse * vec4(pos,1);
    dir = (pos_world.xyz/pos_world.w) - center;
    gl_Position = vec4(pos,1);
}
