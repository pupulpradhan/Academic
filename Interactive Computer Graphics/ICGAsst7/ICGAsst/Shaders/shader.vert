#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightDir;
uniform vec3 cameraPos;
out vec3 normalizedNormal;
out vec3 normalizedLightDir;
out vec3 halfVector;
uniform mat4 matrixShadow;
out vec4 lightView_Position;
void main()
{
   vec3 viewNormal = normalMatrix * normal;
   vec3 viewVec = vec3(mv * vec4(pos,1));
   normalizedNormal = normalize(viewNormal);
   vec3 normalizedView = normalize(-viewVec);
   normalizedLightDir = vec3(normalize(vec4(lightDir,0)));
   halfVector = normalize(lightDir + normalizedView);
   gl_Position = mvp * vec4(pos,1);
   lightView_Position = matrixShadow * vec4(pos,1);
}
