// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	// [TO-DO] Modify the code below to form the transformation matrix.
	var translation = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translationX, translationY, translationZ, 1
	];
	var rotX = [
		1, 0, 0, 0,
		0, Math.cos(rotationX), Math.sin(rotationX), 0,
		0, -Math.sin(rotationX), Math.cos(rotationX), 0,
		0, 0, 0, 1
	];
	var rotY = [
		Math.cos(rotationY), 0, -Math.sin(rotationY), 0,
		0, 1, 0, 0,
		Math.sin(rotationY), 0, Math.cos(rotationY), 0,
		0, 0, 0, 1
	];
	var rotation = MatrixMult( rotX, rotY );
	var mv = MatrixMult( translation, rotation );
	return mv;
}


// [TO-DO] Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		// [TO-DO] initializations

		// Compile the shader program
		this.prog = InitShaderProgram( meshVS, meshFS );
		
		// Get the ids of the uniform variables in the shaders
		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );
		this.mv = gl.getUniformLocation( this.prog, 'mv' );
		this.normMat = gl.getUniformLocation( this.prog, 'normMat' );
		this.tex = gl.getUniformLocation( this.prog, 'tex' );
		this.shininess = gl.getUniformLocation( this.prog, 'shininess' );
		this.lightDir = gl.getUniformLocation( this.prog, 'lightDir' );
		this.swap = gl.getUniformLocation( this.prog, 'swap' );
		this.show = gl.getUniformLocation( this.prog, 'show' );
		this.hasTex = gl.getUniformLocation( this.prog, 'hasTex' );
		
		// Get the ids of the vertex attributes in the shaders
		this.vertPos = gl.getAttribLocation( this.prog, 'pos' );
		this.txc = gl.getAttribLocation( this.prog, 'txc' );
		this.norm = gl.getAttribLocation( this.prog, 'norm' );
		
		// Create the buffer objects
		this.vertbuffer = gl.createBuffer();
		this.texbuffer = gl.createBuffer();
		this.normbuffer = gl.createBuffer();

		// Create texture object
		this.mytex = gl.createTexture();

		// Initialize Show Texture box to true
		gl.useProgram( this.prog );
		gl.uniform1f( this.show, true );
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
		this.numTriangles = vertPos.length / 3;

		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);

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
		gl.useProgram( this.prog );
		gl.uniform1f(this.swap, swap);
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{
		// [TO-DO] Complete the WebGL initializations before drawing
		gl.useProgram( this.prog );
		gl.uniformMatrix4fv( this.mvp, false, matrixMVP );
		gl.uniformMatrix4fv( this.mv, false, matrixMV );
		gl.uniformMatrix3fv( this.normMat, false, matrixNormal );

		gl.bindBuffer( gl.ARRAY_BUFFER, this.vertbuffer );
		gl.vertexAttribPointer( this.vertPos, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.vertPos );

		gl.bindBuffer( gl.ARRAY_BUFFER, this.texbuffer );
		gl.vertexAttribPointer( this.txc, 2, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.txc );

		gl.bindBuffer( gl.ARRAY_BUFFER, this.normbuffer );
		gl.vertexAttribPointer( this.norm, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.norm );

		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
		// [TO-DO] Bind the texture
		gl.useProgram( this.prog );
		gl.bindTexture( gl.TEXTURE_2D, this.mytex );

		// You can set the texture image data using the following command.
		gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img );

		// [TO-DO] Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		gl.generateMipmap( gl.TEXTURE_2D );
		gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR );
		gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR );
		gl.activeTexture( gl.TEXTURE0 );
		gl.uniform1i( this.tex, 0 );
		gl.uniform1f( this.hasTex, true );
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify if it should use the texture.
		gl.useProgram( this.prog );
		gl.uniform1f( this.show, show );
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		var lightDir = [x, y, z];
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the light direction.
		gl.useProgram( this.prog );
		gl.uniform3fv( this.lightDir, lightDir );
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the shininess.
		gl.useProgram( this.prog );
		gl.uniform1f( this.shininess, shininess );
	}
}


// This function is called for every step of the simulation.
// Its job is to advance the simulation for the given time step duration dt.
// It updates the given positions and velocities.
function SimTimeStep( dt, positions, velocities, springs, stiffness, damping, particleMass, gravity, restitution )
{
	var forces = Array( positions.length ); // The total for per particle

	// Initialize the forces array
	for (var i = 0; i < forces.length; ++i) {
		forces[i] = new Vec3( 0,0,0 );
	}

	// [TO-DO] Compute the total force of each particle
	// Add gravity force
	for (var i = 0; i < forces.length; ++i) {
		forces[i].inc(gravity.mul( particleMass ));
	}
	// Add spring forces
	for (var i = 0; i < springs.length; ++i) {
		var particle_i = springs[i].p0;
		var particle_j = springs[i].p1;

		// Compute fS_i
		var len = (positions[particle_j].sub( positions[particle_i] )).len();
		var len_rest = springs[i].rest;
		var d_hat = (positions[particle_j].sub( positions[particle_i] )).div( len );

		var fS_i = (d_hat.mul( len-len_rest )).mul( stiffness );

		// Compute fD_i
		var l_dot = (velocities[particle_j].sub( velocities[particle_i] )).dot( d_hat );

		var fD_i = (d_hat.mul( l_dot )).mul( damping );
		
		// Update forces
		forces[particle_i].inc( fS_i.add( fD_i ));
		forces[particle_j].dec( fS_i.add( fD_i ));
	}
	
	// [TO-DO] Update positions and velocities
	for (var i = 0; i < forces.length; ++i) {
		var a = forces[i].div( particleMass );
		console.log(forces[i]);
		velocities[i].inc(a.mul( dt ));
		positions[i].inc(velocities[i].mul( dt ));
	}
	
	// [TO-DO] Handle collisions
	for (var i = 0; i < positions.length; ++i) {
		if (positions[i].x > 1) {
			var h = positions[i].x - 1;
			positions[i].x = 1 - restitution * h;
			velocities[i].x *= -restitution;
		}
		if (positions[i].x < -1) {
			var h = positions[i].x + 1;
			positions[i].x = (-1) - restitution * h;
			velocities[i].x *= -restitution;
		}
		if (positions[i].y > 1) {
			var h = positions[i].y - 1;
			positions[i].y = 1 - restitution * h;
			velocities[i].y *= -restitution;
		}
		if (positions[i].y < -1) {
			var h = positions[i].y + 1;
			positions[i].y = (-1) - restitution * h;
			velocities[i].y *= -restitution;
		}
		if (positions[i].z > 1) {
			var h = positions[i].z - 1;
			positions[i].z = 1 - restitution * h;
			velocities[i].z *= -restitution;
		}
		if (positions[i].z < -1) {
			var h = positions[i].z + 1;
			positions[i].z = (-1) - restitution * h;
			velocities[i].z *= -restitution;
		}
	}
}

// Vertex shader source code
var meshVS = `
	attribute vec3 pos;
	attribute vec2 txc;
	attribute vec3 norm;
	uniform mat4 mvp;
	uniform bool swap;
	varying vec2 texCoord;
	varying vec3 normal;
	varying vec3 position;
	void main()
	{
		if (swap) {
			gl_Position = mvp * vec4( pos.x, pos.z, pos.y, 1 );
			position = vec3( pos.x, pos.z, pos.y );
			normal = vec3( norm.x, norm.z, norm.y );
		}
		else {
			gl_Position = mvp * vec4(pos, 1);
			position = pos;
			normal = norm;
		}
		texCoord = txc;
	}
`;
// Fragment shader source code
var meshFS = `
	precision mediump float;
	uniform sampler2D tex;
	uniform bool show;
	uniform bool hasTex;
	uniform float shininess;
	uniform vec3 lightDir;
	uniform mat4 mv;
	uniform mat3 normMat;
	varying vec2 texCoord;
	varying vec3 normal;
	varying vec3 position;
	void main()
	{
		vec4 posVec4 = vec4(position,1);
		vec3 positionMV = normalize(mv * -posVec4).xyz;
		vec3 normalMV = normalize(normMat * normal);

		float cosTheta = dot(normalMV, lightDir);
		vec3 halfAng = normalize(lightDir + positionMV);
		float cosPhi = dot(normalMV, halfAng);

		vec4 I = vec4(1,1,1,1);
		vec4 Ks = vec4(1,1,1,1);
		vec4 Kd = vec4(1,1,1,1);

		if (show && hasTex) {
			Kd = texture2D( tex, texCoord );
		}

		gl_FragColor = I * (max(0.0, cosTheta) * Kd + Ks * pow(max(0.0, cosPhi), shininess));
	}
`;
