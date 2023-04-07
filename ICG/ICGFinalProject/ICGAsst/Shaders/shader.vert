#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 txc;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 projectorMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightDir;
uniform sampler2D tex1;
out vec3 normalizedLightDir;
out vec3 halfVector;
out vec4 lightView_Position;
out vec2 texCoord;
out vec3 normalizedNormal;
out vec3 normalizedView;


vec4 computeGridVerticesinWorldSpace(vec3 pos){
    vec4 pos_posZ = projectorMatrix * vec4(pos.xy,1,1);//perspective grid in xy plane
    vec4 pos_negZ = projectorMatrix * vec4(pos.xy,-1,1);//perspective grid in xy plane
    float t = (pos_negZ.y) / (pos_posZ.y - pos_negZ.y);
    return vec4(pos_negZ.x + ((pos_negZ.x - pos_posZ.x) * t), pos_negZ.y + ((pos_negZ.y - pos_posZ.y) * t), pos_negZ.z + ((pos_negZ.z - pos_posZ.z) * t), pos_negZ.w + ((pos_negZ.w - pos_posZ.w) * t) );
}

void main(){
    normalizedNormal = vec3(projectorMatrix * vec4(normal,0));
    vec3 viewVec = vec3(projectorMatrix * vec4(pos,1));
    normalizedView = normalize(-viewVec);
    normalizedLightDir = normalize(lightDir);
    halfVector = normalize(lightDir + normalizedView);
    vec4 pos_ws = computeGridVerticesinWorldSpace(pos);
    gl_Position = vec4(pos_ws.x/pos_ws.w, pos_ws.y/pos_ws.w, pos_ws.z/pos_ws.w, 1);
    texCoord = txc;
}
