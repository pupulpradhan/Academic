#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 txc;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 normalMatrix;
uniform vec3 lightDir;
out vec3 normalizedNormal;
out vec3 normalizedLightDir;
out vec3 halfVector;
out vec2 texCoord;
void main()
{
    vec3 viewNormal = normalMatrix * normal;
    vec3 viewVec = vec3(mv * vec4(pos,1));
    normalizedNormal = normalize(viewNormal);
    vec3 normalizedView = normalize(-viewVec);
    normalizedLightDir = vec3(normalize(vec4(lightDir,0))) ;
    halfVector = normalize(normalizedLightDir + normalizedView);
    gl_Position = mvp * vec4(pos,1);
    texCoord = txc;
}


