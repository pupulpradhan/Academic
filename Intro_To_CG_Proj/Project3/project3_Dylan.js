// [TO-DO] Complete the implementation of the following class and the vertex shader below.

class CurveDrawer {
	constructor()
	{
		console.log("this runs");
		//compile shaders and attatch to the program
		this.prog = InitShaderProgram( curvesVS, curvesFS );
		// [TO-DO] Other initializations should be done here.

		// [TO-DO] This is a good place to get the locations of attributes and uniform variables.
		//from vertex shader 
		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );
		this.verts = [gl.getUniformLocation( this.prog, 'p0'), gl.getUniformLocation( this.prog, 'p1'),
					  gl.getUniformLocation( this.prog, 'p2'), gl.getUniformLocation( this.prog, 'p3')];
		//this.vertPos = gl.getAttribLocation( this.prog, 'pos');

		// Initialize the attribute buffer
		this.steps = 100;
		 var tv = [];
		 for ( var i=0; i<this.steps; ++i ) {
		 	tv.push( i / (this.steps-1) );
		}

		// [TO-DO] This is where you can create and set the contents of the vertex buffer object
		// for the vertex attribute we need.
		this.t_buffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this.t_buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tv), gl.STATIC_DRAW);
	}
	
	setViewport( width, height )
	{
		// [TO-DO] This is where we should set the transformation matrix.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.
		// Compute the orthographic projection matrix and send it to the shader
		var trans = [ 2/width,0,0,0,  0,-2/height,0,0, 0,0,1,0, -1,1,0,1 ];
		gl.useProgram( this.prog );	// Bind the program
		gl.uniformMatrix4fv( this.mvp, false, trans );
	}
	updatePoints( pt )
	{
		// [TO-DO] The control points have changed, we must update corresponding uniform variables.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.
		gl.useProgram( this.prog );	// Bind the program
		// [TO-DO] We can access the x and y coordinates of the i^th control points using
		// var x = pt[i].getAttribute("cx");
		// var y = pt[i].getAttribute("cy");
		var v = [];
		//console.log(verts.length);
		for ( var i=0; i<4; i++ ) {
			var x = pt[i].getAttribute("cx");
			var y = pt[i].getAttribute("cy");
			//gl.uniform2f(this.verts[i], x, y);
			v.push(x);
			v.push(y);
		}
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(v), gl.STATIC_DRAW);
	}
	draw()
	{
		// [TO-DO] This is where we give the command to draw the curve.
		// [TO-DO] Do not forget to bind the program and set the vertex attribute.
		gl.useProgram( this.prog );
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		for(let i = 0; i < 4; i++)
		{
			gl.vertexAttribPointer( this.verts[i], 2, gl.FLOAT, false, 0, 0 );
			gl.enableVertexAttribArray( this.verts[i]);
		}
		gl.drawArrays( gl.LINE_STRIP, 0, 4 );

		var t = gl.getAttribLocation(prog, 't');
		gl.bindBuffer(gl.ARRAY_BUFFER, this.t_buffer);
		gl.vertexAttribPointer(t, 1, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(t);
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

	//remove this later
	attribute vec2 pos;
	
	void main()
	{
		// [TO-DO] Replace the following with the proper vertex shader code
		//gl_Position = vec4(0,0,0,1);
		//P = (1−t)3P1 + 3(1−t)2tP2 +3(1−t)t2P3 + t3P4
		//pos = vec2(pow(1.0-t, 3.0) * p0 +  3.0 * pow(1-t, 2.0) * t * p1 + 3.0 * (1.0-t) * t * t * p2 + t * t * t + p3);

		gl_Position = mvp * vec4(((1.0 - t) * p0) + (t * p3), 0, 1);

		//gl_Position = mvp * vec4(t, 0, 0, 1);
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