#version 330 core
layout(location = 0) in vec3 pos;
uniform mat4 inverse;
uniform vec3 center;
out vec3 dir;
void main()
{
    vec4 pos_world = inverse * vec4(pos,1);
    dir = (pos_world.xyz/pos_world.w) - center;
    gl_Position = vec4(pos,1);
}

