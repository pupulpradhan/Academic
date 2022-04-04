/**
 * @file Fragment shader for DoF rendering
 *
 * @copyright The Board of Trustees of the Leland Stanford Junior University
 * @version 2021/04/01
 */

/* TODO (2.3) DoF Rendering */

var shaderID = "fShaderDof";

var shader = document.createTextNode( `
/**
 * WebGL doesn't set any default precision for fragment shaders.
 * Precision for vertex shader is set to "highp" as default.
 * Do not use "lowp". Some mobile browsers don't support it.
 */
precision mediump float;

// uv coordinates after interpolation
varying vec2 textureCoords;

// texture map from the first rendering
uniform sampler2D textureMap;

// depth map from the first rendering
uniform sampler2D depthMap;

// Projection matrix used for the first pass
uniform mat4 projectionMat;

// Inverse of projectionMat
uniform mat4 invProjectionMat;

// resolution of the window in [pixels]
uniform vec2 windowSize;

// Gaze position in [pixels]
uniform vec2 gazePosition;

// Diameter of pupil in [mm]
uniform float pupilDiameter;

// pixel pitch in [mm]
uniform float pixelPitch;

const float searchRad = 11.0;


// Compute the distance to fragment in [mm]
// p: texture coordinate of a fragment / a gaze position
//
// Note: GLSL is column major
float distToFrag( vec2 p ) {

	/* TODO (2.3.1) Distance to Fragment */
	float xNdc = (p.x * (2.0 / windowSize.x)) - 1.0;
	float yNdc = (p.y * (2.0 / windowSize.y)) - 1.0;
	float zNdc = (texture2D(depthMap, p).x * 2.0) - 1.0;
	float wClip = - (projectionMat[3][2] / (zNdc - (projectionMat[2][2]/ projectionMat[2])[3]));
	float zView = projectionMat[3][2] / (zNdc - (projectionMat[2][2]/ projectionMat[2])[3]);
	float xClip = xNdc * wClip;
	float yClip = yNdc * wClip;
	float zClip = zNdc * wClip;
	vec4  vView = invProjectionMat * vec4(xClip,yClip,zClip,wClip);
	float xView = vView.x;
	float yView = vView.y;
	return sqrt(pow(xView, 2.0) + pow(yView, 2.0) + pow(zView, 2.0));

}


// compute the circle of confusion in [mm]
// fragDist: distance to current fragment in [mm]
// focusDist: distance to focus plane in [mm]
float computeCoC( float fragDist, float focusDist ) {

	/* TODO (2.3.2) Circle of Confusion Computation */
	float b = (abs(fragDist - focusDist)/fragDist) * pupilDiameter;
	float blurDiameter = b/pixelPitch;
	return blurDiameter;

}


// compute depth of field blur and return color at current fragment
vec3 computeBlur() {

	/* TODO (2.3.3) Retinal Blur */	
	vec3 outputColor = vec3(0.0);
	vec3 pixelColor = vec3(0.0);
	float n = 1.0;
	float blurRadius = computeCoC(distToFrag(textureCoords), distToFrag(gazePosition))/2.0;
	//int blurRadiusInt = round(blurRadius);

	// set output color by averaging neighboring pixels in the color image (i.e., convolution)
	//gl_FragColor.rgb = 0;
	for (float i=-searchRad; i<searchRad; i++){
		for (float j=-searchRad; j<searchRad; j++){
			//float d = sqrt(pow((textureCoords.x - (gazePosition.x/windowSize.x)),2.0) + pow((textureCoords.y - (gazePosition.y/windowSize.y)), 2.0));
			
			if (float(pow(abs((textureCoords.x + i/windowSize.x) - gazePosition.x/windowSize.x), 2.0) + 
			pow(abs((textureCoords.y + j/windowSize.y) - gazePosition.y/windowSize.y), 2.0)) <= blurRadius*blurRadius){
				pixelColor = texture2D(textureMap,  vec2(textureCoords.x + i/windowSize.x, textureCoords.y + j/windowSize.y) ).rgb;
				outputColor +=  pixelColor;
				n++;
			}
		}
	}
	//gl_FragColor.rgb += … texture lookup in neighboring pixels
	// normalize color
	//outputColor = outputColor/n;
	return outputColor/n;
}


void main() {

	gl_FragColor = vec4(computeBlur(),1); 
	//texture2D( textureMap,  textureCoords );

}
` );


var shaderNode = document.createElement( "script" );

shaderNode.id = shaderID;

shaderNode.setAttribute( "type", "x-shader/x-fragment" );

shaderNode.appendChild( shader );

document.body.appendChild( shaderNode );
