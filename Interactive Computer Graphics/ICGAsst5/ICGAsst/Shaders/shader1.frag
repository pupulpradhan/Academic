#version 330 core
layout(location =0) out vec4 color;
uniform sampler2D tex;
in vec2 texCoord;
//uniform mat4 mvp;
//uniform mat4 mv;
//uniform vec3 lightDir;
//uniform float shininess;
//in vec3 normalizedNormal;
//in vec3 normalizedLightDir;
//in vec3 halfVector;
void main()
{

//    float cosTheta = max(dot(normalize(normalizedLightDir), normalize(normalizedNormal)), 0.0);
//    float cosPhi = max(dot(normalize(normalizedNormal), normalize(halfVector)), 0.0);
//    float specular = pow(cosPhi, shininess);
//    vec3 intensity = vec3(1,1,1);
//    vec3 diffuseColor = vec3(intensity * cosTheta * vec3(1,0,0));
//    vec3 specularColor = vec3(intensity * specular * vec3(1,1,1));
//    vec3 ambientColor = vec3(intensity * vec3(0.2,0.0,0.0));
//    vec3 clr = vec3(diffuseColor + specularColor + ambientColor);
//    color = vec4(texCoord,0,1);
    color = texture(tex, texCoord);
}


