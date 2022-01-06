// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	//[TO-DO] Modify the code below to form the transformation matrix.
	var rot_x = rotationX;
	var rot_y = rotationY;

	var cosine_x = Math.cos(rot_x);
	var sine_x = Math.sin(rot_x);

	var cosine_y = Math.cos(rot_y);
	var sine_y = Math.sin(rot_y);

	// Rotatin around X axis
	var R_x = [
		1, 0, 0, 0,
		0, cosine_x, -sine_x, 0,
		0, sine_x, cosine_x, 0,
		0, 0, 0, 1
	];

	// Rotating around Y axis
	var R_y = [
		cosine_y, 0, sine_y, 0,
		0, 1, 0, 0,
		-sine_y, 0, cosine_y, 0,
		0, 0, 0, 1
	]


	var Rotation = MatrixMult(R_x, R_y);

	var trans = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translationX, translationY, translationZ, 1
	];

	// Apply rotation to transformation matrix
	var transformation = MatrixMult(trans, Rotation);

	var mvp = MatrixMult( projectionMatrix, transformation);
	return mvp;
}


// [TO-DO] Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		
		// [TO-DO] initializations
		this.prog   = InitShaderProgram( v_shader, f_shader );

		// get attributions
		this.mvp = gl.getUniformLocation(this.prog, 'mvp');
		this.vertPos = gl.getAttribLocation(this.prog, 'vertPos');

		this.MV = this.mvp;
		this.Normal = gl.getAttribLocation(this.prog, 'normals');

		this.txc = gl.getAttribLocation(this.prog, "txc");
		this.sampler = gl.getUniformLocation(this.prog, "tex");
		this.show = gl.getUniformLocation(this.prog, "show");
		this.swap = gl.getUniformLocation(this.prog, "swap");
		this.shininess = gl.getUniformLocation(this.prog, "shininess");


		// Create the buffer
		this.buffer = gl.createBuffer();
		this.texbuffer = gl.createBuffer();
		this.normbuffer = gl.createBuffer();
	}
	
	// This method is called every time the user opens an OBJ file.
	// The arguments of this function is an array of 3D vertex positions,
	// an array of 2D texture coordinates, and an array of vertex normals.
	// Every item in these arrays is a floating point value, representing one
	// coordinate of the vertex position or texture coordinate.
	// Every three consecutive elements in the vertPos array forms one vertex
	// position and every three consecutive vertex positions form a triangle.
	// Similarly, every two consecutive elements in the texCoords array
	// form the texture coordinate of a vertex and every three consecutive 
	// elements in the normals array form a vertex normal.
	// Note that this method can be called multiple times.
	setMesh( vertPos, texCoords, normals )
	{
		// [TO-DO] Update the contents of the vertex buffer objects.
		// Bind the buffer
		gl.useProgram(this.prog);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);
		this.numTriangles = vertPos.length / 3;

		gl.bindBuffer(gl.ARRAY_BUFFER, this.texbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.normbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		// [TO-DO] Set the uniform parameter(s) of the vertex shader
		gl.useProgram(this.prog);
		gl.uniform1i(this.swap, swap);
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( mastrixMVP, matrixMV, Normal )
	{

		//normal transformation?

		// [TO-DO] Complete the WebGL initializations before drawing
		gl.useProgram( this.prog );

		gl.uniformMatrix4fv( this.MV, false, matrixMV ); // set the uniform matrix
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		gl.vertexAttribPointer( this.MV, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.MV );

		gl.uniformMatrix3fv( this.Normal, false, Normal ); // set the uniform matrix
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		gl.vertexAttribPointer( this.Normal, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.Normal);

		gl.uniformMatrix4fv( this.mvp, false, mastrixMVP ); // set the uniform matrix
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		gl.vertexAttribPointer( this.vertPos, 3, gl.FLOAT, false, 0, 0 );

		gl.enableVertexAttribArray( this.vertPos );
		gl.bindBuffer( gl.ARRAY_BUFFER, this.texbuffer );
		gl.vertexAttribPointer( this.txc, 2, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.txc );
		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
		// [TO-DO] Bind the texture
		const myTex = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, myTex);
		// You can set the texture image data using the following command.
		gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img );
		gl.generateMipmap(gl.TEXTURE_2D);
		// [TO-DO] Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);

		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);

		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D, myTex);

		gl.useProgram(this.prog);
		gl.uniform1i(this.sampler, 0);
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify if it should use the texture.
		gl.useProgram(this.prog);
		gl.uniform1i(this.show, show);
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the light direction.
		gl.useProgram(this.prog);
		gl.uniform3f(this.prog, x, y, z);
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the shininess.
		gl.useProgram(this.prog);
		gl.uniform1i(this.shininess, shininess);
	}
}

var v_shader = `
	uniform mat4 mvp;
	attribute vec3 vertPos;
	attribute vec2 txc;
	varying vec2 texCoord;
	uniform bool swap;

	attribute vec3 normals;
	varying vec3 N;


	void main()
	{
		if(swap)
		{
			gl_Position = mvp * vec4(vertPos[0], vertPos[2], vertPos[1], 1);
		}
		else
		{
			gl_Position = mvp * vec4(vertPos, 1);
		}	
		texCoord = txc;	
		N = normals;
	}
`;

// Fragment Shader
var f_shader = `
	precision mediump float;
	uniform sampler2D tex;
	varying vec2 texCoord;
	uniform bool show;

	uniform mat4 mvp;
	varying vec3 N;

	vec4 I;
	vec4 Kd;
	vec4 Ks;
	float alpha;
	vec3 h;
	float cosTheta;
	float cosFi;
	vec3 omega;

	uniform float shininess;


	void main()
	{

		N = mat3 transpose(mat4 inverse(mvp)) * normalize(N)); // Check if correct

		I = vec4(1,1,1,1);
		Kd = vec4(1,1,1,1); // texture 2D
		Ks = vec4(1,1,1,1);
		alpha = shininess;
		omega = vec3(1,1,1);
		cosTheta = dot(omega, N);
		h =  normalize(omega + N);
		cosFi = dot(N, h);
		gl_FragColor = I * (cosTheta * Kd + Ks*pow(cosFi, alpha));
		
		if(show)
		{
			gl_FragColor = texture2D(tex, texCoord);
		}
		else
		{
			gl_FragColor = vec4(1,gl_FragCoord.z*gl_FragCoord.z,0,1);
		}	
	}
`;
