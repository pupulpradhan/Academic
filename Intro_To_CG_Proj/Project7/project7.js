// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	// form transfomation matrix
	var trans = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translationX, translationY, translationZ, 1
	];

	// rotation order: X -> Y
	let cos_theta_x = Math.cos(rotationX);
	let sin_theta_x = Math.sin(rotationX);
	let cos_theta_y = Math.cos(rotationY);
	let sin_theta_y = Math.sin(rotationY);

	var R_x = [
		1, 0, 0, 0,
		0, cos_theta_x, sin_theta_x, 0,
		0, -sin_theta_x, cos_theta_x, 0,
		0, 0, 0, 1
	];

	var R_y = [
		cos_theta_y, 0, - sin_theta_y, 0,
		0, 1, 0, 0,
		sin_theta_y, 0, cos_theta_y, 0,
		0, 0, 0, 1
	];

	// combine rotation
	var R = MatrixMult(R_y, R_x);

	// add rotation to translation
	for (var i = 0; i < 3; ++i) {
		for (var j = 0; j < 3; ++j) {
			trans[4 * i + j] = R[4 * i + j];
		}
	}

	return trans;
}


// [TO-DO] Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		// [TO-DO]initializations
		// program
		this.prog = InitShaderProgram(meshVS, meshFS);
		gl.useProgram(this.prog);

		// attributes
		this.pos = gl.getAttribLocation(this.prog, 'pos');
		this.txc = gl.getAttribLocation(this.prog, 'txc');
		this.normal = gl.getAttribLocation(this.prog, 'normal');

		// uniforms
		this.swap = gl.getUniformLocation(this.prog, 'swap');
		this.show = gl.getUniformLocation(this.prog, 'show');

		this.mvp = gl.getUniformLocation(this.prog, 'mvp');
		this.mv_MV = gl.getUniformLocation(this.prog, 'mv_MV');
		this.mv_Normal = gl.getUniformLocation(this.prog, 'mv_Normal');

		this.tex = gl.getUniformLocation(this.prog, 'tex');

		this.light = gl.getUniformLocation(this.prog, 'light');

		this.alpha = gl.getUniformLocation(this.prog, 'alpha');

		// vertex buffer
		this.vertexBuffer = gl.createBuffer();
		this.txcBuffer = gl.createBuffer();
		this.normalBuffer = gl.createBuffer();

		// texture
		this.texture = gl.createTexture();
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
		// Update the contents of the vertex buffer objects.
		this.numTriangles = vertPos.length / 3;

		gl.useProgram(this.prog);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.txcBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		// Set the uniform parameter(s) of the vertex shader
		gl.useProgram(this.prog);
		gl.uniform1i(this.swap, swap);
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{
		// Complete the WebGL initializations before drawing
		gl.useProgram(this.prog);

		gl.uniformMatrix4fv(this.mvp, false, matrixMVP);

		gl.uniformMatrix4fv(this.mv_MV, false, matrixMV);

		gl.uniformMatrix3fv(this.mv_Normal, false, matrixNormal);
		
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
		gl.vertexAttribPointer(this.pos, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.pos);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.txcBuffer);
		gl.vertexAttribPointer(this.txc, 2, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.txc);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
		gl.vertexAttribPointer(this.normal, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.normal);

		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
		// Bind the texture
		gl.useProgram(this.prog);

		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D, this.texture);

		// You can set the texture image data using the following command.
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img);

		// Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		gl.generateMipmap(gl.TEXTURE_2D);

		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);

		gl.uniform1i(this.tex, 0);
		gl.uniform1i(this.show, 1);
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		// set the uniform parameter(s) of the fragment shader to specify if it should use the texture.
		gl.useProgram(this.prog);

		gl.uniform1i(this.show, show);
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		// set the uniform parameter(s) of the fragment shader to specify the light direction.
		gl.useProgram(this.prog);

		gl.uniform3f(this.light, x, y, z);
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		// set the uniform parameter(s) of the fragment shader to specify the shininess.
		gl.useProgram(this.prog);

		gl.uniform1f(this.alpha, shininess);
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

	// Add gravity force
	for (var i = 0; i < forces.length; ++i) {
		forces[i].inc(gravity.mul( particleMass ));
	}
	// [TO-DO] Compute the total force of each particle
	// calculate force for particals in each mass-spring system
	for (var i = 0; i < springs.length; ++i) {
		// mass-spring system
		var p0 = springs[i].p0; // index of particle p0
		var p1 = springs[i].p1; // index of particle p1
		var lrest = springs[i].rest; // spring's rest length

		// current spring length: sqrt(dx^2 + dy^2 + dz^2) with dv = p0 - p1
		var dv = positions[p0].sub(positions[p1]);
		var l = dv.len();
		// Fs/Fd's direction: p0 = p0<-p1; p1 = p0->p1
		var d_p0 = positions[p1].sub(positions[p0]).div(l);
		var d_p1 = positions[p1].sub(positions[p0]).div(l);
		// spring force: ks*(l - lrest)*d
		var Fs_p0 = d_p0.mul(stiffness * (l - lrest));
		//var Fs_p1 = d_p1.mul(stiffness * (l - lrest));

		// length change speed: (v1 - v0) * d with dv = v1 - v0
		var dv = velocities[p1].sub(velocities[p0]);
		var ls_p0 = d_p0.dot(dv);
		var ls_p1 = d_p1.dot(dv);
		// damping force: k*ls*d
		var Fd_p0 = d_p0.mul(ls_p0).mul(damping);
		//var Fd_p1 = d_p1.mul(ls_p1).mul(damping);

		// calculate and store total force
		forces[p0].inc( Fs_p0.add( Fd_p0 ));
		forces[p1].dec( Fs_p0.add( Fd_p0 ));

		// if (forces[p0] != null){
		// 	forces[p0] = Fg.add(Fs_p0).add(Fd_p0).add(forces[p0]);
		// }
		// else{
		// 	forces[p0] = Fg.add(Fs_p0).add(Fd_p0);
		// }

		// if (forces[p1] != null){
		// 	forces[p1] = Fg.add(Fs_p1).add(Fd_p1).add(forces[p1]);
		// }
		// else{
		// 	forces[p1] = Fg.add(Fs_p1).add(Fd_p1);
		// }
		
	}
	
	// [TO-DO] Update positions and velocities
	for ( var i=0; i < forces.length; ++i ) {
		// total force on each particle
		var F = forces[i];
		if (F == null){
			continue;
		}

		// acceleration a = F/m
		var a = F.div(particleMass);

		// update velocity: v_new = v_old + dt * a
		velocities[i].inc(a.mul(dt));

		// update position: x_new = x_old + dt * v_new
		positions[i].inc(velocities[i].mul(dt));
	}
		// [TO-DO] Handle collisions
		for (var i = 0; i < positions.length; ++i) {
		if (positions[i].y < -1) {
			velocities[i].y *= -restitution;
			positions[i].y = (-1) - Math.abs(positions[i].y + 1) * restitution;
		}

		if (positions[i].y > 1) {
			velocities[i].y *= -restitution;
			positions[i].y = 1 - Math.abs(positions[i].y - 1) * restitution;
		}

		if (positions[i].x < -1) {
			velocities[i].x *= -restitution;
			positions[i].x = (-1) - Math.abs(positions[i].x + 1) * restitution;
		}

		if (positions[i].x > 1) {
			velocities[i].x *= -restitution;
			positions[i].x = 1 -Math.abs(positions[i].x - 1) * restitution;
		}

		if (positions[i].z < -1) {
			velocities[i].z *= -restitution;
			positions[i].z = (-1) - Math.abs(positions[i].z + 1) * restitution;
		}

		if (positions[i].z > 1) {
			velocities[i].z *= -restitution;
			positions[i].z = 1 - Math.abs(positions[i].z - 1) * restitution;
		}
	}
	
}

// Vertex Shader
var meshVS = `
	attribute vec3 pos;
	attribute vec2 txc;
	attribute vec3 normal;
	uniform mat4 mvp;
	uniform bool swap;
	varying vec2 texCoord;
	varying vec3 normals;
	varying vec3 p;
	void main()
	{
		if (swap)
		{
			// swap xyz
			vec3 temp = pos;
			temp[1] = -pos[2];
			temp[2] = pos[1];	
			gl_Position = mvp * vec4(temp, 1);		
		}
		else
		{
			gl_Position = mvp * vec4(pos, 1);
		}
		
		// pass value to FS
		texCoord = txc;
		normals = normal;
		p = pos;
	}
`;

// Fragment Shader
var meshFS = `
	precision mediump float;
	uniform sampler2D tex;
	uniform bool show;
	uniform vec3 light;
	uniform float alpha;
	uniform mat4 mv_MV;
	uniform mat3 mv_Normal;
	varying vec2 texCoord;
	varying vec3 normals;
	varying vec3 p;
	void main()
	{
		// p' = M * p with M = mv_MV
		vec3 v = normalize( ( vec4(p, 1) * mv_MV ).xyz );

		// n' = M * n with M = mv_Normal
		vec3 n = normalize( normals * mv_Normal );

		// w is unit light direction
		vec3 w = normalize(light);

		// cos_theta = n * w should > 0
		float cos_theta = max( 0.0, dot(n, w) );

		// cos_phi = n * h = n * (w + v) should > 0
		float cos_phi = max( 0.0, dot(n, normalize(w + v)) );

		if (show)
		{
			vec4 Kd = texture2D(tex, texCoord);
			vec4 Ks = vec4(1, 1, 1, 1);

			gl_FragColor = cos_theta * Kd + Ks * pow(cos_phi, alpha);
		}
		else
		{
			vec3 Kd = vec3(1, 1, 1);
			vec3 Ks = vec3(1, 1, 1);

			gl_FragColor = vec4( cos_theta * Kd + Ks * pow(cos_phi, alpha), 1);
		}
	}
`;

