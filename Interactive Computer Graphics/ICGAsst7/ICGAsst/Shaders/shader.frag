#version 330 core
layout(location = 0) out vec4 color;
uniform mat4 mvp;
uniform mat4 mv;
uniform vec3 lightDir;
uniform float shininess;
in vec3 normalizedNormal;
in vec3 normalizedLightDir;
in vec3 halfVector;
uniform sampler2DShadow shadow; // depth map
in vec4 lightView_Position;
void main()
{
   float cosTheta = max(dot(normalize(normalizedLightDir), normalize(normalizedNormal)), 0.0);
   float cosPhi = max(dot(normalize(normalizedNormal), normalize(halfVector)), 0.0);
   float specular = pow(cosPhi, shininess);
   vec3 intensity = vec3(1,1,1);
   vec3 p = lightView_Position.xyz / lightView_Position.w;
   float visibility = texture(shadow, p);
    if(p.x > 1.0 || p.y > 1.0){
        visibility = 1;
    }
    if(p.x < 0.0 || p.y < 0.0){
        visibility = 1;
    }
   vec3 diffuseColor = vec3(visibility * intensity * cosTheta * vec3(0.9,0.5,0.5));
   vec3 specularColor = vec3(visibility * intensity * specular * vec3(1,1,1));
   vec3 ambientColor = vec3(0.3,0.1,0.1);
   vec3 clr = vec3(diffuseColor + specularColor + ambientColor);
   color = vec4(clr,1);
   
}
