/**
 * @file Unwarp fragment shader
 *
 * @copyright The Board of Trustees of the Leland Stanford Junior University
 * @version 2021/04/01
 */

/* TODO (2.2.2) Fragment shader implementation */

var shaderID = "fShaderUnwarp";

var shader = document.createTextNode( `
/**
 * WebGL doesn't set any default precision for fragment shaders.
 * Precision for vertex shader is set to "highp" as default.
 * Do not use "lowp". Some mobile browsers don't support it.
 */

precision mediump float;

varying vec2 textureCoords;

// texture rendered in the first rendering pass
uniform sampler2D map;

// center of lens for un-distortion
// in normalized coordinates between 0 and 1
uniform vec2 centerCoordinate;

// [width, height] size of viewport in [mm]
// viewport is the left/right half of the browser window
uniform vec2 viewportSize;

// lens distortion parameters [K_1, K_2]
uniform vec2 K;

// distance between lens and screen in [mm]
uniform float distLensScreen;

void main() {

	float radialDistance = sqrt(pow((textureCoords.x * viewportSize.x - centerCoordinate.x * viewportSize.x) ,2.0) + pow((textureCoords.y * viewportSize.y - centerCoordinate.y * viewportSize.y) ,2.0));
	float normalizedRadialDistance = radialDistance/distLensScreen;
	float multiplier = (K[0] * pow(normalizedRadialDistance,2.0)) + (K[1] * pow(normalizedRadialDistance, 4.0));
	vec2 distortedTextureCoordinates = vec2((textureCoords.x  + (textureCoords.x - centerCoordinate.x) * multiplier) , (textureCoords.y + (textureCoords.y - centerCoordinate.y) * multiplier));
	if(distortedTextureCoordinates.x >= 0.0 && distortedTextureCoordinates.x < 1.0 && distortedTextureCoordinates.y >= 0.0 && distortedTextureCoordinates.y < 1.0 ){
	gl_FragColor = texture2D( map, distortedTextureCoordinates );
	}
	else{
	gl_FragColor = vec4(0,0,0,0);
	}

}
` );


var shaderNode = document.createElement( "script" );

shaderNode.id = shaderID;

shaderNode.setAttribute( "type", "x-shader/x-fragment" );

shaderNode.appendChild( shader );

document.body.appendChild( shaderNode );
