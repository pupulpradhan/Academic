// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
//Modify the code below to form the transformation matrix.
	//Uses formulas given in class
	var trans = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translationX, translationY, translationZ, 1
	];
	var xRotation = [
		1, 0, 0, 0,
		0, Math.cos(rotationX), Math.sin(rotationX), 0,
		0, -Math.sin(rotationX), Math.cos(rotationX), 0,
		0, 0, 0, 1
	];
	var yRotation = [
		Math.cos(rotationY), 0, -Math.sin(rotationY), 0,
		0, 1, 0, 0,
		Math.sin(rotationY), 0, Math.cos(rotationY), 0,
		0, 0, 0, 1
	];
	

	var mvp = MatrixMult(trans, xRotation);
	mvp =  MatrixMult(mvp, yRotation);
	return mvp;
}



class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		// The constructor is a good place for taking care of the necessary initializations.
	
		// Compile the shader program
		this.prog = InitShaderProgram( meshVS, meshFS );
		
		// Get the ids of the uniform variables in the shaders
		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );
		this.mv = gl.getUniformLocation( this.prog, 'mv' );
		this.normalized = gl.getUniformLocation(this.prog, 'normalized');

        //These two variables are used for the methods that determine yz swapping and whether or not the texture is active.
		gl.useProgram(this.prog);
		this.swap = gl.getUniformLocation(this.prog, 'swap');
		gl.uniform1i(this.swap, false);

		this.texOn = gl.getUniformLocation(this.prog, 'texOn')
        gl.uniform1i(this.texOn, true);

		
		// Get the ids of the vertex attributes in the shaders
		this.pos = gl.getAttribLocation( this.prog, 'pos' );
		this.txc = gl.getAttribLocation( this.prog, 'txc');
		this.norm = gl.getAttribLocation(this.prog, 'norm');

		// Create the buffer objects
		
		this.vertbuffer = gl.createBuffer();
		this.meshbuffer = gl.createBuffer();
		this.normalbuffer = gl.createBuffer();
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
		//Updates the contents of the vertex buffer objects to the given buffers.
		this.numTriangles = vertPos.length / 3;

		//Added the normal buffer alongside the other two. 
        
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);
		
		gl.bindBuffer(gl.ARRAY_BUFFER, this.meshbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

        
		gl.bindBuffer(gl.ARRAY_BUFFER, this.normalbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		//Uses the swap uniform variable to set how the object is rendered.
		gl.useProgram(this.prog);
		if (swap){
        gl.uniform1i(this.swap, true);

		}
		else{
        gl.uniform1i(this.swap, false);

		}
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{
		//Mostly copied over from the html file. Uses 2 for meshPos as texture coordinates only have 2 values.
		gl.useProgram(this.prog);

		//Mostly the same as last project. Added the two extra matrices and 
		//added the normal buffer.
		gl.uniformMatrix4fv(this.mvp, false, matrixMVP);
		gl.uniformMatrix4fv(this.mv, false, matrixMV);
		gl.uniformMatrix3fv(this.normalized, false, matrixNormal);

	    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertbuffer);
        gl.vertexAttribPointer(this.pos, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.pos);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.normalbuffer);
        gl.vertexAttribPointer(this.norm, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.norm);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.meshbuffer);
		gl.vertexAttribPointer(this.txc, 2, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.txc);

		// [TO-DO] Complete the WebGL initializations before drawing

		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
	//Most of this comes from lecture videos.
		//Bind the texture

		const mytex = gl.createTexture();
        gl.bindTexture( gl.TEXTURE_2D, mytex);

		// You can set the texture image data using the following command.
		gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img );

		//Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		gl.generateMipmap(gl.TEXTURE_2D);

		gl.texParameteri(
            gl.TEXTURE_2D,
            gl.TEXTURE_MAG_FILTER,
            gl.LINEAR
            );

        gl.texParameteri(
            gl.TEXTURE_2D,
            gl.TEXTURE_MIN_FILTER,
            gl.LINEAR_MIPMAP_LINEAR
            );

        gl.texParameteri(
            gl.TEXTURE_2D,
            gl.TEXTURE_WRAP_S,
            gl.REPEAT
            );

        gl.texParameteri(
            gl.TEXTURE_2D,
            gl.TEXTURE_WRAP_T,
            gl.REPEAT
            );

        gl.activeTexture( gl.TEXTURE0 );
            gl.bindTexture(
            gl.TEXTURE_2D,
            mytex
            );

        var sampler = gl.getUniformLocation( this.prog, 'tex' );
        gl.useProgram( this.prog );
        gl.uniform1i( sampler, 0 );
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		//Essentially the same logic as the yz swap above.
		gl.useProgram(this.prog);
		if (show){
        gl.uniform1i(this.texOn, true);
        }
		else{
        gl.uniform1i(this.texOn, false);

		}
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the light direction.
        this.lightDir = gl.getUniformLocation(this.prog, 'lightDir');
      		gl.useProgram(this.prog);
			 //gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.uniform3fv(this.lightDir, [x,y,z]);

		
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		this.shininess = gl.getUniformLocation(this.prog, 'shininess');
		gl.useProgram(this.prog);
		gl.uniform1f(this.shininess, shininess);


	}
}
// Vertex shader source code

   //Light direction is in view space
   //Surface Normals are defined in model space
   

var meshVS = `
	attribute vec3 pos;
	attribute vec2 txc;
	attribute vec3 norm;
	uniform mat4 mvp;
	uniform mat4 mv;
	uniform mat3 normalized;
	varying vec2 texCoord;
	varying vec3 transNormals;
	varying vec3 vertPos;
	uniform bool swap;
	void main()
	{


		vec4 vertPosHelp;
		if (swap){
                	//Same transformation as last project.
					gl_Position = mvp * vec4(pos.x,pos.z,pos.y,1);

					

					//transforms the position vector into view space
		         vertPosHelp = mv * vec4(pos.x,pos.z,pos.y,1);

		         
		         

		}
		else{
				    //Same transformations as the ones above, but with no yz flip.
					gl_Position = mvp * vec4(pos,1);
					vertPosHelp = mv * vec4(pos,1);
		            
		}

		//Multiply the normal values by the given matrix that translates normals to view space.
		            transNormals = vec3(normalized * norm);
		            
				 vertPos = vec3(vertPosHelp);

        //Same interpolation as last project.
		texCoord = txc;
	}
`;
// Fragment shader source code
//gl_FragColor = vec4((cosTheta*kdhelp) + (vec3(1.0,1.0,1.0) *specular),1.0);
var meshFS = `
	precision mediump float;
	uniform sampler2D tex;
	uniform bool texOn;
	uniform vec3 lightDir;
	uniform float shininess;

	varying vec2 texCoord; 
	varying vec3 transNormals;
	varying vec3 vertPos;
	void main()
	{
		//interpolation should handle the phong shading.

		//The interpolated normals should be normalized as mentioned in lecture 16
        vec3 normalizedNorm = normalize(transNormals);

        //Same thing with the vertex Positions
        vec3 viewVec = normalize(-vertPos);

        //Given in lecture 15, kd should equal the texture at the given point. 
        vec4 kd = texture2D(tex, texCoord);

        vec3 normalizedlightDir = normalize(lightDir);

        //Taken from lecture 15, the half angle is the sum of the light direction and the view vector
        //That sum is then normalized.
        vec3 h = normalize(normalizedlightDir + viewVec);
        
        //Taken from lecture 15 the dot product of the light direction and the surface normal.
        float cosTheta = max(0.0, dot(normalizedNorm, lightDir));
        
        //Also taken from lecture 15, the dot product of the surface normal and the half angle.
        float cosPhi = max(0.0, dot(normalizedNorm, h));

        //specular == cos(phi)^shininess value.
        float specular = pow(cosPhi, shininess);

        //blinn material model another source of confusion for me.

        //kd == texture ks == white I == white.

        //texture vec4, but in the assignment description, white vec3

        //cosPhi^shininess replaced by specular.

        //Ambient lighting optional.

        //This is the actual formula, but my implementation is messed up and only presents the object as white.
       gl_FragColor =  vec4(vec3(1.0,1.0,1.0)*(cosTheta) + ((vec3(1.0,1.0,1.0) + vec3(1.0,1.0,1.0))* specular),1);
	   //vec4((cosTheta*vec3(1,1,1)) + (vec3(1,1,1)*specular),1);
            //gl_FragColor = vec4(1.0,1.0,1.0,1.0)*specular;
       //Test to see if my normals are ok.
          //gl_FragColor = vec4(normalizedNorm,1.0);


	
			
	}
`;