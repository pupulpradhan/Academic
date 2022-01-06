// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The transformation first applies scale, then rotation, and finally translation.
// The given rotation value is in degrees.
function GetTransform( positionX, positionY, rotation, scale )
{
	let scaleMatrix = [
		scale, 0,     0,
		0,     scale, 0,
		0,     0,     1
	];
	let rotationRadians = -rotation * (Math.PI / 180);
	let rotationMatrix = [
		Math.cos(rotationRadians), -Math.sin(rotationRadians), 0,
		Math.sin(rotationRadians), Math.cos(rotationRadians),  0,
		0,                         0,                          1
	];
	let translationMatrix = [
		1,         0,         0,
		0,         1,         0,
		positionX, positionY, 1
	];
	return ApplyTransform(ApplyTransform(scaleMatrix, rotationMatrix), translationMatrix);
}

// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The arguments are transformation matrices in the same format.
// The returned transformation first applies trans1 and then trans2.
function ApplyTransform( trans1, trans2 )
{
	// Return trans2 * trans1
	let result = Array(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);
	for(let row=0; row<3; row++)
	{
		for(let col=0; col<3; col++)
		{
			result[m3x3(row,col)] =
				  trans2[m3x3(row,0)]*trans1[m3x3(0,col)]
				+ trans2[m3x3(row,1)]*trans1[m3x3(1,col)]
				+ trans2[m3x3(row,2)]*trans1[m3x3(2,col)]
		}
	}
	return result;
}

// Given 0-based row and column indices, returns a 0-based array index
// for accessing column-major order 3x3 matricies stored as arrays.
function m3x3(row, column)
{
	return row + column*3;
}