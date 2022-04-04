/**
 * @file Fragment shader for anaglyph rendering
 *
 * @copyright The Board of Trustees of the Leland Stanford Junior University
 * @version 2021/04/01
 */

/* TODO (2.4.3) Color Channel Multiplexing */

var shaderID = "fShaderAnaglyph";

var shader = document.createTextNode( `
/**
 * WebGL doesn't set any default precision for fragment shaders.
 * Precision for vertex shader is set to "highp" as default.
 * Do not use "lowp". Some mobile browsers don't support it.
 */

precision mediump float;

// uv coordinates after interpolation
varying vec2 textureCoords;

// Texture map for the left eye
uniform sampler2D textureMapL;

// Texture map for the right eye
uniform sampler2D textureMapR;

void main() {


	vec4 changeToGrayL = texture2D( textureMapL,  textureCoords ) ;
	float grayL = (0.2989*changeToGrayL.x) + (0.5870*changeToGrayL.y) + (0.1140*changeToGrayL.z);
	
	vec4 changeToGrayR = texture2D( textureMapR,  textureCoords ) ;
	float grayR = (0.2989*changeToGrayR.x) + (0.5870*changeToGrayR.y) + (0.1140*changeToGrayR.z);
	
	gl_FragColor = vec4(grayL,grayR,grayR,1);
	//gl_FragColor = texture2D( textureMapL,  textureCoords );

}
` );

var shaderNode = document.createElement( "script" );

shaderNode.id = shaderID;

shaderNode.setAttribute( "type", "x-shader/x-fragment" );

shaderNode.appendChild( shader );

document.body.appendChild( shaderNode );
