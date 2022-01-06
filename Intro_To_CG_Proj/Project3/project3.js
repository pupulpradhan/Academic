// [TO-DO] Complete the implementation of the following class and the vertex shader below.

class CurveDrawer {
	constructor()
	{
		this.prog   = InitShaderProgram( curvesVS, curvesFS );
		// [TO-DO] Other initializations should be done here.


		// [TO-DO] This is a good place to get the locations of attributes and uniform variables.
		this.mvp = gl.getUniformLocation(this.prog, 'mvp'); // get unifrom locattion of "mvp", the transformation matrix
		//this.t = gl.getAttribLocation(this.prog, 't'); // get the attribute "t"? 
			// Get uniform attributes of p0,p1,p2,p3
		this.p0 = gl.getUniformLocation(this.prog, 'p0');
		this.p1 = gl.getUniformLocation(this.prog, 'p1');
		this.p2 = gl.getUniformLocation(this.prog, 'p2');
		this.p3 = gl.getUniformLocation(this.prog, 'p3');

		
		// Initialize the attribute buffer
		this.steps = 100;
		var tv = [];
		for ( var i=0; i<this.steps; ++i ) {
			tv.push( i / (this.steps-1) );
		}
		// [TO-DO] This is where you can create and set the contents of the vertex buffer object
		// for the vertex attribute we need.
			// May be need more understanding for this
		this.buffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tv), gl.STATIC_DRAW);
	}
	setViewport( width, height )
	{
		// [TO-DO] This is where we should set the transformation matrix.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.
			// Taken from the line drawer code
		var trans = [ 2/width,0,0,0,  0,-2/height,0,0, 0,0,1,0, -1,1,0,1 ];
		gl.useProgram( this.prog );	// Bind the program
		gl.uniformMatrix4fv( this.mvp, false, trans );
	}
	updatePoints( pt )
	{
		// [TO-DO] The control points have changed, we must update corresponding uniform variables.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.
		// [TO-DO] We can access the x and y coordinates of the i^th control points using
		// var x = pt[i].getAttribute("cx");
		// var y = pt[i].getAttribute("cy");
		// gl.useProgram(this.prog); // is it really needed?
		// var p = [];
		// for ( var i=0; i<8; i+2 ) {
		// 	var p[i] = pt[i].getAttribute("cx");
		// 	var p[i+1] = pt[i].getAttribute("cy");
		// 	p.push(x);
		// 	p.push(y);
		// }
		gl.useProgram(this.prog);
		var x = pt[0].getAttribute("cx");
		var y = pt[0].getAttribute("cy");
		var x1 = pt[1].getAttribute("cx");
		var y1 = pt[1].getAttribute("cy");
		var x2 = pt[2].getAttribute("cx");
		var y2 = pt[2].getAttribute("cy");
		var x3 = pt[3].getAttribute("cx");
		var y3 = pt[3].getAttribute("cy");
		gl.uniform2f(this.p0, x,y);
		gl.uniform2f(this.p1,x1,y1);
		gl.uniform2f(this.p2, x2,y2);
		gl.uniform2f(this.p3, x3,y3);
		
	
	}
	draw()
	{
		// [TO-DO] This is where we give the command to draw the curve.
		// [TO-DO] Do not forget to bind the program and set the vertex attribute.s
		gl.useProgram( this.prog );
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		gl.vertexAttribPointer( this.buffer, 1, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.buffer );
		gl.drawArrays( gl.LINE_STRIP, 0, this.steps );
	}
}

// Vertex Shaders
var curvesVS = `
	attribute float t;
	uniform mat4 mvp;
	uniform vec2 p0;
	uniform vec2 p1;
	uniform vec2 p2;
	uniform vec2 p3;

	// do we need to forma a new matrix? based on p0, p1, p2, p3? May be not?

	void main()
	{
		// [TO-DO] Replace the following with the proper vertex shader code
		//gl_Position = vec4(0,0,0,1);
		vec2 pos = vec2(pow(1.0-t,3.0)*p0 + 3.0*pow(1.0-t,2.0)*t*p1 + 3.0*(1.0-t)*pow(t,2.0)*p2 + pow(t,3.0)*p3);
		// might have to use the Bezier curve equation
		gl_Position = mvp * vec4(pos, 0, 1);
		
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