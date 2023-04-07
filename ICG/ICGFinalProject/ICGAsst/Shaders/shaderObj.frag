#version 330 core
layout(location =0) out vec4 color;
uniform mat4 mvp;
uniform mat4 mv;
uniform vec3 lightDir;
uniform float shininess;
uniform vec3 difCol;
uniform vec3 specCol;
uniform vec3 ambCol;
in vec3 normalizedNormal;
in vec3 normalizedLightDir;
in vec3 halfVector;
//in vec3 reflectVec;
in vec3 viewNormal1;
in vec3 viewVec1;
uniform samplerCube env;
void main()
{
    vec3 reflectVec = reflect( -viewVec1, normalize(viewNormal1));
    float cosTheta = max(dot(normalize(normalizedLightDir), normalize(normalizedNormal)), 0.0);
    float cosPhi = max(dot(normalize(normalizedNormal), normalize(halfVector)), 0.0);
    float specular = pow(cosPhi, shininess);
    vec3 intensity = vec3(1,1,1);
    vec3 diffuseColor = vec3(intensity * cosTheta * difCol);
    vec3 specularColor = vec3(intensity * specular * specCol);
    vec3 ambientColor = vec3(intensity * ambCol);
    vec3 clr = vec3(diffuseColor + ambientColor + specularColor);
    color = vec4(clr,1);
}



