#version 330 core
layout(location =0) out vec4 color;
uniform sampler2D tex;
uniform sampler2D tex1;
in vec2 texCoord;
uniform mat4 mvp;
uniform mat4 mv;
uniform vec3 lightDir;
uniform float shininess;
in vec3 normalizedNormal;
in vec3 normalizedLightDir;
in vec3 halfVector;
void main()
{
    vec4 diffuseTexture = texture(tex, texCoord); //same as ambientTex for this project
    vec4 specularTexture = texture(tex1, texCoord);
    vec4 mixTexture = diffuseTexture + specularTexture;
    float cosTheta = max(dot(normalize(normalizedLightDir), normalize(normalizedNormal)), 0.0);
    float cosPhi = max(dot(normalize(normalizedNormal), normalize(halfVector)), 0.0);
    float specular = pow(cosPhi, shininess);
    vec3 intensity = vec3(1,1,1);
    vec3 diffuseColor = vec3(intensity * cosTheta  * vec3(diffuseTexture) * vec3(1,1,1));
    vec3 specularColor = vec3(intensity * specular * vec3(specularTexture) * vec3(1,1,1));
    vec3 ambientColor = vec3(intensity * vec3(diffuseTexture) * vec3(0.3,0.3,0.3));
    vec3 clr = vec3(diffuseColor + specularColor + ambientColor);
    color = vec4(clr,1);
}


