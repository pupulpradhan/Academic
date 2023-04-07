#version 330 core
layout(location = 0) out vec4 color;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 normalMatrix;
in vec3 normalizedLightDir_tes;
in vec3 normalizedNormal_tes;
in vec3 halfVector_tes;
in vec3 normalizedView_tes;
//in vec3 reflectVec;
uniform vec3 lightDir;
uniform vec3 cameraPos;
uniform float shininess;
uniform int visibility;
in vec2 texCoord_tes;
in vec3 viewVec1;
uniform sampler2D tex;
uniform samplerCube env;
uniform sampler2D rockReflection;
void main()
{
 
   vec4 normal =  texture(tex, texCoord_tes) ; // compute normal from normal map
   vec3 viewNormal = vec3(normalMatrix * vec3(normal));
   vec3 normalizedNormal = normalize(viewNormal);
//   vec3 viewVec1 = gl_FragCoord.xyz - cameraPos;
   vec3 reflectVec = reflect(normalize(normalizedView_tes), normalizedNormal);
   float cosTheta = max(dot(normalize(lightDir), normalizedNormal), 0.0);
//   float cosPhi = max(dot(normalize(reflectVec), normalize(lightDir)), 0.0);
    float cosPhi = max(dot(normalize(lightDir), normalize(reflectVec)), 0.0);
   float specular = pow(cosPhi, shininess);
   vec3 intensity = vec3(1,1,1);
   vec3 diffuseColor = vec3(intensity * cosTheta * vec3(0.6,0.6,0.6));
   vec3 specularColor = vec3(intensity * specular * vec3(0.88,0.88,0.0));
   vec3 ambientColor = vec3(0.1,0.1,0.1);
  
   vec4 refl_global = 0.6 * texture( env, reflectVec );
    
    vec4 textureSelected;
    vec3 mat_diffuse = vec3(texture(rockReflection, (texCoord_tes)));
//    vec3 mat_diffuse = vec3(texture(rockReflection, gl_FragCoord.xy / textureSize(rockReflection, 0)));
    if(mat_diffuse == vec3(1,1,1)){
        textureSelected = texture( env, reflectVec );
        textureSelected = textureSelected * vec4(0.8,0.8,0.8,1);
    }
    else{
        textureSelected = 0.8 * vec4(mat_diffuse,1) + 0.2 * texture( env, reflectVec );
    }
    vec3 clr = textureSelected.rgb;
    if(visibility == 1)
    clr += vec3(specularColor);
   color = vec4(clr,1);
}
