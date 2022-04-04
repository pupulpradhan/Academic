/**
 * @file Fragment shader for foveated rendering
 *
 * @copyright The Board of Trustees of the Leland Stanford Junior University
 * @version 2021/04/01
 */

/* TODO (2.2.4) Fragment Shader Foveation Blur */

var shaderID = "fShaderFoveated";

var shader = document.createTextNode( `
/***
 * WebGL doesn't set any default precision for fragment shaders.
 * Precision for vertex shader is set to "highp" as default.
 * Do not use "lowp". Some mobile browsers don't support it.
 */
precision mediump float;

// texture or uv coordinates of current fragment in normalized coordinates [0,1]
varying vec2 textureCoords;

// texture map from the first rendering pass
uniform sampler2D textureMap;

// resolution of the window in [pixels]
uniform vec2 windowSize;

// window space coordinates of gaze position in [pixels]
uniform vec2 gazePosition;

// eccentricity angle at boundary of foveal and middle layers
uniform float e1;

// eccentricity angle at boundary of middle and outer layers
uniform float e2;

// visual angle of one pixel
uniform float pixelVA;

// radius of middle layer blur kernel [in pixels]
const float middleKernelRad = 2.0;

// radius of outer layer blur kernel [in pixels]
const float outerKernelRad = 4.0;

// gaussian blur kernel for middle layer (5x5)
uniform float middleBlurKernel[int(middleKernelRad)*2+1];

// gaussian blur kernel for outer layer (9x9)
uniform float outerBlurKernel[int(outerKernelRad)*2+1];

//blur for middle layer
vec3 computeMiddleBlur(){
	vec3 outputColor = vec3(0.0);

	for(float i=-middleKernelRad;i<=middleKernelRad; i+=1.0){
		for(float j=-middleKernelRad; j<=middleKernelRad; j+=1.0){
			vec3 pixelColor = texture2D(textureMap,  vec2(textureCoords.x + i/windowSize.x, textureCoords.y + j/windowSize.y) ).rgb;
			outputColor += (pixelColor) * middleBlurKernel[int(i + middleKernelRad)] * middleBlurKernel[int(j + middleKernelRad)];
			
		}
	}
return outputColor;
}


//blur for outer layer
vec3 computeOuterBlur(){
	vec3 outputColor = vec3(0.0);

	for(float i=-outerKernelRad;i<=outerKernelRad; i+=1.0){
		for(float j=-outerKernelRad; j<=outerKernelRad; j+=1.0){
			vec3 pixelColor = texture2D(textureMap,  vec2(textureCoords.x + i/windowSize.x, textureCoords.y + j/windowSize.y) ).rgb;
			outputColor += pixelColor * outerBlurKernel[int(i + outerKernelRad)] * outerBlurKernel[int(j + outerKernelRad)];
			
		}
	}
return outputColor;
}

void main() {
	vec4 fragColor = vec4(0.0);
	float d = sqrt(pow((textureCoords.x - (gazePosition.x/windowSize.x)),2.0) + pow((textureCoords.y - (gazePosition.y/windowSize.y)), 2.0));
	float e = (d/pixelVA - 1.0/48.0) / 0.0275;
	if(e<=e1){
		fragColor = texture2D( textureMap,  textureCoords );
	}
	if(e>=e1 && e<=e2){
		fragColor = vec4(computeMiddleBlur(),1);
	}
	if(e>=e2){
		fragColor = vec4(computeOuterBlur(),1);
	}
	gl_FragColor = fragColor;
}
` );

var shaderNode = document.createElement( "script" );

shaderNode.id = shaderID;

shaderNode.setAttribute( "type", "x-shader/x-fragment" );

shaderNode.appendChild( shader );

document.body.appendChild( shaderNode );
