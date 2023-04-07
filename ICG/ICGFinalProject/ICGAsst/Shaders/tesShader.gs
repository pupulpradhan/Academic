#version 410 core
layout(quads, equal_spacing, ccw) in;
uniform mat4 vpMatrix;
uniform float wavetime;
uniform vec3 cameraPos;
uniform float amp1;
uniform float amp2;
uniform float amp3;
in vec3 normalizedLightDir_tcs[];
in vec3 halfVector_tcs[];
in vec4 lightView_Position_tcs[];
in vec2 texCoord_tcs[];
in vec3 normalizedView_tcs[];
out vec3 normalizedView_tes;
in vec3 normalizedNormal_tcs[];
out vec3 normalizedNormal_tes;
out vec3 normalizedLightDir_tes;
out vec3 halfVector_tes;
out vec2 texCoord_tes;
out vec3 viewVec1;
uniform samplerCube env;
uniform sampler2D rockReflection;
vec4 interpolate( vec4 v0, vec4 v1, vec4 v2, vec4 v3 )
{
   vec4 a = mix( v0, v1, gl_TessCoord.x );
   vec4 b = mix( v3, v2, gl_TessCoord.x );
   return mix( a, b, gl_TessCoord.y );
}

vec3 dir1 = normalize(vec3(1.0,4.0,0.25));
vec3 dir2 = normalize(vec3(-1.0,2.0,-1.0));
vec3 dir3 = normalize(vec3(-2.0,3.0,-0.75));
float pi = 3.142;

float random(vec2 st){
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float l = 0.9;
float w = sqrt(9.8*2*pi/l);

void main()
{
    vec4 newPos = interpolate( gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);
//    float amp1 = 0.002;
//    float amp2 = 0.003;
//    float amp3 = 0.025;
    float qa = 1/(w * amp1);
    float qb = 1/(w * amp2);
    float qc = 1/(w * amp3);
    vec3 wave1;
    vec3 wave2;
    vec3 wave3;
    vec3 wave;
    float phase = 5*random(dir1.xy);
    
    wave1.x = (qa * amp1 * dir1.x * cos(dot(dir1,newPos.xyz) + phase * wavetime));
    wave1.y = -(qa * amp1 * dir1.y * cos(dot(dir1,newPos.xyz) + phase * wavetime));
    wave1.z = amp1 * sin(dot(dir1,newPos.xyz) + phase * wavetime);
    
    wave2.x = (qb * amp2 * dir2.x * cos(dot(w*dir2,newPos.xyz) + phase * wavetime));
    wave2.y = -(qb * amp2 * dir2.y * cos(dot(w*dir2,newPos.xyz) + phase * wavetime));
    wave2.z = amp2 * sin(dot(w*dir2,newPos.xyz) + phase * wavetime);
    
    wave3.x = (qc * amp3 * dir3.x * cos(dot(dir3,newPos.xyz) + phase * wavetime));
    wave3.y = -(qc * amp3 * dir3.y * cos(dot(dir3,newPos.xyz) + phase * wavetime));
    wave3.z = amp3 * sin(dot(dir3,newPos.xyz) + phase * wavetime);
    wave = wave1 + wave2 + wave3;
    
    vec4 displacedCoords = vec4(newPos.x + wave.x, newPos.y + wave.y, newPos.z + wave.z, newPos.w);
    gl_Position = vpMatrix * vec4(newPos.x + wave.x, newPos.y + wave.y, newPos.z + wave.z, newPos.w);
    
    normalizedLightDir_tes = normalizedLightDir_tcs[0];
    halfVector_tes = halfVector_tcs[0];
    texCoord_tes =  gl_TessCoord.xy;
    normalizedNormal_tes = normalizedNormal_tcs[0];
    normalizedView_tes = normalize(displacedCoords.xyz - cameraPos);//normalizedView_tcs[0];

}
