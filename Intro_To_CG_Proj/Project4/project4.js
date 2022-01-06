// This function takes the projection matrix, the translation, and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// The given projection matrix is also a 4x4 matrix stored as an array in column-major order.
// You can use the MatrixMult function defined in project4.html to multiply two 4x4 matrices in the same format.
function GetModelViewProjection( projectionMatrix, translationX, translationY, translationZ, rotationX, rotationY )
{
	var trans = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translationX, translationY, translationZ, 1
	];
 

    // homogeneous rotation coordinates 
    var rotX = [1, 0, 0, 0,
                0, Math.cos(rotationX), Math.sin(rotationX), 0,
                0 , -1*Math.sin(rotationX), Math.cos(rotationX), 0,
                0, 0, 0, 1];

    var rotY = [Math.cos(rotationY), 0, -1*Math.sin(rotationY), 0,
                0, 1, 0, 0,
                Math.sin(rotationY) , 0, Math.cos(rotationY), 0,
                0, 0, 0, 1];


    // apply transformations
    trans = MatrixMult( trans, rotX) ;  

    trans = MatrixMult( trans, rotY );
                 
	var mvp = MatrixMult( projectionMatrix, trans );

	return mvp;
}


class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		// initializations
		this.prog = InitShaderProgram( meshVS, meshFS );

		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );

		this.vertP = gl.getAttribLocation( this.prog, 'vertP' );

		this.sampler = gl.getUniformLocation( this.prog, 'tex' );

		this.txc = gl.getAttribLocation( this.prog, 'txc' );

        this.showTex = gl.getUniformLocation( this.prog, 'showTex' );
       
        this.swapAx = gl.getUniformLocation( this.prog, 'swapAx' );

        this.loadedImg = gl.getUniformLocation( this.prog, 'loadedImg');       

        this.numTriangles = 0;


		// init buffers, one for vertex positon  and another for texture coordinates,  bind and fill later
        gl.useProgram( this.prog );

		this.vertBuffer = gl.createBuffer(); 

        this.texBuffer = gl.createBuffer(); 

        this.myTex = gl.createTexture();
    
        //init uniforms for fragment shader
        gl.uniform1i( this.loadedImg, false);
        gl.uniform1i( this.swapAx, false);
        gl.uniform1i( this.showTex, true);

	}
	
	// This method is called every time the user opens an OBJ file.
	// The arguments of this function is an array of 3D vertex positions
	// and an array of 2D texture coordinates.
	// Every item in these arrays is a floating point value, representing one
	// coordinate of the vertex position or texture coordinate.
	// Every three consecutive elements in the vertPos array forms one vertex
	// position and every three consecutive vertex positions form a triangle.
	// Similarly, every two consecutive elements in the texCoords array
	// form the texture coordinate of a vertex.
	// Note that this method can be called multiple times.
	setMesh( vertPos, texCoords )
	{
		// Update the contents of the vertex buffer objects.

		this.numTriangles = vertPos.length / 3;

        // vertex positions
        gl.bindBuffer( gl.ARRAY_BUFFER, this.vertBuffer );
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);

    
        // texture coordinates
        gl.bindBuffer( gl.ARRAY_BUFFER, this. texBuffer );
        gl.bufferData( gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		// Set the uniform parameter for swap of the vertex shader
        gl.useProgram(this.prog);
        gl.uniform1i( this.swapAx, swap);

	}
	
	// This method is called to draw the triangular mesh.
	// The argument is the transformation matrix, the same matrix returned
	// by the GetModelViewProjection function above.
	draw( trans )
	{
        gl.useProgram(this.prog);// bind prog

        gl.uniformMatrix4fv( this.mvp, false, trans );// set uniform var for all vertices

        // vertex positons attribute
        gl.bindBuffer( gl.ARRAY_BUFFER, this.vertBuffer );
		gl.vertexAttribPointer( this.vertP, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.vertP );

        // texture coordinates attribute
		gl.bindBuffer( gl.ARRAY_BUFFER, this.texBuffer );
		gl.vertexAttribPointer( this.txc, 2, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.txc );
       
        // draw
        gl.clear( gl.COLOR_BUFFER_BIT );
        gl.useProgram(this.prog);
		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
		// Bind the texture
		gl.activeTexture( gl.TEXTURE0 );
		
        gl.bindTexture( gl.TEXTURE_2D, this.myTex );

		// Set the texture image data 
		gl.texImage2D( gl.TEXTURE_2D,         //2d texture
		               0,                     // Mipmap level 0
		               gl.RGB,                // Internal Format   
		               gl.RGB,                // Format
		               gl.UNSIGNED_BYTE,      // type
		               img                    // texture image
		               );

		gl.generateMipmap( gl.TEXTURE_2D );  
		     	
	
		// set some uniform parameter of the fragment shader, so that it uses the texture.
        gl.useProgram( this.prog ); // bind program
        gl.uniform1i( this.sampler, 0 );

        gl.uniform1i( this.loadedImg, true);

	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		// set the uniform parameter for show in the fragment shader to specify if it should use the texture.
		gl.useProgram(this.prog);
		gl.uniform1i(this.showTex, show);
	}
	
}

// Vertex Shader
var meshVS = `
    
    
	attribute vec3 vertP; //position of vertex

    attribute vec2 txc;

    // uniform for swap
    uniform bool swapAx;
	
	uniform mat4 mvp; // 4x4 projection matrix      

	varying vec2 texCoord;   

	void main()
	{
		
		if(swapAx)
		{
             gl_Position = mvp * vec4(vertP.x,vertP.z, vertP.y, 1); // swapped y and z coord
        }
        else // no swap
        {
        	 gl_Position = mvp * vec4(vertP, 1); // vec4(x, y, z, 1) homogenous coordinates
        }
          
        texCoord = txc;
	}
`;

// Fragment Shader
var meshFS = `
	precision mediump float;

	uniform sampler2D tex;

    varying vec2 texCoord;

    // uniform for show texture
    uniform bool showTex;

    uniform bool loadedImg;

	void main()
	{		

	    if(loadedImg)
	    {
	    	if(showTex)
		    {
			    gl_FragColor = texture2D(tex, texCoord);
		    }
		    else
		    {
                gl_FragColor = vec4(1,gl_FragCoord.z*gl_FragCoord.z,0,1);
		    }
	    }
	    else
	    {
	    	gl_FragColor = vec4(1,gl_FragCoord.z*gl_FragCoord.z,0,1);
	    }
	       
	}
`;
