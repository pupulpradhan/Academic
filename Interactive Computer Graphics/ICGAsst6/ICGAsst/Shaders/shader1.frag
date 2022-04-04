#version 330 core
layout(location = 0) out vec4 color;
in vec3 dir;
uniform samplerCube env;
void main()
{
    color = texture( env, dir );
    //color = vec4(1,1,1,1);
}
