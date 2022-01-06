// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The transformation first applies scale, then rotation, and finally translation.
// The given rotation value is in degrees.
function GetTransform( positionX, positionY, rotation, scale )
{
//Apply scale
var scaleMatrix = new Array(scale, 0, 0, 0, scale, 0, 0, 0, 1);

	//Apply rotation
var rad = degrees_to_radians(rotation);
var	rotationMatrix = new Array(Math.cos(rad), Math.sin(rad), 0, -Math.sin(rad), Math.cos(rad), 0, 0, 0, 1);

//Apply translation 
var translationMatrix = new Array(1, 0, 0, 0, 1, 0, positionX, positionY, 1);

return ApplyTransform(scaleMatrix, ApplyTransform(rotationMatrix, translationMatrix));
}

// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The arguments are transformation matrices in the same format.
// The returned transformation first applies trans1 and then trans2.
function ApplyTransform( trans1, trans2 )
{
	//return Math.multiply(trans2, trans1);
	//return Array( 1, 0, 0, 0, 1, 0, 0, 0, 1 );
	var a = trans2[0] * trans1[0] + trans2[3] * trans1[1] + trans2[6] * trans1[2];
	var b = trans2[1] * trans1[0] + trans2[4] * trans1[1] + trans2[7] * trans1[2];
	var c = trans2[2] * trans1[0] + trans2[5] * trans1[1] + trans2[8] * trans1[2];
	var d = trans2[0] * trans1[3] + trans2[3] * trans1[4] + trans2[6] * trans1[5];
	var e = trans2[1] * trans1[3] + trans2[4] * trans1[4] + trans2[7] * trans1[5];
	var f = trans2[2] * trans1[3] + trans2[5] * trans1[4] + trans2[8] * trans1[5];
	var g = trans2[0] * trans1[6] + trans2[3] * trans1[7] + trans2[6] * trans1[8];
	var h = trans2[1] * trans1[6] + trans2[4] * trans1[7] + trans2[7] * trans1[8];
	var i = trans2[2] * trans1[6] + trans2[5] * trans1[7] + trans2[8] * trans1[8];
	return Array(a, b, c, d, e, f, g, h, i);
}

function degrees_to_radians(degrees)
{
  var pi = Math.PI;
  return degrees * (pi/180);
}

