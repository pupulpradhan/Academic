#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalMatrix1;
uniform vec3 lightDir;
uniform vec3 cameraPos;
out vec3 normalizedNormal;
out vec3 normalizedLightDir;
out vec3 halfVector;
out vec3 reflectVec;
void main()
{
    vec3 viewNormal = normalMatrix * normal;
    vec3 viewVec = vec3(mv * vec4(pos,1));
    normalizedNormal = normalize(viewNormal);
    vec3 normalizedView = normalize(-viewVec);
    vec3 viewVec1 = normalize(cameraPos - vec3(modelMatrix * vec4(pos,1)));
    vec3 viewNormal1 = normalize(normalMatrix1 * normal);
    reflectVec = reflect( -viewVec1, viewNormal1);
    normalizedLightDir = vec3(normalize(vec4(lightDir,0)));
    halfVector = normalize(lightDir + normalizedView);
    gl_Position = mvp * vec4(pos,1);
}

