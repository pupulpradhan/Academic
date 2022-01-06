// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	//  Modify the code below to form the transformation matrix.

	// definately these in order(T * R * S)
//i think apply projection last (so mult it first) 

	var translationMatrix = [
		1, 0, 0, 0, //col 1
		0, 1, 0, 0, // col 2
		0, 0, 1, 0, //col 3
		translationX, translationY, translationZ, 1 //col 4
	];
	var yRotationMatrix = [
		Math.cos(rotationY), 0, -Math.sin(rotationY), 0,
		0, 1, 0, 0,
		Math.sin(rotationY), 0, Math.cos(rotationY), 0,
		0, 0, 0, 1
	];
	var xRotationMatrix = [
		1, 0, 0, 0,
		0, Math.cos(rotationX), Math.sin(rotationX), 0,
		0, -Math.sin(rotationX), Math.cos(rotationX), 0,
		0, 0, 0, 1
	];

	//without projection we go straight from translation to rotation 
    var mv = translationMatrix;
    // = MatrixMult( projectionMatrix, translationMatrix);
	mv = MatrixMult(mv, yRotationMatrix);
	mv = MatrixMult(mv, xRotationMatrix);
	return mv;
}


// Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		// [TO-DO] initializations
		this.textureSet = false;
		// Compile the shader program
		this.prog = InitShaderProgram( meshVS, meshFS );
		
		// Get the ids of the uniform variables in the shaders
		this.trans = gl.getUniformLocation( this.prog, 'trans' );

		this.mv = gl.getUniformLocation( this.prog, 'mv' );

		this.normalMatrix = gl.getUniformLocation( this.prog, 'normalMatrix' );

		this.swap = gl.getUniformLocation( this.prog, 'swap' );

		this.show = gl.getUniformLocation( this.prog, 'show' );

		this.showVal = true;

		this.tex = gl.getUniformLocation( this.prog, 'tex' );

		this.lightDirection = gl.getUniformLocation( this.prog, 'lightDirection' );

		this.alpha = gl.getUniformLocation( this.prog, 'alpha' );
		
		// Get the ids of the vertex attributes in the shaders
		this.pos = gl.getAttribLocation( this.prog, 'pos' );

		this.txc = gl.getAttribLocation( this.prog, 'txc' );

		this.norm = gl.getAttribLocation( this.prog, 'norm' );

        //make the buffer, but don't fill it because we need the mesh info to fill the buffer
		this.buffer = gl.createBuffer();

        this.texBuffer = gl.createBuffer();

        this.normBuffer = gl.createBuffer();

	
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

		gl.useProgram( this.prog );
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);

		//watch textures on the GPU
		gl.useProgram( this.prog );
		gl.bindBuffer(gl.ARRAY_BUFFER, this.texBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

        //buffer tne normals
		gl.useProgram( this.prog );
		gl.bindBuffer(gl.ARRAY_BUFFER, this.normBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		// [TO-DO] Set the uniform parameter(s) of the vertex shader
		gl.useProgram( this.prog );
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.uniform1i(this.swap, swap);
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{
		
		//  Complete the WebGL initializations before drawing

		gl.useProgram( this.prog );
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);

        //might not be right
		gl.uniformMatrix4fv(this.trans, false, matrixMVP);

		gl.vertexAttribPointer( this.pos, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.pos );
        
        gl.bindBuffer(gl.ARRAY_BUFFER, this.texBuffer);
        gl.vertexAttribPointer( this.txc, 2, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.txc );


        //shading transformations
		gl.bindBuffer(gl.ARRAY_BUFFER, this.normBuffer);
		gl.vertexAttribPointer( this.norm, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.norm );

        //make matrixMV a uniform and pass it along
        gl.uniformMatrix4fv(this.mv, false, matrixMV);

        //make matrixNormal a uniform and pass it along, reminder its a mat3
        gl.uniformMatrix3fv(this.normalMatrix, false, matrixNormal);



		//gl.enableVertexAttribArray( this.txc );
        //go into lecture triangular meshes find how to draw triangles
		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
			//a texture has been set
			if(!this.textureSet)
		{
			this.textureSet = true;
            gl.useProgram( this.prog );
            gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
            gl.uniform1i(this.show, this.showVal);
		}
		//  Bind the texture
        		//texture stuff
		const mytex = gl.createTexture();

		gl.bindTexture(gl.TEXTURE_2D, mytex);
		gl.texImage2D(
		gl.TEXTURE_2D, //2D texture
		0, //mipmap level 0
		gl.RGB, //internal format
		gl.RGB, //format
		gl.UNSIGNED_BYTE, //type
		img //array or <img>
		);

		gl.generateMipmap(gl.TEXTURE_2D);

		// You can set the texture image data using the following command.
		//gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img );

		//Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.

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

		//using the texture
		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(
		gl.TEXTURE_2D,
		mytex
		);
        
		gl.useProgram(this.prog);
		gl.uniform1i(this.tex, 0);
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		this.showVal = show;
		if(this.textureSet)
		{
		gl.useProgram( this.prog );
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.uniform1i(this.show, show);
		}
		//texture not set yet, don't show it regardless
		else
		{
		gl.useProgram( this.prog );
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.uniform1i(this.show, false);
		}
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the light direction.
        gl.useProgram(this.prog);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.uniform3fv(this.lightDirection, [x,y,z]);
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the shininess.
		gl.useProgram(this.prog);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.uniform1f(this.alpha, shininess);
	}
}

// Vertex shader source code
var meshVS = `
	attribute vec3 pos;
	uniform mat4 trans;
	uniform bool swap;
	varying vec2 texCoord;
	varying vec3 normVal;
	varying vec3 pPrime;
	attribute vec2 txc;
	attribute vec3 norm;
	vec4 position;
	uniform mat4 mv;
	void main()
	{
		//decide to swap y-z or not and find the position
		if(swap)
		{position = trans * vec4(pos.x, pos.z, pos.y, 1);}
		else
		{position = trans * vec4(pos,1);}

        //vertexShader values
		texCoord = txc;
        normVal = norm;
        pPrime = vec3(mv * vec4(pos,1));
        //pPrime = position;

        //set the position
        gl_Position = position;
	}
`;
// Fragment shader source code
var meshFS = `
	precision mediump float;
	uniform sampler2D tex;
	uniform bool show;
    varying vec2 texCoord;
    //shading variables
    uniform vec3 lightDirection;
	varying vec3 pPrime;
	uniform mat3 normalMatrix;
    varying vec3 normVal;
    uniform float alpha;
    vec3 I;
    vec3 ks;
    vec3 normal;
    vec3 kd;
    //composite values
    vec3 viewDirection;
    float cosTheta;
    float cosPhi;
    vec3 h;
    vec3 lightDir;

    //start of class
	void main()
	{
		//decide on texture
		if(show)
		{kd = vec3(texture2D(tex,texCoord));}
		else
		{kd = vec3(1,1,1);
		//kd = vec4(1,gl_FragCoord.z*gl_FragCoord.z,0,1);
		} 

		//temp
        //gl_FragColor =  kd;

        //shading
        I = vec3(1,1,1);

        ks = vec3(1,1,1);

        normal = normalize(normalMatrix * normVal);
        
        cosTheta = max(dot(normalize(lightDirection), normal),0.0);

        viewDirection = normalize(-pPrime);

        h = normalize(normalize(lightDirection) + viewDirection);

        cosPhi = max(dot(normal,h),0.0);
        cosPhi = pow(cosPhi,alpha);
        //something wrong with alpha
        

        //something wrong texture not loading

        gl_FragColor = vec4((cosTheta*kd) + (ks*cosPhi),1);
        //gl_FragColor = (vec4(kd,1) + vec4((ks*cosPhi),1));
        //gl_FragColor = vec4(I,1) * ((kd) + vec4((ks),1));



        //dot(vector1,vector2) performs dot product on two vectors
        // inverse(matrix) inverts a matrix
        //normalize(vecrtor) normalizes a matrix



		//TODO blinn shading
		//surface normal = normalMatrix * normVal
		//lightDirection is given
		//pPrime = mv * position
		//viewDirection is negative pPrime normalized
		//kd is material color = white (1,1,1) or texture value
		//ks = white (1,1,1)
		//I is light intensity, is (1,1,1) white light
		//cos(theta) is lightDirection dot normal
		//cos phi is normal dot h
		//h = (lightDirection + viewDirection)/(abs(lightDirection + viewDirection))
		//alpha is shininess from set shininess
		// Color = I(cos(theta)*kd + ks*cos(phi)^alpha)
	}
`;