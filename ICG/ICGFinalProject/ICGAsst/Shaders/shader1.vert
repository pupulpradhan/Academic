#version 330 core
layout(location = 0) in vec3 pos;
uniform mat4 projectorMatrix;

vec4 computeGridVerticesinWorldSpace(vec3 pos){
    vec4 pos_posZ = projectorMatrix * vec4(pos.xy,1,1);//perspective grid in xy plane
    vec4 pos_negZ = projectorMatrix * vec4(pos.xy,-1,1);//perspective grid in xy plane
    float t = (pos_negZ.y) / (pos_posZ.y - pos_negZ.y);
    return vec4(pos_negZ.x + ((pos_negZ.x - pos_posZ.x) * t), pos_negZ.y + ((pos_negZ.y - pos_posZ.y) * t), pos_negZ.z + ((pos_negZ.z - pos_posZ.z) * t), pos_negZ.w + ((pos_negZ.w - pos_posZ.w) * t) );
}

void main(){
    vec4 pos_ws = computeGridVerticesinWorldSpace(pos);
    gl_Position = vec4(pos_ws.x/pos_ws.w, pos_ws.y/pos_ws.w, pos_ws.z/pos_ws.w, 1);
}
