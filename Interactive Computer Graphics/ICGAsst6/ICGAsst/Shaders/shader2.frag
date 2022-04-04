#version 330 core
layout(location =0) out vec4 color;
in vec2 texCoord;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 normalMatrix;
uniform vec3 lightDir;
uniform float shininess;
uniform vec3 cameraPos;
in vec3 normalizedNormal;
in vec3 normalizedLightDir;
in vec3 halfVector;
in vec3 pos_world;
in vec3 normalVec;
//in vec3 reflectVec;
in vec3 cameraPosition_worldSpace;
uniform samplerCube env;
uniform sampler2D teapotReflection;
void main()
{
    vec3 viewVec1 = (pos_world - cameraPos);
    vec3 reflectVec = reflect(viewVec1, normalVec);
    float cosTheta = max(dot(normalize(normalizedLightDir), normalize(normalizedNormal)), 0.0);
    float cosPhi = max(dot(normalize(normalizedNormal), normalize(halfVector)), 0.0);
    float specular = pow(cosPhi, shininess);
    vec3 intensity = vec3(0.5,0.5,0.5);
    vec3 diffuseColor = vec3(intensity * cosTheta * vec3(0.5,0.5,0.5));
    vec3 specularColor = vec3(intensity * specular * vec3(0.5,0.5,0.5));
    vec3 ambientColor = vec3(intensity * vec3(0.2,0.0,0.0));
    vec4 textureSelected;
    vec3 mat_diffuse = vec3(texture(teapotReflection, gl_FragCoord.xy / textureSize(teapotReflection, 0)));
    if(mat_diffuse == vec3(1,1,1)){
        textureSelected = texture( env, reflectVec );
        textureSelected = textureSelected * vec4(0.8,0.8,0.8,1);
    }
    else{
        textureSelected = vec4(mat_diffuse,1);
    }
    vec3 clr = (diffuseColor + specularColor + textureSelected.rgb);
    color = vec4(clr,1);
}

