	/**
 * @file Phong fragment shader
 *
 * @copyright The Board of Trustees of the Leland Stanford Junior University
 * @version 2021/04/01
 */

/* TODO (2.2.2) */

var shaderID = "fShaderPhong";

var shader = document.createTextNode( `
/**
 * WebGL doesn't set any default precision for fragment shaders.
 * Precision for vertex shader is set to "highp" as default.
 * Do not use "lowp". Some mobile browsers don't support it.
 */
precision mediump float;

varying vec3 normalCam; // Normal in view coordinate
varying vec3 fragPosCam; // Fragment position in view cooridnate

uniform mat4 viewMat;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

uniform vec3 attenuation;

uniform vec3 ambientLightColor;


/***
 * NUM_POINT_LIGHTS is replaced to the number of point lights by the
 * replaceNumLights() function in teapot.js before the shader is compiled.
 */
#if NUM_POINT_LIGHTS > 0

	struct PointLight {
		vec3 position;
		vec3 color;
	};

	uniform PointLight pointLights[ NUM_POINT_LIGHTS ];

#endif


void main() {

	// Compute ambient reflection
	vec3 ambientReflection = material.ambient * ambientLightColor;
	//compute diffuse reflection
	vec3 reflectionSummation;
	for(int i=0;i < NUM_POINT_LIGHTS ; i++){
	vec3 surfaceNormal = normalize(normalCam);
	vec4 pointLightPosInViewSpace = viewMat * vec4(pointLights[i].position, 1.0);
	vec3 lightVector = vec3(pointLightPosInViewSpace.x - fragPosCam.x, pointLightPosInViewSpace.y - fragPosCam.y, pointLightPosInViewSpace.z - fragPosCam.z);
	float r = length(lightVector);	
	float m = max(dot(normalize(lightVector),surfaceNormal), 0.0);
	float distanceFallOff = 1.0/(attenuation.x + attenuation.y * r + attenuation.z * (r * r));
	vec3 diffuseReflection = (material.diffuse * pointLights[i].color * m);

	//compute specular reflection 
	vec3 reflected = reflect(normalize(-lightVector), surfaceNormal);
	float n = max(dot(reflected , normalize(surfaceNormal)), 0.0);
	vec3 specularReflection =  (material.specular * pointLights[i].color * pow(n, material.shininess));

	 reflectionSummation = reflectionSummation + distanceFallOff * (diffuseReflection + specularReflection);
	}
	vec3 fColor = ambientReflection + reflectionSummation;

	gl_FragColor = vec4( fColor, 1.0 );

}
` );


var shaderNode = document.createElement( "script" );

shaderNode.id = shaderID;

shaderNode.setAttribute( "type", "x-shader/x-fragment" );

shaderNode.appendChild( shader );

document.body.appendChild( shaderNode );
