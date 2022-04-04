/**
 * @file Gouraud vertex shader with diffuse and ambient light
 *
 * @copyright The Board of Trustees of the Leland Stanford Junior University
 * @version 2021/04/01
 */

/* TODO (2.1.1), (2.1.3) */

var shaderID = "vShaderGouraudDiffuse";

var shader = document.createTextNode( `
/**
 * varying qualifier is used for passing variables from a vertex shader
 * to a fragment shader. In the fragment shader, these variables are
 * interpolated between neighboring vertexes.
 */
varying vec3 vColor; // Color at a vertex

uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform mat4 modelViewMat;
uniform mat3 normalMat;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

uniform vec3 attenuation;

uniform vec3 ambientLightColor;

attribute vec3 position;
attribute vec3 normal;


/***
 * NUM_POINT_LIGHTS is replaced to the number of point lights by the
 * replaceNumLights() function in teapot.js before the shader is compiled.
 */
#if NUM_POINT_LIGHTS > 0

	struct PointLight {
		vec3 color;
		vec3 position;
	};

	uniform PointLight pointLights[ NUM_POINT_LIGHTS ];

#endif


void main() {

	// Compute ambient reflection
	vec3 ambientReflection = material.ambient * ambientLightColor;
	//compute diffuse reflection
	vec3 reflectionSummation;
	for(int i=0;i < NUM_POINT_LIGHTS ; i++){
	vec3 surfaceNormal = normalize(normalMat * normal);
	vec4 pointLightPosInViewSpace = viewMat * vec4(pointLights[i].position, 1.0);
	vec4 vertexPosInViewSpace = modelViewMat * vec4(position, 1.0);
	vec3 lightVector = vec3(pointLightPosInViewSpace.x - vertexPosInViewSpace.x, pointLightPosInViewSpace.y - vertexPosInViewSpace.y, pointLightPosInViewSpace.z - vertexPosInViewSpace.z);
	float r = length(lightVector);	
	float m = max(dot(normalize(lightVector),surfaceNormal), 0.0);
	float distanceFallOff = 1.0/(attenuation.x + attenuation.y * r + attenuation.z * (r * r));
	vec3 diffuseReflection = (material.diffuse * pointLights[i].color * m);

	//compute specular reflection 
	vec3 reflected = reflect(normalize(-lightVector), surfaceNormal);
	float n = max(dot(vec4(reflected, 1.0) , normalize(vec4(surfaceNormal,0.0))), 0.0);
	vec3 specularReflection =  (material.specular * pointLights[i].color * pow(n, material.shininess));

	 reflectionSummation = reflectionSummation + distanceFallOff * (diffuseReflection + specularReflection);
	}
	vColor = ambientReflection + reflectionSummation ;

	gl_Position =
		projectionMat * modelViewMat * vec4( position, 1.0 );

}
` );


var shaderNode = document.createElement( "script" );

shaderNode.id = shaderID;

shaderNode.setAttribute( "type", "x-shader/x-vertex" );

shaderNode.appendChild( shader );

document.body.appendChild( shaderNode );
