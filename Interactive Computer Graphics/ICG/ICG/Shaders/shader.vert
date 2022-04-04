#version 130 core
layout(location = 0) in vec3 pos;
 
void main()
{
gl_Position = vec4( 0.05 * pos,1);
}
