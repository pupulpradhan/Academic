#version 330 core
layout(location =0) out vec4 color;
uniform mat4 mvp;
uniform mat4 mv;
uniform vec3 lightDir;
uniform float shininess;
in vec3 normalizedNormal;
in vec3 normalizedLightDir;
in vec3 halfVector;
in vec3 reflectVec;
uniform samplerCube env;
void main()
{
    float cosTheta = max(dot(normalize(normalizedLightDir), normalize(normalizedNormal)), 0.0);
    float cosPhi = max(dot(normalize(normalizedNormal), normalize(halfVector)), 0.0);
    float specular = pow(cosPhi, shininess);
    vec3 intensity = vec3(0.9,0.5,0.5);
    vec3 diffuseColor = vec3(intensity * cosTheta * vec3(0.5,0.5,0.5));
    vec3 specularColor = vec3(intensity * specular * vec3(1,1,1));
    vec3 ambientColor = vec3(intensity * vec3(0.2,0.0,0.0));
    vec4 envLight = texture( env, reflectVec );
    vec3 envLightSampled = vec3(envLight) * vec3(0.8,0.8,0.8);
    vec3 clr = normalize(vec3(diffuseColor + specularColor + envLightSampled));
    color = vec4(clr,1);
}



