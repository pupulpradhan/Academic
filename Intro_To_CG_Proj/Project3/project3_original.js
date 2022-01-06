// [TO-DO] Complete the implementation of the following class and the vertex shader below.

class CurveDrawer {
	constructor()
	{
		this.prog   = InitShaderProgram( curvesVS, curvesFS );
		// [TO-DO] Other initializations should be done here.
		// [TO-DO] This is a good place to get the locations of attributes and uniform variables.
		
		// Get the ids of the uniform variables in the shaders
		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );
		this.p0 = gl.getUniformLocation( this.prog, 'p0' );
		this.p1 = gl.getUniformLocation( this.prog, 'p1' );
		this.p2 = gl.getUniformLocation( this.prog, 'p2' );
		this.p3 = gl.getUniformLocation( this.prog, 'p3' );
		

		// Initialize the attribute buffer
		this.steps = 100;
		var tv = [];
		for ( var i=0; i<this.steps; ++i ) {
			tv.push( i / (this.steps-1) );
		}
		// [TO-DO] This is where you can create and set the contents of the vertex buffer object
		// for the vertex attribute we need.
		// Create the vertex buffer object
		this.buffer = gl.createBuffer();

		// Bind buffer and set buffer contents
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tv), gl.STATIC_DRAW);
	}
	setViewport( width, height )
	{
		// [TO-DO] This is where we should set the transformation matrix.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.
		// Compute the orthographic projection matrix and send it to the shader
		var trans = [ 2/width,0,0,0,  0,-2/height,0,0, 0,0,1,0, -1,1,0,1 ];
		gl.useProgram( this.prog ); // Bind the program
		gl.uniformMatrix4fv( this.mvp, false, trans );
	}
	updatePoints( pt )
	{
		// [TO-DO] The control points have changed, we must update corresponding uniform variables.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.
		// [TO-DO] We can access the x and y coordinates of the i^th control points using
		// var x = pt[i].getAttribute("cx");
		// var y = pt[i].getAttribute("cy");

		// Pull the new values
		var p0x = pt[0].getAttribute("cx");
		var p0y = pt[0].getAttribute("cy");

		var p1x = pt[1].getAttribute("cx");
		var p1y = pt[1].getAttribute("cy");

		var p2x = pt[2].getAttribute("cx");
		var p2y = pt[2].getAttribute("cy");

		var p3x = pt[3].getAttribute("cx");
		var p3y = pt[3].getAttribute("cy");

		// Bind the program
		gl.useProgram( this.prog );

		// Assign new uniform variable values
		gl.uniform2f( this.p0, p0x, p0y );
		gl.uniform2f( this.p1, p1x, p1y );
		gl.uniform2f( this.p2, p2x, p2y );
		gl.uniform2f( this.p3, p3x, p3y );
	}
	draw()
	{
		// [TO-DO] This is where we give the command to draw the curve.
		// [TO-DO] Do not forget to bind the program and set the vertex attribute.
		gl.useProgram( this.prog );
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		gl.vertexAttribPointer( this.buffer, 1, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.buffer );
		gl.drawArrays( gl.LINE_STRIP, 0, this.steps );
	}
}

// Vertex Shader
var curvesVS = `
	attribute float t;
	uniform mat4 mvp;
	uniform vec2 p0;
	uniform vec2 p1;
	uniform vec2 p2;
	uniform vec2 p3;
	void main()
	{
		// [TO-DO] Replace the following with the proper vertex shader code
		vec2 location = vec2(pow((1.0 - t), 3.0) * p0 + 3.0 * pow((1.0 - t), 2.0) * t * p1 + 3.0 * (1.0 - t) * pow(t, 2.0) * p2 + pow(t, 3.0) * p3);
		gl_Position = mvp * vec4(location,0,1);
	}
`;

// Fragment Shader
var curvesFS = `
	precision mediump float;
	void main()
	{
		gl_FragColor = vec4(1,0,0,1);
	}
`;